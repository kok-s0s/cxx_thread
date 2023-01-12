#include <assert.h>
#include <unistd.h>

#include <iostream>
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

// void *pFunc1(void *pEvent) {
//   while (1) {
//     WaitForSingleObject(g_event, INFINITE);
//     printf("this is func1 print!\n");
//     sleep(1);
//     ResetEvent(g_event);
//   }
// }

// void *pFunc2(void *pEvent) {
//   while (1) {
//     sleep(5);
//     printf("this is func2 print!\n");
//     SetEvent(g_event);
//   }
// }

void *pFunc1(int *pNum) {
  while (1) {
    sleep(1);
    int num = *pNum;
    std::cout << num;
    std::cout << " func1 called\n";
    SetEvent(g_event);
  }
}

int main() {
  g_event = CreateEvent(true, true);

  int iNum = 0;

  std::thread thread1(pFunc1, &iNum);

  thread1.detach();

  while (1) {
    iNum++;
    sleep(1);
    if (iNum > 100) {
      iNum = 0;
    }
  }

  CloseHandle(g_event);
  return 0;
}
