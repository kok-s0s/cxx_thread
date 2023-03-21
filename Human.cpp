#include "Human.h"

void Human::sayHelloSlot() { _sentence = "Hello, " + _name + "!"; }

void Human::todoSlot(std::string doWhat) {
  _sentence = "I'm going to go to " + doWhat;
}

void Human::sayGoodByeSlot() { _sentence = "GoodBye, " + _name + "!"; }

void Human::userCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) {
  switch (threadMsg->_id) {
    case SayHello_SignalID: {
      sayHelloSlot();
      break;
    }
    case ToDo_SignalID: {
      std::string doWhat =
          *(std::static_pointer_cast<std::string>(threadMsg->_msg));
      todoSlot(doWhat);
      break;
    }
    case SayGoodBye_SignalID: {
      sayGoodByeSlot();
      break;
    }
  }
}

Human::Human(std::string name) : _name(name), _sentence("") { createThread(); }

Human::~Human() {}

std::string Human::getSentence() { return _sentence; }

void Human::sendSayHelloSignal() {
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(SayHello_SignalID, nullptr));
  sendMsg(threadMsg);
}

void Human::sendTodoSignal(std::string doWhat) {
  std::shared_ptr<std::string> msgData(new std::string(doWhat));
  std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(ToDo_SignalID, msgData));
  sendMsg(threadMsg);
}

void Human::sendSayGoodByeSignal() {
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(SayGoodBye_SignalID, nullptr));
  sendMsg(threadMsg);
}