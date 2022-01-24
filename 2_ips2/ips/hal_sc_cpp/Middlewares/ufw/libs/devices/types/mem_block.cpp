/*
 * mem_block.cpp
 *
 *  Created on: 3 дек. 2020 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2020 Dmitry Donskih. All rights reserved.
 */

#include "mem_block.h"
#include <cstring>

#include <common/blocks.h>

#ifdef PLATFORM_TEST
#include <cstdio>
#else
#endif

#ifdef PLATFORM_TEST
namespace ufw {
  namespace dev {

    memBlock::memBlock()
      :ufw::dev::blockDevice(64,sizeof(block),true)
    {
      clear();
    }

    memBlock &memBlock::Instance()
    {
      static memBlock s_instance;
      return s_instance;
    }

    memBlock* memBlock::pointer()
    {
      return this;
    }

    int memBlock::blockRead(void *data, address_t address, size_t count)
    {
      return read(data,block_size*count,addressAlign(address,block_size));
    }

    int memBlock::blockWrite(void *data, address_t address, size_t count)
    {
      return write(data,block_size*count,addressAlign(address,block_size));
    }

    void memBlock::clear()
    {
      memset(&block,'0',sizeof(block));
    }

    int memBlock::read(void *data, size_t size, address_t address)
    {
      if((address+size)>=sizeof(block))return ioresult::IncorrectAddress;
      memcpy(data,(&block[0])+address,size);
      return size;
    }

    int memBlock::write(void *data, size_t size, address_t address)
    {
      if((address+size)>=sizeof(block))return ioresult::IncorrectAddress;
      memcpy((&block[0])+address,data,size);
      return size;
    }

  
    ttY &operator<<(ttY &terminal, ufw::dev::memBlock &data)
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
