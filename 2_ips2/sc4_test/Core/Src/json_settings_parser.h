/**
  ******************************************************************************
  * @file    json_settings_parser.h
  * @author  dmib@@ETS
  * @brief   This file contains json PDO SC4 settings parser module header
  ******************************************************************************
  */
#ifndef _JSON_PARSER_
#define _JSON_PARSER_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "system_settings.h"

/** @defgroup JSON_SET_PARSER JSON PDO SC4 settings parser module
  * @brief JSON PDO SC4 settings parser functions
  * @{
  */

/**
  * @brief  JSON tokens maximum number
  */
#define JSON_TOCKENS_MAX	2048

/** @defgroup JSON_CODE JSON parse result code
  * @brief  JSON parse result code
  * @{
  */
#define JSON_OK			0
#define JSON_ERR_PARSE		-1
#define JSON_ERR_GLOBAL		-2
#define JSON_ERR_AB		-3
#define JSON_ERR_GRP		-4
/**
  * @}
  */

/** @defgroup JSON_FUNC JSON settings parser functions
  * @brief Main JSON parse functions
  * @{
  */

/**
  * @brief  parse all PDO settings
  * @param  [in] json_ptr - string with json
  * @param  [in] json_len - length of JSON (in bytes)
  * @param  [out] settings - PDO setting structure
  * @retval result - JSON parse result code: JSON_OK - settings parsed, else error code
  */
int8_t Parse_JSON(char *json_ptr, uint32_t json_len, sPDOSettings *settings);

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
