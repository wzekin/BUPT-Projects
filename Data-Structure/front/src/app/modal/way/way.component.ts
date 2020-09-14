import {
  Component,
  OnInit,
  Input,
  EventEmitter,
  Output,
  ApplicationRef,
} from "@angular/core";
import { ModelService, Way } from "../../core/services";
import {
  FormControl,
  FormBuilder,
  FormGroup,
  Validators,
} from "@angular/forms";

// 路径信息Modal，用于显示路径信息
@Component({
  selector: "app-way",
  templateUrl: "./way.component.html",
  styleUrls: ["./way.component.css"],
})
export class WayComponent implements OnInit {
  @Input() isVisible: boolean = false;
  @Output() onClose: EventEmitter<any> = new EventEmitter();
  @Output() isVisibleChange = new EventEmitter();

  count: number;
  cityName: string[];
  typeName: string[] = ["汽车", "火车", "飞机"];
  validateForm: FormGroup;

  wayData: Way[] = [];
  edit_id: number = -1;
  edit_type: number = -1;

  setFalse() {
    this.onClose.emit();
    this.isVisible = false;
    this.isVisibleChange.emit(false);
  }

  handleOk() {
    for (let i in this.wayData) {
      this.wayData[i].id = Number(i);
    }
    this.modelService.setWayInfo(this.wayData);
    this.setFalse();
  }

  handleCancel() {
    this.setFalse();
  }

  handleAfterOpen() {
    this.wayData = [];
    this.count = this.modelService.wayInfo.length;
    this.modelService.wayInfo.map((data, i) => {
      this.wayData[i] = { ...data };
    });
  }

  startEdit(id: number): void {
    this.edit_id = id;
    const item_value = this.wayData.find((item) => item.id === id);
    this.edit_type = item_value.type;
    this.validateForm.setValue({
      start_city: item_value.start_city,
      end_city: item_value.end_city,
      type: item_value.type,
      cost_time: item_value.cost_time,
      start_time: item_value.start_time,
    });
  }

  cancelEdit(id: number): void {
    this.edit_id = -1;
    this.edit_type = -1;
  }

  saveEdit(id: number): void {
    if (!this.validateForm.valid) {
      return;
    }
    const index = this.wayData.findIndex((item) => item.id === id);
    Object.assign(this.wayData[index], this.validateForm.value);
    this.validateForm.reset();
    this.edit_id = -1;
    this.edit_type = -1;
  }

  deleteField(id: number): void {
    this.wayData = this.wayData.filter((d) => d.id !== id);
  }

  addField(): void {
    const index = this.count++;
    const way: Way = {
      id: index,
      start_city: 0,
      end_city: 1,
      type: 2,
      cost_time: 10,
      start_time: 0,
    };
    this.edit_id = index;
    this.edit_type = 2;
    this.validateForm.setValue({
      start_city: 0,
      end_city: 1,
      type: 2,
      cost_time: 10,
      start_time: 0,
    });
    this.wayData = [...this.wayData, way];
  }

  constructor(
    public modelService: ModelService,
    private fb: FormBuilder,
    private appRef: ApplicationRef
  ) {
    this.cityName = Object.keys(this.modelService.nameToLocation);
    this.validateForm = this.fb.group({
      start_city: [0, Validators.required],
      end_city: [1, [Validators.required, this.repeatValidator]],
      type: [2, [Validators.required, this.typeValidator]],
      cost_time: [1, [Validators.required, Validators.min(1)]],
      start_time: [
        0,
        [Validators.required, Validators.min(0), Validators.max(23)],
      ],
    });
    /// ........
    this.validateForm.controls.type.markAsTouched();
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

  typeValidator = (control: FormControl): { [s: string]: boolean } => {
    let air_count = this.wayData.filter((way) => way.type === 2).length;
    let train_count = this.wayData.filter((way) => way.type === 1).length;
    if (
      (air_count > 10 && control.value === 2) ||
      (air_count == 10 && control.value === 2 && this.edit_type !== 2)
    ) {
      return { air: true, error: true };
    } else if (
      (train_count > 30 && control.value === 1) ||
      (train_count == 30 && control.value === 1 && this.edit_type !== 1)
    ) {
      return { train: true, error: true };
    }
    return {};
  };

  validateRepeatValue(): void {
    this.validateForm.controls.end_city.updateValueAndValidity();
  }

  ngOnInit(): void {}
}
