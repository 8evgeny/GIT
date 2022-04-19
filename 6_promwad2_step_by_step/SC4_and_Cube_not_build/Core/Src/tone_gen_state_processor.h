/**
  ******************************************************************************
  * @file    tone_gen_state_processor.h
  * @author  dmib@@ETS
  * @brief   This file contains tone generator state processor module header
  ******************************************************************************
  */
#ifndef ToneGenStateH
#define ToneGenStateH
//---------------------------------------------------------------------------
#include "stdint.h"

/** @defgroup ToneGenState Tone signals state processor module
  * @brief Tone signals state processor  implementation
  * @{
  */

/** @defgroup ToneGenState_CONST Tone signals type constants
  * @brief Tone signals type constants
  * @{
  */
#define SIG_GEN_OFF      0   // выключить сигнал
#define SIG_GEN_TCH600   1 	 // вызов абонента                    600Гц  (maxlen)
#define SIG_GEN_TCH1000  2 	 // сишнал 1000 Гц                    1000 Гц (maxlen)
#define SIG_GEN_TCH2100  3 	 // вызов абонента                    2100Гц (maxlen)
#define SIG_GEN_CALL     4 	 // "вызов абонента"                  425Гц (maxlen)
#define SIG_GEN_STATION  5 	 // "ответ станции"                   425Гц непрерывный (maxlen)
#define SIG_GEN_SENDCTRL 6 	 // "контроль посылки вызова"         425Гц 1c+4c
#define SIG_GEN_BUSY     7 	 // "абонент занят"                   425Гц 0,3c+0,3c
#define SIG_GEN_UNAVAIL  8   // "абонент недоступен"              950Гц 0,33c + 1400Гц 0,33c + 1800Гц 0,33c
#define SIG_GEN_OPVACK   9   // "запрос подтверждение оповещения" 425Гц 0,2c+5c
#define SIG_GEN_WAVE1    10	 // WAVE1 (maxlen)
#define SIG_GEN_WAVE2    11	 // WAVE2 (maxlen)
#define SIG_GEN_WAVE3    12  // WAVE3 (maxlen)
/**
  * @}
  */

/**
  * @brief  Tone signal state structure
  */
typedef struct TONE_gen_state_tag {
 uint8_t sig_type;
 uint8_t sig_state;
 uint8_t sig_pause;
 uint32_t sig_time;
 uint16_t maxlen;	
 union {
  struct {
   int16_t sigX[3];
   int32_t sigK;
  } tone_sig;
  struct {
   int32_t *wave_file;
   int16_t wave_idx;
   int16_t wave_smp_num;
  } wave_sig;
 } param;
 struct TONE_gen_state_tag *prev_st;
} sTONE_gen_state;

/**
  * @brief  initialization of signal processor
  * @retval none
  */
void tone_genInit();

/**
  * @brief  play new signal
  * @param  [in] sigtype - signal type
  * @param  [in] maxlen - length of signal in ms
  * @retval none
  */
void tone_genPlayTone(uint8_t sigtype, uint16_t maxlen);

/**
  * @brief get current playing state
  * @retval result - 0 - no playing, 1 - playing
  */
uint8_t tone_genIsTone();

/**
  * @brief stop current signal generation
  * @retval none
  */
void tone_genStop();

/**
  * @brief  periodic update signal processor state
  * @retval none
  */
void tone_genProcess();

/**
  * @brief  get current playing signal samples
  * @param  [inout] buf - buffer for samples
  * @param  [in] len - number of samples to generate
  * @retval result - number of generated samples (if less then len - signal ended)
  */
uint16_t tone_genGetData(int16_t *buf, uint16_t len);

/**
  * @}
  */

#endif
