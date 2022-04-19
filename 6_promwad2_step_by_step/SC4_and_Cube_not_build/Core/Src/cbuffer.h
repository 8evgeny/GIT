/**
  ******************************************************************************
  * @file    cbuffer.h
  * @author  dmib@@ETS
  * @brief   This file contains circular buffer module header
  ******************************************************************************
  */
#ifndef _CBuffer_H_
#define _CBuffer_H_

#include "stdint.h"

/** @defgroup CBuffer Circular buffer module
  * @brief Circular buffer implementation
  * @{
  */

/**
  * @brief  circular buffer structure
  */
typedef struct 
{
	uint16_t rd_ptr;
	uint16_t wr_ptr;
	uint8_t *buffer;
	uint16_t bufsize;
	uint16_t datalen;
	uint16_t lock;
} CBuffer;

/** @defgroup CBuffer_FUNC CBuffer functions
  * @brief CBuffer operation functions
  * @{
  */
  
/**
  * @brief  circular buffer structure initialization
  * @param  [inout] buf - circular buffer structure
  * @param  [in] bufmem - memory for circular buffer
  * @param  [in] memsize - size in bytes of bufmem
  * @retval none
  */
void CBuffer_Init(CBuffer *buf, uint8_t *bufmem, uint16_t memsize);

/**
  * @brief  circular buffer reset
  * @param  [inout] buf - circular buffer structure
  * @retval none
  */
void CBuffer_Reset(CBuffer *buf);

/**
  * @brief  read byte from circular buffer
  * @param  [in] buf - circular buffer structure
  * @retval result - byte from circular buffer
  */
uint8_t CBuffer_ReadByte(CBuffer *buf);

/**
  * @brief  write bytes to circular buffer
  * @param  [in] buf - circular buffer structure
  * @param  [in] data - array of bytes
  * @param  [in] size - size of data
  * @retval result - 0 - no space for data; 1 - writing ok
  */
uint8_t CBuffer_WriteData(CBuffer *buf, uint8_t *data,uint16_t size);

/**
  * @brief  read bytes from circular buffer
  * @param  [in] buf - circular buffer structure
  * @param  [out] data - array of bytes
  * @param  [in] size - size of data
  * @retval len - actual number of read bytes
  */
uint16_t CBuffer_ReadData(CBuffer *buf, uint8_t *data,uint16_t size);

/**
  * @}
  */

/**
  * @}
  */

#endif // _CBuffer_H_
