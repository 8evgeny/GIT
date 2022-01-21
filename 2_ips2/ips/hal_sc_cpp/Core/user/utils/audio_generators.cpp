/*
 * audio_generators.cpp
 *
 *  Created on: Sep 4, 2020
 *      Author: Dmitry Dmitry Donskih
 */
#include "FreeRTOS.h"

#include <audio_generators.hpp>
#include <math.h>
#include <new>
#include <ips_globals.hpp>


sineAudioGenerator::sineAudioGenerator()
:ipcThread("Sine gen", 512, 8)
    {
	child=this;
    }

//sineGenerator::sineGenerator(basicPipe *pipe):ipcThread("Sine generator", 512, osPriorityNormal)
//    {
//	//sineGenerator();
//	setPipe(pipe);
//    }

int sineAudioGenerator::setPipe(basicPipe *pipe)
    {
	o_pipe=pipe;
	if(buffer){
	    vPortFree(buffer);
	}
	buffer=(audio_frame_t*)pvPortMalloc(o_pipe->getSize());
	num_frames=o_pipe->getSize()/sizeof(uint32_t);
    }

void sineAudioGenerator::setFrequency(uint32_t Herz)
    {
	frequency=Herz;
	period=(double)	AUDIO_SAI_SAMPLERATE/(double)frequency;
    }

void sineAudioGenerator::generate()
    {
	for(int i=0;i<num_frames;i++){
	    buffer[i].left=g_function();
	}
    }

void sineAudioGenerator::reset()
    {
	timestamp=0;
    }

void sineAudioGenerator::taskRunFcn()
    {
	while(!o_pipe){
	    osDelay(1);
	}
	while(1){
	    generate();
	    o_pipe->push((uint8_t*)buffer);
	}
    }

int16_t sineAudioGenerator::g_function()
    {
	double val=sin((2*M_PI*timestamp)/period);
	val*=gain;
	val+=(gain*15);
	timestamp++;
	return (int16_t)val;
    }



