#include "TestThread.h"

TestThread::TestThread(std::function<void()> func) {
  _status = ThreadStatus::Ready;
  _thread = nullptr;
  _thread_pause_flag = false;
  _thread_stop_flag = false;
  _func = func;
}

TestThread::~TestThread() {
  if (_thread->joinable())
    _thread->join();

  _status = ThreadStatus::Finished;
}

std::thread::id TestThread::get_id() { return _thread->get_id(); }

int TestThread::getStatus() const { return (int)_status; }

bool TestThread::isRunning() { return _status == ThreadStatus::Running; }

bool TestThread::isPaused() { return _status == ThreadStatus::Paused; }

bool TestThread::isFinished() { return _status == ThreadStatus::Finished; }

void TestThread::run() {
  while (!_thread_stop_flag) {
    try {
      _func();
    } catch (std::exception &e) {
      break;
    }

    if (_thread_pause_flag) {
      std::unique_lock<std::mutex> thread_locker(_mutex);
      if (_thread_pause_flag) {
        _condition_variable.wait(thread_locker);
      }

      thread_locker.unlock();
    }
  }

  _thread_pause_flag = false;
  _thread_stop_flag = false;
}

void TestThread::start() {
  if (_thread == nullptr) {
    _thread = std::make_unique<std::thread>(&TestThread::run, this);
    if (_thread != nullptr) {
      _status = ThreadStatus::Running;
      _thread_pause_flag = false;
      _thread_stop_flag = false;
    }
  }
}

void TestThread::pause() {
  if (_thread != nullptr) {
    if (_status == ThreadStatus::Running) {
      _thread_pause_flag = true;
      _status = ThreadStatus::Paused;
    }
  }
}

void TestThread::resume() {
  if (_thread != nullptr) {
    if (_status == ThreadStatus::Paused) {
      _thread_pause_flag = false;
      _condition_variable.notify_all();
      _status = ThreadStatus::Running;
    }
  }
}

void TestThread::quit() {
  if (_thread != nullptr) {
    _thread_stop_flag = true;
    _thread_pause_flag = false;

    _condition_variable.notify_all();

    if (_thread->joinable()) {
      _thread->join();
    }

    _thread.reset();

    if (_thread == nullptr) {
      _status = ThreadStatus::Finished;
    }
  }
}

bool TestThread::detach() {
  if (_thread->joinable()) {
    _thread->detach();
    return true;
  }

  return false;
}

bool TestThread::join() {
  if (_thread->joinable()) {
    _thread->join();
    _status = ThreadStatus::Finished;
    return true;
  }

  return false;
}