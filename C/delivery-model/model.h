#pragma once
#include <cstdio>
#include <vector>
#include "order.h"
#include "point.h"
#include "rider.h"
#include "algorithm.h"
#include "constant.h"


//��¼���������״̬
enum ProgramStatus {
  GOOD,     //��������
  BREAK,    //�Ʋ�
  REVOKE,   //����ִ��
  FINISHED  //û���������͵Ķ���
};


//ģ���࣬���ڴ洢��������
//��ʼ���������ֵĳ�ʼλ��
//Example: 
//	Model m = Model({1,2})
class Model {
 public:
  //��ʼ��Model
  Model(Point init_position);

  //����µĶ���
  void AddOrder(Order o);
  void AddOrder(int n, int time, int x1, int y1, int x2, int y2);

  // ����һ����λʱ��
  // �ɵ����������1. ����ǰʱ��Ķ������������
  //							 2. ÿ��������ǰ�н�һ��
  void Step();

  //�����ѽӶ�����
  int num_all() const;

  //��������ɵĶ���
  int num_finished() const;

  //���س�ʱ�Ķ���
  int num_outdate() const;

  //�������е�Ǯ��
  int money() const;

  //���ص�ǰʱ��
  int time() const;

  //���ص�ǰ״̬
  ProgramStatus statu() const;

  //����
  std::vector<Rider> riders;

 private:
  //�������Ƹ������
  void AddRider();

  //����Ƿ��Ʋ�������Ʋ�������statu_
  void CheckIsBreak();

  //����Ƿ񱻵���ִ��,����ǣ�����statu_
  void CheckIsRevoke();

  //����Ƿ�û���������͵Ķ���������ǣ�����statu_
  void CheckIsFinished();

  //���ֳ�ʼλ��
  Point init_position_;

  //��ǰ״̬
  ProgramStatus statu_;

  //�ѽӶ�����
  int num_all_;

  //��ǰʱ��
  int time_;

  //����ӵĶ���
  std::queue<Order> waiting_order_;

};
