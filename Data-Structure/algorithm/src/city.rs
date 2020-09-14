//! 程序的city模块，用来存储和计算城市的各种信息
//!
//! # examples
//! ``` rust
//! use data_structure::city::*;
//!
//! let city = city::new(1, RiskLevel::High);
//! ```

use crate::way::*;
use getset::Getters;
use std::rc::Rc;
use wasm_bindgen::prelude::*;

#[cfg(target_arch = "wasm32")]
use js_sys::Array;

/// 城市的风险等级，一共分万三种
/// 1. High（高风险）
/// 2. Medium (中风险)
/// 3. Low （低风险）
#[wasm_bindgen]
#[derive(Debug, PartialEq, Clone)]
pub enum RiskLevel {
    /// 高风险， 风险权重为 0.9
    High,
    /// 中风险， 风险权重为 0.5
    Medium,
    /// 低风险， 风险权重为 0.2
    Low,
}

/// 城市类， 表示城市的各种基本信息
#[wasm_bindgen]
#[derive(Debug, Getters, Clone)]
pub struct City {
    /// 返回城市的唯一id
    #[getset(get = "pub")]
    id: i32,

    /// 返回城市名称
    #[getset(get = "pub")]
    name: String,

    /// 返回城市的风险等级
    #[getset(get = "pub")]
    risk_level: RiskLevel,

    /// 返回由城市出发的交通工具
    #[getset(get = "pub")]
    start_ways: Vec<Rc<Way>>,
}

impl PartialEq for City {
    fn eq(&self, other: &Self) -> bool {
        self.id == other.id
    }
}

impl Eq for City {}

impl City {
    /// City 的构造函数，创建一个新City
    /// # Arguments
    /// * `id` - 城市的唯一id
    /// * `risk_level` - 城市的风险等级
    pub fn new(id: i32, name: String, risk_level: RiskLevel) -> City {
        City {
            id: id,
            name: name,
            risk_level: risk_level,
            start_ways: Vec::new(),
        }
    }

    /// 城市添加一个交通工具
    /// # Arguments
    /// * `way` - way的Rc指针
    pub fn add_way(&mut self, way: Rc<Way>) {
        self.start_ways.push(way);
    }

    /// 返回城市的风险权重系数
    /// * 高风险 => 0.9
    /// * 中风险 => 0.5
    /// * 低风险 => 0.2
    pub fn risk(&self) -> f64 {
        match self.risk_level {
            RiskLevel::High => 0.9,
            RiskLevel::Medium => 0.5,
            RiskLevel::Low => 0.2,
        }
    }
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen]
impl City {
    /// 返回城市的唯一id
    #[wasm_bindgen(getter = id)]
    pub fn get_id_ts_binding(&self) -> i32 {
        self.id
    }
    
    /// 返回城市的名称
    #[wasm_bindgen(getter = name)]
    pub fn get_name_ts_binding(&self) -> String {
        self.name().clone()
    }

    /// 返回城市的风险等级
    #[wasm_bindgen(getter = risk)]
    pub fn get_risk_ts_binding(&self) -> f64 {
        self.risk()
    }

    /// 返回由城市出发的交通工具
    #[wasm_bindgen(getter = start_city)]
    pub fn get_start_ways_ts_binding(&self) -> Array {
        unsafe {
            self.start_ways
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
    fn get_high_risk() {
        let city_high_risk_level = City::new(0, String::from("111"), RiskLevel::High);
        assert_eq!(city_high_risk_level.risk(), 0.9);
    }

    #[test]
    fn get_medium_risk() {
        let city_medium_risk_level = City::new(0, String::from("111"), RiskLevel::Medium);
        assert_eq!(city_medium_risk_level.risk(), 0.5);
    }

    #[test]
    fn get_low_risk() {
        let city_low_risk_level = City::new(0, String::from("111"), RiskLevel::Low);
        assert_eq!(city_low_risk_level.risk(), 0.2);
    }

    #[test]
    fn add_way() {
        let mut city_0 = Rc::new(City::new(0, String::from("111"), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::from("111"), RiskLevel::Low));
        let way_rc = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));

        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_rc.clone());
        }
        assert_eq!(city_0.start_ways().len(), 1);
        assert_eq!(city_1.start_ways().len(), 0);
    }
}

#[cfg(target_arch = "wasm32")]
#[cfg(test)]
mod tests_ts_binding {
    use super::*;
    use wasm_bindgen_test::*;

    #[wasm_bindgen_test]
    fn get_id_0() {
        let city_0 = City::new(0, String::new(), RiskLevel::High);
        assert_eq!(city_0.get_id_ts_binding(), 0);
    }

    #[wasm_bindgen_test]
    fn get_id_1() {
        let city_1 = City::new(1, String::new(), RiskLevel::High);
        assert_eq!(city_1.get_id_ts_binding(), 1);
    }

    #[wasm_bindgen_test]
    fn get_high_risk() {
        let city_high_risk_level = City::new(0, String::new(), RiskLevel::High);
        assert_eq!(city_high_risk_level.get_risk_ts_binding(), 0.9);
    }

    #[wasm_bindgen_test]
    fn get_medium_risk() {
        let city_medium_risk_level = City::new(0, String::new(), RiskLevel::Medium);
        assert_eq!(city_medium_risk_level.get_risk_ts_binding(), 0.5);
    }

    #[wasm_bindgen_test]
    fn get_low_risk() {
        let city_low_risk_level = City::new(0, String::new(), RiskLevel::Low);
        assert_eq!(city_low_risk_level.get_risk_ts_binding(), 0.2);
    }

    #[wasm_bindgen_test]
    fn get_start_ways() {
        let mut city_0 = Rc::new(City::new(0, String::new(), RiskLevel::Low));
        let city_1 = Rc::new(City::new(1, String::new(), RiskLevel::Low));
        let way_rc = Rc::new(Way::new(1, Type::Air, city_0.clone(), city_1.clone(), 3, 4));

        unsafe {
            Rc::get_mut_unchecked(&mut city_0).add_way(way_rc.clone());
        }
        assert_eq!(city_0.start_ways().len(), 1);
        assert_eq!(city_1.start_ways().len(), 0);

        let array = city_0.get_start_ways_ts_binding();
        assert_eq!(array.length(), 1);
    }
}
