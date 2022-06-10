/**
  ******************************************************************************
  * @file    pckt_buffer.c
  * @author  dmib@@ETS
  * @brief   This file contains packet FIFO buffer implementation
  ******************************************************************************
  */
#include "pckt_buffer.h"

/**
  * @brief  packet buffer enter critical section macro
  */
#define CS_ENTER		while (buf->lock) ; buf->lock = 1
/**
  * @brief  packet buffer leave critical section macro
  */
#define CS_LEAVE	    buf->lock = 0
/**
  * @brief  packet buffer increment read index macro
  */
#define PKTBUFFER_INC_RDPTR buf->rd_ptr++;if(buf->rd_ptr>=buf->bufsize) buf->rd_ptr=0
/**
  * @brief  packet buffer increment write index macro
  */
#define PKTBUFFER_INC_WRPTR buf->wr_ptr++;if(buf->wr_ptr>=buf->bufsize) buf->wr_ptr=0

/**
  * @brief  packet buffer structure initialization
  * @param  [inout] buf - packet buffer structure
  * @param  [in] bufmem - memory for packet buffer
  * @param  [in] memsize - size in bytes of bufmem
  * @retval none
  */
void PktBuffer_Init(PktBuffer *buf, uint8_t *bufmem, uint16_t memsize)
{
    buf->buffer = bufmem;
	buf->bufsize = memsize;
	PktBuffer_Reset(buf);
}

/**
  * @brief  packet buffer reset
  * @param  [inout] buf - circular buffer structure
  * @retval none
  */
void PktBuffer_Reset(PktBuffer *buf)
{
  buf->rd_ptr = 0;
  buf->wr_ptr = 0;
  buf->datalen = 0;
  buf->pktnum = 0;
  buf->lock = 0;
}

/**
  * @brief  write packet to packet buffer
  * @param  [in] buf - packet buffer structure
  * @param  [in] pktdata - packet data
  * @param  [in] pktsize - size of packet data
  * @retval result - 0 - no space for packet; 1 - writing ok
  */
uint8_t PktBuffer_Write(PktBuffer *buf, uint8_t *pktdata,uint16_t pktsize)
{  
	uint16_t tmp_len;
	if ((buf->datalen + pktsize + 2) > buf->bufsize) return 0;  // no space for packet
	buf->buffer[buf->wr_ptr]=pktsize&0xFF;
	PKTBUFFER_INC_WRPTR;
	buf->buffer[buf->wr_ptr]=pktsize>>8;
	PKTBUFFER_INC_WRPTR;
	tmp_len = 0;
	while (pktsize) {
		buf->buffer[buf->wr_ptr]=*pktdata;
		pktdata++;
		PKTBUFFER_INC_WRPTR;
		tmp_len++;
		pktsize--;
	}	
	CS_ENTER;	
	buf->pktnum++;
	buf->datalen=buf->datalen+tmp_len+2;
	CS_LEAVE;
	return 1;       // ok
}

/**
  * @brief  read packet from packet buffer
  * @param  [in] buf - packet buffer structure
  * @param  [out] pktdata - packet data
  * @retval result - 0 - no packets in buf; 1 - reading ok
  */
uint8_t PktBuffer_Read(PktBuffer *buf, uint8_t *pktdata)
{
	uint16_t size,tmp_len;
	if (!buf->pktnum) return 0;	// no packets
	size = buf->buffer[buf->rd_ptr];
	PKTBUFFER_INC_RDPTR;
	size |= buf->buffer[buf->rd_ptr] << 8;
	PKTBUFFER_INC_RDPTR;
	tmp_len = 0;
	while (size) {
	 *pktdata++ = buf->buffer[buf->rd_ptr];
	 PKTBUFFER_INC_RDPTR;
	 tmp_len++;
	 size--;
	}	
	CS_ENTER;
	buf->datalen=buf->datalen-tmp_len-2;
	buf->pktnum--;
	CS_LEAVE;
	return 1;       // ok
}

/**
  * @brief  read next packet size from packet buffer
  * @param  [in] buf - packet buffer structure
  * @param  [out] pktsize - size of next packet in buf
  * @retval result - 0 - no packets in buf; 1 - pktsize is valid
  */
uint8_t PktBuffer_GetNextPktSize(PktBuffer *buf, uint16_t *pktsize)
{
	int16_t tmp_size;
	if (!buf->pktnum) return 0;	// no packets
		tmp_size = buf->buffer[buf->rd_ptr];
	if ((buf->rd_ptr+1)>=buf->bufsize)
		tmp_size |= buf->buffer[0] << 8;
	else
		tmp_size |= buf->buffer[buf->rd_ptr+1] << 8;
	*pktsize = tmp_size;
	return 1;       // ok
}
