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
#ifndef RCC_HSI_72_INIT_H
#define RCC_HSI_72_INIT_H

  //******************************************************************************
  //  Секция include: здесь подключаются заголовочные файлы используемых модулей
  //******************************************************************************
#include <stdint.h>
#include "STM32.h"
#include "Timers.h"

  //******************************************************************************
  //  Секция определения констант
  //******************************************************************************
  //#define MY_CONST1            1
  //#define MY_CONST2            2
  //#define ...

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
  void SystemClock_Config(void);                                                // Конфигурация тактового генератора и SysTick таймера
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
