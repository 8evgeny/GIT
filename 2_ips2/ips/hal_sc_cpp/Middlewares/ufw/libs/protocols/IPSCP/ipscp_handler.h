/*
 * ipscp_handler.h - a part of ufw++ object envirinment for
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

#ifndef UFW_LIBS_PROTOCOLS_IPSCP_IPSCP_HANDLER_H_
#define UFW_LIBS_PROTOCOLS_IPSCP_IPSCP_HANDLER_H_

#include <protocols/message_handler.h>

#include <cstdint>
#include <cstddef>
#include <cstdlib>

namespace ufw
{
  
  class ipscpHandler : public MessageHandler
  {
    public:
      ipscpHandler ();
      ~ipscpHandler ();
      int handleMessage(ByteArray* msg) override;
      void reset();
    private:
      void Accept();
      void Accept(uint32_t size);
      int CommandRecieveing(ByteArray* msg);
      int iptableRecieveing(ByteArray* msg);
      int sysinfoRecieveing(ByteArray* msg);
      int keysRecieveing(ByteArray* msg);
      int clinesRecieveing(ByteArray* msg);
      void EOT();
      ByteArray* m_int_buffer=nullptr;
      uint32_t m_state=0;
      uint8_t m_errors=0;
  };

} /* namespace ufw */

#endif /* UFW_LIBS_PROTOCOLS_IPSCP_IPSCP_HANDLER_H_ */
