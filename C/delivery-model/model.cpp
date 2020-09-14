#include "model.h"

Model::Model(Point init_position)
    : init_position_(init_position), statu_(GOOD),time_(0),num_all_(0) {
}



void Model::CheckIsBreak() {
  if (money() < 0) {  //��Ǯ��С����ʱ�Ʋ�
    statu_ = BREAK;
  }
}

void Model::CheckIsRevoke() {
  for (int i = 0; i < riders.size(); i++)
    if (riders[i].illegal_orders() != 0) { // ����Ƿ��д���60����λʱ�����͵Ķ���
      statu_ = REVOKE;
      return;
    }
}

void Model::CheckIsFinished() {
  int flag = 0;  //��ǵ�ǰ״̬�ı�������flag==0ʱ���ӵ����ɵ�����Ϊ��
  for (int i = 0; i < riders.size(); i++) { //��鵱ǰ�Ƿ����ж����������
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
  waiting_order_.push(o); //���뵽�ȴ��ɵ��Ķ�����
}

void Model::AddOrder(int n, int time, int x1, int y1, int x2, int y2) {
  Order tempOrder =
      Order(n, Point(x1, y1, FROM, n), Point(x2, y2, TO, n), time);
  AddOrder(tempOrder);
}

void Model::Step() {
  statu_ = GOOD;  // ��ʼ��statu_

  while (!waiting_order_.empty()) {  //������е�δ�ɷ�����
    std::queue<Point> path;
    int index = -1, min = INT_MAX, cost = 100, is_outdate = 1;
    for (auto i = 0; i < riders.size(); i++) {
      // ����Ƿ񳬹�һ�����ֵĽӵ���
      if (riders[i].sending_orders().size() +
              riders[i].received_orders().size() * 2 >
          15 && money() >= 300) {
        continue;
      }

      //���������Ӷ����������ѵ�cost �� ����·�������ҳ�·���ı�����С������
      auto [path_tmp, cost_tmp] = Algorithm::cal_solution(
          riders[i], waiting_order_.front(), time_);  // c++17 ��׼
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
    // ����г�ʱ�����ɶ��ҿ�������һ�����֣����¹�Ӷһ������
    if (cost > 2 * OUTDATE_TIME && money() >= 300) {
      AddRider();  //�������
      auto [path_tmp, cost_tmp] = Algorithm::cal_solution(
          riders[riders.size() - 1], waiting_order_.front(), time_);
      riders[riders.size() - 1].AddOrder(waiting_order_.front());  //��Ӷ���
      riders[riders.size() - 1].ChangePath(path_tmp, cost_tmp);   //����·��
    } else {
      riders[index].ChangePath(path, cost);
      riders[index].AddOrder(waiting_order_.front());
    }
    // waiting_order_.erase(waiting_order_.begin());
    waiting_order_.pop();
  }

  //ÿһ����������
  for (auto i = 0; i < riders.size(); i++) {
    riders[i].Step(time_);
  }

  //ʱ��++
  time_++;

  //����Ƿ���״̬�ı�
  CheckIsBreak();
  CheckIsRevoke();
  CheckIsFinished();
}

int Model::num_all() const { return num_all_; }

int Model::num_finished() const {
  int finished = 0;  //���������
  for (int i = 0; i < riders.size(); i++) {
    finished = finished + riders[i].finished_orders().size();
  }  //�����������֣�ͳ����ɵĶ�����
  return finished;
}

int Model::num_outdate() const {
  int outdate = 0;  //��ʱ������
  for (int i = 0; i < riders.size(); i++) {
    outdate = outdate + riders[i].outdate_orders().size();
  }  //�����������֣�ͳ�Ƴ�ʱ������
  return outdate;
}

int Model::money() const {
  int total = INIT_MONEY - riders.size() * 300;  //��ǰ��Ǯ��
  for (int i = 0; i < riders.size(); i++) {
    total = total + 10 * riders[i].finished_orders().size();
    total = total - 50 * riders[i].outdate_orders().size();
  }  //�����������֣�ͳ�Ƶ�ǰ��Ǯ��
  return total;
}
int Model::time() const { return time_; }

ProgramStatus Model::statu() const { return statu_; }

void Model::AddRider() { riders.push_back(Rider(init_position_)); }
