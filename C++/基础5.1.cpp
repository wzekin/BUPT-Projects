#include <cmath>
#include <iostream>
class Shape {
 public:
  static const int value = 1;
  virtual double getArea() = 0;
  Shape() {
#ifdef DEBUG
    std::cout << "[DEBUG] 构造Shape对象\n";
#endif
  }
  virtual ~Shape() {
#ifdef DEBUG
    std::cout << "[DEBUG] 析构Shape对象\n";
#endif
  }
};

class Rectangle : virtual public Shape {
 public:
  static const int value = 2;
  /**
   * 高
   */
  int height;

  /**
   * 宽
   */
  int width;

  /**
   * 计算面积
   */
  double getArea() { return height * width; }

  /**
   * 构造函数
   */
  Rectangle(int height, int width) : height(height), width(width) {
#ifdef DEBUG
    std::cout << "[DEBUG] 构造Rectangle对象\n";
#endif
  };

  ~Rectangle() {
#ifdef DEBUG
    std::cout << "[DEBUG] 析构Rectangle对象\n";
#endif
  }
};

class Square : public Rectangle {
 public:
  Square(int height) : Rectangle(height, height) {
#ifdef DEBUG
    std::cout << "[DEBUG] 构造Square对象\n";
#endif
  }
  ~Square() {
#ifdef DEBUG
    std::cout << "[DEBUG] 析构Square对象\n";
#endif
  }
};

class Circle : virtual public Shape {
 public:
  /**
   * 半径
   */
  int redius;

  /**
   * 计算面积
   */
  double getArea() { return M_PI * redius * redius; }

  /**
   * 构造函数
   */
  Circle(int redius) : redius(redius) {
#ifdef DEBUG
    std::cout << "[DEBUG] 构造Circle对象\n";
#endif
  };

  ~Circle() {
#ifdef DEBUG
    std::cout << "[DEBUG] 析构Circle对象\n";
#endif
  }
};

int main() {
  // 矩形
  Shape* R1 = new Rectangle(2, 3);
  // 正方形
  Shape* S1 = new Square(2);
  // 圆形
  Shape* C1 = new Circle(2);

  std::cout << R1->value << std::endl;
  std::cout << "矩形的面积为：" << R1->getArea() << std::endl;
  std::cout << "正方形的面积为：" << S1->getArea() << std::endl;
  std::cout << "圆形的面积为：" << C1->getArea() << std::endl;

  delete R1;
  delete S1;
  delete C1;
}
