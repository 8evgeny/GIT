#pragma once

#include <QObject>
#include <QThread>

#include <vector>

#include <multicastsocket.hpp>

class QmcastRecieve : public QObject, public multicastSocketIn
{
  Q_OBJECT
public:
  /**
   * @brief QmcastRecieve Multicast IP reciever wrapper for Qt constructor.
   * @param host Multicast group IP.
   * @param port Multicast port
   * @param parent
   */
  explicit QmcastRecieve(const char* host, uint16_t port, QObject *parent = nullptr);

  /**
   * @brief start - creates socket, sets up multicast options, and starts sender lifecycle
   */
  void start();

  ~QmcastRecieve();
private:
  /**
     * @brief runner - sender lifecycle fcn.
     * Listens socket, and emits signals, when recieved.
     */
  void runner();

  QThread *worker = nullptr;
signals:
  void Recieved(std::vector<uint8_t>* message);
};

