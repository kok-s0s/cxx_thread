#include "CustomThread.h"

void CustomThread::sayHelloSlot() { cout << "Hello! " << _name << endl; }

void CustomThread::todoSlot(string something) { cout << something << endl; }

void CustomThread::sayGoodByeSlot() { cout << "GoodBye! " << _name << endl; }

void CustomThread::userCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) {
  switch (threadMsg->_id) {
    case SayHello_SignalID: {
      sayHelloSlot();
      break;
    }
    case ToDo_SignalID: {
      string something = *(std::static_pointer_cast<string>(threadMsg->_msg));
      todoSlot(something);
      break;
    }
    case SayGoodBye_SignalID: {
      sayGoodByeSlot();
      break;
    }
  }
}

CustomThread::CustomThread(string name) : _name(name) { createThread(); }

CustomThread::~CustomThread() {}

void CustomThread::sendSayHelloSignal() {
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(SayHello_SignalID, nullptr));
  sendMsg(threadMsg);
}

void CustomThread::sendTodoSignal(string something) {
  std::shared_ptr<string> msgData(new string(something));
  std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(ToDo_SignalID, msgData));
  sendMsg(threadMsg);
}

void CustomThread::sendSayGoodByeSignal() {
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(SayGoodBye_SignalID, nullptr));
  sendMsg(threadMsg);
}