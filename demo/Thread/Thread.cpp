#include "Thread.h"

Thread::Thread() {
  _status = ThreadStatus::Ready;
  _thread = nullptr;
  _thread_pause_flag = false;
  _thread_stop_flag = false;
}

Thread::~Thread() {
  if (_thread->joinable())
    _thread->join();

  _status = ThreadStatus::Finished;
}

void Thread::setFunc(std::function<void()> func) { _func = func; }

std::thread::id Thread::get_id() { return _thread->get_id(); }

int Thread::getStatus() const { return (int)_status; }

bool Thread::isRunning() { return _status == ThreadStatus::Running; }

bool Thread::isPaused() { return _status == ThreadStatus::Paused; }

bool Thread::isFinished() { return _status == ThreadStatus::Finished; }

void Thread::run() {
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

void Thread::start() {
  if (_thread == nullptr) {
    _thread = std::make_unique<std::thread>(&Thread::run, this);
    if (_thread != nullptr) {
      _status = ThreadStatus::Running;
      _thread_pause_flag = false;
      _thread_stop_flag = false;
    }
  }
}

void Thread::pause() {
  if (_thread != nullptr) {
    if (_status == ThreadStatus::Running) {
      _thread_pause_flag = true;
      _status = ThreadStatus::Paused;
    }
  }
}

void Thread::resume() {
  if (_thread != nullptr) {
    if (_status == ThreadStatus::Paused) {
      _thread_pause_flag = false;
      _condition_variable.notify_all();
      _status = ThreadStatus::Running;
    }
  }
}

void Thread::quit() {
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

bool Thread::detach() {
  if (_thread->joinable()) {
    _thread->detach();
    return true;
  }

  return false;
}

bool Thread::join() {
  if (_thread->joinable()) {
    _thread->join();
    _status = ThreadStatus::Finished;
    return true;
  }

  return false;
}
