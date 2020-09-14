#pragma once
#include <fstream>
#include <queue>
#include "../delivery-model/model.h"
#include "../delivery-model/order.h"
#include "../delivery-model/point.h"


//��ȡ����������������
class InputServer {
 public:
	//�����ļ�ָ��
  std::ifstream InFile;
	//���붩������
  std::queue<Order> q;

  InputServer();
  ~InputServer();

	//�������ж�ȡ����
  void inputFromConsole();

	//���ļ���ȡ����
  void inputFromFile();

	//���������뵽ģ����
  void inputData(Model &m);
};
