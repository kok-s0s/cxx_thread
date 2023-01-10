#include <iostream>
#include <string>

#include "Thread.h"

class A {
 public:
  enum class Option { KEY01, KEY02, END };
  struct Param {
    std::string name;
    int age;
  };

 private:
  Thread _p;
  Param _param;
  bool _flag_01 = false;
  bool _flag_02 = false;
  Option _key = Option::KEY01;

 public:
  A() {
    _p.setFunc([this]() {
      // if (_flag_01) {
      //   task_01();
      //   _flag_01 = false;
      // } else {
      //   std::cout << "false 01\n";
      //   std::this_thread::sleep_for(std::chrono::milliseconds(10));
      // }
      // if (_flag_02) {
      //   task_02(_param);
      //   _flag_02 = false;
      // } else {
      //   std::cout << "false 02\n";
      //   std::this_thread::sleep_for(std::chrono::milliseconds(10));
      // }
      switch (_key) {
        case Option::KEY01:
          task_01();
          break;
        case Option::KEY02:
          task_02(_param);
          break;
        default:
          std::cout << "nothing to do!\n";
          std::this_thread::sleep_for(std::chrono::milliseconds(10));
          break;
      }
    });
    _p.start();
  }
  ~A() {}

  void task_01() {
    std::cout << "task 01 start\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "task 01 end\n";
  }
  void task_02(Param param) {
    std::cout << "task 02 start\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << param.name << "\n";
    std::cout << param.age << "\n";
    std::cout << "task 02 end\n";
  }

  void setFlag_01_true() { _flag_01 = true; }
  void setFlag_02_true() { _flag_02 = true; }

  void setParam(std::string name, int age) {
    _param.name = name;
    _param.age = age;
  }

  void setKey(Option key) { _key = key; }
};

int main(int, char **) {
  A test_a;
  while (1) {
    // test_a.setFlag_01_true();
    // test_a.setFlag_02_true();
    test_a.setParam("me", 23);
    test_a.setKey(A::Option::KEY02);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    test_a.setKey(A::Option::END);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  return 0;
}
