#include "Human.h"

#include <plog/Log.h>

Human::Human(std::string name) : _name(name), _sentence("") {
  CreateThread();
  PLOGD << "Call the constructor";
}

Human::~Human() { PLOGD << "Call the destructor"; }

#pragma region public_member_function_to_get_private_member_variable_value

std::string Human::GetName() { return _name; }

std::string Human::GetSentence() { return _sentence; }

std::string Human::GetQuestionFromOtherPeople() { return _question.content; }

std::string Human::GetAnswerFromOtherPeople() { return _question.answer; }

int Human::GetCountSayWantToSleep() { return _countSayWantToSleep; }

#pragma endregion

#pragma region public_function_to_send_signal

void Human::SendSayHelloSignal() {
  PLOGD << "SendSayHelloSignal";
  std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
      SayHello_Signal, std::shared_ptr<std::string>(nullptr));
  SendSlotFuncSyncRunMsg(std::move(signalMsg));
}

void Human::SendSayGoodByeSignal() {
  PLOGD << "SendSayGoodByeSignal";
  std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
      SayGoodBye_Signal, std::shared_ptr<std::string>(nullptr));
  SendSlotFuncSyncRunMsg(std::move(signalMsg));
}

void Human::SendWillDoSignal(const std::string& doWhat) {
  PLOGD << "SendWillDoSignal";
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(doWhat);
  std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
      WillDo_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(signalMsg));
}

void Human::SendPlanToDoSignal(const Plan& plan) {
  PLOGD << "SendPlanToDoSignal";
  std::shared_ptr<Plan> msgData = std::make_shared<Plan>(plan);
  std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
      PlanToDo_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(signalMsg));
}

void Human::SendAskAQuestionSignal(std::shared_ptr<Human> respondent,
                                   const std::string& question) {
  PLOGD << "SendAskAQuestionSignal";
  _sentence = respondent->GetName() + ", " + question;
  std::shared_ptr<std::string> msgData =
      std::make_shared<std::string>(question);
  std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
      AskAQuestion_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(signalMsg));
  respondent->SendGetAQuestionSignal(question);
  respondent = nullptr;
}

void Human::SendGetAQuestionSignal(const std::string& question) {
  PLOGD << "SendGetAQuestionSignal";
  std::shared_ptr<std::string> msgData =
      std::make_shared<std::string>(question);
  std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
      GetAQuestion_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(signalMsg));
}

void Human::SendAnswerAQuestionSignal(std::shared_ptr<Human> questioner,
                                      const std::string& answer) {
  PLOGD << "SendAnswerAQuestionSignal";
  _sentence = questioner->GetName() + ", " + answer;
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(answer);
  std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
      AnswerAQuestion_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(signalMsg));
  questioner->SendGetAAnswerSignal(answer);
  questioner = nullptr;
}

void Human::SendGetAAnswerSignal(const std::string& answer) {
  PLOGD << "SendGetAAnswerSignal";
  std::shared_ptr<std::string> msgData = std::make_shared<std::string>(answer);
  std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
      GetAAnswer_Signal, std::static_pointer_cast<void>(msgData));
  SendSlotFuncSyncRunMsg(std::move(signalMsg));
}

#pragma endregion

#pragma region public_function_to_control_timer

void Human::CreateTimer() {
  if (_timerThread == nullptr) {
    PLOGD << "CreateTimer";
    _destroyTimer = false;
    _timerThread = std::make_unique<std::thread>(&Human::TimedTask, this);
  }
}

void Human::DestroyTimer() {
  if (_timerThread != nullptr) {
    PLOGD << "DestroyTimer";
    _destroyTimer = true;
    _timerThread->join();
    _timerThread = nullptr;
  }
}

#pragma endregion

void Human::UserCustomFunction(std::shared_ptr<SignalMsg> signalMsg) {
  PLOGD << "UserCustomFunction"
        << " -- " << signalMsg->GetSignal();
  switch (signalMsg->GetSignal()) {
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
          *(std::static_pointer_cast<std::string>(signalMsg->GetMsg()));
      WillDoSlot(doWhat);
      break;
    }
    case PlanToDo_Signal: {
      auto plan = *(std::static_pointer_cast<Plan>(signalMsg->GetMsg()));
      PlanToDoSlot(plan);
      break;
    }
    case AskAQuestion_Signal: {
      auto question =
          *(std::static_pointer_cast<std::string>(signalMsg->GetMsg()));
      AskAQuestionSlot(question);
      break;
    }
    case GetAQuestion_Signal: {
      auto question =
          *(std::static_pointer_cast<std::string>(signalMsg->GetMsg()));
      GetAQuestionSlot(question);
      break;
    }
    case AnswerAQuestion_Signal: {
      std::string answer =
          *(std::static_pointer_cast<std::string>(signalMsg->GetMsg()));
      AnswerAQuestionSlot(answer);
      break;
    }
    case GetAAnswer_Signal: {
      std::string answer =
          *(std::static_pointer_cast<std::string>(signalMsg->GetMsg()));
      GetAAnswerSlot(answer);
      break;
    }
    case WantToSleep_Signal: {
      WantToSleepSlot();
      break;
    }
  }
}

#pragma region private_slot_function

void Human::SayHelloSlot() {
  PLOGD << "SayHelloSlot";
  _sentence = "Hello, " + _name + "!";
}

void Human::SayGoodByeSlot() {
  PLOGD << "SayGoodByeSlot";
  _sentence = "GoodBye, " + _name + "!";
}

void Human::WillDoSlot(const std::string& doWhat) {
  PLOGD << "WillDoSlot";
  _sentence = "I'm going to go to " + doWhat;
}

void Human::PlanToDoSlot(const Plan& plan) {
  PLOGD << "PlanToDoSlot";
  _sentence = "I plan to " + plan.event + " from " + plan.startTime + " to " +
              plan.endTime + ".";
}

void Human::AskAQuestionSlot(const std::string& question) {
  PLOGD << "AskAQuestionSlot";
  _question.content = question;
}

void Human::GetAQuestionSlot(const std::string& question) {
  PLOGD << "GetAQuestionSlot";
  _question.content = question;
}

void Human::AnswerAQuestionSlot(const std::string& answer) {
  PLOGD << "AnswerAQuestionSlot";
  _question.answer = answer;
}

void Human::GetAAnswerSlot(const std::string& answer) {
  PLOGD << "GetAAnswerSlot";
  _question.answer = answer;
}

void Human::WantToSleepSlot() {
  PLOGD << "WantToSleepSlot";
  _sentence = "I want to sleep.";
}

void Human::TimedTask() {
  while (!_destroyTimer) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    _countSayWantToSleep++;
    PLOGD << "TimedTask -- _countSayWantToSleep: " << _countSayWantToSleep;

    std::shared_ptr<SignalMsg> signalMsg = std::make_shared<SignalMsg>(
        WantToSleep_Signal, std::shared_ptr<std::string>(nullptr));
    SendSlotFuncAsyncRunMsg(std::move(signalMsg));
  }
  PLOGD << "TimedTask -- destroy";
}

#pragma endregion
