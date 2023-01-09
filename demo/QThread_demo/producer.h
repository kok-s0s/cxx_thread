#ifndef PRODUCER_H
#define PRODUCER_H

#include <QObject>

class Producer : public QObject {
  Q_OBJECT
 public:
  explicit Producer(QObject *parent = nullptr) {}

 signals:
  void produce(QString fruit);  //生产水果完成信号
 public slots:
};

#endif  // PRODUCER_H
