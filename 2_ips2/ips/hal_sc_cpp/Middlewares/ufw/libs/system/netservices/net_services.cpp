/*
 * net_services.cpp - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 22 дек. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#include <system/netservices/net_services.h>
#include <io/net/b_tcp_server.h>
//#include <protocols/bufferized_io.h>
#include <system/System.h>
#include <protocols/IPSDP/debug_server.h>
#include <protocols/islp.h>
static ufw::System& Self = ufw::System::instance();
extern __IO uint32_t uwTick;

namespace ufw
{
  
  netServices& netServices::Instance ()
  {
    static netServices s_netservices;
    return s_netservices;
  }
  
  netServices* netServices::pointer ()
  {
    return this;
  }
  
  netServices::netServices ()
  {
    Self.stddebug()<<"IP Ready";
    ufw::osServices::Instance().AfterIP();
//    announce_test= new AnnouncerTest;
//    tcpServer<ufw::BufferizedIO> *BufferizedEcho = new ufw::tcpServer<ufw::BufferizedIO>(2121, 4, "Echo Server");
    DebugServer::Instance();
    ISLP::IslpHandler::Instance();
  }
  
  netServices::~netServices ()
  {
  }

} /* namespace ufw */
