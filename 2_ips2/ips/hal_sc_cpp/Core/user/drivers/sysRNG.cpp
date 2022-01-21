/*
 * sysRNG.cpp - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 *  Created on: 24 сент. 2020 г.
 *      Author: Dmitry Donskih
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 * 
 * Attention Software modification allowed only with 
 * the permission of the author.
 */
#include <sysRNG.hpp>
#include <cstring>

#include "main.h"
#include "stm32h7xx_hal_rng.h"

extern RNG_HandleTypeDef hrng;

#define RNGWORD_SIZE		sizeof(uint32_t)
#define RNGWORD_ALIGN(mem)	(((uint32_t)mem&(~(uint32_t)0x03)))
#define RNGWORD_OFFSET(mem)	(((uint32_t)mem&(uint32_t)0x03))

sysRNG::sysRNG ()
{
}

sysRNG::~sysRNG ()
{
}

sysRNG& sysRNG::getInstance ()
{
  static sysRNG instance;
  return instance;
}

sysRNG* sysRNG::getInstancePtr ()
{
  return this;
}

void sysRNG::number (uint32_t *num_ptr)
{
  while(HAL_RNG_GetState(&hrng)!=HAL_RNG_STATE_READY);
  HAL_RNG_GenerateRandomNumber (&hrng, num_ptr);
}

int sysRNG::number ()
{
  int temp;
  number((uint32_t*)&temp);
  return temp;
}


void sysRNG::numbers (void *data_ptr, size_t size)
{
  size_t i = 0;
  for (i = 0; i < RNGWORD_ALIGN(size); i += RNGWORD_SIZE)
    {
      number ((uint32_t*) ((uint32_t)data_ptr + i));
    }
  if (RNGWORD_OFFSET(size) != 0)
    {
      uint32_t temp;
      number (&temp);
      memcpy ((data_ptr + i * RNGWORD_SIZE), &temp, RNGWORD_OFFSET(size));
    }

}

#ifdef TESTS

#include <cli_apps.hpp>
#include <perf.hpp>

void test_rng (void *args)
{
  perf_t meter;
  sysRNG& cm7trng=sysRNG::getInstance();
  ttY *term=reinterpret_cast<app_param_t*>(args)->terminal;
  // @formatter:off
  term->printfl("Check i32 generator: %d, %d, %d\n",cm7trng.number(),cm7trng.number(),cm7trng.number());
  term->printfl("Check u16 generator: %d, %d, %d\n",cm7trng.get<uint16_t>(),cm7trng.get<uint16_t>(),cm7trng.get<uint16_t>());
  term->printfl("Check char generator: %d, %d, %d\n",cm7trng.get<char>(),cm7trng.get<char>(),cm7trng.get<char>());
  term->printfl("Check float32 generator: %f, %f, %f\n",cm7trng.get<float>(),cm7trng.get<float>(),cm7trng.get<float>());
  // @formatter:on

  uint8_t *testarray=(uint8_t*)pvPortMalloc(2*1024);
  for(int i=8;i<512;i++)
    {
      PERFCHECK_START;
      cm7trng.numbers(testarray, i);
      PERFCHECK_STOP(meter);
      term->printfl("%d units generated at %d clock cycles (%f ms %d us)\n",i,meter.tick,meter.us_f,meter.us);
  }

  PERFCHECK_START;
  cm7trng.numbers(testarray, 2*1024-1);
  PERFCHECK_STOP(meter);
  term->printfl("%d units generated at %d clock cycles (%f ms/%d us\n",2*1024-1,meter.tick,meter.us_f,meter.us);

  vPortFree(testarray);
}

#endif
