/*
 * io_endpoint.h - a part of ufw++ object envirinment for
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

#ifndef UFW_LIBS_IO_IO_ENDPOINT_H_
#define UFW_LIBS_IO_IO_ENDPOINT_H_

#include <cstdint>
#include <cstddef>
#include <cstdlib>

namespace ufw
{
  
  class ioEndpoint
  {
    public:
      virtual size_t write_to(uint8_t *data,size_t size)=0;

  };

  class ipEndpoint :public ioEndpoint
  {
    public:
      virtual void setWriteback(ioEndpoint* point)=0;
  };

} /* namespace ufw */

#endif /* UFW_LIBS_IO_IO_ENDPOINT_H_ */
