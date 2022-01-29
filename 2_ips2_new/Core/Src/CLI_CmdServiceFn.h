/**
  ******************************************************************************
  * @file    CLI_CmdServiceFn.h
  * @author  dmib@@ETS
  * @brief   This file contains CLI service functions module header
  ******************************************************************************
  */
#ifndef __CLISVCFN__
#define __CLISVCFN__

/** @defgroup CmdServiceFn CLI service functions module
  * @brief CLI service functions implementation
  * @{
  */

/**
  * @brief service mode types
  */
typedef enum
{
    svcNone = 0,
    svcFirmware,     
    svcConfig,   
} servicemode_t;

/// Firmware read address
#define FIRMWARE_GET_ADDR	0x08000000
/// Firmware save address
#define FIRMWARE_SET_ADDR	0x60000000
/// Firmware max size
#define FIRMWARE_MAX_SIZE	0x100000

/**
  * @brief  CLI service module initialization
  * @retval none
  */
void service_init();

/**
  * @brief  get current service mode
  * @retval result - current mode
  */
servicemode_t service_getmode();

/**
  * @brief  process configuration mode on command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnConfigOn(int numParam, char** params);

/**
  * @brief  process configuration start write command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnConfigStartWrite(int numParam, char** params);

/**
  * @brief  process configuration mode off command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnConfigOFF(int numParam, char** params);

/**
  * @brief  process firmware write mode on command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnFmwareOn(int numParam, char** params);

/**
  * @brief  process firmware start write command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnFmwareStartWrite(int numParam, char** params);

/**
  * @brief  process firmware write mode off command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnFmwareOFF(int numParam, char** params);

/**
  * @brief  process write data block command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnSetBlock(int numParam, char** params);

/**
  * @brief  process get data block command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnGetBlock(int numParam, char** params);

/**
  * @}
  */

#endif
