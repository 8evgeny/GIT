/**
  ******************************************************************************
  * @file    rtp.c
  * @author  dmib@@ETS
  * @brief   This file contains rtp protocol functions implementation
  ******************************************************************************
  */

#include "rtp.h"

/**
  * @brief Byte swap unsigned int32
  */
u32_t SwapUInt32(u32_t d) {
  u8_t *p=(u8_t *)&d;
  p[0]+=p[3];p[3]=p[0]-p[3];p[0]-=p[3];
  p[1]+=p[2];p[2]=p[1]-p[2];p[1]-=p[2];
  return d;
}

/**
  * @brief Byte swap unsigned int16
  */
u16_t SwapUInt16(u16_t d) {
  u8_t *p=(u8_t *)&d;
  u8_t t;
  t=p[0];p[0]=p[1];p[1]=t;
  return d;
}

/**
  * @brief  RTP header structure initialization
  * @param  [inout] hdr - RTP header structure
  * @param  [in] ssrc - audio source identifier
  * @retval none
  */
void rtp_hdr_init(rtp_hdr *hdr, unsigned int ssrc) {
  // version = 2, padding = 0, extension = 0, CSRC count = 0
  hdr->f_info = (RTP_VERSION << 6)|(0<<5)|(0<<4)|0;
  // marker = 0, ptype = 8 (PCMA)
  hdr->p_info = (0<<7)|RTP_PAYLOAD_PCMA;
  hdr->seq=rand();                     // номер пакета, начальное знач. должно быть случайным
  hdr->ts=(rand() << 16) + rand();    // отсчет времени (количество сэмплов), начальное знач. должно быть случайным
  hdr->ssrc=ssrc;           // индент. источника
}

/**
  * @brief  RTP header write
  * @param  [inout] buf - buffer to write RTP header
  * @param  [inout] hdr - RTP header structure (will be updated)
  * @param  [in] smp_num - number of samples in packet
  * @param  [in] marker - RTP marker field
  * @retval none
  */
void rtp_hdr_encode(void *buf, rtp_hdr *hdr, int smp_num, u8_t marker) {
   rtp_hdr *b_hdr = (rtp_hdr *) buf;

   b_hdr->f_info=hdr->f_info;
   b_hdr->p_info=hdr->p_info;
   if (marker) b_hdr->p_info|=0x80;
   b_hdr->seq=SwapUInt16(hdr->seq);
   b_hdr->ts=SwapUInt16(hdr->ts);
   b_hdr->ssrc=hdr->ssrc;
   // update hdr
   hdr->seq++;
   hdr->ts+=smp_num;
}

/**
  * @brief  read and check RTP header
  * @param  [inout] buf - buffer to read RTP header
  * @retval result - 0 - invalid header; else SSRC field value (source identifier)
  */
unsigned int rtp_hdr_decode(void *buf) {
  rtp_hdr *hdr = (rtp_hdr *) buf;

  // check: version = 2, padding = 0, extension = 0, CSRC count = 0
  if (hdr->f_info!=0x80) return 0;
  // check: ptype = 8 (PCMA)
  if ((hdr->p_info&0x7F)!=RTP_PAYLOAD_PCMA) return 0;

  return SwapUInt32(hdr->ssrc);
}
