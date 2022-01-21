/*
 * ITM.cpp
 *
 *  Created on: 21 сент. 2020 г.
 *      Author: Dmitry Donskih
 */

/*
 * ITMacrocell.cpp - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */

#include  "ITMacrocell.hpp"
#include  "main.h"

#define UFW_ITMDBG_SIZE	(1024)

extern ufw::ioITM* jdebug=ufw::ioITM::Instance().pointer();

void itmPutc (char c)
{
  if (c == '\n') itmPutc ('\r');
  ITM_SendChar (c); // TODO: write thread safety
}

void itmPuts (const char *str)
{
  while (*str)
    {
      itmPutc (*str++);
    }
}

static uint8_t io_buffer_itmdbg[UFW_ITMDBG_SIZE];

namespace ufw
{

  ioITM& ioITM::Instance ()
  {
    static ioITM s_io_itmdbg;
    return s_io_itmdbg;
  }

  ioITM* ioITM::pointer ()
  {
    return this;
  }

  void ioITM::write_t (const char *buf, size_t size)
  {
    for (uint32_t i = 0; i < size; i++)
      {
	itmPutc (buf[i]);
      }
  }



  ioITM::ioITM () :
      ttY (&io_buffer_itmdbg, sizeof(io_buffer_itmdbg))
  {
  }

//  void ioITM::printfl (const char *fmt, ...)
//  {
//    va_list arp;
//    va_start(arp, fmt);
//     int len=SEGGER_RTT_vprintf(0,fmt,&arp);//(unsigned BufferIndex, const char * sFormat, va_list * pParamList);
//    va_end(arp);
//  }

  ioITM::~ioITM ()
  {
  }
}
