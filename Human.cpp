#include "Human.h"

void Human::SayHelloSlot() { _sentence = "Hello, " + _name + "!"; }

void Human::PlanToDoSlot(Plan plan) {
  _sentence = "I plan to " + plan.event + " from " + plan.startTime + " to " +
              plan.endTime + ".";
}

void Human::WillDoSlot(std::string doWhat) {
  _sentence = "I'm going to go to " + doWhat;
}

void Human::AskAQuestion(Question question) {
  _sentence = question.respondentName + ", " + question.content;
}

void Human::AnswerAQuestion(Answer answer) {
  _sentence = answer.questionerName + ", " + answer.content;
}

void Human::SayGoodByeSlot() { _sentence = "GoodBye, " + _name + "!"; }

void Human::UserCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) {
  switch (threadMsg->_id) {
    case SayHello_SignalID: {
      SayHelloSlot();
      break;
    }
    case PlanToDo_SignalID: {
      auto plan = *(std::static_pointer_cast<Plan>(threadMsg->_msg));
      PlanToDoSlot(plan);
      break;
    }
    case WillDo_SignalID: {
      std::string doWhat =
          *(std::static_pointer_cast<std::string>(threadMsg->_msg));
      WillDoSlot(doWhat);
      break;
    }
    case AskAQuestion_SignalID: {
      auto question = *(std::static_pointer_cast<Question>(threadMsg->_msg));
      AskAQuestion(question);
      break;
    }
    case AnswerAQuestion_SignalID: {
      auto answer = *(std::static_pointer_cast<Answer>(threadMsg->_msg));
      AnswerAQuestion(answer);
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

void Human::SendPlanToDoSignal(const std::string& startTime,
                               const std::string& endTime,
                               const std::string& event) {
  std::shared_ptr<Plan> msgData(new Plan());
  msgData->startTime = startTime;
  msgData->endTime = endTime;
  msgData->event = event;
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(PlanToDo_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendWillDoSignal(const std::string& doWhat) {
  std::shared_ptr<std::string> msgData(new std::string(doWhat));
  std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(WillDo_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendAskAQuestionSignal(const std::string& respondentName,
                                   const std::string& content) {
  std::shared_ptr<Question> msgData(new Question());
  msgData->respondentName = respondentName;
  msgData->content = content;
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(AskAQuestion_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendAnswerAQuestionSignal(const std::string& questionerName,
                                      const std::string& content) {
  std::shared_ptr<Answer> msgData(new Answer());
  msgData->questionerName = questionerName;
  msgData->content = content;
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(AnswerAQuestion_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendSayGoodByeSignal() {
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(SayGoodBye_SignalID, nullptr));
  SendMsg(threadMsg);
}