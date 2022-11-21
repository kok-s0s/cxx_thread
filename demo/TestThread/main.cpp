#include "TestThread.h"
#include <chrono>
#include <iomanip>
#include <iostream>

void normal_task() {
  std::cout << "normal task start\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "normal task end\n";
}

int main() {
  TestThread n_thread(normal_task);

  while (1) {
    std::cout << "Thread State:" << n_thread.getStatus() << std::endl;

    n_thread.start();
    std::cout << "Thread State:" << n_thread.getStatus() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    n_thread.pause();
    std::cout << "Thread State:" << n_thread.getStatus() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(10000));

    n_thread.resume();
    std::cout << "Thread State:" << n_thread.getStatus() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    n_thread.quit();
    std::cout << "Thread State:" << n_thread.getStatus() << std::endl;

    std::cout << "One more" << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  }

  return 0;
}