#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>

#include "Event.h"

event_handle g_event = NULL;

void *pFunc1(int *pNum) {
  while (1) {
    sleep(1);
    std::cout << *pNum << " func1 called\n";
    event_set(g_event);
  }
}

void *pFunc2(char *pCh) {
  while (1) {
    if (!event_timedwait(g_event, 50000)) {
      sleep(5);
      std::cout << *pCh << " func2 called\n";
      event_reset(g_event);
    }
  }
}

int main() {
  g_event = event_create(true, true);

  int iNum = 0;
  char ch = 'a';

  std::thread thread1(pFunc1, &iNum);
  std::thread thread2(pFunc2, &ch);

  thread1.detach();
  thread2.detach();

  while (1) {
    iNum++;
    ch = 'a' + iNum % 26;
    sleep(1);
    if (iNum > 100) {
      iNum = 0;
    }
  }

  event_destroy(g_event);

  return 0;
}
