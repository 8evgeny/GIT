/*
 * perf.h
 *
 *  Created on: 25 июн. 2020 г.
 *      Author: Donskikh Dmitry
 */

#ifndef INC_PERF_HPP_
#define INC_PERF_HPP_

#include <cinttypes>

typedef struct
{
    uint32_t us;
    uint32_t tick;
    uint32_t ms;
    float us_f;
} perf_t;

#define PERFCHECK_START 	SysTick->LOAD=0x00FFFFFF; \
				SysTick->VAL= 0UL; \
				SysTick->CTRL= SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_ENABLE_Msk
#define PERFCHECK_STOP(meter) 	SysTick->CTRL=0x0; \
				meter.tick=0x00FFFFFF-((SysTick->VAL)&SysTick_VAL_CURRENT_Msk); \
				meter.us=(meter.tick)/480; \
				meter.ms=meter.us/1000; \
				meter.us_f=((meter.tick)/((float)480.0))/1000.0

#endif /* INC_PERF_HPP_ */
