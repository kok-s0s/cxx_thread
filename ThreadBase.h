#ifndef THREADBASE_H_
#define THREADBASE_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

struct ThreadMsg {
 public:
  ThreadMsg(int signal, std::shared_ptr<void> msg)
      : _wait(false), _signal(signal), _msg(msg) {}

  bool GetWait() const { return _wait; }
  void SetWait(bool wait) { _wait = wait; }
  int GetSignal() const { return _signal; }
  std::shared_ptr<void> GetMsg() const { return _msg; }

 private:
  bool _wait = false;          // async: false, sync: true (default: false)
  int _signal = -1;            // -1: destroy thread
  std::shared_ptr<void> _msg;  // thread specific message information
};

class ThreadBase {
 public:
  ThreadBase &operator=(const ThreadBase &) = delete;  // non copyable
  ThreadBase &operator=(ThreadBase &&) = delete;       // non movable
  ThreadBase(ThreadBase &&) = delete;                  // non movable
  ThreadBase(const ThreadBase &) = delete;  // non construction-copyable

  /// Constructor
  explicit ThreadBase();

  /// Destructor
  virtual ~ThreadBase();

  /// Called once to create the worker thread
  /// @return True if thread is created. False otherwise.
  bool CreateThread();

  /// Called once to destroy the worker thread
  void DestroyThread();

  /// Get the ID of the worker thread
  /// @return The worker thread ID
  std::thread::id GetThreadId();

  /// Get the ID of the current thread
  /// @return The current thread ID
  static std::thread::id GetCurrentThreadId();

  /// Send a message to the message queue (async)
  /// @param[in] data - thread specific message information
  void SendSlotFuncAsyncRunMsg(std::shared_ptr<ThreadMsg> threadMsg);

  /// Send a message to the message queue (sync)
  /// @param[in] data - thread specific message information
  void SendSlotFuncSyncRunMsg(std::shared_ptr<ThreadMsg> threadMsg);

 protected:
  /// Build the relationship between the signal and the slot function
  virtual void UserCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) = 0;

 private:
  /// Send a message to the thread queue (async or sync)
  void SendMsg(bool wait, std::shared_ptr<ThreadMsg> threadMsg);

  /// Process the message queue
  void Process();

 private:
  const int DestroyThread_Signal = -1;

 private:
  std::unique_ptr<std::thread> _thread;
  std::mutex _mutex;
  std::condition_variable _cv;
  std::queue<std::shared_ptr<ThreadMsg>> _queue;
  bool _syncProcessed = false;
};

#endif  // THREADBASE_H_
