#include "input_server.h"

InputServer::InputServer() { InFile = std::ifstream("./sales.txt"); }

void InputServer::inputData(Model &m) {
  while (!q.empty() && q.front().time == m.time()) {
    m.AddOrder(q.front());  //��Ӧʱ��ӵ�
    q.pop();                 //ɾ��
  }
}

void InputServer::inputFromConsole() {
  int n, time;         //������ţ��µ�ʱ��
  int x1, y1, x2, y2;  //����͹ݣ�ʳ������
  while (1) {
    std::wcout << L"�����붩������-1������";
    std::cin >> n;
    if (n == -1) {
      break;
    }
    std::cin >> time >> x1 >> y1 >> x2 >> y2;
    Order tempOrder = Order(n, Point(x1, y1, FROM, n), Point(x2, y2, TO, n),
                            time);  // �˶���Ӧ����
    q.push(tempOrder);              //���
  }                                 //��ȡ��������
}
void InputServer::inputFromFile() {
  int n, time;         //������ţ��µ�ʱ��
  int x1, y1, x2, y2;  //����͹ݣ�ʳ������
  while (!InFile.eof() && InFile.peek() != EOF) { //��ĩβֹͣ
    InFile >> n >> time >> x1 >> y1 >> x2 >> y2;
    Order tempOrder = Order(n, Point(x1, y1, FROM, n), Point(x2, y2, TO, n),
                            time);  // �˶���Ӧ����
    q.push(tempOrder);              //���
    InFile.get(); //��ȡ���Ļ��з�
  }  //��ȡ��������
}
InputServer::~InputServer() {}
