#include "ThreadBase.h"

ThreadBase::ThreadBase() : _thread(nullptr) {}

ThreadBase::~ThreadBase() {
  if (!_thread) return;

  {
    std::lock_guard<std::mutex> lock(_mutex);
    _signalMsgQueue.emplace(std::make_shared<SignalMsg>(
        DestroyThread_Signal, std::shared_ptr<void>(nullptr)));
    _cv.notify_one();
  }

  try {
    if (_thread->joinable()) {
      _thread->join();
    }
    _thread = nullptr;
  } catch (const std::exception& e) {
    std::cerr << "Error joining thread: " << e.what() << std::endl;
  }
}

bool ThreadBase::CreateThread() {
  if (_thread) {
    std::cerr << "Thread already exists." << std::endl;
    return false;
  }

  try {
    if (!_thread) {
      _thread = std::make_unique<std::thread>(&ThreadBase::Process, this);
    }
    return true;
  } catch (const std::exception& e) {
    std::cerr << "Error creating thread: " << e.what() << std::endl;
    return false;
  }
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
  _cv.wait(lock, [this] { return _syncProcessed; });
}

void ThreadBase::SendMsg(bool wait, std::shared_ptr<SignalMsg> signalMsg) {
  if (!_thread) return;

  signalMsg->SetWait(wait);
  {
    std::unique_lock<std::mutex> lock(_mutex);
    _signalMsgQueue.emplace(std::move(signalMsg));

    if (wait) {
      _syncProcessed = false;
    }
  }
  _cv.notify_one();
}

/**
 * @brief Process function for the ThreadBase class.
 *
 * This function is responsible for processing the signal messages in the
 * thread. It waits for a signal message to be available in the signal message
 * queue, retrieves the signal message, and calls the UserCustomFunction to
 * handle the signal. If an exception occurs in the UserCustomFunction, an error
 * message is printed to std::cerr. If the signal message has the "wait" flag
 * set, it notifies the waiting thread that the processing is done.
 *
 * @note This function runs in an infinite loop until a DestroyThread_Signal is
 * received.
 */
void ThreadBase::Process() {
  std::shared_ptr<SignalMsg> signalMsg;

  while (1) {
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _cv.wait(lock, [this] { return !_signalMsgQueue.empty(); });
      if (_signalMsgQueue.empty()) continue;
      signalMsg = std::move(_signalMsgQueue.front());
      _signalMsgQueue.pop();
    }

    if (signalMsg->GetSignal() == DestroyThread_Signal) break;

    try {
      UserCustomFunction(signalMsg);
    } catch (...) {
      std::cerr << "Unknown error in UserCustomFunction." << std::endl;
    }

    if (signalMsg->GetWait()) {
      std::lock_guard<std::mutex> lock(_mutex);
      _syncProcessed = true;
      _cv.notify_one();
    }
  }
}
