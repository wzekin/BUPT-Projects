//! 程序的person模块， 用来储存和计算旅客的各种信息
//!
//! # examples
//! ``` rust
//! use std::rc::Rc;
//! use data_structure::person::*;
//! use data_structure::city::*;
//! use data_structure::way::*;
//!
//! let mut city_0 = Rc::new(City::new(0, RiskLevel::Low));
//! let city_1 = Rc::new(City::new(1, RiskLevel::Low));
//!
//! let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
//!
//! unsafe {
//!     Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
//! }
//!
//! let person = Person::new(city_0.clone(), city_1, 1, None);
//! assert!(person.is_ok());
//!
//! let person = person.ok().unwrap();
//! assert_eq!(person.statu, Status::Waiting(city_0));
//! ```

use crate::algorithm::*;
use crate::city::*;
use crate::err::ModuleErr;
use crate::way::*;
use getset::Getters;
use std::rc::Rc;
use wasm_bindgen::prelude::*;

#[cfg(target_arch = "wasm32")]
use js_sys::Array;

/// 旅客当前的状态分类，一共有三种状态
/// 1. Arrived（已到达）
/// 2. Waiting (等待中)
/// 3. Taking （乘坐交通工具中）
#[derive(Debug, Clone, PartialEq, Eq)]
pub enum Status {
    /// 已到达
    ///
    /// * 其中值为已到达的城市
    Arrived(Rc<City>),
    /// 等待中
    ///
    /// * 其中值为等待中的城市
    Waiting(Rc<City>),
    /// 乘坐交通工具中
    ///
    /// * 其中值为正在乘坐的交通工具
    Taking(Rc<Way>),
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen]
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
pub enum StatusTsBinding {
    Arrived,
    Waiting,
    Taking,
}

/// 旅客类， 表示旅客的各种基本信息
#[wasm_bindgen]
#[derive(Debug, Getters, Clone)]
pub struct Person {
    /// 返回旅客的当前状态
    #[getset(get = "pub")]
    statu: Status,

    /// 返回旅客的起始城市
    #[getset(get = "pub")]
    start_city: Rc<City>,

    /// 返回旅客的目的城市
    #[getset(get = "pub")]
    end_city: Rc<City>,

    /// 返回旅客是否有时间限制
    #[getset(get = "pub")]
    time_limit: Option<i32>,

    /// 返回旅客预计花费的总时间
    #[getset(get = "pub")]
    cost_time: i32,

    /// 返回旅客的出发时间
    #[getset(get = "pub")]
    start_time: i32,

    /// 返回旅客此次旅途的总风险
    #[getset(get = "pub")]
    total_risk: f64,

    /// 返回旅客此次旅途的详细路程
    #[getset(get = "pub")]
    road: Vec<Rc<Way>>,
}

impl Person {
    /// Person 的构造函数，创建一个新Person
    /// # Arguments
    /// * `start_city` - 旅客的起始城市
    /// * `end_city` - 旅客的目的城市
    /// * `start_time` - 旅客的出发时间
    /// * `time_limit` - 旅客是否有时间限制
    /// # Returns
    /// * 一个Result, 程序会自动计算是否有可行的路径，如果有则返回乘客的信息，如果没有，则返回Err
    pub fn new(
        start_city: Rc<City>,
        end_city: Rc<City>,
        start_time: i32,
        time_limit: Option<i32>,
    ) -> Result<Person, ModuleErr> {
        let r = calculate(start_city.clone(), end_city.clone(), start_time, time_limit)?;
        Ok(Person {
            start_city: start_city.clone(),
            end_city,
            time_limit,
            start_time,
            statu: Status::Waiting(start_city.clone()),

            cost_time: r.end_time() - start_time,
            total_risk: r.total_risk().clone(),
            road: r.road().clone(),
        })
    }

    /// Person 的移动函数
    ///
    /// Person将随所给的时间计算当前移动的进程， 并更新statu，如果Person已经处于Arrived状态，则不会再更新
    pub fn step(&mut self, now_time: i32) {
        match self.statu {
            Status::Arrived(_) => return,
            _ => {}
        }
        let mut pass_time = now_time - self.start_time;
        let mut time = self.start_time;
        for way in &self.road {
            let waiting_time = calculate_time(&(time % 24), way.start_time());
            if waiting_time > pass_time {
                self.statu = Status::Waiting(way.start_city().clone());
                return;
            }
            pass_time -= waiting_time;
            if way.cost_time() > &pass_time {
                self.statu = Status::Taking(way.clone());
                return;
            }
            pass_time -= way.cost_time();
            time += waiting_time + way.cost_time();
        }
        self.statu = Status::Arrived(self.end_city.clone());
    }
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen]
impl Person {
    /// 返回旅客的当前状态
    #[wasm_bindgen(getter = statu)]
    pub fn get_status_ts_binding(&self) -> StatusTsBinding {
        match self.statu {
            Status::Arrived(_) => StatusTsBinding::Arrived,
            Status::Taking(_) => StatusTsBinding::Taking,
            Status::Waiting(_) => StatusTsBinding::Waiting,
        }
    }

    /// 返回旅客的当前状态的值
    #[wasm_bindgen(getter = statu_value)]
    pub fn get_status_value_ts_binding(&self) -> JsValue {
        match &self.statu {
            Status::Arrived(a) => JsValue::from(unsafe { (*Rc::into_raw(a.clone())).clone() }),
            Status::Taking(a) => JsValue::from(unsafe { (*Rc::into_raw(a.clone())).clone() }),
            Status::Waiting(a) => JsValue::from(unsafe { (*Rc::into_raw(a.clone())).clone() }),
        }
    }

    /// 返回旅客的起始城市
    #[wasm_bindgen(getter = start_city)]
    pub fn get_start_city_ts_binding(&self) -> City {
        unsafe { (*Rc::into_raw(self.start_city.clone())).clone() }
    }

    /// 返回旅客的目的城市
    #[wasm_bindgen(getter = end_city)]
    pub fn get_end_city_ts_binding(&self) -> City {
        unsafe { (*Rc::into_raw(self.end_city.clone())).clone() }
    }

    /// 返回旅客是否有时间限制
    #[wasm_bindgen(getter = time_limit)]
    pub fn get_time_limit_ts_binding(&self) -> Option<i32> {
        self.time_limit
    }

    /// 返回旅客预计花费的总时间
    #[wasm_bindgen(getter = cost_time)]
    pub fn get_cost_time_ts_binding(&self) -> i32 {
        self.cost_time
    }

    /// 返回旅客的出发时间
    #[wasm_bindgen(getter = start_time)]
    pub fn get_start_time_ts_binding(&self) -> i32 {
        self.start_time
    }

    /// 返回旅客此次旅途的总风险
    #[wasm_bindgen(getter = total_risk)]
    pub fn get_total_risk_ts_binding(&self) -> f64 {
        self.total_risk
    }

    /// 返回旅客此次旅途的详细路程
    #[wasm_bindgen(getter = road)]
    pub fn get_road_ts_binding(&self) -> Array {
        unsafe {
            self.road
                .iter()
                .map(|i| JsValue::from((*Rc::into_raw(i.clone())).clone()))
                .collect()
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn calculate_time_from_up_to() {
        assert_eq!(calculate_time(&20, &10), 14);
    }

    #[test]
    fn calculate_time_from_down_to() {
        assert_eq!(calculate_time(&10, &20), 10);
    }
    #[test]
    fn new_person() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, None);
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(person.statu, Status::Waiting(city_0));
    }

    #[test]
    fn step() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1.clone(), 2, None);
        assert!(person.is_ok());
        let mut person = person.ok().unwrap();
        assert_eq!(person.statu, Status::Waiting(city_0));
        person.step(4);
        assert_eq!(person.statu, Status::Taking(way_air.clone()));
        person.step(7);
        assert_eq!(person.statu, Status::Arrived(city_1));
    }
}

#[cfg(target_arch = "wasm32")]
#[cfg(test)]
mod tests_ts_binding {
    use super::*;
    use wasm_bindgen_test::*;

    #[wasm_bindgen_test]
    fn get_status() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, None);
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(person.get_status_ts_binding(), StatusTsBinding::Waiting);
    }

    #[wasm_bindgen_test]
    fn get_status_value() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, None);
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert!(person.get_status_value_ts_binding().is_object())
    }

    #[wasm_bindgen_test]
    fn get_time_limit_none() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, None);
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(person.get_time_limit_ts_binding(), person.time_limit);
    }

    #[wasm_bindgen_test]
    fn get_time_limit_some() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, Some(10));
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(person.get_time_limit_ts_binding(), person.time_limit);
    }

    #[wasm_bindgen_test]
    fn get_cost_time() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, Some(10));
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(person.get_cost_time_ts_binding(), person.cost_time);
    }

    #[wasm_bindgen_test]
    fn get_start_city() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, Some(10));
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(
            person.get_start_city_ts_binding().get_id_ts_binding(),
            city_0.get_id_ts_binding()
        );
    }

    #[wasm_bindgen_test]
    fn get_end_city() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1.clone(), 1, Some(10));
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(
            person.get_end_city_ts_binding().get_id_ts_binding(),
            city_1.get_id_ts_binding()
        );
    }

    #[wasm_bindgen_test]
    fn get_start_time() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, Some(10));
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(person.get_start_time_ts_binding(), person.start_time);
    }

    #[wasm_bindgen_test]
    fn get_total_risk() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_air.clone());
        }
        let person = Person::new(city_0.clone(), city_1, 1, Some(10));
        assert!(person.is_ok());
        let person = person.ok().unwrap();
        assert_eq!(person.get_total_risk_ts_binding(), person.total_risk);
    }
}
