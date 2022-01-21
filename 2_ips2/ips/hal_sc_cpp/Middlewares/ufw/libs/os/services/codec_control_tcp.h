/*
 * codec_control_tcp.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 19 янв. 2021 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_OS_SERVICES_CODEC_CONTROL_TCP_H_
#define UFW_LIBS_OS_SERVICES_CODEC_CONTROL_TCP_H_

#include <cstdint>
#include <cstddef>
#include <cstdlib>

namespace ufw
{
  
  class codecControlTCP
  {
    public:
      codecControlTCP (uint16_t port);
      virtual ~codecControlTCP ();
  };

} /* namespace ufw */

#endif /* UFW_LIBS_OS_SERVICES_CODEC_CONTROL_TCP_H_ */
