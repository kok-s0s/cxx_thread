#ifndef THREADBASE_H_
#define THREADBASE_H_
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

struct ThreadMsg {
  int _id = -1;  // -1:exit thread
  std::shared_ptr<void> _msg;
  bool _wait = false;  // false:post/asynchronous, true:send/synchronous

  ThreadMsg(int id, std::shared_ptr<void> msg) {
    _id = id;
    _msg = msg;
  }
};

class ThreadBase {
 private:
  std::unique_ptr<std::thread> _thread;
  std::mutex _mutex;
  std::condition_variable _cv;
  std::queue<std::shared_ptr<ThreadMsg>> _queue;
  bool _syncProcessed;
  const int exit_SignalID = -1;

 protected:
  /// Build the relationship between the signal and the slot
  virtual void UserCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) = 0;

 private:
  /// Entry point for the worker thread
  void Process();

 public:
  ThreadBase(const ThreadBase &) = delete;
  ThreadBase &operator=(const ThreadBase &) = delete;

  /// Constructor
  ThreadBase();

  /// Destructor
  ~ThreadBase();

  /// Called once to create the worker thread
  /// @return True if thread is created. False otherwise.
  bool CreateThread();

  /// Called once a program exit to exit the worker thread
  void ExitThread();

  /// Get the ID of this thread instance
  /// @return The worker thread ID
  std::thread::id GetThreadId();

  /// Get the ID of the currently executing thread
  /// @return The current thread ID
  static std::thread::id GetCurrentThreadId();

  /// Post a message to the thread queue
  /// @param[in] data - thread specific message information
  void PostMsg(std::shared_ptr<ThreadMsg> threadMsg);

  /// Send a message to the thread queue
  /// @param[in] data - thread specific message information
  void SendMsg(std::shared_ptr<ThreadMsg> threadMsg);
};

#endif  // THREADBASE_H_
