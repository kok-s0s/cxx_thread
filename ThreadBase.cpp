#include "ThreadBase.h"

void ThreadBase::Process() {
  while (1) {
    std::shared_ptr<ThreadMsg> threadMsg;
    {
      // Wait for a message to be added to the queue
      std::unique_lock<std::mutex> lk(_mutex);
      _cv.wait(lk, [this] { return !_queue.empty(); });

      if (_queue.empty()) continue;
      threadMsg = _queue.front();
      _queue.pop();
    }

    if (threadMsg->_id == exit_SignalID) {
      break;
    }

    UserCustomFunction(threadMsg);

    if (threadMsg->_wait) {
      _syncProcessed = true;
      _cv.notify_one();
    }
  }
}

ThreadBase::ThreadBase() : _thread(nullptr) {}

ThreadBase::~ThreadBase() { ExitThread(); }

bool ThreadBase::CreateThread() {
  if (!_thread)
    _thread = std::unique_ptr<std::thread>(
        new std::thread(&ThreadBase::Process, this));
  return true;
}

void ThreadBase::ExitThread() {
  if (!_thread) return;

  // Create a new ThreadMsg
  std::shared_ptr<ThreadMsg> threadMsg(new ThreadMsg(exit_SignalID, nullptr));

  // Put exit thread message into the queue
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push(threadMsg);
    _cv.notify_one();
  }
  _thread->join();
  _thread = nullptr;
}

std::thread::id ThreadBase::GetThreadId() {
  if (_thread != nullptr)
    return _thread->get_id();
  else
    return std::thread::id();
}

std::thread::id ThreadBase::GetCurrentThreadId() {
  return std::this_thread::get_id();
}

void ThreadBase::PostMsg(std::shared_ptr<ThreadMsg> threadMsg) {
  if (!_thread) return;

  threadMsg->_wait = false;

  // Add msg to queue and notify worker thread
  std::unique_lock<std::mutex> lk(_mutex);
  _queue.push(threadMsg);
  _cv.notify_one();
}

void ThreadBase::SendMsg(std::shared_ptr<ThreadMsg> threadMsg) {
  if (!_thread) return;

  threadMsg->_wait = true;
  _syncProcessed = false;

  // Add msg to queue and notify worker thread
  std::unique_lock<std::mutex> lk(_mutex);
  _queue.push(threadMsg);
  _cv.notify_one();

  // Wait for the synchronization process to be true
  _cv.wait(lk, [this] { return _syncProcessed; });
}