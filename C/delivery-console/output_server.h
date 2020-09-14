#pragma once
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "../delivery-model/model.h"
#include "../delivery-model/point.h"

static const char *BLANK = "                ";
//输出程序运行结果
class OutputServer {
 public:
  //输出文件指针
  std::ofstream OutFile;

  OutputServer();
  ~OutputServer();
  //打印到控制台
  void PrintToConsole(const Model &m);
  //打印到文件
  void PrintToFile(const Model &m);
  //打印结束界面
  void EndPrint(const Model &m);

 private:
	 //转移命令行打印指针位置
  void gotoxy(short x, short y);
};
