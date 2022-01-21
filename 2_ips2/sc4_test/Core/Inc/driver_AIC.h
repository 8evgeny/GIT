/**
  ******************************************************************************
  * @file    driver_AIC.h
  * @author  dmib@@ETS
  * @brief   This file contains AIC driver module header
  ******************************************************************************
  */
#ifndef __DRIVER_AIC__
#define __DRIVER_AIC__

#include "stdint.h"

/** @defgroup driver_AIC AIC driver module
  * @brief AIC driver implementation
  * @{
  */

/// AIC I2C address
#define TLV320AIC3254_Address	0x30

/// SAI DMA buffer size
#define SAI_DMA_BUFFER_SIZE 160

//---hardware control function------------------------------------------------------
/**
  * @brief  read AIC current page register value
  * @param  [in] reg - AIC register number
  * @param  [inout] data - pointer to store register value
  * @retval result - HAL result code
  */
uint8_t TLV320_Read(uint8_t reg, uint8_t *data);

/**
  * @brief  read AIC register value with page change
  * @param  [in] page - AIC register page number
  * @param  [in] reg - AIC register number
  * @param  [inout] data - pointer to store register value
  * @retval result - HAL result code
  */
uint8_t TLV320_ReadPage(uint8_t page, uint8_t reg, uint8_t *data);

/**
  * @brief  write AIC current page register value
  * @param  [in] reg - AIC register number
  * @param  [in] data - register value
  * @retval result - HAL result code
  */
uint8_t TLV320_Write(uint8_t reg, uint8_t data);

/**
  * @brief  write AIC register value with page change
  * @param  [in] page - AIC register page number
  * @param  [in] reg - AIC register number
  * @param  [in] data - register value
  * @retval result - HAL result code
  */
uint8_t TLV320_WritePage(uint8_t page, uint8_t reg, uint8_t data);

/**
  * @brief  sequental write AIC registers values
  * @param  [in] data - array of AIC register/value pairs
  * @param  [in] len - number of pairs to write
  * @retval result - HAL result code
  */
uint8_t TLV320_WriteSq(uint8_t *data, uint16_t len);

/**
  * @brief  reset AIC
  * @retval result - HAL result code
  */
uint8_t TLV320_Reset();

//---hardware control function end---------------------------------------------------

/** @defgroup driver_EEPROM_vol Volume control parameters
  * @brief Volume control parameters
  * @{
  */

/// minimum OUT sound volume level
#define VOL_OUT_MIN		-42		// -21 db
/// maximum OUT sound volume level
#define VOL_OUT_MAX		24		// +3 db		//	+12 db		as GIT say!
/// minimum MIC sensivity level
#define VOL_MIC_MIN		-24		// -9 db		//	-12 db		as GIT say!
/// maximum MIC sensivity level
#define VOL_MIC_MAX		24		// +6 db		//	+12 db		as GIT say!

/// sound volume change step
#define VOL_OUT_STEP	6		// 3 db
/// sensivity change step
#define VOL_MIC_STEP	3		// 1.5 db

/**
  * @}
  */

/// out device GR code
#define AIC_OUTDEV_GR		0
/// out device PHONE step
#define AIC_OUTDEV_PHONE	1

/// in device INTMIC code
#define AIC_INDEV_INTMIC	0
/// in device EXTMIC code
#define AIC_INDEV_EXTMIC	1

/**
  * @brief  read system block from EEPROM
  * @param  [in] volume - AIC DAC sound volume
  * @retval result - HAL result code
  */
uint8_t aic_setDACOutVolume (int8_t volume);

/**
  * @brief  read system block from EEPROM
  * @param  [in] volume - AIC ADC sound volume
  * @retval result - HAL result code
  */
uint8_t aic_setADCInVolume (int8_t volume);

/**
  * @brief  mute/unmute AIC DAC
  * @param  [in] ismuted - 0 - unmuted; 1 - muted
  * @retval result - HAL result code
  */
uint8_t aic_setDACMute (uint8_t ismuted);

/**
  * @brief  mute/unmute AIC ADC
  * @param  [in] ismuted - 0 - unmuted; 1 - muted
  * @retval result - HAL result code
  */
uint8_t aic_setADCMute (uint8_t ismuted);

/**
  * @brief  read AIC ADC flags
  * @param  [inout] flags - variable to receive flags
  * @retval result - HAL result code
  */
uint8_t aic_getADCFlags (uint8_t *flags);

/**
  * @brief  read AIC DAC flags
  * @param  [inout] flags - variable to receive flags
  * @retval result - HAL result code
  */
uint8_t aic_getDACFlags (uint8_t *flags);

/**
  * @brief  enable/disable software audioloop
  * @param  [in] enable - 0 - disable audioloop; 1 - enable audioloop
  * @retval none
  */
void aic_setSAIloop(uint8_t enable);

/**
  * @brief  AIC driver initialization
  * @retval result - 0 - operation done (<0 - hardware error)
  */
int8_t aic_init();

/**
  * @brief  AIC driver deinitialization
  * @retval none
  */
void aic_deinit();

/**
  * @brief  AIC driver periodic task
  * @retval none
  */
void aic_task();

/**
  * @brief  set audio output device
  * @param  [in] dev - output device code
  * @retval none
  */
void aic_setOutDev(uint8_t dev);

/**
  * @brief  set audio input device
  * @param  [in] dev - input device code
  * @retval none
  */
void aic_setInDev(uint8_t dev);

/**
  * @brief  step up audio input sound volume
  * @retval none
  */
void aic_setInVolUp();

/**
  * @brief  step down audio input sound volume
  * @retval none
  */
void aic_setInVolDown();

/**
  * @brief  step up audio output sound volume
  * @retval none
  */
void aic_setOutVolUp();

/**
  * @brief  step down audio output sound volume
  * @retval none
  */
void aic_setOutVolDown();

/**
  * @}
  */

#endif
