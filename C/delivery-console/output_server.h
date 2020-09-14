#pragma once
#include <windows.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "../delivery-model/model.h"
#include "../delivery-model/point.h"

static const char *BLANK = "                ";
//����������н��
class OutputServer {
 public:
  //����ļ�ָ��
  std::ofstream OutFile;

  OutputServer();
  ~OutputServer();
  //��ӡ������̨
  void PrintToConsole(const Model &m);
  //��ӡ���ļ�
  void PrintToFile(const Model &m);
  //��ӡ��������
  void EndPrint(const Model &m);

 private:
	 //ת�������д�ӡָ��λ��
  void gotoxy(short x, short y);
};
