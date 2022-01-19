/*
 * stream_device.cpp
 *
 *  Created on: 26 нояб. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */
#include <system/System.h>
//#include <cstdio>
#include <cstdarg>
#include <printf.h>
#include "stream_device.h"

#ifdef PLATFORM_TEST
#include <iostream>
#else

#endif

namespace ufw {
  namespace dev {

    streamDevice::streamDevice() :
	dev::streamDevice (ufw::System::instance ().getBuffer (),
			   ufw::System::instance ().buffer_size)
    {
#ifdef PLATFORM_TEST
      std::cout << "streamDevice::streamDevice()\n";
#endif
    }

    streamDevice::~streamDevice()
    {
#ifdef PLATFORM_TEST
      std::cout << "streamDevice::~streamDevice()\n";
#endif
      ufw::System::instance ().freeBuffer (linebuffer);
    }

    streamDevice::streamDevice(void *buf, size_t size)
    {
      if(!buf) errors::Handle (errors::Critical, errors::Memory, __PRETTY_FUNCTION__);
      linebuffer = reinterpret_cast<char*> (buf);
      b_size = size;
    }

    streamDevice::streamDevice (size_t size)
    :b_size(size)
    {
      linebuffer = new char[size];
    }

    void streamDevice::resizeBuffer(size_t size)
    {
      if(size > b_size) errors::Handle (errors::Assert, errors::Logical, __PRETTY_FUNCTION__);
    }

    void streamDevice::printfl(const char *fmt, ...)
    {
      va_list arp;
      va_start(arp, fmt);
      int len = vsnprintf (linebuffer, b_size, fmt, arp);
      va_end(arp);
      write_t (linebuffer, len);
    }

    void streamDevice::cls()
    {
      write_t ("\033[2J", 8);
    }

    void streamDevice::endl()
    {
      printfl ("\f\n");
    }

    void streamDevice::flush()
    {

    }

    void streamDevice::s_close()
    {
      ufw::System::instance ().freeBuffer (linebuffer);
    }
    
  
  } /* namespace dev */
}

ttY &operator<<(ttY &terminal, char data)
{
  terminal.printfl("%c", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, unsigned char data)
{
  terminal.printfl("%u", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, short data)
{
  terminal.printfl("%h", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, unsigned short data)
{
  terminal.printfl("%h", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, int data)
{
  terminal.printfl("%d", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, unsigned int data)
{
  terminal.printfl("%d", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, long data)
{
  terminal.printfl("%l", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, unsigned long data)
{
  terminal.printfl("%ul", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, const char *data)
{
  terminal.printfl("%s", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, const unsigned char *data)
{
  terminal.printfl("%s", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, double data)
{
  terminal.printfl("%f", data);
  return terminal;
}

ttY &operator<<(ttY &terminal, void *data)
{
  terminal.printfl("%x", (uint64_t)data);
  return terminal;
}
