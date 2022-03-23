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

#define toMilandr_BtnImpedance_On     "Q"
#define toMilandr_BtnImpedance_Off    "W"
#define toMilandr_BtnCalibr_On        "E"
#define toMilandr_BtnCalibr_Off       "R"
#define toMilandr_BtnReset_On         "T"
#define toMilandr_BtnReset_Off        "Y"
#define toMilandr_SignalPowerOn_ON    "U"
#define toMilandr_SignalPowerOn_OFF   "I"
#define toMilandr_SignalImpedanse_ON  "O"
#define toMilandr_SignalImpedanse_OFF "P"
#define toMilandr_SignalTranslate_ON  "A"
#define toMilandr_SignalTranslate_OFF "S"
#define toMilandr_SignalFromOut_ON    "D"
#define toMilandr_SignalFromOut_OFF   "F"
#define toMilandr_SignalMic_ON        "G"
#define toMilandr_SignalMic_OFF       "H"

#define fromMilandr_BtnImpedance_On_OK     'q'
#define fromMilandr_BtnImpedance_Off_OK    'w'
#define fromMilandr_BtnCalibr_On_OK        'e'
#define fromMilandr_BtnCalibr_Off_OK       'r'
#define fromMilandr_BtnReset_On_OK         't'
#define fromMilandr_BtnReset_Off_OK        'y'
#define fromMilandr_SignalPowerOn_ON_OK    'u'
#define fromMilandr_SignalPowerOn_OFF_OK   'i'
#define fromMilandr_SignalImpedanse_ON_OK  'o'
#define fromMilandr_SignalImpedanse_OFF_OK 'p'
#define fromMilandr_SignalTranslate_ON_OK  'a'
#define fromMilandr_SignalTranslate_OFF_OK 's'
#define fromMilandr_SignalFromOut_ON_OK    'd'
#define fromMilandr_SignalFromOut_OFF_OK   'f'
#define fromMilandr_SignalMic_ON_OK        'g'
#define fromMilandr_SignalMic_OFF_OK       'h'


static bool btnImpedance = 0;
static bool btnCalibrovka = 0;
static bool btnReset = 0;
static bool signalPowerOn = 0;
static bool signalImpedance = 0;
static bool signalTranslate = 0;
static bool input_IMP_UPR = 0;
static bool input_VOLT_UPR = 0;
static bool input_CUR_UPR1 = 0;
static bool input_CUR_UPR2 = 0;
static bool signalVnesh = 0;
static bool signalMic = 0;



static QueueHandle_t uart_txq;				// TX queue for UART

void testTask1(void *args __attribute((unused)));
void testTask2(void *args __attribute((unused)));
void testTask3(void *args __attribute((unused)));
void testSendUartCommand(void *args __attribute((unused)));


void testUART1(void *args __attribute__((unused)));

void checkInputs();
char * checkReceivedByteFromMilandr(char data);
void setOutputs();


void i2c_main_vers1();
void i2c_main_vers2();
void stringToLcd(char* str);


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
