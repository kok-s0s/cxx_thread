#include "TestThread.h"
#include <chrono>
#include <iomanip>
#include <iostream>

void background_task_1(bool &flag) {
  while (1) {
    while (flag) {
      std::cout << "1st useful content\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    while (!flag) {
      std::cout << "1st uselessful content\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

void background_task_2(bool &flag) {
  while (1) {
    while (flag) {
      std::cout << "2nd useful content\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    while (!flag) {
      std::cout << "2nd uselessful content\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

void normal_task() {
  std::cout << "normal task start\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  std::cout << "normal task end\n";
}

#define CURMODE 1

int main() {

#if CURMODE == 0

  // eg-1 background_task
  bool flag = true;
  TestThread bt_thread_1(background_task_1, std::ref(flag));
  TestThread bt_thread_2(background_task_2, std::ref(flag));
  bt_thread_1.detach();
  bt_thread_2.detach();
  std::thread t1(background_task_1, std::ref(flag));
  while (1) {
    std::cout << "main thread\n";
    flag = !flag;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }

#elif CURMODE == 1

  // eg-2 normal_task Creation -> Ready -> Running -> Finished
  TestThread n_thread(normal_task);
  std::cout << n_thread.get_id() << std::endl;
  if (n_thread.isFinished()) {
    std::cout << n_thread.getStatus() << std::endl;
  } else {
    n_thread.join();
    std::cout << n_thread.getStatus() << std::endl;
  }

#endif

  return 0;
}