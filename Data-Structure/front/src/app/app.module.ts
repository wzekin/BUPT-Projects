import "reflect-metadata";
import "../polyfills";

import { NgModule } from "@angular/core";
import { BrowserModule } from "@angular/platform-browser";
import { FormsModule, ReactiveFormsModule } from "@angular/forms";
import { CoreModule } from "./core/core.module";
import { BrowserAnimationsModule } from "@angular/platform-browser/animations";

import { NzLayoutModule } from "ng-zorro-antd/layout";
import { NzButtonModule } from "ng-zorro-antd/button";
import { NzMenuModule } from "ng-zorro-antd/menu";
import { NzModalModule } from "ng-zorro-antd/modal";
import { NzTableModule } from "ng-zorro-antd/table";
import { NzSelectModule } from "ng-zorro-antd/select";
import { NzPopconfirmModule } from "ng-zorro-antd/popconfirm";
import { NzTagModule } from "ng-zorro-antd/tag";
import { NzInputModule } from "ng-zorro-antd/input";
import { NzFormModule } from "ng-zorro-antd/form";
import { NzIconModule } from "ng-zorro-antd/icon";
import { NzCheckboxModule } from "ng-zorro-antd/checkbox";
import { NzMessageModule } from "ng-zorro-antd/message";
import { NzTypographyModule } from "ng-zorro-antd/typography";
// 引入你需要的图标，比如你需要 fill 主题的 AccountBook Alert 和 outline 主题的 Alert，推荐 ✔️
import {
  PauseOutline,
  CaretRightOutline,
  UserAddOutline,
} from "@ant-design/icons-angular/icons";
import { IconDefinition } from "@ant-design/icons-angular";

const icons: IconDefinition[] = [
  PauseOutline,
  CaretRightOutline,
  UserAddOutline,
];

import { AppComponent } from "./app.component";
import { CityComponent } from "./modal/city/city.component";
import { WayComponent } from "./modal/way/way.component";
import { PersonComponent } from "./modal/person/person.component";
import { AddPersonComponent } from "./modal/add-person/add-person.component";

declare global {
  interface Window {
    AMap: any;
    AMapUI: any;
  }
}

@NgModule({
  declarations: [
    AppComponent,
    CityComponent,
    WayComponent,
    PersonComponent,
    AddPersonComponent,
  ],
  imports: [
    BrowserModule,
    FormsModule,
    ReactiveFormsModule,
    CoreModule,
    BrowserAnimationsModule,

    NzLayoutModule,
    NzMenuModule,
    NzModalModule,
    NzTableModule,
    NzSelectModule,
    NzPopconfirmModule,
    NzTagModule,
    NzButtonModule,
    NzFormModule,
    NzInputModule,
    NzCheckboxModule,
    NzMessageModule,
    NzTypographyModule,
    NzIconModule.forRoot(icons),
  ],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
