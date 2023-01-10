#include "Example.h"

Example::Example() {
  normalSig.connect(&Example::normalSlot, this);
  paramSig.connect(&Example::paramSlot, this);
  thread.setFunc([this]() {
    normalSlot(false);
    // paramSlot(false, 1.f, 2, false, );
  });
  thread.start();
}

Example::~Example() {}

void Example::normalSlot(bool flag) {
  if (flag) {
    std::cout << "normalSlot 所在线程的 Id：" << std::this_thread::get_id()
              << std::endl;
    // std::cout << "normalSlot thread id = " << thread.get_id() << std::endl;
  }
}

void Example::paramSlot(bool flag, double d, int i, bool b, std::string &s) {
  if (flag) {
    s = b ? std::to_string(i) : std::to_string(d);
  }
}
