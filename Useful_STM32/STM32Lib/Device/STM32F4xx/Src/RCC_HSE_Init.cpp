//******************************************************************************
//  Секция include: здесь подключается заголовочный файл к модулю
//******************************************************************************
#include  <stdint.h>
#include "RCC_HSE_Init.h"      // Включаем файл заголовка для нашего модуля

//******************************************************************************
//  Секция определения переменных, используемых в модуле
//******************************************************************************
//
//------------------------------------------------------------------------------
// Глобальные
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Локальные
//------------------------------------------------------------------------------
//******************************************************************************
//  Секция прототипов локальных функций
//******************************************************************************

//******************************************************************************
//  Секция описания глобальных функций
//******************************************************************************

/*
 * Настройка внутреннего генератора на масимальную частоту.
 * Запуск идёт от внешнего кварцевого резонатора на 8 МГц
 */
void SystemClock_Config(uint8_t Quartz)
{
  uint8_t
    Div_PLLM,
    Mul_PLLN,
    Div_PLLP,
    Div_PLLQ;

  RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  uint32_t StartUpCounter;

  RCC->CR |= RCC_CR_HSEON;                                                      // Запускаем генератор HSE
  for (StartUpCounter = 0;; StartUpCounter++)                                   // В цикле ждём определённое время ожидая запуска
  {
    if(RCC->CR & RCC_CR_HSERDY) break;                                          // Запуск есть, выходим из цикла
    if(StartUpCounter > 0x1000)
    {
      RCC->CR &= ~RCC_CR_HSEON;                                                 // Отключаем HSE
      return;                                                                   // Запуска нет, выходим из функции
    }
  }

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx) || defined(STM32F417xx)
  Div_PLLP = PLLP_Div_2;
#endif

#if defined(STM32F401xE)
  Div_PLLP = PLLP_Div_4;
#endif

  Mul_PLLN = 168;
  Div_PLLQ = PLLQ_Div_7;

  switch (Quartz)
  {
    case Quartz_4:
      Div_PLLM = 2;
      HSE_Value = 4000000;
      break;
    case Quartz_6:
      Div_PLLM = 3;
      HSE_Value = 6000000;
      break;
    case Quartz_8:
      Div_PLLM = 4;
      HSE_Value = 8000000;
      break;
    case Quartz_10:
      Div_PLLM = 5;
      HSE_Value = 10000000;
      break;
    case Quartz_12:
      Div_PLLM = 6;
      HSE_Value = 12000000;
      break;
    case Quartz_16:
      Div_PLLM = 8;
      HSE_Value = 16000000;
      break;
    case Quartz_20:
      Div_PLLM = 10;
      HSE_Value = 20000000;
      break;
    case Quartz_24:
      Div_PLLM = 12;
      HSE_Value = 24000000;
      break;
    default:
      Div_PLLM = 4;
      HSE_Value = 8000000;
      break;
  }
//#endif

  RCC->PLLCFGR = 0;                                                             // На всякий пожарный обнуляем регистр конфигурации PLL
  RCC->PLLCFGR |= (Div_PLLM << RCC_PLLCFGR_PLLM_Pos);                           // Программируем умножитель M
  RCC->PLLCFGR |= (Div_PLLQ << RCC_PLLCFGR_PLLQ_Pos);                           // Устанавливаем делитель Q для USB
  RCC->PLLCFGR |= (Mul_PLLN << RCC_PLLCFGR_PLLN_Pos);                           // Устанавливаем умножитель N
  RCC->PLLCFGR |= (Div_PLLP << RCC_PLLCFGR_PLLP_Pos);                           // Устанавливаем делитель P
  RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;                                           // Переключаем вход PLL на HSE

  RCC->CR |= RCC_CR_PLLON;                                                      // Включаем PLL

  for (StartUpCounter = 0;; StartUpCounter++)
  {
    if(RCC->CR & RCC_CR_PLLRDY) break;                                          // Запустился, вываливаемся из цикла
    if(StartUpCounter > 0x1000)                                                 // Не запустился гад
    {
      RCC->CR &= ~RCC_CR_HSEON;                                                 // Отключаем HSE
      RCC->CR &= ~RCC_CR_PLLON;                                                 //Останавливаем PLL
      return;
    }
  }

  FLASH->ACR |= (0x05 << FLASH_ACR_LATENCY_Pos);                                // Таймаут FLASH=5, т.к. частота задающего генератора 168 МГц

  //Делители для шин
  RCC->CFGR |= (0x04 << RCC_CFGR_PPRE2_Pos)                                     //Делитель шины APB2 равен 2
             | (0x05 << RCC_CFGR_PPRE1_Pos)                                     //Делитель нишы APB1 равен 4
             | (0x00 << RCC_CFGR_HPRE_Pos);                                     //Делитель AHB отключен (оставляем 0 по умолчанию)

  RCC->CFGR |= (0x02 << RCC_CFGR_SW_Pos);                                       // Переключаемся на работу от PLL

  while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != (0x02 << RCC_CFGR_SWS_Pos))          // Ждём переключения
  {
  }

    SysTickConfig();                                                            // Вызываем настройку системного таймера SysTick
}

//******************************************************************************
//  Секция описания локальных функций
//******************************************************************************
//******************************************************************************
//  ENF OF FILE
//******************************************************************************

