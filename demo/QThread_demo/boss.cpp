#include "boss.h"

#include <QDebug>

Boss::Boss(QObject *parent) : QObject(parent) {
  worker = new Worker("A");
  worker->moveToThread(&workThread);
  connect(&workThread, &QThread::finished, worker, &QObject::deleteLater);
  connect(this, &Boss::order, worker,
          &Worker::doWork);  //用信号和槽的方式调用doWork()函数
  connect(worker, &Worker::resultReady, this, &Boss::handleResult);
  workThread.start();
}
Boss::~Boss() {
  workThread.quit();
  workThread.wait();
}

void Boss::handleResult(const QString result) {
  qDebug() << "Boss got it :" << result << "\n";
}

void Boss::order2(const QString order) {
  qDebug() << "order2:";
  worker->doWork(order);  //直接调用
}
