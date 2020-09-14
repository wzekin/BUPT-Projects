import { City, Way } from "./model.service";

// 一些初始资料
export const NameToLocation = {
  兰州: new window.AMap.LngLat(103.766496, 35.982602),
  西宁: new window.AMap.LngLat(101.701066, 36.549512),
  成都: new window.AMap.LngLat(104.030167, 30.548231),
  石家庄: new window.AMap.LngLat(114.477184, 37.969095),
  拉萨: new window.AMap.LngLat(91.154191, 29.635708),
  贵阳: new window.AMap.LngLat(106.53505, 26.613253),
  武汉: new window.AMap.LngLat(114.269425, 30.510377),
  郑州: new window.AMap.LngLat(113.610246, 34.692098),
  济南: new window.AMap.LngLat(117.169816, 36.584808),
  南京: new window.AMap.LngLat(118.839738, 32.012892),
  合肥: new window.AMap.LngLat(117.213761, 31.714308),
  杭州: new window.AMap.LngLat(120.245988, 30.131037),
  南昌: new window.AMap.LngLat(117.069958, 36.535012),
  福州: new window.AMap.LngLat(119.296125, 26.075806),
  广州: new window.AMap.LngLat(113.193758, 23.145944),
  长沙: new window.AMap.LngLat(112.930086, 28.23232),
  海口: new window.AMap.LngLat(110.183504, 19.999705),
  沈阳: new window.AMap.LngLat(123.442028, 41.614409),
  长春: new window.AMap.LngLat(125.287731, 43.778029),
  哈尔滨: new window.AMap.LngLat(126.562145, 45.835491),
  太原: new window.AMap.LngLat(112.556551, 37.844952),
  西安: new window.AMap.LngLat(108.90909, 34.366589),
  台湾: new window.AMap.LngLat(121.653231, 24.872012),
  北京: new window.AMap.LngLat(116.384686,39.907623),
  上海: new window.AMap.LngLat(121.564347, 31.151867),
  重庆: new window.AMap.LngLat(106.536043, 29.521395),
  天津: new window.AMap.LngLat(117.201794, 39.103416),
  内蒙古: new window.AMap.LngLat(111.727776,40.795637),
  宁夏: new window.AMap.LngLat(106.228426, 38.398098),
  新疆: new window.AMap.LngLat(87.551668, 43.727863),
  香港: new window.AMap.LngLat(114.181535, 22.172318),
  澳门: new window.AMap.LngLat(113.611239, 22.13198),
};

const CityInfo = [
  { key: 0, name: "兰州", risk_level: 0 },
  { key: 1, name: "西宁", risk_level: 1 },
  { key: 2, name: "成都", risk_level: 0 },
  { key: 3, name: "石家庄", risk_level: 2 },
  { key: 4, name: "拉萨", risk_level: 2 },
  { key: 5, name: "贵阳", risk_level: 2 },
  { key: 6, name: "武汉", risk_level: 0 },
  { key: 7, name: "郑州", risk_level: 0 },
  { key: 8, name: "济南", risk_level: 1 },
  { key: 9, name: "南京", risk_level: 2 },
  { key: 10, name: "合肥", risk_level: 0 },
  { key: 11, name: "杭州", risk_level: 2 },
  { key: 12, name: "南昌", risk_level: 0 },
  { key: 13, name: "福州", risk_level: 2 },
  { key: 14, name: "广州", risk_level: 1 },
  { key: 15, name: "长沙", risk_level: 2 },
  { key: 16, name: "海口", risk_level: 0 },
  { key: 17, name: "沈阳", risk_level: 1 },
  { key: 18, name: "长春", risk_level: 1 },
  { key: 19, name: "哈尔滨", risk_level: 0 },
  { key: 20, name: "太原", risk_level: 0 },
  { key: 21, name: "西安", risk_level: 1 },
  { key: 22, name: "台湾", risk_level: 0 },
  { key: 23, name: "北京", risk_level: 2 },
  { key: 24, name: "上海", risk_level: 2 },
  { key: 25, name: "重庆", risk_level: 0 },
  { key: 26, name: "天津", risk_level: 0 },
  { key: 27, name: "内蒙古", risk_level: 1 },
  { key: 28, name: "宁夏", risk_level: 2 },
  { key: 29, name: "新疆", risk_level: 1 },
  { key: 30, name: "香港", risk_level: 0 },
  { key: 31, name: "澳门", risk_level: 2 },
];

const WayInfo = [
  {
    id: 0,
    start_city: 25,
    end_city: 16,
    type: 2,
    start_time: 10,
    cost_time: 2,
  },
  { id: 1, start_city: 31, end_city: 25, type: 2, start_time: 3, cost_time: 2 },
  {
    id: 2,
    start_city: 27,
    end_city: 31,
    type: 2,
    start_time: 22,
    cost_time: 2,
  },
  { id: 3, start_city: 1, end_city: 16, type: 2, start_time: 17, cost_time: 4 },
  { id: 4, start_city: 28, end_city: 22, type: 2, start_time: 5, cost_time: 4 },
  { id: 5, start_city: 27, end_city: 3, type: 2, start_time: 11, cost_time: 2 },
  { id: 6, start_city: 4, end_city: 17, type: 2, start_time: 10, cost_time: 6 },
  {
    id: 7,
    start_city: 15,
    end_city: 20,
    type: 2,
    start_time: 22,
    cost_time: 2,
  },
  {
    id: 8,
    start_city: 11,
    end_city: 19,
    type: 2,
    start_time: 21,
    cost_time: 3,
  },
  { id: 9, start_city: 24, end_city: 1, type: 2, start_time: 22, cost_time: 4 },
  {
    id: 10,
    start_city: 2,
    end_city: 2,
    type: 1,
    start_time: 20,
    cost_time: 8,
  },
  { id: 11, start_city: 15, end_city: 1, type: 1, start_time: 2, cost_time: 5 },
  {
    id: 12,
    start_city: 31,
    end_city: 16,
    type: 1,
    start_time: 6,
    cost_time: 2,
  },
  {
    id: 13,
    start_city: 22,
    end_city: 11,
    type: 1,
    start_time: 16,
    cost_time: 2,
  },
  {
    id: 14,
    start_city: 15,
    end_city: 2,
    type: 1,
    start_time: 11,
    cost_time: 2,
  },
  {
    id: 15,
    start_city: 11,
    end_city: 27,
    type: 1,
    start_time: 5,
    cost_time: 5,
  },
  { id: 16, start_city: 9, end_city: 14, type: 1, start_time: 9, cost_time: 4 },
  {
    id: 17,
    start_city: 14,
    end_city: 3,
    type: 1,
    start_time: 22,
    cost_time: 5,
  },
  {
    id: 18,
    start_city: 17,
    end_city: 27,
    type: 1,
    start_time: 11,
    cost_time: 7,
  },
  { id: 19, start_city: 1, end_city: 6, type: 1, start_time: 5, cost_time: 5 },
  {
    id: 20,
    start_city: 16,
    end_city: 21,
    type: 1,
    start_time: 22,
    cost_time: 5,
  },
  {
    id: 21,
    start_city: 12,
    end_city: 7,
    type: 1,
    start_time: 21,
    cost_time: 2,
  },
  {
    id: 22,
    start_city: 19,
    end_city: 14,
    type: 1,
    start_time: 11,
    cost_time: 8,
  },
  { id: 23, start_city: 4, end_city: 13, type: 1, start_time: 7, cost_time: 9 },
  {
    id: 24,
    start_city: 27,
    end_city: 20,
    type: 1,
    start_time: 8,
    cost_time: 4,
  },
  { id: 25, start_city: 3, end_city: 8, type: 1, start_time: 6, cost_time: 1 },
  { id: 26, start_city: 7, end_city: 26, type: 1, start_time: 2, cost_time: 2 },
  {
    id: 27,
    start_city: 30,
    end_city: 9,
    type: 1,
    start_time: 23,
    cost_time: 4,
  },
  {
    id: 28,
    start_city: 25,
    end_city: 23,
    type: 1,
    start_time: 13,
    cost_time: 7,
  },
  {
    id: 29,
    start_city: 25,
    end_city: 9,
    type: 1,
    start_time: 16,
    cost_time: 4,
  },
  {
    id: 30,
    start_city: 13,
    end_city: 27,
    type: 1,
    start_time: 17,
    cost_time: 4,
  },
  {
    id: 31,
    start_city: 21,
    end_city: 0,
    type: 1,
    start_time: 15,
    cost_time: 2,
  },
  {
    id: 32,
    start_city: 19,
    end_city: 30,
    type: 1,
    start_time: 3,
    cost_time: 9,
  },
  {
    id: 33,
    start_city: 20,
    end_city: 13,
    type: 1,
    start_time: 8,
    cost_time: 5,
  },
  {
    id: 34,
    start_city: 18,
    end_city: 24,
    type: 1,
    start_time: 4,
    cost_time: 4,
  },
  {
    id: 35,
    start_city: 10,
    end_city: 26,
    type: 1,
    start_time: 22,
    cost_time: 3,
  },
  {
    id: 36,
    start_city: 23,
    end_city: 4,
    type: 1,
    start_time: 15,
    cost_time: 11,
  },
  {
    id: 37,
    start_city: 14,
    end_city: 29,
    type: 1,
    start_time: 3,
    cost_time: 10,
  },
  {
    id: 38,
    start_city: 18,
    end_city: 0,
    type: 1,
    start_time: 19,
    cost_time: 7,
  },
  {
    id: 39,
    start_city: 28,
    end_city: 15,
    type: 1,
    start_time: 6,
    cost_time: 4,
  },
  {
    id: 40,
    start_city: 28,
    end_city: 9,
    type: 0,
    start_time: 17,
    cost_time: 11,
  },
  {
    id: 41,
    start_city: 11,
    end_city: 31,
    type: 0,
    start_time: 11,
    cost_time: 8,
  },
  {
    id: 42,
    start_city: 17,
    end_city: 26,
    type: 0,
    start_time: 14,
    cost_time: 6,
  },
  {
    id: 43,
    start_city: 23,
    end_city: 8,
    type: 0,
    start_time: 22,
    cost_time: 7,
  },
  {
    id: 44,
    start_city: 4,
    end_city: 10,
    type: 0,
    start_time: 10,
    cost_time: 20,
  },
  {
    id: 45,
    start_city: 20,
    end_city: 30,
    type: 0,
    start_time: 17,
    cost_time: 12,
  },
  {
    id: 46,
    start_city: 11,
    end_city: 25,
    type: 0,
    start_time: 18,
    cost_time: 11,
  },
  { id: 47, start_city: 4, end_city: 1, type: 0, start_time: 3, cost_time: 10 },
  {
    id: 48,
    start_city: 27,
    end_city: 14,
    type: 0,
    start_time: 14,
    cost_time: 7,
  },
  {
    id: 49,
    start_city: 22,
    end_city: 30,
    type: 0,
    start_time: 23,
    cost_time: 6,
  },
  {
    id: 50,
    start_city: 26,
    end_city: 11,
    type: 0,
    start_time: 0,
    cost_time: 8,
  },
  {
    id: 51,
    start_city: 16,
    end_city: 5,
    type: 0,
    start_time: 20,
    cost_time: 6,
  },
  { id: 52, start_city: 0, end_city: 1, type: 0, start_time: 21, cost_time: 2 },
  {
    id: 53,
    start_city: 16,
    end_city: 6,
    type: 0,
    start_time: 20,
    cost_time: 9,
  },
  {
    id: 54,
    start_city: 10,
    end_city: 25,
    type: 0,
    start_time: 7,
    cost_time: 9,
  },
  {
    id: 55,
    start_city: 9,
    end_city: 25,
    type: 0,
    start_time: 19,
    cost_time: 10,
  },
  {
    id: 56,
    start_city: 8,
    end_city: 27,
    type: 0,
    start_time: 0,
    cost_time: 10,
  },
  {
    id: 57,
    start_city: 4,
    end_city: 15,
    type: 0,
    start_time: 0,
    cost_time: 17,
  },
  {
    id: 58,
    start_city: 16,
    end_city: 17,
    type: 0,
    start_time: 16,
    cost_time: 20,
  },
  {
    id: 59,
    start_city: 29,
    end_city: 30,
    type: 0,
    start_time: 14,
    cost_time: 26,
  },
  {
    id: 60,
    start_city: 7,
    end_city: 22,
    type: 0,
    start_time: 11,
    cost_time: 10,
  },
  {
    id: 61,
    start_city: 15,
    end_city: 21,
    type: 0,
    start_time: 7,
    cost_time: 6,
  },
  {
    id: 62,
    start_city: 17,
    end_city: 27,
    type: 0,
    start_time: 1,
    cost_time: 16,
  },
  { id: 63, start_city: 9, end_city: 8, type: 0, start_time: 10, cost_time: 4 },
  {
    id: 64,
    start_city: 1,
    end_city: 9,
    type: 0,
    start_time: 15,
    cost_time: 14,
  },
  {
    id: 65,
    start_city: 18,
    end_city: 11,
    type: 0,
    start_time: 22,
    cost_time: 11,
  },
  {
    id: 66,
    start_city: 6,
    end_city: 20,
    type: 0,
    start_time: 16,
    cost_time: 6,
  },
  {
    id: 67,
    start_city: 20,
    end_city: 23,
    type: 0,
    start_time: 7,
    cost_time: 9,
  },
  {
    id: 68,
    start_city: 16,
    end_city: 20,
    type: 0,
    start_time: 13,
    cost_time: 14,
  },
  {
    id: 69,
    start_city: 13,
    end_city: 17,
    type: 0,
    start_time: 20,
    cost_time: 13,
  },
  {
    id: 70,
    start_city: 17,
    end_city: 23,
    type: 0,
    start_time: 11,
    cost_time: 1,
  },
  {
    id: 71,
    start_city: 4,
    end_city: 29,
    type: 0,
    start_time: 11,
    cost_time: 11,
  },
  {
    id: 72,
    start_city: 22,
    end_city: 15,
    type: 0,
    start_time: 22,
    cost_time: 8,
  },
  {
    id: 73,
    start_city: 22,
    end_city: 6,
    type: 0,
    start_time: 22,
    cost_time: 7,
  },
  {
    id: 74,
    start_city: 12,
    end_city: 3,
    type: 0,
    start_time: 13,
    cost_time: 3,
  },
  { id: 75, start_city: 1, end_city: 25, type: 0, start_time: 9, cost_time: 7 },
  {
    id: 76,
    start_city: 27,
    end_city: 17,
    type: 0,
    start_time: 12,
    cost_time: 16,
  },
  {
    id: 77,
    start_city: 1,
    end_city: 6,
    type: 0,
    start_time: 22,
    cost_time: 11,
  },
  {
    id: 78,
    start_city: 22,
    end_city: 12,
    type: 0,
    start_time: 19,
    cost_time: 10,
  },
  {
    id: 79,
    start_city: 4,
    end_city: 10,
    type: 0,
    start_time: 23,
    cost_time: 20,
  },
  { id: 80, start_city: 8, end_city: 3, type: 0, start_time: 8, cost_time: 3 },
  {
    id: 81,
    start_city: 8,
    end_city: 29,
    type: 0,
    start_time: 6,
    cost_time: 23,
  },
  {
    id: 82,
    start_city: 17,
    end_city: 18,
    type: 0,
    start_time: 9,
    cost_time: 3,
  },
  {
    id: 83,
    start_city: 30,
    end_city: 15,
    type: 0,
    start_time: 0,
    cost_time: 5,
  },
  {
    id: 84,
    start_city: 9,
    end_city: 18,
    type: 0,
    start_time: 2,
    cost_time: 11,
  },
  {
    id: 85,
    start_city: 19,
    end_city: 2,
    type: 0,
    start_time: 14,
    cost_time: 21,
  },
  {
    id: 86,
    start_city: 17,
    end_city: 29,
    type: 0,
    start_time: 7,
    cost_time: 27,
  },
  {
    id: 87,
    start_city: 18,
    end_city: 24,
    type: 0,
    start_time: 8,
    cost_time: 10,
  },
  {
    id: 88,
    start_city: 17,
    end_city: 4,
    type: 0,
    start_time: 7,
    cost_time: 26,
  },
  {
    id: 89,
    start_city: 15,
    end_city: 6,
    type: 0,
    start_time: 22,
    cost_time: 2,
  },
  {
    id: 90,
    start_city: 31,
    end_city: 27,
    type: 0,
    start_time: 13,
    cost_time: 8,
  },
  {
    id: 91,
    start_city: 3,
    end_city: 18,
    type: 0,
    start_time: 19,
    cost_time: 10,
  },
  {
    id: 92,
    start_city: 27,
    end_city: 6,
    type: 0,
    start_time: 18,
    cost_time: 6,
  },
  {
    id: 93,
    start_city: 10,
    end_city: 23,
    type: 0,
    start_time: 13,
    cost_time: 9,
  },
  {
    id: 94,
    start_city: 22,
    end_city: 11,
    type: 0,
    start_time: 17,
    cost_time: 5,
  },
  {
    id: 95,
    start_city: 5,
    end_city: 28,
    type: 0,
    start_time: 17,
    cost_time: 9,
  },
  {
    id: 96,
    start_city: 12,
    end_city: 25,
    type: 0,
    start_time: 20,
    cost_time: 10,
  },
  {
    id: 97,
    start_city: 24,
    end_city: 7,
    type: 0,
    start_time: 11,
    cost_time: 7,
  },
  {
    id: 98,
    start_city: 23,
    end_city: 13,
    type: 0,
    start_time: 23,
    cost_time: 13,
  },
  {
    id: 99,
    start_city: 4,
    end_city: 26,
    type: 0,
    start_time: 12,
    cost_time: 21,
  },
];

export function getCityInfo(): City[] {
  let data = localStorage.getItem("cityInfo");
  if (data === null) {
    return CityInfo;
  } else {
    return JSON.parse(data) as City[];
  }
}

export function setCityInfo(data: City[]) {
  localStorage.setItem("cityInfo", JSON.stringify(data));
}

export function getWayInfo(): Way[] {
  let data = localStorage.getItem("wayInfo");
  if (data === null) {
    // generateWayInfo();
    // console.log(JSON.stringify(WayInfo));
    return WayInfo;
  } else {
    return JSON.parse(data) as Way[];
  }
}

export function setWayInfo(data: Way[]) {
  localStorage.setItem("wayInfo", JSON.stringify(data));
}

//function getRandomArbitrary(min, max) {
  //return Math.floor(Math.random() * (max - min) + min);
//}

//function generateWayInfo() {
  //let count = 0;
  //for (let i = 0; i < 10; i++) {
    //let start_city = getRandomArbitrary(0, CityInfo.length);
    //let end_city = getRandomArbitrary(0, CityInfo.length);
    //if (start_city === end_city) {
      //end_city = (end_city + 1) % CityInfo.length;
    //}
    //let start_time = getRandomArbitrary(0, 24);
    //let start_city_location = NameToLocation[CityInfo[start_city].name];
    //let end_city_location = NameToLocation[CityInfo[end_city].name];
    //let distance = Math.sqrt(
      //Math.pow(start_city_location.lng - end_city_location.lng, 2) +
        //Math.pow(start_city_location.lat - end_city_location.lat, 2)
    //);
    //let cost_time = Math.floor((distance / 9) * 1.5) + 1;
    //WayInfo.push({
      //id: count++,
      //start_city,
      //end_city,
      //type: 0,
      //start_time,
      //cost_time,
    //});
  //}
  //for (let i = 0; i < 30; i++) {
    //let start_city = getRandomArbitrary(0, CityInfo.length);
    //let end_city = getRandomArbitrary(0, CityInfo.length);
    //if (start_city === end_city) {
      //end_city = (end_city + 1) % CityInfo.length;
    //}
    //let start_time = getRandomArbitrary(0, 24);
    //let start_city_location = NameToLocation[CityInfo[start_city].name];
    //let end_city_location = NameToLocation[CityInfo[end_city].name];
    //let distance = Math.sqrt(
      //Math.pow(start_city_location.lng - end_city_location.lng, 2) +
        //Math.pow(start_city_location.lat - end_city_location.lat, 2)
    //);
    //let cost_time = Math.floor((distance / 5) * 1.5) + 1;
    //WayInfo.push({
      //id: count++,
      //start_city,
      //end_city,
      //type: 1,
      //start_time,
      //cost_time,
    //});
  //}
  //for (let i = 0; i < 60; i++) {
    //let start_city = getRandomArbitrary(0, CityInfo.length);
    //let end_city = getRandomArbitrary(0, CityInfo.length);
    //if (start_city === end_city) {
      //end_city = (end_city + 1) % CityInfo.length;
    //}
    //let start_time = getRandomArbitrary(0, 24);
    //let start_city_location = NameToLocation[CityInfo[start_city].name];
    //let end_city_location = NameToLocation[CityInfo[end_city].name];
    //let distance = Math.sqrt(
      //Math.pow(start_city_location.lng - end_city_location.lng, 2) +
        //Math.pow(start_city_location.lat - end_city_location.lat, 2)
    //);
    //let cost_time = Math.floor((distance / 2) * 1.5) + 1;
    //WayInfo.push({
      //id: count++,
      //start_city,
      //end_city,
      //type: 0,
      //start_time,
      //cost_time,
    //});
  //}
//}
