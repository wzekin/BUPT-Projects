#pragma once
#include <set>
#include<queue>
#include<climits>
#include<cmath>
#include<tuple>
#include<algorithm>
#include "point.h"
#include "order.h"
#include "rider.h"
#include "constant.h"


// �㷨�࣬��Ҫʵ����״ѹdp������ÿһ��������
class Algorithm {
 public:
  //������������Ͷ����ľ���·�̺;��廨��
  //����Ļ�������ÿһ����������پ������������������ֵ��ٶ�Ϊ2
  // ���ԣ�һ����˵�����ѵ���ʱ��Ӧ��Ϊcost/2
  // ����һ������ֵ
  //����:
  //		Rider r ���Ͷ���������
  //    Order new_order �¼���Ķ���
  //		time ��ǰʱ��
  //���:
  //		std::queue<Point> path �����·�̣��ö��д洢
  //		int cost �ܵĻ���
  //ע�⣺
  //		״ѹdp���㷨���Ӷ�ΪO(N^2*2^N),ע���������ֵĶ���������ֹ��ʱ��
  static std::tuple<std::queue<Point>, int> cal_solution(
      const Rider &r, const Order &new_order, const int time);

 private:
  //������״̬�Ƿ�Ϸ������Ƿ�˶��������ĵ�����ʳ��֮ǰ
  //���룺
  //	 int S ��ǰ״̬��
  //	 int i ��һ�������λ��
  //	 int received_num �����ֽӵ��Ķ�����
  //�����
  //		bool �Ƿ�Ϸ�
  static bool Check(int S, int i, int received_num);

  //��from����ݹ�õ�·��
  //���룺
  // int end ��ǰ״̬���һ������ĵ�
  // int S ��ǰ״̬
  // int **from ����
  // Point P[] ���е�Ҫ����ĵ�
  // std::queue<Point> &path �洢·���Ķ���
  static void GetPath(int end, int S, int **from, Point P[],
                      std::queue<Point> &path);
};

const static int INTMAX = INT_MAX / 2;
