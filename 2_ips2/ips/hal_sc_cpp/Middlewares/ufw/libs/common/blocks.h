/*
 *  Created on: 24 сент. 2020 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include "address_type.h"


/**
 * @brief blockIndex Returns index of block alligned to block_size by address
 * @param address
 * @param block_size - in bytes
 * @return
 */
inline uint16_t blockIndex(address_t address, size_t block_size)
{
  return address / block_size;
}


/**
 * @brief addressAlign Returns address aligned to block size
 * @param address
 * @param block_size - in bytes
 * @return
 */
inline address_t addressAlign(address_t address,size_t block_size)
{
  return blockIndex (address,block_size) * block_size;
}


/**
 * @brief addressOffset Returns offset of given address relative to block begin
 * @param address
 * @param block_size - in bytes
 * @return
 */
inline address_t addressOffset(address_t address,size_t block_size)
{
  return address % block_size;
}

template <size_t BlockSize>
uint16_t blockIndex(address_t address)
{
  return address / BlockSize;
}

