import { Component, OnInit, Input, EventEmitter, Output } from "@angular/core";
import { ModelService, City } from "../../core/services";

// 城市信息Modal, 用于显示城市信息
@Component({
  selector: "app-city",
  templateUrl: "./city.component.html",
})
export class CityComponent implements OnInit {
  @Input() isVisible: boolean = false;
  @Output() onClose: EventEmitter<any> = new EventEmitter();
  @Output() isVisibleChange = new EventEmitter();

  cityData: City[];

  setFalse() {
    this.onClose.emit();
    this.isVisible = false;
    this.isVisibleChange.emit(false);
  }

  handleOk() {
    this.modelService.setCityInfo(this.cityData);
    this.setFalse();
  }

  handleCancle() {
    this.setFalse();
  }

  handleAfterOpen() {
    this.cityData = [...this.modelService.cityInfo];
  }

  ModelChange($event: number, index: number) {
    this.cityData[index].risk_level = $event;
  }

  constructor(public modelService: ModelService) {}

  ngOnInit(): void {}
}
