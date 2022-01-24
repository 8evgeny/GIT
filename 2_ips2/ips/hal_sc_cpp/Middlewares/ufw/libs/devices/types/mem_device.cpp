/*
 * mem_device.cpp
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#include "mem_device.h"
#include <cstring>

#ifdef PLATFORM_TEST
#include <cstdio>
#else
#endif

#ifdef PLATFORM_TEST
namespace ufw {
  namespace dev {

    memDevice::memDevice()
      :ufw::dev::basicDevice()
    {
      clear();
    }

    memDevice &memDevice::Instance()
    {
      static memDevice s_instance;
      return s_instance;
    }

    memDevice *memDevice::pointer()
    {
      return this;
    }

    int memDevice::read(void *data, size_t size, uint32_t address)
    {
      if((address+size)>=sizeof(block))return ioresult::IncorrectAddress;
      memcpy(data,(&block[0])+address,size);
      return size;
    }

    int memDevice::write(void *data, size_t size, uint32_t address)
    {
      if((address+size)>=sizeof(block))return ioresult::IncorrectAddress;

      memcpy((&block[address]),data,size);
      return size;
    }

    int memDevice::ioctl(ctl_reg reg, ioops request, uint32_t &value)
    {
      if((request<0)||(request>1)) return ioresult::WRONGCMD;
      printf("memblock::ioctl trying ");
      if(request==ioops::readreg)
        {
          printf("reads register 0x%x\n",reg);
        }else
        {
          printf("write 0x%x to register 0x%x\n",value,reg);
        }
      return ioresult::OK;
    }

    void memDevice::clear()
    {
      memset(&block,'0',sizeof(block));
    }
  
    ttY &operator<<(ttY &terminal, ufw::dev::memDevice &data)
    {
      terminal.printfl("memDevice data:\n");
      for(size_t i=0;i<sizeof (data.block);i++)
        {
          terminal.printfl("%c",(char)data.block[i]);
        }
      return terminal;
    }
  } /* namespace dev */
}
#endif

