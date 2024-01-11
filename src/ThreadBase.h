#ifndef THREADBASE_H_
#define THREADBASE_H_

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

struct SignalMsg {
 public:
  SignalMsg(int signal, std::shared_ptr<void> msg)
      : _wait(false), _signal(signal), _msg(msg) {}

  bool GetWait() const { return _wait; }
  void SetWait(bool wait) { _wait = wait; }
  int GetSignal() const { return _signal; }
  std::shared_ptr<void> GetMsg() const { return _msg; }

 private:
  bool _wait = false;
  int _signal = -1;
  std::shared_ptr<void> _msg;
};

/**
 * @brief The ThreadBase class is a base class for creating threads in C++.
 *
 * This class provides functionality for creating and managing threads. It
 * allows users to define their own custom function that will be executed in a
 * separate thread.
 *
 * The class also provides methods for sending messages to the thread and
 * retrieving information about the thread.
 */
class ThreadBase {
 public:
  /**
   * @brief Deleted copy assignment operator.
   */
  ThreadBase &operator=(const ThreadBase &) = delete;

  /**
   * @brief Deleted copy constructor.
   */
  ThreadBase(const ThreadBase &) = delete;

  /**
   * @brief Deleted move assignment operator.
   */
  ThreadBase &operator=(ThreadBase &&) = delete;

  /**
   * @brief Deleted move constructor.
   */
  ThreadBase(ThreadBase &&) = delete;

  /**
   * @brief Constructs a ThreadBase object.
   */
  explicit ThreadBase();

  /**
   * @brief Destroys the ThreadBase object.
   */
  virtual ~ThreadBase();

  /**
   * @brief Creates a new thread and starts executing the custom function.
   *
   * @return true if the thread was successfully created, false otherwise.
   */
  bool CreateThread();

  /**
   * @brief Gets the ID of the thread.
   *
   * @return The ID of the thread.
   */
  std::thread::id GetThreadId();

  /**
   * @brief Gets the ID of the current thread.
   *
   * @return The ID of the current thread.
   */
  std::thread::id GetCurrentThreadId();

  /**
   * @brief Sends a signal message to the thread and runs the custom function
   * asynchronously.
   *
   * @param signalMsg The signal message to send.
   */
  void SendSlotFuncAsyncRunMsg(std::shared_ptr<SignalMsg> signalMsg);

  /**
   * @brief Sends a signal message to the thread and runs the custom function
   * synchronously.
   *
   * @param signalMsg The signal message to send.
   */
  void SendSlotFuncSyncRunMsg(std::shared_ptr<SignalMsg> signalMsg);

 protected:
  /**
   * @brief This is a pure virtual function that should be overridden by derived
   * classes.
   *
   * This function is called when a signal message is received and should be
   * implemented to perform the desired functionality in the separate thread.
   *
   * @param signalMsg The signal message received.
   */
  virtual void UserCustomFunction(std::shared_ptr<SignalMsg> signalMsg) = 0;

 private:
  /**
   * @brief Sends a message to the thread.
   *
   * @param wait If true, the calling thread will wait until the message is
   * processed.
   * @param signalMsg The signal message to send.
   */
  void SendMsg(bool wait, std::shared_ptr<SignalMsg> signalMsg);

  /**
   * @brief Processes the signal messages in the queue.
   */
  void Process();

 private:
  const int DestroyThread_Signal = -1;

 private:
  std::unique_ptr<std::thread> _thread;
  std::mutex _mutex;
  std::condition_variable _cv;
  std::queue<std::shared_ptr<SignalMsg>> _signalMsgQueue;
  bool _syncProcessed = false;
};

#endif  // THREADBASE_H_
