#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_iwdg.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_uart.h"
//#include "MDR32F9Qx_eeprom.h"
#include <stdbool.h>


//---------------------------------

//---------------------------------
#ifndef _MAIN
#define _MAIN


static int i_delay; // Глобальная переменная счетчика, которая используется в функции delay()
#define delay(T) for(i_delay = T; i_delay > 0; i_delay--)


//---------------------------------
#endif

#ifdef __cplusplus
}
#endif
