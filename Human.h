#ifndef Human_H_
#define Human_H_
#include <iostream>
#include <string>

#include "ThreadBase.h"

class Human : public ThreadBase {
  enum SignalID : int {
    SayHello_SignalID,
    ToDo_SignalID,
    SayGoodBye_SignalID,
  };

 private:
  std::string _name;
  std::string _sentence;

  void SayHelloSlot();
  void TodoSlot(std::string something);
  void SayGoodByeSlot();

 protected:
  virtual void UserCustomFunction(
      std::shared_ptr<ThreadMsg> threadMsg) override;

 public:
  Human(std::string name);
  ~Human();

  std::string GetSentence();
  void SendSayHelloSignal();
  void SendTodoSignal(std::string something);
  void SendSayGoodByeSignal();
};

#endif  // Human_H_
