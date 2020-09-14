#include "model.h"

Model::Model(Point init_position)
    : init_position_(init_position), statu_(GOOD),time_(0),num_all_(0) {
}



void Model::CheckIsBreak() {
  if (money() < 0) {  //金钱数小于零时破产
    statu_ = BREAK;
  }
}

void Model::CheckIsRevoke() {
  for (int i = 0; i < riders.size(); i++)
    if (riders[i].illegal_orders() != 0) { // 检查是否有大于60个单位时间派送的订单
      statu_ = REVOKE;
      return;
    }
}

void Model::CheckIsFinished() {
  int flag = 0;  //标记当前状态的变量，当flag==0时，接单和派单数都为零
  for (int i = 0; i < riders.size(); i++) { //检查当前是否所有订单都已完成
    if (riders[i].received_orders().size() != 0 ||
        riders[i].sending_orders().size() != 0) {
      flag = 1;
    }
  }
  if (flag == 0) {
    statu_ = FINISHED;
  }
}

void Model::AddOrder(Order o) {
  num_all_++;
  waiting_order_.push(o); //加入到等待派单的队列里
}

void Model::AddOrder(int n, int time, int x1, int y1, int x2, int y2) {
  Order tempOrder =
      Order(n, Point(x1, y1, FROM, n), Point(x2, y2, TO, n), time);
  AddOrder(tempOrder);
}

void Model::Step() {
  statu_ = GOOD;  // 初始化statu_

  while (!waiting_order_.empty()) {  //添加所有的未派发订单
    std::queue<Point> path;
    int index = -1, min = INT_MAX, cost = 100, is_outdate = 1;
    for (auto i = 0; i < riders.size(); i++) {
      // 检查是否超过一个骑手的接单数
      if (riders[i].sending_orders().size() +
              riders[i].received_orders().size() * 2 >
          15 && money() >= 300) {
        continue;
      }

      //如果骑手添加订单，所花费的cost 和 所走路径，并找出路径改变量最小的骑手
      auto [path_tmp, cost_tmp] = Algorithm::cal_solution(
          riders[i], waiting_order_.front(), time_);  // c++17 标准
      if (min == INT_MAX || (cost > 2 * OUTDATE_TIME && cost_tmp < cost)) {
        path = path_tmp, cost = cost_tmp, index = i,
        min = cost - riders[i].all_cost();
      } else if (cost_tmp - riders[i].all_cost() < min) {
        path = path_tmp, cost = cost_tmp, index = i,
        min = cost - riders[i].all_cost();
      } else if (cost_tmp - riders[i].all_cost() == min &&
                 (cost_tmp < cost ||
                  (cost_tmp == cost && path_tmp.size() < path.size()))) {
        path = path_tmp, cost = cost_tmp, index = i;
      }
    }
    // 如果有超时的嫌疑而且可以买下一个骑手，则新雇佣一个骑手
    if (cost > 2 * OUTDATE_TIME && money() >= 300) {
      AddRider();  //添加骑手
      auto [path_tmp, cost_tmp] = Algorithm::cal_solution(
          riders[riders.size() - 1], waiting_order_.front(), time_);
      riders[riders.size() - 1].AddOrder(waiting_order_.front());  //添加订单
      riders[riders.size() - 1].ChangePath(path_tmp, cost_tmp);   //更新路径
    } else {
      riders[index].ChangePath(path, cost);
      riders[index].AddOrder(waiting_order_.front());
    }
    // waiting_order_.erase(waiting_order_.begin());
    waiting_order_.pop();
  }

  //每一个骑手行走
  for (auto i = 0; i < riders.size(); i++) {
    riders[i].Step(time_);
  }

  //时间++
  time_++;

  //检查是否有状态改变
  CheckIsBreak();
  CheckIsRevoke();
  CheckIsFinished();
}

int Model::num_all() const { return num_all_; }

int Model::num_finished() const {
  int finished = 0;  //订单完成数
  for (int i = 0; i < riders.size(); i++) {
    finished = finished + riders[i].finished_orders().size();
  }  //遍历所有骑手，统计完成的订单数
  return finished;
}

int Model::num_outdate() const {
  int outdate = 0;  //超时订单数
  for (int i = 0; i < riders.size(); i++) {
    outdate = outdate + riders[i].outdate_orders().size();
  }  //遍历所有骑手，统计超时订单数
  return outdate;
}

int Model::money() const {
  int total = INIT_MONEY - riders.size() * 300;  //当前金钱数
  for (int i = 0; i < riders.size(); i++) {
    total = total + 10 * riders[i].finished_orders().size();
    total = total - 50 * riders[i].outdate_orders().size();
  }  //遍历所有骑手，统计当前金钱数
  return total;
}
int Model::time() const { return time_; }

ProgramStatus Model::statu() const { return statu_; }

void Model::AddRider() { riders.push_back(Rider(init_position_)); }
