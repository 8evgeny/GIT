#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

#include "delay.h"

void delay_setup(void)
{
    /* set up a microsecond free running timer for ... things... */
    rcc_periph_clock_enable(RCC_TIM6);
    /* microsecond counter */
    timer_set_prescaler(TIM6, rcc_apb1_frequency / 1000000 - 1);
    timer_set_period(TIM6, 0xffff);
    timer_one_shot_mode(TIM6);
}

void delay_us(uint16_t us)
{
    TIM_ARR(TIM6) = us * 10;
    TIM_EGR(TIM6) = TIM_EGR_UG;
    TIM_CR1(TIM6) |= TIM_CR1_CEN;
    //timer_enable_counter(TIM6);
    while (TIM_CR1(TIM6) & TIM_CR1_CEN);
}

void Delay(uint32_t ms)
{
    delay_setup();
    delay_us(ms * 1000);
    //        volatile uint32_t nCount;
    //        RCC_ClocksTypeDef RCC_Clocks;
    //        RCC_GetClocksFreq (&RCC_Clocks);

    //        nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
    //        for (; nCount!=0; nCount--);
}


void DelayMC(uint32_t mc)
{
    delay_setup();
    delay_us(mc);

    //        volatile uint32_t nCount;
    //        RCC_ClocksTypeDef RCC_Clocks;
    //        RCC_GetClocksFreq (&RCC_Clocks);

    //        nCount=(RCC_Clocks.HCLK_Frequency/10000000)*mc;
    //        for (; nCount!=0; nCount--);
}
