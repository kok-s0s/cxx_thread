#include <iostream>

#include "Example.h"

int main(int, char **) {
  std::cout << "主线程启动：" << std::this_thread::get_id() << std::endl;

  Example example;

  while (1) {
    example.normalSig(true);

    float f = 1.f;
    short i = 2;  // convertible to int
    std::string s = "0";

    example.paramSig(true, f, i, false, s);
    std::cout << s << std::endl;
    example.paramSig(true, f, i, true, s);
    std::cout << s << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }

  return 0;
}
