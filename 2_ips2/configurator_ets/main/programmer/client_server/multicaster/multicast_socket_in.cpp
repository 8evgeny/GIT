#include "multicast_socket_in.h"
#include <cstring>

#define MCASTSOC_TOUT 900

multicastSocketIn::multicastSocketIn(const char *group, uint16_t port) {
  /*
   * заполняем локальную структуру
   */
#ifdef __WIN32__
  WSADATA wsaData;
  if(WSAStartup(0x0202, &wsaData)!=0)
    exit(-1);
#endif

  localSock.sin_family = AF_INET;
  localSock.sin_port = htons(port); //порт который слушаешь
#ifdef __WIN32__
  localSock.sin_addr.s_addr = INADDR_ANY;
#else
  localSock.sin_addr.s_addr = inet_addr(group); // Сокет линукса должен слушать только тот ip на котором находится группа
#endif

   // заполняем структуру мультикаст группы

  groupSock.imr_multiaddr.s_addr = inet_addr(group); // добавляем ip группы
  groupSock.imr_interface.s_addr = INADDR_ANY; // добавляем собственный интерфейс (INADDR_ANY - показывает что интерфейс может принимать входящие с любого ip)
}

// Создаем и настраиваем сокет на прием мультикаста
int multicastSocketIn::config() {
  // Создаём UDP сокет (мультикаст только udp)
  {
//    m_socket = static_cast<long long int>(socket(AF_INET, SOCK_DGRAM, 0));
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);
//	if (m_socket < 0)
    if (m_socket == INVALID_SOCKET)
      return -1;
  }
  // Делаем сокет реюзабельным чтобы другие тоже могли слушать этот адрес
  {
    int reuse = 1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(&reuse), sizeof(reuse)) <
        0)
      return -2;
  }
  // IP_ADD_MEMBERSHIP - добавляет тебя в группу
  {
      if (setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, reinterpret_cast<char*>(&groupSock),
                   sizeof(groupSock)) < 0)
      return -3;
  }
  // Биндим сокет на себя
  {
    if (bind(m_socket, reinterpret_cast<struct sockaddr *>(&localSock),
             sizeof(localSock)) < 0)
      return -4;
  }
  if (setTimeout(MCASTSOC_TOUT)<0)
  {
    return -5;
  }
  return 0;
}

// Устанавливаем таймаут передачи в миллисекундах
int multicastSocketIn::setTimeout(int msec)
{
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = msec * 1000;
  if(setsockopt(m_socket,SOL_SOCKET, SO_RCVTIMEO,reinterpret_cast<char*>(&timeout),static_cast<long unsigned int>(sizeof(timeout)))<0)
    return -5;
  return msec;
}
// Чтение из сокета. Эта функция является простой обёрткой вызова recv(socket=this,data*,max_size,options=NULL)
// !! Функция является блокирующей. Thread её вызвавший останавливается пока данные не будут приняты!!!
size_t multicastSocketIn::read(uint8_t *data, size_t max_size)
{
    return static_cast<size_t>(recv(m_socket, reinterpret_cast<char*>(data), static_cast<int>(max_size), 0));
}
// Чтение из сокета. Функция динамически создаёт вектор из данных прочитанных из сокета, и возвращает на него указатель.
// !! Вектор создаётся динамически и должен быть уничтожен пользователем после использования(delete ).
// !! Функция является блокирующей. Thread её вызвавший останавливается пока данные не будут приняты!!!
std::vector<uint8_t> *multicastSocketIn::read()
{
  if (m_socket == INVALID_SOCKET) return nullptr;
//  if(m_socket<0)return nullptr;
  int recieved=static_cast<int>(recv(m_socket, &buffer[0], sizeof(buffer), 0));
  if(recieved>0)
  {
    std::vector<uint8_t> *msg=new std::vector<uint8_t>(static_cast<uint8_t>(recieved));
    memcpy(msg->data(),&buffer,msg->size());
    return msg;
  }
  return nullptr;
}
