#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "event.h"
CEvent *g_event = NULL;

CEvent *CreateEvent(bool bManualReset, bool bInitialState) {
  CEvent *pEvent = new CEvent(bManualReset, bInitialState);
  assert(pEvent);

  bool bRet = pEvent->CreateEvent();
  assert(bRet);

  return pEvent;
}

unsigned int WaitForSingleObject(CEvent *pEvent, int cms) {
  assert(pEvent);
  if (pEvent->Wait(cms)) {
    return 0;
  }

  return 1;
}

void CloseHandle(CEvent *pEvent) { delete pEvent; }

void SetEvent(CEvent *pEvent) { pEvent->Set(); }

void ResetEvent(CEvent *pEvent) { pEvent->Reset(); }

void *pFunc1(void *pEvent) {
  while (1) {
    WaitForSingleObject(g_event, INFINITE);
    sleep(1);
    printf("this is func1 print!\n");
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
