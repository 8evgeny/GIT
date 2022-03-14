#ifdef __cplusplus
extern "C" {
#endif

#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_usb_handlers.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_iwdg.h"
#include "gpio.h"

#ifndef _MAIN
    
#define _MAIN

#define LED1_ERROR	PORT_Pin_5
#define LED2_REC	PORT_Pin_6
#define BUZZER		PORT_Pin_7
#define RS			PORT_Pin_8
#define RW 			PORT_Pin_9
#define EN 			PORT_Pin_10


//---------------------------------
void GPIO_init();

//---------------------------------
// Задержка SysTick
//---------------------------------
extern volatile uint32_t delay_dec;
//---------------------------------
#endif

#ifdef __cplusplus
}
#endif
