/**
  ******************************************************************************
  * @file    fmware_flasher.h
  * @author  dmib@@ETS
  * @brief   This file contains write firmware to internal flash module header
  ******************************************************************************
  */
 
#ifndef _FIRMWARE_FLASHER_
#define _FIRMWARE_FLASHER_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

/** @defgroup FIRMWARE_FLASHER_module Firmware flasher
  * @brief Firmware flasher module
  * @{
  */

/**
  * @brief  write firmware to flash
  * @param  [in] fmware_ptr - firmware data
  * @param  [in] size - size in bytes of firmware
  * @retval none
  */
__RAM_FUNC void WriteFirmware(uint8_t *fmware_ptr, uint32_t size);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
