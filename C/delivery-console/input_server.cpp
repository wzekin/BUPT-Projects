#include "input_server.h"

InputServer::InputServer() { InFile = std::ifstream("./sales.txt"); }

void InputServer::inputData(Model &m) {
  while (!q.empty() && q.front().time == m.time()) {
    m.AddOrder(q.front());  //对应时间接单
    q.pop();                 //删除
  }
}

void InputServer::inputFromConsole() {
  int n, time;         //定义序号，下单时间
  int x1, y1, x2, y2;  //定义餐馆，食客坐标
  while (1) {
    std::wcout << L"请输入订单（按-1结束）";
    std::cin >> n;
    if (n == -1) {
      break;
    }
    std::cin >> time >> x1 >> y1 >> x2 >> y2;
    Order tempOrder = Order(n, Point(x1, y1, FROM, n), Point(x2, y2, TO, n),
                            time);  // 乘二适应坐标
    q.push(tempOrder);              //入队
  }                                 //读取所有数据
}
void InputServer::inputFromFile() {
  int n, time;         //定义序号，下单时间
  int x1, y1, x2, y2;  //定义餐馆，食客坐标
  while (!InFile.eof() && InFile.peek() != EOF) { //到末尾停止
    InFile >> n >> time >> x1 >> y1 >> x2 >> y2;
    Order tempOrder = Order(n, Point(x1, y1, FROM, n), Point(x2, y2, TO, n),
                            time);  // 乘二适应坐标
    q.push(tempOrder);              //入队
    InFile.get(); //读取最后的换行符
  }  //读取所有数据
}
InputServer::~InputServer() {}
