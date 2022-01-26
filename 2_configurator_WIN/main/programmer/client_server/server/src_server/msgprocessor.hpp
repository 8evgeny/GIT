#pragma once

#include <list>
#include <QObject>
#include <QThread>

#include "include/typedefs.h"
#include "../../../libips/islp/details.hpp"
#include "../../multicaster/mcastRecieve.hpp"
#include "../../multicaster/mcastSend.hpp"

#define ISLP_PROT_IP
#define ISLP_STATION_PORT
#define ISLP_SERVER_PORT

#ifdef Q_OS_UNIX
#include <pthread.h>
#endif

class msgProcessor : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief msgProcessor
   * @param parent
   */
  explicit msgProcessor(QObject *parent = nullptr);

  ~msgProcessor();
  /**
   * @brief start
   */
  void start();
  /**
   * @brief reset
   */
  void reset();
  /**
   * @brief yellStations
   */
  void yellStations();
  /**
   * @brief stations
   * @return
   */
  std::list<intercomStation>& stations();

private slots:
  void recieved(std::vector<uint8_t> *message); //TODO Incorrect visibility

private:
  std::list<intercomStation> m_stations;
  std::list<ISLP::stationResponse> m_messages;

  arcCrypt *m_key=nullptr;

  const char* m_group_recv_ip="239.168.10.100";
  const uint16_t m_group_recv_port=13121;

  const char* m_group_send_ip="239.168.10.100"; //101

  const uint16_t m_group_send_port=13120;

  QmcastRecieve m_reciever;
  QmcastSend m_sender;

  QThread *processor=nullptr;

  void process();

  void messageProcess(ISLP::stationResponse& msg);

signals:
  void Refreshed();
};
