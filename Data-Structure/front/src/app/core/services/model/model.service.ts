import { Injectable } from "@angular/core";
import { Module } from "zekin_data_structure";
import {
  getCityInfo,
  setCityInfo,
  getWayInfo,
  setWayInfo,
  NameToLocation,
} from "./model.init";
import { ElectronService } from "../electron/electron.service";

export interface City {
  key: number;
  name: string;
  risk_level: number;
}

export interface Way {
  id: number;
  start_city: number;
  end_city: number;
  type: number;
  start_time: number;
  cost_time: number;
}

@Injectable({
  providedIn: "root",
})
export class ModelService {
  nameToLocation: Object;
  // 城市信息
  cityInfo: City[];
  // 路径信息
  wayInfo: Way[];

  wasm: any;
  module: Module;

  constructor(private electronService: ElectronService) {
    this.nameToLocation = NameToLocation;
    this.cityInfo = getCityInfo();
    this.wayInfo = getWayInfo();
    this.loadWasm();
  }

  // 加载wasm模块
  async loadWasm() {
    try {
      this.wasm = await import("zekin_data_structure");
      this.module = new this.wasm.Module();
      this.reset();
    } catch (err) {
      console.error(`Unexpected error in loadWasm. [Message: ${err.message}]`);
    }
  }

  // 重置模型
  reset() {
    this.electronService.writeFile("时刻数据发生变化，模块数据已被重置");
    this.module.clear();
    this.cityInfo.forEach((value) => {
      this.module.add_city(value.key, value.name, value.risk_level);
    });
    this.wayInfo.forEach((value) => {
      this.module.add_way(
        value.id,
        value.type,
        value.cost_time,
        value.start_time,
        value.start_city,
        value.end_city
      );
    });
  }

  // 添加person
  addPerson(start_city: number, end_city: number, time_limit?: number) {
    this.module.add_person(start_city, end_city, time_limit);
  }

  // 时间经过一个小时
  step() {
    this.module.step();
  }

  // 设置城市信息
  setCityInfo(data: City[]) {
    this.cityInfo = JSON.parse(JSON.stringify(data));
    setCityInfo(this.cityInfo);
    this.reset();
  }

  // 设置路径信息
  setWayInfo(data: Way[]) {
    this.wayInfo = JSON.parse(JSON.stringify(data));
    setWayInfo(this.wayInfo);
    this.reset();
  }
}
