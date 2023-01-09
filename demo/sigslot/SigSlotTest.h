#ifndef SigSlotTest_H_
#define SigSlotTest_H_

#include <iostream>
#include <string>

#include "signal.hpp"

class SigSlotTest {
 public:
  sigslot::signal<> normalSig;
  sigslot::signal<float, int, bool, std::string &> paramSig;

  SigSlotTest();
  ~SigSlotTest();

  void normalSlot();

  void paramSlot(double d, int i, bool b, std::string &s);
};

#endif  // SigSlotTest_H_
