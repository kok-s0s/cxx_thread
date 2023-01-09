#include "SigSlotTest.h"

SigSlotTest::SigSlotTest() {
  normalSig.connect(&SigSlotTest::normalSlot, this);
  paramSig.connect(&SigSlotTest::paramSlot, this);
}

SigSlotTest::~SigSlotTest() {}

void SigSlotTest::normalSlot() { std::cout << "SigSlotTest!" << std::endl; }

void SigSlotTest::paramSlot(double d, int i, bool b, std::string &s) {
  s = b ? std::to_string(i) : std::to_string(d);
}
