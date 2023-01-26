#pragma once
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
     * Initialize the timers used for delays.
     */
void delay_setup(void);
void Delay(uint32_t ms);
void DelayMC(uint32_t mc);
/**
     * busy wait for a number of usecs.
     * @param us number of usecs to delay.
     */
void delay_us(uint16_t us);

#ifdef __cplusplus
}
#endif
