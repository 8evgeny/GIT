/* 
 * File:   audio_in_out.c
 * Author: Роман
 * 
 * Created on 7 января 2021 г., 22:39
 */
#include "audio_in_out.h"
#include "audio_settings.h"
#include "main.h"
#include "uart.h"
#include "FIFO.h"
#include "codec.h"

#define AUDIO_OUT_FIFO_SIZE     128 // степень 2!
#define AUDIO_IN_FIFO_SIZE     128 // степень 2!

int16_t sinebuf1[96] = {0, 0, 4276, 4276, 8480, 8480, 12539, 12539, 16383, 16383, 19947, 19947, 23169, 23169, 25995, 25995,
  28377, 28377, 30272, 30272, 31650, 31650, 32486, 32486, 32767, 32767, 32486, 32486, 31650, 31650, 30272, 30272,
  28377, 28377, 25995, 25995, 23169, 23169, 19947, 19947, 16383, 16383, 12539, 12539, 8480, 8480, 4276, 4276,
  0, 0, -4276, -4276, -8480, -8480, -12539, -12539, -16383, -16383, -19947, -19947, -23169, -23169, -25995, -25995,
  -28377, -28377, -30272, -30272, -31650, -31650, -32486, -32486, -32767, -32767, -32486, -32486, -31650, -31650, -30272, -30272,
  -28377, -28377, -25995, -25995, -23169, -23169, -19947, -19947, -16383, -16383, -12539, -12539, -8480, -8480, -4276, -4276};

//---------------------------------------------------------------------------------------------------------------

extern I2S_HandleTypeDef hi2s3;

uint16_t audio_out_DMA_Buffer[AUDIO_IN_OUT_DMA_BUFFER_SIZE];
uint16_t audio_in_DMA_Buffer[AUDIO_IN_OUT_DMA_BUFFER_SIZE];
uint16_t audio_out_sample = 0;

FIFO_int16(AUDIO_OUT_FIFO_SIZE) audio_out_fifo;
FIFO_int16(AUDIO_IN_FIFO_SIZE) audio_in_fifo;

uint8_t flag_audio_OK = 0; // флаг успешного инита аудио

///////////// Prototypes

void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s);
void I2SEx_TxRxDMACpltAlt(DMA_HandleTypeDef *hdma);
void HAL_I2SEx_TxRxCpltAltCallback(I2S_HandleTypeDef *hi2s);

///////////// Functions


void AudioInOutInit(void) {
  uint8_t i;
  HAL_StatusTypeDef hal_err;
  uint8_t codec_err;
  
  for(i = 0; i < 96; i++) sinebuf1[i] = sinebuf1[i]/8;
//  for(i = 0; i < 96; i++) sinebuf1[i] = sinebuf1[i]/1;
//  for(i = 0; i < 96; i++) sinebuf1[i] = 65535*i/96-32767; // пила

	// -------------------------------------------------------------------
    // закомментированные параметры устанавливаются в initUserInterface()

	
  while(FIFO_COUNT(audio_out_fifo) < FIFO_SIZE(audio_out_fifo)/2) { // заполняем FIFO на половину
    FIFO_PUSH(audio_out_fifo, 0);
  }

  codec_err = codecInit();
  
  if (codec_err != 0) uartPuts("codecInit ERROR!\r");
  else uartPuts("codecInit OK!\r");
  
  // по-умолчанию, в MT9092 всё выключено
  selectMic(0);
  micOff();
  headsetSpeakerOff();
  lineOutSpeakerOff();
  
	if (NVIC_GetEnableIRQ(DMA1_Stream0_IRQn)) { // если сучий Cube опять включил второе прерывание по I2S DMA (по Rx)
		HAL_NVIC_DisableIRQ(DMA1_Stream0_IRQn); // вырубаем его на хрен
	}
  
  uartPuts("AudioInOutInitDMA...");
  // исправляем баг в HAL (не работает от внешнего тактирования):
  // 1. настраиваем предделитель
  // When the master clock is disabled(MCKOE bit cleared) :
  // FS = I2SxCLK / [(16 * 2)*((2 * I2SDIV) + ODD))] when the channel frame is 16 - bit wide
  // FS = I2SxCLK / [(32 * 2)*((2 * I2SDIV) + ODD))] when the channel frame is 32 - bit wide
  //  hi2s3.Instance->I2SPR = 2 | (0 << 8); // I2SDIV = 2, ODD = 0, MCKOE = 0;
  hi2s3.Instance->I2SPR = 4 | (0 << 8); // I2SDIV = 4, ODD = 0, MCKOE = 0; - для 16 бит в 16 битном фрейме при I2SxCLK = 2,048 МГц

  // 2. Включаем внешний источник тактирования
  __HAL_RCC_I2S_CONFIG(RCC_I2SCLKSOURCE_EXT);  
  
  // I2S кодека (ввод/вывод, дуплекс)
  hal_err = HAL_I2SEx_TransmitReceive_DMA(&hi2s3, audio_out_DMA_Buffer, audio_in_DMA_Buffer, AUDIO_IN_OUT_DMA_BUFFER_SIZE);
  // по TxCplt прерывание наступает позже на 10 мкс, используем его
  hi2s3.hdmatx->XferCpltCallback = I2SEx_TxRxDMACpltAlt; // подменяем callback

  if ((hal_err == HAL_OK)&&(codec_err == 0)) {
    flag_audio_OK = 1;
  }
  
  if (hal_err == HAL_OK) uartPuts("OK\r");
  else uartPuts("ERROR!\r");
}

// кладем сэмпл в FIFO для последующего вывода через ЦАП кодека
void AudioOutPush(int16_t sample) {
  // отправляем сэмпл в FIFO
  if (!FIFO_IS_FULL(audio_out_fifo)) {
    FIFO_PUSH(audio_out_fifo, sample);
  } else {
    uartPuts("Error: audio_out_fifo overflow!");
  }
}

// возвращает сэмпл с АЦП кодека
int16_t AudioInPop(void) {
  if (!flag_audio_OK) return 0;

  // берём сэмпл в FIFO
  if (!FIFO_IS_EMPTY(audio_in_fifo)) {
    return FIFO_POP(audio_in_fifo);
  } else {
    uartPuts("Error: audio_in_fifo underflow!");
    return 0;
  }
}

// вызывается с частотой 8 кГц
// (левый канал кодека)
void HAL_I2SEx_TxRxHalfCpltCallback(I2S_HandleTypeDef *hi2s) {
  static uint16_t i = 0;

  // !!! ВНИМАНИЕ!!! Эта функция может вызываться 2 раза, если Cube включил оба прерывания I2S TX/RX
// синус
  i += 8;
  if (i >= 96) i = 0;
  
// AUDIO IN - левый канал кодека
  
// AUDIO OUT - левый канал кодека
  
//   audio_out_sample = sinebuf1[i]; // синус
  if (flag_audio_OK) {
    if (!FIFO_IS_EMPTY(audio_out_fifo)) {
      audio_out_sample = FIFO_POP(audio_out_fifo);
      audio_out_DMA_Buffer[0] = audio_out_sample;
    } else {
      audio_out_DMA_Buffer[0] = 0;
      uartPuts("Error: audio_out_fifo underflow!");
    }
  }
}


// вызывается с частотой 8 кГц
// (правый канал кодека)
void HAL_I2SEx_TxRxCpltAltCallback(I2S_HandleTypeDef *hi2s) { // альтернативный колбэк, т.к. штатный только для DMA mode NORMAL
  //  static uint16_t i = 0;

  // !!! ВНИМАНИЕ!!! Эта функция может вызываться 2 раза, если Cube включил оба прерывания I2S TX/RX

  // AUDIO IN - правый канал кодека
  // отправляем сэмпл в FIFO
  if (!FIFO_IS_FULL(audio_in_fifo)) {
    FIFO_PUSH(audio_in_fifo, audio_in_DMA_Buffer[1]);
    //      FIFO_PUSH(audio_in_fifo, sinebuf1[i]); // синус
  } else {
    uartPuts("Error: audio_in_fifo overflow!");
  }

  // AUDIO OUT - правый канал кодека
  //  i += 8;
  //  if (i >= 96) i = 0;
  //  t = sinebuf1[i];
  if (flag_audio_OK) {
      audio_out_DMA_Buffer[1] = -audio_out_sample;
  }
}

void I2SEx_TxRxDMACpltAlt(DMA_HandleTypeDef *hdma) { // альтернативный обработчик, т.к. штатный только для Normal mode DMA
  I2S_HandleTypeDef *hi2s = (I2S_HandleTypeDef *) ((DMA_HandleTypeDef *) hdma)->Parent;
  
  hi2s->State = HAL_I2S_STATE_READY;
  HAL_I2SEx_TxRxCpltAltCallback(hi2s);
}


void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s){
	uartPuts("Error: HAL_I2S_ErrorCallback\r");
}
