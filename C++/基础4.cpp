#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

int main() {
  int guess_value;
  srand(time(0));
  int value = rand() % 1000 + 1;
  std::string temp;
  while (1) {
    std::cout << "请输入商品的价格: ";
    getline(std::cin, temp);

    try {
      guess_value = std::stoi(temp);
    } catch (std::invalid_argument) {
      std::cout << "请输入正确的数字！" << std::endl;
      continue;
    }

    if (guess_value == value) {
      std::cout << "你猜对了！" << std::endl;
      break;
    } else if (guess_value < value) {
      std::cout << "你猜小了！" << std::endl;
    } else {
      std::cout << "你猜大了！" << std::endl;
    }
  }
}
