#pragma once
#include<cmath>
enum PointType {
	FROM, //餐馆
	TO, //食客
};
//数据类Point
class Point {
 public:
  // x坐标
  int x;

  // y坐标
  int y;

  //点的种类
  PointType type;

  //所对应的订单id
  unsigned order_id;

  Point();
  Point(int x, int y);
  Point(int x, int y, PointType type, unsigned order_id);
  ~Point();

  //两个点的曼哈顿距离
	//输入
	//		Point from 目标点1
	//		Point to 目标点2
	//输出
	//		int 曼哈顿距离
  static int GetDistant(const Point& from, const Point& to);

	//判断是否到达
	//输入
	//		Point from 当前位置
	//    Point to 目标位置
	//输出
	//		是否到达
  static bool IsArrive(const Point& from, const Point& to);

	//骑手下一次行走的位置
	//输入
	//    Point from 当前位置
	//    Point to 目标位置
	//输出
	//    Point  移动到下一个点的坐标
  static Point Move(const Point& from, const Point& to);

	//操作符重载
  bool operator==(const Point& p) const;
  bool operator<(const Point& p) const;
  bool operator>(const Point& p) const;
};
