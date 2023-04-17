#include "ThreadBase.h"

ThreadBase::ThreadBase() : _thread(nullptr) {}

ThreadBase::~ThreadBase() { DestroyThread(); }

bool ThreadBase::CreateThread() {
  if (!_thread) {
    _thread = std::make_unique<std::thread>(&ThreadBase::Process, this);
  }
  return true;
}

void ThreadBase::DestroyThread() {
  if (!_thread) return;

  // Send a message to the thread queue to destroy the thread
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.emplace(std::make_shared<ThreadMsg>(DestroyThread_Signal,
                                               std::shared_ptr<void>(nullptr)));
    _cv.notify_one();
  }

  _thread->join();
  _thread = nullptr;
}

std::thread::id ThreadBase::GetThreadId() {
  return _thread ? _thread->get_id() : std::thread::id();
}

std::thread::id ThreadBase::GetCurrentThreadId() {
  return std::this_thread::get_id();
}

void ThreadBase::SendSlotFuncAsyncRunMsg(std::shared_ptr<ThreadMsg> threadMsg) {
  SendMsg(false, std::move(threadMsg));
}

void ThreadBase::SendSlotFuncSyncRunMsg(std::shared_ptr<ThreadMsg> threadMsg) {
  SendMsg(true, std::move(threadMsg));
  std::unique_lock<std::mutex> lk(_mutex);
  try {
    _cv.wait(lk, [this] { return _syncProcessed; });
  } catch (...) {
    // Ensure _syncProcessed is set to true even if an exception is thrown
  }
}

void ThreadBase::SendMsg(bool wait, std::shared_ptr<ThreadMsg> threadMsg) {
  if (!_thread) return;

  threadMsg->SetWait(wait);

  // Add the message to the queue
  std::unique_lock<std::mutex> lk(_mutex);
  _queue.emplace(std::move(threadMsg));
  _cv.notify_one();

  if (wait) {
    _syncProcessed = false;
    // Wait for the message to be processed by the worker thread synchronously
    _cv.wait(lk, [this] { return _syncProcessed; });
  }
}

void ThreadBase::Process() {
  std::shared_ptr<ThreadMsg> threadMsg;

  while (1) {
    {
      // Wait for a message to be added to the queue
      std::unique_lock<std::mutex> lk(_mutex);
      _cv.wait(lk, [this] { return !_queue.empty(); });
      if (_queue.empty()) continue;
      threadMsg = std::move(_queue.front());
      _queue.pop();
    }

    if (threadMsg->GetSignal() == DestroyThread_Signal) break;

    UserCustomFunction(threadMsg);

    if (threadMsg->GetWait()) {
      std::lock_guard<std::mutex> lock(_mutex);
      _syncProcessed = true;
      _cv.notify_one();
    }
  }
}
