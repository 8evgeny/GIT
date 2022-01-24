/*
 * audio.cpp
 *
 *  Created on: 4 сент. 2020 г.
 *      Author: Dmitry Dmitry Donskih
 */


#include <audio.hpp>
#include <limits.h>
#include <ipssystem.hpp>

#include <system_c.h>

#define SAI_NOTE_HALF_COMPL_OUT		0x0001
#define SAI_NOTE_FULL_COMPL_OUT		0x0002
#define SAI_NOTE_HALF_COMPL_IN		0x0003
#define SAI_NOTE_FULL_COMPL_IN		0x0004

#define LAZYINIT_IPC			0x01

static TaskHandle_t runner=nullptr;
static TaskHandle_t runner2=nullptr;

extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;

audioDevice::audioDevice()
:dac_fifo(AOUT_BUFFER_BYTES/2,&dac_fifo_buffer[0],AOUT_BUFFER_BYTES*AOUT_FRAME_COUNT)
,adc_fifo(AOUT_BUFFER_BYTES/2,&adc_fifo_buffer[0],AOUT_BUFFER_BYTES*AOUT_FRAME_COUNT)
    {
    out_buffer=(uint8_t*)system_cpp.allocateDmaBuffer(AOUT_BUFFER_BYTES);
    in_buffer=(uint8_t*)system_cpp.allocateDmaBuffer(AOUT_BUFFER_BYTES);
    xTaskCreate(_runner_fcn, "DAC Output", 512, (void*)this, 8, &task);
    xTaskCreate(_runner_fcn1, "ADC Input", 512, (void*)this, 8, &task2);
    lock_output=osSemaphoreNew(1, 1, NULL);
    }

audioDevice& audioDevice::getInstance()
    {
      static audioDevice instance;
      return instance;
    }

int audioDevice::Play()
    {
	if (HAL_SAI_Transmit_DMA(&hsai_BlockA1, (uint8_t*)out_buffer,AOUT_BUFFER_SAMPLES*2)!=HAL_OK){
	    Error_Handler();
    	}
	if (HAL_SAI_Receive_DMA(&hsai_BlockB1, (uint8_t*)in_buffer,AOUT_BUFFER_SAMPLES*2)!=HAL_OK){
	    Error_Handler();
	}
	return 0;
    }

int audioDevice::Stop()
    {
	HAL_SAI_DMAStop(&hsai_BlockA1);
	HAL_SAI_DMAStop(&hsai_BlockB1);
	dac_fifo.clean();
	adc_fifo.clean();
	return 0;
    }

basicPipe* audioDevice::ADCPointer()
    {
    return &adc_fifo;
    }

basicPipe& audioDevice::ADCInstance()
    {
    return adc_fifo;
    }

void audioDevice::_runner_fcn(void *arg)
    {
	(static_cast<audioDevice*>(arg))->taskRunFcn();
    }
void audioDevice::_runner_fcn1(void *arg)
    {
	(static_cast<audioDevice*>(arg))->taskRunFcn2();
    }

audioDevice* audioDevice::getPtr()
    {
    return this;
    }

void audioDevice::taskRunFcn()
{
    runner=xTaskGetCurrentTaskHandle();
    while (1){
	xTaskNotifyWait(0x00,ULONG_MAX , &notification, portMAX_DELAY);

	if(notification==SAI_NOTE_HALF_COMPL_OUT){
	    outHalf();
	}
	if(notification==SAI_NOTE_FULL_COMPL_OUT){
	    outFull();
	}
	notification=0;

	//sysLedTestToggle();

    }
}

void audioDevice::taskRunFcn2()
{
    runner2=xTaskGetCurrentTaskHandle();
    while (1){
	xTaskNotifyWait(0x00,ULONG_MAX , &notification2, portMAX_DELAY);

	if(notification2==SAI_NOTE_HALF_COMPL_IN){
	    inHalf();
	}
	if(notification2==SAI_NOTE_FULL_COMPL_IN){
	    inFull();
	}
	notification=0;

	//sysLedTestToggle();

    }
}

void audioDevice::outHalf(){
    dac_fifo.pop((uint8_t*)out_buffer);
}

basicPipe* audioDevice::DACPointer ()
{
  return &dac_fifo;
}

basicPipe& audioDevice::DACInstance ()
{
  return dac_fifo;
}


void audioDevice::outFull()
{
    dac_fifo.pop(((uint8_t*)out_buffer)+AUDIO_FRAME_SIZE_BYTES);
}

void audioDevice::inHalf ()
{
  adc_fifo.push((uint8_t*)in_buffer);
}

void audioDevice::inFull ()
{
  adc_fifo.push(((uint8_t*)in_buffer)+AUDIO_FRAME_SIZE_BYTES);
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
//    BaseType_t xsYeld=pdFALSE;
    xTaskNotifyFromISR(runner,SAI_NOTE_FULL_COMPL_OUT,eSetValueWithOverwrite,NULL);
//    //xTaskNotify(*runner,SAI_NOTE_FULL_COMPL_OUT,eSetBits);
//    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//    (audioDevice::getInstance()).notification=SAI_NOTE_FULL_COMPL_OUT;
//    osSemaphoreRelease((audioDevice::getInstance()).lock_output);
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai){
    //BaseType_t xsYeld=pdFALSE;
    xTaskNotifyFromISR(runner,SAI_NOTE_HALF_COMPL_OUT,eSetValueWithOverwrite,NULL);
//    //xTaskNotify(*runner,SAI_NOTE_HALF_COMPL_OUT,eSetBits);
//
//    (audioDevice::getInstance()).notification=SAI_NOTE_HALF_COMPL_OUT;
//    osSemaphoreRelease((audioDevice::getInstance()).lock_output);
}

void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hsai)
{
  xTaskNotifyFromISR(runner2,SAI_NOTE_HALF_COMPL_IN,eSetValueWithOverwrite,NULL);
}

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
  xTaskNotifyFromISR(runner2,SAI_NOTE_FULL_COMPL_IN,eSetValueWithOverwrite,NULL);
}


