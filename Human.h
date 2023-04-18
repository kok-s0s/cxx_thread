#ifndef Human_H_
#define Human_H_

#include <iostream>
#include <string>

#include "ThreadBase.h"

struct Plan {
  std::string startTime;
  std::string endTime;
  std::string event;
};

struct Question {
  std::string content;
  std::string answer;
};

class Human : public ThreadBase {
  enum Signal : int {
    SayHello_Signal,
    SayGoodBye_Signal,
    WillDo_Signal,
    PlanToDo_Signal,
    AskAQuestion_Signal,
    GetAQuestion_Signal,
    AnswerAQuestion_Signal,
    GetAAnswer_Signal,
    WantToSleep_Signal,
  };

 public:
  Human(std::string name);
  ~Human();
  std::string GetName();
  std::string GetSentence();
  std::string GetQuestionFromOtherPeople();
  std::string GetAnswerFromOtherPeople();
  int GetCountSayWantToSleep();
  void SendSayHelloSignal();
  void SendSayGoodByeSignal();
  void SendWillDoSignal(const std::string& doWhat);
  void SendPlanToDoSignal(const Plan& plan);
  void SendAskAQuestionSignal(std::shared_ptr<Human> respondent,
                              const std::string& content);
  void SendGetAQuestionSignal(const std::string& question);
  void SendAnswerAQuestionSignal(std::shared_ptr<Human> questioner,
                                 const std::string& answer);
  void SendGetAAnswerSignal(const std::string& answer);

 public:
  void CreateTimer();
  void DestroyTimer();

 protected:
  virtual void UserCustomFunction(
      std::shared_ptr<SignalMsg> signalMsg) override;

 private:
  void SayHelloSlot();
  void SayGoodByeSlot();
  void WillDoSlot(const std::string& something);
  void PlanToDoSlot(const Plan& plan);
  void AskAQuestionSlot(const std::string& question);
  void GetAQuestionSlot(const std::string& question);
  void AnswerAQuestionSlot(const std::string& answer);
  void GetAAnswerSlot(const std::string& answer);
  void WantToSleepSlot();

 private:
  void TimedTask();

 private:
  std::string _name;
  std::string _sentence;
  Question _question;
  int _countSayWantToSleep = 0;
  std::unique_ptr<std::thread> _timerThread;
  bool _destroyTimer = false;
};

#endif  // Human_H_
