//! 数据结构课设
//!
//! COVID-19疫情环境下低风险旅行模拟系统的设计(Rust 算法部分)
//!
//! 提供了两组接口，一组Rust，另外一组将Rust编译成wasm，供ts调用
//!
#![feature(get_mut_unchecked)]
pub mod algorithm;
pub mod city;
pub mod err;
pub mod person;
pub mod way;

use crate::city::*;
use crate::err::*;
use crate::person::*;
use crate::way::*;
use getset::Getters;
use std::rc::Rc;
use wasm_bindgen::prelude::*;

#[cfg(target_arch = "wasm32")]
use js_sys::Array;

/// 主模块，用于统筹资源
#[wasm_bindgen]
#[derive(Debug, Getters, Default)]
pub struct Module {
    ///存储所有城市
    #[getset(get = "pub")]
    cities: Vec<Rc<City>>,

    ///存储所有交通方式
    #[getset(get = "pub")]
    ways: Vec<Rc<Way>>,

    ///存储所有乘客
    #[getset(get = "pub")]
    people: Vec<Rc<Person>>,

    ///系统时间
    #[getset(get = "pub")]
    time: i32,
}

impl Module {
    /// 添加一座城市
    pub fn add_city(&mut self, id: i32, name: String, risk_level: RiskLevel) {
        self.cities.push(Rc::new(City::new(id, name, risk_level)))
    }

    /// 添加一个交通方式
    pub fn add_way(
        &mut self,
        id: i32,
        way_type: Type,
        cost_time: i32,
        start_time: i32,
        start_city_index: usize,
        end_city_index: usize,
    ) -> Result<(), ModuleErr> {
        let (mut start_city, end_city) =
            self.get_start_end_city(start_city_index, end_city_index)?;
        let way = Rc::new(Way::new(
            id,
            way_type,
            start_city.clone(),
            end_city,
            cost_time,
            start_time,
        ));
        unsafe {
            Rc::get_mut_unchecked(&mut start_city).add_way(way.clone());
        }
        self.ways.push(way.clone());
        Ok(())
    }

    /// 添加一个乘客
    pub fn add_person(
        &mut self,
        start_city_index: usize,
        end_city_index: usize,
        time_limit: Option<i32>,
    ) -> Result<(), ModuleErr> {
        let (start_city, end_city) = self.get_start_end_city(start_city_index, end_city_index)?;
        let person = Person::new(start_city, end_city, self.time, time_limit)?;
        self.people.push(Rc::new(person));
        Ok(())
    }

    /// 使Module的时间前进1小时，并更新各个乘客的状态
    pub fn step(&mut self) {
        self.time += 1;
        unsafe {
            for person in &mut self.people {
                Rc::get_mut_unchecked(person).step(self.time);
            }
        }
    }

    ///清理Module的数据
    pub fn clear(&mut self) {
        self.time = 0;
        self.cities.clear();
        self.ways.clear();
        self.people.clear();
    }

    fn get_start_end_city(
        &self,
        start_city_index: usize,
        end_city_index: usize,
    ) -> Result<(Rc<City>, Rc<City>), ModuleErr> {
        let start_city = self.cities.get(start_city_index);
        let start_city = match start_city {
            Some(city) => city,
            None => return Err(ModuleErr::StartCityNotFound),
        };
        let end_city = self.cities.get(end_city_index);
        let end_city = match end_city {
            Some(city) => city,
            None => return Err(ModuleErr::EndCityNotFound),
        };
        Ok((start_city.clone(), end_city.clone()))
    }
}

#[cfg(target_arch = "wasm32")]
#[wasm_bindgen]
impl Module {
    /// 创建一个新的Module对象
    #[wasm_bindgen(constructor)]
    pub fn new() -> Self {
        Default::default()
    }

    /// 返回当前时间
    #[wasm_bindgen(getter = time)]
    pub fn get_time_ts_binding(&self) -> i32 {
        self.time
    }

    /// 返回所有城市
    #[wasm_bindgen(getter = cities)]
    pub fn get_cities_ts_binding(&self) -> Array {
        unsafe {
            self.cities
                .iter()
                .map(|i| JsValue::from((*Rc::into_raw(i.clone())).clone()))
                .collect()
        }
    }

    /// 返回所有交通工具
    #[wasm_bindgen(getter = ways)]
    pub fn get_ways_ts_binding(&self) -> Array {
        unsafe {
            self.ways
                .iter()
                .map(|i| JsValue::from((*Rc::into_raw(i.clone())).clone()))
                .collect()
        }
    }

    /// 返回所有旅客
    #[wasm_bindgen(getter = people)]
    pub fn get_people_ts_binding(&self) -> Array {
        unsafe {
            self.people
                .iter()
                .map(|i| JsValue::from((*Rc::into_raw(i.clone())).clone()))
                .collect()
        }
    }

    /// 添加一座城市
    #[wasm_bindgen(js_name = add_city)]
    pub fn add_city_ts_binding(&mut self, id: i32, name: String, risk_level: RiskLevel) {
        self.add_city(id, name, risk_level)
    }

    /// 添加一个交通方式
    #[wasm_bindgen(js_name = add_way)]
    pub fn add_way_ts_binding(
        &mut self,
        id: i32,
        way_type: Type,
        cost_time: i32,
        start_time: i32,
        start_city_index: usize,
        end_city_index: usize,
    ) -> Result<(), JsValue> {
        match self.add_way(
            id,
            way_type,
            cost_time,
            start_time,
            start_city_index,
            end_city_index,
        ) {
            Ok(_) => Ok(()),
            Err(err) => Err(JsValue::from_serde(&err).ok().unwrap()),
        }
    }

    /// 添加一个乘客
    #[wasm_bindgen(js_name = add_person)]
    pub fn add_person_ts_binding(
        &mut self,
        start_city_index: usize,
        end_city_index: usize,
        time_limit: Option<i32>,
    ) -> Result<(), JsValue> {
        match self.add_person(start_city_index, end_city_index, time_limit) {
            Ok(_) => Ok(()),
            Err(err) => Err(JsValue::from_serde(&err).ok().unwrap()),
        }
    }

    /// 使Module的时间前进1小时，并更新各个乘客的状态
    #[wasm_bindgen(js_name = step)]
    pub fn step_ts_binding(&mut self) {
        self.step()
    }

    /// 使Module的时间前进1小时，并更新各个乘客的状态
    #[wasm_bindgen(js_name = clear)]
    pub fn clear_ts_binding(&mut self) {
        self.clear()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn add_city() {
        let mut m: Module = Default::default();
        m.add_city(0, String::new(), RiskLevel::Low);
        m.add_city(1, String::new(), RiskLevel::High);
        m.add_city(2, String::new(), RiskLevel::Medium);
        assert_eq!(m.cities().len(), 3);
        assert_eq!(m.cities()[0].risk(), 0.2);
        assert_eq!(m.cities()[1].risk(), 0.9);
        assert_eq!(m.cities()[2].risk(), 0.5);
    }

    #[test]
    fn add_way() {
        let mut m: Module = Default::default();
        m.add_city(0, String::new(), RiskLevel::Low);
        m.add_city(1, String::new(), RiskLevel::High);
        m.add_city(2, String::new(), RiskLevel::Medium);
        assert!(m.add_way(0, Type::Air, 5, 20, 0, 1).is_ok());
        assert!(m.add_way(1, Type::Car, 3, 21, 1, 0).is_ok());
        assert!(m.add_way(2, Type::Train, 4, 18, 1, 2).is_ok());
        assert!(m.add_way(3, Type::Car, 9, 10, 2, 0).is_ok());

        assert_eq!(m.cities().len(), 3);
        assert_eq!(m.ways().len(), 4);
    }

    #[test]
    fn add_way_failed() {
        let mut m: Module = Default::default();
        m.add_city(0, String::new(), RiskLevel::Low);
        m.add_city(1, String::new(), RiskLevel::High);
        m.add_city(2, String::new(), RiskLevel::Medium);

        assert!(m.add_way(4, Type::Car, 9, 10, 0, 3).is_err());
    }

    #[test]
    fn add_person() {
        let mut m: Module = Default::default();
        m.add_city(0, String::new(), RiskLevel::Low);
        m.add_city(1, String::new(), RiskLevel::High);
        m.add_city(2, String::new(), RiskLevel::Medium);
        assert!(m.add_way(0, Type::Air, 5, 20, 0, 1).is_ok());
        assert!(m.add_way(1, Type::Car, 3, 21, 1, 0).is_ok());
        assert!(m.add_way(2, Type::Train, 4, 18, 1, 2).is_ok());
        assert!(m.add_way(3, Type::Car, 9, 10, 2, 0).is_ok());
        assert!(m.add_person(0, 1, None).is_ok());
        assert_eq!(m.people().len(), 1);
    }
}

#[cfg(target_arch = "wasm32")]
// wasm_bindgen_test::wasm_bindgen_test_configure!(run_in_browser);
#[cfg(test)]
mod tests_ts_binding {
    use super::*;
    use wasm_bindgen_test::*;

    #[wasm_bindgen_test]
    fn get_time() {
        let m: Module = Default::default();
        assert_eq!(m.get_time_ts_binding(), 0);
    }
}
