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


//Команды  Stm -> Milandr
#define fromStm_BtnImpedance_On     'Q'
#define fromStm_BtnImpedance_Off    'W'
#define fromStm_BtnCalibr_On        'E'
#define fromStm_BtnCalibr_Off       'R'
#define fromStm_BtnReset_On         'T'
#define fromStm_BtnReset_Off        'Y'
#define fromStm_SignalPowerOn_ON    'U'
#define fromStm_SignalPowerOn_OFF   'I'
#define fromStm_SignalImpedanse_ON  'O'
#define fromStm_SignalImpedanse_OFF 'P'
#define fromStm_SignalTranslate_ON  'A'
#define fromStm_SignalTranslate_OFF 'S'
#define fromStm_SignalFromOut_ON    'D'
#define fromStm_SignalFromOut_OFF   'F'
#define fromStm_SignalMic_ON        'G'
#define fromStm_SignalMic_OFF       'H'
#define fromStm_BlankCommand        '&'

//Milandr -> Stm (подтверждение получения команды)
#define toStm_BtnImpedance_On_OK     'q'
#define toStm_BtnImpedance_Off_OK    'w'
#define toStm_BtnCalibr_On_OK        'e'
#define toStm_BtnCalibr_Off_OK       'r'
#define toStm_BtnReset_On_OK         't'
#define toStm_BtnReset_Off_OK        'y'
#define toStm_SignalPowerOn_ON_OK    'u'
#define toStm_SignalPowerOn_OFF_OK   'i'
#define toStm_SignalImpedanse_ON_OK  'o'
#define toStm_SignalImpedanse_OFF_OK 'p'
#define toStm_SignalTranslate_ON_OK  'a'
#define toStm_SignalTranslate_OFF_OK 's'
#define toStm_SignalFromOut_ON_OK    'd'
#define toStm_SignalFromOut_OFF_OK   'f'
#define toStm_SignalMic_ON_OK        'g'
#define toStm_SignalMic_OFF_OK       'h'
#define toStm_SignalMic_OFF_OK       'h'
#define UncnownCmd                   '#'

//Команды Milandr -> Stm
#define cmdToStm_OVERHEAT_60_ON      'j'
#define cmdToStm_OVERHEAT_60_OFF     'k'
#define cmdToStm_OVERHEAT_85_ON      'l'
#define cmdToStm_OVERHEAT_85_OFF     'z'
#define cmdToStm_OVERHEAT_MC_ON      'x'
#define cmdToStm_OVERHEAT_MC_OFF     'c'
#define cmdToStm_BOARD_OK_ON         'v'
#define cmdToStm_BOARD_OK_OFF        'b'
#define cmdToStm_ERROR_MC_ON         'n'
#define cmdToStm_ERROR_MC_OFF        'm'
#define toStm_BlankCommand           '&'

//Stm -> Milandr (подтверждение получения команды)
#define fromStm_OVERHEAT_60_ON_OK        'J'
#define fromStm_OVERHEAT_60_OFF_OK       'K'
#define fromStm_OVERHEAT_85_ON_OK        'L'
#define fromStm_OVERHEAT_85_OFF_OK       'Z'
#define fromStm_OVERHEAT_MC_ON_OK        'X'
#define fromStm_OVERHEAT_MC_OFF_OK       'C'
#define fromStm_BOARD_OK_ON_OK           'V'
#define fromStm_BOARD_OK_OFF_OK          'B'
#define fromStm_ERROR_MC_ON_OK           'N'
#define fromStm_ERROR_MC_OFF_OK          'M'

static bool StmNoReceiveCmd = 0;
static bool UncnownCmdFromStm = 0;


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
static bool OVERHEAT_60 = 0;
static bool OVERHEAT_85 = 0;
static bool OVERHEAT_MC = 0;
static bool BOARD_OK = 0;
static bool ERROR_MC = 0;

static char toStmCmd = 0x00;
static bool toStmCmdSend = true;

static int i_delay; // Глобальная переменная счетчика, которая используется в функции delay()
#define delay(T) for(i_delay = T; i_delay > 0; i_delay--)

static PORT_InitTypeDef PORTB_Init;
static UART_InitTypeDef UART_InitStructure1;

void initUART();
void initGPIO();
char checkReceivedByte(uint8_t data);
void checkGlobalVarables();
void checkPinStates();

uint8_t gpioOVERHEAT_60;
uint8_t gpioBOARD_OK;














//---------------------------------
#endif

#ifdef __cplusplus
}
#endif
