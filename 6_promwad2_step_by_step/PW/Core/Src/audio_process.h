/**
  ******************************************************************************
  * @file    audio_process.h
  * @author  dmib@@ETS
  * @brief   This file contains audio processing module header
  ******************************************************************************
  */
 
#ifndef _AUDIO_PROCESS_
#define _AUDIO_PROCESS_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

///overflow level of output mixer buffer
#define MAX_BUFIN_OCCUPANCY (AUDIO_FRAME_SIZE*8)

/** @defgroup audio_process Audio processing module
  * @brief audio processing implementation
  * @{
  */

/// audio frame size
#define AUDIO_FRAME_SIZE		160

/// audio input/output buffers size
#define AUDIO_IO_BUFFER_SIZE	8000
/// audio maximum mixer channels
#define AUDIO_MAX_MIX_CHANNELS	16
/// audio mixer buffer size
#define AUDIO_MIX_BUFFER_SIZE	4000

#define AUDIO_MIX_CHANNEL_FREE		0	///< channel is free code
#define AUDIO_MIX_CHANNEL_STOP		1	///< channel is stopped code
#define AUDIO_MIX_CHANNEL_PLAY		2	///< channel is playing code

/**
  * @brief  audio processing module initialization
  * @retval none
  */
void audio_init();

/**
  * @brief  start audio processing
  * @retval none
  */
void audio_start();

/**
  * @brief  stop audio processing
  * @retval none
  */
void audio_stop();

/**
  * @brief  audio processing periodic task
  * @retval none
  */
void audio_process();

//-----------------------------------------/
//interface to AIC driver

/**
  * @brief  write input audio data from AIC driver
  * @param  [in] data - input audio data for processing
  * @param  [in] size - size of data
  * @retval none
  */
int8_t audio_write_input(int16_t *data, uint16_t size);

/**
  * @brief  get output audio data for AIC driver
  * @param  [inout] data - output audio data for playing
  * @param  [in] size - size of data
  * @retval none
  */
int8_t audio_get_output(int16_t *data, uint16_t size);

//interface to AIC driver end
//-----------------------------------------/

//-----------------------------------------/
//interface to UDP module

/**
  * @brief  write audio data to mixer channel
  * @param  [in] ch_num - channel index
  * @param  [in] data - audio data
  * @param  [in] size - size of data
  * @retval none
  */
int8_t audio_write_mix_channel(uint8_t ch_num, uint8_t *data, uint16_t size);

/**
  * @brief  get input audio data for transmitting
  * @param  [inout] data - array for audio data
  * @param  [in] size - size of data
  * @retval none
  */
int8_t audio_get_input(uint8_t *data, uint16_t size);

//interface to UDP module end
//-----------------------------------------/

//-----------------------------------------/
//common audio mixer interface

/**
  * @brief  get new mixer channel
  * @retval result - index of new mixer channel (-1 - no free channels)
  */
int8_t audio_get_mix_channel();

/**
  * @brief  free mixer channel
  * @param  [in] ch_num - channel index
  * @retval none
  */
void audio_free_mix_channel(uint8_t ch_num);

/**
  * @brief  reset mixer channel
  * @param  [in] ch_num - channel index
  * @retval result - 0 - operation done; - 1 - channel is free
  */
int8_t audio_reset_mix_channel(uint8_t ch_num);

/**
  * @brief  play mixer channel
  * @param  [in] ch_num - channel index
  * @retval result - 0 - operation done; - 1 - channel is free
  */
int8_t audio_play_mix_channel(uint8_t ch_num);

/**
  * @brief  stop mixer channel
  * @param  [in] ch_num - channel index
  * @retval result - 0 - operation done; - 1 - channel is free
  */
int8_t audio_stop_mix_channel(uint8_t ch_num);

//common audio mixer interface end
//-----------------------------------------/

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
