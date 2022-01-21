/*
 * ipscp_handler.cpp - a part of ufw++ object envirinment for
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

#include <protocols/IPSCP/ipscp_handler.h>

namespace ufw
{
  
  ipscpHandler::ipscpHandler ()
  {
    
  }
  
  ipscpHandler::~ipscpHandler ()
  {
  }
  
  int ipscpHandler::handleMessage (ByteArray *msg)
  {
  }
  
  void ipscpHandler::reset ()
  {
  }
  
  void ipscpHandler::Accept ()
  {
  }
  
  void ipscpHandler::Accept (uint32_t size)
  {
  }
  
  int ipscpHandler::CommandRecieveing (ByteArray *msg)
  {
  }
  
  int ipscpHandler::iptableRecieveing (ByteArray *msg)
  {
  }
  
  int ipscpHandler::sysinfoRecieveing (ByteArray *msg)
  {
  }
  
  int ipscpHandler::keysRecieveing (ByteArray *msg)
  {
  }
  
  int ipscpHandler::clinesRecieveing (ByteArray *msg)
  {
  }
  
  void ipscpHandler::EOT ()
  {
  }

} /* namespace ufw */
