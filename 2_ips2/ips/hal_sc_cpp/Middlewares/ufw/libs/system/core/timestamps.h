/*
 * timestamps.h
 *
 *  Created on: 18 дек. 2020 г.
 *      Author: vader
 */


#ifndef UFW_LIBS_SYSTEM_TIMESTAMPS_H_
#define UFW_LIBS_SYSTEM_TIMESTAMPS_H_

#ifndef PLATFORM_TEST

#include "main.h"
#include <cinttypes>

namespace ufw
{
  static uint64_t timestamp;
  inline void ts_init()
  {
    timestamp=0;
    SysTick->LOAD=0x00FFFFFF;
    SysTick->VAL= 0UL;
    SysTick->CTRL= SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_ENABLE_Msk;
  }
  inline void ts_run()
  {
    SysTick->LOAD=0x00FFFFFF;
    SysTick->VAL= 0UL;
    SysTick->CTRL= SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_ENABLE_Msk;
  }
  inline uint64_t timestamp()
  {
    SysTick->CTRL=0x0;
    timestamp+=(0x00FFFFFF-((SysTick->VAL)&SysTick_VAL_CURRENT_Msk))/480;
    return timestamp;
  }
}

#else

#endif





#endif /* UFW_LIBS_SYSTEM_TIMESTAMPS_H_ */
