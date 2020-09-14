#pragma once
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <queue>
#include <set>
#include "order.h"
#include "point.h"
#include "constant.h"

//������
//�����������ֵĶ�������Ϣ
class Rider {
 public:
  //���캯��
  //�������ֵĳ�ʼλ��
  Rider(Point position);
  ~Rider();

  //�������� һ��λʱ��
  //	1. ����
  //  2. �������һ���ط������¼,���¶�����
  void Step(unsigned time);

  // ���һ������
  void AddOrder(Order r);

  //�������ֵ��ܻ��Ѻ�·��
  void ChangePath(std::queue<Point> path, int all_cost);

  //�������ֵ�λ����Ϣ
  Point position() const;

  //���طǷ�������ʱ�䳬��60��������
  int illegal_orders() const;

  //���س�ʱ������ʱ�䳬��30������
  const std::vector<Order> &outdate_orders() const;

  //��������ɶ���
  const std::vector<Order> &finished_orders() const;

  //���س�ʱ������ʱ�䳬��30���������
  const std::vector<int> &outdate_orders_now() const;

  //��������ɶ������
  const std::vector<int> &finished_orders_now() const;

  //�����ѽӵ�����
  const std::set<Order> &received_orders() const;

  //���������ж���
  const std::set<Order> &sending_orders() const;

  //����ͣ���ĵ�
  const std::vector<Point> &dock_points() const;

  //�����ܻ���
  int all_cost() const;

 private:
  //����·��
  std::queue<Point> path_;

  //����λ��
  Point position_;

  //�Ƿ�������ʱ�䳬��60��������
  int illegal_orders_;

  //��ʱ������ʱ�䳬��30������
  std::vector<Order> outdate_orders_;

  //����ɶ���
  std::vector<Order> finished_orders_;

  //��ǰʱ�䳬ʱ������ʱ�䳬��30���������
  std::vector<int> outdate_orders_now_;

  //��ǰʱ����ɶ������
  std::vector<int> finished_orders_now_;

  //�ѽӵ�����
  std::set<Order> received_orders_;

  //�����ж���
  std::set<Order> sending_orders_;

  //��ǰʱ��ͣ���ĵ�
  std::vector<Point> dock_points_;

  //������ʱ��
  int all_cost_;

};
