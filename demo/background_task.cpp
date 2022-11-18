#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

bool flag = true;

void background_task_1() {
  while (1) {
    while (flag) {
      std::cout << "111 这是一个可持续运行的后台线程\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    while (!flag) {
      std::cout << "111 没运行到指定内容\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

void background_task_2() {
  while (1) {
    while (flag) {
      std::cout << "222 这是一个可持续运行的后台线程\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    while (!flag) {
      std::cout << "222 没运行到指定内容\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
  }
}

int main() {
  std::thread bt_trhead_1(background_task_1);
  std::thread bt_trhead_2(background_task_2);
  bt_trhead_1.detach();
  bt_trhead_2.detach();

  while (1) {
    std::cout << "这是主线程\n";
    flag = !flag;
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
  return 0;
}