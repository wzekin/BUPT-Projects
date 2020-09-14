//! 程序的algorithm模块， 用来书写核心算法
//! [`Resolution`]: struct.Resolution.html
use crate::city::*;
use crate::err::ModuleErr;
use crate::way::*;
use getset::Getters;
use itertools::izip;
use std::cmp::Ordering;
use std::cmp::Reverse;
use std::collections::binary_heap::BinaryHeap;
use std::collections::HashMap;
use std::collections::HashSet;
use std::rc::Rc;

/// Resulotion 算法的返回值
#[derive(Debug, Clone, Getters)]
pub struct Resolution {
    /// 算法计算的总风险
    #[getset(get = "pub")]
    total_risk: f64,

    /// 算法计算的最优道路
    #[getset(get = "pub")]
    road: Vec<Rc<Way>>,

    /// 算法计算的到达时间
    #[getset(get = "pub")]
    end_time: i32,

    end: Rc<City>,
}

impl Ord for Resolution {
    fn cmp(&self, other: &Self) -> Ordering {
        self.total_risk.partial_cmp(&other.total_risk).unwrap()
    }
}

impl PartialOrd for Resolution {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        let f1 = self.total_risk + self.road.len() as f64;
        let f2 = other.total_risk + other.road.len() as f64;
        if f1 == f2 {
            self.end_time.partial_cmp(&other.end_time)
        } else {
            f1.partial_cmp(&f2)
        }
    }
}

impl PartialEq for Resolution {
    fn eq(&self, other: &Self) -> bool {
        if self.road.len() != other.road.len() {
            return false;
        }
        for (a, b) in izip!(&self.road, &other.road) {
            if a != b {
                return false;
            }
        }
        self.total_risk == other.total_risk && self.end_time == other.end_time
    }
}

impl Eq for Resolution {}

/// 主算法，使用A算法，找出一条可行解（可能不是最优）
///
/// # Arguments
/// * `start_city` - 旅客的起始城市
/// * `end_city` - 旅客的终点城市
/// * `start_time` - 旅客的出发时间
/// * `time_limit` - 旅客是否有时间限制
///
/// # Details
/// 1. 先判断可达性，如果不可达，则无解
///
/// 2. 初始化open_set和closed_set
///
/// 3. 将起点加入open_set中
///
/// 4. 如果open_set不为空，则从open_set中选取优先级最高路径
///
///     * 如果路径终点为旅客的终点，则返回此路径，算法结束
///
///     * 如果路径超过了时间限制（如果有），则跳过
///
///     * 如果路径在closed_set中，则跳过
///
///     * 如果都不是
///
///         1. 将此路径加入到closed_set中
///
///         2. 遍历路径终点的所有可达城市，并将其加入到open_set中
///         
/// 5. 如果open_set为空，则为无解
///
/// # Returns
/// 一个Result 如果有可行解，返回[`Resolution`][Resolution]
pub fn calculate(
    start_city: Rc<City>,
    end_city: Rc<City>,
    start_time: i32,
    time_limit: Option<i32>,
) -> Result<Resolution, ModuleErr> {
    if !dfs(start_city.clone(), end_city.clone()) {
        return Err(ModuleErr::WaysNotFound);
    }

    let mut closed_set: HashMap<(i32, i32), i32> = HashMap::new();
    let mut open_set = BinaryHeap::new();
    open_set.push(Reverse(Resolution {
        total_risk: 0.0,
        road: Vec::new(),
        end: start_city.clone(),
        end_time: start_time,
    }));
    while !open_set.is_empty() {
        let r = open_set.pop().unwrap().0;

        if time_limit.is_some() && time_limit.unwrap() < r.end_time - start_time {
            continue;
        }

        if r.end == end_city {
            return Ok(r);
        }

        if let Some(end_time) = closed_set.get(&(r.end.id().clone(), r.end_time % 24)) {
            if &r.end_time >= end_time {
                continue;
            }
        } else {
            closed_set.insert((r.end.id().clone(), r.end_time % 24), r.end_time);
        }

        for way in r.end.start_ways() {
            let mut now = r.clone();
            let waiting_time = calculate_time(&(r.end_time % 24), way.start_time());
            now.road.push(way.clone());
            now.end_time += waiting_time + way.cost_time();
            now.total_risk += waiting_time as f64 * r.end.risk()
                + way.risk() * r.end.risk() * way.cost_time().clone() as f64;
            now.end = way.end_city().clone();

            if !now.total_risk.is_nan() {
                open_set.push(Reverse(now.clone()));
            }
        }
    }
    Err(ModuleErr::WaysNotFound)
}

///计算两个时间值之间相隔的时间
#[inline(always)]
pub fn calculate_time(from: &i32, to: &i32) -> i32 {
    if from > to {
        to - from + 24
    } else {
        to - from
    }
}

/// 判断两点是否可达
fn dfs(from: Rc<City>, to: Rc<City>) -> bool {
    let mut set: HashSet<i32> = HashSet::new();
    let mut vec: Vec<Rc<City>> = Vec::new();

    set.insert(from.id().clone());
    vec.push(from);

    while !vec.is_empty() {
        let city = vec.pop().unwrap();
        for way in city.start_ways() {
            if way.end_city().id() == to.id() {
                return true;
            }
            if set.get(way.end_city().id()).is_none() {
                set.insert(way.end_city().id().clone());
                vec.push(way.end_city().clone());
            }
        }
    }
    return false;
}

#[cfg(test)]
mod tests {
    use super::*;
    use float_cmp::approx_eq;
    #[test]
    fn calculate_time_from_up_to() {
        assert_eq!(calculate_time(&20, &10), 14);
    }

    #[test]
    fn calculate_time_from_down_to() {
        assert_eq!(calculate_time(&10, &20), 10);
    }

    #[test]
    fn calculate_road() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let mut city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let mut city_2 = Rc::new(City::new(2, String::new(), RiskLevel::Low));
        let way_0 = Rc::new(Way::new(0, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        let way_1 = Rc::new(Way::new(
            1,
            Type::Car,
            city_1.clone(),
            city_2.clone(),
            8,
            10,
        ));
        let way_2 = Rc::new(Way::new(
            2,
            Type::Train,
            city_2.clone(),
            city_1.clone(),
            5,
            9,
        ));
        let way_3 = Rc::new(Way::new(
            3,
            Type::Train,
            city_2.clone(),
            city_0.clone(),
            4,
            18,
        ));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_0.clone());
            Rc::get_mut_unchecked(&mut city_1).add_way(way_1.clone());
            Rc::get_mut_unchecked(&mut city_2).add_way(way_2.clone());
            Rc::get_mut_unchecked(&mut city_2).add_way(way_3.clone());
        }
        let calculation = calculate(city_0.clone(), city_1.clone(), 3, None);
        assert!(calculation.is_ok());
        let calculation = calculation.ok().unwrap();
        assert_eq!(calculation.road().len(), 1);
        assert_eq!(calculation.end_time(), &7);
        assert!(approx_eq!(
            f64,
            calculation.total_risk().clone(),
            5.6,
            ulps = 2
        ));
    }

    #[test]
    fn calculate_road_1() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let mut city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let mut city_2 = Rc::new(City::new(2, String::new(), RiskLevel::Low));
        let mut city_3 = Rc::new(City::new(3, String::new(), RiskLevel::Medium));
        let mut city_4 = Rc::new(City::new(4, String::new(), RiskLevel::High));
        let way_0 = Rc::new(Way::new(0, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        let way_1 = Rc::new(Way::new(
            1,
            Type::Car,
            city_1.clone(),
            city_2.clone(),
            8,
            10,
        ));
        let way_2 = Rc::new(Way::new(
            2,
            Type::Train,
            city_2.clone(),
            city_1.clone(),
            5,
            9,
        ));
        let way_3 = Rc::new(Way::new(
            3,
            Type::Train,
            city_2.clone(),
            city_4.clone(),
            4,
            18,
        ));
        let way_4 = Rc::new(Way::new(
            4,
            Type::Air,
            city_3.clone(),
            city_2.clone(),
            4,
            20,
        ));
        let way_5 = Rc::new(Way::new(
            5,
            Type::Car,
            city_4.clone(),
            city_2.clone(),
            6,
            16,
        ));
        let way_6 = Rc::new(Way::new(
            6,
            Type::Car,
            city_4.clone(),
            city_1.clone(),
            10,
            15,
        ));
        let way_7 = Rc::new(Way::new(
            7,
            Type::Train,
            city_1.clone(),
            city_3.clone(),
            4,
            8,
        ));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_0.clone());
            Rc::get_mut_unchecked(&mut city_1).add_way(way_1.clone());
            Rc::get_mut_unchecked(&mut city_2).add_way(way_2.clone());
            Rc::get_mut_unchecked(&mut city_2).add_way(way_3.clone());
            Rc::get_mut_unchecked(&mut city_3).add_way(way_4.clone());
            Rc::get_mut_unchecked(&mut city_4).add_way(way_5.clone());
            Rc::get_mut_unchecked(&mut city_4).add_way(way_6.clone());
            Rc::get_mut_unchecked(&mut city_1).add_way(way_7.clone());
        }
        let calculation = calculate(city_1.clone(), city_4.clone(), 20, None);
        assert!(calculation.is_ok());
        let calculation = calculation.ok().unwrap();
        assert!(
            approx_eq!(f64, calculation.total_risk().clone(), 10.0, ulps = 2),
            "left = {}, right = {}",
            calculation.total_risk().clone(),
            5.6
        );
        assert_eq!(calculation.road().len(), 2);
        assert_eq!(calculation.end_time(), &46);
    }

    #[test]
    fn calculate_road_1_with_time_limit() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let mut city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let mut city_2 = Rc::new(City::new(2, String::new(), RiskLevel::Low));
        let mut city_3 = Rc::new(City::new(3, String::new(), RiskLevel::Medium));
        let mut city_4 = Rc::new(City::new(4, String::new(), RiskLevel::High));
        let way_0 = Rc::new(Way::new(0, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        let way_1 = Rc::new(Way::new(
            1,
            Type::Car,
            city_1.clone(),
            city_2.clone(),
            8,
            10,
        ));
        let way_2 = Rc::new(Way::new(
            2,
            Type::Train,
            city_2.clone(),
            city_1.clone(),
            5,
            9,
        ));
        let way_3 = Rc::new(Way::new(
            3,
            Type::Train,
            city_2.clone(),
            city_4.clone(),
            4,
            18,
        ));
        let way_4 = Rc::new(Way::new(
            4,
            Type::Air,
            city_3.clone(),
            city_2.clone(),
            4,
            20,
        ));
        let way_5 = Rc::new(Way::new(
            5,
            Type::Air,
            city_4.clone(),
            city_1.clone(),
            6,
            16,
        ));
        let way_6 = Rc::new(Way::new(
            6,
            Type::Car,
            city_4.clone(),
            city_1.clone(),
            10,
            15,
        ));
        let way_7 = Rc::new(Way::new(
            7,
            Type::Train,
            city_1.clone(),
            city_3.clone(),
            4,
            8,
        ));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_0.clone());
            Rc::get_mut_unchecked(&mut city_1).add_way(way_1.clone());
            Rc::get_mut_unchecked(&mut city_2).add_way(way_2.clone());
            Rc::get_mut_unchecked(&mut city_2).add_way(way_3.clone());
            Rc::get_mut_unchecked(&mut city_3).add_way(way_4.clone());
            Rc::get_mut_unchecked(&mut city_4).add_way(way_5.clone());
            Rc::get_mut_unchecked(&mut city_4).add_way(way_6.clone());
            Rc::get_mut_unchecked(&mut city_1).add_way(way_7.clone());
        }
        let calculation = calculate(city_4.clone(), city_1.clone(), 15, Some(9));
        assert!(calculation.is_ok());
        let calculation = calculation.ok().unwrap();
        assert_eq!(calculation.road().len(), 1);
        assert_eq!(calculation.end_time(), &22);
        assert!(
            approx_eq!(f64, calculation.total_risk().clone(), 49.5, ulps = 2),
            "left = {}, right = {}",
            calculation.total_risk().clone(),
            5.6
        );
    }
    #[test]
    fn calculate_road_1_no_way_because_of_time_out() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let mut city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let mut city_2 = Rc::new(City::new(2, String::new(), RiskLevel::Low));
        let mut city_3 = Rc::new(City::new(3, String::new(), RiskLevel::Medium));
        let mut city_4 = Rc::new(City::new(4, String::new(), RiskLevel::High));
        let way_0 = Rc::new(Way::new(0, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        let way_1 = Rc::new(Way::new(
            1,
            Type::Car,
            city_1.clone(),
            city_2.clone(),
            8,
            10,
        ));
        let way_2 = Rc::new(Way::new(
            2,
            Type::Train,
            city_2.clone(),
            city_1.clone(),
            5,
            9,
        ));
        let way_3 = Rc::new(Way::new(
            3,
            Type::Train,
            city_2.clone(),
            city_4.clone(),
            4,
            18,
        ));
        let way_4 = Rc::new(Way::new(
            4,
            Type::Air,
            city_3.clone(),
            city_2.clone(),
            4,
            20,
        ));
        let way_5 = Rc::new(Way::new(
            5,
            Type::Air,
            city_4.clone(),
            city_1.clone(),
            6,
            16,
        ));
        let way_6 = Rc::new(Way::new(
            6,
            Type::Car,
            city_4.clone(),
            city_1.clone(),
            10,
            15,
        ));
        let way_7 = Rc::new(Way::new(
            7,
            Type::Train,
            city_1.clone(),
            city_3.clone(),
            4,
            8,
        ));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_0.clone());
            Rc::get_mut_unchecked(&mut city_1).add_way(way_1.clone());
            Rc::get_mut_unchecked(&mut city_2).add_way(way_2.clone());
            Rc::get_mut_unchecked(&mut city_2).add_way(way_3.clone());
            Rc::get_mut_unchecked(&mut city_3).add_way(way_4.clone());
            Rc::get_mut_unchecked(&mut city_4).add_way(way_5.clone());
            Rc::get_mut_unchecked(&mut city_4).add_way(way_6.clone());
            Rc::get_mut_unchecked(&mut city_1).add_way(way_7.clone());
        }
        let calculation = calculate(city_4.clone(), city_1.clone(), 15, Some(6));
        assert!(calculation.is_err());
    }
    #[test]
    fn calculate_road_2_no_way() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let city_2 = Rc::new(City::new(2, String::new(), RiskLevel::Low));
        let way_0 = Rc::new(Way::new(0, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_0.clone());
        }
        let calculation = calculate(city_2.clone(), city_1.clone(), 15, Some(9));
        assert!(calculation.is_err());
    }
}
