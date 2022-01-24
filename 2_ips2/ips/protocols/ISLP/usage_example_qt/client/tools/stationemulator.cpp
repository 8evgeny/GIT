#include "stationemulator.hpp"
#include <QDebug>

stationEmulator::stationEmulator(intercomStation &station, QObject *parent) :
  QObject(parent),
  station_ref(station),
  m_statemachine(station,m_key)
{
}

void stationEmulator::operator()()
{
  m_sender= new QmcastSend(m_group_send_ip,m_group_send_port,this);
  m_reciever= new QmcastRecieve(m_group_recv_ip,m_group_recv_port,this);
  QmcastSend& sender_ref=*m_sender;  // разыменовываем указатель (в ссылку) (необходимо для создания функтора)
  connect(m_reciever,SIGNAL(Recieved(std::vector<uint8_t>*)),this,SLOT(islpRecieved(std::vector<uint8_t>*)));
  m_statemachine.setSenderFunction(std::ref(sender_ref)); // передаем обработчику функтор отправки обратных сообщений
  m_reciever->start(); //запускаем цикл приёма сообщений
}

stationEmulator::~stationEmulator()
{
  delete m_key;
  delete m_sender;
  delete m_reciever;
}

void stationEmulator::islpRecieved(std::vector<uint8_t> *message)
{
  ISLP::basicMessage msg(std::move(*message));
  m_statemachine(msg);
  delete message;
}

