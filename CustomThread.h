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

 public:
  CustomThread(string name);
  ~CustomThread();

  void sayHello();

  void todo(string something);

  void sayGoodBye();

 protected:
  virtual void userCustomFunction(
      std::shared_ptr<ThreadMsg> threadMsg) override;
};

#endif  // CUSTOMTHREAD_H_
