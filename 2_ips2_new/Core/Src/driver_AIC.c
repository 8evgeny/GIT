/**
  ******************************************************************************
  * @file    driver_AIC.c
  * @author  dmib@@ETS
  * @brief   This file contains AIC driver implementation
  ******************************************************************************
  */
#include "string.h"
#include "stm32h7xx_hal.h"
#include "CLI_io.h"
#include "tone_gen_state_processor.h"
#include "audio_process.h"
#include "SELC_sound_process.h"
#include "driver_AIC_reg.h"
#include "driver_AIC.h"

/// AIC driver print error level
#define AIC_CLI_ERROR_LEVEL	CLI_PRINTLEVEL_WORK
/// AIC driver print debug level
#define AIC_CLI_DEBUG_LEVEL	CLI_PRINTLEVEL_SVCDEBUG

/// use DCACHE
#define DCACHE_ENABLE

/// link to I2C2 hardware module control/status structure
extern I2C_HandleTypeDef hi2c2;

/// link to SAI_BlockA1 hardware module control/status structure
extern SAI_HandleTypeDef hsai_BlockA1;

/// link to SAI_BlockB1 hardware module control/status structure
extern SAI_HandleTypeDef hsai_BlockB1;


/// SAI TX DMA buffers
int16_t SAI_TX_DMA_Buffer[2][SAI_DMA_BUFFER_SIZE] __attribute__((section(".DMATransferSection")));
/// SAI RX DMA buffers
int16_t SAI_RX_DMA_Buffer[2][SAI_DMA_BUFFER_SIZE] __attribute__((section(".DMATransferSection")));

/// number of current DMA RX buffer
uint8_t nrxbuf;
/// number of current DMA TX buffer
uint8_t ntxbuf;
/// number of ready DMA RX buffer
uint8_t readyrxbuf;
/// number of ready DMA TX buffer
uint8_t readytxbuf;

/// current audio output PHONE sound volume
int8_t vol_Phone;
/// current audio output GR sound volume
int8_t vol_GR;
/// current audio input sound volume
int8_t vol_Mic;

/// current audio output device code
int8_t aic_curoutdev;
/// current audio input device code
int8_t aic_curindev;
/// audio loop enable flag
uint8_t sai_loop_on;

/// SAI TX complete interrupt callback
void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hOutSai)
{
    if (hOutSai->Instance == SAI1_Block_A) {
    	readytxbuf = ntxbuf;
    	ntxbuf ^= 1;
    }
}

/// SAI half TX complete interrupt callback
void HAL_SAI_TxHalfCpltCallback(SAI_HandleTypeDef *hOutSai)
{
    if (hOutSai->Instance == SAI1_Block_A) {
    	readytxbuf = ntxbuf;
    	ntxbuf ^= 1;
    }
}

/// SAI RX complete interrupt callback
void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hInSai)
{
    if (hInSai->Instance == SAI1_Block_B) {
    	readyrxbuf = nrxbuf;
    	nrxbuf ^= 1;
    }
}

/// SAI half RX complete interrupt callback
void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef *hInSai)
{
    if (hInSai->Instance == SAI1_Block_B) {
    	readyrxbuf = nrxbuf;
    	nrxbuf ^= 1;
    }
}

//-------------I2C control--------------------

/// AIC reset sequence register values
const uint8_t TLV320_Reset_Seq[] = {
			TLV320AIC3254_REG_PAGE_SELECT	, 0x00,
			TLV320AIC3254_REG_RESET			, 0x01
       };
	   
// MCLK = 4096000,		Fs = 8000
/// AIC clock init sequence register values
const uint8_t TLV320_ClockInit_Seq[] = {
			TLV320AIC3254_REG_PAGE_SELECT	, 0x00,
			TLV320AIC3254_REG_CLK_SRC_MUX	, 0x00, // PLL_clkin = MCLK, codec_clkin = MCLK
//			TLV320AIC3254_REG_PLL_P_R	  , 0x11, // PLL off (default)
//			TLV320AIC3254_REG_PLL_J			, 0xXX		// not used
//			TLV320AIC3254_REG_PLL_D_MSB		, 0xXX		// not used
//			TLV320AIC3254_REG_PLL_D_LSB		, 0xXX		// not used
			TLV320AIC3254_REG_NDAC			, 0x81,		// NDAC On, =1
			TLV320AIC3254_REG_MDAC			, 0x81,		// MDAC On, =1
			TLV320AIC3254_REG_DOSR_MSB		, 0x02,		// DOSR = 512
			TLV320AIC3254_REG_DOSR_LSB		, 0x00,
			TLV320AIC3254_REG_NADC			, 0x81,		// NADC On, =1
			TLV320AIC3254_REG_MADC			, 0x84,		// MADC On, =4
			TLV320AIC3254_REG_AOSR			, 0x80		// AOSR = 128
       };

/// AIC powerup sequence register values
const uint8_t TLV320_PowerUp_Seq[] = {
			TLV320AIC3254_REG_PAGE_SELECT		, 0x01,
			TLV320AIC3254_REG_LDO_CR			, 0x09,		// AVDD LDO Enable, Analog Blocks Disabled
			TLV320AIC3254_REG_PWR_CFG			, 0x08,		// Disabled weak connection of AVDD with DVDD
			TLV320AIC3254_REG_LDO_CR			, 0x01,		// AVDD LDO On, Analog Blocks Enable
			TLV320AIC3254_REG_CM_CR				, 0x43, 	// CM=0.75V,  HPCM = CM, LOCM = CM, HP powered from LDOIN,  LDOIN>2V
			TLV320AIC3254_REG_PLAYBACK_CFG1		, 0x00,		// LDAC - PTM_P3/4,  LDAC HPL - AB Driver
			TLV320AIC3254_REG_PLAYBACK_CFG1		, 0x00,		// RDAC - PTM_P3/4,  LDAC HPL - AB Driver
			TLV320AIC3254_REG_ADC_PTUNE_CFG		, 0x00,		// ADC - PTM_R4
			TLV320AIC3254_REG_AINPUT_CHARGE_CFG	, 0x32,		// AINPUT powerup time = 6.4ms
			TLV320AIC3254_REG_REF_PWRUP_CFG		, 0x01,		// Reference powerup = 40ms
			TLV320AIC3254_REG_HPHONE_STARTUP_CR	, 0x40		// set soft-step the DC-offset – 50 ms
		};
	   
/// AIC ADC setup sequence register values
const uint8_t TLV320_ADCSetup_Seq[] = {
			TLV320AIC3254_REG_PAGE_SELECT		, 0x01,
//			TLV320AIC3254_REG_MICBIAS_CFG		, 0x48,		// ? MICBIAS settings?	 not used in rev1?

//			TLV320AIC3254_REG_LMICPGA_PMUX		, 0x10,		// IN2L -> P Left MICPGA
//			TLV320AIC3254_REG_LMICPGA_NMUX		, 0x10,		// IN2R -> N Left MICPGA
//			TLV320AIC3254_REG_RMICPGA_PMUX		, 0x40,		// IN1L -> P Right MICPGA
//			TLV320AIC3254_REG_RMICPGA_NMUX		, 0x10,		// IN1R -> N Right MICPGA

			TLV320AIC3254_REG_LMICPGA_PMUX		, 0x40,		// IN1L -> P Left MICPGA
			TLV320AIC3254_REG_LMICPGA_NMUX		, 0x40,		// CM1L -> N Left MICPGA
//			TLV320AIC3254_REG_LMICPGA_PMUX		, 0x50,		// IN1L + IN2L -> P Left MICPGA
//			TLV320AIC3254_REG_LMICPGA_NMUX		, 0x50,		// CM1L + IN2R -> N Left MICPGA
//			TLV320AIC3254_REG_LMICPGA_PMUX		, 0x10,		// IN2L -> P Left MICPGA
//			TLV320AIC3254_REG_LMICPGA_NMUX		, 0x10,		// IN2R -> N Left MICPGA

//			TLV320AIC3254_REG_LMICPGA_PMUX		, 0x40,		// IN1L -> P Left MICPGA		 not used
//			TLV320AIC3254_REG_LMICPGA_NMUX		, 0x40,		// CM1L -> N Left MICPGA		 not used
//			TLV320AIC3254_REG_RMICPGA_PMUX		, 0x40,		// IN1L -> P Right MICPGA		 not used
//			TLV320AIC3254_REG_RMICPGA_NMUX		, 0x10,		// IN1R -> N Right MICPGA		 not used
			TLV320AIC3254_REG_LMICPGA_VOL		, 0x2F,		// // LMICPGA Gain enabled, +23.5db gain     as GIT say!
			//TLV320AIC3254_REG_RMICPGA_VOL		, 0x1F,		// ??? not used

			TLV320AIC3254_REG_PAGE_SELECT		, 0x00,
//			TLV320AIC3254_REG_ADC_SETUP			, 0x40,		// Right ADC on, Left ADC off
			TLV320AIC3254_REG_ADC_SETUP			, 0xC0,		// Right ADC on, Left ADC on
//			TLV320AIC3254_REG_ADC_FGAIN			, 0x88,		// ADC mute ctrl: Right ADC muted (8.) , Left ADC muted (.8)
			TLV320AIC3254_REG_ADC_FGAIN			, 0x00,		// ADC mute ctrl: Right ADC unmuted (0.) , Left ADC unmuted (.0)
			//TLV320AIC3254_REG_RADC_AGC_CR1	, 0xXX,		// Rigth AGC On? in future
		};

/// AIC DAC setup sequence register values
const uint8_t TLV320_DACSetup_Seq[] = {
			TLV320AIC3254_REG_PAGE_SELECT	, 0x01,
			TLV320AIC3254_REG_HPL_SEL		, 0x00,		// HPL <- off
			TLV320AIC3254_REG_HPR_SEL		, 0x00,		// HPR <- off

			TLV320AIC3254_REG_LOL_SEL		, 0x08,		// LOL <- Left DAC
//			TLV320AIC3254_REG_LOL_SEL		, 0x1A,		// LOL <- MAL , LOL <- Left DAC,LOL <- Right DAC

//			TLV320AIC3254_REG_LOR_SEL		, 0x00,		// LOR <- off
            TLV320AIC3254_REG_LOR_SEL		, 0x08,		// LOR <- Left DAC - Для SC_2

			TLV320AIC3254_REG_OUTDRV_PWR_CR	, 0x0C,		// HPL/HPR pwroff, LOL/LOR pwron, MAL/MAR pwroff
//			TLV320AIC3254_REG_OUTDRV_PWR_CR	, 0x0F,		// HPL/HPR pwroff, LOL/LOR pwron, MAL/MAR pwron

			//TLV320AIC3254_REG_HPL_GAIN		, 0x40,		// HPL muted	(default)
			//TLV320AIC3254_REG_HPR_GAIN		, 0x40,		// HPR muted	(default)
			TLV320AIC3254_REG_LOL_GAIN		, 0x00,		// LOL unmuted, 0db gain		as GIT say!
			TLV320AIC3254_REG_LOR_GAIN		, 0x00,		// LOR unmuted, 0db gain  ? 
			TLV320AIC3254_REG_PAGE_SELECT	, 0x00,
			//TLV320AIC3254_REG_LDAC_DVOL_CR	, 0x00,		// LDAC dvol = 0db	(default)
			//TLV320AIC3254_REG_RDAC_DVOL_CR	, 0x00,		// RDAC dvol = 0db	(default)
			TLV320AIC3254_REG_DAC_SETUP1	, 0xD4,		// LDAC pwrup, RDAC pwrup, LDAC <- LeftData, RDAC <- RightData  (?)
			TLV320AIC3254_REG_DAC_SETUP2	, 0x00,		// LDAC unmuted, RDAC unmuted  (?)
		};
		
uint8_t TLV320_Read(uint8_t reg, uint8_t *data) 
{
  return HAL_I2C_Mem_Read(&hi2c2, TLV320AIC3254_Address, reg ,I2C_MEMADD_SIZE_8BIT,data, 1, 100);
}

uint8_t TLV320_ReadPage(uint8_t page, uint8_t reg, uint8_t *data)
{
  uint8_t res;
  res = HAL_I2C_Mem_Write(&hi2c2, TLV320AIC3254_Address, TLV320AIC3254_REG_PAGE_SELECT ,I2C_MEMADD_SIZE_8BIT,&page, 1, 100);
  if (res != HAL_OK) return res;
  return HAL_I2C_Mem_Read(&hi2c2, TLV320AIC3254_Address, reg ,I2C_MEMADD_SIZE_8BIT,data, 1, 100);
}

uint8_t TLV320_Write(uint8_t reg, uint8_t data) 
{
  return HAL_I2C_Mem_Write(&hi2c2, TLV320AIC3254_Address, reg ,I2C_MEMADD_SIZE_8BIT,&data, 1, 100);
}

uint8_t TLV320_WritePage(uint8_t page, uint8_t reg, uint8_t data) 
{
  uint8_t res;
  res = HAL_I2C_Mem_Write(&hi2c2, TLV320AIC3254_Address, TLV320AIC3254_REG_PAGE_SELECT ,I2C_MEMADD_SIZE_8BIT,&page, 1, 100);
  if (res != HAL_OK) return res;
  return HAL_I2C_Mem_Write(&hi2c2, TLV320AIC3254_Address, reg ,I2C_MEMADD_SIZE_8BIT,&data, 1, 100);
}
   
uint8_t TLV320_WriteSq(uint8_t *data, uint16_t len) 
{
  uint8_t i,res;
  for (i = 0; i < len; i+=2) {
   res = HAL_I2C_Mem_Write(&hi2c2, TLV320AIC3254_Address, data[i] ,I2C_MEMADD_SIZE_8BIT,(uint8_t *)&data[i+1], 1, 100);
   if (res != HAL_OK) return res;
  }
  return res;
}
	   
uint8_t TLV320_Reset()
{
  uint8_t res;
  res = TLV320_WriteSq((uint8_t *)&TLV320_Reset_Seq[0], sizeof(TLV320_Reset_Seq));
  if (res != HAL_OK) return res;
  HAL_Delay(100);
  return res;
}

uint8_t aic_setDACOutVolume (int8_t volume)
{
  // -127..48 -> -63.5 .. +24 db  , 0.5db step
  #ifdef SC_2
    //Выход LINE_OUT2  (0x42 регистр)
  return TLV320_WritePage(0, TLV320AIC3254_REG_RDAC_DVOL_CR, volume);
  #endif
  #ifdef SC_4
    //Выход LINE_OUT1  (0x41 регистр)
  return TLV320_WritePage(0, TLV320AIC3254_REG_LDAC_DVOL_CR, volume);
  #endif
}

uint8_t aic_setADCInVolume (int8_t volume)
{
  // -24..40 -> -12 .. +20 db  , 0.5db step
#ifdef USE_SELC
  selc_adjust_threshold_level(volume);
#endif
    //Вход MIC2_L  (0x53 регистр)
  TLV320_WritePage(0, TLV320AIC3254_REG_LADC_VOL_CR, volume & 0x7F);
    //Вход MIC2_R  (0x54 регистр)
  return TLV320_Write(TLV320AIC3254_REG_RADC_VOL_CR, volume & 0x7F);
}

uint8_t aic_setDACMute (uint8_t ismuted)
{
  if (ismuted)
    return TLV320_WritePage(0, TLV320AIC3254_REG_DAC_SETUP2, 0xC0);
  else
    return TLV320_WritePage(0, TLV320AIC3254_REG_DAC_SETUP2, 0x00);
}

uint8_t aic_setADCMute (uint8_t ismuted)
{
  if (ismuted)
    return TLV320_WritePage(0, TLV320AIC3254_REG_ADC_FGAIN, 0x88);
  else
    return TLV320_WritePage(0, TLV320AIC3254_REG_ADC_FGAIN, 0x00);
}

uint8_t aic_getADCFlags (uint8_t *flags)
{
  return TLV320_Read(TLV320AIC3254_REG_ADCFLAGS, flags);
}

uint8_t aic_getDACFlags (uint8_t *flags)
{
  TLV320_Read(TLV320AIC3254_REG_DACFLAGS1, &flags[0]);
  return TLV320_Read(TLV320AIC3254_REG_DACFLAGS2, &flags[1]);
}

void aic_setOutDev(uint8_t dev)
{
	if (aic_curoutdev == dev) return;

	switch (dev) {
		case AIC_OUTDEV_GR:
			aic_setDACOutVolume(vol_GR);
		break;
		case AIC_OUTDEV_PHONE:
			aic_setDACOutVolume(vol_Phone);
		break;
		default: return;
	}
	aic_curoutdev = dev;
}

void aic_setInDev(uint8_t dev)
{
	if (aic_curindev == dev) return;

	switch (dev) {
		case AIC_INDEV_INTMIC:
            aic_setADCInVolume(vol_Mic);
			// TODO update aic regs
			TLV320_WritePage(1, TLV320AIC3254_REG_LMICPGA_PMUX, 0x40); // IN1L -> P Left MICPGA
			TLV320_WritePage(1, TLV320AIC3254_REG_LMICPGA_NMUX, 0x40); // CM1L -> N Left MICPGA
		break;
		case AIC_INDEV_EXTMIC:
            aic_setADCInVolume(vol_Mic);
			// TODO update aic regs
			TLV320_WritePage(1, TLV320AIC3254_REG_LMICPGA_PMUX, 0x10); // IN2L -> P Left MICPGA
			TLV320_WritePage(1, TLV320AIC3254_REG_LMICPGA_NMUX, 0x10); // IN2R -> N Left MICPGA
		break;
		default: return;
	}
	aic_curindev = dev;
}

/// change sound volume signal macro
#define LEV_CHNG_TONE		tone_genPlayTone(SIG_GEN_TCH600, 50)
/// sound volume boundary signal macro
#define LEV_CHNG_TONE_MAX	tone_genPlayTone(SIG_GEN_TCH1000, 50)

void aic_setInVolUp()
{
	if (vol_Mic<VOL_MIC_MAX) {
		vol_Mic += VOL_MIC_STEP;
		aic_setADCInVolume(vol_Mic);
		LEV_CHNG_TONE;
	} else
		LEV_CHNG_TONE_MAX;
	CLI_print_lev(AIC_CLI_DEBUG_LEVEL,"MIC Vol %i\r\n",vol_Mic);
}

void aic_setInVolDown()
{
	if (vol_Mic>VOL_MIC_MIN) {
	 vol_Mic -= VOL_MIC_STEP;
	 aic_setADCInVolume(vol_Mic);
	 LEV_CHNG_TONE;
	} else
	 LEV_CHNG_TONE_MAX;
	CLI_print_lev(AIC_CLI_DEBUG_LEVEL,"MIC Vol %i\r\n",vol_Mic);
}

void aic_setOutVolUp()
{
	switch (aic_curoutdev) {
		case AIC_OUTDEV_GR:
			if (vol_GR<VOL_OUT_MAX) {
				vol_GR += VOL_OUT_STEP;
				aic_setDACOutVolume(vol_GR);
				LEV_CHNG_TONE;
			} else
				LEV_CHNG_TONE_MAX;
			CLI_print_lev(AIC_CLI_DEBUG_LEVEL,"OUT Vol %i\r\n",vol_GR);
		break;
		case AIC_OUTDEV_PHONE:
			if (vol_Phone<VOL_OUT_MAX) {
				vol_Phone += VOL_OUT_STEP;
				aic_setDACOutVolume(vol_Phone);
				LEV_CHNG_TONE;
			} else
				LEV_CHNG_TONE_MAX;
			CLI_print_lev(AIC_CLI_DEBUG_LEVEL,"OUT Vol %i\r\n",vol_Phone);
		break;
		default: return;
	}

}

void aic_setOutVolDown()
{
	switch (aic_curoutdev) {
		case AIC_OUTDEV_GR:
			if (vol_GR>VOL_OUT_MIN) {
				vol_GR -= VOL_OUT_STEP;
				aic_setDACOutVolume(vol_GR);
				LEV_CHNG_TONE;
			} else
				LEV_CHNG_TONE_MAX;
			CLI_print_lev(AIC_CLI_DEBUG_LEVEL,"OUT Vol %i\r\n",vol_GR);
		break;
		case AIC_OUTDEV_PHONE:
			if (vol_Phone>VOL_OUT_MIN) {
				vol_Phone -= VOL_OUT_STEP;
				aic_setDACOutVolume(vol_Phone);
				LEV_CHNG_TONE;
			} else
				LEV_CHNG_TONE_MAX;
			CLI_print_lev(AIC_CLI_DEBUG_LEVEL,"OUT Vol %i\r\n",vol_Phone);
		break;
		default: return;
	}
}

void aic_setSAIloop(uint8_t enable)
{
	sai_loop_on = enable;
}

void aic_setup_AGC()
{
	TLV320_WritePage(0, TLV320AIC3254_REG_LADC_AGC_CR2, 0x1E); // 00 01010 0 // AGC Hyst 1.0dB, 0x1E = 30 = -60dB Noise Thresh
	TLV320_WritePage(0, TLV320AIC3254_REG_LADC_AGC_CR3, 0x2F); //  2F = +23.5db MAX PGA gain
	TLV320_WritePage(0, TLV320AIC3254_REG_LADC_AGC_CR4, 0x00); // 00000 000, Attack = 32 clk (12ms)
	TLV320_WritePage(0, TLV320AIC3254_REG_LADC_AGC_CR5, 0x00); // 00000 000, Decay = 512 clk (64ms)
	TLV320_WritePage(0, TLV320AIC3254_REG_LADC_AGC_CR6, 0x09); // 000 01001, Noise Debounce = 1024 clk (128ms)
	TLV320_WritePage(0, TLV320AIC3254_REG_LADC_AGC_CR7, 0x06); // 0000 0110 Signal Debounce = 64 clk (8 ms)
	TLV320_WritePage(0, TLV320AIC3254_REG_LADC_AGC_CR1, 0x11); // 0 001 00 01  // AGC OFF, -8.0dBFS, 1.0dB GainHyst
	//TLV320_WritePage(0, TLV320AIC3254_REG_LADC_AGC_CR1, 0x91); // 1 001 00 01  // AGC ON, -8.0dBFS, 1.0dB GainHyst
}

uint32_t aic_smp_cnt; // debug

int8_t aic_init()
{
	if (TLV320_Reset()!=HAL_OK) {
		CLI_print_lev(AIC_CLI_ERROR_LEVEL,"Error : AIC reset\r\n");
		return -1;
	};

	if (TLV320_WriteSq((uint8_t *)TLV320_ClockInit_Seq, sizeof(TLV320_ClockInit_Seq))!=HAL_OK) {
		CLI_print_lev(AIC_CLI_ERROR_LEVEL,"Error : AIC init\r\n");
		return -2;
	};

	if (TLV320_WriteSq((uint8_t *)TLV320_PowerUp_Seq,sizeof(TLV320_PowerUp_Seq))!=HAL_OK) {
		CLI_print_lev(AIC_CLI_ERROR_LEVEL,"Error : AIC init\r\n");
		return -2;
	};

	if (TLV320_WriteSq((uint8_t *)TLV320_ADCSetup_Seq,sizeof(TLV320_ADCSetup_Seq))!=HAL_OK) {
		CLI_print_lev(AIC_CLI_ERROR_LEVEL,"Error : AIC init\r\n");
		return -2;
	};
	
	if (TLV320_WriteSq((uint8_t *)TLV320_DACSetup_Seq,sizeof(TLV320_DACSetup_Seq))!=HAL_OK) {
		CLI_print_lev(AIC_CLI_ERROR_LEVEL,"Error : AIC init\r\n");
		return -2;
	};
	
//	aic_setADCMute(0);

	TLV320_WritePage(0, TLV320AIC3254_REG_DAC_DSP_CR, 0x19); // DAC PRB_P25
	TLV320_WritePage(0, TLV320AIC3254_REG_BEEP_LEN_1, 0x00);
	TLV320_WritePage(0, TLV320AIC3254_REG_BEEP_LEN_2, 0x1F);
	TLV320_WritePage(0, TLV320AIC3254_REG_BEEP_LEN_3, 0x40);
	TLV320_WritePage(0, TLV320AIC3254_REG_BEEP_SIN_MSB, 0x01);
	TLV320_WritePage(0, TLV320AIC3254_REG_BEEP_SIN_LSB, 0x90);
	TLV320_WritePage(0, TLV320AIC3254_REG_BEEP_COS_MSB, 0x0);
	TLV320_WritePage(0, TLV320AIC3254_REG_BEEP_COS_LSB, 0x0);

	aic_setup_AGC();

	memset((uint8_t *)&SAI_TX_DMA_Buffer[0][0], 0, 4*SAI_DMA_BUFFER_SIZE);
	memset((uint8_t *)&SAI_RX_DMA_Buffer[0][0], 0, 4*SAI_DMA_BUFFER_SIZE);
	//SAI_TX_DMA_Buffer[0][0] = 300;
#ifdef DCACHE_ENABLE
	SCB_CleanDCache_by_Addr((uint32_t *)&SAI_TX_DMA_Buffer[0][0], SAI_DMA_BUFFER_SIZE*2); // flush DCache to TX buffer for DMA
	SCB_CleanDCache_by_Addr((uint32_t *)&SAI_TX_DMA_Buffer[1][0], SAI_DMA_BUFFER_SIZE*2); // flush DCache to TX buffer for DMA
#endif

	vol_Phone = 0;
	vol_GR = 0;
	vol_Mic = 0;
    aic_setOutDev(AIC_OUTDEV_GR);
//    aic_setOutDev(AIC_OUTDEV_PHONE);
    aic_setInDev(AIC_INDEV_INTMIC);
	sai_loop_on = 0;

	nrxbuf = 0;
	ntxbuf = 0;
	readyrxbuf = 0xFF;
	readytxbuf = 0xFF;
	if (HAL_SAI_Transmit_DMA(&hsai_BlockA1, (uint8_t *)&SAI_TX_DMA_Buffer[0][0], 2*SAI_DMA_BUFFER_SIZE)!=HAL_OK)
	{
		CLI_print_lev(AIC_CLI_ERROR_LEVEL,"Error : AIC SAI DMA init\r\n");
		return -3;

	}
	if (HAL_SAI_Receive_DMA(&hsai_BlockB1, (uint8_t *)&SAI_RX_DMA_Buffer[0][0], 2*SAI_DMA_BUFFER_SIZE)!=HAL_OK)
	{
		CLI_print_lev(AIC_CLI_ERROR_LEVEL,"Error : AIC SAI DMA init\r\n");
		return -3;
	}

	//HAL_SAI_Transmit_IT(&hsai_BlockA1, (uint8_t *)&SAI_TX_DMA_Buffer[0][0], SAI_DMA_BUFFER_SIZE);
	//HAL_SAI_Receive_IT(&hsai_BlockB1, (uint8_t *)&SAI_RX_DMA_Buffer[0][0], SAI_DMA_BUFFER_SIZE);
	aic_smp_cnt = 0;

#ifdef USE_SELC
    selc_init();
#endif
	
	return 0;
}

void aic_deinit()
{
	HAL_SAI_DMAStop(&hsai_BlockA1);
	HAL_SAI_DMAStop(&hsai_BlockB1);
}

void aic_task()
{
	int16_t *srcbuf,*dstbuf;

	if (readyrxbuf!=0xFF)
	if (readytxbuf!=0xFF)
	{
		aic_smp_cnt += SAI_DMA_BUFFER_SIZE;

		if (readyrxbuf==0)
			srcbuf = &SAI_RX_DMA_Buffer[0][0];
		else
			srcbuf = &SAI_RX_DMA_Buffer[1][0];
		if (readytxbuf == 0)
			dstbuf = &SAI_TX_DMA_Buffer[0][0];
		else
			dstbuf = &SAI_TX_DMA_Buffer[1][0];

#ifdef DCACHE_ENABLE
		SCB_InvalidateDCache_by_Addr((uint8_t *)srcbuf, SAI_DMA_BUFFER_SIZE*2); // invalidate RX buffer DCache after DMA rx
#endif
		if (sai_loop_on) {
			memcpy((uint8_t *)dstbuf, (uint8_t *)srcbuf ,SAI_DMA_BUFFER_SIZE*2);
		} else {
			audio_get_output(dstbuf,SAI_DMA_BUFFER_SIZE*2);

#ifdef USE_SELC
            selc_process_sound(srcbuf, dstbuf, SAI_DMA_BUFFER_SIZE);
#endif

			audio_write_input(srcbuf ,SAI_DMA_BUFFER_SIZE*2);
		}
		if (tone_genIsTone()) {
		 	tone_genGetData(dstbuf, SAI_DMA_BUFFER_SIZE);
#ifdef USE_SELC
            selc_set_tone_gen();
#endif
		}

#ifdef DCACHE_ENABLE
		SCB_CleanDCache_by_Addr((uint32_t *)dstbuf, SAI_DMA_BUFFER_SIZE*2); // flush DCache to TX buffer for DMA tx
#endif
		HAL_NVIC_DisableIRQ(SAI1_IRQn);
		readyrxbuf = 0xFF;
		readytxbuf = 0xFF;
		HAL_NVIC_EnableIRQ(SAI1_IRQn);
	}
}

/*
// for HAL_SAI_Receive_IT/HAL_SAI_Transmit_IT test

void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef *hsai)
{
	if (nrxbuf==0)
	 HAL_SAI_Receive_IT(&hsai_BlockB1, (uint8_t *)&SAI_RX_DMA_Buffer[1][0], SAI_DMA_BUFFER_SIZE);
	else
	 HAL_SAI_Receive_IT(&hsai_BlockB1, (uint8_t *)&SAI_RX_DMA_Buffer[0][0], SAI_DMA_BUFFER_SIZE);
	readyrxbuf = nrxbuf;
	nrxbuf ^= 1;
}

void HAL_SAI_TxCpltCallback(SAI_HandleTypeDef *hsai)
{
	if (ntxbuf==0)
	  HAL_SAI_Transmit_IT(&hsai_BlockA1, (uint8_t *)&SAI_TX_DMA_Buffer[1][0], SAI_DMA_BUFFER_SIZE);
	else
	  HAL_SAI_Transmit_IT(&hsai_BlockA1, (uint8_t *)&SAI_TX_DMA_Buffer[0][0], SAI_DMA_BUFFER_SIZE);
	readytxbuf = ntxbuf;
	ntxbuf ^= 1;
}
*/
