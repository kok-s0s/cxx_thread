#include "Human.h"

Human::Human(std::string name) : _name(name), _sentence("") { CreateThread(); }

Human::~Human() {}

#pragma region public_member_function_to_control_private_variable_value

std::string Human::GetName() { return _name; }

void Human::SetName(const std::string name) { _name = name; }

std::string Human::GetSentence() { return _sentence; }

std::string Human::GetQuestionFromOtherPeople() { return _question.content; }

std::string Human::GetAnswerFromOtherPeople() { return _answer; }

#pragma endregion

#pragma region public_function_to_send_signal

void Human::SendSayHelloSignal() {
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      SayHello_SignalID, std::shared_ptr<std::string>(nullptr));
  SendMsg(std::move(threadMsg));
}

void Human::SendSayGoodByeSignal() {
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      SayGoodBye_SignalID, std::shared_ptr<std::string>(nullptr));
  SendMsg(std::move(threadMsg));
}

void Human::SendWillDoSignal(const std::string& doWhat) {
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(doWhat);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      WillDo_SignalID, std::static_pointer_cast<void>(msgData));
  SendMsg(std::move(threadMsg));
}

void Human::SendPlanToDoSignal(const std::string& startTime,
                               const std::string& endTime,
                               const std::string& event) {
  std::shared_ptr<Plan> msgData = std::make_shared<Plan>();
  msgData->startTime = startTime;
  msgData->endTime = endTime;
  msgData->event = event;
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      PlanToDo_SignalID, std::static_pointer_cast<void>(msgData));
  SendMsg(std::move(threadMsg));
}

void Human::SendAskAQuestionSignal(const std::shared_ptr<Human>& questioner,
                                   std::shared_ptr<Human>& respondent,
                                   const std::string& content) {
  std::shared_ptr<Question> msgData = std::make_shared<Question>();
  msgData->questioner = questioner;
  msgData->respondent = respondent;
  msgData->content = content;
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      AskAQuestion_SignalID, std::static_pointer_cast<void>(msgData));
  SendMsg(std::move(threadMsg));
}

void Human::SendGetAQuestionSignal(Question question) {
  std::shared_ptr<Question> msgData = std::make_shared<Question>(question);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      GetAQuestion_SignalID, std::static_pointer_cast<void>(msgData));
  SendMsg(std::move(threadMsg));
}

void Human::SendAnswerAQuestionSignal(const std::string& answer) {
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(answer);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      AnswerAQuestion_SignalID, std::static_pointer_cast<void>(msgData));
  SendMsg(std::move(threadMsg));
}

void Human::SendGetAAnswerSignal(const std::string& answer) {
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(answer);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      GetAAnswer_SignalID, std::static_pointer_cast<void>(msgData));
  SendMsg(std::move(threadMsg));
}

#pragma endregion

#pragma region public_function_to_control_timer

void Human::WakeUp() {
  if (_timerThread == nullptr) {
    _exitTimer = false;
    _timerThread = std::make_unique<std::thread>(&Human::TimerSlot, this);
  }
}

void Human::FellAsleep() {
  if (_timerThread != nullptr) {
    _exitTimer = true;
    std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
        ExitTimer_SignalID, std::shared_ptr<std::string>(nullptr));
    SendMsg(std::move(threadMsg));
  }
}

#pragma endregion

void Human::UserCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) {
  switch (threadMsg->GetId()) {
    case SayHello_SignalID: {
      SayHelloSlot();
      break;
    }
    case SayGoodBye_SignalID: {
      SayGoodByeSlot();
      break;
    }
    case WillDo_SignalID: {
      std::string doWhat =
          *(std::static_pointer_cast<std::string>(threadMsg->GetMsg()));
      WillDoSlot(doWhat);
      break;
    }
    case PlanToDo_SignalID: {
      auto plan = *(std::static_pointer_cast<Plan>(threadMsg->GetMsg()));
      PlanToDoSlot(plan);
      break;
    }
    case AskAQuestion_SignalID: {
      auto question =
          *(std::static_pointer_cast<Question>(threadMsg->GetMsg()));
      AskAQuestionSlot(question);
      break;
    }
    case GetAQuestion_SignalID: {
      auto question =
          *(std::static_pointer_cast<Question>(threadMsg->GetMsg()));
      GetAQuestionSlot(question);
      break;
    }
    case AnswerAQuestion_SignalID: {
      std::string answer =
          *(std::static_pointer_cast<std::string>(threadMsg->GetMsg()));
      AnswerAQuestionSlot(answer);
      break;
    }
    case GetAAnswer_SignalID: {
      std::string answer =
          *(std::static_pointer_cast<std::string>(threadMsg->GetMsg()));
      GetAAnswerSlot(answer);
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

#pragma region private_slot_function

void Human::SayHelloSlot() { _sentence = "Hello, " + _name + "!"; }

void Human::SayGoodByeSlot() { _sentence = "GoodBye, " + _name + "!"; }

void Human::WillDoSlot(const std::string& doWhat) {
  _sentence = "I'm going to go to " + doWhat;
}

void Human::PlanToDoSlot(const Plan& plan) {
  _sentence = "I plan to " + plan.event + " from " + plan.startTime + " to " +
              plan.endTime + ".";
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

void Human::WantToSleepSlot() { _sentence = "I want to sleep."; }

void Human::TimerSlot() {
  while (!_exitTimer) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    countSayWantToSleep++;

    std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
        WantToSleep_SignalID, std::shared_ptr<std::string>(nullptr));
    PostMsg(std::move(threadMsg));
  }
}

#pragma endregion
