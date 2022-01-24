/*
 * multicast.h
 *
 *  Created on: 21 дек. 2020 г.
 *      Author: vader
 */

#ifndef UFW_LIBS_TYPES_MULTICAST_H_
#define UFW_LIBS_TYPES_MULTICAST_H_

#define MCASTSOC_MTU (1536)
#define MCAST_CLOSED (-1000)

#include <socket.h>
#include <io/io_endpoint.h>

class multicastSocketIn
{
public:
  /**
   * @brief multicastSocketIn
   * @param group
   * @param port
   */
  explicit multicastSocketIn(const char *group, uint16_t port);

  /**
   * @brief config
   * @return
   */
  int config();
  /**
   * @brief setTimeout
   * @param msec
   * @return
   */
  int setTimeout(int msec = 0);

  /**
   * @brief read
   * @param data
   * @param max_size
   * @return
   */
  size_t mcast_read(uint8_t *data,size_t max_size);
  /**
   * @brief read
   * @return
   */

  virtual ~multicastSocketIn();

  bool isSocketCreated();
private:
  struct sockaddr_in localSock{};
  struct ip_mreq groupSock{};

  int m_socket=MCAST_CLOSED;

  //char buffer[MCASTSOC_MTU]={};

  int mcastClose();
};


class multicastSocketOut :public ufw::ioEndpoint
{
public:
  /**
   * @brief multicastSocketOut
   * @param group
   * @param port
   */
  multicastSocketOut(const char* group, uint16_t port);

  /**
   * @brief config
   * @return
   */
  int config();

  /**
   * @brief send
   * @param data
   * @param size
   * @return
   */
  size_t mcast_send(uint8_t* data,size_t size);


  size_t write_to(uint8_t *data,size_t size) override;

  virtual ~multicastSocketOut();

  /**
   *
   * @return
   */
  bool isSocketCreated();
private:
  struct in_addr localInterface{};
  struct sockaddr_in groupSock{};

  int m_socket=MCAST_CLOSED;

  int mcastClose();
};


#endif /* UFW_LIBS_TYPES_MULTICAST_H_ */
