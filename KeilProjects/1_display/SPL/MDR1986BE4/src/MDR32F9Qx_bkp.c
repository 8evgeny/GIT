/**
  ******************************************************************************
  * @file    MDR32F9Qx_bkp.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   This file contains all the BKP firmware functions.
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
#include "MDR32F9Qx_bkp.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @defgroup BKP BKP
  * @{
  */

/** @defgroup BKP_Private_Defines BKP Private Defines
  * @{
  */

/* --------------------- BKP registers bit mask ------------------------ */

/* BKP_REG0E register bit mask */
#define DUcc_Mask         ((uint32_t)(BKP_REG_0E_LOW_Msk | BKP_REG_0E_SELECTRI_Msk))
#define DUccTrim_Mask     ((uint32_t)BKP_REG_0E_TRIM20_Msk)
#define BKP_REG_0E_ON     ((uint32_t)(0x01U << 11))
#define BKP_REG_0E_OFF    ((uint32_t)(~((0x01U << 31) | (0x07U << 8) | (0x3FU))))

/* CFR register bit mask */
#define WDGTB_Mask        ((uint32_t)0xFFFFFE7F)
#define WIN_Mask          ((uint32_t)0x7F)
#define CTR_Mask          ((uint32_t)0x7F)

/** @} */ /* End of group BKP_Private_Defines */


/** @defgroup BKP_Private_Functions BKP Private Functions
  * @{
  */

/**
  * @brief  Deinitializes the BKP peripheral registers to their default reset values.
  * @param  None
  * @retval None
  */
void BKP_DeInit(void)
{
    #define BKP_RTC_CS_NUM    0x14
    #define BKP_REG_0F_NUM    0x0F
    #define BKP_REG_0E_NUM    0x0E
    #define BKP_RTC_CNT_NUM   0x10

    uint32_t *_bkp = (uint32_t*) MDR_BKP_BASE;
    uint32_t i;

    for (i = 0; i < BKP_REG_0E_NUM; i++)
    {
        _bkp[i] = 0;
    }

    for (i = BKP_RTC_CNT_NUM; i <= BKP_RTC_CS_NUM; i++)
    {
        _bkp[i] = 0;
    }

    MDR_BKP->REG_0E |= (uint32_t) (BKP_REG_0E_ON);
    MDR_BKP->REG_0E &= (uint32_t) (BKP_REG_0E_OFF);
    MDR_BKP->REG_0F = (uint32_t) (BKP_REG_0F_LSI_ON);
}

/**
  * @brief  Enables or disables the RTC.
  * @param  NewState - @ref FunctionalState - new state of the SWD (ENABLE or DISABLE).
  * @retval None
  */
void BKP_SWD_CMD(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState != DISABLE)
    {
        MDR_BKP->REG_0E |= BKP_REG_0F_RTC_EN;
    }
    else
    {
        MDR_BKP->REG_0F &= ~BKP_REG_0F_RTC_EN;
    }
}

/**
  * @brief  Configures the RTC clock source.
  * @param  RTC_CLK - @ref BKP_RTC_CLK_TypeDef - specifies the RTC clock source.
  * @retval None
  */
void BKP_RTCclkSource(BKP_RTC_CLK_TypeDef RTC_CLK)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_RTC_CLK_SOURCE(RTC_CLK));

    /* Clear BKP_REG0F[3:2] bits */
    tmpreg = MDR_BKP ->REG_0F & (uint32_t) (~BKP_REG_0F_RTC_SEL_Msk );
    /* Set BKP_REG0F[3:2] bits according to RTC clock source*/
    tmpreg |= BKP_REG_0F_RTC_SEL_Msk & (uint32_t)RTC_CLK;

    MDR_BKP ->REG_0F = tmpreg;
}


/**
  * @brief  Enables or disables the RTC.
  * @param  NewState - @ref FunctionalState - new state of the RTC.
  * @retval None
  */
void BKP_RTC_WorkPermit(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState != DISABLE)
    {
        MDR_BKP->REG_0F |= BKP_REG_0F_RTC_EN;
    }
    else
    {
        MDR_BKP->REG_0F &= ~BKP_REG_0F_RTC_EN;
    }
}

/**
  * @brief  Configures the RTC clock source.
  * @param  RTC_Calibration: specifies the tact drop count.
  *         @arg 0 <= RTC_Calibration <= 255.
  * @retval None
  */
void BKP_RTC_Calibration(uint32_t RTC_Calibration)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_RTC_CALIBRATION(RTC_Calibration));

    /* Clear BKP_REG0F[12:5] bits */
    tmpreg  = MDR_BKP->REG_0F & (uint32_t) (~BKP_REG_0F_CAL_Msk);
    /* Set BKP_REG0F[12:5] bits according to RTC clock source*/
    tmpreg |= BKP_REG_0F_CAL_Msk & (RTC_Calibration << BKP_REG_0F_CAL_Pos);

    MDR_BKP -> REG_0F = tmpreg;
}


/**
  * @brief  Reset the RTC.
  * @param  NewState - @ref FunctionalState - new state of the RTC (ENABLE or DISABLE).
  * @retval None
  */
void BKP_RTC_Reset(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState != DISABLE)
    {
        MDR_BKP->REG_0F |= BKP_REG_0F_RTC_RESET;
    }
    else
    {
        MDR_BKP->REG_0F &= ~BKP_REG_0F_RTC_RESET;
    }
}

/**
  * @brief  Enables or disables the specified RTC interrupts.
  * @param  BKP_RTC_IT_Source - @ref BKP_RTC_IT - specifies the RTC interrupts sources to be enabled or disabled.
  * @param  NewState - @ref FunctionalState - new state of the specified RTC interrupts (ENABLE or DISABLE).
  * @retval None
  */
void BKP_RTC_ITConfig(BKP_RTC_IT BKP_RTC_IT_Source, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_RTC_CLK_IT(BKP_RTC_IT_Source));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        MDR_BKP->RTC_CS |= (uint32_t)BKP_RTC_IT_Source;
    }
    else
    {
        MDR_BKP->RTC_CS &= ~((uint32_t)BKP_RTC_IT_Source);
    }
}

/**
  * @brief  Gets the RTC counter value.
  * @param  None
  * @retval RTC counter value.
  */
uint32_t BKP_RTC_GetCounter(void)
{
    return MDR_BKP->RTC_CNT;
}

/**
  * @brief  Sets the RTC counter value.
  * @param  CounterValue: RTC counter new value.
  * @retval None
  */
void BKP_RTC_SetCounter(uint32_t CounterValue)
{
    MDR_BKP->RTC_CNT = CounterValue;
}

/**
  * @brief  Sets the RTC alarm value.
  * @param  AlarmValue: RTC alarm new value.
  * @retval None
  */
void BKP_RTC_SetAlarm(uint32_t AlarmValue)
{
    MDR_BKP->RTC_ALRM = AlarmValue;
}

/**
  * @brief  Gets the RTC divider value.
  * @param  None
  * @retval RTC Divider value.
  */
uint32_t BKP_RTC_GetDivider(void)
{
    return MDR_BKP->RTC_DIV;
}

/**
  * @brief  Sets the RTC prescaler (RTC_DIV) preloaded value.
  * @param  PrescalerValue: RTC prescaler new value.
  *         This parameter should be less or equal then 0x000FFFFF
  * @retval None
  */
void BKP_RTC_SetPrescaler(uint32_t PrescalerValue)
{
  /* Check the parameter */
  assert_param(IS_BKP_PRESCALER_VALUE(PrescalerValue));

  MDR_BKP->RTC_PRL = PrescalerValue;
}

/**
  * @brief  Waits until last write operation on RTC registers has finished.
  * @note   This function must be called before any write to RTC registers.
  * @param  None
  * @retval None
  */
void BKP_RTC_WaitForUpdate(void)
{
    /* Loop until WEC flag is set */
    while((MDR_BKP->RTC_CS & BKP_RTC_CS_WEC) == BKP_RTC_CS_WEC);
}

/**
  * @brief  Checks whether the specified RTC flag is set or not.
  * @param  BKP_RTC_Flag_Value - @ref BKP_RTC_FLAGS - specifies the flag to check.
  * @retval @ref FlagStatus - Current state of BKP_RTC_Flag_Value (SET or RESET).
  */
FlagStatus BKP_RTC_GetFlagStatus(BKP_RTC_FLAGS BKP_RTC_Flag_Value)
{
    FlagStatus status;

    /* Check the parameters */
    assert_param(IS_BKP_RTC_GET_FLAG(BKP_RTC_Flag_Value));

    if ((MDR_BKP -> RTC_CS & (uint32_t)BKP_RTC_Flag_Value) != 0)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }

    return status;
}

/**
  * @brief  Clears the specified RTC flag(s).
  * @param  BKP_RTC_FLAG: specifies the flag to clear.
  *         This parameter can be any combination of @ref BKP_RTC_FLAGS values.
  * @retval None.
  */
void BKP_RTC_ClearFlagStatus(uint32_t BKP_RTC_FLAG)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_BKP_RTC_CLAER_FLAG(BKP_RTC_FLAG));

    tmpreg = MDR_BKP->RTC_CS & (~BKP_RTC_CS_Msk);
    tmpreg |= BKP_RTC_FLAG;

    MDR_BKP->RTC_CS = tmpreg;
}

/**
  * @brief  Select the internal voltage regulator mode
  * @param  DUccMode - @ref BKP_DUcc_Mode - specifies the DUcc mode.
  * @retval None.
  */
void BKP_DUccMode(BKP_DUcc_Mode DUccMode)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_BKP_DUCC_MODE(DUccMode));

    /* Clear BKP_REG0E[5:0] bits */
    tmpreg  = MDR_BKP -> REG_0E & (uint32_t) (~DUcc_Mask);
    /* Set BKP_REG0E[5:0] bits according to DUcc mode */
    tmpreg |= DUcc_Mask & (uint32_t)DUccMode;

    MDR_BKP -> REG_0E = tmpreg;
}

/**
  * @brief  Adjustment coefficient of the reference voltage
  *         integrated voltage regulator DUcc roughly.
  * @param  trim - @ref BKP_TRIM - coefficient of the reference voltage.
  * @return None.
  */
void BKP_SetTrim(BKP_TRIM trim)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_BKP_TRIM(trim));

    tmpreg = MDR_BKP->REG_0E & (~(3 << BKP_REG_0E_TRIM43_Pos));
    tmpreg = tmpreg | (uint32_t)trim;
    MDR_BKP->REG_0E = tmpreg;
}

/**
  * @brief  Set the internal voltage regulator trim
  * @param  DUccTrim - @ref BKP_DUcc_Trim - specifies the DUcc trim.
  * @retval None
  */
void BKP_DUccTrim(BKP_DUcc_Trim DUccTrim)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_BKP_DUCC_TRIM(DUccTrim));

    /* Clear BKP_REG0E[5:0] bits */
    tmpreg  = MDR_BKP->REG_0E & (~(uint32_t)DUccTrim_Mask);
    /* Set BKP_REG0E[5:0] bits according to DUcc mode */
    tmpreg |= DUccTrim_Mask & (uint32_t)DUccTrim;

    MDR_BKP -> REG_0E = tmpreg;
}

/**
  * @brief  Set power on reset flag (FPOR).
  * @param  None
  * @retval None
  */
void BKP_SetFlagPOR(void)
{
    MDR_BKP->REG_0E |= BKP_REG_0E_FPOR;
}

/**
  * @brief  Power on reset flag (FPOR) status
  * @param  None
  * @retval @ref ErrorStatus - SUCCESS if FPOR is zero, else ERROR
  */
ErrorStatus BKP_FlagPORstatus(void)
{
    ErrorStatus state = ERROR;

    if ( (MDR_BKP->REG_0E & BKP_REG_0E_FPOR) == BKP_REG_0E_FPOR)
    {
        state = SUCCESS;
    }

    return state;
}

/**
  * @brief  Enters STOP mode.
  * @param  BKP_Regulator_state - @ref FunctionalState - specifies the regulator state in STOP mode.
  *             @arg ENABLE:  STOP mode with regulator ON
  *             @arg DISABLE: STOP mode with regulator in low power mode
  * @param  BKP_STOPentry - @ref BKP_Stop_Entry_Mode - specifies if
  *         STOP mode in entered with WFI or WFE instruction.
  * @retval None
  */
void BKP_EnterSTOPMode(FunctionalState BKP_Regulator_state, BKP_Stop_Entry_Mode BKP_STOPentry)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(BKP_Regulator_state));
    assert_param(IS_BKP_STOP_ENTRY(BKP_STOPentry));

    if(BKP_Regulator_state == DISABLE)
    {
        MDR_BKP->REG_0F |= BKP_REG_0F_STANDBY;
    }
    else
    {
        /* Set SLEEPDEEP bit of System Control Register */
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

        /* Select STOP mode entry */
        if(BKP_STOPentry == BKP_STOPentry_WFE)
        {
            /* Request Wait For Event */
            __WFE();
        }
        else
        {
            /* Request Wait For Interrupt */
            __WFI();
        }
    }
}

/**
  * @brief  Controls current limit protection 150mA
  * @param  NewState - @ref FunctionalState - new state of ilimen bit (ENABLE or DISABLE).
  * @return None.
  */
void BKP_CurrentLimitProtection150mA(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState == DISABLE)
    {
        MDR_BKP->REG_0E &= ~(1 << BKP_REG_0E_ILIMEN_Pos);
    }
    else
    {
        MDR_BKP->REG_0E |= (1 << BKP_REG_0E_ILIMEN_Pos);
    }
}

/**
  * @brief  Enters STANDBY mode.
  * @param  None
  * @retval None
  */
void BKP_EnterSTANDBYMode(void)
{
    /* Enter STANDBY mode */
    MDR_BKP->REG_0F |= BKP_REG_0F_STANDBY;
}

/**
  * @brief  Sign status of unauthorized opening of the device (COVDET state).
  * @param  None
  * @retval @ref FunctionalState - DISABLE if COVDET is zero, else ENABLE
  */
FunctionalState BKP_COVDET_Status(void)
{
    FunctionalState state = DISABLE;

    if (MDR_BKP->REG_0E & BKP_REG_0E_COVDET)
    {
        state = (FunctionalState)ENABLE;
    }

    return state;
}

/** @} */ /* End of group BKP_Private_Functions */

/** @} */ /* End of group BKP */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_bkp.c */
