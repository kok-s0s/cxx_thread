#include "TestThread.h"

TestThread::TestThread(std::function<void()> func) : _thread(func) {
  _status = ThreadStatus::Ready;
  start();
}

TestThread::TestThread(std::function<void(bool &flag)> func,
                       std::reference_wrapper<bool> flag)
    : _thread(func, flag) {
  _status = ThreadStatus::Ready;
  start();
}

TestThread::TestThread(TestThread &&obj) : _thread(std::move(obj._thread)) {
  _status = ThreadStatus::Ready;
  start();
}

TestThread &TestThread::operator=(TestThread &&obj) {
  if (_thread.joinable())
    _thread.join();

  _thread = std::move(obj._thread);
  _status = ThreadStatus::Ready;
  start();
  return *this;
}

TestThread::~TestThread() {
  if (_thread.joinable())
    _thread.join();

  _status = ThreadStatus::Finished;
}

std::thread::id TestThread::get_id() { return _thread.get_id(); }

int TestThread::getStatus() const { return _status; }

bool TestThread::isRunning() { return _status == ThreadStatus::Running; }

bool TestThread::isWaiting() { return _status == ThreadStatus::Waiting; }

bool TestThread::isDelayed() { return _status == ThreadStatus::Delayed; }

bool TestThread::isBlocked() { return _status == ThreadStatus::Blocked; }

bool TestThread::isFinished() {
  if (_thread.joinable()) {
    return false;
  } else {
    _status = ThreadStatus::Finished;
    return true;
  }
}

void TestThread::start() {
  if (_thread.joinable())
    _status = ThreadStatus::Running;
}

bool TestThread::detach() {
  if (_thread.joinable()) {
    _thread.detach();
    return true;
  }

  return false;
}

bool TestThread::join() {
  if (_thread.joinable()) {
    _thread.join();
    _status = ThreadStatus::Finished;
    return true;
  }

  return false;
}