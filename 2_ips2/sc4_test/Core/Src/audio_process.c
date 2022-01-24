/**
  ******************************************************************************
  * @file    audio_process.c
  * @author  dmib@@ETS
  * @brief   This file contains audio processing module implementation
  ******************************************************************************
  */

#include "stm32h7xx_hal.h"
#include "cbuffer.h"
#include "CLI_io.h"
#include "g711.h"
#include "driver_AIC.h"
#include "driver_ui.h"
#include "string.h"
#include "system_settings.h"
#include "audio_process.h"

#define AUDIO_MIX_CHANNEL_FREE		0	///< channel is free code
#define AUDIO_MIX_CHANNEL_STOP		1	///< channel is stopped code
#define AUDIO_MIX_CHANNEL_PLAY		2	///< channel is playing code

///overflow level of output mixer buffer
#define MAX_BUFIN_OCCUPANCY (AUDIO_FRAME_SIZE*8)

/// link to TIMER3 hardware module control/status structure
extern TIM_HandleTypeDef htim3;

/// audio input buffer
uint8_t audio_mic_input_buffer[AUDIO_IO_BUFFER_SIZE]  __attribute__((section(".RAM_D1_data")));
/// audio mixer output buffer
int16_t audio_mix_output_buffer[AUDIO_IO_BUFFER_SIZE]  __attribute__((section(".RAM_D1_data")));
/// audio mixer channels buffers
uint8_t audio_mix_channels_memory[AUDIO_MAX_MIX_CHANNELS][AUDIO_MIX_BUFFER_SIZE]  __attribute__((section(".RAM_D1_data")));
//uint8_t audio_mic_input_buffer[AUDIO_IO_BUFFER_SIZE]  __attribute__((section(".ExtRamData")));
//int16_t audio_mix_output_buffer[AUDIO_IO_BUFFER_SIZE]  __attribute__((section(".ExtRamData")));
//uint8_t audio_mix_channels_memory[AUDIO_MAX_MIX_CHANNELS][AUDIO_MIX_BUFFER_SIZE]  __attribute__((section(".ExtRamData")));

/// audio input buffer description structure
CBuffer audio_mic_input;
/// audio mixer output buffer description structure
CBuffer audio_mix_output;
/// audio mixer channel buffer description structures
CBuffer audio_mix_channels[AUDIO_MAX_MIX_CHANNELS];
/// audio mixer channels states
uint8_t audio_mix_channels_state[AUDIO_MAX_MIX_CHANNELS];

/// buffer for G711 encoding
uint8_t audio_g711_buf[SAI_DMA_BUFFER_SIZE];

uint8_t temp_audio_buf[AUDIO_FRAME_SIZE]; // TEST
uint8_t mix_loop_on;

/// audio mixer timer samples counter
uint32_t smp_count;
uint32_t mixsmpcnt; // debug

void audio_init()
{
	uint8_t i;
	
	CBuffer_Init(&audio_mic_input, audio_mic_input_buffer, AUDIO_IO_BUFFER_SIZE);
	CBuffer_Init(&audio_mix_output, (uint8_t *) audio_mix_output_buffer, AUDIO_IO_BUFFER_SIZE * sizeof(int16_t));
	
	for (i = 0 ; i < AUDIO_MAX_MIX_CHANNELS; i++) {
	 audio_mix_channels_state[i] = AUDIO_MIX_CHANNEL_FREE;
	 CBuffer_Init(&audio_mix_channels[i], audio_mix_channels_memory[i], AUDIO_MIX_BUFFER_SIZE);
	}
}

void audio_start()
{
	uint8_t i;

	CBuffer_Reset(&audio_mic_input);
	CBuffer_Reset(&audio_mix_output);

	for (i = 0 ; i < AUDIO_MAX_MIX_CHANNELS; i++) {
	 audio_mix_channels_state[i] = AUDIO_MIX_CHANNEL_FREE;
	 CBuffer_Reset(&audio_mix_channels[i]);
	}

	audio_mix_channels_state[0] = AUDIO_MIX_CHANNEL_PLAY; // TEST
	mix_loop_on = 1;

	HAL_TIM_Base_Start_IT(&htim3);
}

void audio_stop()
{
	CBuffer_Reset(&audio_mix_output);
	HAL_TIM_Base_Stop_IT(&htim3);
}

void audio_process()
{
	//uint16_t i;
	//int16_t sum;
	//TODO if (audio_mic_input.datalen > AUDIO_FRAME_SIZE) sendrtp
	
	//TEST loop
	// dis|en timer
	if (audio_mic_input.datalen >= AUDIO_FRAME_SIZE) {
		if (mix_loop_on) {
			CBuffer_ReadData(&audio_mic_input, &temp_audio_buf[0], AUDIO_FRAME_SIZE);
			/*for (i=0;i<AUDIO_FRAME_SIZE;i++) {
				sum = g711_A2l[temp_audio_buf[i]];
				CBuffer_WriteData(&audio_mix_output, (uint8_t *)&sum, 2);
			}*/
			audio_write_mix_channel(0, &temp_audio_buf[0], AUDIO_FRAME_SIZE);
		}
/*		HAL_NVIC_DisableIRQ(TIM3_IRQn);
		if (!CBuffer_WriteData(&audio_mix_channels[0], &audio_g711_buf[0], AUDIO_FRAME_SIZE)) {
		 ///
			CBuffer_Reset(&audio_mix_channels[0]);
		};
		HAL_NVIC_EnableIRQ(TIM3_IRQn);*/
	}
}

int8_t audio_write_input(int16_t *data, uint16_t size)
{
	//data(dma) -> encode G711 -> audio_mic_input
	size >>= 1;

	g711_pcm2alaw(data, audio_g711_buf, size);
	//CBuffer_WriteData(&audio_mix_output, (uint8_t *)data, size<<1);  // TEST write to out immediatly

	if (!CBuffer_WriteData(&audio_mic_input, &audio_g711_buf[0], size)) return -2;

	return 0;
}

int8_t audio_get_output(int16_t *data, uint16_t size)
{
	//audio_mix_output -> data(dma)
	if (audio_mix_output.datalen >= size) {
		HAL_NVIC_DisableIRQ(TIM3_IRQn);
		CBuffer_ReadData(&audio_mix_output, (uint8_t *) data, size);
		HAL_NVIC_EnableIRQ(TIM3_IRQn);
	} else
		memset((uint8_t *)data, 0 ,size);

	return 0;
}

int8_t audio_write_mix_channel(uint8_t ch_num, uint8_t *data, uint16_t size)
{
	//rtp -> audio_mix_chnneal
	if (audio_mix_channels_state[ch_num]==AUDIO_MIX_CHANNEL_FREE) return -1;

	HAL_NVIC_DisableIRQ(TIM3_IRQn);
	if (!CBuffer_WriteData(&audio_mix_channels[ch_num], data, size)) return -2;
	HAL_NVIC_EnableIRQ(TIM3_IRQn);

	return 0;
}

int8_t audio_get_input(uint8_t *data, uint16_t size)
{
	if (audio_mic_input.datalen < size) return -1;

	CBuffer_ReadData(&audio_mic_input, (uint8_t *) data, size);

	return 0;
}

int8_t audio_get_mix_channel()
{
	uint8_t i;
	
	for (i = 0 ; i < AUDIO_MAX_MIX_CHANNELS; i++)
	 if (audio_mix_channels_state[i]==AUDIO_MIX_CHANNEL_FREE) {
	  audio_mix_channels_state[i] = AUDIO_MIX_CHANNEL_STOP;
	  CBuffer_Reset(&audio_mix_channels[i]);
 	  return i;
	 }
	
	return -1;
}

void audio_free_mix_channel(uint8_t ch_num)
{
	audio_mix_channels_state[ch_num]=AUDIO_MIX_CHANNEL_FREE;
	CBuffer_Reset(&audio_mix_channels[ch_num]);
}

int8_t audio_reset_mix_channel(uint8_t ch_num)
{
	if (audio_mix_channels_state[ch_num]==AUDIO_MIX_CHANNEL_FREE) return -1;
	CBuffer_Reset(&audio_mix_channels[ch_num]);
	return ch_num;
}

int8_t audio_play_mix_channel(uint8_t ch_num)
{
	if (audio_mix_channels_state[ch_num]==AUDIO_MIX_CHANNEL_FREE) return -1;
	audio_mix_channels_state[ch_num]=AUDIO_MIX_CHANNEL_PLAY;
	return ch_num;
}

int8_t audio_stop_mix_channel(uint8_t ch_num)
{
	if (audio_mix_channels_state[ch_num]==AUDIO_MIX_CHANNEL_FREE) return -1;
	audio_mix_channels_state[ch_num]=AUDIO_MIX_CHANNEL_STOP;
	return ch_num;
}

/**
  * @brief audio mixer timer interrupt callback
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint8_t i;
	int16_t sum;
	uint8_t smp;

	if (htim != &htim3) return;

	smp_count++;
	if (smp_count>16000) {  // debug
//		CLI_print("Mix Onesec");
		smp_count=0;
	}
	if (smp_count&1) {
	// mix sig_gen/audio_udp_input to audio_mix_output
	mixsmpcnt++;
	sum = 0;
	for (i = 0 ; i < AUDIO_MAX_MIX_CHANNELS; i++)
	{
	 if (audio_mix_channels_state[i]==AUDIO_MIX_CHANNEL_PLAY) {
	   if (audio_mix_channels[i].datalen)
		    smp = CBuffer_ReadByte(&audio_mix_channels[i]);
	   else
		    smp = 0xD5; // silence

	   sum = sum + g711_A2l[smp];

	   if (audio_mix_channels[i].datalen > MAX_BUFIN_OCCUPANCY) // flush if overflow
		   smp = CBuffer_ReadByte(&audio_mix_channels[i]);
	 }
	}
	CBuffer_WriteData(&audio_mix_output, (uint8_t *)&sum, 2);
	}
}
