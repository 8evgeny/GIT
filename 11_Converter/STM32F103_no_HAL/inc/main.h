#pragma once

#include <string.h>

#ifdef uselibopencm3
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#endif



#include <errno.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "uartlib.h"
#include "task.h"
#include "stdio.h"

#define mainECHO_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )


static QueueHandle_t usart_diagnostic_txq;

void testTask1(void *args __attribute((unused)));
void testTask2(void *args __attribute((unused)));
void testTask3(void *args __attribute((unused)));
void testImpuls(void *args);
void delay();
void gpio_setup();






