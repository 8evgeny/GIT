/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DV_port_SAI_input.c
 * Author: roman.ganaza
 *
 * Created on September 11, 2020, 4:41 PM
 */
#include <stdio.h>
#include <string.h>

#include "ST-BUS_SAI_in_out.h"
#include "main.h"
#include "dbg_uart.h"
#include "HDLC_Rx_low_level.h"
#include "HDLC_Tx_low_level.h"
#include "g711.h"
#include "main_logic.h"
#include "audio_in_out.h"

extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;

//uint8_t SAI_DMA_input_buffer[DV_INPUT_BUFFER_SIZE];
//uint8_t SAI_DMA_output_buffer[DV_INPUT_BUFFER_SIZE];
uint8_t SAI_DMA_input_buffer[32];
uint8_t SAI_DMA_output_buffer[32];
unsigned char C_channel_out = 0;
unsigned char sync_bit = 0;
// порядок следования бит и байт для буферов SAI_DMA_input/output_buffer
// байты 0, 1 и 2,3 попарно поменяны местами по сравнению с DS на MT9092
// т.е.
// SAI_DMA_x_buffer[0] - C-channel
// SAI_DMA_x_buffer[1] - D-channel
// SAI_DMA_x_buffer[2] - B2-channel
// SAI_DMA_x_buffer[3] - B1-channel
// порядок следования бит в байтах
// D-channel LSB (младший вперед)
// C, B1, B2 - MSB (старший вперед)
// SAI прием и передача настроены как MSB
static char str[64];

void SAI_DMA_Init(void) {
	HAL_SAI_Receive_DMA(&hsai_BlockB1, SAI_DMA_input_buffer, DV_INPUT_BUFFER_SIZE / 2); // делим на 2, у нас 16 битный прием/передача
	HAL_SAI_Transmit_DMA(&hsai_BlockA1, SAI_DMA_output_buffer, DV_INPUT_BUFFER_SIZE / 2); // делим на 2, у нас 16 битный прием/передача
	puts("SAI init\r");
}

void SAI_reset(void) {
	HAL_SAI_Abort(&hsai_BlockB1);
	HAL_SAI_Abort(&hsai_BlockA1);
	SAI_DMA_Init();
}

void DV_port_input_Init(void) {
	BitRingInit();
	SAI_DMA_Init();
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai) {

}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai) {

}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai) {

}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai) {
//  static uint16_t i;
	static unsigned char prev_in_c_channel = 0xAA, prev_out_c_channel = 0xAA;
	unsigned char hbit, lbit;
//	static uint8_t s = 0;
	unsigned char filtered_c_channel;
	static unsigned char prev_sync_bit = 0;
	
	// приём
	//--------------------------------------------------------------------------//
	// C-channel
	// для отладки выводим изменения принимаемого от MT9172 С-канала
	sync_bit = SAI_DMA_input_buffer[C_CHANNEL] >> 7;
	filtered_c_channel = SAI_DMA_input_buffer[C_CHANNEL] & ~(0x03 << 5); // биты качества сигнала (5-6) игнорируем
	HAL_GPIO_WritePin(LED_S1_GPIO_Port, LED_S1_Pin, (GPIO_PinState)!sync_bit);
	
	if ((prev_sync_bit != sync_bit) || ((sync_bit != 0)&&(prev_in_c_channel != filtered_c_channel))){ // либо изменился бит синхронизации, либо при установленном этом бите изменилось что-то еще
		strcpy(str, "IN C-ch: 0x");
		HexToChar(&str[11], filtered_c_channel);
		str[13] = '\r';
		str[14] = '\0';
		puts(str);
		prev_in_c_channel = filtered_c_channel;
		prev_sync_bit = sync_bit;
	}

	// D-channel (HDLC)
  // сохраняем биты, чтобы не успело перезаписать
  lbit = SAI_DMA_input_buffer[D_CHANNEL] >> 7;
  hbit = (SAI_DMA_input_buffer[D_CHANNEL] >> 6) & 0x01;

  // 1-й байт 1-го слота SAI (16 битного)
  BitRingProcessBit(lbit); // младший бит слота D-channel (HDLC)
  BitRingProcessBit(hbit); // старший бит слота D-channel (HDLC)

  // B1, B2-channels (звук, входящий поток)
  if (flag_sound_en) {
    if (sound_channel == SOUND_CH_B1) {
      AudioSamleToDAC(SAI_DMA_input_buffer[B1_CHANNEL]);
    } else if (sound_channel == SOUND_CH_B2) {
      AudioSamleToDAC(SAI_DMA_input_buffer[B2_CHANNEL]);
    } else {
      AudioSamleToDAC(0);
    }
  } else {
    AudioSamleToDAC(0);
  }
  // синус на ЦАП
//  i += 8;
//  if (i >= 96) i = 0;
//  AudioOutPush(sinebuf1[i]);
 
	// передача
	//--------------------------------------------------------------------------//
	// C-channel
  SAI_DMA_output_buffer[C_CHANNEL] = C_channel_out; // передаем на MT9172 то, что принято по SPI
  // вывод отладочной информации
  if (prev_out_c_channel != C_channel_out) {
    strcpy(str, "OUT C-ch: 0x");
    HexToChar(&str[12], C_channel_out);
    str[14] = '\r';
    str[15] = '\0';
    puts(str);
    prev_out_c_channel = C_channel_out;
  }
	

	// D-channel (HDLC)
  if (flag_HDLC_bitstream_ready) { // если готов пакет на передачу (включение передатчика HDLC не важно, т.к. пакет надо допередать в любом случае)
    SAI_DMA_output_buffer[D_CHANNEL] = HdlcBitStreamGetBit() << 7; // младший бит
    SAI_DMA_output_buffer[D_CHANNEL] |= HdlcBitStreamGetBit() << 6; // старший бит


  } else { // Idle
    SAI_DMA_output_buffer[D_CHANNEL] = 1 << 7; // младший бит
    SAI_DMA_output_buffer[D_CHANNEL] |= 1 << 6; // старший бит
  }
	
	// B1, B2-channels (звук, исходящий поток аудио)
  if (sound_channel == SOUND_CH_B1) {
    SAI_DMA_output_buffer[B1_CHANNEL] = AudioSamleFromADC();
    SAI_DMA_output_buffer[B2_CHANNEL] = 0;
  } else if (sound_channel == SOUND_CH_B2) {
    SAI_DMA_output_buffer[B1_CHANNEL] = 0;
    SAI_DMA_output_buffer[B2_CHANNEL] = AudioSamleFromADC();
  } else {
    SAI_DMA_output_buffer[B1_CHANNEL] = 0;
    SAI_DMA_output_buffer[B2_CHANNEL] = 0;
  }
}

void HAL_SAI_ErrorCallback(SAI_HandleTypeDef *hsai) {
//#define HAL_SAI_ERROR_NONE    0x00000000U  /*!< No error                                    */
//#define HAL_SAI_ERROR_OVR     0x00000001U  /*!< Overrun Error                               */
//#define HAL_SAI_ERROR_UDR     0x00000002U  /*!< Underrun error                              */
//#define HAL_SAI_ERROR_AFSDET  0x00000004U  /*!< Anticipated Frame synchronisation detection */
//#define HAL_SAI_ERROR_LFSDET  0x00000008U  /*!< Late Frame synchronisation detection        */
//#define HAL_SAI_ERROR_CNREADY 0x00000010U  /*!< codec not ready                             */
//#define HAL_SAI_ERROR_WCKCFG  0x00000020U  /*!< Wrong clock configuration                   */
//#define HAL_SAI_ERROR_TIMEOUT 0x00000040U  /*!< Timeout error                               */
//#define HAL_SAI_ERROR_DMA     0x00000080U  /*!< DMA error                                   */
    sprintf(str, "SAI error: %x\r", (uint8_t)HAL_SAI_GetError(hsai));
	puts(str);

	SAI_DMA_Init();
}


