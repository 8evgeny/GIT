/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   codec.h
 * Author: Роман
 *
 * Created on 9 января 2021 г., 21:38
 */

#ifndef CODEC_H
#define CODEC_H

#include <stdint.h>

uint8_t codecInit(void);
void selectGoosneckMic(void); // включаем микрофон на IN1R (гусак)
void selectHeadsetMic(void); // включаем микрофон на IN3R (гарнитура)

void headsetSpeakerOn(void); // включает выход на наушники (гарнитуру) (HPR/HPL driver)
void headsetSpeakerOff(void); // выключает выход на наушники (гарнитуру) (HPR/HPL driver)
void lineOutSpeakerOn(void); // включает линейный выход (LOL/LOR Driver)
void lineOutSpeakerOff(void); // выключает линейный выход (LOL/LOR Driver)
void selectMic(uint8_t mic);
void micOn(void);
void micOff(void);
void setPGAgain(unsigned char gain);
void doCodecTest(void);

#define LOL_DRIVER_GAIN    0 // -6..29 dB
#define LOR_DRIVER_GAIN    0 // -6..29 dB 

#define DAC_VOLUME_L     0 // 0 дБ громкость ЦАП левый канал
#define DAC_VOLUME_R     0 //0 дБ громкость ЦАП Правый канал

#define MIC_PGA_GAIN    16 // дБ усиление микрофонного усилителя (0..47 дБ)
#define HEADSET_MIC_PGA_GAIN    10 // дБ усиление микрофонного усилителя гарнитуры (0..47 дБ)

/*!
 * @defgroup Page0_regs Page 0 Register Definitions
 * @brief Defines associated with codec's page 0 registers.
 * @{
 */
#define TI3254_PAGE_SEL_REG             0x00    /*!< Page Select Register  */
#define TI3254_SW_RESET_REG             0x01    /*!< Software Reset Register */
#define TI3254_CLK_MUX_REG              0x04    /*!< Clock Setting Register 1, Multiplexers */
#define TI3254_CLK_PLL_P_R_REG          0x05    /*!< Clock Setting Register 2, PLL P and R Values */
#define TI3254_CLK_PLL_J_REG            0x06    /*!< Clock Setting Register 3, PLL J Values */
#define TI3254_CLK_PLL_D_MSB_REG        0x07    /*!< Clock Setting Register 4, PLL D Values (MSB) */
#define TI3254_CLK_PLL_D_LSB_REG        0x08    /*!< Clock Setting Register 5, PLL D Values (LSB) */
#define TI3254_CLK_NDAC_REG             0x0B    /*!< Clock Setting Register 6, NDAC Values */
#define TI3254_CLK_MDAC_REG             0x0C    /*!< Clock Setting Register 7, MDAC Values */
#define TI3254_DAC_OSR_MSB_REG          0x0D    /*!< DAC OSR Setting Register 1, MSB Value */
#define TI3254_DAC_OSR_LSB_REG          0x0E    /*!< DAC OSR Setting Register 2, LSB Value */
#define TI3254_DSP_D_CTRL_1_REG         0x0F    /*!< miniDSP_D Instruction Control Register 1 */
#define TI3254_DSP_D_CTRL_2_REG         0x10    /*!< miniDSP_D Instruction Control Register 2 */
#define TI3254_DSP_D_INTERPOL_REG       0x11    /*!< miniDSP_D Interpolation Factor Setting Register */
#define TI3254_CLK_NADC_REG             0x12    /*!< Clock Setting Register 8, NADC Values */
#define TI3254_CLK_MADC_REG             0x13    /*!< Clock Setting Register 9, MADC Values */
#define TI3254_ADC_OSR_REG              0x14    /*!< ADC Oversampling (AOSR) Register */
#define TI3254_DSP_A_CTRL_1_REG         0x15    /*!< miniDSP_A Instruction Control Register 1 */
#define TI3254_DSP_A_CTRL_2_REG         0x16    /*!< miniDSP_A Instruction Control Register 2 */
#define TI3254_DSP_A_DEC_FACT_REG       0x17    /*!< miniDSP_A Decimation Factor Setting Register */
#define TI3254_AUDIO_IF_1_REG           0x1B    /*!< Audio Interface Setting Register 1 */
#define TI3254_AUDIO_IF_3_REG           0x1D    /*!< Audio Interface Setting Register 3 */
#define TI3254_BCLK_N_DIV               0x1E    /*!< Clock Setting Register 12, BCLK N Divider */
#define TI3254_DAC_SIG_P_BLK_CTRL_REG   0x3C    /*!< DAC Signal Processing Block Control Register */
#define TI3254_ADC_SIG_P_BLK_CTRL_REG   0x3D    /*!< ADC Signal Processing Block Control Register */
#define TI3254_DAC_CHANNEL_SETUP_1_REG  0x3F    /*!< DAC Channel Setup Register 1 */
#define TI3254_DAC_CHANNEL_SETUP_2_REG  0x40    /*!< DAC Channel Setup Register 2 */
#define TI3254_LEFT_DAC_VOL_CTRL_REG    0x41    /*!< Left DAC Channel Digital Volume Control Register */
#define TI3254_RIGHT_DAC_VOL_CTRL_REG   0x42    /*!< Right DAC Channel Digital Volume Control Register */
#define TI3254_ADC_CHANNEL_SETUP_REG    0x51    /*!< ADC Channel Setup Register */
#define TI3254_ADC_FINE_GAIN_ADJ_REG    0x52    /*!< ADC Fine Gain Adjust Register */
#define TI3254_LEFT_ADC_VOL_CTRL_REG    0x53    /*!< Left ADC Channel Volume Control Register */
#define TI3254_RIGHT_ADC_VOL_CTRL_REG   0x54    /*!< Right ADC Channel Volume Control Register */
/*! @} End Page0_regs */


/*!
 * @defgroup Page1_regs Page 1 Register Definitions
 * @brief Defines associated with codec's page 1 registers.
 * @{
 */
#define TI3254_PWR_CTRL_REG                 0x01    /*!< Power Configuration Register */
#define TI3254_LDO_CTRL_REG                 0x02    /*!< LDO Control Register */
#define TI3254_OP_DRV_PWR_CTRL_REG          0x09    /*!< Output Driver Power Control Register */
#define TI3254_HPL_ROUTING_SEL_REG          0x0C    /*!< HPL Routing Selection Register */
#define TI3254_HPR_ROUTING_SEL_REG          0x0D    /*!< HPR Routing Selection Register */
#define TI3254_LOL_ROUTING_SEL_REG          0x0E    /*!< LOL Routing Selection Register */
#define TI3254_LOR_ROUTING_SEL_REG          0x0F    /*!< LOR Routing Selection Register */
#define TI3254_HPL_DRV_GAIN_CTRL_REG        0x10    /*!< HPL Driver Gain Setting Register */
#define TI3254_HPR_DRV_GAIN_CTRL_REG        0x11    /*!< HPR Driver Gain Setting Register */
#define TI3254_LOL_DRV_GAIN_CTRL_REG        0x12    /*!< LOL Driver Gain Setting Register */
#define TI3254_LOR_DRV_GAIN_CTRL_REG        0x13    /*!< LOR Driver Gain Setting Register */
#define TI3254_HP_DRV_START_UP_CTRL_REG     0x14    /*!< Headphone Driver Startup Control Register */
#define TI3254_MICBIAS_CTRL_REG             0x33    /*!< MICBIAS Configuration Register */
#define TI3254_LEFT_MICPGA_P_CTRL_REG       0x34    /*!< Left MICPGA Positive Terminal Input Routing Configuration Register */
#define TI3254_LEFT_MICPGA_N_CTRL_REG       0x36    /*!< Left MICPGA Negative Terminal Input Routing Configuration Register */
#define TI3254_RIGHT_MICPGA_P_CTRL_REG      0x37    /*!< Right MICPGA Positive Terminal Input Routing Configuration Register */
#define TI3254_RIGHT_MICPGA_N_CTRL_REG      0x39    /*!< Right MICPGA Negative Terminal Input Routing Configuration Register */
#define TI3254_FLOAT_IP_CTRL_REG            0x3a    /*!< Floating Input Configuration Register */
#define TI3254_LEFT_MICPGA_VOL_CTRL_REG     0x3B    /*!< Left MICPGA Volume Control Register */
#define TI3254_RIGHT_MICPGA_VOL_CTRL_REG    0x3C    /*!< Right MICPGA Volume Control Register */
#define TI3254_ANALOG_IP_QCHRG_CTRL_REG     0x47    /*!< Analog Input Quick Charging Configuration Register */
#define TI3254_REF_PWR_UP_CTRL_REG          0x7B    /*!< Reference Power-up Configuration Register */
/*! @} End Page1_regs */

/*!
 * @defgroup Page8_regs Page 8 Register Definitions
 * @brief Defines associated with codec's page 8 registers.
 * @{
 */
#define TI3254_ADC_ADP_FILTER_CTRL_REG  0x01    /*!< ADC Adaptive Filter Configuration Register */
/*! @} End Page8_regs */

/*!
 * @defgroup Page44_regs Page 44 Register Definitions
 * @brief Defines associated with codec's page 44 registers.
 * @{
 */
#define TI3254_DAC_ADP_FILTER_CTRL_REG  0x01    /*!< DAC Adaptive Filter Configuration Register */
/*! @} End Page44_regs */

#endif /* CODEC_H */
