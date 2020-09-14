import { Injectable } from "@angular/core";

// If you import a module but never use any of the imported values other than as TypeScript types,
// the resulting javascript file will look as if you never imported the module at all.
import { ipcRenderer, webFrame, remote } from "electron";
import * as childProcess from "child_process";
import * as fs from "fs";

@Injectable({
  providedIn: "root",
})
export class ElectronService {
  ipcRenderer: typeof ipcRenderer;
  webFrame: typeof webFrame;
  remote: typeof remote;
  shell: any;
  childProcess: typeof childProcess;
  fs: typeof fs;
  log_file: string = "log.txt";

  get isElectron(): boolean {
    return !!(window && window.process && window.process.type);
  }

  constructor() {
    if (this.isElectron) {
      this.ipcRenderer = window.require("electron").ipcRenderer;
      this.webFrame = window.require("electron").webFrame;
      this.remote = window.require("electron").remote;
      this.shell = window.require("electron").shell;
      this.childProcess = window.require("child_process");
      this.fs = window.require("fs");
    }
  }

  // 在electron环境下写入文件， 否则打印到console
  writeFile(data: string) {
    const writeData = new Date().toISOString() + " " + data + "\n";
    if (this.isElectron) {
      this.fs.writeFileSync(this.log_file, writeData, { flag: "a" });
    } else {
      console.log(writeData);
    }
  }

  // 在electron 环境下打开日志文件
  openLog() {
    if (this.isElectron) {
      this.shell.openItem(this.log_file);
    }
  }

  // 在electron环境下关闭系统
  close() {
    if (this.isElectron) {
      this.writeFile("关闭系统");
      this.remote.getCurrentWindow().close();
    }
  }
}
