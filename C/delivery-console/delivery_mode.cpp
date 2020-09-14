// 外卖派单模拟系统.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <fcntl.h> //用于输出unicode字符
#include <io.h>

#include <iostream>
#include "input_server.h"
#include "../delivery-model/model.h"
#include "output_server.h"
#include "../delivery-model/constant.h"

int main() {
  //初始化
  _setmode(_fileno(stdout), _O_U16TEXT);  //用于输出unicode字符
  InputServer in;  //输入服务
  OutputServer out; //输出服务
  Model m = Model(Point(X, Y)); //初始化模型类

  //欢迎程序
  std::wcout << L"用户您好," << std::endl;
  std::wcout << L"欢迎您使用外卖订单派送系统 ~o(≧口≦) o ~" << std::endl;
  std::wcout << std::endl;
  if (!in.InFile) {
    std::wcout << L"没有找到文件, 使用手动输入模式" << std::endl;
    in.inputFromConsole(); //从命令行输入
  } else {
    in.inputFromFile(); //从文件输入
  }
  system("pause");
  system("cls");

  //主程序
  while (m.statu() == GOOD || (m.statu() == FINISHED) && in.q.size() != 0) {
    in.inputData(m); //将数据输入模型
    m.Step();  //进行下一步
    out.PrintToConsole(m); //打印到命令行
    out.PrintToFile(m); //打印到文件
  }
  system("cls");
  out.EndPrint(m); //结束总结
  system("pause");
}
