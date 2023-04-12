#include "Human.h"

#include <plog/Log.h>

Human::Human(std::string name) : _name(name), _sentence("") {
  CreateThread();
  PLOGD << "Call the constructor";
}

Human::~Human() { PLOGD << "Call the destructor"; }

#pragma region public_member_function_to_control_private_variable_value

std::string Human::GetName() { return _name; }

void Human::SetName(const std::string name) { _name = name; }

std::string Human::GetSentence() { return _sentence; }

std::string Human::GetQuestionFromOtherPeople() { return _question.content; }

std::string Human::GetAnswerFromOtherPeople() { return _question.answer; }

#pragma endregion

#pragma region public_function_to_send_signal

void Human::SendSayHelloSignal() {
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      SayHello_Signal, std::shared_ptr<std::string>(nullptr));
  SendSlotFuncSyncRunMsg(std::move(threadMsg));
}

void Human::SendSayGoodByeSignal() {
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      SayGoodBye_Signal, std::shared_ptr<std::string>(nullptr));
  SendSlotFuncSyncRunMsg(std::move(threadMsg));
}

void Human::SendWillDoSignal(const std::string& doWhat) {
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(doWhat);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      WillDo_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(threadMsg));
}

void Human::SendPlanToDoSignal(const std::string& startTime,
                               const std::string& endTime,
                               const std::string& event) {
  std::shared_ptr<Plan> msgData = std::make_shared<Plan>();
  msgData->startTime = startTime;
  msgData->endTime = endTime;
  msgData->event = event;
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      PlanToDo_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(threadMsg));
}

void Human::SendAskAQuestionSignal(std::shared_ptr<Human> respondent,
                                   const std::string& question) {
  _sentence = respondent->GetName() + ", " + question;
  std::shared_ptr<std::string> msgData =
      std::make_shared<std::string>(question);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      AskAQuestion_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(threadMsg));
  respondent->SendGetAQuestionSignal(question);
  respondent = nullptr;
}

void Human::SendGetAQuestionSignal(const std::string& question) {
  std::shared_ptr<std::string> msgData =
      std::make_shared<std::string>(question);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      GetAQuestion_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(threadMsg));
}

void Human::SendAnswerAQuestionSignal(std::shared_ptr<Human> questioner,
                                      const std::string& answer) {
  _sentence = questioner->GetName() + ", " + answer;
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(answer);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      AnswerAQuestion_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(threadMsg));
  questioner->SendGetAAnswerSignal(answer);
  questioner = nullptr;
}

void Human::SendGetAAnswerSignal(const std::string& answer) {
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(answer);
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      GetAAnswer_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(threadMsg));
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
        ExitTimer_Signal, std::shared_ptr<std::string>(nullptr));
    SendSlotFuncSyncRunMsg(std::move(threadMsg));
  }
}

#pragma endregion

void Human::UserCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) {
  switch (threadMsg->GetSignal()) {
    case SayHello_Signal: {
      SayHelloSlot();
      break;
    }
    case SayGoodBye_Signal: {
      SayGoodByeSlot();
      break;
    }
    case WillDo_Signal: {
      std::string doWhat =
          *(std::static_pointer_cast<std::string>(threadMsg->GetMsg()));
      WillDoSlot(doWhat);
      break;
    }
    case PlanToDo_Signal: {
      auto plan = *(std::static_pointer_cast<Plan>(threadMsg->GetMsg()));
      PlanToDoSlot(plan);
      break;
    }
    case AskAQuestion_Signal: {
      auto question =
          *(std::static_pointer_cast<std::string>(threadMsg->GetMsg()));
      AskAQuestionSlot(question);
      break;
    }
    case GetAQuestion_Signal: {
      auto question =
          *(std::static_pointer_cast<std::string>(threadMsg->GetMsg()));
      GetAQuestionSlot(question);
      break;
    }
    case AnswerAQuestion_Signal: {
      std::string answer =
          *(std::static_pointer_cast<std::string>(threadMsg->GetMsg()));
      AnswerAQuestionSlot(answer);
      break;
    }
    case GetAAnswer_Signal: {
      std::string answer =
          *(std::static_pointer_cast<std::string>(threadMsg->GetMsg()));
      GetAAnswerSlot(answer);
      break;
    }
    case WantToSleep_Signal: {
      WantToSleepSlot();
      break;
    }
    case ExitTimer_Signal: {
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

void Human::AskAQuestionSlot(const std::string& question) {
  _question.content = question;
}

void Human::GetAQuestionSlot(const std::string& question) {
  _question.content = question;
}

void Human::AnswerAQuestionSlot(const std::string& answer) {
  _question.answer = answer;
}

void Human::GetAAnswerSlot(const std::string& answer) {
  _question.answer = answer;
}

void Human::WantToSleepSlot() { _sentence = "I want to sleep."; }

void Human::TimerSlot() {
  while (!_exitTimer) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    countSayWantToSleep++;

    std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
        WantToSleep_Signal, std::shared_ptr<std::string>(nullptr));
    SendSlotFuncAsyncRunMsg(std::move(threadMsg));
  }
}

#pragma endregion
