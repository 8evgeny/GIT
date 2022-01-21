#include "mldr124_delay.h"
#include "mldr124_clk.h"

static uint32_t sysClkMhz;

void DELAY_Config( void )
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;                                     // Включение модуля DWT
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0;
    sysClkMhz = CLK_GetSourceClk( CLK_SOURCE_MAX_CLK ) / 1000000;
}

__ramfunc void DELAY_us( uint32_t ulUs )
{
    DWT->CYCCNT = 0;
    while ( DWT->CYCCNT < ( ulUs * sysClkMhz ) );    
}

__ramfunc void DELAY_ms( uint32_t ulMs )
{
    DWT->CYCCNT = 0;
    while ( DWT->CYCCNT < ( ulMs * 1000 * sysClkMhz ) );    
}

