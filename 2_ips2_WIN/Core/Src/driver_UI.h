/**
  ******************************************************************************
  * @file    driver_ui.h
  * @author  dmib@@ETS
  * @brief   This file contains UI driver module header
  ******************************************************************************
  */
#ifndef __DRIVER_UI__
#define __DRIVER_UI__

#include "stdint.h"

/** @defgroup driver_ui UI driver module
  * @brief UI driver implementation
  * @{
  */

/** @defgroup driver_ui_LEDst LED state types
  * @brief LED state types
  * @{
  */
#define LED_STATE_OFF	0
#define LED_STATE_ON	1
#define LED_STATE_FL1	2		// slow blink
#define LED_STATE_FL2	3		// fast blink
#define LED_STATE_FL3	4		// impulse blink
///@}

/** @defgroup driver_ui_LEDtime LED flash time
  * @brief LED flash time
  * @{
  */
#define LED_PERIOD_FL1		500
#define LED_PERIOD_FL2		250
#define LED_PERIOD_FL3_0	900
#define LED_PERIOD_FL3_1	100
///@}

/** @defgroup driver_ui_LEDidx LED indexes
  * @brief LED indexes
  * @{
  */
#define LED_TEST	0
#define LED_NORMA	1
#define LED_MKVKL	2
#define LED_AB1R	8
#define LED_AB1G	9
#define LED_AB48R	(LED_AB1R + (48*2))
#define LED_AB48G	(LED_AB1G + (48*2))
#define LED_NUM     (LED_AB48G + 1)
///@}

/** @defgroup driver_ui_bval Button values
  * @brief Button values
  * @{
  */
#define BTN_VAL_UP			0
#define BTN_VAL_DOWN		1
///@}

/** @defgroup driver_ui_bst Button states
  * @brief Button states
  * @{
  */
#define BTN_STATE_IDLE		0
#define BTN_STATE_TOGGLE	1
///@}

/** @defgroup driver_ui_bidx Button indexes
  * @brief Button indexes
  * @{
  */
#define BTN_TEST	0
#define BTN_VOLUP	1
#define BTN_VOLDN	2
#define BTN_MICUP	3
#define BTN_MICDN	4
#define BTN_AB1		8
#define BTN_AB48	(BTN_AB1 + 48)
#define BTN_NN1		(BTN_AB48 + 1)
#define BTN_NN9		(BTN_NN1 + 9)
#define BTN_NNST	(BTN_NN9 + 1)
#define BTN_NN0		(BTN_NN9 + 2)
#define BTN_NNSH	(BTN_NN9 + 3)
#define BTN_NUM	    (BTN_NNSH + 1)
#define BTN_NONE	0xFF
///@}

/**
  * @brief  get current button value
  * @param  [in] btn - button index
  * @retval result - button value
  */
uint8_t ui_getbtnval(uint8_t btn);

/**
  * @brief  get button state
  * @param  [in] btn - button index
  * @retval result - button state
  */
uint8_t ui_getbtnstate(uint8_t btn);

/**
  * @brief  get first button with active state
  * @retval result - button index (BTN_NONE - no active button)
  * @note also reset state of returned button
  */
uint8_t ui_getactivebtn();

/**
  * @brief  reset button state
  * @param  [in] btn - button index
  * @retval none
  */
void ui_resetbtnstate(uint8_t btn);

/**
  * @brief  set LED state
  * @param  [in] led - LED index
  * @param  [in] state - LED state
  * @retval none
  */
void ui_setledstate(uint8_t led, uint8_t state);

/**
  * @brief  get LED state
  * @param  [in] led - LED index
  * @retval result - LED state
  */
uint8_t ui_getledstate(uint8_t led);

/**
  * @brief  UI board driver initialization
  * @retval none
  */
void ui_init();

/**
  * @brief  UI board driver periodic task
  * @retval none
  */
void ui_task();

/**
  * @brief  get UI board mode
  * @retval result - code of UI board mode
  */
uint8_t ui_get_uimode();

/**
  * @brief  set block keyboard
  * @param  [in] isblock - 1- block; 0 - unblock
  * @retval none
  */
void ui_set_block_kbd(uint8_t isblock);

/**
  * @}
  */

#endif
