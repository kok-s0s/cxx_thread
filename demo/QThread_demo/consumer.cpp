#include "consumer.h"

#include <QDebug>

Consumer::Consumer() {}

void Consumer::get(const QString fruit) {
  qDebug() << "当前get()所在线程ID:" << QThread::currentThreadId() << "\n";
  fruits.enqueue(fruit);
}

void Consumer::run() {
  while (true) {
    if (fruits.size() > 0) {
      qDebug() << "当前run()所在线程ID:" << QThread::currentThreadId() << "\n";
      qDebug() << "我吃了一个:" << fruits.dequeue() << "\n";
    }
  }
}
