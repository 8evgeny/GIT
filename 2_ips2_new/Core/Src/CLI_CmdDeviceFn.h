/**
  ******************************************************************************
  * @file    CLI_CmdDeviceFn.h
  * @author  dmib@@ETS
  * @brief   This file contains CLI device common functions module header
  ******************************************************************************
  */

#ifndef __CLIDEVFN__
#define __CLIDEVFN__

/** @defgroup CmdDeviceFn CLI device common functions module
  * @brief CLI device common functions implementation
  * @{
  */

/**
  * @brief  process get device type command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnGetType(int numParam, char** params);

/**
  * @brief  process reset device command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnResetDevice(int numParam, char** params);

/**
  * @brief  process connect to device command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnConnect(int numParam, char** params);

/**
  * @brief  process print help command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnHelp(int numParam, char** params);

/**
  * @brief  print PDO device type
  * @retval none
  */
void printDeviceID();

/**
  * @brief  check and print firmware information
  * @retval none
  */
void checkFirmware();

/**
  * @brief  process print firmware info command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnGetFirmwareInfo(int numParam, char** params);

/**
  * @brief  process set firmware info command
  * @param  [in] numParam - number of command parameters
  * @param  [in] params - array of pointers to string parameters
  * @retval none
  */
void fnSetFirmwareInfo(int numParam, char** params);

/**
  * @}
  */

void fnVolumeUp(int numParam, char** params);
void fnVolumeDown(int numParam, char** params);
void fnSensUp(int numParam, char** params);
void fnSensDown(int numParam, char** params);

#endif
