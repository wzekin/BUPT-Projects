#include "algorithm.h"

const static int INTMAX = INT_MAX / 2;
std::tuple<std::queue<Point>, int> Algorithm::cal_solution(
    const Rider &r, const Order &new_order, const int time) {

  int sending_num = r.sending_orders().size();					//正在派送的订单
  int received_num = r.received_orders().size() + 1;    //已接单，但还没派送的订单 (加入新order计算）
  int n = sending_num + received_num * 2;             //所有要经过的点的数量
  Point *P = new Point[n];                            //所有的点
  int *last_time = new int[n];                        //所有的点在不超时到达的情况下的最后时刻
  int **is_outdate = new int *[n];                    //状态是否已经有订单超时
  int **G = new int *[n];                             //各个点距离 n*n二维矩阵
  int **from = new int *[n];                          //当前状态的最后一个到达的点 n*(2^n)二维矩阵
  int **dp = new int *[n];                            //记录当前状态花费的时间
  int index_from = 2;                                 // 标记

  //初始化数组
  for (int i = 0; i < n; i++) {
    last_time[i] = INTMAX;
    G[i] = new int[n];
    is_outdate[i] = new int[(long long)1 << n];
    from[i] = new int[(long long)1 << n];
    dp[i] = new int[(long long)1 << n];
    for (int j = 0; j < n; j++) {
      G[i][j] = INTMAX;
    }
    for (int j = 0; j < ((long long)1 << n); j++) {
      is_outdate[i][j] = 0;
      from[i][j] = -1;
      dp[i][j] = INTMAX;
    }
  }

  //初始化P
  P[0] = new_order.restaurant;  //给新增加的订单单独赋值
  P[1] = new_order.dinner;
  last_time[1] =
      OUTDATE_TIME * 2 - 2;  // -2 以忽略曼哈顿距离估算带来的误差，下面同上
  for (auto it_from = r.received_orders().begin();
       it_from != r.received_orders().end(); it_from++) {
    P[index_from] = (*it_from).restaurant;
    P[index_from + 1] = (*it_from).dinner;
    last_time[index_from + 1] =
        (OUTDATE_TIME - (time - (*it_from).time)) * 2 - 2;  //乘2以翻译成cost 减2以平衡估算带来的误差
    index_from += 2;
  }
  for (auto it_from = r.sending_orders().begin();
       it_from != r.sending_orders().end(); it_from++) {
    P[index_from] = (*it_from).dinner;
    last_time[index_from] = (OUTDATE_TIME - (time - (*it_from).time)) * 2 - 2; // 减2以平衡估算带来的误差
    index_from += 1;
  }

  //给G和dp赋值
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      G[i][j] = G[j][i] = Point::GetDistant(P[i], P[j]);
    }
    dp[i][1 << i] = Point::GetDistant(r.position(), P[i]);
  }

  //列dp方程;
  for (int S = 0; S < (1 << n); S++)
    for (int i = 0; i < n; ++i)
      if (S & (1 << i) && Check(S, i, received_num))  //判断此状态是否合法
        for (int j = 0; j < n; ++j)
          if (!(S & (1 << j)) &&
              G[i][j] != INTMAX) {  // 判断是否到过j 且i j 是否可达
            if (dp[j][S | (1 << j)] > dp[i][S] + G[i][j]) {  //判断是否需要更新
              if (dp[i][S] + G[i][j] > last_time[j] ||
                  is_outdate[i][S]) {  //判断此状态是否超时
                if (dp[j][S | (1 << j)] == INTMAX ||
                    is_outdate[j][S | (1 << j)] ==
                        1) {  // 判断已超时状态是否可以更新
                  dp[j][S | (1 << j)] = dp[i][S] + G[i][j];
                  from[j][S | (1 << j)] = i;
                  is_outdate[j][S | (1 << j)] = 1;
                }
              } else {
                dp[j][S | (1 << j)] = dp[i][S] + G[i][j];
                from[j][S | (1 << j)] = i;
                is_outdate[j][S | (1 << j)] = 0;
              }
            }
          }

  //找出cost最小且不会超时的路径
  int index = 0, all_cost = INTMAX, is_out = 1;
  for (int i = 0; i < n; i++) {
    if (dp[i][(1 << n) - 1] < all_cost &&
        !(is_outdate[i][(1 << n) - 1] == 1 && is_out == 0)) {
      index = i;
      is_out = is_outdate[i][(1 << n) - 1];
      all_cost = dp[i][(1 << n) - 1];
    } else if (is_outdate[i][(1 << n) - 1] == 0 && is_out == 1) {
      index = i;
      is_out = is_outdate[i][(1 << n) - 1];
      all_cost = dp[i][(1 << n) - 1];
    }
  }

  //拿到路径
  std::queue<Point> path;
  GetPath(index, (1 << n) - 1, from, P, path);

  //释放资源
  for (int i = 0; i < n; i++) {
    delete[] is_outdate[i];
    delete[] G[i];
    delete[] from[i];
    delete[] dp[i];
  }
  delete[] last_time;
  delete[] is_outdate;
  delete[] G;
  delete[] from;
  delete[] dp;
  return {path, all_cost};
}

bool Algorithm::Check(int S, int i, int received_num) {
  if (i < 2 * received_num && i % 2 == 1 && !(S & (1 << (i - 1)))) return false;
  return true;
}

void Algorithm::GetPath(int end, int S, int **from, Point P[],
                        std::queue<Point> &path) {
  if (end != -1) {
    GetPath(from[end][S], S ^ 1 << end, from, P, path);
    path.push(P[end]);
  }
}
