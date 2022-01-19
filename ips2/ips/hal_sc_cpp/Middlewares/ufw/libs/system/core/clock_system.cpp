/*
 * clock_system.cpp
 *
 *  Created on: 18 дек. 2020 г.
 *      Author: vader
 */
#ifndef PLATFORM_TEST

#include "main.h"
#include <system/core/clock_system.h>
#include <lib_asserts.h>

namespace ufw
{
  
  ClockSystem& ClockSystem::Instance ()
  {
    static ClockSystem s_clock_system;
    return s_clock_system;
  }
  
  void ClockSystem::Init ()
  {
    RCC_OscInitTypeDef RCC_OscInitStruct =
      { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct =
      { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct =
      { 0 };

    /** Supply configuration update enable
     */
    HAL_PWREx_ConfigSupply (PWR_LDO_SUPPLY);
    /** Configure the main internal regulator output voltage
     */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
      {
      }
    /** Macro to configure the PLL clock source
     */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 5;
    RCC_OscInitStruct.PLL.PLLN = 192;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 10;
    RCC_OscInitStruct.PLL.PLLR = 2;  //60;//2;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK)
      {
	LIB_CPUERROR_HANDLE;
      }
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
        | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1 | RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
      {
	LIB_CPUERROR_HANDLE;
      }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_UART7
        | RCC_PERIPHCLK_RNG | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_I2C1
        | RCC_PERIPHCLK_FMC;
    PeriphClkInitStruct.PLL3.PLL3M = 2;
    PeriphClkInitStruct.PLL3.PLL3N = 32;
    PeriphClkInitStruct.PLL3.PLL3P = 49;
    PeriphClkInitStruct.PLL3.PLL3Q = 2;
    PeriphClkInitStruct.PLL3.PLL3R = 2;
    PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_3;
    PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
    PeriphClkInitStruct.PLL3.PLL3FRACN = 923;
    PeriphClkInitStruct.FmcClockSelection = RCC_FMCCLKSOURCE_PLL;
    PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL3;
    PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
    PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
    PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_HSI;			//D2PCLK1;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct) != HAL_OK)
      {
	LIB_CPUERROR_HANDLE;
      }
    HAL_RCC_MCOConfig (RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
  }
  
  ClockSystem::ClockSystem ()
  {
    Init();
  }
  
  ClockSystem::~ClockSystem ()
  {
  }

} /* namespace ufw */

#else

#endif
