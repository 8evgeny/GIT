/**
  ******************************************************************************
  * @file    pckt_buffer.h
  * @author  dmib@@ETS
  * @brief   This file contains packet FIFO buffer module header
  ******************************************************************************
  */
#ifndef _PktBuffer_H_
#define _PktBuffer_H_

#include "stdint.h"

/** @defgroup PBuffer Packet buffer module
  * @brief Packet buffer implementation
  * @{
  */

/**
  * @brief  packet buffer structure
  */
typedef struct 
{
	uint16_t rd_ptr;
	uint16_t wr_ptr;
	uint8_t *buffer;
	uint16_t bufsize;
	uint16_t datalen;	
	uint16_t pktnum;
	uint16_t lock;
} PktBuffer;

/** @defgroup PBuffer_FUNC PBuffer functions
  * @brief PBuffer operation functions
  * @{
  */

/**
  * @brief  packet buffer structure initialization
  * @param  [inout] buf - packet buffer structure
  * @param  [in] bufmem - memory for packet buffer
  * @param  [in] memsize - size in bytes of bufmem
  * @retval none
  */  
void PktBuffer_Init(PktBuffer *buf, uint8_t *bufmem, uint16_t memsize);

/**
  * @brief  packet buffer reset
  * @param  [inout] buf - circular buffer structure
  * @retval none
  */
void PktBuffer_Reset(PktBuffer *buf);

/**
  * @brief  write packet to packet buffer
  * @param  [in] buf - packet buffer structure
  * @param  [in] pktdata - packet data
  * @param  [in] pktsize - size of packet data
  * @retval result - 0 - no space for packet; 1 - writing ok
  */
uint8_t PktBuffer_Write(PktBuffer *buf, uint8_t *pktdata,uint16_t pktsize);

/**
  * @brief  read packet from packet buffer
  * @param  [in] buf - packet buffer structure
  * @param  [out] pktdata - packet data
  * @retval result - 0 - no packets in buf; 1 - reading ok
  */
uint8_t PktBuffer_Read(PktBuffer *buf, uint8_t *pktdata);

/**
  * @brief  read next packet size from packet buffer
  * @param  [in] buf - packet buffer structure
  * @param  [out] pktsize - size of next packet in buf
  * @retval result - 0 - no packets in buf; 1 - pktsize is valid
  */
uint8_t PktBuffer_GetNextPktSize(PktBuffer *buf, uint16_t *pktsize);

/**
  * @}
  */

/**
  * @}
  */

#endif
