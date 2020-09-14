#pragma once
#include<cmath>
enum PointType {
	FROM, //�͹�
	TO, //ʳ��
};
//������Point
class Point {
 public:
  // x����
  int x;

  // y����
  int y;

  //�������
  PointType type;

  //����Ӧ�Ķ���id
  unsigned order_id;

  Point();
  Point(int x, int y);
  Point(int x, int y, PointType type, unsigned order_id);
  ~Point();

  //������������پ���
	//����
	//		Point from Ŀ���1
	//		Point to Ŀ���2
	//���
	//		int �����پ���
  static int GetDistant(const Point& from, const Point& to);

	//�ж��Ƿ񵽴�
	//����
	//		Point from ��ǰλ��
	//    Point to Ŀ��λ��
	//���
	//		�Ƿ񵽴�
  static bool IsArrive(const Point& from, const Point& to);

	//������һ�����ߵ�λ��
	//����
	//    Point from ��ǰλ��
	//    Point to Ŀ��λ��
	//���
	//    Point  �ƶ�����һ���������
  static Point Move(const Point& from, const Point& to);

	//����������
  bool operator==(const Point& p) const;
  bool operator<(const Point& p) const;
  bool operator>(const Point& p) const;
};
