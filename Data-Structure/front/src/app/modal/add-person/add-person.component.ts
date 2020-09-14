import { Component, OnInit, Input, EventEmitter, Output } from "@angular/core";
import { ModelService, ElectronService } from "../../core/services";
import {
  FormControl,
  FormBuilder,
  FormGroup,
  Validators,
} from "@angular/forms";
import { NzMessageService } from "ng-zorro-antd/message";
import { Person } from "zekin_data_structure";

interface PersonValue {
  start_city: number;
  end_city: number;
  statu: number;
  statu_value: string;
}

// 添加旅客Modal
@Component({
  selector: "app-add-person",
  templateUrl: "./add-person.component.html",
})
export class AddPersonComponent implements OnInit {
  @Input() isVisible: boolean = false;
  @Output() onClose: EventEmitter<any> = new EventEmitter();
  @Output() isVisibleChange = new EventEmitter();

  validateForm: FormGroup;
  cityName: string[];
  hasTimeLimit: boolean;

  setFalse() {
    this.onClose.emit();
    this.isVisible = false;
    this.isVisibleChange.emit(false);
  }

  handleOk() {
    if (!this.validateForm.valid) {
      return;
    }
    const value = this.validateForm.value;
    try {
      this.modelService.addPerson(
        value.start_city,
        value.end_city,
        value.has_time_limit ? value.time_limit : null
      );
      let person: Person = this.modelService.module.people.slice(-1)[0];
      this.message.create("success", "添加成功");
      this.electronService.writeFile(`添加一名乘客：`);
      this.electronService.writeFile(`  起始城市：${person.start_city.name}`);
      this.electronService.writeFile(`  目的城市：${person.end_city.name}`);
      this.electronService.writeFile(`  花费时间：${person.cost_time}`);
      this.electronService.writeFile(`  总计风险：${person.total_risk}`);
      this.setFalse();
    } catch (e) {
      this.message.create("error", "没有找到可行的方案！");
    }
  }

  constructor(
    public modelService: ModelService,
    public electronService: ElectronService,
    private fb: FormBuilder,
    private message: NzMessageService
  ) {
    this.cityName = Object.keys(this.modelService.nameToLocation);
    this.validateForm = this.fb.group({
      start_city: [0, Validators.required],
      end_city: [1, [Validators.required, this.repeatValidator]],
      has_time_limit: [false],
      time_limit: [1, [Validators.required, Validators.min(1)]],
    });
    this.validateForm.controls.end_city.markAsTouched();
  }

  repeatValidator = (control: FormControl): { [s: string]: boolean } => {
    if (
      this.validateForm &&
      control.value === this.validateForm.controls.start_city.value
    ) {
      return { repeat: true, error: true };
    }
    return {};
  };

  validateRepeatValue(): void {
    setTimeout(() =>
      this.validateForm.controls.end_city.updateValueAndValidity()
    );
  }

  ngOnInit(): void {}
}
