/*
 * AIC3254.cpp
 *
 *  Created on: 4 авг. 2020 г.
 *      Author: Dmitry Donskih
 */

/*
 * file - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */




#include <AIC3254.hpp>

#define AIC3254_ADDR	0x30

#define AIC3254_RQ_SIZE	0x02

// @formatter:off
static const aic3254Reg c_Reset[] =
{
    { 0x00, 0x00 },
    { 0x01, 0x01 },
};

static const aic3254Reg c_ClockInitSequence[] =
{
    { 0, 0x00 }, 										/// Page Select
    { AIC3254_CLK_SRC_MUX, AIC3254_CLKSRC_MCLK },
    { AIC3254_NDAC_REG, (AIC3254_xDAC_ON | 1U) },
    { AIC3254_MDAC_REG, (AIC3254_xDAC_ON | 2U) },
    { AIC3254_DOSR_REG1, 0U },
    { AIC3254_DOSR_REG2, 128U },
    { 60, 6 },
    { AIC3254_NADC_REG, (AIC3254_xADC_ON | 1U) },
    { AIC3254_MADC_REG, (AIC3254_xADC_ON | 2U) },
    { AIC3254_AOSR_REG, 128U },
    { AIC3254_INTERFACE_REG, AIC3254_I2CSTD },
};

static const aic3254Reg c_PowerUpSequence[] =
{
    { 0, 0x01 },
    { 2, 0x09 },
    { 1, 0x08 },
    { 2, 0x01 },
    { 10, 0x3B },
    { 3, 0x00 },
    { 4, 0x00 },
    { 61, 0x00 },
    { 71, 0x32 },
    { 123, 0x01 },
};

static const aic3254Reg c_ADCStartupMute[] =
{
    { 0, 0x01 },
    { 52, 0x00 },
    { 54, 0x00 },
    { 55, 0x40 },//    { 55, 0xC0 },
    { 57, 0x10 },//    { 57, 0x30 },
    { 0, 0x00 },
    { 81, 0x80 }, 								///Left ADC Powerup
    { 82, 0x88 },
};

static const aic3254Reg c_DACStartup[] =
{
    { 0, 0x01 },
    { 10, 0x03 },
    { 14, 0x08 },
    { 9, 0x0C },
    { 18, 0x08 },
    { 19, 0x08 },
    { 0, 0x00 },
    { 65, 0x10 },
    { 63, 0x90 },
    { 64, 0x00 },
};
// @formatter:on

AIC3254::AIC3254(I2C_HandleTypeDef *handle)
    {
    hi2c = handle;
    codecSetup();
    }

void AIC3254::codecRegWrite(uint8_t reg, uint8_t value)
    {
    uint8_t i2cRQ[2];
    i2cRQ[0] = reg;
    i2cRQ[1] = value;
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
	;
    if (HAL_I2C_Master_Transmit(hi2c, AIC3254_ADDR, &i2cRQ[0], AIC3254_RQ_SIZE,
	    1000) != HAL_OK)
	{
	Error_Handler();
	}
    }

void AIC3254::codecRegWriteP(uint8_t page, uint8_t reg, uint8_t value)
    {
    codecRegWrite(0x00, page);
    codecRegWrite(reg, value);
    }

void AIC3254::codecRegWriteSq(const aic3254Reg *reg_pairs, uint16_t byte_size)
    {
    uint16_t word_size = byte_size / 2;
    for (uint16_t i = 0; i < word_size; i++)
	{
	codecRegWrite(reg_pairs[i].regOffset, reg_pairs[i].regVal);
	}
    }

uint8_t AIC3254::codecRegRead(uint8_t reg)
    {
    uint8_t value = 0;
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
	;
    if (HAL_I2C_Master_Transmit(hi2c, AIC3254_ADDR, &reg, sizeof(reg), 1000)
	    != HAL_OK)
	{
	Error_Handler();
	}
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
	;
    if (HAL_I2C_Master_Receive(hi2c, AIC3254_ADDR, &value, sizeof(value), 1000)
	    != HAL_OK)
	{
	Error_Handler();
	}
    return value;

    }

void AIC3254::codecSetup()
    {
    codecRegWriteSq(&c_Reset[0], sizeof(c_Reset));
    HAL_Delay(100);
    codecRegWriteSq(&c_ClockInitSequence[0], sizeof(c_ClockInitSequence));
    codecRegWriteSq(&c_PowerUpSequence[0], sizeof(c_PowerUpSequence));
    codecRegWriteSq(&c_ADCStartupMute[0], sizeof(c_ADCStartupMute));
    codecRegWriteSq(&c_DACStartup[0], sizeof(c_DACStartup));
    }

void AIC3254::codecSetVolume(int8_t volume)
    {
    codecRegWriteP(0, 65, ((volume > 48) ? 48 : volume));

    }

void AIC3254::codecSetInpGain(uint8_t gain)
    {
    }

void AIC3254::codecMute(uint8_t channel)
    {
    if ((CH_OUT_L | CH_OUT_R) & channel)
	{
	codecRegWriteP(0, AIC3254_MUTE_DAC_REG, AIC3254_MUTE_DAC);
	}
    if ((CH_IN_L | CH_IN_R) & channel)
	{
	codecRegWriteP(0, AIC3254_MUTE_ADC_REG, AIC3254_MUTE_ADC);
	}

    }

void AIC3254::codecUnmute(uint8_t channel)
    {
    if ((CH_OUT_L | CH_OUT_R) & channel)
	{
	codecRegWriteP(0, AIC3254_MUTE_DAC_REG, 0x00);
	}
    if ((CH_IN_L | CH_IN_R) & channel)
	{
	codecRegWriteP(0, AIC3254_MUTE_ADC_REG, 0x00);
	}

    }

void AIC3254::codecReadFault(uint8_t *sticky1, uint8_t *sticky2,
	uint8_t *sticky3)
    {
    codecRegWrite(0, 0);
    *sticky1 = codecRegRead(42);
    *sticky2 = codecRegRead(44);
    *sticky3 = codecRegRead(45);
    }

void AIC3254::codecReadDACStatus(uint8_t *status1, uint8_t *status2)
    {
    codecRegWrite(0, 0);
    *status1 = codecRegRead(37);
    *status2 = codecRegRead(38);
    }

