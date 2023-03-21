#ifndef THREADBASE_HPP_
#define THREADBASE_HPP_
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

struct ThreadMsg {
  int _id = -1;  // -1:exit thread
  std::shared_ptr<void> _msg;
  bool _wait = false;  // false:post, true:send

  ThreadMsg(int id, std::shared_ptr<void> msg) {
    _id = id;
    _msg = msg;
  }
};

class ThreadBasic {
 private:
  std::unique_ptr<std::thread> _thread;
  std::mutex _mutex;
  std::condition_variable _cv;
  std::queue<std::shared_ptr<ThreadMsg>> _queue;
  bool _syncProcessed;

 public:
  ThreadBasic(const ThreadBasic &) = delete;
  ThreadBasic &operator=(const ThreadBasic &) = delete;
  ThreadBasic() : _thread(nullptr) {}
  ~ThreadBasic() { exitThread(); }

  /**
   * @brief get the ID of the currently executing thread
   * @return the current thread ID
   */
  static std::thread::id getCurrentThreadId() {
    return std::this_thread::get_id();
  }

  /**
   * @brief get the ID of this thread instance
   * @return the worker thread ID
   */
  std::thread::id getThreadId() {
    if (_thread != nullptr)
      return _thread->get_id();
    else
      return std::thread::id();
  }

  /**
   * @brief called once to create the worker thread
   * @return true if thread is created. False otherwise.
   */
  bool createThread() {
    if (!_thread)
      _thread = std::unique_ptr<std::thread>(
          new std::thread(&ThreadBasic::process, this));
    return true;
  }

  /**
   * @brief called once a program exit to exit the worker thread
   */
  void exitThread() {
    if (!_thread) return;

    std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(-1, nullptr));
    {
      std::lock_guard<std::mutex> lock(_mutex);
      _queue.push(threadMsg);
      _cv.notify_one();
    }
    _thread->join();
    _thread = nullptr;
  }

 protected:
  /**
   * @brief post a message to the thread queue
   * @param[in] msg thread specific message information
   */
  void postMsg(std::shared_ptr<ThreadMsg> threadMsg) {
    if (!_thread) return;

    std::unique_lock<std::mutex> lk(_mutex);
    threadMsg->_wait = false;
    _queue.push(threadMsg);
    _cv.notify_one();
  }

  /**
   * @brief send a message to the thread queue
   * @param[in] msg thread specific message information
   */
  void sendMsg(std::shared_ptr<ThreadMsg> threadMsg) {
    if (!_thread) return;

    std::unique_lock<std::mutex> lk(_mutex);
    threadMsg->_wait = true;
    _queue.push(threadMsg);
    _syncProcessed = false;
    _cv.notify_one();
    _cv.wait(lk, [this] { return _syncProcessed; });
  }

  /**
   * @brief Entry point for the worker thread
   */
  void process() {
    while (1) {
      std::shared_ptr<ThreadMsg> threadMsg;
      {
        std::unique_lock<std::mutex> lk(_mutex);
        _cv.wait(lk, [this] { return !_queue.empty(); });
        if (_queue.empty()) continue;
        threadMsg = _queue.front();
        _queue.pop();
      }

      userCustomFunction(threadMsg);

      if (threadMsg->_wait) {
        _syncProcessed = true;
        _cv.notify_one();
      }
    }
  }

  /**
   * @brief user custom function
   */
  virtual void userCustomFunction(std::shared_ptr<ThreadMsg> threadMsg) = 0;
};
#endif  // THREADBASE_HPP_
