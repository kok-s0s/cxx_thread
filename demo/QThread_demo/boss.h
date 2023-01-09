#ifndef BOSS_H
#define BOSS_H

#include <QObject>
#include <QThread>

#include "worker.h"

class Boss : public QObject {
  Q_OBJECT
 public:
  explicit Boss(QObject *parent = nullptr);
  ~Boss();
  void order2(const QString order);  //用于测试是否使用了线程

 signals:
  void order(const QString order);  //用于测试是否使用了线程

 public slots:
  void handleResult(const QString result);

 private:
  QThread workThread;  // worker 线程
  Worker *worker;      // worker 对象指针
};

#endif  // BOSS_H
