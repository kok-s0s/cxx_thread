#include <functional>
#include <iostream>
#include <thread>

enum ThreadStatus {
  Creation,
  Ready,
  Running,
  Waiting,
  Delayed,
  Blocked,
  Finished
};

class TestThread {
private:
  std::thread _thread;
  int _status = ThreadStatus::Creation;

public:
  TestThread(const TestThread &) = delete;
  TestThread &operator=(const TestThread &) = delete;
  TestThread(std::function<void()> func);
  TestThread(std::function<void(bool &flag)> func,
             std::reference_wrapper<bool> flag);
  TestThread(TestThread &&obj);
  TestThread &operator=(TestThread &&obj);
  ~TestThread();
  std::thread::id get_id();
  int getStatus() const;
  bool isRunning();
  bool isWaiting();
  bool isDelayed();
  bool isBlocked();
  bool isFinished();
  void start();
  bool detach();
  bool join();
  void quit();
};