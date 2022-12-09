#include "ThreadExtension.h"
#include <chrono>
#include <iomanip>
#include <iostream>

void normal_task_01() {
  std::cout << "normal task 01 start\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "normal task 01 end\n";
}

void normal_task_02() {
  std::cout << "normal task 02 start\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "normal task 02 end\n";
}

class Test {
public:
  ThreadExtension p_thread;
  Test() {
    p_thread.addFunc(normal_task_01);
    p_thread.addFunc(normal_task_02);
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