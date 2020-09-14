#include <exception>
#include <iostream>
#include <stdexcept>
class Matrix {
 public:
  /**
   * 矩阵的行数
   */
  int lines;

  /**
   * 矩阵的列数
   */
  int rows;

  /**
   * 矩阵的数据
   */
  int** value;

  /**
   * 矩阵构造函数
   */
  Matrix(int lines, int rows) : lines(lines), rows(rows) {
#ifdef DEBUG
    printf("[DEBUG] 构造Matrix对象\n");
#endif
    value = (int**)malloc(sizeof(int*) * lines);
    for (int i = 0; i < lines; i++) {
      value[i] = (int*)malloc(sizeof(int) * rows);
    }
  }

  /**
   * 矩阵拷贝构造函数
   */
  Matrix(const Matrix& other) : lines(other.lines), rows(other.rows) {
#ifdef DEBUG
    printf("[DEBUG] 构造Matrix对象\n");
#endif
    value = (int**)malloc(sizeof(int*) * lines);
    for (int i = 0; i < lines; i++) {
      value[i] = (int*)malloc(sizeof(int) * rows);
      for (int j = 0; j < rows; j++) {
        value[i][j] = other.value[i][j];
      }
    }
  }

  Matrix operator+(const Matrix& other) {
    if (lines != other.lines || rows != other.rows) {
      throw std::exception();
    }
    Matrix m = Matrix(lines, rows);
    for (int i = 0; i < lines; i++) {
      for (int j = 0; j < rows; j++) {
        m.value[i][j] = value[i][j] + other.value[i][j];
      }
    }
    return m;
  }

  Matrix operator-(const Matrix& other) {
    if (lines != other.lines || rows != other.rows) {
      throw std::exception();
    }
    Matrix m = Matrix(lines, rows);
    for (int i = 0; i < lines; i++) {
      for (int j = 0; j < rows; j++) {
        m.value[i][j] = value[i][j] - other.value[i][j];
      }
    }
    return m;
  }

  Matrix& operator=(const Matrix& other) {
    if (lines != other.lines || rows != other.rows) {
      throw std::exception();
    }
    for (int i = 0; i < lines; i++) {
      for (int j = 0; j < rows; j++) {
        value[i][j] = other.value[i][j];
      }
    }
    return *this;
  }

  friend std::istream& operator>>(std::istream& in, Matrix& a) {
    for (int i = 0; i < a.lines; i++) {
      for (int j = 0; j < a.rows; j++) {
        in >> a.value[i][j];
      }
    }
    return in;
  }

  friend std::ostream& operator<<(std::ostream& out, Matrix& a) {
    for (int i = 0; i < a.lines; i++) {
      for (int j = 0; j < a.rows; j++) {
        std::cout << a.value[i][j];
        if (j != a.rows - 1) {
          std::cout << " ";
        }
      }
      std::cout << std::endl;
    }
    return out;
  }

  ~Matrix() {
#ifdef DEBUG
    printf("[DEBUG] 析构Matrix对象\n");
#endif
    for (int i = 0; i < lines; i++) {
      free(value[i]);
    }
    free(value);
  }
};

int main() {
  Matrix A1 = Matrix(4, 5);
  Matrix A2 = Matrix(4, 5);
  Matrix A3 = Matrix(4, 5);

  std::cin >> A1 >> A2;

  A3 = A1 + A2;
  std::cout << A3;

  A3 = A1 - A2;
  std::cout << A3;

  Matrix* pA1 = new Matrix(4, 5);
  Matrix* pA2 = new Matrix(4, 5);
  Matrix* pA3 = new Matrix(4, 5);

  std::cin >> *pA1 >> *pA2;

  *pA3 = *pA1 + *pA2;
  std::cout << *pA3;

  *pA3 = *pA1 - *pA2;
  std::cout << *pA3;
  delete pA1;
  delete pA2;
  delete pA3;
}
