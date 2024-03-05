/*
 * File:   RCC_HSI_48.h
 * Author: Анисимов Э.В.
 *
 * Файл содержит функции для инициализации тактового генератора на максимальную частоту 48МГц для МК на базе ядра Cortex-M0
 * Пока он содержит настройку МК STM32F030F4P. По мере надобности, если генератор другого MK серии STM32F0xx будет отличаться,
 * новые МК будут заноситься по мере надобности с помощью условной компиляции.
 *
 */

// Блокируем повторное включение этого модуля
#ifndef RCC_HSE_INIT_H
#define RCC_HSE_INIT_H

  //******************************************************************************
  //  Секция include: здесь подключаются заголовочные файлы используемых модулей
  //******************************************************************************
#include <stdint.h>
#include "STM32.h"

  //******************************************************************************
  //  Секция определения констант
  //******************************************************************************
// Делители HSE
#define HSE_Div_1  0b0000
#define HSE_Div_2  0b0001
#define HSE_Div_3  0b0010
#define HSE_Div_4  0b0011

// Множители PLL
#define PLL_Mul_9  0b0111
#define PLL_Mul_12 0b1010

enum Quartz
{
  Quartz_8  = 8,
  Quartz_12 = 12,
  Quartz_16 = 16,
  Quartz_24 = 24,
  Quartz_32 = 32
};

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
  extern uint32_t HSE_Value;

  //******************************************************************************
  //  Секция прототипов глобальных функций
  //******************************************************************************
  void SystemClock_Config(uint8_t Quartz);                                      // Конфигурация тактового генератора и SysTick таймера
  //******************************************************************************
  //  Секция определения макросов
  //******************************************************************************
  //#define MACRO1    M1
  //#define MACRO2    M2

// Закрывающий #endif к блокировке повторного включения
#endif  /* MAIN_H */

//******************************************************************************
//  ENF OF FILE
//****************************************************************************** /*
