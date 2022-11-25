#include "safe_stack.h"
#include <iostream>

int main(int, char **) {
  threadsafe_stack<int> ts_stack;
  ts_stack.push(5);
  ts_stack.push(2);
  return 0;
}
