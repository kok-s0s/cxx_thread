#include "ThreadExtension.h"

ThreadExtension::ThreadExtension() {
  _status = ThreadStatus::Ready;
  _thread = nullptr;
  _thread_pause_flag = false;
  _thread_stop_flag = false;
}

ThreadExtension::~ThreadExtension() {
  if (_thread->joinable())
    _thread->join();

  _status = ThreadStatus::Finished;
}

void ThreadExtension::addFunc(std::function<void()> func) {
  _v_func.push_back(func);
}

std::thread::id ThreadExtension::get_id() { return _thread->get_id(); }

int ThreadExtension::getStatus() const { return (int)_status; }

bool ThreadExtension::isRunning() { return _status == ThreadStatus::Running; }

bool ThreadExtension::isPaused() { return _status == ThreadStatus::Paused; }

bool ThreadExtension::isFinished() { return _status == ThreadStatus::Finished; }

void ThreadExtension::run() {
  while (!_thread_stop_flag) {
    try {
      for (auto _func : _v_func) {
        _func();
      }
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

void ThreadExtension::start() {
  if (_thread == nullptr) {
    _thread = std::make_unique<std::thread>(&ThreadExtension::run, this);
    if (_thread != nullptr) {
      _status = ThreadStatus::Running;
      _thread_pause_flag = false;
      _thread_stop_flag = false;
    }
  }
}

void ThreadExtension::pause() {
  if (_thread != nullptr) {
    if (_status == ThreadStatus::Running) {
      _thread_pause_flag = true;
      _status = ThreadStatus::Paused;
    }
  }
}

void ThreadExtension::resume() {
  if (_thread != nullptr) {
    if (_status == ThreadStatus::Paused) {
      _thread_pause_flag = false;
      _condition_variable.notify_all();
      _status = ThreadStatus::Running;
    }
  }
}

void ThreadExtension::quit() {
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

bool ThreadExtension::detach() {
  if (_thread->joinable()) {
    _thread->detach();
    return true;
  }

  return false;
}

bool ThreadExtension::join() {
  if (_thread->joinable()) {
    _thread->join();
    _status = ThreadStatus::Finished;
    return true;
  }

  return false;
}