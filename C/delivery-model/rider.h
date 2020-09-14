#pragma once
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <queue>
#include <set>
#include "order.h"
#include "point.h"
#include "constant.h"

//骑手类
//用来处理骑手的动作和信息
class Rider {
 public:
  //构造函数
  //传入骑手的初始位置
  Rider(Point position);
  ~Rider();

  //骑手运行 一单位时间
  //	1. 行走
  //  2. 如果到达一个地方，则记录,更新订单数
  void Step(unsigned time);

  // 添加一个订单
  void AddOrder(Order r);

  //更新骑手的总花费和路径
  void ChangePath(std::queue<Point> path, int all_cost);

  //返回骑手的位置信息
  Point position() const;

  //返回非法（运送时间超过60）订单数
  int illegal_orders() const;

  //返回超时（运送时间超过30）订单
  const std::vector<Order> &outdate_orders() const;

  //返回已完成订单
  const std::vector<Order> &finished_orders() const;

  //返回超时（运送时间超过30）订单编号
  const std::vector<int> &outdate_orders_now() const;

  //返回已完成订单编号
  const std::vector<int> &finished_orders_now() const;

  //返回已接单订单
  const std::set<Order> &received_orders() const;

  //返回运送中订单
  const std::set<Order> &sending_orders() const;

  //返回停靠的点
  const std::vector<Point> &dock_points() const;

  //返回总花费
  int all_cost() const;

 private:
  //骑手路径
  std::queue<Point> path_;

  //骑手位置
  Point position_;

  //非法（运送时间超过60）订单数
  int illegal_orders_;

  //超时（运送时间超过30）订单
  std::vector<Order> outdate_orders_;

  //已完成订单
  std::vector<Order> finished_orders_;

  //当前时间超时（运送时间超过30）订单编号
  std::vector<int> outdate_orders_now_;

  //当前时间完成订单编号
  std::vector<int> finished_orders_now_;

  //已接单订单
  std::set<Order> received_orders_;

  //运送中订单
  std::set<Order> sending_orders_;

  //当前时间停靠的点
  std::vector<Point> dock_points_;

  //花费总时间
  int all_cost_;

};
