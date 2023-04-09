#include "ThreadBase.h"

ThreadBase::ThreadBase() : _thread(nullptr) {}

ThreadBase::~ThreadBase() { ExitThread(); }

bool ThreadBase::CreateThread() {
  if (!_thread) {
    _thread = std::make_unique<std::thread>(&ThreadBase::Process, this);
  }
  return true;
}

void ThreadBase::ExitThread() {
  if (!_thread) return;

  // Create a ThreadMsg
  std::shared_ptr<ThreadMsg> threadMsg = std::make_shared<ThreadMsg>(
      Exit_SignalID, std::shared_ptr<void>(nullptr));

  // Put exit thread message into the queue
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.emplace(threadMsg);
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

void ThreadBase::PostMsg(std::shared_ptr<ThreadMsg> threadMsg) {
  PostOrSendMsg(std::move(threadMsg), false);
}

void ThreadBase::SendMsg(std::shared_ptr<ThreadMsg> threadMsg) {
  PostOrSendMsg(std::move(threadMsg), true);
  std::unique_lock<std::mutex> lk(_mutex);
  try {
    _cv.wait(lk, [this] { return _syncProcessed; });
  } catch (...) {
    // Ensure _syncProcessed is set to true even if an exception is thrown
  }
}

void ThreadBase::PostOrSendMsg(std::shared_ptr<ThreadMsg> threadMsg,
                               bool wait) {
  if (!_thread) return;

  threadMsg->SetWait(wait);

  // Add msg to queue and notify worker thread
  std::unique_lock<std::mutex> lk(_mutex);
  _queue.emplace(std::move(threadMsg));
  _cv.notify_one();

  if (wait) {
    _syncProcessed = false;
    // Wait for the synchronization process to be true
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

    if (threadMsg->GetId() == Exit_SignalID) {
      break;
    }

    UserCustomFunction(threadMsg);

    if (threadMsg->GetWait()) {
      _syncProcessed = true;
      _cv.notify_one();
    }
  }
}
