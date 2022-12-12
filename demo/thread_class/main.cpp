#include "Thread.h"
#include <iostream>

class A {
private:
  int num = 0;
  Thread p;
  bool flag_01 = false;
  bool flag_02 = false;
  bool flag_03 = false;

public:
  A() {
    p.setFunc([this]() {
      //   if (flag_01) {
      //     task_01();
      //   } else {
      //     std::cout << "false 01\n";
      //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
      //   }
      //   if (flag_02) {
      //     task_02();
      //   } else {
      //     std::cout << "false 02\n";
      //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
      //   }
      if (flag_03) {
        task_03(num);
      } else {
        std::cout << "false 03\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      }
    });
    p.start();
  }
  ~A() {}
  void task_01() {
    std::cout << "task 01 start\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "task 01 end\n";
    flag_01 = false;
  }
  void task_02() {
    std::cout << "task 02 start\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "task 02 end\n";
    flag_02 = false;
  }
  void task_03(int num) {
    std::cout << "task 03 start\n";
    std::cout << num << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "task 03 end\n";
    flag_03 = false;
  }

  void setFlag_01() { flag_01 = true; }
  void setFlag_02() { flag_02 = true; }
  void setFlag_03() { flag_03 = true; }

  void add() {
    num++;
    if (num > 100) {
      num = 0;
    }
  }
};

int main(int, char **) {
  A test_a;
  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(60));
    // test_a.setFlag_01();
    // test_a.setFlag_02();
    test_a.setFlag_03();
    test_a.add();
  }
  return 0;
}
