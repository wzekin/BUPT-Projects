// 一个简单的计时器,用于记录时间
export class MiniDaemon {
  owner = null;
  task = null;
  rate = 100;
  length = Infinity;

  SESSION: NodeJS.Timeout;
  INDEX = 0;
  PAUSED = true;
  BACKW = true;

  constructor(oOwner, fTask, nRate?, nLen?) {
    if (!(this && this instanceof MiniDaemon)) {
      return;
    }
    if (arguments.length < 2) {
      throw new TypeError("MiniDaemon - not enough arguments");
    }
    if (oOwner) {
      this.owner = oOwner;
    }
    this.task = fTask;
    if (isFinite(nRate) && nRate > 0) {
      this.rate = Math.floor(nRate);
    }
    if (nLen > 0) {
      this.length = Math.floor(nLen);
    }
  }

  static forceCall(oDmn: MiniDaemon) {
    oDmn.INDEX += oDmn.BACKW ? -1 : 1;
    if (
      oDmn.task.call(oDmn.owner, oDmn.INDEX, oDmn.length, oDmn.BACKW) ===
        false ||
      oDmn.isAtEnd()
    ) {
      oDmn.pause();
      return false;
    }
    return true;
  }

  isAtEnd() {
    return this.BACKW
      ? isFinite(this.length) && this.INDEX < 1
      : this.INDEX + 1 > this.length;
  }

  pause() {
    clearInterval(this.SESSION);
    this.PAUSED = true;
  }

  synchronize() {
    if (this.PAUSED) {
      return;
    }
    clearInterval(this.SESSION);
    this.SESSION = setInterval(MiniDaemon.forceCall, this.rate, this);
  }

  start(bReverse?: boolean) {
    var bBackw = Boolean(bReverse);
    if (this.BACKW === bBackw && (this.isAtEnd() || !this.PAUSED)) {
      return;
    }
    this.BACKW = bBackw;
    this.PAUSED = false;
    this.synchronize();
  }
}

// 将person当前状态转换为文本
export function calulate_statu(person): string {
  let statu_value = person.statu_value;
  if (person.statu === 2) {
    return `${statu_value.start_city.name} -> ${statu_value.end_city.name}`;
  } else {
    return `${statu_value.name}`;
  }
}
