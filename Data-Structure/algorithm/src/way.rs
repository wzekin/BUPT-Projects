//! 程序的way模块， 用来储存和计算交通方式的各种信息
//!
//! # examples
//! ``` rust
//! use std::rc::Rc;
//! use data_structure::city::*;
//! use data_structure::way::*;
//!
//! let city_0 = Rc::new(City::new(0, RiskLevel::Low));
//! let city_1 = Rc::new(City::new(1, RiskLevel::Low));
//! let way_0 = Rc::new(Way::new(0, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
//! ```

use crate::city::*;
use getset::Getters;
use std::rc::Rc;
use wasm_bindgen::prelude::*;

/// 交通工具的种类，一共分为三种
/// 1. Car（汽车）
/// 2. Train（火车）
/// 3. Air（飞机）
#[wasm_bindgen]
#[derive(Debug, Clone, Copy)]
pub enum Type {
    /// 汽车
    Car,
    /// 火车
    Train,
    /// 飞机
    Air,
}

//交通方式类， 表示交通方式的各种信息
#[wasm_bindgen]
#[derive(Debug, Getters, Clone)]
pub struct Way {
    ///返回交通方式的唯一id
    #[getset(get = "pub")]
    id: i32,

    ///返回交通方式的种类
    #[getset(get = "pub")]
    way_type: Type,

    ///返回交通方式的起始城市
    #[getset(get = "pub")]
    start_city: Rc<City>,

    ///返回交通方式的终点城市
    #[getset(get = "pub")]
    end_city: Rc<City>,

    ///返回交通方式花费的时间
    #[getset(get = "pub")]
    cost_time: i32,

    ///返回交通方式花费的开始时间
    ///
    /// 取值为0-23
    #[getset(get = "pub")]
    start_time: i32,
}

impl PartialEq for Way {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

impl Eq for Way {}

impl Way {
    /// Way 的构造函数，创建一个新Way
    ///
    /// # Arguments
    /// * `id` - 交通方式的唯一id
    /// * `type` - 交通方式的种类
    /// * `start_city` - 交通方式的起始城市
    /// * `end_city` - 交通方式的终点城市
    /// * `cost_time` - 交通方式花费的时间
    /// * `start_time` - 交通方式的出发时间
    pub fn new(
        id: i32,
        way_type: Type,
        start_city: Rc<City>,
        end_city: Rc<City>,
        cost_time: i32,
        start_time: i32,
    ) -> Way {
        Way {
            id,
            way_type,
            start_city,
            end_city,
            cost_time,
            start_time,
        }
    }

    /// 返回交通方式的风险权重系数
    /// * 汽车 => 2
    /// * 火车 => 5
    /// * 飞机 => 9
    pub fn risk(&self) -> f64 {
        match &self.way_type {
            Type::Car => 2.0,
            Type::Train => 5.0,
            Type::Air => 9.0,
        }
    }
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen]
impl Way {
    ///返回交通方式的唯一id
    #[wasm_bindgen(getter = id)]
    pub fn get_id_ts_binding(&self) -> i32 {
        self.id
    }

    ///返回交通方式的种类
    #[wasm_bindgen(getter = type)]
    pub fn get_type_ts_binding(&self) -> Type {
        self.way_type
    }

    ///返回交通方式的风险
    #[wasm_bindgen(getter = risk)]
    pub fn get_risk_ts_binding(&self) -> f64 {
        self.risk()
    }

    ///返回交通方式的起始城市
    #[wasm_bindgen(getter = start_city)]
    pub fn get_start_city_ts_binding(&self) -> City {
        unsafe { (*Rc::into_raw(self.start_city.clone())).clone() }
    }

    ///返回交通方式的终点城市
    #[wasm_bindgen(getter = end_city)]
    pub fn get_end_city_ts_binding(&self) -> City {
        unsafe { (*Rc::into_raw(self.end_city.clone())).clone() }
    }

    ///返回交通方式花费的时间
    #[wasm_bindgen(getter = cost_time)]
    pub fn get_cost_time_ts_binding(&self) -> i32 {
        self.cost_time
    }

    ///返回交通方式花费的开始时间
    ///
    /// 取值为0-23
    #[wasm_bindgen(getter = start_time)]
    pub fn get_start_time_ts_binding(&self) -> i32 {
        self.start_time
    }
}

#[cfg(test)]

mod tests {
    use super::*;
    #[test]
    fn get_air_risk() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(way_air.risk(), 9.0);
    }

    #[test]
    fn get_car_risk() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_car = Rc::new(Way::new(1, Type::Car, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(way_car.risk(), 2.0);
    }

    #[test]
    fn get_train_risk() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_train = Rc::new(Way::new(
            1,
            Type::Train,
            city_0.clone(),
            city_1.clone(),
            3,
            4,
        ));
        assert_eq!(way_train.risk(), 5.0);
    }
}

#[cfg(target_arch = "wasm32")]
#[cfg(test)]
mod tests_ts_binding {
    use super::*;
    use wasm_bindgen_test::*;

    #[wasm_bindgen_test]
    fn get_id_0() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_0 = Rc::new(Way::new(0, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(way_0.get_id_ts_binding(), 0);
    }

    #[wasm_bindgen_test]
    fn get_id_1() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_1 = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(way_1.get_id_ts_binding(), 1);
    }

    #[wasm_bindgen_test]
    fn get_air_risk() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_air = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(way_air.get_risk_ts_binding(), 9.0);
    }

    #[wasm_bindgen_test]
    fn get_car_risk() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_car = Rc::new(Way::new(
            1,
            Type::Car,
            city_0.clone(),
            city_1.clone(),
            3,
            4,
        ));
        assert_eq!(way_car.get_risk_ts_binding(), 2.0);
    }

    #[wasm_bindgen_test]
    fn get_train_risk() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_train = Rc::new(Way::new(
            1,
            Type::Train,
            city_0.clone(),
            city_1.clone(),
            3,
            4,
        ));
        assert_eq!(way_train.get_risk_ts_binding(), 5.0);
    }

    #[wasm_bindgen_test]
    fn get_start_city() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_car = Rc::new(Way::new(1, Type::Car, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(
            way_car.get_start_city_ts_binding().get_id_ts_binding(),
            city_0.get_id_ts_binding()
        );
    }

    #[wasm_bindgen_test]
    fn get_end_city() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_car = Rc::new(Way::new(1, Type::Car, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(
            way_car.get_end_city_ts_binding().get_id_ts_binding(),
            city_1.get_id_ts_binding()
        );
    }

    #[wasm_bindgen_test]
    fn get_cost_time() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_car = Rc::new(Way::new(1, Type::Car, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(way_car.get_cost_time_ts_binding(), 3);
    }

    #[wasm_bindgen_test]
    fn get_start_time() {
        let city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_car = Rc::new(Way::new(1, Type::Car, city_0.clone(), city_1.clone(), 3, 4));
        assert_eq!(way_car.get_start_time_ts_binding(), 4);
    }
}
