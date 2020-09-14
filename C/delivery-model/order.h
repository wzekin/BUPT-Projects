#pragma once
#include <cstdio>
#include <vector>
#include "point.h"

//������ ������¼��������
class Order {
 public:
  //����id
  const unsigned id;

  //�͹�
  const Point restaurant;

  //ʳ��
  const Point dinner;

  //�µ�ʱ��
  const unsigned time;

  Order();
  Order(const unsigned id);
  Order(const unsigned id, const Point restaurant, const Point dinner,
        const unsigned time);
  ~Order();

	//����������
  bool operator==(const Order& b) const;
  bool operator<(const Order& b) const;
  Order operator=(const Order& b);
};
