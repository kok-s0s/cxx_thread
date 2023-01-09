#include "Thread.h"
#include <chrono>
#include <iomanip>
#include <iostream>

class Test {
public:
  Thread p_thread;
  Test() {
    p_thread.setFunc([this]() {
      menber_task_01();
      menber_task_02();
    });
  }

  void menber_task_01() {
    std::cout << "menber task 01 start\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "menber task 01 end\n";
  }

  void menber_task_02() {
    std::cout << "menber task 02 start\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::cout << "menber task 02 end\n";
  }
};

int main() {
  Test t;

  while (1) {
    std::cout << "Thread State:" << t.p_thread.getStatus() << std::endl;

    t.p_thread.start();
    std::cout << "Thread State:" << t.p_thread.getStatus() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    t.p_thread.pause();
    std::cout << "Thread State:" << t.p_thread.getStatus() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    t.p_thread.resume();
    std::cout << "Thread State:" << t.p_thread.getStatus() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    t.p_thread.quit();
    std::cout << "Thread State:" << t.p_thread.getStatus() << std::endl;

    std::cout << "One more" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  }

  return 0;
}
