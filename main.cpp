#include <iostream>

#include "CustomThread.h"

using namespace std;

int main(int, char**) {
  CustomThread human("xiao ming");

  while (1) {
    human.sendSayHelloSignal();
    human.sendTodoSignal("sleep");
    human.sendSayGoodByeSignal();
  }

  return 0;
}
