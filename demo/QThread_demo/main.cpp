// #include <QCoreApplication>
// #include <QDebug>

// #include "boss.h"

// int main(int argc, char *argv[]) {
//   QCoreApplication a(argc, argv);
//   qDebug() << "当前Boss线程ID:" << QThread::currentThreadId() << "\n";
//   Boss *boss = new Boss();
//   boss->order("整理资料(order)");    //用信号触发槽函数的方式
//   boss->order2("整理资料(order2)");  //直接调用函数的方式
//   return a.exec();
// }

#include <QCoreApplication>
#include <QDebug>

#include "consumer.h"
#include "producer.h"

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);

  qDebug() << "当前consumer所在线程ID:" << QThread::currentThreadId() << "\n";
  Producer producer;  //生产者
  Consumer consumer;  //消费者线程
  QObject::connect(&producer, &Producer::produce, &consumer,
                   &Consumer::get);  //绑定生产者信号
  consumer.start();
  emit producer.produce("apple");   //生产苹果
  emit producer.produce("banana");  //生产香蕉

  return a.exec();
}
