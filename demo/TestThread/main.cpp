#include "ThreadExtension.h"
#include <chrono>
#include <iomanip>
#include <iostream>

void normal_task() {
  std::cout << "normal task start\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "normal task end\n";
}

class Test {
private:
public:
  ThreadExtension p_thread;
  Test() {}
};

int main() {
  Test t;
  t.p_thread.setFunc(normal_task);

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