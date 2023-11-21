/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   codec_TLV320AIC3100.c
 * Author: roman.ganaza
 * 
 * Created on July 4, 2023, 11:21 AM
 */
// используемый кодек TLV320AIC3100 (Texas Instruments)

#include "main.h"
#include "dbg_uart.h"
#include "codec_TLV320AIC3100.h"
#include "main_logic.h"


#define USED_I2C        hi2c1

#define AIC3100_ADDR      (0x18)

#define RESET_LOW()         HAL_GPIO_WritePin(nAIC_RES_GPIO_Port, nAIC_RES_Pin, GPIO_PIN_RESET)
#define RESET_HIGH()         HAL_GPIO_WritePin(nAIC_RES_GPIO_Port, nAIC_RES_Pin, GPIO_PIN_SET)


extern I2C_HandleTypeDef USED_I2C;

uint8_t flag_codec_init_err = 0;
uint8_t selected_mic = LOUDSPEAKER_MIC;
uint32_t vol_timer;


static void writeReg(unsigned char reg, unsigned char data) {
  HAL_StatusTypeDef err;

  err = HAL_I2C_Mem_Write(&USED_I2C, AIC3100_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
  if (err == HAL_OK) {
    //printf("writeReg: REG_0x%02x = 0x%02x\r\n", (unsigned int)reg, (unsigned int)data);
  } else {
    printf("writeReg: FAILED, err_code = %u\r\n", err);
    flag_codec_init_err = 1;
  }
}

static unsigned char readReg(unsigned char reg) {
  HAL_StatusTypeDef err;
  unsigned char data;

  err = HAL_I2C_Mem_Read(&USED_I2C, AIC3100_ADDR << 1, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
  if (err == HAL_OK) {
    //printf("readReg: REG_0x%02x = 0x%02x\r\n", (unsigned int)reg, (unsigned int)data);
  } else {
    printf("readReg: FAILED, err_code = %u\r\n", err);
    flag_codec_init_err = 1;
  }
  return data;
}
// инициализирует кодек
// возвращает
// 0, если OK
// 1, если ошибка
uint8_t codecInit(void) {

  flag_codec_init_err = 0;
  printf("Codec init...");

  RESET_LOW();
  HAL_Delay(50);
  RESET_HIGH();

  writeReg(PAGE_SELECT, 5);
  if (readReg(PAGE_SELECT) != 5) {
    flag_codec_init_err = 1;
    puts("ERROR: codecInit failed!");
    return 1; // ошибка
  }

  writeReg(PAGE_SELECT, PAGE0);
  
  writeReg(0x0B, 0x82); // NDAC_CLK_REG
  writeReg(0x0C, 0x81); // MDAC_CLK_REG
  writeReg(0x0D, 0x01); // DAC_OSR_MSB
  writeReg(0x0E, 0x00); // DAC_OSR_LSB
  
  writeReg(0x12, 0x82); // NADC_CLK_REG
  writeReg(0x13, 0x82); // MADC_CLK_REG
  writeReg(0x14, 0x80); // ADC_OSR_REG

  writeReg(0x1E, 0x84); // BCLK_N_VAL
  writeReg(0x3C, 0x01); // DAC_PRB_SEL_REG
  writeReg(0x3D, 0x04); // ADC_PRB_SEL_REG
  writeReg(0x3F, 0xD4); // DAC_CHN_REG
  writeReg(0x43, 0xD3); // HEADSET_DETECT
  writeReg(0x56, 0x00); // AGC_CTRL_1
  writeReg(0x57, 0xFE); // AGC_CTRL_2
  writeReg(0x58, 0x42); // AGC_CTRL_3
  writeReg(0x59, 0x20); // AGC_CTRL_4
  writeReg(0x5A, 0x38); // AGC_CTRL_5
  writeReg(0x5B, 0x00); // AGC_CTRL_6
  writeReg(0x5C, 0x00); // AGC_CTRL_7
  writeReg(0x5D, 0x80); // AGC_CTRL_8
  writeReg(0x51, 0x82); // ADC_DIG_MIC
  

  // page 1
  writeReg(PAGE_SELECT, PAGE1);
  writeReg(0x1F, 0xC4); // HEADPHONE_DRIVER
  writeReg(0x20, 0x86); // CLASSD_SPEAKER_AMP
  writeReg(0x21, 0x18); // HP_POP_CTRL
  writeReg(0x23, 0x44); // DAC_MIX_CTRL
  writeReg(0x24, 0x80); // L_ANLOG_VOL_2_HPL
  writeReg(0x25, 0x80); // R_ANLOG_VOL_2_HPR
  writeReg(0x28, 0x42); // HPL_DRIVER
  writeReg(0x29, 0x42); // HPR_DRIVER
  writeReg(0x2A, 0x10); // SPL_DRIVER
  writeReg(0x2E, 0x0A); // MICBIAS_CTRL
  writeReg(0x2F, 0x42); // MIC_PGA
  writeReg(0x31, 0x80); // ADC_IP_SEL

  ///////////////////////////////// 

  // включаем динамик
  writeReg(PAGE_SELECT, PAGE0); // page select
  writeReg(0x40, 0x00); // DAC_MUTE_CTRL_REG
  writeReg(0x41, 0x00); // LDAC_VOL
  writeReg(0x42, 0x00); // RDAC_VOL

  writeReg(PAGE_SELECT, PAGE1); // page select
  writeReg(37, 0x80); // Right Analog Volume to HPR
  writeReg(38, 0x80); // Left Analog Volume to SPK
  writeReg(HPR_DRIVER, 0x47); // HPR_DRIVER (динамик трубки)
  writeReg(SPL_DRIVER, 0x04); // SPK DRIVER (динамик громкой связи)

  //////////////////////////////////

  // включаем микрофон
  writeReg(PAGE_SELECT, PAGE0); // page select
  writeReg(0x52, 0x00); // ADC Digital Volume Control Fine Adjust, ADC unmuted
  writeReg(0x53, 0x00);

  // ADC Input Selection for P-Terminal
  writeReg(PAGE_SELECT, PAGE1); // page select
  writeReg(0x30, 0x80); // MIC1LP is selected for the MIC PGA with feed-forward resistance RIN = 20 kΩ

  // ADC Input Selection for M-Terminal
  writeReg(0x31, 0x80); // CM is selected for the MIC PGA with feed-forward resistance RIN = 20 kΩ.
  writeReg(0x32, 0x00); // MIC1LP input is floating

  writeReg(PAGE_SELECT, PAGE0); // page select
//  writeReg(0x56, 0x00); // АРУ выкл
  writeReg(0x56, 0x80); // вкл АРУ, -5,5 dB
  
  selectLoudspeakMic();
  muteLoudspeaker();
  muteHlt();

  if (flag_codec_init_err) {
    vol_timer = 0xFFFFFFFF;
    puts("ERROR: codecInit failed!");
    return 1; // ошибка
  } else {
    vol_timer = HAL_GetTick() + 5000;
    puts("OK");
    return 0;
  }
}

void setSpeakerVol(uint16_t vol) {
  uint16_t dB;
  static uint8_t prev_percent;
  static uint8_t percent;

  percent = 100u * (vol + 5) / 4095;
  if (prev_percent != percent) {
    if (HAL_GetTick() > vol_timer) {
      printf("Speaker Vol: %u%%\r", percent);
    }
    prev_percent = percent;
  }
  
  vol = 4095u - vol; // регулируется, по-факту, ослабление
  
  dB = 0x50 * vol / 4095u; // 0..4095 -> 0..0x50
  
  writeReg(PAGE_SELECT, PAGE1);
  writeReg(L_ANLOG_VOL_2_SPL, dB | 0x80); // старший бит = 1 ( Left-channel analog volume control output is routed to mono class-D output driver)
}

void setMicVol(uint16_t vol, uint8_t sel_mic) {
  uint16_t dB;
  static uint8_t prev_percent[2];
  static uint8_t percent;
  
  percent = 100u*(vol+5)/4095;
  if (prev_percent[sel_mic] != percent) {
    if (HAL_GetTick() > vol_timer) {
      if (sel_mic == LOUDSPEAKER_MIC) {
        printf("Mic Vol: %u%%\r", percent);
      } else if (sel_mic == HANDSET_MIC) {
        printf("Hmic Vol: %u%%\r", percent);
      }
    }
    prev_percent[sel_mic] = percent;
  }

  dB = 0x14 + (0x52 - 0x14) * vol / 4095u;
//  dB = 0x68 * (0x52 - 0x14) * vol / 4095u;

  writeReg(PAGE_SELECT, PAGE0);
  writeReg(AGC_MAX_GAIN, dB);
//  writeReg(83, dB);
}

void setHltVol(uint16_t vol) {
  uint16_t dB;
  static uint8_t prev_percent;
  static uint8_t percent;

  percent = 100u * (vol + 5) / 4095;
  if (prev_percent != percent) {
    if (HAL_GetTick() > vol_timer) {
      printf("Handset Vol: %u%%\r", percent);
    }
    prev_percent = percent;
  }

  vol = 4095u - vol; // регулируется, по-факту, ослабление

  dB = 0x50 * vol / 4095u; // 0..4095 -> 0..0x50

  writeReg(PAGE_SELECT, PAGE1);
  writeReg(R_ANLOG_VOL_2_HPR, dB | 0x80); // старший бит = 1
}

void muteHlt(void) {
  puts("\rMUTE Hlt\r");
  handset_speaker_en = 0;
  
  writeReg(PAGE_SELECT, PAGE1);
  writeReg(HPR_DRIVER, readReg(HPR_DRIVER) & (~HP_UNMUTE));
}

void unmuteHlt(void) {
  puts("\rUNMUTE Hlt\r");
  handset_speaker_en = 1;
  
  writeReg(PAGE_SELECT, PAGE1);
  writeReg(HPR_DRIVER, readReg(HPR_DRIVER) | HP_UNMUTE);
}

void muteLoudspeaker(void) {
  
  puts("\rMUTE Loudspeaker\r");
  loud_speaker_en = 0;
  
  writeReg(PAGE_SELECT, PAGE1);
//  writeReg(SPL_DRIVER, readReg(SPL_DRIVER) & (~HP_UNMUTE));
  writeReg(42, 0x00);
}

void unmuteLoudspeaker(void) {
  
  puts("\rUNMUTE Loudspeaker\r");
  loud_speaker_en = 1;
  
  writeReg(PAGE_SELECT, PAGE1);
//  writeReg(SPL_DRIVER, readReg(SPL_DRIVER) | HP_UNMUTE);
  writeReg(42, 0x04);
}

void selectHandsetMic(void) {
  selected_mic = HANDSET_MIC;
  writeReg(PAGE_SELECT, PAGE1); // page select
  writeReg(0x30, 0x20); // MIC1RP is selected for the MIC PGA with feed-forward resistance RIN = 20 kΩ
}

void selectLoudspeakMic(void) {
  selected_mic = LOUDSPEAKER_MIC;
  writeReg(PAGE_SELECT, PAGE1); // page select
  writeReg(0x30, 0x80); // MIC1LP is selected for the MIC PGA with feed-forward resistance RIN = 20 kΩ
}

/*uint8_t codecInit2(void) {
  
  flag_codec_init_err = 0;
  printf("Codec init...");
  
  RESET_LOW();
  HAL_Delay(50);
  RESET_HIGH();

  writeReg(PAGE_SELECT, 5);
  if (readReg(PAGE_SELECT) != 5) {
    flag_codec_init_err = 1;
    puts("ERROR: codecInit failed!");
    return 1; // ошибка
  }
  
  // page 0
  writeReg(0x00, 0x00);
  //writeReg(CLK_REG_1, 0); //  PLL_CLKIN = MCLK (device pin), CODEC_CLKIN = MCLK (device pin)
  writeReg(0x0B, 0x82);
  writeReg(0x0C, 0x81);
  writeReg(0x0D, 0x01);
  writeReg(0x0E, 0x00);
  writeReg(0x12, 0x82);//--
  writeReg(0x13, 0x82);
  writeReg(0x14, 0x80);
  writeReg(0x1E, 0x84);
  writeReg(0x3C, 0x01);
  writeReg(0x3D, 0x04);
  writeReg(0x3F, 0xD4);
  writeReg(0x43, 0xD3);
  writeReg(0x56, 0x00);
  writeReg(0x57, 0xFE);
  writeReg(0x58, 0x42);
  writeReg(0x59, 0x20);
  writeReg(0x5A, 0x38);
  writeReg(0x5B, 0x00);
  writeReg(0x5C, 0x00);
  writeReg(0x5D, 0x80);
  writeReg(0x51, 0x82);
  
  // page 1
  writeReg(0x00, 0x01);
  writeReg(0x1F, 0xC4);
  writeReg(0x20, 0x86);
  writeReg(0x21, 0x18);
  writeReg(0x23, 0x44);
  writeReg(0x24, 0x80);
  writeReg(0x25, 0x80);
  writeReg(0x28, 0x42);
  writeReg(0x29, 0x42);
  writeReg(0x2A, 0x10);
  writeReg(0x2E, 0x0A);
  writeReg(0x2F, 0x42);
  writeReg(0x31, 0x80);
  
  //
  writeReg(0x00, 0x00); // page select
  writeReg(0x00, 0x01); // page select
  writeReg(0x24, 0x80);
  
  writeReg(0x00, 0x01); // page select
  writeReg(0x25, 0x9E);//------------------91
  
  writeReg(0x00, 0x01); // page select
  writeReg(0x26, 0xA8); //----------------------ac
  
  writeReg(0x00, 0x00); // page select
  writeReg(0x41, 0x00); // ----------------------------d0
  
  writeReg(0x00, 0x00); // page select
  writeReg(0x42, 0x00); // ------------------------- d0
  
  // MIC PGA
  writeReg(0x00, 0x01); // page select
  readReg(0x2F); // answer: 0x42
  writeReg(0x2F, 0x3C);

  // ADC Digital Volume Control Coarse Adjust
  writeReg(0x00, 0x00); // page select
  writeReg(0x53, 0x0B);
  
  // AGC Control 1
  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x00);

  // AGC Control 2
  writeReg(0x00, 0x00); // page select
  readReg(0x57); // answer: 0xFE
  writeReg(0x57, 0x3E);

  // AGC Control 2
  writeReg(0x00, 0x00); // page select
  readReg(0x57); // answer: 0x3E
  writeReg(0x57, 0x3E);

  writeReg(0x00, 0x00); // page select
  readReg(0x51); // answer: 0x82
  writeReg(0x51, 0x02);
  
  writeReg(0x58, 0x3C);//-----------------------------3f
  
  readReg(0x51); // answer: 0x02
  writeReg(0x51, 0x82);
  
  writeReg(0x00, 0x00); // page select
  writeReg(0x59, 0x91);
  
  writeReg(0x00, 0x00); // page select
  writeReg(0x5A, 0x9A);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x5B); // answer: 0x00
  writeReg(0x5B, 0x00);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x5C); // answer: 0x00
  writeReg(0x5C, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x80);

 
  /////////////////////////
  // page 0
  writeReg(0x00, 0x00); // page select
  writeReg(0x0B, 0x82);
  writeReg(0x0C, 0x81);
  writeReg(0x0D, 0x01);
  writeReg(0x0E, 0x00);
  writeReg(0x12, 0x82);
  writeReg(0x13, 0x82);
  writeReg(0x14, 0x80);
  writeReg(0x1E, 0x84);
  writeReg(0x3C, 0x01);
  writeReg(0x3D, 0x04);
  writeReg(0x3F, 0xD4);
  writeReg(0x43, 0xD3);
  writeReg(0x56, 0x00);
  
  ///////////////////////////
  
  writeReg(0x57, 0xFE);
  writeReg(0x58, 0x42);
  writeReg(0x59, 0x20);
  writeReg(0x5A, 0x38);
  writeReg(0x5B, 0x00);
  writeReg(0x5C, 0x00);
  writeReg(0x5D, 0x80);
  writeReg(0x51, 0x82);

  // page 1
  writeReg(0x00, 0x01);
  writeReg(0x1F, 0xC4);
  writeReg(0x20, 0x86);
  writeReg(0x21, 0x18);
  writeReg(0x23, 0x44);
  writeReg(0x24, 0x80);
  writeReg(0x25, 0x80);
  writeReg(0x28, 0x42);
  writeReg(0x29, 0x42);
  writeReg(0x2A, 0x10);
  writeReg(0x2E, 0x0A);
  writeReg(0x2F, 0x42);
  writeReg(0x31, 0x80);

  //
  writeReg(0x00, 0x00); // page select
  writeReg(0x00, 0x01); // page select
  writeReg(0x24, 0x80);

  writeReg(0x00, 0x01); // page select
  writeReg(0x25, 0x9E);

  writeReg(0x00, 0x01); // page select
  writeReg(0x26, 0xA8);

  writeReg(0x00, 0x00); // page select
  writeReg(0x41, 0x00); // page select

  writeReg(0x00, 0x00); // page select
  writeReg(0x42, 0x00); // 

  // MIC PGA
  writeReg(0x00, 0x01); // page select
  readReg(0x2F); // answer: 0x42
  writeReg(0x2F, 0x3C);

  // ADC Digital Volume Control Coarse Adjust
  writeReg(0x00, 0x00); // page select
  writeReg(0x53, 0x0B);

  // AGC Control 1
  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x00);

  // AGC Control 2
  writeReg(0x00, 0x00); // page select
  readReg(0x57); // answer: 0xFE
  writeReg(0x57, 0x3E);

  // AGC Control 2
  writeReg(0x00, 0x00); // page select
  readReg(0x57); // answer: 0x3E
  writeReg(0x57, 0x3E); 

  writeReg(0x00, 0x00); // page select
  readReg(0x51); // answer: 0x82
  writeReg(0x51, 0x02);

  writeReg(0x58, 0x3C);

  readReg(0x51); // answer: 0x02
  writeReg(0x51, 0x82);

  writeReg(0x00, 0x00); // page select
  writeReg(0x59, 0x91);

  writeReg(0x00, 0x00); // page select
  writeReg(0x5A, 0x9A);

  writeReg(0x00, 0x00); // page select
  readReg(0x5B); // answer: 0x00
  writeReg(0x5B, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x5C); // answer: 0x00
  writeReg(0x5C, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x80);
  
  ///////////////////////////////// пауза, дальше отличается, 4-й кусок
  writeReg(0x00, 0x01); // page select
  readReg(0x29); // answer: 0x43
  writeReg(0x29, 0x47);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x28); // answer: 0x43
  writeReg(0x28, 0x43);
  
  readReg(0x28); // answer: 0x43
  writeReg(0x28, 0x43);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x2A); // answer: 0x11
  writeReg(0x2A, 0x11);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x29); // answer: 0x46
  writeReg(0x29, 0x42);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x0C
  writeReg(0x40, 0x0C);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x0C
  writeReg(0x40, 0x0C);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x30); // answer: 0x00
  writeReg(0x00, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x80

  writeReg(0x00, 0x00); // page select
  readReg(0x51); // answer: 0x82
  writeReg(0x51, 0x02);
  writeReg(0x58, 0x3C);
  
  readReg(0x51); // answer: 0x02
  writeReg(0x51, 0x82);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x30); // answer: 0x00
  writeReg(0x30, 0x80);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x30); // answer: 0x80
  writeReg(0x30, 0x00);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x30); // answer: 0x00
  writeReg(0x30, 0x00);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x52); // answer: 0x80
  writeReg(0x52, 0x80);

  writeReg(0x00, 0x01); // page select
  readReg(0x28); // answer: 0x43
  writeReg(0x28, 0x43);

  writeReg(0x00, 0x01); // page select
  readReg(0x2A); // answer: 0x11
  writeReg(0x2A, 0x11);

  writeReg(0x00, 0x01); // page select
  readReg(0x29); // answer: 0x43
  writeReg(0x29, 0x43);


  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x0C
  writeReg(0x40, 0x0C);

  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x0C
  writeReg(0x40, 0x0C);
  
  ///////////////////////////////// 
  
  // включаем динамик
  writeReg(0x00, 0x00); // page select
  writeReg(64, 0x00);
  writeReg(65, 0x00);
  writeReg(66, 0x00);

  writeReg(0x00, 0x01); // page select

  writeReg(38, 0x80);
  writeReg(42, 0x04);
  
  //////////////////////////////////
  
  // включаем микрофон
  writeReg(0x00, 0x00); // page select

  writeReg(0x12, 0x82);
  writeReg(0x13, 0x81);
  writeReg(0x14, 0x00);
  
  writeReg(0x52, 0x00); // ADC Digital Volume Control Fine Adjust, ADC unmuted
  writeReg(0x53, 0x00);

  writeReg(0x00, 0x01); // page select
  //writeReg(0x23, 0x44 | (1 << 5));

  // ADC Input Selection for P-Terminal
  writeReg(0x00, 0x01); // page select
  writeReg(0x30, 0x80); // MIC1LP is selected for the MIC PGA with feed-forward resistance RIN = 20 kΩ
  
  // ADC Input Selection for M-Terminal
  writeReg(0x00, 0x01); // page select
  writeReg(0x31, 0x80); // CM is selected for the MIC PGA with feed-forward resistance RIN = 20 kΩ.
  
  writeReg(0x00, 0x01); // page select
  writeReg(0x32, 0x00); // MIC1LP input is floating

  writeReg(0x00, 0x00); // page select
  writeReg(0x56, 0x00); // АРУ выкл
  
  

  
  writeReg(0x00, 0x00); // page select
  writeReg(0x51, 0x82); // ADC Digital Mic, ADC is powered up

  writeReg(0x00, 0x00); // page select
  writeReg(0x59, 0x91);

  writeReg(0x00, 0x00); // page select
  writeReg(0x5A, 0x9A);

  writeReg(0x00, 0x00); // page select
  readReg(0x5B); // answer: 0x00
  writeReg(0x5B, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x5C); // answer: 0x00
  writeReg(0x5C, 0x00);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x5C); // answer: 0x00
  writeReg(0x5C, 0x00);

  // во время включения звука (связи)
  writeReg(0x5A, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x80); // вкл АРУ, -5,5 dB

  // во время включения звука (связи)
  writeReg(0x0B, 0x82);
  writeReg(0x12, 0x82);

  writeReg(0x00, 0x01); // page select
  readReg(0x30); // answer: 0x00
  writeReg(0x30, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x80

  writeReg(0x00, 0x00); // page select
  readReg(0x51); // answer: 0x82
  writeReg(0x51, 0x02);
  writeReg(0x58, 0x3F); // AGC Maximum Gain

  readReg(0x51); // answer: 0x02
  writeReg(0x51, 0x82);

  writeReg(0x00, 0x01); // page select
  readReg(0x30); // answer: 0x00
  writeReg(0x30, 0x80);


*//*
  if (flag_codec_init_err) {
    puts("ERROR: codecInit failed!");
    return 1; // ошибка
  } else {
    puts("OK");
    return 0;
  }
}
*/
/*
void onConnect(void) {
  // page 0
  writeReg(0x00, 0x00);
  //writeReg(CLK_REG_1, 0); //  PLL_CLKIN = MCLK (device pin), CODEC_CLKIN = MCLK (device pin)
  writeReg(0x0B, 0x82);
  writeReg(0x0C, 0x81);
  writeReg(0x0D, 0x01);
  writeReg(0x0E, 0x00);
  writeReg(0x12, 0x82);
  writeReg(0x13, 0x82);
  writeReg(0x14, 0x80);
  writeReg(0x1E, 0x84);
  writeReg(0x3C, 0x01);
  writeReg(0x3D, 0x04);
  writeReg(0x3F, 0xD4);
  writeReg(0x43, 0xD3);
  writeReg(0x56, 0x00);
  writeReg(0x57, 0xFE);
  writeReg(0x58, 0x42);
  writeReg(0x59, 0x20);
  writeReg(0x5A, 0x38);
  writeReg(0x5B, 0x00);
  writeReg(0x5C, 0x00);
  writeReg(0x5D, 0x80);
  writeReg(0x51, 0x82);

  // page 1
  writeReg(0x00, 0x01);
  writeReg(0x1F, 0xC4);
  writeReg(0x20, 0x86);
  writeReg(0x21, 0x18);
  writeReg(0x23, 0x44);
  writeReg(0x24, 0x80);
  writeReg(0x25, 0x80);
  writeReg(0x28, 0x42);
  writeReg(0x29, 0x42);
  writeReg(0x2A, 0x10);
  writeReg(0x2E, 0x0A);
  writeReg(0x2F, 0x42);
  writeReg(0x31, 0x80);

  //
  writeReg(0x00, 0x00); // page select
  writeReg(0x00, 0x01); // page select
  writeReg(0x24, 0x80);

  writeReg(0x00, 0x01); // page select
  writeReg(0x25, 0x9E); //------------------91

  writeReg(0x00, 0x01); // page select
  writeReg(0x26, 0xA8); //----------------------ac

  writeReg(0x00, 0x00); // page select
  writeReg(0x41, 0x00); // ----------------------------d0

  writeReg(0x00, 0x00); // page select
  writeReg(0x42, 0x00); // ------------------------- d0

  // MIC PGA
  writeReg(0x00, 0x01); // page select
  readReg(0x2F); // answer: 0x42
  writeReg(0x2F, 0x3C);

  // ADC Digital Volume Control Coarse Adjust
  writeReg(0x00, 0x00); // page select
  writeReg(0x53, 0x0B);

  // AGC Control 1
  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x00);

  // AGC Control 2
  writeReg(0x00, 0x00); // page select
  readReg(0x57); // answer: 0xFE
  writeReg(0x57, 0x3E);

  // AGC Control 2
  writeReg(0x00, 0x00); // page select
  readReg(0x57); // answer: 0x3E
  writeReg(0x57, 0x3E);

  writeReg(0x00, 0x00); // page select
  readReg(0x51); // answer: 0x82
  writeReg(0x51, 0x02);

  writeReg(0x58, 0x3C); //-----------------------------3f

  readReg(0x51); // answer: 0x02
  writeReg(0x51, 0x82);

  writeReg(0x00, 0x00); // page select
  writeReg(0x59, 0x91);

  writeReg(0x00, 0x00); // page select
  writeReg(0x5A, 0x9A);

  writeReg(0x00, 0x00); // page select
  readReg(0x5B); // answer: 0x00
  writeReg(0x5B, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x5C); // answer: 0x00
  writeReg(0x5C, 0x00);

  { // во время включения звука (связи)
    writeReg(0x5A, 0x00);


  }
  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x80);

  { // во время включения звука (связи)
    writeReg(0x0B, 0x82);
    writeReg(0x12, 0x82);

    writeReg(0x00, 0x01); // page select
    readReg(0x30); // answer: 0x00
    writeReg(0x30, 0x00);

    writeReg(0x00, 0x00); // page select
    readReg(0x56); // answer: 0x80

    writeReg(0x00, 0x00); // page select
    readReg(0x51); // answer: 0x82
    writeReg(0x51, 0x02);
    writeReg(0x58, 0x3F);

    readReg(0x51); // answer: 0x02
    writeReg(0x51, 0x82);

    writeReg(0x00, 0x01); // page select
    readReg(0x30); // answer: 0x00
    writeReg(0x30, 0x80);

    writeReg(0x00, 0x00); // page select
    readReg(0x52); // answer: 0x80
    writeReg(0x52, 0x00);

  }
}

void onDisconnect(void) {
  writeReg(0x00, 0x01); // page select
  readReg(0x28); // answer: 0x47
  writeReg(0x28, 0x43);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x2A); // answer: 0x15
  writeReg(0x2A, 0x11);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x29); // answer: 0x47
  writeReg(0x29, 0x43);

  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x00
  writeReg(0x40, 0x08);

  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x08
  writeReg(0x40, 0x0C);
  
  
  
  
}

void unprogrammedKeyBeep(void) { // отзвук незапрограммированной кнопки
  writeReg(0x00, 0x00); // page select
  writeReg(0x41, 0xD0);
  
  writeReg(0x00, 0x00); // page select
  writeReg(0x42, 0xD0);
  
  // pause 8.5 ms
  

  writeReg(0x00, 0x00); // page select
  writeReg(0x0B, 0x82);
  writeReg(0x0C, 0x81);
  writeReg(0x0D, 0x01);
  writeReg(0x0E, 0x00);
  writeReg(0x12, 0x82);
  writeReg(0x13, 0x82);
  writeReg(0x14, 0x80);
  writeReg(0x1E, 0x84);
  writeReg(0x3C, 0x01);
  writeReg(0x3C, 0x04);
  writeReg(0x3F, 0xD4);
  writeReg(0x43, 0xD3);
  writeReg(0x56, 0x00);
  writeReg(0x57, 0xFE);
  writeReg(0x58, 0x42);
  writeReg(0x59, 0x20);
  writeReg(0x5A, 0x38);
  writeReg(0x5B, 0x00);
  writeReg(0x5C, 0x00);
  writeReg(0x5D, 0x80);

  writeReg(0x00, 0x01); // page select
  writeReg(0x1F, 0xC4);
  writeReg(0x20, 0x86);
  writeReg(0x21, 0x18);
  writeReg(0x23, 0x44);
  writeReg(0x24, 0x80);
  writeReg(0x25, 0x80);
  writeReg(0x28, 0x42);
  writeReg(0x29, 0x42);
  writeReg(0x2A, 0x10);
  writeReg(0x2E, 0x0A);
  writeReg(0x2F, 0x42);
  writeReg(0x31, 0x80);
  
  writeReg(0x00, 0x00); // page select
  writeReg(0x00, 0x01); // page select
  writeReg(0x24, 0x80);
  
  writeReg(0x00, 0x01); // page select
  writeReg(0x25, 0x91);
  
  writeReg(0x00, 0x01); // page select
  writeReg(0x26, 0x96);
  
  writeReg(0x00, 0x00); // page select
  writeReg(0x41, 0xD0);
  writeReg(0x42, 0xD0);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x2F); // answer: 0x42
  writeReg(0x2F, 0x3C);

  writeReg(0x00, 0x00); // page select
  writeReg(0x53, 0x0B);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x00);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x57); // answer: 0xFE
  writeReg(0x57, 0x3E);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x57); // answer: 0x3E
  writeReg(0x57, 0x3E);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x51); // answer: 0x82
  writeReg(0x51, 0x02);
  
  writeReg(0x58, 0x3C);
  
  readReg(0x51); // answer: 0x02
  writeReg(0x51, 0x82);

  writeReg(0x00, 0x00); // page select
  writeReg(0x59, 0x91);

  writeReg(0x00, 0x00); // page select
  writeReg(0x5A, 0x9A);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x5B); // answer: 0x00
  writeReg(0x5B, 0x00);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x5C); // answer: 0x00
  writeReg(0x5C, 0x00);
  
  writeReg(0x00, 0x00); // page select
  writeReg(0x5A, 0x00);

  writeReg(0x00, 0x00); // page select
  readReg(0x56); // answer: 0x00
  writeReg(0x56, 0x80);

  writeReg(0x00, 0x00); // page select
  writeReg(0x0B, 0x82);  
  writeReg(0x12, 0x82);

  writeReg(0x00, 0x01); // page select
  readReg(0x29); // answer: 0x43
  writeReg(0x29, 0x47);

  writeReg(0x00, 0x01); // page select
  readReg(0x2A); // answer: 0x11
  writeReg(0x2A, 0x15);

  writeReg(0x00, 0x01); // page select
  readReg(0x28); // answer: 0x43
  writeReg(0x28, 0x43);

  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x0C
  writeReg(0x40, 0x04);

  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x04
  writeReg(0x40, 0x00);
}


void unprogrammedKeyBeepEnd(void) { // по завершении отзвука незапрограммированной кнопки
  writeReg(0x00, 0x01); // page select
  readReg(0x28); // answer: 0x43
  writeReg(0x28, 0x43);
  
  writeReg(0x00, 0x01); // page select
  readReg(0x2A); // answer: 0x15
  writeReg(0x2A, 0x11);

  writeReg(0x00, 0x01); // page select
  readReg(0x29); // answer: 0x47
  writeReg(0x29, 0x43);  
  
  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x00
  writeReg(0x40, 0x08);
  
  writeReg(0x00, 0x00); // page select
  readReg(0x40); // answer: 0x08
  writeReg(0x40, 0x0C);
  
}
*/
