/**
  ******************************************************************************
  * @file    MDR32F9Qx_adciu.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   This file contains all the ADCIU firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_adciu.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @addtogroup ADCIU ADCIU
  * @{
  */

/** @defgroup ADIUC_Private_Functions ADCIU Private Functions
  * @{
  */

/**
  * @brief  Deinitializes the ADCIU peripheral registers to their default reset values.
  * @param  None.
  * @retval None.
  */
void ADCIU_DeInit(void)
{
    /* Reset digital block */
    MDR_ADCIU->CTRL2   = ADCIU_CTRL2_RESET;
    MDR_ADCIU->CTRL2   &= ~ADCIU_CTRL2_RESET;

    MDR_ADCIU->CTRL1   = 0x00;
    MDR_ADCIU->ANGAIN  = 0x00;
    MDR_ADCIU->DIGGAIN = 0x2A;
    MDR_ADCIU->DMAEN   = 0x00;
    MDR_ADCIU->CTRL2   = 0x00;
    MDR_ADCIU->INTEN   = 0x00;
    MDR_ADCIU->STAT    = 0x00;
}

/**
  * @brief  Initializes the ADCIU peripheral according to
  *         the specified parameters in the ADCIU_InitStruct.
  * @param  ADCIU_InitStruct: pointer to a @ref ADCIU_InitTypeDef structure that
  *         contains the configuration information for the specified ADCIU peripheral.
  * @retval None.
  */
void ADCIU_Init(ADCIU_InitTypeDef* ADCIU_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_ADCIU_REFEREVCE_VOLTAGE(ADCIU_InitStruct->ADCIU_VoltageReference));
    assert_param(IS_ADCIU_CHOPPER_FREQUENCY_DIVIDER(ADCIU_InitStruct->ADCIU_ChopperFrequencyDivider));
    assert_param(IS_ADCIU_BYPASS_BUFFER_REFERNCE_CONF(ADCIU_InitStruct->ADCIU_BypassBufferReference));
    assert_param(IS_ADCIU_DGAIN(ADCIU_InitStruct->ADCIU_DigitalGain));
    assert_param(IS_ADCIU_SFC(ADCIU_InitStruct->ADCIU_SFCDivider));
    assert_param(IS_ADCIU_SFF(ADCIU_InitStruct->ADCIU_SFFDivider));

    /* Set reference voltage source */
    MDR_ADCIU->CTRL1 = ADCIU_InitStruct->ADCIU_VoltageReference;

    /* Set ADCIU CTRL2 */
    MDR_ADCIU->CTRL2 = ADCIU_InitStruct->ADCIU_ChopperFrequencyDivider
                     | ADCIU_InitStruct->ADCIU_BypassBufferReference
                     | ADCIU_InitStruct->ADCIU_SFCDivider
                     | ADCIU_InitStruct->ADCIU_SFFDivider;

    /* Set ADCIU DIGGAIN */
    MDR_ADCIU->DIGGAIN = ADCIU_InitStruct->ADCIU_DigitalGain;
}

/**
  * @brief Initializes the ADCIU channel peripheral according to
  *        the specified parameters in the ADCIU_ChannelInitStruct.
  * @param ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the number of the initialized channel.
  * @param ADCIU_ChannelInitStruct: pointer to a @ref ADCIU_ChannelInitTypeDef
  *        structure that contains the configuration information.
  * @retval None.
  */
void ADCIU_InitChannel(ADCIU_Channel_Number_TypeDef ChannelNumber, ADCIU_ChannelInitTypeDef* ADCIU_ChannelInitStruct)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));
    assert_param(IS_FUNCTIONAL_STATE(ADCIU_ChannelInitStruct->ADCIU_ChopperState));
    assert_param(IS_ADCIU_AGAIN(ADCIU_ChannelInitStruct->ADCIU_AnalogGain));
    assert_param(IS_FUNCTIONAL_STATE(ADCIU_ChannelInitStruct->ADCIU_DMARequest));
    assert_param(IS_FUNCTIONAL_STATE(ADCIU_ChannelInitStruct->ADCIU_IT_FIFO_OverflowState));
    assert_param(IS_FUNCTIONAL_STATE(ADCIU_ChannelInitStruct->ADCIU_IT_FIFO_OverflowState));

    /* Set mode to eliminate the constant component  */
    if(ADCIU_ChannelInitStruct->ADCIU_ChopperState != DISABLE)
    {
        MDR_ADCIU->CTRL1 |= 1 << ((uint32_t)ChannelNumber + ADCIU_CTRL1_CHP1EN_Pos);
    }
    else
    {
        MDR_ADCIU->CTRL1 &= ~(1 << ((uint32_t)ChannelNumber + ADCIU_CTRL1_CHP1EN_Pos));
    }

    /* Set new analog gain value */
    tmpreg = MDR_ADCIU->ANGAIN & ~(ADCIU_ANGAIN_ADC1PGA_Msk << (2 * (uint32_t)ChannelNumber + ADCIU_ANGAIN_ADC1PGA_Pos));
    tmpreg |= ADCIU_ChannelInitStruct->ADCIU_AnalogGain << (2 * (uint32_t)ChannelNumber + ADCIU_ANGAIN_ADC1PGA_Pos);
    MDR_ADCIU->ANGAIN = tmpreg;

    /* Set dma request when there is data in the FIFO */
    if(ADCIU_ChannelInitStruct->ADCIU_DMARequest != DISABLE)
    {
        MDR_ADCIU->DMAEN |= 1 << (uint32_t)ChannelNumber;
    }
    else
    {
        MDR_ADCIU->DMAEN &= ~(1 << (uint32_t)ChannelNumber);
    }

    /* Config interrupt config */
    if(ADCIU_ChannelInitStruct->ADCIU_IT_FIFO_NotEmptyState != DISABLE)
    {
        MDR_ADCIU->INTEN |= 1 << (uint32_t)ChannelNumber;
    }
    else
    {
        MDR_ADCIU->INTEN &= ~(1 << (uint32_t)ChannelNumber);
    }

    /* Config interrupt config */
    if(ADCIU_ChannelInitStruct->ADCIU_IT_FIFO_OverflowState != DISABLE)
    {
        MDR_ADCIU->INTEN |= 1 << ((uint32_t)ChannelNumber + ADCIU_INTEN_OV1M_Pos);
    }
    else
    {
        MDR_ADCIU->INTEN &= ~(1 << ((uint32_t)ChannelNumber + ADCIU_INTEN_OV1M_Pos));
    }
}

/**
  * @brief  Fills each ADCIU_InitStruct member with its default value.
  * @param  ADCIU_InitStruct: pointer to a @ref ADCIU_InitStruct structure
  *         which will be initialized.
  * @retval None.
  */
void ADCIU_StructInit(ADCIU_InitTypeDef* ADCIU_InitStruct)
{
    ADCIU_InitStruct->ADCIU_VoltageReference        = ADCIU_REFEREVCE_VOLTAGE_INTERNAL;
    ADCIU_InitStruct->ADCIU_ChopperFrequencyDivider = ADCIU_CHOPPER_FREQUENCY_DIVIDER4;
    ADCIU_InitStruct->ADCIU_BypassBufferReference   = ADCIU_BYPASS_BUFFER_REFERNCE_DISABLE;
    ADCIU_InitStruct->ADCIU_DigitalGain             = 0x2A;
    ADCIU_InitStruct->ADCIU_SFCDivider              = ADCIU_SFC0;
    ADCIU_InitStruct->ADCIU_SFFDivider              = 0;
}

/**
  * @brief Fills each ADCIU_ChannelInitStruct member with its default value.
  * @param ADCIU_ChannelInitStruct: pointer to a @ref ADCIU_ChannelInitTypeDef
  *        structure which will be initialized.
  * @retval None.
  */
void ADCIU_ChannelStructInit(ADCIU_ChannelInitTypeDef* ADCIU_ChannelInitStruct)
{
    ADCIU_ChannelInitStruct->ADCIU_ChopperState          = DISABLE;
    ADCIU_ChannelInitStruct->ADCIU_AnalogGain            = ADCIU_AGAIN0dB;
    ADCIU_ChannelInitStruct->ADCIU_DMARequest            = DISABLE;
    ADCIU_ChannelInitStruct->ADCIU_IT_FIFO_OverflowState = DISABLE;
    ADCIU_ChannelInitStruct->ADCIU_IT_FIFO_NotEmptyState = DISABLE;
}

/**
  * @brief  Enables or disables the ADCIUx channel.
  * @param  ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the numbers of channel.
  * @param  NewState - @ref FunctionalState - new state of the ADCIUx channel.
  * @retval None.
  */
void ADCIU_ChannelCmd(ADCIU_Channel_Number_TypeDef ChannelNumber, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState != DISABLE)
    {
        MDR_ADCIU->CTRL1 |= 1 << ChannelNumber;
    }
    else
    {
        MDR_ADCIU->CTRL1 &= ~(1 << ChannelNumber);
    }
}

/**
  * @brief  Enables or disables the ADCIUx channel chopper.
  * @param  ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the number of channel.
  * @param  NewState - @ref FunctionalState - new state of the ADCIUx channel.
  * @retval None.
  */
void ADCIU_ChopperCmd(ADCIU_Channel_Number_TypeDef ChannelNumber, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Set mode to eliminate the constant component */
    if(NewState != DISABLE)
    {
        MDR_ADCIU->CTRL1 |= 1 << (ADCIU_CTRL1_CHP1EN_Pos + ChannelNumber);
    }
    else
    {
        MDR_ADCIU->CTRL1 &= ~(1 << (ADCIU_CTRL1_CHP1EN_Pos + ChannelNumber));
    }
}

/**
  * @brief  Set the analog gain for the specified channel.
  * @param  ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the number of channel.
  * @param  gain - @ref ADCIU_AGain_TypeDef - the new value of the analog gain.
  * @retval None.
  */
void ADCIU_AGain(ADCIU_Channel_Number_TypeDef ChannelNumber, ADCIU_AGain_TypeDef gain)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));
    assert_param(IS_ADCIU_AGAIN(gain));

    /* Set analog gain */
    tmpreg = MDR_ADCIU->ANGAIN;
    tmpreg &= ~(ADCIU_ANGAIN_ADC1PGA_Msk << (uint32_t)ChannelNumber * 2);
    tmpreg |= gain << (uint32_t)ChannelNumber * 2;

    /* Set ADCIU AGAIN */
    MDR_ADCIU->ANGAIN = tmpreg;
}

/**
  * @brief  Enables or disables the ADCIU interrupts.
  * @param  ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the channel number.
  * @param  ADCIU_IT - @ref ADCIU_IT_TypeDef - specifies the ADCIU interrupts sources to be enabled or disabled.
  * @param  NewState - @ref FunctionalState - new state of the ADCIU interrupts.
  * @retval None.
  */
void ADCIU_ITConfig(ADCIU_Channel_Number_TypeDef ChannelNumber, ADCIU_IT_TypeDef ADCIU_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));
    assert_param(IS_ADCIU_IT(ADCIU_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    /* Config IT state */
    if(NewState != DISABLE)
    {
        MDR_ADCIU->INTEN |= ((uint32_t)ADCIU_IT << ChannelNumber);
    }
    else
    {
        MDR_ADCIU->INTEN &= ~((uint32_t)ADCIU_IT << ChannelNumber);
    }

}

/**
  * @brief  Clears the ADCIU OVERFLOW interupt flag.
  * @param  ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the channel number.
  * @retval None.
  */
void ADCIU_ClearIT_OVERFLOW(ADCIU_Channel_Number_TypeDef ChannelNumber)
{
    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));

    MDR_ADCIU->STAT |= ADCIU_IT_FIFO_OVERFLOW << ChannelNumber;
}

/**
  * @brief  Enables or disables the ADCIUx channel DMA request.
  * @param  ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the channel number.
  * @param  NewState - @ref FunctionalState - new state of the ADCIUx channel.
  * @retval None.
  */
void ADCIU_DMARequestCmd(ADCIU_Channel_Number_TypeDef ChannelNumber, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState != DISABLE)
    {
        MDR_ADCIU->DMAEN |= (1 << ChannelNumber);
    }
    else
    {
        MDR_ADCIU->DMAEN &= ~(1 << ChannelNumber);
    }
}

/**
  * @brief  Checks whether the specified ADCIU Status flag is set or not.
  * @param  ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the channel number.
  * @param  ADCIU_STATUS_FLAG - @ref ADCIU_Flags_TypeDef - specifies the flag to check.
  * @retval Current Status flag state (SET or RESET).
  */
FlagStatus ADCIU_GetStatusFlag(ADCIU_Channel_Number_TypeDef ChannelNumber, ADCIU_Flags_TypeDef ADCIU_STATUS_FLAG)
{
    FlagStatus flag = RESET;

    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));
    assert_param(IS_ADCIU_FLAG(ADCIU_STATUS_FLAG));

    if(MDR_ADCIU->STAT & (ADCIU_STATUS_FLAG << ChannelNumber))
    {
        flag = SET;
    }

    return (flag);
}

/**
  * @brief  Returns the ADCIU result of specific channel.
  * @param  ChannelNumber - @ref ADCIU_Channel_Number_TypeDef - the channel number.
  * @return ADCIU Result Register value for the specified channel.
  */
uint32_t ADCIU_GetResult(ADCIU_Channel_Number_TypeDef ChannelNumber)
{
    /* Check the parameters */
    assert_param(IS_ADCIU_CHANNEL_NUMBER(ChannelNumber));

    return (MDR_ADCIU->F_DAT[ChannelNumber]);
}

/** @} */ /* End of group ADCIU_Private_Functions */

/** @} */ /* End of group ADCIU */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_adciu.c */
