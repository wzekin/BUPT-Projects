#pragma once
#include <cstdio>
#include <vector>
#include "point.h"

//订单类 用来记录订单数据
class Order {
 public:
  //订单id
  const unsigned id;

  //餐馆
  const Point restaurant;

  //食客
  const Point dinner;

  //下单时间
  const unsigned time;

  Order();
  Order(const unsigned id);
  Order(const unsigned id, const Point restaurant, const Point dinner,
        const unsigned time);
  ~Order();

	//操作符重载
  bool operator==(const Order& b) const;
  bool operator<(const Order& b) const;
  Order operator=(const Order& b);
};
