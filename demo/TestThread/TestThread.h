#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>

enum class ThreadStatus { Creation, Ready, Running, Paused, Finished };

class TestThread {
private:
  ThreadStatus _status = ThreadStatus::Creation;
  std::unique_ptr<std::thread> _thread;
  std::mutex _mutex;
  std::condition_variable _condition_variable;
  std::atomic<bool> _thread_pause_flag;
  std::atomic<bool> _thread_stop_flag;
  std::function<void()> _func;

public:
  TestThread(const TestThread &) = delete;
  TestThread &operator=(const TestThread &) = delete;
  TestThread(std::function<void()> func);
  ~TestThread();
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