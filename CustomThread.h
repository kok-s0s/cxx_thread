#ifndef CUSTOMTHREAD_H_
#define CUSTOMTHREAD_H_
#include <iostream>
#include <string>

#include "ThreadBase.hpp"

using namespace std;

class CustomThread : public ThreadBasic {
  enum SignalID : int {
    SayHello_SignalID,
    ToDo_SignalID,
    SayGoodBye_SignalID,
  };

 private:
  string _name;

  void sayHelloSlot();

  void todoSlot(string something);

  void sayGoodByeSlot();

 protected:
  virtual void userCustomFunction(
      std::shared_ptr<ThreadMsg> threadMsg) override;

 public:
  CustomThread(string name);
  ~CustomThread();

  void sendSayHelloSignal();

  void sendTodoSignal(string something);

  void sendSayGoodByeSignal();
};

#endif  // CUSTOMTHREAD_H_
