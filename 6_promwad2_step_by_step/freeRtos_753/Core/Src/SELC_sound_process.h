/**
  ******************************************************************************
  * @file    SELC_sound_process.h
  * @author  dmib@@ETS
  * @brief   This file contains sound echo loop cancelation processing module header
  ******************************************************************************
  */
#ifndef __DRIVER_SELC__
#define __DRIVER_SELC__

#include "stdint.h"

/// use sound echo loop canceler
#define USE_SELC

/** @defgroup SELC_soundmodule sound echo loop cancelation module
  * @brief sound echo loop cancelation processing implementation
  * @{
  */

/**
  * @brief  initialization of sound echo loop cancelation algorithm
  * @retval none
  */
void selc_init();

/**
  * @brief  enable/disable SELC algorithm
  * @param  [in] enable - 1=enable, 0=disable
  * @retval none
  */
void selc_set_enable(uint8_t enable);

/**
  * @brief  process sound in and out for SELC algorithm
  * @param  [in/out] srcbuf - input sound samples from MIC
  * @param  [in/out] dstbuf - output sound samples to GR
  * @param  [in] smpnum - number of samples in srcbuf/dstbuf
  * @retval none
  */
void selc_process_sound(int16_t *srcbuf, int16_t *dstbuf, uint16_t smpnum);

/**
  * @brief  set SELC algorithm parameters during tone generation
  * @retval none
  */
void selc_set_tone_gen();

/**
  * @brief  adjust MIC detector threshold level from current MIC volume
  * @param  [in] mic_vol - current MIC volume
  * @retval none
  */
void selc_adjust_threshold_level(int8_t mic_vol);

/**
  * @}
  */

#endif
