#ifndef CONSUMER_H
#define CONSUMER_H

#include <QQueue>
#include <QThread>

class Consumer : public QThread {
  Q_OBJECT
 public:
  Consumer();

 private:
  void run() override;     //重新实现run（）
  QQueue<QString> fruits;  //用于接收生产者生产的水果
 public slots:
  void get(const QString fruit);  //接收水果的槽函数
};

#endif  // CONSUMER_H
