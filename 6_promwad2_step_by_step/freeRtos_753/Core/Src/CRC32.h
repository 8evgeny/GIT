/**
  ******************************************************************************
  * @file    CRC32.h
  * @author  dmib@@ETS
  * @brief   This file contains CRC32 IEEE802.3 calculation module header
  ******************************************************************************
  */
#ifndef __CRC32_IEEE__
#define __CRC32_IEEE__

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup CRC32_module CRC32 calculation module
  * @brief CRC32 sum implementation
  * @{
  */

/**
  * @brief use table CRC32 calculation (uses 1KB RAM for table)
  */
#define USE_TABLE_CRC 0

/**
  * @brief  CRC32 IEEE802.3 sum calculation of array (direct algorithm)
  * @param  [in] crc - initial CRC32 value
  * @param  [in] buf - array of data
  * @param  [in] len - size in bytes of buf
  * @retval CRC32 calculated CRC32 sum of buf
  */
uint32_t crc32NT(uint32_t crc, uint8_t *buf,uint32_t len);

#if USE_TABLE_CRC == 0

/**
  * @brief CRC32 calculation fucntion definition macro
  */
#define crc32_calc	crc32NT

#else 

/**
  * @brief  CRC32 IEEE802.3 table init
  */
void init_crc32_table(void);

/**
  * @brief  CRC32 IEEE802.3 sum calculation of array (table algorithm)
  * @param  [in] crc - initial CRC32 value
  * @param  [in] buf - array of data
  * @param  [in] len - size in bytes of buf
  * @retval calculated CRC32 of buf
  */
uint32_t crc32T(uint32_t crc, uint8_t *buf,uint32_t len);

/**
  * @brief CRC32 calculation fucntion definition macro
  */
#define crc32_calc	crc32T

#endif

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
