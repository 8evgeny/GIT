/**
  ******************************************************************************
  * @file    MDR32F9Qx_adc.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   This file contains all the ADC firmware functions.
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
#include "MDR32F9Qx_adc.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @defgroup ADC ADC
  * @{
  */

/** @defgroup ADC_Private_Functions ADC Private Functions
  * @{
  */

/**
  * @brief  Deinitializes the ADC peripheral registers to their default reset values.
  * @param  None.
  * @retval None.
  */
void ADC_DeInit(void)
{
    MDR_ADC->ADC1_CFG     = 0;
    MDR_ADC->ADC2_CFG     = 0;
    MDR_ADC->ADC1_H_LEVEL = 0;
    MDR_ADC->ADC1_L_LEVEL = 0;
    MDR_ADC->ADC1_RESULT;
    MDR_ADC->ADC1_STATUS  = 0;
    MDR_ADC->ADC1_CHSEL   = 0;
    MDR_ADC->ADC1_TRIM    = 0;
}

/**
  * @brief  Initializes the ADC peripheral according to
  *         the specified parameters in the ADC_InitStruct.
  * @param  ADC_InitStruct: pointer to a @ref ADC_InitTypeDef structure that
            contains the configuration information for the specified ADC peripheral.
  * @retval None
  */
void ADC_Init(const ADC_InitTypeDef* ADC_InitStruct)
{
    uint32_t tmpreg_CFG;
    uint32_t tmpreg_MSK;
    uint32_t tmpreg_TRIM;

    /* Check the parameters */
    assert_param(IS_ADC_TEMP_SENSOR_CONFIG(ADC_InitStruct->ADC_TempSensor));
    assert_param(IS_ADC_TEMP_SENSOR_AMP_CONFIG(ADC_InitStruct->ADC_TempSensorAmplifier));
    assert_param(IS_ADC_TEMP_SENSOR_CONVERSION_CONFIG(ADC_InitStruct->ADC_TempSensorConversion));
    assert_param(IS_ADC_VREF_CONVERSION_CONFIG(ADC_InitStruct->ADC_IntVRefConversion));
    assert_param(IS_ADC_VREF_TRIMMING_VALUE(ADC_InitStruct->ADC_IntVRefTrimming));
    assert_param(IS_ADC_INT_VREF_AMPLIFIER(ADC_InitStruct->ADC_IntVRefAmplifier));

    tmpreg_CFG = ADC_InitStruct->ADC_TempSensor
               + ADC_InitStruct->ADC_TempSensorAmplifier
               + ADC_InitStruct->ADC_TempSensorConversion
               + ADC_InitStruct->ADC_IntVRefConversion;

    tmpreg_MSK = ADC1_CFG_DELAY_ADC_Msk
               | ADC1_CFG_TS_EN
               | ADC1_CFG_TS_BUF_EN
               | ADC1_CFG_SEL_TS
               | ADC1_CFG_SEL_VREF;

    tmpreg_TRIM = (ADC_InitStruct->ADC_IntVRefTrimming << ADC1_TRIM_TS_TRIM_Pos)
                 | ADC_InitStruct->ADC_IntVRefAmplifier;

    MDR_ADC->ADC1_TRIM  = tmpreg_TRIM;
    MDR_ADC->ADC1_CFG = (MDR_ADC->ADC1_CFG & ~tmpreg_MSK) + tmpreg_CFG;
}

/**
  * @brief  Fills each ADC_InitStruct member with its default value.
  * @param  ADC_InitStruct: pointer to a @ref ADC_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
#if defined (MDR_LEGACY_SUPPORT) && (MDR_LEGACY_SUPPORT) == 153
    ADC_InitStruct->ADC_StartDelay           = 0;
#endif /* MDR_LEGACY_SUPPORT 153 */
    ADC_InitStruct->ADC_TempSensor           = ADC_TEMP_SENSOR_Disable;
    ADC_InitStruct->ADC_TempSensorAmplifier  = ADC_TEMP_SENSOR_AMPLIFIER_Disable;
    ADC_InitStruct->ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Disable;
    ADC_InitStruct->ADC_IntVRefConversion    = ADC_VREF_CONVERSION_Disable;
    ADC_InitStruct->ADC_IntVRefTrimming      = 0;
    ADC_InitStruct->ADC_IntVRefAmplifier     = ADC_INT_VREF_AMPLIFIER_Disable;
}

/**
  * @brief  Sets the Internal Voltage Reference trimming.
  * @param  Trim: trimming value in range 0..7.
  * @retval None
  */
void ADC_SetTrim(uint32_t Trim)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_ADC_VREF_TRIMMING_VALUE(Trim));

    tmpreg = MDR_ADC->ADC1_TRIM;
    tmpreg &= ~ADC1_TRIM_TS_TRIM_Msk;
    tmpreg |= Trim << ADC1_TRIM_TS_TRIM_Pos;

    MDR_ADC->ADC1_TRIM = tmpreg;
}

/**
  * @brief  Initializes the ADC1 peripheral according to
  *         the specified parameters in the ADCx_InitStruct.
  * @param  ADCx_InitStruct: pointer to a @ref ADCx_InitTypeDef structure
  *         that contains the configuration information for the ADC1 peripheral.
  * @retval None
  */
void ADC1_Init(const ADCx_InitTypeDef* ADCx_InitStruct)
{
    uint32_t tmpreg_CFG1;
    uint32_t tmpreg_CFG2;

    /* Check the parameters */
    assert_param(IS_ADC_CLOCK_SOURCE_CONFIG(ADCx_InitStruct->ADC_ClockSource));
    assert_param(IS_ADC_SAMPLING_MODE_CONFIG(ADCx_InitStruct->ADC_SamplingMode));
    assert_param(IS_ADC_CH_SWITCHING_CONFIG(ADCx_InitStruct->ADC_ChannelSwitching));
    assert_param(IS_ADC1_CH_NUM(ADCx_InitStruct->ADC_ChannelNumber));
    assert_param(IS_ADC1_CH_MASK(ADCx_InitStruct->ADC_Channels));
    assert_param(IS_ADC_LEVEL_CONTROL_CONFIG(ADCx_InitStruct->ADC_LevelControl));
    assert_param(IS_ADC_VALUE(ADCx_InitStruct->ADC_LowLevel));
    assert_param(IS_ADC_VALUE(ADCx_InitStruct->ADC_HighLevel));
    assert_param(IS_ADC_VREF_SOURCE_CONFIG(ADCx_InitStruct->ADC_VRefSource));
    assert_param(IS_ADC_INT_VREF_SOURCE_CONFIG(ADCx_InitStruct->ADC_IntVRefSource));
    assert_param(IS_ADC_CLK_div_VALUE(ADCx_InitStruct->ADC_Prescaler));
    assert_param(IS_ADC_DELAY_GO_VALUE(ADCx_InitStruct->ADC_DelayGo));

    tmpreg_CFG1 = MDR_ADC->ADC1_CFG;

    tmpreg_CFG1 &= ~(ADC1_CFG_REG_CLKS
                   | ADC1_CFG_REG_SAMPLE
                   | ADC1_CFG_REG_CHCH
                   | ADC1_CFG_REG_CHS_Msk
                   | ADC1_CFG_REG_RNGC
                   | ADC1_CFG_M_REF
                   | ADC1_CFG_REG_DIVCLK_Msk
                   | ADC1_CFG_DELAY_GO_Msk);

    tmpreg_CFG1 += ADCx_InitStruct->ADC_ClockSource
                 + ADCx_InitStruct->ADC_SamplingMode
                 + ADCx_InitStruct->ADC_ChannelSwitching
                 + (ADCx_InitStruct->ADC_ChannelNumber << ADC1_CFG_REG_CHS_Pos)
                 + ADCx_InitStruct->ADC_LevelControl
                 + ADCx_InitStruct->ADC_VRefSource
                 + ADCx_InitStruct->ADC_Prescaler
                 + (ADCx_InitStruct->ADC_DelayGo << ADC1_CFG_DELAY_GO_Pos);

    tmpreg_CFG2 = MDR_ADC->ADC2_CFG;
    tmpreg_CFG2 &= ~ADC2_CFG_ADC1_OP;
    tmpreg_CFG2 += ADCx_InitStruct->ADC_IntVRefSource << ADC2_CFG_ADC1_OP_Pos;

    MDR_ADC->ADC1_L_LEVEL = ADCx_InitStruct->ADC_LowLevel;
    MDR_ADC->ADC1_H_LEVEL = ADCx_InitStruct->ADC_HighLevel;
    MDR_ADC->ADC1_CHSEL   = ADCx_InitStruct->ADC_Channels;

    MDR_ADC->ADC1_CFG = tmpreg_CFG1;
    MDR_ADC->ADC2_CFG = tmpreg_CFG2;
}

/**
  * @brief  Fills each ADCx_InitStruct member with its default value.
  * @param  ADCx_InitStruct: pointer to a @ref ADCx_InitTypeDef structure
  *         which will be initialized.
  * @retval None
  */
void ADCx_StructInit(ADCx_InitTypeDef* ADCx_InitStruct)
{
    ADCx_InitStruct->ADC_ClockSource      = ADC_CLOCK_SOURCE_CPU;
    ADCx_InitStruct->ADC_SamplingMode     = ADC_SAMPLING_MODE_SINGLE_CONV;
    ADCx_InitStruct->ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;
    ADCx_InitStruct->ADC_ChannelNumber    = ADC_CH_ADC0;
    ADCx_InitStruct->ADC_Channels         = 0;
    ADCx_InitStruct->ADC_LevelControl     = ADC_LEVEL_CONTROL_Disable;
    ADCx_InitStruct->ADC_LowLevel         = 0;
    ADCx_InitStruct->ADC_HighLevel        = 0;
    ADCx_InitStruct->ADC_VRefSource       = ADC_VREF_SOURCE_INTERNAL;
    ADCx_InitStruct->ADC_IntVRefSource    = ADC_INT_VREF_SOURCE_INEXACT;
    ADCx_InitStruct->ADC_Prescaler        = ADC_CLK_div_None;
    ADCx_InitStruct->ADC_DelayGo          = 0;
}

/**
  * @brief  Enables or disables the ADC1 peripheral.
  * @param  NewState - @ref FunctionalState - new state of the ADC1 peripheral.
  * @retval None
  */
void ADC1_Cmd(FunctionalState NewState)
{
    uint32_t tmpreg_CFG;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmpreg_CFG = MDR_ADC->ADC1_CFG;

    /* Form new value */
    if (NewState != DISABLE)
    {
        /* Enable ADC1 by setting the Cfg_REG_ADON bit in the ADC1_CFG register */
        tmpreg_CFG |= ADC1_CFG_REG_ADON;
    }
    else
    {
        /* Disable ADC1 by resetting the Cfg_REG_ADON bit in the ADC1_CFG register */
        tmpreg_CFG &= ~ADC1_CFG_REG_ADON;
    }

    /* Configure ADC1_CFG register with new value */
    MDR_ADC->ADC1_CFG = tmpreg_CFG;
}

/**
  * @brief  Selects the ADC1 Channel number for Single Channel Mode conversion.
  * @param  Channel - @ref ADCx_Channel_Number - specifies the ADC Channel number.
  * @retval None
  */
void ADC1_SetChannel(ADCx_Channel_Number Channel)
{
    uint32_t tmpreg_CFG;

    /* Check the parameters */
    assert_param(IS_ADC1_CH_NUM(Channel));

    tmpreg_CFG = MDR_ADC->ADC1_CFG;
    tmpreg_CFG &= ~ADC1_CFG_REG_CHS_Msk;
    tmpreg_CFG += (uint32_t)Channel << ADC1_CFG_REG_CHS_Pos;

    MDR_ADC->ADC1_CFG = tmpreg_CFG;
}

/**
  * @brief  Selects the ADC1 Channels for conversion with Channels switching.
  * @param  ChannelMask: specifies the ADC Channels Mask.
  *         This parameter can be any combination of @ref ADCx_Channel_Number values.
  * @retval None
  */
void ADC1_SetChannels(uint32_t ChannelMask)
{
    /* Check the parameters */
    assert_param(IS_ADC1_CH_MASK(ChannelMask));

    MDR_ADC->ADC1_CHSEL = ChannelMask;
}

/**
  * @brief  Sets the ADC1 operation mode.
  * @param  SamplingMode - @ref ADCx_Sampling_Mode - specifies the ADC1 sampling.
  * @param  SwitchingMode - @ref ADCx_Channel_Switching - specifies the ADC1 channel switching.
  * @retval None
  */
void ADC1_OperationModeConfig(ADCx_Sampling_Mode SamplingMode, ADCx_Channel_Switching SwitchingMode)
{
    uint32_t tmpreg_CFG;

    /* Check the parameters */
    assert_param(IS_ADC_SAMPLING_MODE_CONFIG(SamplingMode));
    assert_param(IS_ADC_CH_SWITCHING_CONFIG(SwitchingMode));

    tmpreg_CFG = MDR_ADC->ADC1_CFG;
    tmpreg_CFG &= ~(ADC1_CFG_REG_SAMPLE | ADC1_CFG_REG_CHCH);
    tmpreg_CFG += (uint32_t)SamplingMode + (uint32_t)SwitchingMode;

    MDR_ADC->ADC1_CFG = tmpreg_CFG;
}

/**
  * @brief  Sets the ADC1 sampling mode.
  * @param  SamplingMode - @ref ADCx_Sampling_Mode - specifies the ADC1 sampling.
  * @retval None
  */
void ADC1_SamplingModeConfig(ADCx_Sampling_Mode SamplingMode)
{
    uint32_t tmpreg_CFG;

    /* Check the parameters */
    assert_param(IS_ADC_SAMPLING_MODE_CONFIG(SamplingMode));

    tmpreg_CFG = MDR_ADC->ADC1_CFG;
    tmpreg_CFG &= ~ADC1_CFG_REG_SAMPLE;
    tmpreg_CFG += (uint32_t)SamplingMode;

    MDR_ADC->ADC1_CFG = tmpreg_CFG;
}

/**
  * @brief  Sets the ADC1 channel switching mode.
  * @param  SwitchingMode - @ref ADCx_Channel_Switching - specifies the ADC1 channel switching.
  * @retval None
  */
void ADC1_ChannelSwithingConfig(ADCx_Channel_Switching SwitchingMode)
{
    uint32_t tmpreg_CFG;

    /* Check the parameters */
    assert_param(IS_ADC_CH_SWITCHING_CONFIG(SwitchingMode));

    tmpreg_CFG = MDR_ADC->ADC1_CFG;
    tmpreg_CFG &= ~ADC1_CFG_REG_CHCH;
    tmpreg_CFG += (uint32_t)SwitchingMode;

    MDR_ADC->ADC1_CFG = tmpreg_CFG;
}

/**
  * @brief  Configures the ADC1 threshould levels.
  * @param  LowLevel: specifies the ADC1 low level value.
  *         This parameter can be a number between 0x0000 and 0x0FFF.
  * @param  HighLevel: specifies the ADC1 high level value.
  *         This parameter can be a number between 0x0000 and 0x0FFF.
  * @param  NewState - @ref ADCx_Level_Control - enables or disables levels control.
  * @retval None
  */
void ADC1_LevelsConfig(uint32_t LowLevel, uint32_t HighLevel, ADCx_Level_Control NewState)
{
    uint32_t tmpreg_CFG;

    /* Check the parameters */
    assert_param(IS_ADC_LEVEL_CONTROL_CONFIG(NewState));
    assert_param(IS_ADC_VALUE(LowLevel));
    assert_param(IS_ADC_VALUE(HighLevel));

    tmpreg_CFG = MDR_ADC->ADC1_CFG;
    tmpreg_CFG &= ~ADC1_CFG_REG_RNGC;
    tmpreg_CFG += (uint32_t)NewState;

    MDR_ADC->ADC1_CFG = tmpreg_CFG;

    MDR_ADC->ADC1_L_LEVEL = LowLevel;
    MDR_ADC->ADC1_H_LEVEL = HighLevel;
}

/**
  * @brief  Sets the ADC1 low level.
  * @param  LowLevel: specifies the ADC1 low level value.
  *         This parameter can be a number between 0x0000 and 0x0FFF.
  * @retval None
  */
void ADC1_SetLowLevel(uint32_t LowLevel)
{
    /* Check the parameters */
    assert_param(IS_ADC_VALUE(LowLevel));

    MDR_ADC->ADC1_L_LEVEL = LowLevel;
}

/**
  * @brief  Sets the ADC1 high level.
  * @param  HighLevel: specifies the ADC1 high level value.
  *         This parameter can be a number between 0x0000 and 0x0FFF.
  * @retval None
  */
void ADC1_SetHighLevel(uint32_t HighLevel)
{
    /* Check the parameters */
    assert_param(IS_ADC_VALUE(HighLevel));

    MDR_ADC->ADC1_H_LEVEL = HighLevel;
}

/**
  * @brief  Starts the ADC1 conversion.
  * @param  None.
  * @retval None.
  */
void ADC1_Start(void)
{
    MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_GO;
}

/**
  * @brief  Returns the ADC1 result.
  * @param  None.
  * @retval ADC1 Result Register value.
  */
uint32_t ADC1_GetResult(void)
{
    return MDR_ADC->ADC1_RESULT;
}

/**
  * @brief  Returns the ADC1 Status Registers combined value.
  * @param  None.
  * @retval The ADC1_STATUS Registers combined value.
  */
uint32_t ADC1_GetStatus(void)
{
    return MDR_ADC->ADC1_STATUS;
}

/**
  * @brief  Checks whether the specified ADC1 Status flag is set or not.
  * @param  Flag: specifies the flag to check.
  *         This parameter can be one of @ref ADC1_Flags values.
  * @retval @ref FlagStatus - Current Status flag state (SET or RESET).
  */
FlagStatus ADC1_GetFlagStatus(ADC1_Flags Flag)
{
    FlagStatus bitstatus;

    /* Check the parameters */
    assert_param(IS_ADC_STATUS_FLAG(Flag));

    if ((MDR_ADC->ADC1_STATUS & Flag) == 0)
    {
        bitstatus = RESET;
    }
    else
    {
        bitstatus = SET;
    }

    return bitstatus;
}

/**
  * @brief  Clears the ADC1 Overwrite flag.
  * @param  None.
  * @retval None
  */
void ADC1_ClearOverwriteFlag(void)
{
    MDR_ADC->ADC1_STATUS &= ~ADC1_FLAG_OVERWRITE;
}

/**
  * @brief  Clears the ADC1 AWOIFEN flag.
  * @param  None.
  * @retval None
  */
void ADC1_ClearOutOfRangeFlag(void)
{
    MDR_ADC->ADC1_STATUS &= ~ADC1_FLAG_OUT_OF_RANGE;
}

/**
  * @brief  Enables or disables the ADC1 interrupts.
  * @param  ADC_IT: specifies the ADC1 interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of @ref ADC1_IT_Def values.
  * @param  NewState - @ref FunctionalState - new state of the ADC1 interrupts.
  * @retval None
  */
void ADC1_ITConfig(uint32_t ADC_IT, FunctionalState NewState)
{
    uint32_t tmpreg_ADC1_IE;

    /* Check the parameters */
    assert_param(IS_ADC_CONFIG_IT(ADC_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    tmpreg_ADC1_IE = MDR_ADC->ADC1_STATUS;

    /* Form new value */
    if (NewState != DISABLE)
    {
        /* Enable the ADC Interrupt requests by setting bits in the ADC1_STATUS register */
        tmpreg_ADC1_IE |= ((ADC_IT << 2) & 0xFFFF);
    }
    else
    {
        /* Disable the ADC Interrupt requests by clearing bits in the ADC1_STATUS register */
        tmpreg_ADC1_IE &= ~((ADC_IT << 2) & 0xFFFF);
    }

    /* Configure ADC1_STATUS registers with new value */
    MDR_ADC->ADC1_STATUS = tmpreg_ADC1_IE;
}

/**
  * @brief  Checks whether the ADC1 interrupt has occurred or not.
  * @param  ADC_IT: specifies the ADC interrupt source to check.
  *         This parameter can be one of @ref ADC1_IT_Def values.
  * @retval @ref ITStatus - The state of the ADC_IT (SET or RESET).
  */
ITStatus ADC1_GetITStatus(uint32_t ADC_IT)
{
    ITStatus bitstatus;
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_ADC_CONFIG_IT(ADC_IT));

    tmpreg = MDR_ADC->ADC1_STATUS;
    tmpreg &= (tmpreg >> 2) & ADC_IT;

    if (tmpreg == 0)
    {
        bitstatus = RESET;
    }
    else
    {
        bitstatus = SET;
    }

    return bitstatus;
}

/** @} */ /* End of group ADC_Private_Functions */

/** @} */ /* End of group ADC */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_adc.c */
