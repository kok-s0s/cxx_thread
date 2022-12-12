#include "Thread.h"
#include <iostream>

class A {
private:
  int num = 0;
  Thread p;
  bool flag_01 = false;
  bool flag_02 = false;

public:
  A() {
    p.setFunc([this]() {
      task_01();
      task_02();
    });
    p.start();
  }
  ~A() {}
  void task_01() {
    if (flag_01) {
      std::cout << "task 01 start\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      std::cout << "task 01 end\n";
    } else {
      std::cout << "false 01\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }
  void task_02() {
    if (flag_02) {
      std::cout << "task 02 start\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      std::cout << "task 02 end\n";
    } else {
      std::cout << "false 02\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
  }

  void setFlag_01() { flag_01 = !flag_01; }
  void setFlag_02() { flag_02 = !flag_02; }
};

int main(int, char **) {
  A test_a;
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    test_a.setFlag_01();
    test_a.setFlag_02();
  }
  return 0;
}
