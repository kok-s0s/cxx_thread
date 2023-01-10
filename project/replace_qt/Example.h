#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <iostream>
#include <string>

#include "Thread.h"
#include "signal.hpp"

class Example {
 public:
  Example();
  ~Example();

  Thread thread;

  sigslot::signal<bool> normalSig;
  sigslot::signal<bool, float, int, bool, std::string &> paramSig;

  void normalSlot(bool flag);
  void paramSlot(bool flag, double d, int i, bool b, std::string &s);
};

#endif  // EXAMPLE_H_
