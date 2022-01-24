/*
 * error_blinker.h - a part of ufw++ object envirinment for
 * microcontrollers.
 *
 *   Created on: 23 дек. 2020 г.
 *       Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with
 * the permission of the author.
 * 
 */

#ifndef UFW_LIBS_ERROR_ERROR_BLINKER_H_
#define UFW_LIBS_ERROR_ERROR_BLINKER_H_

#include <ipcThread.h>
#include <ipcQueue.h>
#include <basicDO.h>
#include <basicDI.hpp>
#include "system_c.h"
#include <types/array_type.h>

using blink_pattern_t=ufw::array_type<uint8_t,10>;

namespace ufw
{
  
  class blinker :ipcThread
  {
    public:
      blinker();
      virtual ~blinker ();
      void taskRunFcn();
    protected:
      void tick();
      blink_pattern_t error_led {0,0,1,1,0,0,1,1,0,0};
      uint8_t* iter,*beg_iter,*end_iter;
      DOUnit& out=DOUnit::getInstance();
  };

} /* namespace ufw */

#endif /* UFW_LIBS_ERROR_ERROR_BLINKER_H_ */
