/*
 * debug_server.h - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 *  Created on: 31 янв. 2021 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2021 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */
#ifndef UFW_LIBS_PROTOCOLS_IPSDP_DEBUG_SERVER_H_
#define UFW_LIBS_PROTOCOLS_IPSDP_DEBUG_SERVER_H_

#include "cmsis_os2.h"
#include <io/net/b_tcp_server.h>
#include "ipsdpdefs.h"

#include <cstdarg>
#include <printf.h>

namespace ufw
{
  class DebugHandler :public ProtocolHandler
  {
    public:
      DebugHandler();
      virtual ~DebugHandler();
      int handleMessage (ByteArray *msg) override;
      void setWritebackIF (ufw::ioEndpoint *point) override;
      static inline bool isValid(uint8_t* message)
          {
            return (*(uint32_t*)message==IPSDP::magic);
          }
      static void timeoutFunction(void* arg);
      void Heartbeat();
    protected:
      uint32_t Gettype(uint8_t* message);
      void SetIP(uint8_t *msg);
      void SetVol(uint8_t* msg);
      void GetInfo();
      void SetCodec(uint8_t* msg);

      ByteArray m_buffer,m_hb_buffer;
      ioEndpoint* m_parent=nullptr;
      osTimerId_t m_heartbeat_timer;




  };
  
  class DebugServer
  {
    public:
      static DebugServer& Instance ();
      DebugServer* pointer();
      void printf(const char *fmt, ...);
    private:
      char m_buffer[512];
      tcpServer<DebugHandler> server;
      DebugServer ();
      ~DebugServer ();
      DebugServer (const DebugServer &other) = delete;
      DebugServer (DebugServer &&other) = delete;
      DebugServer& operator= (const DebugServer &other) = delete;
      DebugServer& operator= (DebugServer &&other) = delete;
  };

} /* namespace ufw */

#endif /* UFW_LIBS_PROTOCOLS_IPSDP_DEBUG_SERVER_H_ */
