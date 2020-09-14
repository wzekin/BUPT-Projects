#pragma once
#include <cstdio>
#include <vector>
#include "order.h"
#include "point.h"
#include "rider.h"
#include "algorithm.h"
#include "constant.h"


//记录程序的运行状态
enum ProgramStatus {
  GOOD,     //正常运行
  BREAK,    //破产
  REVOKE,   //吊销执照
  FINISHED  //没有正在派送的订单
};


//模型类，用于存储整个数据
//初始化传入骑手的初始位置
//Example: 
//	Model m = Model({1,2})
class Model {
 public:
  //初始化Model
  Model(Point init_position);

  //添加新的订单
  void AddOrder(Order o);
  void AddOrder(int n, int time, int x1, int y1, int x2, int y2);

  // 行走一个单位时间
  // 干的事情包含：1. 将当前时间的订单分配给骑手
  //							 2. 每个骑手向前行进一格
  void Step();

  //返回已接订单数
  int num_all() const;

  //返回已完成的订单
  int num_finished() const;

  //返回超时的订单
  int num_outdate() const;

  //返回现有的钱数
  int money() const;

  //返回当前时间
  int time() const;

  //返回当前状态
  ProgramStatus statu() const;

  //骑手
  std::vector<Rider> riders;

 private:
  //添加新招聘的骑手
  void AddRider();

  //检查是否破产，如果破产，则标记statu_
  void CheckIsBreak();

  //检查是否被吊销执照,如果是，则标记statu_
  void CheckIsRevoke();

  //检查是否没有正在派送的订单，如果是，则标记statu_
  void CheckIsFinished();

  //骑手初始位置
  Point init_position_;

  //当前状态
  ProgramStatus statu_;

  //已接订单数
  int num_all_;

  //当前时间
  int time_;

  //待添加的订单
  std::queue<Order> waiting_order_;

};
