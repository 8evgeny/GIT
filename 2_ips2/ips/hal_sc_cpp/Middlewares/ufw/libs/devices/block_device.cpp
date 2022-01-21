/*
 * block_device.cpp
 *
 *  Created on: 2 мая. 2018 г.
 *      Author: Dmitry Donskih
 *  Copyright © 2018-2020 Dmitry Donskih. All rights reserved.
 */

#include "block_device.h"

#include <cstring>

#include <lib_macros.hpp>
#include <common/blocks.h>

#include <system/System.h>
namespace ufw {
  namespace dev {

    /**
     * \fn  blockDevice(uint32_t, bool)
     * \brief
     *
     * \pre
     * \post
     * \param bs
     * \param is_aligned
     */
    blockDevice::blockDevice(uint16_t bs,size_t dev_capacity, bool is_aligned) :
        block_size (bs),m_capacity(dev_capacity), align_required (is_aligned)
    {
    }

    /**
     * \fn int byteRead(void*, uint32_t, size_t)
     * \brief
     *
     * \pre
     * \post
     * \param data
     * \param address
     * \param size
     * \return
     */
    int blockDevice::byteRead(void *data, address_t address, size_t size)
    {
      size_t block_read_count=blockIndex(address+size,block_size)-blockIndex(address,block_size)+1;
      uint8_t *temp = new uint8_t[block_size*block_read_count];
      int operation_result=blockRead(temp,addressAlign(address,block_size),block_read_count);
      if(operation_result<0)
        {
          delete [] temp;
          return operation_result;
        }
      memcpy(data,temp+addressOffset(address,block_size),size);
      delete [] temp;
      return size;
    }

    /**
     * \fn int byteWrite(void*, uint32_t, size_t)
     * \brief
     *
     * \pre
     * \post
     * \param data
     * \param address
     * \param size
     * \return
     */
    int blockDevice::byteWrite(void *data, address_t address, size_t size)
    {
      size_t block_read_count=blockIndex(address+size,block_size)-blockIndex(address,block_size)+1;
      uint8_t *temp = new uint8_t[block_size*block_read_count];
      int operation_result=blockRead(temp,addressAlign(address,block_size),block_read_count);
      if(operation_result<0)
        {
          delete [] temp;
          return operation_result;
        }
      memcpy(temp+addressOffset(address,block_size),data,size);
      operation_result=blockWrite(temp,addressAlign(address,block_size),block_read_count);
      delete [] temp;
      return (operation_result<0)?operation_result:size;
    }

    uint16_t blockDevice::blockSize()
    {
      return block_size;
    }

    size_t blockDevice::capacity()
    {
      return m_capacity;
    }

    blockDevice &blockDevice::interface()
    {
      return *this;
    }
  } /* namespace dev */
}

