


#include "mcastRecieve.hpp"
#include <QDebug>

QmcastRecieve::QmcastRecieve(const char *host, uint16_t port, QObject *parent)
    : QObject(parent),
      multicastSocketIn(host,port)

{
  // Создаём тред для обработки входящих сообщений из функции void QmcastRecieve::runner(). Единственный параметр тут указатель на собственный объект.
  worker = QThread::create(&QmcastRecieve::runner, this);
}

void QmcastRecieve::start()
{
  // Создаём сокет. В случае ошибки завершаем работу
  if(config()<0)
  {
    qDebug()<<this<<" :Reciever socket creation failed. Errno:"<<errno;
    exit(-1); //TODO change to exception with handler.
  }
  qDebug()<<this<<" :Reciever socket created!";
  // Запускаем тред обработчика
  worker->start();
}

void QmcastRecieve::runner()
{
  // Запускаем цикл пока не будет получен запрос на прерывание (для корректного завершения/удаления треда)
  while(!worker->isInterruptionRequested())
  {
    // Читаем
    std::vector<uint8_t>* result=read();
    if(result!=nullptr)
    {
      // Если принято без ошибок, посылаем сигнал.
      emit Recieved(result);
    }
  }
}

QmcastRecieve::~QmcastRecieve()
{
  worker->requestInterruption(); //Посылаем запрос на прерывание треда
  worker->wait(1500); // Ждём завершения (лучше указать конкретное время
  if(worker->isRunning()) worker->terminate();
  delete worker; // Удаляем объект
}
