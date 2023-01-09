#ifndef WORKER_H_
#define WORKER_H_

#include <QObject>
#include <iostream>

class Worker : public QObject {
  Q_OBJECT
 public:
  explicit Worker(QObject *parent = nullptr);
  Worker(QString name, QObject *parent = nullptr);

 signals:
  void resultReady(const QString &resultReady);  //工作完成信号

 public slots:
  void doWork(const QString &parameter);  //工作函数

 private:
  QString name;
};

#endif  // WORKER_H
