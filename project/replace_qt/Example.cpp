#include "Example.h"

Example::Example() {
  _event = event_create(true, true);

  _thread.setFunc([this]() {
    if (!event_timedwait(_event, 50000) && _flag) {
      std::cout << "event called\n";
      event_reset(_event);
      _flag = false;
    }
  });

  _thread.start();
}

Example::~Example() {
  event_destroy(_event);
  _thread.quit();
}

void Example::eventSet(bool value) {
  event_set(_event);
  _flag = value;
}
