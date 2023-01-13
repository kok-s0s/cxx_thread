#ifndef EXAMPLE_H_
#define EXAMPLE_H_

#include <assert.h>

#include <iostream>
#include <string>

#include "Hik_Event.h"
#include "Thread.h"

class Example {
 private:
  event_handle _event = NULL;
  Thread _thread;
  bool _flag = false;

 public:
  Example();
  ~Example();

  void eventSet(bool value);
};

#endif  // EXAMPLE_H_
