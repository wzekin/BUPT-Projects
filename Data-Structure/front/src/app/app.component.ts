import { Component, OnInit } from "@angular/core";
import { ElectronService, ModelService } from "./core/services";
import { AppConfig } from "../environments/environment";
import { MiniDaemon, calulate_statu } from "./core/services/util";
import { Way } from "zekin_data_structure";

@Component({
  selector: "app-root",
  templateUrl: "./app.component.html",
  styleUrls: ["./app.component.scss"]
})
export class AppComponent implements OnInit {
  // App初始化log
  constructor(
    public modelService: ModelService,
    public electronService: ElectronService
  ) {
    console.log("AppConfig", AppConfig);

    if (electronService.isElectron) {
      console.log(process.env);
      console.log("Mode electron");
      console.log("Electron ipcRenderer", electronService.ipcRenderer);
      console.log("NodeJS childProcess", electronService.childProcess);
    } else {
      console.log("Mode web");
    }
  }

  // App地图
  map: any;
  // 地图上线的集合
  polyLineLayer: any[] = [];
  // 地图上点的集合
  markerLayer: any[] = [];

  // 计时器，用于计时
  time: MiniDaemon = new MiniDaemon(
    this,
    () => {
      this.modelService.step();
      this.printData();
      this.draw();
    },
    8000
  );

  cityVisible: boolean = false; // city modal 是否可见
  wayVisible: boolean = false; // way modal 是否可见
  personVisible: boolean = false; // person modal 是否可见
  addPersonVisible: boolean = false; // addperson modal 是否可见
  open_modal_is_pause: boolean = false; // 在打开modal时计时器是否处于开启状态

  // 关闭modal事件绑定
  handleCloseModal() {
    this.draw();
    if (!this.open_modal_is_pause) {
      this.time.start();
    }
  }

  // 打开modal事件绑定
  handleOpenModal(name: string) {
    this[name] = true;
    this.open_modal_is_pause = this.time.PAUSED;
    this.time.pause();
  }

  // App Init事件绑定
  ngOnInit() {
    this.map = new window.AMap.Map("map", {
      zoom: 5 //级别
    });
  }

  // 转换计时器状态
  toggleState() {
    if (this.time.PAUSED) {
      this.draw();
      this.time.start();
    } else {
      this.time.pause();
    }
  }

  // 打印时刻信息
  printData() {
    const people = this.modelService.module.people
      .filter(person => person.statu !== 0)
      .map(value => ({
        start_city: value.start_city,
        end_city: value.end_city,
        statu: value.statu,
        statu_value: calulate_statu(value)
      }));
    this.electronService.writeFile(
      `-----------------当前时间：${this.modelService.module.time}-----------------`
    );
    this.electronService.writeFile(`当前未到达乘客：${people.length}`);
    for (let i in people) {
      this.electronService.writeFile(`乘客${i}：`);
      this.electronService.writeFile(
        `  起始城市：${people[i].start_city.name}`
      );
      this.electronService.writeFile(`  目的城市：${people[i].end_city.name}`);
      this.electronService.writeFile(
        `  当前状态：${people[i].statu === 1 ? "等待中" : "乘坐交通工具中"}`
      );
      this.electronService.writeFile(`  当前城市：${people[i].statu_value}`);
    }
  }

  // 绘制地图
  draw() {
    for (const line of this.polyLineLayer) {
      this.map.remove(line);
    }
    for (const marker of this.markerLayer) {
      this.map.remove(marker);
    }
    this.polyLineLayer = [];
    const people = this.modelService.module.people;
    for (const person of people) {
      if (person.statu === 0) {
        continue;
      } else if (person.statu === 1) {
        this.draw_marker(
          this.modelService.nameToLocation[person.statu_value.name],
          "user"
        );
      } else {
        const way: Way = person.statu_value;
        const position = this.get_position(person);
        if (way.type === 0) {
          this.draw_marker(position, "car");
        } else if (way.type === 1) {
          this.draw_marker(position, "train");
        } else {
          this.draw_marker(position, "plane");
        }
      }
      this.draw_polyline(person);
    }
  }

  // 添加一个点
  draw_marker(location, icon) {
    let that = this;
    window.AMapUI.loadUI(["overlay/AwesomeMarker"], function(AwesomeMarker) {
      that.markerLayer.push(
        new AwesomeMarker({
          awesomeIcon: icon,
          iconLabel: {
            style: {
              color: "#333"
            }
          },
          iconStyle: "orange",
          map: that.map,
          position: location
        })
      );
    });
  }

  // 添加一条路径
  draw_polyline(person) {
    const path = [];
    const road = person.road;
    path.push(this.modelService.nameToLocation[road[0].start_city.name]);
    for (const way of road) {
      path.push(this.modelService.nameToLocation[way.end_city.name]);
    }
    var polyline = new window.AMap.Polyline({
      path: path,
      borderWeight: 2, // 线条宽度，默认为 1
      strokeColor: "red", // 线条颜色
      lineJoin: "round" // 折线拐点连接处样式
    });

    this.polyLineLayer.push(polyline);

    this.map.add(polyline);
  }

  // 拿到未到达的旅客的人数
  get_people_number() {
    return this.modelService.module
      ? this.modelService.module.people.filter(item => item.statu !== 0).length
      : 0;
  }

  // 拿到旅客当前位置
  get_position(person): any {
    const time = this.modelService.module.time;
    const way: Way = person.statu_value;
    const start_city = this.modelService.nameToLocation[way.start_city.name];
    const end_city = this.modelService.nameToLocation[way.end_city.name];
    let now = (time % 24) - way.start_time;
    now = now >= 0 ? now : now + 24;
    return new window.AMap.LngLat(
      start_city.lng - ((start_city.lng - end_city.lng) / way.cost_time) * now,
      start_city.lat - ((start_city.lat - end_city.lat) / way.cost_time) * now
    );
  }
}
