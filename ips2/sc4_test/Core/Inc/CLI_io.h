/**
  ******************************************************************************
  * @file    CLI_io.h
  * @author  dmib@@ETS
  * @brief   This file contains CLI input/output function module header
  ******************************************************************************
  */
#ifndef __CLIIO__
#define __CLIIO__

#include "stdint.h"
#include "stm32h7xx_hal.h"

/** @defgroup CLI_io CLI input/output function module
  * @brief CLI input/output function implementation
  * @{
  */

/** @defgroup CLI_io_lev CLI input/output level
  * @brief CLI input/output level codes
  * @{
  */
#define CLI_PRINTLEVEL_ALWAYS	00
#define CLI_PRINTLEVEL_WORK		10
#define CLI_PRINTLEVEL_TEST		20
#define CLI_PRINTLEVEL_SERVICE	30
#define CLI_PRINTLEVEL_SVCDEBUG	40
#define CLI_PRINTLEVEL_ALLMSG	250
#define CLI_PRINTLEVEL_NONE		255
/**
  * @}
  */

/**
  * @brief  CLI input/output initialization
  * @retval none
  */
void CLI_init();

/**
  * @brief  set CLI print level
  * @param  [in] lev - CLI print level
  * @retval none
  */
void CLI_setPrintLevel(uint8_t lev);

/**
  * @brief  CLI print function
  * @param  [in] str - format string
  * @param  [in] ... - print parameters
  * @retval none
  */
void CLI_print(char *str, ...);

/**
  * @brief  CLI print with level function
  * @param  [in] lev - desired level
  * @param  [in] str - format string
  * @param  [in] ... - print parameters
  * @retval none
  */
void CLI_print_lev(uint8_t lev, char *str, ...);

/**
  * @brief  process CLI input from Ethernet (UDP socket)
  * @param  [in] data - input data
  * @param  [in] len - length of data
  * @param  [in] socket - UDP socket
  * @retval none
  */
void CLI_ethernet_input(uint8_t *data, uint32_t len, void *socket);

/**
  * @brief  process CLI input from UART (RS232)
  * @param  [in] data - input data
  * @param  [in] len - length of data
  * @retval none
  */
void CLI_uart_input(uint8_t *data, uint32_t len);

/**
  * @brief  CLI process periodic task
  * @retval none
  */
void CLI_uart_task();

/**
  * @brief  convert HEX-string to integer value
  * @param  [in] _src - HEX-string
  * @param  [in] _size - size of HEX-string
  * @param  [inout] _ok - 1 - conversion ok; 0 - syntax error
  * @retval result - converted value
  */
uint32_t Hex2Int(char* _src, uint8_t _size, uint8_t* _ok);

/**
  * @brief  convert integer value to HEX-string
  * @param  [in] _src - integer value
  * @param  [in] _size - number of bytes to convert
  * @param  [inout] _dst - output HEX-string
  * @retval none
  */
void Int2Hex(uint32_t _src, uint8_t _size, char* _dst);

/**
  * @brief  convert HEX-string to array of bytes
  * @param  [in] _src - HEX-string
  * @param  [in] _size - size of HEX-string
  * @param  [inout] _dst - output array of bytes
  * @retval result - 1 - conversion ok; 0 - syntax error
  */
uint8_t Hex2Array(char* _src, uint16_t _size, uint8_t* _dst);

/**
  * @brief  convert array of bytes to HEX-string
  * @param  [in] _src - array of bytes
  * @param  [in] _size - number of bytes in array to convert
  * @param  [inout] _dst - output HEX-string
  * @retval none
  */
void Array2Hex(uint8_t* _src, uint16_t _size, char* _dst);

/**
  * @}
  */

#endif
