/*
 * message_handler.h - a part of ufw++ object envirinment for
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

#ifndef UFW_LIBS_PROTOCOLS_MESSAGE_HANDLER_H_
#define UFW_LIBS_PROTOCOLS_MESSAGE_HANDLER_H_

#include <containers/ufw_containers.h>
#include <io/io_endpoint.h>

namespace ufw
{
  
  class MessageHandler
  {
    public:
      virtual int handleMessage(ByteArray* msg)=0;
  };

  class ProtocolHandler :public virtual MessageHandler
  {
    public:
      virtual int handleMessage(ByteArray* msg)=0;
      virtual void setWritebackIF(ioEndpoint* point)=0;
  };

} /* namespace ufw */

#endif /* UFW_LIBS_PROTOCOLS_MESSAGE_HANDLER_H_ */
