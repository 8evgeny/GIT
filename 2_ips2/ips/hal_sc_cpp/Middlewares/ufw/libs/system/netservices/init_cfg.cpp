/*
 * init_cfg.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 23 дек. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include <system/netservices/init_cfg.h>
#include <system/configs/static_config.h>
#include <system/System.h>
static ufw::System& Self = ufw::System::instance();
static const uint8_t s_uinit_pattern[10]={108,1,75,81,33,120,74,45,85,56};
#include "socket.h"

#define FW_UINIT_MN_SIZE	10
#define FW_UINIT_CFG_SIZE 	32

namespace ufw
{
  
  initCFG& initCFG::Instance ()
  {
    static initCFG s_initcfg;
    return s_initcfg;
  }
  
  initCFG* initCFG::pointer ()
  {
    return this;
  }
  
  void initCFG::taskRunFcn ()
  {
    struct sockaddr_in 	server, client;
    struct sockaddr 	*sa, *ca;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons (uint16_t(2648));

    sa = (sockaddr*) &server;
    ca = (sockaddr*) &client;

    while (!Self.isIPready())
      osDelay (100);

    int sock = socket (AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
      {
	Error_Handler ();
      }

    if ((bind (sock, sa, sizeof(sockaddr_in))) == -1)
      {
	Error_Handler ();
      }

    struct timeval timeout;
    timeout.tv_sec = 59;
    timeout.tv_usec = 0 * 1000;
    if(setsockopt(sock,SOL_SOCKET, SO_RCVTIMEO,reinterpret_cast<char*>(&timeout),sizeof(timeout))<0)Error_Handler();
    while(1)
      {
	uint8_t *buffer=new uint8_t[128];
	while(listen(sock,5)!=0)osDelay(1);
	socklen_t other;
	int connection=accept(sock, ca, &other);
	int length=recv(connection, buffer,128,NULL);
	if (!proceed(buffer,length))
	  {
	    delete buffer;
	    continue;
	  }
	shutdown(connection,SHUT_RDWR);
	close(connection);
	delete buffer;
      }
  }
  
  initCFG::initCFG ()
  :ipcThread("XXX", 256, osPriorityLow)
  {
  }
  
  initCFG::~initCFG ()
  {
  }

  bool initCFG::proceed (uint8_t *data, size_t size)
  {
    if (size<FW_UINIT_CFG_SIZE)return false;
    for (size_t i=0;i<FW_UINIT_MN_SIZE;i++)
      {
	if(s_uinit_pattern[i]!=data[i])return false;
      }
   Self.ConfigFlash().writeSystem(&data[10], FW_UINIT_CFG_SIZE-FW_UINIT_MN_SIZE, 0x00);
   md5 key=Self.Hash().compute(&data[10],FW_UINIT_CFG_SIZE-FW_UINIT_MN_SIZE);
   Self.ConfigFlash().writeSystem(key.data(),key.size(),uint32_t(0xA0));
   Self.stddebug()<<"Initial config successfully flashed! Reboot at 2s.\n";
   osDelay(200);
   Self.reboot();
   return true;
  }

} /* namespace ufw */
