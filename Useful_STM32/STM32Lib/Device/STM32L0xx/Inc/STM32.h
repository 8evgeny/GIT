/*
 * File:   main.h
 * Author: User
 *
 * Created on 18 сентября 2019 г., 13:05
 */

// Блокируем повторное включение этого модуля 
#ifndef _STM32_H
#define	_STM32_H

  //****************************************************************************** 
  //  Секция include: здесь подключаются заголовочные файлы используемых модулей 
  //****************************************************************************** 
#include <stdint.h>
#include "stm32l052xx.h"
#include "stm32l0xx.h"
#include "stm32l0xx_gpio.h"
#include "EEPROM.h"
#include "RCC_32_Init.h"
#include <stdbool.h>
#include <stdio.h>
//#include "PeripheralUnit.h"
#include "Timers.h"
#include "gpio_main.h"
#include "ADC.h"



  //****************************************************************************** 
  //  Секция определения констант 
  //****************************************************************************** 
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitToggle(value, bit) ((value) ^= (1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit)) 
//#define PWR_CR_VOS_1  (0x1UL << PWR_CR_VOS_Pos)                       /*!< VOS[1:0] bits (Voltage scaling range selection) 1.8v */
//#define PWR_CR_VOS_2  (0x2UL << PWR_CR_VOS_Pos)                       /*!< VOS[1:0] bits (Voltage scaling range selection) 1.5v */
//#define PWR_CR_VOS_3  PWR_CR_VOS_Pos                                  /*!< VOS[1:0] bits (Voltage scaling range selection) 1.2v */

  //****************************************************************************** 
  //  Секция определения типов 
  //****************************************************************************** 
  //typedef struct  
  //{ 
  //    ... 
  //} T_STRUCT; 

  //typedef ... 

  //****************************************************************************** 
  //  Секция определения глобальных переменных 
  //****************************************************************************** 
  //extern char GlobalVar1; 
  //extern char GlobalVar2; 
  //****************************************************************************** 
  //  Секция прототипов глобальных функций 
  //****************************************************************************** 
  //void global_func1 (void); 
  //void global_func2 (void); 
  //****************************************************************************** 
  //  Секция определения макросов 
  //****************************************************************************** 
  //#define MACRO1    M1
  //#define MACRO2    M2

// Закрывающий #endif к блокировке повторного включения 
#endif	/* MAIN_H */

//****************************************************************************** 
//  ENF OF FILE 
//****************************************************************************** /* 
