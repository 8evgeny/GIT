#include "multicast_socket_out.h"
#include <cstring>

#define MCASTSOC_TOUT 900

multicastSocketOut::multicastSocketOut(const char *group, uint16_t port)
{
#ifdef __WIN32__
  WSADATA wsaData;
  if(WSAStartup(0x0202, &wsaData)!=0)
    exit(-1);
#endif
  // Задаём данные группы
  groupSock.sin_family = AF_INET;
  groupSock.sin_addr.s_addr = inet_addr(group);
  groupSock.sin_port = htons(port);
  // Задаём данные собственного интерфейса
  localInterface.s_addr = INADDR_ANY;
}

// Создаем и настраиваем сокет на передачу мультикаста
// В случае успеха вернется 0 иначе отрицательное значение
int multicastSocketOut::config()
{
   //Создаём сокет
  {
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(m_socket<0)
      return -1;
  }
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

// Передача данных
size_t multicastSocketOut::send(uint8_t *data, size_t size)
{
  if(m_socket<0)return 0; 
  auto sockError = sendto(m_socket, reinterpret_cast<char*>(data), size, 0, reinterpret_cast<struct sockaddr*>(&groupSock),sizeof(groupSock)); 
  return static_cast<size_t>(sockError);
}
