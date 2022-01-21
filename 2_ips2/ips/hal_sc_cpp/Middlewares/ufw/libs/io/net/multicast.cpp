/*
 * multicast.cpp
 *
 *  Created on: 21 дек. 2020 г.
 *      Author: vader
 */

#include <io/net/multicast.h>

extern uint8_t *g_ufw_ip_settings;

multicastSocketIn::multicastSocketIn (const char *group, uint16_t port)
{
  localSock.sin_family = AF_INET;
  localSock.sin_port = htons (port);
  localSock.sin_addr.s_addr = inet_addr (group); //INADDR_ANY;//

  groupSock.imr_multiaddr.s_addr = inet_addr (group); // добавляем ip группы
  groupSock.imr_interface.s_addr = INADDR_ANY;//htonl(*(in_addr_t*)g_ufw_ip_settings); //

}

int multicastSocketIn::config ()
{
  m_socket = socket (AF_INET, SOCK_DGRAM, 0);
  if (m_socket < 0) return -1;

// Делаем сокет реюзабельным чтобы другие тоже могли слушать этот адрес
    {
      int reuse = 1;
      if (setsockopt (m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*> (&reuse),
		      sizeof(reuse)) < 0) return -2;
    }
// IP_ADD_MEMBERSHIP - добавляет тебя в группу
    {
      if (setsockopt (m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<char*> (&groupSock),
		      sizeof(groupSock)) < 0) return -3;
    }
// Биндим сокет на себя
    {
      if (bind (m_socket, reinterpret_cast<struct sockaddr*> (&localSock), sizeof(localSock)) < 0)
	return -4;
    }
//if (setTimeout(MCASTSOC_TOUT)<0)
//  {
//    return -5;
//  }
  return 0;
}

size_t multicastSocketIn::mcast_read (uint8_t *data, size_t max_size)
{
  return recv (m_socket, reinterpret_cast<char*> (data), max_size, 0);
}


int multicastSocketIn::setTimeout (int msec)
{
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = msec * 1000;
  if(setsockopt(m_socket,SOL_SOCKET, SO_RCVTIMEO,reinterpret_cast<char*>(&timeout),sizeof(timeout))<0)
    return -5;
  return msec;
}

multicastSocketIn::~multicastSocketIn ()
{
  mcastClose ();
}

bool multicastSocketIn::isSocketCreated ()
{
  return (m_socket>=0);
}

int multicastSocketIn::mcastClose ()
{
  shutdown (m_socket, SHUT_RDWR);
  int result = close (m_socket);
  m_socket = MCAST_CLOSED;
  return result;
}

multicastSocketOut::multicastSocketOut (const char *group, uint16_t port)
{
  groupSock.sin_family = AF_INET;
  groupSock.sin_addr.s_addr = inet_addr (group);
  groupSock.sin_port = htons (port);

  localInterface.s_addr = INADDR_ANY;
}

int multicastSocketOut::config ()
{
  m_socket = socket (AF_INET, SOCK_DGRAM, 0);
  if (m_socket < 0) return -1;
// Устанавливаем опцию указываюшую нужно ли возвращать переданные данные обратно себе (в данном случае ДА)
  {
    char loopch = 1;
    if(setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loopch,sizeof(loopch))<0)
    return -2;
  }
// Указываем сокету собственный интерфейс передачи мультикаст сообщений
  {
    if(setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_IF, reinterpret_cast<char*>(&localInterface), sizeof(localInterface))<0)
    return -3;
  }
return 0;
}

size_t multicastSocketOut::mcast_send (uint8_t *data, size_t size)
{
  if(m_socket<0)return 0;
  return sendto(m_socket, reinterpret_cast<char*>(data), size, 0, reinterpret_cast<struct sockaddr*>(&groupSock),sizeof(groupSock));
}

multicastSocketOut::~multicastSocketOut ()
{
  mcastClose();
}

int multicastSocketOut::mcastClose ()
{
  shutdown(m_socket,SHUT_RDWR);
  int result=close(m_socket);
  m_socket=MCAST_CLOSED;
  return result;
}


size_t multicastSocketOut::write_to (uint8_t *data, size_t size)
{
  return mcast_send(data, size);
}


bool multicastSocketOut::isSocketCreated ()
{
  return (m_socket>=0);
}
