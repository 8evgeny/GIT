/*
 * tcp_server.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 22 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include <io/net/b_tcp_server.h>
#include <lib_asserts.h>
#include <protocols/bufferized_io.h>

namespace ufw
{
  
  b_tcpServer::b_tcpServer (const char *name, uint16_t port, uint8_t max_clients) :
      rtosThread (name, 512, osPriorityLow), m_free_workers (max_clients), m_tcp_port (port), m_workers (
	  max_clients)
  {
    //m_workers=new ufw::vector<ufw::tcpServerWorker<>*>(max_clients);
    m_ip_table = new uint32_t[max_clients];
    for (size_t i = 0; i < max_clients; i++)
      {
	m_workers[i] = nullptr;
      }
  }

  b_tcpServer::~b_tcpServer ()
  {
    delete[] m_ip_table;
  }

  void b_tcpServer::threadFunction ()
  {
    int listener = 0;
    struct sockaddr_in server, client;
    struct sockaddr *sa, *ca;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(m_tcp_port);

    sa = (sockaddr*) &server;
    ca = (sockaddr*) &client;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0)
      {
	LIB_ERROR_HANDL;
      }
    if ((bind(listener, sa, sizeof(sockaddr_in))) == -1)
      {
	close(listener);
	LIB_ERROR_HANDL;
      }
    while (listen(listener, 5) < 0)
      osDelay (1);

    socklen_t server_len = sizeof(server);
    while (1)
      {
	uint32_t next_id = findFree ();
	if (next_id >= 0)
	  {
	    int con_socket = accept(listener, ca, &server_len);
	    if (con_socket < 0)
	      {
		close(listener);
		LIB_ERROR_HANDL;
	      } else
	      {
		create_connection (con_socket, client.sin_addr.s_addr, next_id);
	      }
	  } else
	  {
	    osDelay (10);
	  }
      }
  }
  
  void b_tcpServer::freeWorker (uint32_t id)
  {
    m_workers[id] = nullptr;
  }
  
  ioEndpoint* b_tcpServer::getClientOutput (uint8_t id)
  {
    if (id < m_workers.size ())
      {
	return (ioEndpoint*)m_workers[id];
      }
    return nullptr;
  }

  int b_tcpServer::findFree ()
  {
    for (int i = 0; i < m_workers.size (); i++)
      {
	if (m_workers[i] == nullptr) return i;
      }
    return -1;
  }

//  void b_tcpServer::create_connection (int sock, uint32_t addr)
//  {
//    uint32_t t_place=findFree();
//    m_workers[t_place]=new tcpServerWorker<BufferizedIO>(sock, addr, t_place,this);
//  }

} /* namespace ufw */
