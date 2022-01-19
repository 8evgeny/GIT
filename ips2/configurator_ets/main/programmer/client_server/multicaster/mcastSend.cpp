#include "mcastSend.hpp"
#include <QThread>
#include <QDebug>
#include "multicast_socket_out.h"

QmcastSend::QmcastSend(const char* host, uint16_t port, QObject *parent)
    : QObject(parent)
    , multicastSocketOut(host,port)
{
  // Создаём сокет. В случае ошибки завершаем работу
  if(config()<0)
  {
    qDebug()<<this<<": Sender socket creation failed. Errno:"<<errno;
    exit(-1); //TODO change to exception with handler.
  }
}

// СЛОТ QT: Отправляем в группу
size_t QmcastSend::sendToGroup(uint8_t *data, size_t size)
{
  return send(data,size);
}

// Отправляем в группу (перегрузка оператора вызова для создания функциональных объектов)
void QmcastSend::operator()(uint8_t *data, size_t size)
{ 
  if(send(data,size)<size)
  {
    qDebug()<<this<<": Data transfer mismatch\n";
  }
}

// Создаёт и возвращает функтор стандартной библиотеки для отправки сообщения
std::function<void (uint8_t *, size_t)> QmcastSend::functor()
{
  return std::function<void (uint8_t *, size_t)>(std::ref(*this));
}

QmcastSend::~QmcastSend()
{
  qDebug()<<this<<": sender deleted";
}
