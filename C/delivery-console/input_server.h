#pragma once
#include <fstream>
#include <queue>
#include "../delivery-model/model.h"
#include "../delivery-model/order.h"
#include "../delivery-model/point.h"


//读取订单，并添加入队列
class InputServer {
 public:
	//输入文件指针
  std::ifstream InFile;
	//输入订单队列
  std::queue<Order> q;

  InputServer();
  ~InputServer();

	//从命令行读取数据
  void inputFromConsole();

	//从文件读取数据
  void inputFromFile();

	//将数据输入到模型中
  void inputData(Model &m);
};
