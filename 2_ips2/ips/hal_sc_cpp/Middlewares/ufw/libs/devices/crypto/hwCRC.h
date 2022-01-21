/*
 * hwCRC.h
 *
 *  Created on: 6 дек. 2020 г.
 *      Author: vader
 */

#pragma once

#ifndef PLATFORM_TEST

#include "main.h"


#else

#endif

#include <cstdint>
#include <cstdlib>

#include <type_traits>

#include <common/serializer.h>
#include <devices/devices.h>
#include <containers/ufw_vector.h>

//#define UFW_CRC32_TEST

namespace ufw
{
  enum crcType
  {
    CRC7=CRC_POLYLENGTH_7B,
    CRC8=CRC_POLYLENGTH_8B,
    CRC16=CRC_POLYLENGTH_16B,
    CRC32=CRC_POLYLENGTH_32B
  };
  
  class hwCRC
  {
    public:
      static hwCRC& Instance();
      hwCRC* Pointer();

      ioresult setPolynomial(crcType type,uint32_t poly);

      uint32_t Calculate(void* data_ptr,size_t size);

      uint32_t Calculate(ufw::iSerializable &object);
      uint32_t Calculate(ufw::baseContainer<auto>& data);


    private:

      hwCRC ();
      ~hwCRC ();
      hwCRC (const hwCRC &other) = delete;
      hwCRC (hwCRC &&other) = delete;
      hwCRC& operator= (const hwCRC &other) = delete;
      hwCRC& operator= (hwCRC &&other) = delete;

      void Init();

      CRC_HandleTypeDef *m_handle;
  };

} /* namespace ufw */

#ifdef UFW_CRC32_TEST
    int CRC32Test();
#endif
