#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "FreeRTOS.h"
#include "task.h"

void task1(void *args __attribute((unused)));
void task2(void *args __attribute((unused)));
void task3(void *args __attribute((unused)));
void init();
