#pragma once

#include <list>
#include <functional>

#include <QObject>
#include <QThread>


#include <mcastRecieve.hpp>
#include <mcastSend.hpp>

#include <tools/arcstylecrypt.hpp>
#include <tools/intercomstation.hpp>
#include <islp/basicmessage.hpp>
#include <islp/clientsm.hpp>

/**
 * @brief The stationEmulator class - Эмуляция поведения оконечного оборудования
 */
class stationEmulator : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief stationEmulator - Единствннный конструктор.
   * @param station - структура станции
   * @param parent
   */
  explicit stationEmulator(intercomStation& station,QObject *parent = nullptr);
  /**
   * @brief operator () - Запускает жизненный цикл эмулятора, создаёт сущности приёма/передачи мультикаст посылок.
   */
  void operator()();

  ~stationEmulator();

public slots:
  void islpRecieved(std::vector<uint8_t>* message);

private:

  arcCrypt* m_key=nullptr;
  QThread* m_client_thread=nullptr;
  QmcastRecieve* m_reciever=nullptr;
  QmcastSend* m_sender=nullptr;
  intercomStation& station_ref;
  ISLP::clientStatemachine m_statemachine;

  const char* m_group_recv_ip="239.168.10.100";
  const uint16_t m_group_recv_port=13120;

  const char* m_group_send_ip="239.168.10.100";
  const uint16_t m_group_send_port=13121;
signals:

};

