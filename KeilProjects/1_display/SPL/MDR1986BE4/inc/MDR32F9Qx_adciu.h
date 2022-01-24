/**
  ******************************************************************************
  * @file    MDR32F9Qx_adciu.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains all the functions prototypes for the ADCIU
  *          firmware library.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MDR32F9QX_ADCIU_H
#define MDR32F9QX_ADCIU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @addtogroup ADCIU ADCIU
  * @{
  */

/** @defgroup ADCIU_Exported_Types ADCIU Exported Types
  * @{
  */

/**
  * @brief ADCIU Reference Voltage Configuration
  */
typedef enum
{
    ADCIU_REFEREVCE_VOLTAGE_INTERNAL = (uint32_t)(0 << ADCIU_CTRL1_REF_EX_Pos),
    ADCIU_REFEREVCE_VOLTAGE_EXTERNAL = (uint32_t)(1 << ADCIU_CTRL1_REF_EX_Pos)
} ADC_VRef_Config_TypeDef;

#define IS_ADCIU_REFEREVCE_VOLTAGE(RV)      ((RV == ADCIU_REFEREVCE_VOLTAGE_INTERNAL) || \
                                             (RV == ADCIU_REFEREVCE_VOLTAGE_EXTERNAL))

/**
  * @brief ADCIU Chopper Frequency Divider
  */
typedef enum
{
    ADCIU_CHOPPER_FREQUENCY_DIVIDER4  = (0 << ADCIU_CTRL2_CHOP_FREQ_Pos),
    ADCIU_CHOPPER_FREQUENCY_DIVIDER8  = (1 << ADCIU_CTRL2_CHOP_FREQ_Pos),
    ADCIU_CHOPPER_FREQUENCY_DIVIDER16 = (2 << ADCIU_CTRL2_CHOP_FREQ_Pos),
    ADCIU_CHOPPER_FREQUENCY_DIVIDER32 = (3 << ADCIU_CTRL2_CHOP_FREQ_Pos)
} ADCIU_Chopper_Freq_Divider_TypeDef;

#define IS_ADCIU_CHOPPER_FREQUENCY_DIVIDER(FD)        ((FD & (~ADCIU_CTRL2_CHOP_FREQ_Msk)) == 0)

/**
  * @brief ADCIU Bypass buffer reference configuration
  */
typedef enum
{
    ADCIU_BYPASS_BUFFER_REFERNCE_ENABLE  = (1 << ADCIU_CTRL2_BF_BP_Pos),
    ADCIU_BYPASS_BUFFER_REFERNCE_DISABLE = (0 << ADCIU_CTRL2_BF_BP_Pos)
} ADCIU_Bypass_Buffer_Ref_Config_TypeDef;

#define IS_ADCIU_BYPASS_BUFFER_REFERNCE_CONF(BBRF)  ((BBRF == ADCIU_BYPASS_BUFFER_REFERNCE_ENABLE) || \
                                                     (BBRF == ADCIU_BYPASS_BUFFER_REFERNCE_DISABLE))

/**
  * @brief ADCIU SFC divider
  */
typedef enum
{
    ADCIU_SFC0 = (0 << ADCIU_CTRL2_SFC_Pos),
    ADCIU_SFC1 = (1 << ADCIU_CTRL2_SFC_Pos),
    ADCIU_SFC2 = (2 << ADCIU_CTRL2_SFC_Pos),
    ADCIU_SFC3 = (3 << ADCIU_CTRL2_SFC_Pos),
    ADCIU_SFC4 = (4 << ADCIU_CTRL2_SFC_Pos)
} ADCIU_SFC_Divider_TypeDef;

#define IS_ADCIU_SFC(SFC)   ((SFC == ADCIU_SFC0) || \
                             (SFC == ADCIU_SFC1) || \
                             (SFC == ADCIU_SFC2) || \
                             (SFC == ADCIU_SFC3) || \
                             (SFC == ADCIU_SFC4))

/**
  * @brief ADCIU Channel number
  */
typedef enum
{
    ADCIU_CHANNEL_NUMBER1 = (uint32_t)(0x00),
    ADCIU_CHANNEL_NUMBER2 = (uint32_t)(0x01),
    ADCIU_CHANNEL_NUMBER3 = (uint32_t)(0x02),
    ADCIU_CHANNEL_NUMBER4 = (uint32_t)(0x03),
    ADCIU_CHANNEL_NUMBER5 = (uint32_t)(0x04),
    ADCIU_CHANNEL_NUMBER6 = (uint32_t)(0x05),
    ADCIU_CHANNEL_NUMBER7 = (uint32_t)(0x06),
    ADCIU_CHANNEL_NUMBER8 = (uint32_t)(0x07)
} ADCIU_Channel_Number_TypeDef;

#define IS_ADCIU_CHANNEL_NUMBER(CHN)    ((CHN >= ADCIU_CHANNEL_NUMBER1) && \
                                         (CHN <= ADCIU_CHANNEL_NUMBER8))

/**
  * @brief ADCIU status flags
  */
typedef enum
{
    ADCIU_FLAG_FIFO_OVERFLOW  = (uint32_t)(1 << 8),
    ADCIU_FLAG_FITO_NOT_EMPTY = (uint32_t)(1 << 0)
} ADCIU_Flags_TypeDef;

#define IS_ADCIU_FLAG(F)    ((F == ADCIU_FLAG_FIFO_OVERFLOW) || \
                             (F == ADCIU_FLAG_FITO_NOT_EMPTY))

/**
  * @brief ADCIU IT
  */
typedef enum
{
    ADCIU_IT_FIFO_OVERFLOW  = (uint32_t)(1 << 8),
    ADCIU_IT_FITO_NOT_EMPTY = (uint32_t)(1 << 0)
} ADCIU_IT_TypeDef;

#define IS_ADCIU_IT(IT)     ((IT == ADCIU_IT_FIFO_OVERFLOW) || \
                             (IT == ADCIU_IT_FITO_NOT_EMPTY))

/**
  * @brief ADCIU Analog gain
  */
typedef enum
{
    ADCIU_AGAIN0dB  = (uint32_t)(0x00),
    ADCIU_AGAIN6dB  = (uint32_t)(0x02),
    ADCIU_AGAIN12dB = (uint32_t)(0x01),
    ADCIU_AGAIN24dB = (uint32_t)(0x03)
} ADCIU_AGain_TypeDef;

#define IS_ADCIU_AGAIN(AGAIN)   ((AGAIN == ADCIU_AGAIN0dB)  || \
                                 (AGAIN == ADCIU_AGAIN6dB)  || \
                                 (AGAIN == ADCIU_AGAIN12dB) || \
                                 (AGAIN == ADCIU_AGAIN24dB))

/**
 * @brief ADCIU Init structure defintion
 */
typedef struct
{
    ADC_VRef_Config_TypeDef                ADCIU_VoltageReference;        /*!< Select the reference voltage source.
                                                                               This parameter can be a value of @ref ADC_VRef_Config_TypeDef. */
    ADCIU_Chopper_Freq_Divider_TypeDef     ADCIU_ChopperFrequencyDivider; /*!< Choosing scaler mode eliminate the constant component.
                                                                               This parameter can be a value of @ref ADCIU_Chopper_Freq_Divider_TypeDef. */
    ADCIU_Bypass_Buffer_Ref_Config_TypeDef ADCIU_BypassBufferReference;   /*!< Enable or disable bypass buffers reference.
                                                                               This parameter can be a value of  @ref ADCIU_Bypass_Buffer_Ref_Config_TypeDef*/
    uint32_t                               ADCIU_DigitalGain;             /*!< Determines the level of digital amplification.
                                                                               This parameter can be a number between 0x0000 and 0x3F. */
    ADCIU_SFC_Divider_TypeDef              ADCIU_SFCDivider;              /*!< Set the SFC divider.
                                                                               This parameter can be a value of @ref ADCIU_SFC_Divider_TypeDef. */
    uint32_t                               ADCIU_SFFDivider;              /*!< Set the SFF divider.
                                                                               This parameter can be a number between 0x0000 and 0x7F. */
} ADCIU_InitTypeDef;

/**
 * @brief ADCIU Channel init structure
 */
typedef struct
{
    FunctionalState     ADCIU_ChopperState;          /*!< Enable or disable mode eliminate of the constant component.
                                                          This parameter can be a value of FunctionalState */
    ADCIU_AGain_TypeDef ADCIU_AnalogGain;            /*!< Specifies the level of analog gain.
                                                          This parameter can be a value of @ref ADCIU_AGain_TypeDef */
    FunctionalState     ADCIU_DMARequest;            /*!< Enable or disable dma request when there is data in the FIFO.
                                                          This parameter can be a value of FunctionalState */
    FunctionalState     ADCIU_IT_FIFO_OverflowState; /*!< Enable or disable FIFO overflow interrupt.
                                                          This parameter can be a value of FunctionalState */
    FunctionalState     ADCIU_IT_FIFO_NotEmptyState; /*!< Enable or disable FIFO data present interrupt.
                                                          This parameter can be a value of FunctionalState */
} ADCIU_ChannelInitTypeDef;

/** @} */ /* End of group ADCIU_Exported_Types */


/** @defgroup ADCIU_Exported_Constants ADCIU Exported Constants
  * @{
  */

/** @defgroup ADCIU_DGAIN ADCIU Digital gain
  * @{
  */

#define IS_ADCIU_DGAIN(DGAIN)   (((DGAIN) & (~ADCIU_DIGGAN_ADCPGA_Msk)) == 0)

/** @} */ /* End of group ADCIU_DGAIN */

/** @defgroup ADCIU_SFF_divider ADCIU SFF divider
  * @{
  */

#define IS_ADCIU_SFF(SFF)       ((SFF & (~ADCIU_CTRL2_SFF_Msk)) == 0)

/** @} */ /* End of group ADCIU_SFF_divider */

/** @} */ /* End of group ADCIU_Exported_Constants */


/** @defgroup ADCIU_Exported_Functions ADCIU Exported Functions
  * @{
  */

void ADCIU_DeInit(void);
void ADCIU_Init(ADCIU_InitTypeDef* ADCIU_InitStruct);
void ADCIU_InitChannel(ADCIU_Channel_Number_TypeDef ChannelNumber, ADCIU_ChannelInitTypeDef* ADCIU_ChannelInitStruct);
void ADCIU_StructInit(ADCIU_InitTypeDef* ADCIU_InitStruct);
void ADCIU_ChannelStructInit(ADCIU_ChannelInitTypeDef* ADCIU_ChannelInitStruct);
void ADCIU_ChannelCmd(ADCIU_Channel_Number_TypeDef ChannelNumber, FunctionalState NewState);
void ADCIU_ChopperCmd(ADCIU_Channel_Number_TypeDef ChannelNumber, FunctionalState NewState);
void ADCIU_AGain(ADCIU_Channel_Number_TypeDef ChannelNumber, ADCIU_AGain_TypeDef gain);
void ADCIU_ITConfig(ADCIU_Channel_Number_TypeDef ChannelNumber, ADCIU_IT_TypeDef ADCIU_IT, FunctionalState NewState);
void ADCIU_ClearIT_OVERFLOW(ADCIU_Channel_Number_TypeDef ChannelNumber);
void ADCIU_DMARequestCmd(ADCIU_Channel_Number_TypeDef ChannelNumber, FunctionalState NewState);
FlagStatus ADCIU_GetStatusFlag(ADCIU_Channel_Number_TypeDef ChannelNumber, ADCIU_Flags_TypeDef ADCIU_STATUS_FLAG);
uint32_t ADCIU_GetResult(ADCIU_Channel_Number_TypeDef ChannelNumber);

/** @} */ /* End of group ADCIU_Exported_Functions */

/** @} */ /* End of group ADCIU */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* MDR32F9QX_ADCIU_H */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_adciu.h */
