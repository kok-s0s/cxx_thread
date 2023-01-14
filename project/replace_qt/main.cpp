#include <iostream>

#include "Example.h"

int main(int, char **) {
  std::cout << "main thread start：" << std::this_thread::get_id() << std::endl;

  Example example;

  int num = 0;

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (num % 5 == 0) {
      example.eventSet(true);
    } else {
      std::cout << "no event!\n";
    }
    num = num <= 100 ? ++num : 0;
  }

  return 0;
}
