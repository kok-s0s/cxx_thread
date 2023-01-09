#include "worker.h"

#include <QDebug>
#include <QThread>

Worker::Worker(QObject *parent) : QObject(parent) {}

Worker::Worker(QString name, QObject *parent) { this->name = name; }

void Worker::doWork(const QString &parameter) {
  qDebug() << "当前Worker线程ID:" << QThread::currentThreadId() << "\n"
           << name + "正在" + parameter << "\n";
  emit resultReady(name + parameter + "完成");
}
