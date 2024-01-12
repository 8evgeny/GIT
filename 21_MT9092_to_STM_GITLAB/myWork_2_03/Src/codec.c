/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   codec.c
 * Author: Роман
 * 
 * Created on 9 января 2021 г., 21:38
 */
// используемый кодек TLV320AIC3254 (Texas Instruments)

#include "codec.h"
#include "main.h"
#include "uart.h"

extern SPI_HandleTypeDef hspi1;

uint16_t tmp[48] = {0x0000,0xFFFF}, tmp2[2];

static void writeReg(unsigned char reg, unsigned char data) {
  
  reg = reg << 1; // 7-1 bits - address, 0 bit - r/w (0 - write, 1 - read)
  
  HAL_GPIO_WritePin(nSS_GPIO_Port, nSS_Pin, GPIO_PIN_RESET); // CS On
  
  HAL_SPI_Transmit(&hspi1, &reg, 1, 10);
  HAL_SPI_Transmit(&hspi1, &data, 1, 10);

  HAL_GPIO_WritePin(nSS_GPIO_Port, nSS_Pin, GPIO_PIN_SET); // CS Off
}

static unsigned char readReg(unsigned char reg) {
  unsigned char rec = 0;

  reg = (reg << 1) | 0x01; // 7-1 bits - address, 0 bit - r/w (0 - write, 1 - read)

  HAL_GPIO_WritePin(nSS_GPIO_Port, nSS_Pin, GPIO_PIN_RESET); // CS On
  
  HAL_SPI_Transmit(&hspi1, &reg, 1, 10);
  HAL_SPI_Receive(&hspi1, &rec, 1, 10);

  HAL_GPIO_WritePin(nSS_GPIO_Port, nSS_Pin, GPIO_PIN_SET); // CS Off
  
  return rec;
}

void selectGoosneckMic(void) { // включаем микрофон на IN1R (гусак)
	//# Select Page 1
	writeReg(0x00, 0x01);
	
	//# Route IN1R to RIGHT_P with 20K input impedance
	writeReg(0x37, 0x80);

	writeReg(0x3c, MIC_PGA_GAIN * 2); // усиление микрофонного усилиителя
}

void selectHeadsetMic(void) { // включаем микрофон на IN3R (гарнитура)
	//# Select Page 1
	writeReg(0x00, 0x01);

	//# Route IN1R to RIGHT_P with 20K input impedance
	writeReg(0x37, 0x08);

	writeReg(0x3c, HEADSET_MIC_PGA_GAIN * 2); // усиление микрофонного усилиителя
}

void headsetSpeakerOn(void) { // включает выход на наушники (гарнитуру) (HPR/HPL driver)
    uartPuts("headsetSpeakerOn\r\n");
  //# Select Page 1
  writeReg(0x00, 0x01);
	//# Unmute HPL/HPR driver, 1dB Gain
  writeReg(0x10, readReg(0x10) & (~0x40));
  writeReg(0x11, readReg(0x11) & (~0x40));
}

void headsetSpeakerOff(void) { // выключает выход на наушники (гарнитуру) (HPR/HPL driver)
    uartPuts("headsetSpeakerOff\r\n");
  //# Select Page 1
  writeReg(0x00, 0x01);
	//# Mute HPL/HPR driver
  writeReg(0x10, readReg(0x10) | 0x40);
  writeReg(0x11, readReg(0x11) | 0x40);
}

void lineOutSpeakerOn(void) { // включает линейный выход (LOL/LOR Driver)
  uartPuts("lineOutSpeakerOn\r\n");
  //# Select Page 1
  writeReg(0x00, 0x01);
  //# Unmute LOL/LOR driver
  writeReg(0x12, readReg(0x12) & (~0x40));
  writeReg(0x13, readReg(0x13) & (~0x40));
}

void lineOutSpeakerOff(void) { // выключает линейный выход (LOL/LOR Driver)
  uartPuts("lineOutSpeakerOff\r\n");
  //# Select Page 1
  writeReg(0x00, 0x01);
  //# Mute LOL/LOR driver
  writeReg(0x12, readReg(0x12)| 0x40);
  writeReg(0x13, readReg(0x13)| 0x40);
}

// выбирает, какой микрофон подключить к Right ADC
// mic = 0 - микрофон гарнитуры (диф. пара IN3_R, IN3_L)
// mic = 1 - встроенный микрофон (диф. пара IN1_R, IN1_L)
void selectMic(uint8_t mic) {

  switch(mic) {
    case 0:
      uartPuts("selectHeadsetMic\r\n");
      //# Select Page 1
      writeReg(0x00, 0x01);
      //# Route IN3R to RIGHT_P with 10K input impedance
      //w 30 37 80
      writeReg(0x37, 0x04);
      //  writeReg(0x39, 0x80);
      //# Route IN3L to RIGHT_M with 10K input impedance (дифф. режим)
      writeReg(0x39, 0x04);

      writeReg(0x3c, HEADSET_MIC_PGA_GAIN * 2); // усиление микрофонного усилиителя
      break;
    case 1:
      uartPuts("selectLoudspeakerMic\r\n");
      //# Select Page 1
      writeReg(0x00, 0x01);  
      //# Route IN1R to RIGHT_P with 10K input impedance
      //w 30 37 80
      writeReg(0x37, 0x40);
      //  writeReg(0x39, 0x80);
      //# Route IN1L to RIGHT_M with 10K input impedance (дифф. режим)
      writeReg(0x39, 0x10);
      
      writeReg(0x3c, MIC_PGA_GAIN * 2); // усиление микрофонного усилиителя
      break;
  }
}
// включает микрофон
void micOn(void) {
  uartPuts("Mic On\r\n");
  
  //# Select Page 0
  writeReg(0x00, 0x00);
  
  // Right ADC Channel Muted
  writeReg(0x52, readReg(0x52) & (~0x08));
}
// выключает микрофон
void micOff(void) {
  uartPuts("Mic Off\r\n");
  
  //# Select Page 0
  writeReg(0x00, 0x00);

  // Right ADC Channel Un-Muted
  writeReg(0x52, readReg(0x52) | 0x08);
}
// устанавливает усиление микрофона 0..47,5 дБ
// gain соответственно от 0 до 95
void setPGAgain(unsigned char gain) {
  //# Select Page 1
  //w 30 00 01
  writeReg(0x00, 0x01);
  //# Unmute Left MICPGA, Gain selection of 6dB to
  //# make channel gain 0dB, since 20K input
  //# impedance is used single ended
  //w 30 3b 0c
  writeReg(0x3b, gain); // усиление микрофонного усилиителя
  //# Unmute Right MICPGA, Gain selection of 6dB to
  //# make channel gain 0dB, since 20K input
  //# impedance is used single ended
  //w 30 3c 0c
  writeReg(0x3c, gain); // усиление микрофонного усилиителя
}

// инициализирует кодек
// возвращает
// 0, если OK
// 1, если ошибка
uint8_t codecInit(void) {
	//###############################################
	//# Software Reset
	//###############################################
	//#
	//# Select Page 0
	//w 30 00 00
	writeReg(0x00, 0x00);
	//# Initialize the device through software reset
	//w 30 01 01
	writeReg(0x01, 0x01);
	//###############################################
	//
	//
	//
	//###############################################
	//# Clock and Interface Settings
  // подробнее см. стр. 74-75 "Aplication reference guide"
	// ПРИ НАЛИЧИИ MCLK!!!!!
	//# ---------------------------------------------
	//# The codec receives: MCLK = 2.048 MHz,
	//# BLCK = 256 kHz, WCLK = 8 kHz
	//###############################################
	//#
	//# Select Page 0
	writeReg(0x00, 0x00);

  //# PLL_clkin = MCLK, codec_clkin = PLL_CLK,
  //# PLL on, P=1, R=1, J=48, D=0000
  writeReg(TI3254_CLK_MUX_REG, 0x03);
  writeReg(TI3254_CLK_PLL_P_R_REG, 0x91);
  writeReg(TI3254_CLK_PLL_J_REG, 0x00 + 48);
  writeReg(TI3254_CLK_PLL_D_MSB_REG, 0x00);
  writeReg(TI3254_CLK_PLL_D_LSB_REG, 0x00);

	//# NDAC = 2, MDAC = 48, dividers powered on
	writeReg(TI3254_CLK_NDAC_REG, 0x82);
	writeReg(TI3254_CLK_MDAC_REG, 0x80+48);
  
	//# DOSR = 128
	writeReg(TI3254_DAC_OSR_MSB_REG, 0x00);
	writeReg(TI3254_DAC_OSR_LSB_REG, 0x80);
  
  //# NADC = 2, MADC = 48, dividers powered on
  writeReg(TI3254_CLK_NADC_REG, 0x82);
  writeReg(TI3254_CLK_MADC_REG, 0x80+48);
	//# AOSR = 128
	//w 30 14 80
	writeReg(TI3254_ADC_OSR_REG, 0x80);
	//###############################################
	//
	//
	//
	//###############################################
	//# Configure Power Supplies
	//###############################################
	//#
	//# Select Page 1
	//w 30 00 01
	writeReg(0x00, 0x01);
	//# Disable weak AVDD in presence of external
	//# AVDD supply
	//w 30 01 08
	writeReg(0x01, 0x08);
	//# Enable Master Analog Power Control
	//w 30 02 00
	writeReg(0x02, 0x01);
	//# Set the input power-up time to 3.1ms (for ADC)
	//w 30 47 32
	writeReg(0x47, 0x32);

	// проверяем, что у нас записалось
	if (readReg(0x47) != 0x32) return 1; // возвращаем ошибку - неисправен SPI

	//# Set the REF charging time to 40ms
	//w 30 7b 01
	writeReg(0x7b, 0x01);
	//###############################################
	//
	//
	//
	//###############################################
	//# Configure Processing Blocks
	//###############################################
	//#
	//# Select Page 0
	//w 30 00 00
	writeReg(0x00, 0x00);
	//# PRB_P2 and PRB_R2 selected
	//w 30 3C 02 02
	writeReg(0x3c, 0x02);
	writeReg(0x3d, 0x02);
	//###############################################
	//
	//
	//
	//###############################################
	//# Configure ADC Channel
	//###############################################
	//#
	//# Select Page 1
	//w 30 00 01
	writeReg(0x00, 0x01);
	// включаем MICBIAS на 2,5V
	writeReg(0x33, 0x68);
  
  
//	//# Route IN1L to LEFT_P with 20K input impedance
//	//w 30 34 80
//	writeReg(0x34, 0x80);
//	//# Route CM1L to LEFT_M with 20K input impedance
//	//w 30 36 80
//	writeReg(0x36, 0x80);
  // LEFT ADC not used
  writeReg(0x34, 0x00);
  writeReg(0x36, 0x00);
  
	//# Route IN1R to RIGHT_P with 10K input impedance
	//w 30 37 80
	writeReg(0x37, 0x40);
	//  writeReg(0x39, 0x80);
	//# Route IN1L to RIGHT_M with 10K input impedance (дифф. режим)
	writeReg(0x39, 0x10);
  
  
//	//# Unmute Left MICPGA, Gain selection of 6dB to
//	//# make channel gain 0dB, since 20K input
//	//# impedance is used single ended
//	//w 30 3b 0c
//	writeReg(0x3b, 0x0c);
  
  // Left MICPGA Mutted (not used)
	writeReg(0x3b, 0x80);
  
  
	//# Unmute Right MICPGA, Gain selection of 6dB to
	//# make channel gain 0dB, since 20K input
	//# impedance is used single ended
	//w 30 3c 0c
	writeReg(0x3c, MIC_PGA_GAIN * 2); // усиление микрофонного усилиителя
  
	//# Select Page 0
	//w 30 00 00
	writeReg(0x00, 0x00);
//	//# Power up LADC/RADC
//	//w 30 51 c0
//	writeReg(0x51, 0xc0);
  
  // # Power up RADC, LADC power off
	writeReg(0x51, 0x40);
	//# Unmute LADC/RADC
	//w 30 52 00
//	writeReg(0x52, 0x00);
	//# Unmute RADC, LADC mutted
	//w 30 52 00
	writeReg(0x52, 0x80);
	//###############################################
	//
	//
	//
	//###############################################
	//# Configure DAC Channel
	//###############################################
	//#
	//# Select Page 1
	//w 30 00 01
	writeReg(0x00, 0x01);
	//# De-pop
	//w 30 14 25
	writeReg(0x14, 0x25);
	//# Route LDAC/RDAC to HPL/HPR
	//w 30 0c 08 08
	writeReg(0x0c, 0x08);
	writeReg(0x0d, 0x08);
	//# Route LDAC/RDAC to LOL/LOR
	//w 30 0e 08 08
	writeReg(0x0e, 0x08);
	writeReg(0x0f, 0x08);
	//# Power up HPL/HPR and LOL/LOR drivers
	//w 30 09 3C
	writeReg(0x09, 0x3c);
	//# Unmute HPL/HPR driver, 1dB Gain
	//w 30 10 00 00
	writeReg(0x10, 0x01);
	writeReg(0x11, 0x01);
	//# Unmute LOL/LOR driver, set Gain
	//w 30 12 00 00
	writeReg(0x12, 0x00 | LOL_DRIVER_GAIN & 0x3F);
	writeReg(0x13, 0x00 | LOR_DRIVER_GAIN & 0x3F);
	//# Select Page 0
	//w 30 00 00
	writeReg(0x00, 0x00);
	//# DAC => 0dB
	//w 30 41 00 00
	writeReg(0x41, DAC_VOLUME_L);
	writeReg(0x42, DAC_VOLUME_R);
	//# Power up LDAC/RDAC
	//w 30 3f d6
	writeReg(0x3f, 0xd6);
	//# Unmute LDAC/RDAC
	//w 30 40 00
	writeReg(0x40, 0x00);

	/////////////

	writeReg(0x00, 0x00);
	writeReg(TI3254_DAC_SIG_P_BLK_CTRL_REG, 0x02);


	writeReg(0x00, 0x2C);
	writeReg(TI3254_DAC_ADP_FILTER_CTRL_REG, 0x04);

	return 0; // OK

}

void doCodecTest(void) {
	while(1) {
		HAL_Delay(100);

		writeReg(0x10, 0x01);
		
//		if (readReg(0x10) == 0x01) TEST_TOG();
	}
}
