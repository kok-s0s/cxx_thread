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
    _signalMsgQueue.emplace(std::make_shared<SignalMsg>(
        DestroyThread_Signal, std::shared_ptr<void>(nullptr)));
    _cv.notify_one();
  }

  // Wait for the thread to be destroyed
  _thread->join();
  _thread = nullptr;
}

std::thread::id ThreadBase::GetThreadId() {
  return _thread ? _thread->get_id() : std::thread::id();
}

std::thread::id ThreadBase::GetCurrentThreadId() {
  return std::this_thread::get_id();
}

void ThreadBase::SendSlotFuncAsyncRunMsg(std::shared_ptr<SignalMsg> signalMsg) {
  SendMsg(false, std::move(signalMsg));
}

void ThreadBase::SendSlotFuncSyncRunMsg(std::shared_ptr<SignalMsg> signalMsg) {
  SendMsg(true, std::move(signalMsg));
  std::unique_lock<std::mutex> lock(_mutex);
  try {
    _cv.wait(lock, [this] { return _syncProcessed; });
  } catch (...) {
    // Ensure _syncProcessed is set to true even if an exception is thrown
  }
}

void ThreadBase::SendMsg(bool wait, std::shared_ptr<SignalMsg> signalMsg) {
  if (!_thread) return;

  signalMsg->SetWait(wait);

  // Add the message to the queue
  std::unique_lock<std::mutex> lock(_mutex);
  _signalMsgQueue.emplace(std::move(signalMsg));
  _cv.notify_one();

  if (wait) {
    _syncProcessed = false;
    // Wait for the message to be processed by the worker thread synchronously
    _cv.wait(lock, [this] { return _syncProcessed; });
  }
}

void ThreadBase::Process() {
  std::shared_ptr<SignalMsg> signalMsg;

  while (1) {
    {
      // Wait for a message to be added to the queue
      std::unique_lock<std::mutex> lock(_mutex);
      _cv.wait(lock, [this] { return !_signalMsgQueue.empty(); });
      if (_signalMsgQueue.empty()) continue;
      signalMsg = std::move(_signalMsgQueue.front());
      _signalMsgQueue.pop();
    }

    if (signalMsg->GetSignal() == DestroyThread_Signal) break;

    UserCustomFunction(signalMsg);

    if (signalMsg->GetWait()) {
      std::lock_guard<std::mutex> lock(_mutex);
      _syncProcessed = true;
      _cv.notify_one();
    }
  }
}
