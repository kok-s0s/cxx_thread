#ifndef Human_H_
#define Human_H_
#include <iostream>
#include <string>

#include "ThreadBase.h"

class Human : public ThreadBase {
  struct Plan {
    std::string startTime;
    std::string endTime;
    std::string event;
  };

  struct Question {
    std::shared_ptr<Human> questioner;
    std::shared_ptr<Human> respondent;
    std::string content;
  };

  struct Answer {
    std::string questionerName;
    std::string content;
  };

  enum SignalID : int {
    SayHello_SignalID,
    PlanToDo_SignalID,
    WillDo_SignalID,
    AskAQuestion_SignalID,
    GetAQuestion_SignalID,
    AnswerAQuestion_SignalID,
    SayGoodBye_SignalID,
    WantToSleep_SignalID,
    ExitTimer_SignalID
  };

 private:
  std::string _name;
  std::string _sentence;
  Question _question;
  std::unique_ptr<std::thread> _timerThread;
  bool _exitTimer = false;

 protected:
  virtual void UserCustomFunction(
      std::shared_ptr<ThreadMsg> threadMsg) override;

 private:
  void SayHelloSlot();
  void PlanToDoSlot(const Plan& plan);
  void WillDoSlot(const std::string& something);
  void AskAQuestionSlot(Question& question);
  void GetAQuestionSlot(const Question& question);
  void AnswerAQuestionSlot(const Answer& answer);
  void SayGoodByeSlot();
  void WantToSleepSlot();
  void TimerSlot();

 public:
  Human(std::string name);
  ~Human();
  std::string GetName();
  void SetName(const std::string name);
  std::string GetSentence();
  void SendSayHelloSignal();
  void SendPlanToDoSignal(const std::string& startTime,
                          const std::string& endTime, const std::string& event);
  void SendWillDoSignal(const std::string& doWhat);
  void SendAskAQuestionSignal(const std::shared_ptr<Human>& questioner,
                              std::shared_ptr<Human>& respondent,
                              const std::string& content);
  void SendGetAQuestionSignal(Question question);
  void SendAnswerAQuestionSignal(const std::string& questionerName,
                                 const std::string& content);
  void SendSayGoodByeSignal();

 public:
  int countSayWantToSleep = 0;

 public:
  void WakeUp();
  void FellAsleep();
};

#endif  // Human_H_
