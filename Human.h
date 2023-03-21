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
    std::string respondentName;
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
    AnswerAQuestion_SignalID,
    SayGoodBye_SignalID,
    WantToSleep_SignalID,
    ExitTimer_SignalID
  };

 private:
  std::string _name;
  std::string _sentence;
  bool _timerExit = false;
  std::unique_ptr<std::thread> _timerThread;

 protected:
  virtual void UserCustomFunction(
      std::shared_ptr<ThreadMsg> threadMsg) override;

 private:
  void SayHelloSlot();
  void PlanToDoSlot(Plan plan);
  void WillDoSlot(std::string something);
  void AskAQuestion(Question question);
  void AnswerAQuestion(Answer answer);
  void SayGoodByeSlot();
  void WantToSleepSlot();
  void timerSlot();

 public:
  Human(std::string name);
  ~Human();

  std::string GetSentence();
  void SendSayHelloSignal();
  void SendPlanToDoSignal(const std::string& startTime,
                          const std::string& endTime, const std::string& event);
  void SendWillDoSignal(const std::string& doWhat);
  void SendAskAQuestionSignal(const std::string& respondentName,
                              const std::string& content);
  void SendAnswerAQuestionSignal(const std::string& questionerName,
                                 const std::string& content);
  void SendSayGoodByeSignal();

 public:
  int countSayWantToSleep = 0;

 public:
  void FellAsleep();
  void WakeUp();
};

#endif  // Human_H_
