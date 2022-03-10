/**
  ******************************************************************************
  * @file    rtp.h
  * @author  dmib@@ETS
  * @brief   This file contains rtp protocol functions module header
  ******************************************************************************
  */

#ifndef rtpH
#define rtpH

#include "stdint.h"
#include "lwip.h"
#include <arch.h>

/** @defgroup RTP_module RTP module
  * @brief RTP protocol functions module
  * @{
  */

/** @defgroup RTP_const RTP header constants
  * @brief RTP header constants
  * @{
  */
#define RTP_VERSION			2
#define RTP_HDR_SIZE		12  // sizeof(rtp_hdr)
#define RTP_PAYLOAD_PCMA	8 	// PCM A-law, 8000Hz
/**
  * @}
  */

/**
  * @brief RTP header structure
  * @note byteorder in fields must be BigEndian (high order first)
  */
//#pragma pack(push,1)
typedef struct {
	/// RTP version number, Padding bit, Extension bit, CSRC count 
	unsigned char  f_info;    
	/// Marker bit, Payload type
	unsigned char  p_info;    
	/// Sequence number        
	unsigned short seq;       
	/// Timestamp              
	unsigned int   ts;        
	/// Synchronization source 
	unsigned int   ssrc;
} rtp_hdr;
//#pragma pack(pop)

/** @defgroup RTP_FUNC RTP header functions
  * @brief RTP header operation functions
  * @{
  */

/**
  * @brief  RTP header structure initialization
  * @param  [inout] hdr - RTP header structure
  * @param  [in] ssrc - audio source identifier
  * @retval none
  */
void rtp_hdr_init(rtp_hdr *hdr, unsigned int ssrc);

/**
  * @brief  RTP header write
  * @param  [inout] buf - buffer to write RTP header
  * @param  [inout] hdr - RTP header structure (will be updated)
  * @param  [in] smp_num - number of samples in packet
  * @param  [in] marker - RTP marker field
  * @retval none
  */
void rtp_hdr_encode(void *buf, rtp_hdr *hdr, int smp_num, u8_t marker);

/**
  * @brief  read and check RTP header
  * @param  [inout] buf - buffer to read RTP header
  * @retval result - 0 - invalid header; else SSRC field value (source identifier)
  */
unsigned int rtp_hdr_decode(void *buf);

/**
  * @}
  */

u32_t SwapUInt32(u32_t d);

/**
  * @}
  */

//---------------------------------------------------------------------------
#endif
