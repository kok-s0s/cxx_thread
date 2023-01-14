#include "Example.h"

Example::Example() {
  _event = event_create(false, false);

  _thread.setFunc([this]() {
    if (!event_timedwait(_event, 50000) && _flag) {
      std::cout << "event called\n";
      std::cout << "sub thread：" << std::this_thread::get_id() << std::endl;
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
