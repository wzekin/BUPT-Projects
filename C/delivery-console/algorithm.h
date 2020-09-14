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


// 算法类，主要实现了状压dp，来解每一个分问题
class Algorithm {
 public:
  //计算出骑手派送订单的具体路程和具体花费
  //这里的花费是由每一个点的曼哈顿距离计算而来，由于骑手的速度为2
  // 所以，一般来说，花费的总时间应该为cost/2
  // 这是一个估计值
  //输入:
  //		Rider r 派送订单的骑手
  //    Order new_order 新加入的订单
  //		time 当前时间
  //输出:
  //		std::queue<Point> path 具体的路程，用队列存储
  //		int cost 总的花费
  //注意：
  //		状压dp的算法复杂度为O(N^2*2^N),注意限制骑手的订单数，防止超时；
  static std::tuple<std::queue<Point>, int> cal_solution(
      const Rider &r, const Order &new_order, const int time);

 private:
  //检查这个状态是否合法，即是否此订单餐厅的到达在食客之前
  //输入：
  //	 int S 当前状态；
  //	 int i 下一个到达的位置
  //	 int received_num 此骑手接单的订单数
  //输出：
  //		bool 是否合法
  static bool Check(int S, int i, int received_num);

  //由from矩阵递归得到路径
  //输入：
  // int end 当前状态最后一个到达的点
  // int S 当前状态
  // int **from 矩阵
  // Point P[] 所有的要到达的点
  // std::queue<Point> &path 存储路径的队列
  static void GetPath(int end, int S, int **from, Point P[],
                      std::queue<Point> &path);
};

const static int INTMAX = INT_MAX / 2;
