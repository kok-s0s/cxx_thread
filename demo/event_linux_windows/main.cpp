#include "Hik_Event.h"
#include <assert.h>
#include <iostream>
#include <unistd.h>

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

void *pFunc1(void *pEvent) {
  while (1) {
    WaitForSingleObject(g_event, INFINITE);
    printf("this is func1 print!\n");
    sleep(1);
    ResetEvent(g_event);
  }
}

void *pFunc2(void *pEvent) {
  while (1) {
    sleep(5);
    printf("this is func2 print!\n");
    SetEvent(g_event);
  }
}

int main() {
  // g_event = CreateEvent(false, true);
  g_event = CreateEvent(true, true);

  pthread_t pid1;
  pthread_t pid2;

  pthread_create(&pid1, NULL, pFunc1, NULL);
  pthread_create(&pid2, NULL, pFunc2, NULL);

  pthread_join(pid1, NULL);
  pthread_join(pid2, NULL);

  CloseHandle(g_event);
  return 0;
}
