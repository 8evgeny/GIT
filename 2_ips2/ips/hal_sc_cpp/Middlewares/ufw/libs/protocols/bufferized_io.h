/*
 * bufferized_io.h - a part of ufw++ object envirinment for
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

#pragma once

#include <io/io_endpoint.h>
#include <protocols/message_handler.h>
#include <containers/ufw_containers.h>

namespace ufw
{
  
  class BufferizedIO : public ufw::ProtocolHandler, public ufw::ioEndpoint
  {
    public:
      BufferizedIO ();
      virtual ~BufferizedIO ();
      size_t write_to (uint8_t *data, size_t size) override;
      int handleMessage (ByteArray *msg) override;
      void setWritebackIF (ufw::ioEndpoint *point) override;

    private:
      ByteArray m_buffer;
      ioEndpoint* m_parent=nullptr;

  };

} /* namespace ufw */

