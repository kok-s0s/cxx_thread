#ifndef THREAD_H
#define THREAD_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

enum class ThreadStatus { Creation, Ready, Running, Paused, Finished };

class Thread {
private:
  ThreadStatus _status = ThreadStatus::Creation;
  std::unique_ptr<std::thread> _thread;
  std::mutex _mutex;
  std::condition_variable _condition_variable;
  std::atomic<bool> _thread_pause_flag;
  std::atomic<bool> _thread_stop_flag;
  std::function<void()> _func;

public:
  Thread(const Thread &) = delete;
  Thread &operator=(const Thread &) = delete;
  Thread();
  ~Thread();
  void setFunc(std::function<void()> func);
  std::thread::id get_id();
  int getStatus() const;
  bool isRunning();
  bool isPaused();
  bool isFinished();
  void run();
  void start();
  void pause();
  void resume();
  void quit();
  bool detach();
  bool join();
};

#endif /* THREAD_H */