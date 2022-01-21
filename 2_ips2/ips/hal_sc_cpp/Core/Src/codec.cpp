/*
 * codec.c
 *
 *  Created on: 18 июн. 2020 г.
 *      Author: Donskikh Dmitry
 */

/*
 * codec.cpp - a part of CMppCore -an object engine for Cortex-M
 * microcontrollers.
 *
 * Copyright (c) 2019-2020 by Dmitry Donskih
 * All rights reserved.
 *
 * Attention Software modification allowed only with
 * the permission of the author.
 */

#include "codec.h"
#include "main.h"

#define AIC3254_ADDR	0x30

#define AIC3254_RQ_SIZE	0x02

//static const aic3254Reg c_Reset[] =
//  {
//    { 0x00, 0x00 },
//    { 0x01, 0x01 }, };

//static const aic3254Reg c_ClockInitSequence[] =
//  {
//      {0x0, 0x0},
//          //Page 0 / Register 4: Clock Setting Register 1, Multiplexers - 0x00 / 0x04 (P0_R4)
//          //11: PLL Clock is CODEC_CLKIN
//          {0x4, 0x3},
//          //001: R = 1
//          //001: P = 1
//          //1: PLL is powered up
//          {0x5, 0x91},
//          //J = 8
//          {0x6, 0x8},
//          //Page 0 / Register 7: Clock Setting Register 4, PLL D Values (MSB) - 0x00 / 0x07 (P0_R7)
//          //00 0000 0000 0000: D = 0000
//          {0x7, 0x0},
//          //Page 0 / Register 8: Clock Setting Register 5, PLL D Values (LSB) - 0x00 / 0x08 (P0_R8)
//          //00 0000 0000 0000: D = 0000
//          {0x8, 0x0},
//          //NDAC = 2, MDAC = 8, dividers powered on
//          {0xb, 0x81},
//          {0xc, 0x84},
//          //DOSR = 128
//          {0xd, 0x00},
//          {0xe, 0x80},
//          //NADC = 2, MADC = 8, dividers powered on
//          {0x12, 0x81},
//          {0x13, 0x84},
//          //AOSR = 128
//          //1000 0000: AOSR = 128 (Use with PRB_R1 to PRB_R6, ADC Filter Type A) !!!
//          {0x14, 0x80},
//  };
//
//static const aic3254Reg c_PowerUpSequence[] =
//  {
//      //Configure Power Supplies
//         { 0x0, 0x1 },
//         //Page 1 / Register 2: LDO Control Register - 0x01 / 0x02 (P1_R2)
//         //1: AVDD LDO Powered up
//         //1: Analog Blocks Disabled
//         //# Power up AVDD LDO
//         { 0x2, 0x9 },
//
//         //Page 1 / Register 1: Power Configuration Register - 0x01 / 0x01 (P1_R1)
//         //1: Disabled weak connection of AVDD with DVDD
//         { 0x1, 0x8 },
//
//         //# Enable Master Analog Power Control
//         //# Power up AVDD LDO
//         { 0x2, 0x1 },
//
//         //# Set full chip common mode to 0.9V
//         //# HP output CM = 1.65V
//         //# HP driver supply = LDOin voltage
//         //# Line output CM = 1.65V
//         //# Line output supply = LDOin voltage
//         { 0x0A, 0x3B },
//         //#
//         //# Select ADC PTM_R4
//         { 0x3d, 0x00},
//         //w 30 3d 00
//         //#
//         //# Select DAC PTM_P3/4
//         { 0x03, 0x00 },
//         { 0x04, 0x00 },
//
//
//         //# Set the input power-up time to 3.1ms (for ADC)
//         { 0x47, 0x32 },
//         //# Set the REF charging time to 40ms
//         { 0x7b, 0x01 },
//  };
//
//static const aic3254Reg c_ADCStartupMute[] =
//  {
//      //Select Page 1
//          {0x00, 0x01},
//          //Route IN2L to LEFT_P with 10K input impedance
//          {0x34, 0x10},
//          //Route IN2R to LEFT_M with 10K input impedance
//          {0x36, 0x10},
//          //Route IN1R to RIGHT_P with 10K input impedance
//          {0x37, 0x40},
//          //Route IN1L to RIGHT_M with 10K input impedance
//          {0x39, 0x10},
//          //Unmute Left MICPGA, Gain selection of 6dB to
//          //make channel gain 0dB, since 20K input
//          //impedance is usedsingle ended
//          //101 1111: Volume Control = 47.5dB
//          //{0x3b, 0x5f},
//
//          //Unmute Right MICPGA, Gain selection of 6dB to
//          //make channel gain 0dB, since 20K input
//          //impedance is usedsingle ended
//          //{0x3c, 0x5f},
//
//          //micbias
//          //10: MICBIAS = 2.075V(CM = 0.75V) or MICBIAS = 2.5V(CM = 0.9V)
//          //1: MICBIAS powered up
//          //1: MI2CBIAS voltage is generated from LDOIN
//          {0x33, 0x58},
//
//          //Select Page 0
//          {0x00, 0x00},
//          //Power up LADC/RADC
//          {0x51, 0xc0},
//          //Unmute LADC/RADC
//          {0x52, 0x00},
//
//          //mute LADC/RADC
////          {0x52, 0x88},
//	  };
//
//static const aic3254Reg c_DACStartup[] =
//  {
//      {0x00, 0x01},
//         //Page 1 / Register 10: Common Mode Control Register - 0x01 / 0x0A (P1_R10)
//         //: When Page-1, Reg-10, D1 = 1, then LDOIN input range is 1.8V to 3.6V
//         //Output Common Mode for LOL and LOR is 1.65V and output is powered by LDOIN
//
//         {0xa, 0xb},
//         // Route LDAC/RDAC to LOL/LOR
//         {0x0e, 0x08},
//         {0x0f, 0x08},
//         // Powerup HPL/HPR and LOL/LOR drivers
//         {0x09, 0x0C},
//         // Unmute LOL/LOR driver, 0dB Gain
//         {0x12, 0x00},//1c
//         {0x13, 0x00},
//         // Select Page 0
//         {0x00, 0x00},
//         // DAC => 0dB
//         {0x41, 0x10},
//         {0x42, 0x10},
//         // Powerup LDAC/RDAC
//         {0x3f, 0xd4},
//         // UnmuteLDAC/RDAC
//         {0x40, 0x00},
//  };
//
//static const aic3254Reg adaptiveDAFilter[] =
//  {
//      {0x00, 8},
//      {0x00, 0x5},
//
//      {0x00, 44},
//      {0x00, 0x5},
//  };


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
//              //Route IN2L to LEFT_P with 10K input impedance
//    {0x34, 0x10},
//              //Route IN2R to LEFT_M with 10K input impedance
//    {0x36, 0x10},

    { 55, 0x40 },//    { 55, 0xC0 },
    { 57, 0x10 },//    { 57, 0x30 },
    { 0, 0x00 },
    { 81, 0x40 }, 								///Left ADC Powerup
//    { 82, 0x88 },
    { 82, 0x00 },
    /*
     * Setup Right Channel AGC (default) -5.5 dbfs
     */
    { 94, 0b11110011 }, //0x80 //max at 0xF3
};

static const aic3254Reg c_DACStartup[] =
{
    { 0, 0x01 },
    { 10, 0x03 },
    { 14, 0x08 }, //{ 14, 0x10 },
    { 9, 0x0C },
    { 18, 0x00 },
    { 19, 0x00 },
    { 0, 0x00 },
    { 65, 0x10 },
    { 63, 0xFC },
//    { 63, 0xB0 },
//    { 63, 0x90 },
    { 64, 0x00 },
};
// @formatter:on

extern I2C_HandleTypeDef hi2c2;

/*!
 *
 * @param reg
 * @param value
 */
void codecRegWrite (uint8_t reg, uint8_t value)
{
  uint8_t i2cRQ[2];
  i2cRQ[0] = reg;
  i2cRQ[1] = value;
  while (HAL_I2C_GetState (&hi2c2) != HAL_I2C_STATE_READY);
  if (HAL_I2C_Master_Transmit (&hi2c2, AIC3254_ADDR, &i2cRQ[0], AIC3254_RQ_SIZE, 1000) != HAL_OK)
    {
      Error_Handler ();
    }
}
/*!
 *
 * @param page
 * @param reg
 * @param value
 */
void codecRegWriteP (uint8_t page, uint8_t reg, uint8_t value)
{
  codecRegWrite (0x00, page);
  codecRegWrite (reg, value);
}
/**
 *
 * @param reg_pairs
 * @param byte_size
 */
void codecRegWriteSq (aic3254Reg *reg_pairs, uint16_t byte_size)
{
  uint16_t word_size = byte_size / 2;
  for (uint16_t i = 0; i < word_size; i++)
    {
      codecRegWrite (reg_pairs[i].regOffset, reg_pairs[i].regVal);
    }
}
/**
 *
 * @param reg
 * @return
 */
uint8_t codecRegRead (uint8_t reg)
{
  uint8_t value = 0;
  while (HAL_I2C_GetState (&hi2c2) != HAL_I2C_STATE_READY);
  if (HAL_I2C_Master_Transmit (&hi2c2, AIC3254_ADDR, &reg, sizeof(reg), 1000) != HAL_OK)
    {
      Error_Handler ();
    }
  while (HAL_I2C_GetState (&hi2c2) != HAL_I2C_STATE_READY);
  if (HAL_I2C_Master_Receive (&hi2c2, AIC3254_ADDR, &value, sizeof(value), 1000) != HAL_OK)
    {
      Error_Handler ();
    }
  return value;

}
/*
 *
 */
void codecSetup ()
{

  codecRegWriteSq ((aic3254Reg*) &c_Reset, sizeof(c_Reset));
  HAL_Delay (100);
  codecRegWriteSq ((aic3254Reg*) &c_ClockInitSequence, sizeof(c_ClockInitSequence));
  codecRegWriteSq ((aic3254Reg*) &c_PowerUpSequence, sizeof(c_PowerUpSequence));
  codecRegWriteSq ((aic3254Reg*) &c_ADCStartupMute, sizeof(c_ADCStartupMute));
  codecRegWriteSq ((aic3254Reg*) &c_DACStartup, sizeof(c_DACStartup));
//  codecRegWriteP (0, 84, 0x0A);//right adc gain
//  codecRegWriteP (0, 83, 0x0A);//left adc gain
  codecSetOutVolume(0b11111000);
}
/**
 *
 * @param volume
 */
void codecSetOutVolume (int8_t volume)
{
  codecRegWriteP (0, 65, ((volume > 48) ? 48 : volume));

}
/*!
 *
 * @param volume
 */
void codecSetInVolume (int8_t volume)
{
  if(volume<(-48))
    {
      volume=-48;
    }
  else if(volume>80)
    {
      volume=80;
    }
  codecRegWriteP(0,83,volume>>1);
  codecRegWriteP(0,84,volume>>1);
}
/*!
 *
 * @param channel
 */
void codecMute (uint8_t channel)
{
  if ((CH_OUT_L | CH_OUT_R) & channel)
    {
      codecRegWriteP (0, AIC3254_MUTE_DAC_REG, AIC3254_MUTE_DAC);
    }
  if ((CH_IN_L | CH_IN_R) & channel)
    {
      codecRegWriteP (0, AIC3254_MUTE_ADC_REG, AIC3254_MUTE_ADC);
    }

}
/*!
 *
 * @param channel
 */
void codecUnmute (uint8_t channel)
{
  if ((CH_OUT_L | CH_OUT_R) & channel)
    {
      codecRegWriteP (0, AIC3254_MUTE_DAC_REG, 0x00);
    }
  if ((CH_IN_L | CH_IN_R) & channel)
    {
      codecRegWriteP (0, AIC3254_MUTE_ADC_REG, 0x00);
    }

}
/*!
 *
 * @param sticky1
 * @param sticky2
 * @param sticky3
 */
void codecReadFault (uint8_t *sticky1, uint8_t *sticky2, uint8_t *sticky3)
{
  codecRegWrite (0, 0);
  *sticky1 = codecRegRead (42);
  *sticky2 = codecRegRead (44);
  *sticky3 = codecRegRead (45);
}
/*!
 *
 * @param status1
 * @param status2
 */
void codecReadDACStatus (uint8_t *status1, uint8_t *status2)
{
  codecRegWrite (0, 0);
  *status1 = codecRegRead (37);
  *status2 = codecRegRead (38);
}

void codecMicMute (bool mute)
{
  if (mute)
    {
      codecRegWriteP (0, AIC3254_MUTE_ADC_REG, 0x88);
    }
  else
    {
      codecRegWriteP (0, AIC3254_MUTE_ADC_REG, 0x00);
    }
}

void codecAmpMute (bool mute)
{
  if (mute)
    {
      codecRegWriteP (0, AIC3254_MUTE_DAC_REG, AIC3254_MUTE_DAC);
    }
  else
    {
      codecRegWriteP (0, AIC3254_MUTE_DAC_REG, 0x02);
    }
}

void MicMuteToggle ()
{
  codecRegWrite (0, 0);
  codecMicMute((codecRegRead(AIC3254_MUTE_ADC_REG)&0x88)==0);
}

void AmpMuteToggle ()
{
  codecRegWrite (0, 0);
  codecAmpMute((codecRegRead(AIC3254_MUTE_DAC_REG)&0x0C)==0);
}

void VolPlus ()
{
  codecSetOutVolume(codecRegRead(65)+1);
}

void VolMinus ()
{
  codecSetOutVolume(codecRegRead(65)-1);
}
