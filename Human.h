#ifndef Human_H_
#define Human_H_
#include <iostream>
#include <string>

#include "ThreadBase.hpp"

class Human : public ThreadBasic {
  enum SignalID : int {
    SayHello_SignalID,
    ToDo_SignalID,
    SayGoodBye_SignalID,
  };

 private:
  std::string _name;
  std::string _sentence;

  void sayHelloSlot();
  void todoSlot(std::string something);
  void sayGoodByeSlot();

 protected:
  virtual void userCustomFunction(
      std::shared_ptr<ThreadMsg> threadMsg) override;

 public:
  Human(std::string name);
  ~Human();

  std::string getSentence();
  void sendSayHelloSignal();
  void sendTodoSignal(std::string something);
  void sendSayGoodByeSignal();
};

#endif  // Human_H_
