#pragma once
#include <vector>

#ifdef __WIN32__
    #include <winsock2.h>
    #include <Ws2tcpip.h>
    #include <MSWSock.h>
    #include <stdint.h>
#else
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netinet/in.h>
#endif

#define MCASTSOC_MTU 1536

class multicastSocketOut
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
  size_t send(uint8_t* data,size_t size);

private:
  struct in_addr localInterface{};
  struct sockaddr_in groupSock{};

//  int m_socket=-1000;
  SOCKET m_socket;
};
