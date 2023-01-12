#include <assert.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <thread>

#include "Hik_Event.h"

event_handle g_event = NULL;

event_handle CreateEvent(bool bManualReset, bool bInitialState) {
  event_handle pEvent = event_create(bManualReset, bInitialState);
  assert(pEvent);

  return pEvent;
}

unsigned int WaitForSingleObject(event_handle pEvent, int cms) {
  assert(pEvent);
  if (event_timedwait(pEvent, cms)) {
    return 0;
  }

  return 1;
}

void CloseHandle(event_handle pEvent) { delete pEvent; }

void SetEvent(event_handle pEvent) { event_set(pEvent); }

void ResetEvent(event_handle pEvent) { event_reset(pEvent); }

void *pFunc1(int *pNum) {
  while (1) {
    sleep(1);
    std::cout << *pNum << " func1 called\n";
    SetEvent(g_event);
  }
}

void *pFunc2(char *pCh) {
  while (1) {
    WaitForSingleObject(g_event, INFINITE);
    std::cout << *pCh << " func2 called\n";
    sleep(5);
    ResetEvent(g_event);
  }
}

int main() {
  g_event = CreateEvent(true, true);

  int iNum = 0;
  char ch = 'a';

  std::thread thread1(pFunc1, &iNum);
  std::thread thread2(pFunc2, &ch);

  thread1.detach();

  while (1) {
    iNum++;
    ch = 'a' + iNum % 26;
    sleep(1);
    if (iNum > 100) {
      iNum = 0;
    }
  }

  CloseHandle(g_event);
  return 0;
}
