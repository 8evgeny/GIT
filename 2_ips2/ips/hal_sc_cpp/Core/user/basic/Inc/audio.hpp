/*
 * audio.hpp
 *
 *  Created on: 4 сент. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */

#ifndef USER_BASIC_INC_AUDIO_HPP_
#define USER_BASIC_INC_AUDIO_HPP_

#include "main.h"

#include <cstdint>
#include <ipc_pipe.hpp>
#include "cmsis_os2.h"
#include "semphr.h"


#define AUDIO_FRAME_SIZE	256
#define AUDIO_FRAME_SIZE_BYTES	(AUDIO_FRAME_SIZE*sizeof(uint32_t))
#define AOUT_BUFFER_SAMPLES	(AUDIO_FRAME_SIZE*2)
#define AOUT_BUFFER_BYTES	(AOUT_BUFFER_SAMPLES*sizeof(uint32_t))

#define AOUT_FRAME_COUNT	40

#define AUDIO_SAI_SAMPLERATE	16000

typedef union{
    uint32_t w;
    struct{
	int16_t left;
	int16_t right;
    };
}audio_frame_t;


class audioDevice{
public:
    static audioDevice& getInstance();
    int Play();
    int Stop();
    basicPipe* ADCPointer();
    basicPipe& ADCInstance();

    basicPipe* DACPointer();
    basicPipe& DACInstance();

    audioDevice* getPtr();


    static void _runner_fcn(void* arg);
    static void _runner_fcn1(void* arg);

    friend void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai);
    friend void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai);

    friend void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai);
    friend void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai);

private:
    audioDevice();
    audioDevice(const audioDevice&);
    audioDevice& operator=(audioDevice&);

    void taskRunFcn();
    void taskRunFcn2();
    void outHalf();
    void outFull();

    void inHalf();
    void inFull();

    audio_frame_t frame_test={.w=0xABCDEF01};
    osSemaphoreId_t lock_output;
    TaskHandle_t task, task2;
    basicPipe dac_fifo;
    basicPipe adc_fifo;
    uint32_t notification=0;
    uint32_t notification2=0;
    uint8_t *out_buffer=nullptr;
    uint8_t *in_buffer=nullptr;

    uint8_t dac_fifo_buffer[AOUT_BUFFER_BYTES*AOUT_FRAME_COUNT];
    uint8_t adc_fifo_buffer[AOUT_BUFFER_BYTES*AOUT_FRAME_COUNT];

};



#endif /* USER_BASIC_INC_AUDIO_HPP_ */
