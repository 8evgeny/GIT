#pragma once
#include <vector>

#ifdef __WIN32__
#include <winsock2.h>
#include <Ws2tcpip.h>
#include <MSWSock.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#define MCASTSOC_MTU 1536

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
  size_t read(uint8_t *data,size_t max_size);
  /**
   * @brief read
   * @return
   */
  std::vector<uint8_t> *read();

private:
  struct sockaddr_in localSock{};
  struct ip_mreq groupSock{};

  int m_socket=-1000;
  char buffer[MCASTSOC_MTU]={};
};
