/**
  ******************************************************************************
  * @file    cbuffer.c
  * @author  dmib@@ETS
  * @brief   This file contains circular buffer implementation
  ******************************************************************************
  */
#include "CBuffer.h"

/**
  * @brief  circular buffer enter critical section macro
  */
#define CS_ENTER		while (buf->lock) ; buf->lock = 1
/**
  * @brief  circular buffer leave critical section macro
  */
#define CS_LEAVE	    buf->lock = 0

/**
  * @brief  circular buffer structure initialization
  * @param  [inout] buf - circular buffer structure
  * @param  [in] bufmem - memory for circular buffer
  * @param  [in] memsize - size in bytes of bufmem
  * @retval none
  */
void CBuffer_Init(CBuffer *buf, uint8_t *bufmem, uint16_t memsize)
{
    buf->buffer = bufmem;
	buf->bufsize = memsize;
	CBuffer_Reset(buf);
}

/**
  * @brief  circular buffer reset
  * @param  [inout] buf - circular buffer structure
  * @retval none
  */
void CBuffer_Reset(CBuffer *buf)
{
  buf->rd_ptr=buf->wr_ptr=buf->datalen=0;
  buf->lock = 0;
}

/**
  * @brief  read byte from circular buffer
  * @param  [in] buf - circular buffer structure
  * @retval result - byte from circular buffer
  */
uint8_t CBuffer_ReadByte(CBuffer *buf){
	uint8_t res;
	if (!buf->datalen) return 0; 
	res=buf->buffer[buf->rd_ptr];
	buf->rd_ptr++;
	if(buf->rd_ptr>=buf->bufsize)	buf->rd_ptr=0;
	CS_ENTER;
	buf->datalen--;	
	CS_LEAVE;
	return res;
}

/**
  * @brief  write bytes to circular buffer
  * @param  [in] buf - circular buffer structure
  * @param  [in] data - array of bytes
  * @param  [in] size - size of data
  * @retval result - 0 - no space for data; 1 - writing ok
  */
uint8_t CBuffer_WriteData(CBuffer *buf, uint8_t *data,uint16_t size)
{  
	uint16_t tmp_len = buf->datalen;
	if ((tmp_len + size) > buf->bufsize) size = buf->bufsize - tmp_len;
	if (!size) return 0;  // no space for data
	tmp_len = 0;
	while (size) {
		buf->buffer[buf->wr_ptr]=*data;
		data++;
		buf->wr_ptr++;
		if(buf->wr_ptr>=buf->bufsize)	buf->wr_ptr=0;
		tmp_len++;
		size--;
	}	
	CS_ENTER;
	buf->datalen=buf->datalen+tmp_len;
	CS_LEAVE;
	return 1;       // ok
}

/**
  * @brief  read bytes from circular buffer
  * @param  [in] buf - circular buffer structure
  * @param  [out] data - array of bytes
  * @param  [in] size - size of data
  * @retval len - actual number of read bytes
  */
uint16_t CBuffer_ReadData(CBuffer *buf, uint8_t *data,uint16_t size)
{
   uint16_t tmp_len=0;
   while (size) {
     if (tmp_len==buf->datalen) break;
	 
	 *data++ = buf->buffer[buf->rd_ptr];
	 buf->rd_ptr++;
	 if(buf->rd_ptr>=buf->bufsize)	buf->rd_ptr=0;
	 
	 tmp_len++;
	 size--;
	}	
	CS_ENTER;
	buf->datalen=buf->datalen-tmp_len;
	CS_LEAVE;
	return tmp_len;
}
