/**
  ******************************************************************************
  * @file    tone_generator.h
  * @author  dmib@@ETS
  * @brief   This file contains tone generator module header
  ******************************************************************************
  */
#ifndef ToneGeneratorH
#define ToneGeneratorH
//---------------------------------------------------------------------------
#include "stdint.h"

/** @defgroup ToneGenerator Sine Tone Generator module
  * @brief ToneGenerator implementation
  * @{
  */

/** @defgroup ToneGenerator_DEF frequency number constants
  * @brief frequency number constants
  * @{
  */
// константы для массива freq_coef
#define FREQ_GEN_K425   8
#define FREQ_GEN_K600   9
#define FREQ_GEN_K1000  10
#define FREQ_GEN_K1200  11
#define FREQ_GEN_K2100  12
#define FREQ_GEN_MAX    13
/**
  * @}
  */

/**
  * @brief  DTMF filter desciption structure
  */
typedef struct {
  int16_t D_X0[3];
  int16_t D_X1[3];
  uint16_t D_K1,D_K2;
} DTMF_Gen_State;

extern const int16_t gen_freq_coef[FREQ_GEN_MAX][2];

/** @defgroup ToneGenerator_FUNC sine tone generator functions
  * @brief sine tone generator operation functions
  * @{
  */

/**
  * @brief  initialization of sine tone generation
  * @param  [in] k - sine coefficient
  * @param  [in] amp - start amplitude
  * @param  [inout] X - filter description structure
  * @retval none
  */
void Freq_GenInit(uint32_t k, int16_t amp, int16_t *X);

/**
  * @brief  generate one sine tone sample
  * @param  [in] k - sine coefficient
  * @param  [in] X - filter description structure
  * @retval result - generated sample (int16)
  */
int16_t Freq_GenSample(uint32_t k, int16_t *X);

/**
  * @}
  */

/**
  * @brief  initialization of sine tone generation
  * @param  [inout] st - DTMF filter description structure
  * @param  [in] amp - start amplitude
  * @param  [in] DTMFNum - DTMF number
  * @retval none
  */
void DTMF_GenInit(DTMF_Gen_State *st, uint16_t amp, uint8_t DTMFNum);

/**
  * @brief  generate one DTMF sample
  * @param  [in] st - DTMF filter description structure
  * @retval result - generated sample (int16)
  */
int16_t DTMF_GenSample(DTMF_Gen_State *st);

/**
  * @}
  */

#endif
