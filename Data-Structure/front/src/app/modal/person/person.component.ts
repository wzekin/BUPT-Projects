import { Component, OnInit, Input, EventEmitter, Output } from "@angular/core";
import { ModelService, City } from "../../core/services";

interface PersonValue {
  start_city: number;
  end_city: number;
  statu: number;
  statu_value: string;
}

// Person信息Modal，用于显示Modal信息
@Component({
  selector: "app-person",
  templateUrl: "./person.component.html",
})
export class PersonComponent implements OnInit {
  @Input() isVisible: boolean = false;
  @Output() onClose: EventEmitter<any> = new EventEmitter();
  @Output() isVisibleChange = new EventEmitter();

  cityName: string[];
  people: PersonValue[];

  setFalse() {
    this.onClose.emit();
    this.isVisible = false;
    this.isVisibleChange.emit(false);
  }

  constructor(public modelService: ModelService) {
    this.cityName = Object.keys(this.modelService.nameToLocation);
  }

  update() {
    this.people = this.modelService.module.people.map((value) => ({
      start_city: value.start_city.id,
      end_city: value.end_city.id,
      statu: value.statu,
      statu_value: this.calulate_statu(value),
    }));
  }

  calulate_statu(person): string {
    let statu_value = person.statu_value;
    if (person.statu === 2) {
      return `${this.cityName[statu_value.start_city.id]} -> ${
        this.cityName[statu_value.end_city.id]
      }`;
    } else {
      return `${this.cityName[statu_value.id]}`;
    }
  }

  ngOnInit(): void {}
}
