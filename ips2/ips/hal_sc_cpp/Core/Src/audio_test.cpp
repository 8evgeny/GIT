/*
 * audio_test.c
 *
 *  Created on: Jun 18, 2020
 *      Author: Donskikh Dmitry
 */

#include "audio_test.h"
#include "main.h"
#include "math.h"
#include "string.h"
#include "system.h"
#include "SRAM.h"
#include "i2cflash.h"

#include <ipssystem.hpp>


//#define M_PIf32	__f32 (3.141592653589793238462643383279502884)
#define VAL_BIAS	32767
//#define EXTERNAL
//#define MODE_POLLING
const uint32_t  c_ringrack_size=RBT_TOTAL*2;
int frames_played4=0;


extern SAI_HandleTypeDef hsai_BlockA1;

#ifndef EXTERNAL
static uint16_t ringback_tone_val[RBT_TOTAL*2]; // __attribute__ ((section (".intflash")));
uint16_t *ringback_tone=&ringback_tone_val[0];
#else
uint16_t *ringback_tone=NULL;
#endif

static uint16_t *frame=NULL;
static uint32_t playing_file_size=0;

static perf_t audio_perf;


typedef void (*func_t)(void);
static func_t	halfRecievedFct=NULL;
static func_t	fullRecievedFct=NULL;

static uint16_t *playing_buffer=NULL;
static FILE_t	*playing_file=NULL;

static void bufferHalfRecieved(void);
static void bufferFullRecieved(void);

static void pipeHalf();
static void pipeFull();

void makeRingBackTone(){
#ifdef EXTERNAL
	ringback_tone=allocateExtSramStatic(RBT_TOTAL*2*sizeof(uint16_t));
#endif
	memset(ringback_tone,0x0000,RBT_TOTAL*2*sizeof(uint16_t));
	double period_osc1=(double)TEST_SAMPLERATE/(double)RBT_FOSC1;
	double period_osc2=(double)TEST_SAMPLERATE/(double)RBT_FOSC2;
	double period_am=(double)TEST_SAMPLERATE/(double)RBT_FAM;
	for (int i=0;i<RBT_TONE;i++){
		//PERFCHECK_START
		double val=(sin((2*M_PI*i)/period_osc1)+sin((2*M_PI*i)/period_osc2))/2;
		val*=(sin((2*M_PI*i)/period_am)+1)*0.05;
		val*=5000;
		//PERFCHECK_STOP(audio_perf);
		*(ringback_tone+i*2)=(int16_t)val;
	}
/*
#ifdef EXTERNAL
	SCB_CleanDCache_by_Addr(ringback_tone, (RBT_TOTAL*2*sizeof(uint16_t)));
#endif
*/
}


static uint32_t cursor;

void playBuffer(uint16_t *buffer,uint32_t size)
{
	if (frame==NULL){frame=(uint16_t*)system_cpp.allocateDmaBuffer(AOUT_BUFF*2*sizeof(uint16_t));}
	playing_buffer=buffer;
	playing_file_size=size;
	halfRecievedFct=&bufferHalfRecieved;
	fullRecievedFct=&bufferFullRecieved;
	memcpy(frame,playing_buffer,AOUT_BUFF*2*sizeof(uint16_t));
	cursor=AOUT_BUFF*2;
	if (HAL_SAI_Transmit_DMA(&hsai_BlockA1, (uint8_t*)frame,AOUT_BUFF*2)!=HAL_OK){
		Error_Handler();
	}

}

void stopPlaying(){
	HAL_SAI_DMAStop(&hsai_BlockA1);
	halfRecievedFct=NULL;
	fullRecievedFct=NULL;
	playing_file_size=0;
	playing_buffer=NULL;
	playing_file=NULL;
	cursor=0;

}


void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
	fullRecievedFct();
}

void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hsai){
	halfRecievedFct();

	/*
	memcpy(frame,ringback_tone+cursor,1024*sizeof(uint16_t));
	cursor+=1024;
	if(cursor>=RBT_TOTAL*2){cursor=0;}
	*/
	//SCB_CleanDCache_by_Addr(&(frame[0]), 1024*sizeof(uint16_t));

}

void bufferFullRecieved(void){
    memcpy(frame+AOUT_BUFF,playing_buffer+cursor,AOUT_BUFF*sizeof(uint16_t));
    if(cursor+AOUT_BUFF>playing_file_size*2){
    		cursor=0;
    	}else{
    		cursor+=AOUT_BUFF;
    	}
}

void bufferHalfRecieved(void)
    {
    memcpy(frame,playing_buffer+cursor,AOUT_BUFF*sizeof(uint16_t));
    if(cursor+AOUT_BUFF>playing_file_size*2){
    		cursor=0;
    	}else{
    		cursor+=AOUT_BUFF;
    	}
    }




