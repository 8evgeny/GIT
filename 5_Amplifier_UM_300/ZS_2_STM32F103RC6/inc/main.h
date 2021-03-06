#pragma once

#include <string.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/i2c.h>
#include <errno.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "queue.h"
#include "uartlib.h"
#include "task.h"
#include "stdio.h"

#define mainECHO_TASK_PRIORITY				( tskIDLE_PRIORITY + 1 )

//Команды  Stm -> Milandr   (обработка в  check_input.c)
//В тесте отправляются в цикле в UART
#define toMilandr_BtnImpedance_On          "Q"
#define toMilandr_BtnImpedance_Off         "W"
#define toMilandr_BtnCalibr_On             "E"
#define toMilandr_BtnCalibr_Off            "R"
#define toMilandr_BtnReset_On              "T"
#define toMilandr_BtnReset_Off             "Y"
#define toMilandr_SignalPowerOn_ON         "U"
#define toMilandr_SignalPowerOn_OFF        "I"
#define toMilandr_SignalImpedanse_ON       "O"
#define toMilandr_SignalImpedanse_OFF      "P"
#define toMilandr_SignalTranslate_ON       "A"
#define toMilandr_SignalTranslate_OFF      "S"
#define toMilandr_SignalFromOut_ON         "D"
#define toMilandr_SignalFromOut_OFF        "F"
#define toMilandr_SignalMic_ON             "G"
#define toMilandr_SignalMic_OFF            "H"
#define toMilandr_BlankCommand             "&"

//Milandr -> Stm (подтверждение получения команды)
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
#define fromMilandr_MilandrNoReceiveCmd    '#'

//Команды Milandr -> Stm
#define cmdFromMilandr_OVERHEAT_MC_ON      'x'
#define cmdFromMilandr_OVERHEAT_MC_OFF     'c'
#define cmdFromMilandr_BOARD_OK_ON        'v'
#define cmdFromMilandr_BOARD_OK_OFF       'b'
#define cmdFromMilandr_ERROR_MC_ON         'n'
#define cmdFromMilandr_ERROR_MC_OFF        'm'

//Stm -> Milandr (подтверждение получения команды)
#define toMilandr_OVERHEAT_MC_ON_OK        "X"
#define toMilandr_OVERHEAT_MC_OFF_OK       "C"
#define toMilandr_BOARD_OK_ON_OK          "V"
#define toMilandr_BOARD_OK_OFF_OK         "B"
#define toMilandr_ERROR_MC_ON_OK           "N"
#define toMilandr_ERROR_MC_OFF_OK          "M"

static uint8_t ii;

static bool MilandrNoReceiveCmd = 0;
static bool UncnownCmdFromMilandr = 0;

//Stm -> Milandr
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

//Milandr -> Stm
static bool OVERHEAT_MC = 0;
static bool BOARD_OK = 0;
static bool ERROR_MC = 0;

static QueueHandle_t uart_txq;				// TX queue for UART
static QueueHandle_t usart_diagnostic_txq;

void digitaPOT(void *args __attribute((unused)));
void digitalPOT_send_data(uint8_t byte1,uint8_t byte2,uint8_t byte3 );

void testTask1(void *args __attribute((unused)));
void testTask2(void *args __attribute((unused)));
void testTask3(void *args __attribute((unused)));
void testSendUartCommand(void *args __attribute((unused)));
void SendUartCommand(void *args __attribute((unused)));
void testUART1(void *args __attribute__((unused)));
void checkInputs(void *args __attribute__((unused)));
void setOutputs(void *args __attribute((unused)));
void i2c_main_vers2(void *args __attribute((unused)));
void send_to_POT(uint8_t data);

const char * checkReceivedByteFromMilandr(char data);

void i2c_main_vers1();
void stringToLcd(const char* str);
void gpio_setup();

void stringToUart(const char *s);
void stringTo_diagnostic_Usart1(const char *s);

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
void setFan(bool);
void setErrorRele(bool set);
void setReleLine1(bool set);
void setReleLine2(bool set);
void setReleLine3(bool set);
void setReleLine4(bool set);

void setRele24V(bool set);

void setTestLed1(bool set);
void setTestLed2(bool set);
void setTestLed3(bool set);

void init_I2C1();

void digitalPOT_I2C_init(uint8_t pot_Addr);

void backlight();
void noBacklight();

#ifdef __cplusplus
}
#endif
