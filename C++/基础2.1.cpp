#include <cmath>
#include <iostream>

class Point {
 public:
  /**
   * x坐标
   */
  double x;

  /**
   * y坐标
   */
  double y;

  /**
   * Point 构造函数
   */
  Point(double x, double y)
      : x(x),
        y(y){
#ifdef DEBUG
            printf("[DEBUG] 构建新Point对象 x坐标为%lf,y坐标为%lf\n", x, y);
#endif
        };

  Point()
      : x(0),
        y(0){

#ifdef DEBUG
            printf("[DEBUG] 构建新Point对象 x坐标为%lf,y坐标为%lf\n", 0.0, 0.0);
#endif
        };

  ~Point(){
#ifdef DEBUG
            printf("[DEBUG] 析构Point对象 x坐标为%lf,y坐标为%lf\n", x, y);
#endif

  }

  /**
   * 计算两点距离
   * @param other 另一点
   */
  double distance(const Point& other) {
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
  }
};

class Circle {
 public:
  /**
   * 圆心
   */
  Point center;

  /**
   * 半径
   */
  double redius;

  /**
   * 两个圆是否相交
   */
  bool is_intersect(const Circle& other) {
    return center.distance(other.center) <= (redius + other.redius);
  }

  Circle(Point center, double redius) : center(center), redius(redius) {
#ifdef DEBUG
    printf("[DEBUG] 构建新Circle对象 圆心坐标为%lf,%lf, 半径为%lf\n", center.x,
           center.y, redius);
#endif
  };

  ~Circle(){
#ifdef DEBUG
    printf("[DEBUG] 析构Circle对象 圆心坐标为%lf,%lf, 半径为%lf\n", center.x,
           center.y, redius);
#endif

  }
};

int main() {
  double x, y, redius;

  std::cout << "请输入第一个圆的数据：" << std::endl;
  std::cout << "圆的圆心为：" << std::endl;
  std::cin >> x >> y;
  std::cout << "圆的半径为：" << std::endl;
  std::cin >> redius;

  Circle C1 = Circle(Point(x, y), redius);

  std::cout << "请输入第二个圆的数据：" << std::endl;
  std::cout << "圆的圆心为：" << std::endl;
  std::cin >> x >> y;
  std::cout << "圆的半径为：" << std::endl;
  std::cin >> redius;

  Circle C2 = Circle(Point(x, y), redius);

  if (C1.is_intersect(C2)) {
    std::cout << "两个圆相交！" << std::endl;
  } else {
    std::cout << "两个圆不相交！" << std::endl;
  }
}
