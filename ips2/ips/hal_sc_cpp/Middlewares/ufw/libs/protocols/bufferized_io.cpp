/*
 * bufferized_io.cpp - a part of ufw++ object envirinment for
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

#include <protocols/bufferized_io.h>

#define MAXIOBUFFERIZED 1000

namespace ufw
{
  
  BufferizedIO::BufferizedIO ()
  :ProtocolHandler()
  ,ioEndpoint()
  {

  }
  
  BufferizedIO::~BufferizedIO ()
  {

  }

  size_t BufferizedIO::write_to (uint8_t *data, size_t size)
  {
    m_parent->write_to(data, size);
    return size;
  }
  
  int BufferizedIO::handleMessage (ByteArray *msg)
  {
    if(m_buffer.size()<MAXIOBUFFERIZED)
      {
	m_buffer.append(*msg);
	write_to(msg->data(), msg->size());
      }
    delete msg;
    return 0;
  }
  
  void BufferizedIO::setWritebackIF (ufw::ioEndpoint *point)
  {
    m_parent=point;
  }
  

} /* namespace ufw */
