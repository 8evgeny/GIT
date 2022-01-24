#pragma once

#include <QObject>

#include <functional>

#include <multicastsocket.hpp>

class QmcastSend
    : public QObject, public multicastSocketOut
{
  Q_OBJECT
public:
  /**
   * @brief QmcastSend Multicast IP sender wrapper for Qt constructor.
   * @param host Multicast group IP.
   * @param port Multicast port
   * @param parent
   */
  explicit QmcastSend(const char* host, uint16_t port, QObject *parent = nullptr);

  /**
   * @brief operator () Call operator overload. Sends data to group.
   * @param data data pointer
   * @param size size of data in bytes
   */
  void operator()(uint8_t *data, size_t size);

  /**
   * @brief functor - Returns Std Library functor object asiigned to send fcn.
   * @return
   */
  std::function<void(uint8_t*,size_t)> functor();

  ~QmcastSend();
public slots:
  /**
   * @brief sendToGroup Send Qt Slot
   * @param data
   * @param size
   * @return
   */
  size_t sendToGroup(uint8_t *data, size_t size);

private:

signals:

};

