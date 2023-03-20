#include "CustomThread.h"

CustomThread::CustomThread(string name) : _name(name) { createThread(); }

CustomThread::~CustomThread() {}

void CustomThread::sayHello() { cout << "Hello! " << _name << endl; }

void CustomThread::todo(string something) {}

void CustomThread::sayGoodBye() { cout << "GoodBye! " << _name << endl; }

void CustomThread::userCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) {
  switch (threadMsg->_id) {
    case SayHello_SignalID: {
      sayHello();
      break;
    }
    case ToDo_SignalID: {
      string something = *(std::static_pointer_cast<string>(threadMsg->_msg));
      todo(something);
      break;
    }
    case SayGoodBye_SignalID: {
      sayGoodBye();
      break;
    }
  }
}