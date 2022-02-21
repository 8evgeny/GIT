/**
  ******************************************************************************
  * @file    g711.h
  * @author  dmib@@ETS
  * @brief   This file contains G.711 codec module header
  ******************************************************************************
  */

#ifndef g711_H
#define g711_H

/** @defgroup G711_module G711 codec module
  * @brief G711 codec implementation
  * @{
  */

/**
  * @brief  PCM i16 to A-Law encoding
  * @param  [in] src - array of PCM i16 samples
  * @param  [out] dst - array of G711 data
  * @param  [in] nsmpl - number of samples in src
  * @retval none
  */
void g711_pcm2alaw(short *src,unsigned char *dst,unsigned int nsmpl);

/**
  * @brief  A-Law to PCM i16 decoding
  * @param  [in] src - array of G711 data
  * @param  [out] dst - array of PCM i16 samples
  * @param  [in] nsmpl - number of samples in src
  * @retval none
  */
void g711_alaw2pcm(unsigned char *src,short *dst,unsigned int nsmpl);

/**
  * @brief  PCM i16 to u-Law encoding
  * @param  [in] src - array of PCM i16 samples
  * @param  [out] dst - array of G711 data
  * @param  [in] nsmpl - number of samples in src
  * @retval none
  */
void g711_pcm2ulaw(short *src,unsigned char *dst,unsigned int nsmpl);

/**
  * @brief  u-Law to PCM i16 decoding
  * @param  [in] src - array of G711 data
  * @param  [out] dst - array of PCM i16 samples
  * @param  [in] nsmpl - number of samples in src
  * @retval none
  */
void g711_ulaw2pcm(unsigned char *src,short *dst,unsigned int nsmpl);

extern const unsigned char g711_l2A[];
extern const short g711_A2l[];

/**
  * @brief  G711 A-Law to i12 conversion macro
  */
#define g711_alaw8_2_pcm12(a) (((unsigned short)(g711_A2l[a]+0x7FFF))>>4)

/*
unsigned char g711_pcm12_2_alaw8(unsigned short a)
{
   unsigned char mask;

   a-=0x7FF;
   mask = (a < 0) ? 0x7f : 0xff;
   if (a < 0) a = -a;
   return g711_l2A[a] & mask;
}
*/

/**
  * @}
  */


#endif


