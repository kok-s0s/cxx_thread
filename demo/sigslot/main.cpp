#include <iostream>

#include "SigSlotTest.h"

int main() {
  SigSlotTest sigSlotTest;

  sigSlotTest.normalSig();

  float f = 1.f;
  short i = 2;  // convertible to int
  std::string s = "0";

  sigSlotTest.paramSig(f, i, false, s);
  std::cout << s << std::endl;
  sigSlotTest.paramSig(f, i, true, s);
  std::cout << s << std::endl;

  return 0;
}
