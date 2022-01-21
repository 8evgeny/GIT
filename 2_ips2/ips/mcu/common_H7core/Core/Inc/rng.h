/**
  ******************************************************************************
  * File Name          : RNG.h
  * Description        : This file provides code for the configuration
  *                      of the RNG instances.
  ******************************************************************************
  * @attention
  *
  *<h2><center>A part of GITComm IPS Project for Group of Industrial
  * Technologies LLC.</center></h2>
  * <h2><center>&copy; Copyright (c) 2020 Donskih Dmitriy.
  * All rights reserved.</center></h2>
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __rng_H
#define __rng_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RNG_HandleTypeDef hrng;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_RNG_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ rng_H */

/**
  * @}
  */

/**
  * @}
  */
