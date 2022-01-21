/*
 * audio_test.h
 *
 *  Created on: Jun 18, 2020
 *      Author: Donskikh Dmitry
 */

#ifndef INC_AUDIO_TEST_H_
#define INC_AUDIO_TEST_H_

#include "inttypes.h"

#define TEST_SAMPLERATE (uint32_t)16000
#define BUFF_SIZE		2*1024*2

#define RBT_TONE	(uint32_t)(TEST_SAMPLERATE*2)
#define RBT_SILENCE	(uint32_t)(TEST_SAMPLERATE*2)
#define RBT_TOTAL	(uint32_t)(RBT_SILENCE+RBT_TONE)
#define RBT_FOSC1	440//440
#define RBT_FOSC2	480//480
#define RBT_FAM		20
#define AOUT_BUFF	320

//void generateSine(uint16_t freq);
void makeRingBackTone(void);
void playBuffer(uint16_t *buffer,uint32_t size);
void stopPlaying();


#endif /* INC_AUDIO_TEST_H_ */
