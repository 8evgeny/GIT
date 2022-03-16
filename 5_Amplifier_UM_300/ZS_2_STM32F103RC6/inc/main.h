#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "FreeRTOS.h"

#include "task.h"

void testTask1(void *args __attribute((unused)));
void testTask2(void *args __attribute((unused)));

void init();

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

#ifdef __cplusplus
}
#endif
