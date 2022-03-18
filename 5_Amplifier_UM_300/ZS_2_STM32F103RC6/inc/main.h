#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

//#ifdef __cplusplus
//extern "C" {
//#endif

#include <string.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "uartlib.h"
#include "task.h"

#define mainECHO_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )

static QueueHandle_t uart_txq;				// TX queue for UART

void testTask1(void *args __attribute((unused)));
void testTask2(void *args __attribute((unused)));
void testTask3(void *args __attribute((unused)));
void testTask4(void *args __attribute((unused)));
void testTask5(void *args __attribute((unused)));

void testUART1(void *args __attribute__((unused)));

//void gpio_setup();
//void uart_setup();

void setImpedanceRele(bool);
void setReleTr1(bool);
void setReleTr2(bool);
void setReleTr3(bool);
void setReleTr4(bool);
void setReadyLed(bool);
void setUpr1(bool);
void setUpr2(bool);
void setLedShortOut(bool);
void setLedBreakOut(bool);
void setLedOvercutOut(bool);
void setLedOverheatOut(bool);
void setfan(bool);
void setPowerOut(bool);
void setErrorRele(bool set);
void setReleLine1(bool set);
void setRele24V(bool set);


//#ifdef __cplusplus
//}
//#endif
