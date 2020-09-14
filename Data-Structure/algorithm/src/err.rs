//! 模块的错误处理
use serde::Serialize;
use wasm_bindgen::prelude::*;

/// 模块的错误处理
#[wasm_bindgen]
#[derive(Serialize)]
pub enum ModuleErr {
    /// 没有找到开始城市
    StartCityNotFound,
    /// 没有找到结束城市
    EndCityNotFound,
    /// 没有找到可行道路
    WaysNotFound,
}
