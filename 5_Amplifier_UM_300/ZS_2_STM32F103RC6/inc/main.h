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
#include "stdio.h"

#define mainECHO_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )

static QueueHandle_t uart_txq;				// TX queue for UART

void testTask1(void *args __attribute((unused)));
void testTask2(void *args __attribute((unused)));
void testTask3(void *args __attribute((unused)));
void testTask4(void *args __attribute((unused)));


void testUART1(void *args __attribute__((unused)));
void testUART2(void *args);

void checkButtons();
void setStateRele();

void i2c_main_vers1();
void i2c_main_vers2();
void LCDI2C_write_String(char* str);

void gpio_setup();
//void uart_setup();

void stringToUart(const char *s);

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

void setTestLed1(bool set);
void setTestLed2(bool set);
void setTestLed3(bool set);
void setTestLed4(bool set);


//#ifdef __cplusplus
//}
//#endif
