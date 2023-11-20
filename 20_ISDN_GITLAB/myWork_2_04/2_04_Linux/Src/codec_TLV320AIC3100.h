/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   codec_TLV320AIC3100.h
 * Author: roman.ganaza
 *
 * Created on July 4, 2023, 11:21 AM
 */

#ifndef CODEC_TLV320AIC3100_H
#define CODEC_TLV320AIC3100_H

#include <stdint.h>

uint8_t codecInit(void);
void setSpeakerVol(uint16_t vol);
void setMicVol(uint16_t vol, uint8_t sel_mic);
void setHltVol(uint16_t vol);
void muteHlt(void);
void unmuteHlt(void);
void selectHandsetMic(void);
void selectLoudspeakMic(void);
void muteLoudspeaker(void);
void unmuteLoudspeaker(void);

enum {HANDSET_MIC = 0, LOUDSPEAKER_MIC};

extern uint8_t selected_mic;

struct aic3100_rate_divs {
	uint32_t mclk;
	uint32_t rate;
	uint8_t p_val;
	uint8_t pll_j;
	uint16_t pll_d;
	uint16_t dosr;
	uint8_t ndac;
	uint8_t mdac;
	uint16_t aosr;
	uint8_t nadc;
	uint8_t madc;
	uint8_t blck_N;
};

/* aic3100 register space */

/* ****************** Page 0 Registers **************************************/
#define	PAGE_SELECT			0
#define	RESET				1
#define OT_FLAG				3
#define	CLK_REG_1			4
#define	CLK_REG_2			5
#define	CLK_REG_3			6
#define	CLK_REG_4			7
#define	CLK_REG_5			8
#define	NDAC_CLK_REG			11 // 0x0B
#define	MDAC_CLK_REG			12 // 0x0C
#define DAC_OSR_MSB			13 // 0x0D
#define DAC_OSR_LSB			14 // 0x0E
#define	NADC_CLK_REG			18 // 0x12
#define	MADC_CLK_REG			19 // 0x13
#define ADC_OSR_REG			20 // 0x14
#define CLK_MUX_REG_9		        25 // 0x19
#define CLK_REG_10			26 // 0x1A
#define INTERFACE_SET_REG_1	        27 // 0x1B
#define AIS_REG_2			28 // 0x1C
#define INTERFACE_SET_REG_2		29 // 0x1D
#define BCLK_N_VAL			30 // 0x1E
#define AIS_REG_4			31 // 0x1F
#define INTERFACE_SET_REG_4		32 // 0x20
#define INTERFACE_SET_REG_5		33 // 0x21
#define I2C_FLAG			34 // 0x22
#define ADC_FLAG			36 // 0x24
#define DAC_FLAG_1			37 // 0x25
#define DAC_FLAG_2			38 // 0x26
#define OVERFLOW_FLAG			39 // 0x27
#define INTR_FLAG_1			44 // 0x2C
#define INTR_FLAG_2			46 // 0x2E
#define INTL_CRTL_REG_1			48 // 0x30
#define INT2_CTRL			49 // 0x31
#define GPIO_CRTL_REG_1			51 // 0x33
#define DOUT_CTRL			53 // 0x35
#define DIN_CTL				54 // 0x36
#define DAC_PRB_SEL_REG			60 // 0x3C
#define ADC_PRB_SEL_REG			61 // 0x3D
#define DAC_CHN_REG			63 // 0x3F
#define DAC_MUTE_CTRL_REG		64 // 0x40
#define LDAC_VOL			65 // 0x41
#define RDAC_VOL			66 // 0x42
#define HEADSET_DETECT			67 // 0x43
#define DRC_CTRL_1			68 // 0x44
#define DRC_CTRL_2			69 // 0x45
#define DRC_CTRL_3			70
#define BEEP_GEN_L			71
#define BEEP_GEN_R			72
#define BEEP_LEN_MSB			73
#define BEEP_LEN_MID			74
#define BEEP_LEN_LSB			75
#define BEEP_SINX_MSB			76
#define BEEP_SINX_LSB			77
#define BEEP_COSX_MSB			78
#define BEEP_COSX_LSB			79
#define ADC_DIG_MIC			81 // 0x51
#define	ADC_FGA				82 // 0x52
#define	ADC_CGA				83 // 0x53

/*Channel AGC Control Register 1*/
#define AGC_CTRL_1			    86 // 0x56
/*Channel AGC Control Register 2*/
#define AGC_CTRL_2			    87 // 0x57
/*Channel AGC Control Register 3 */
#define AGC_MAX_GAIN			    88 // 0x58
/*Channel AGC Control Register 4 */
#define AGC_CTRL_4			    89 // 0x59
/*Channel AGC Control Register 5 */
#define AGC_CTRL_5			    90 // 0x5A
/*Channel AGC Control Register 6 */
#define AGC_CTRL_6			    91 // 0x5B
/*Channel AGC Control Register 7 */
#define AGC_CTRL_7			    92 // 0x5C
/* AGC Gain applied reading (RO) */
#define AGC_CTRL_8			    93 // 0x5D
/* VOL/MICDET-Pin SAR ADC Volume Control */
#define VOL_MICDECT_ADC			116
/* VOL/MICDET-Pin Gain*/
#define VOL_MICDECT_GAIN		117

/******************** Page 1 Registers **************************************/
#define PAGE_1				1
#define HP_SPK_ERR_CTL                  (30) // 0x1E
/* Headphone drivers */
#define HEADPHONE_DRIVER		(31) // 0x1F
/* Class-D Speakear Amplifier */
#define CLASSD_SPEAKER_AMP              (32) // 0x20
/* HP Output Drivers POP Removal Settings */
#define HP_POP_CTRL			(33) // 0x21
/* Output Driver PGA Ramp-Down Period Control */
#define PGA_RAMP_CTRL			(34) // 0x22
/* DAC_L and DAC_R Output Mixer Routing */
#define DAC_MIX_CTRL			(35) // 0x23
/*Left Analog Vol to HPL */
#define L_ANLOG_VOL_2_HPL		(36) // 0x24
/* Right Analog Vol to HPR */
#define R_ANLOG_VOL_2_HPR		(37) // 0x25
/* Left Analog Vol to SPL */
#define L_ANLOG_VOL_2_SPL		(38) // 0x26
/* Right Analog Vol to SPR */
#define R_ANLOG_VOL_2_SPR		(39) // 0x27
/* HPL Driver */
#define HPL_DRIVER			(40) // 0x28
/* HPR Driver */
#define HPR_DRIVER			(41) // 0x29
/* SPL Driver */
#define SPL_DRIVER			(42) // 0x2A
/* HP Driver Control */
#define HP_DRIVER_CTRL			(44) // 0x2C
/*MICBIAS Configuration Register*/
#define MICBIAS_CTRL			(46) // 0x2E
/* MIC PGA*/
#define MIC_PGA				(47) // 0x2F
/* Delta-Sigma Mono ADC Channel Fine-Gain Input Selection for P-Terminal */
#define MIC_GAIN			(48) // 0x30
/* ADC Input Selection for M-Terminal */
#define ADC_IP_SEL			(49) // 0x31
/*MICBIAS Configuration*/
#define CM_SET	         		(50) // 0x32
/****************************************************************************/
/*  Page 3 Registers 				              	  	    */
/****************************************************************************/
#define PAGE_3				(3)

/* Timer Clock MCLK Divider */
#define TIMER_CLOCK_MCLK_DIVIDER	(16)

/****************************************************************************/
/****************************************************************************/
#define BIT7		(1 << 7)
#define BIT6		(1 << 6)
#define BIT5		(1 << 5)
#define BIT4		(1 << 4)
#define	BIT3		(1 << 3)
#define BIT2		(1 << 2)
#define BIT1		(1 << 1)
#define BIT0		(1 << 0)

#define HP_UNMUTE			BIT2
#define HPL_UNMUTE			BIT3
#define ENABLE_DAC_CHN			(BIT6 | BIT7)
#define ENABLE_ADC_CHN			(BIT6 | BIT7)
#define BCLK_DIR_CTRL			0x00
#define CODEC_CLKIN_MASK		0x03
#define MCLK_2_CODEC_CLKIN		0x00
#define PLLCLK_2_CODEC_CLKIN	0x03
#define CODEC_MUX_VALUE			0x03
/*Bclk_in selection*/
#define BDIV_CLKIN_MASK			0x03
#define	DAC_MOD_CLK_2_BDIV_CLKIN 	0x01

#define SOFT_RESET			0x01
#define PAGE0				0x00
#define PAGE1				0x01
#define BIT_CLK_MASTER			0x08
#define WORD_CLK_MASTER			0x04
#define ENABLE_PLL			BIT7
#define ENABLE_NDAC			BIT7
#define ENABLE_MDAC			BIT7
#define ENABLE_NADC			BIT7
#define ENABLE_MADC			BIT7
#define ENABLE_BCLK			BIT7
#define ENABLE_DAC			(0x03 << 6)
#define ENABLE_ADC			BIT7
#define LDAC_2_LCHN			BIT4
#define RDAC_2_RCHN			BIT2
#define RDAC_2_RAMP			BIT2
#define LDAC_2_LAMP			BIT6
#define LDAC_CHNL_2_HPL			BIT3
#define RDAC_CHNL_2_HPR			BIT3
#define LDAC_LCHN_RCHN_2		(BIT4 | BIT5)
#define SOFT_STEP_2WCLK			BIT0

#define MUTE_ON				0x0C
#define DEFAULT_VOL			0x0
#define HEADSET_ON_OFF			0xC0
/* DEFAULT VOL MODIFIED [OLD VAL = 0XFC] */
#define DAC_DEFAULT_VOL			0x81
/* HP DEFAULT VOL Updated from -78.3db to -16db */
#define HP_DEFAULT_VOL			0x20
#define SPK_DEFAULT_VOL			0x75
#define DISABLE_ANALOG			BIT3
#define LDAC_2_HPL_ROUTEON		BIT3
#define RDAC_2_HPR_ROUTEON		BIT3
#define LINEIN_L_2_LMICPGA_10K		BIT6
#define LINEIN_L_2_LMICPGA_20K		BIT7
#define LINEIN_L_2_LMICPGA_40K		(0x3 << 6)
#define LINEIN_R_2_RMICPGA_10K		BIT6
#define LINEIN_R_2_RMICPGA_20K		BIT7
#define LINEIN_R_2_RMICPGA_40K		(0x3 << 6)

/* DAC volume normalization, 0=-127dB, 127=0dB, 175=+48dB */
#define DAC_MAX_VOLUME                  175
#define DAC_POS_VOL                     127
/* Headphone POP Removal Settings defines */
#define HP_POWER_UP_0_USEC	0
#define HP_POWER_UP_15_3_USEC   BIT3
#define HP_POWER_UP_153_USEC    BIT4
#define HP_POWER_UP_1_53_MSEC   (BIT4 | BIT3)
#define HP_POWER_UP_15_3_MSEC   BIT5
#define HP_POWER_UP_76_2_MSEC   (BIT5 | BIT3)
#define HP_POWER_UP_153_MSEC    (BIT5 | BIT4)
#define HP_POWER_UP_304_MSEC    (BIT5 | BIT4 | BIT3)
#define HP_POWER_UP_610_MSEC    (BIT6)
#define HP_POWER_UP_1_2_SEC     (BIT6 | BIT3)
#define HP_POWER_UP_3_04_SEC	(BIT6 | BIT4)
#define HP_POWER_UP_6_1_SEC	(BIT6 | BIT4 | BIT3)
/* Driver Ramp-up Time Settings */
#define HP_DRIVER_0_MS		0
#define HP_DRIVER_0_98_MS	BIT1
#define HP_DRIVER_1_95_MS	BIT2
#define HP_DRIVER_3_9_MS	(BIT2 | BIT1)

/* Common Mode VOltage SEttings */
#define CM_VOLTAGE_FROM_AVDD		0
#define CM_VOLTAGE_FROM_BAND_GAP	BIT0

/****************************************************************************/
/*  DAPM related Enum Defines  		              	  	            */
/****************************************************************************/

#define LEFT_DAC_MUTE_ENUM		0
#define RIGHT_DAC_MUTE_ENUM 		1
#define HP_DRIVER_VOLTAGE_ENUM  	2
#define DRC_STATUS_ENUM         	3
#define ADC_MUTE_ENUM	         	4
#define MICPGA_LP_ENUM			5
#define MICPGA_RP_ENUM			6
#define MICPGA_LM_ENUM			7
#define MICPGA_CM_ENUM			8
#define MIC1LM_ENUM			9
#define MICPGA_GAIN_ENUM		10

/*  List of ramp-up step times */
#define HP_RAMP_UP_STIME_0MS		(0x0 << 1)
#define HP_RAMP_UP_STIME_0_98MS		(0x1 << 1)
#define HP_RAMP_UP_STIME_1_95MS		(0x2 << 1)
#define HP_RAMP_UP_STIME_3_9MS		(0x3 << 1)

/* List of Common-mode voltage settings */
#define HP_RESISTOR_COMMON_MODE 	0x00
#define HP_BANDGAP_COMMON_MODE  	0x01

#define AUDIO_GPIO_INTERRUPT_1		103

#endif /* CODEC_TLV320AIC3100_H */
