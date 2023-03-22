#include "Human.h"

void Human::SayHelloSlot() { _sentence = "Hello, " + _name + "!"; }

void Human::PlanToDoSlot(const Plan& plan) {
  _sentence = "I plan to " + plan.event + " from " + plan.startTime + " to " +
              plan.endTime + ".";
}

void Human::WillDoSlot(const std::string& doWhat) {
  _sentence = "I'm going to go to " + doWhat;
}

void Human::AskAQuestionSlot(const Question& question) {
  _sentence = question.respondent->GetName() + ", " + question.content;
  question.respondent->SendGetAQuestionSignal(question);
}

void Human::GetAQuestionSlot(const Question& question) { _question = question; }

void Human::AnswerAQuestionSlot(const std::string& answer) {
  _sentence = _question.questioner->GetName() + ", " + answer;
  _question.questioner->SendGetAAnswerSignal(answer);
}

void Human::GetAAnswerSlot(const std::string& answer) { _answer = answer; }

void Human::SayGoodByeSlot() { _sentence = "GoodBye, " + _name + "!"; }

void Human::WantToSleepSlot() { _sentence = "I want to sleep."; }

void Human::TimerSlot() {
  while (!_exitTimer) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    countSayWantToSleep++;

    std::shared_ptr<ThreadMsg> threadMsg(
        new ThreadMsg(WantToSleep_SignalID, nullptr));
    PostMsg(threadMsg);
  }
}

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
      AskAQuestionSlot(question);
      break;
    }
    case GetAQuestion_SignalID: {
      auto question = *(std::static_pointer_cast<Question>(threadMsg->_msg));
      GetAQuestionSlot(question);
      break;
    }
    case AnswerAQuestion_SignalID: {
      std::string answer =
          *(std::static_pointer_cast<std::string>(threadMsg->_msg));
      AnswerAQuestionSlot(answer);
      break;
    }
    case GetAAnswer_SignalID: {
      std::string answer =
          *(std::static_pointer_cast<std::string>(threadMsg->_msg));
      GetAAnswerSlot(answer);
      break;
    }
    case SayGoodBye_SignalID: {
      SayGoodByeSlot();
      break;
    }
    case WantToSleep_SignalID: {
      WantToSleepSlot();
      break;
    }
    case ExitTimer_SignalID: {
      _timerThread->join();
      _timerThread = nullptr;
      break;
    }
  }
}

Human::Human(std::string name) : _name(name), _sentence("") { CreateThread(); }

Human::~Human() {}

std::string Human::GetName() { return _name; }

void Human::SetName(const std::string name) { _name = name; }

std::string Human::GetSentence() { return _sentence; }

std::string Human::GetQuestionFromOtherPeople() { return _question.content; }
std::string Human::GetAnswerFromOtherPeople() { return _answer; }

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

void Human::SendAskAQuestionSignal(const std::shared_ptr<Human>& questioner,
                                   std::shared_ptr<Human>& respondent,
                                   const std::string& content) {
  std::shared_ptr<Question> msgData(new Question());
  msgData->questioner = questioner;
  msgData->respondent = respondent;
  msgData->content = content;
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(AskAQuestion_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendGetAQuestionSignal(Question question) {
  std::shared_ptr<Question> msgData(new Question(question));
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(GetAQuestion_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendAnswerAQuestionSignal(const std::string& answer) {
  std::shared_ptr<std::string> msgData(new std::string(answer));
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(AnswerAQuestion_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendGetAAnswerSignal(const std::string& answer) {
  std::shared_ptr<std::string> msgData(new std::string(answer));
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(GetAAnswer_SignalID, msgData));
  SendMsg(threadMsg);
}

void Human::SendSayGoodByeSignal() {
  std::shared_ptr<ThreadMsg> threadMsg(
      new ThreadMsg(SayGoodBye_SignalID, nullptr));
  SendMsg(threadMsg);
}

void Human::WakeUp() {
  if (_timerThread == nullptr) {
    _exitTimer = false;
    _timerThread =
        std::unique_ptr<std::thread>(new std::thread(&Human::TimerSlot, this));
  }
}

void Human::FellAsleep() {
  if (_timerThread != nullptr) {
    _exitTimer = true;
    std::shared_ptr<ThreadMsg> threadMsg(
        new ThreadMsg(ExitTimer_SignalID, nullptr));
    SendMsg(threadMsg);
  }
}
