/**
  ******************************************************************************
  * @file    driver_pinIO.h
  * @author  dmib@@ETS
  * @brief   This file contains pinouts I/O driver module header
  ******************************************************************************
  */
#ifndef __DRIVER_PINIO__
#define __DRIVER_PINIO__

#include "stdint.h"

/** @defgroup driver_pinIO Pinouts I/O driver driver module
  * @brief pinouts I/O driver implementation
  * @{
  */

/** @defgroup driver_pinIO_cfgUI CFGUI constants
  * @brief CFGUI constants
  * @{
  */
#define UI_mode_UI16		0x00
#define UI_mode_UI32		0x01
#define UI_mode_UI48		0x02
#define UI_mode_UI16N		0x03
#define UI_mode_UI32N		0x04
#define UI_mode_NOUI		0x07
/**
  * @}
  */

/** @defgroup driver_pinIO_out_st pinIO out signal states
  * @brief pinIO out signal states
  * @{
  */
#define UPR_STATE_ON		0
#define UPR_STATE_OFF		1
/**
  * @}
  */

/** @defgroup driver_pinIO_in_st pinIO in signal states
  * @brief pinIO in signal states
  * @{
  */
#define DET_STATE_IDLE		0
#define DET_STATE_ON		1
#define DET_STATE_OFF		2
/**
  * @}
  */

/**
  * @brief  pinout I/O driver initialization
  * @retval none
  */
void pinio_init();

/**
  * @brief  pinout I/O driver periodic update
  * @retval none
  */
void pinio_task();

/**
  * @brief  set state of UPR_SP signal
  * @retval none
  */
void pinio_set_UPR_SP(uint8_t state);

/**
  * @brief  set state of UPR_FAN signal
  * @retval none
  */
void pinio_set_UPR_FAN(uint8_t state);  // only SC4 rev1

/**
  * @brief  set state of UPR_RELE signal
  * @retval none
  */
void pinio_set_UPR_RELE(uint8_t state);

/**
  * @brief  get UI config type value
  * @retval result - UI cobfig type value
  */
uint8_t pinio_get_CFGUI();

/**
  * @brief  get DET_48V signal state
  * @retval result - DET_48V signal state
  * @note only SC4 rev1
  */
uint8_t pinio_getstate_DET_48V();	
/**
  * @brief  get DET_48V signal value
  * @retval result - DET_48V signal value
  * @note only SC4 rev1
  */
uint8_t pinio_getval_DET_48V();	

/**
  * @brief  get DET_PHONE signal state
  * @retval result - DET_PHONE signal state
  * @note only SC4 rev1, SC4 rev2 = DET_MIC
  */
uint8_t pinio_getstate_DET_PHONE();
/**
  * @brief  get DET_PHONE signal value
  * @retval result - DET_PHONE signal value
  * @note only SC4 rev1, SC4 rev2 = DET_MIC
  */
uint8_t pinio_getval_DET_PHONE();

/**
  * @brief  get DET_MIC signal state
  * @retval result - DET_MIC signal state
  * @note SC4 rev1 from port, SC4 rev2 from AIC
  */
uint8_t pinio_getstate_DET_MIC();  
/**
  * @brief  get DET_MIC signal value
  * @retval result - DET_MIC signal value
  * @note SC4 rev1 from port, SC4 rev2 from AIC
  */
uint8_t pinio_getval_DET_MIC();  

/**
  * @}
  */

#endif


