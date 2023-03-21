#include "Human.h"

void Human::SayHelloSlot() { _sentence = "Hello, " + _name + "!"; }

void Human::TodoSlot(std::string doWhat) {
  _sentence = "I'm going to go to " + doWhat;
}

void Human::SayGoodByeSlot() { _sentence = "GoodBye, " + _name + "!"; }

void Human::UserCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) {
  switch (threadMsg->_id) {
    case SayHello_SignalID: {
      SayHelloSlot();
      break;
    }
    case ToDo_SignalID: {
      std::string doWhat =
          *(std::static_pointer_cast<std::string>(threadMsg->_msg));
      TodoSlot(doWhat);
      break;
    }
    case SayGoodBye_SignalID: {
      SayGoodByeSlot();
      break;
    }
  }
}

Human::Human(std::string name) : _name(name), _sentence("") { CreateThread(); }

Human::~Human() {}

std::string Human::GetSentence() { return _sentence; }

void Human::SendSayHelloSignal() {
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(SayHello_SignalID, nullptr));
  SendMsg(threadMsg);
}

void Human::SendTodoSignal(std::string doWhat) {
  std::shared_ptr<std::string> msgData(new std::string(doWhat));
  std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(ToDo_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendSayGoodByeSignal() {
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(SayGoodBye_SignalID, nullptr));
  SendMsg(threadMsg);
}