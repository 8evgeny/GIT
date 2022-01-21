/*
 * codec.h
 *
 *  Created on: 18 июн. 2020 г.
 *      Author: Donskikh Dmitry
 */

/*
 * codec.h - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */

#ifndef INC_CODEC_H_
#define INC_CODEC_H_

#include "inttypes.h"

#define CH_OUT_L 	0x01
#define CH_OUT_R 	0x02
#define CH_IN_L		0x04
#define CH_IN_R		0x08

/////////////////////////page0///////////////////////////

#define AIC3254_CLK_SRC_MUX		4
#define AIC3254_NDAC_REG		11
#define AIC3254_MDAC_REG		12
#define AIC3254_DOSR_REG1		13 //MSB
#define AIC3254_DOSR_REG2		14 //LSB
#define AIC3254_NADC_REG		18
#define AIC3254_MADC_REG		19
#define AIC3254_AOSR_REG		20
#define AIC3254_INTERFACE_REG		27


#define AIC3254_MUTE_ADC_REG		82
#define AIC3254_MUTE_DAC_REG		64

////////////////////////page1////////////////////////////

#define AIC3254_MUTE_LOUT_REG	18

///////////////////////values///////////////////////////
#define AIC3254_MUTE_ADC		0x88
#define AIC3254_MUTE_DAC		0x0E
#define AIC3254_MUTE_LOUT		0x40



#define AIC3254_CLKSRC_MCLK		0x00
#define AIC3254_xDAC_ON			0x80
#define AIC3254_xADC_ON			0x80
#define AIC3254_I2CSTD			0x00


typedef struct {
    uint8_t regOffset;
    uint8_t regVal;
}aic3254Reg;

void codecRegWriteP(uint8_t page,uint8_t reg, uint8_t value);
void codecRegWrite(uint8_t reg,uint8_t value);
void codecRegWriteSq( aic3254Reg *reg_pairs,uint16_t byte_size);
uint8_t codecRegRead(uint8_t reg);
void codecSetup ();
void codecSetOutVolume(int8_t volume);
void codecSetInpGain(uint8_t gain);
void codecMute(uint8_t channel);
void codecUnmute(uint8_t channel);
void codecReadFault(uint8_t *sticky1, uint8_t *sticky2, uint8_t *sticky3);
void codecReadDACStatus(uint8_t *status1, uint8_t *status2);

void codecMicMute(bool mute);
void codecAmpMute(bool mute);
void MicMuteToggle();
void AmpMuteToggle();
void VolPlus();
void VolMinus();

#endif /* INC_CODEC_H_ */
