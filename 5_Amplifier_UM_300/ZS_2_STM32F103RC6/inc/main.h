#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "FreeRTOS.h"
#include "task.h"

void task1(void *args __attribute((unused)));
void task2(void *args __attribute((unused)));
void task3(void *args __attribute((unused)));
void init();
void setImpedanceRele();
void resetImpedanceRele();
void setReleTr1();
void resetReleTr1();
void setReleTr2();
void resetReleTr2();
void setReleTr3();
void resetReleTr3();
void setReleTr4();
void resetReleTr4();
void setReadyLed();
void resetReadyLed();
void setUpr1();
void resetUpr1();
void setUpr2();
void resetUpr2();
void setLedShortOut();
void resetLedShortOut();
void setLedBreakOut();
void resetLedBreakOut();
void setLedOvercutOut();
void resetLedOvercutOut();
void setLedOverheatOut();
void resetLedOverheatOut();
void fanOn();
void fanOff();
