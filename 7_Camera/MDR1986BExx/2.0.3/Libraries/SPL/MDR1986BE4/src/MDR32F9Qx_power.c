/**
  ******************************************************************************
  * @file    MDR32F9Qx_power.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   This file contains all the POWER firmware functions.
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
#include "MDR32F9Qx_power.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @defgroup POWER POWER
  * @{
  */

/** @defgroup POWER_Private_Defines POWER Private Defines
  * @{
  */

/* BKP_REG0E register bit mask */
#define DUcc_Mask       ((uint32_t)(BKP_REG_0E_LOW_Msk | BKP_REG_0E_SELECTRI_Msk))
#define DUccTrim_Mask   ((uint32_t)BKP_REG_0E_TRIM_Msk)

/** @} */ /* End of group POWER_Private_Defines */


/** @defgroup POWER_Private_Functions POWER Private Functions
  * @{
  */

/**
  * @brief  Deinitializes the POWER peripheral registers to their default reset values.
  * @param  None.
  * @retval None.
  */
void POWER_DeInit(void)
{
    MDR_POWER -> PVDCS = (uint32_t)0x00000000;
}

#if defined (MDR_LEGACY_SUPPORT) && (MDR_LEGACY_SUPPORT) == 153
/**
  * @brief  Select the internal voltage regulator mode
  * @param  DUccMode - @ref PWR_DUcc_Mode - specifies the DUcc mode.
  * @retval None
  */
void POWER_DUccMode(PWR_DUcc_Mode DUccMode)
{
    #warning "Legacy v1.5.3: POWER_DUccMode() moved to MDR32F9Qx_bkp.c as BKP_DUccMode() and will be removed from MDR32F9Qx_power.c"
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_POWER_DUCC_MODE(DUccMode));

    /* Clear POWER_REG0E[5:0] bits */
    tmpreg = MDR_BKP->REG_0E & (uint32_t) (~DUcc_Mask);
    /* Set POWER_REG0E[5:0] bits according to DUcc mode */
    tmpreg |= DUcc_Mask & (uint32_t)DUccMode;

    MDR_BKP->REG_0E = tmpreg;
}

/**
  * @brief  Set the internal voltage regulator trim
  * @param  DUccTrim - @ref PWR_DUcc_Trim - specifies the DUcc trim.
  * @retval None
  */
void POWER_DUccTrim(PWR_DUcc_Trim DUccTrim)
{
    #warning "Legacy v1.5.3: POWER_DUccTrim() moved to MDR32F9Qx_bkp.c as BKP_DUccTrim() and will be removed from MDR32F9Qx_power.c"
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_POWER_DUCC_TRIM(DUccTrim));

    /* Clear POWER_REG0E[5:0] bits */
    tmpreg  = MDR_BKP->REG_0E & (uint32_t) (~BKP_REG_0E_TRIM20_Msk);
    /* Set POWER_REG0E[5:0] bits according to DUcc mode */
    tmpreg |= (BKP_REG_0E_TRIM20_Msk) & (uint32_t)DUccTrim;

    MDR_BKP->REG_0E = tmpreg;
}

/**
  * @brief  Enter standby mode.
  * @param  None
  * @retval None
  */
void POWER_DUccStandby(void)
{
    #warning "Legacy v1.5.3: POWER_DUccStandby() moved to MDR32F9Qx_bkp.c as BKP_EnterSTANDBYMode() and will be removed from MDR32F9Qx_power.c"
    MDR_BKP->REG_0F |= BKP_REG_0F_STANDBY;
}

/**
  * @brief  Enters STANDBY mode.
  * @param  None
  * @retval None
  */
void POWER_EnterSTANDBYMode(void)
{
    #warning "Legacy v1.5.3: POWER_EnterSTANDBYMode() will be removed from MDR32F9Qx_power.c"
    /* Select STANDBY mode */
    /* Set SLEEPDEEP bit of System Control Register */
    *(__IO uint32_t *) SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    MDR_BKP->REG_0F |= BKP_REG_0F_STANDBY;
    /* Request Wait For Interrupt */
    __WFI();
}

/**
  * @brief  Enters STOP mode.
  * @param  POWER_Regulator_state - @ref FunctionalState - specifies the regulator state in STOP mode.
  *         This parameter can be: ENABLE or DISABLE.
  *             @arg ENABLE: STOP mode with regulator ON
  *             @arg DISABLE: STOP mode with regulator in low power mode
  * @param  POWER_STOPentry - @ref PWR_Stop_Entry_Mode - specifies if STOP mode in entered with WFI or WFE instruction.
  * @retval None
  */
void POWER_EnterSTOPMode(FunctionalState POWER_Regulator_state, PWR_Stop_Entry_Mode POWER_STOPentry)
{
    #warning "Legacy v1.5.3: POWER_EnterSTOPMode() moved to MDR32F9Qx_bkp.c as BKP_EnterSTOPMode() and will be removed from MDR32F9Qx_power.c"

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(POWER_Regulator_state));
    assert_param(IS_POWER_STOP_ENTRY(POWER_STOPentry));

    if(POWER_Regulator_state == DISABLE)
    {
        MDR_BKP->REG_0F |= BKP_REG_0F_STANDBY;
    }
    else
    {
        /* Set SLEEPDEEP bit */
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
        /* Select STOP mode entry */
        if(POWER_STOPentry == POWER_STOPentry_WFI)
        {
            /* Request Wait For Interrupt */
            __WFI();
        }
        else
        {
            /* Request Wait For Event */
            __WFE();
        }
    }
}

/**
  * @brief  Set power on reset flag (FPOR).
  * @param  None
  * @retval None
  */
void POWER_SetFlagPOR(void)
{
    #warning "Legacy v1.5.3: POWER_SetFlagPOR() moved to MDR32F9Qx_bkp.c as BKP_SetFlagPOR() and will be removed from MDR32F9Qx_power.c"
    MDR_BKP->REG_0E |= BKP_REG_0E_FPOR;

}

/**
  * @brief  Power on reset flag (FPOR) status.
  * @param  None
  * @retval @ref ErrorStatus - SUCCESS if FPOR is zero, else ERROR
  */
ErrorStatus POWER_FlagPORstatus(void)
{
    #warning "Legacy v1.5.3: POWER_FlagPORstatus() moved to MDR32F9Qx_bkp.c as BKP_FlagPORstatus() and will be removed from MDR32F9Qx_power.c"
    ErrorStatus state = ERROR;

    if ( (MDR_BKP->REG_0E & BKP_REG_0E_FPOR) == BKP_REG_0E_FPOR)
    {
        state = SUCCESS;
    }

    return state;
}
#endif /* MDR_LEGACY_SUPPORT 153 */

/**
  * @brief  Enables or disables the Power Voltage Detectors (PVD, PVBD).
  * @param  NewState - @ref FunctionalState - new state of the PVDs.
  * @retval None
  */
void POWER_PVDenable(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState != DISABLE)
    {
        MDR_POWER->PVDCS |= POWER_PVDCS_PVDEN;
    }
    else
    {
        MDR_POWER->PVDCS &= ~POWER_PVDCS_PVDEN;
    }
}

/**
  * @brief  Configures the voltage threshold detected by the Power Voltage Detector(PVD).
  * @param  POWER_PVDlevel - @ref PWR_Voltage_Detector_Lvl - specifies the PVD detection level.
  * @retval None
  */
void POWER_PVDlevelConfig(PWR_Voltage_Detector_Lvl POWER_PVDlevel)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_POWER_PVD_LEVEL(POWER_PVDlevel));

    tmpreg = MDR_POWER->PVDCS;

    /* Clear PLS[5:3] bits */
    tmpreg &= (uint32_t) ~POWER_PVDCS_PLS_Msk;

    /* Set PLS[5:3] bits according to POWER_PVDlevel value */
    tmpreg |= (POWER_PVDlevel & POWER_PVDCS_PLS_Msk);

    /* Store the new value */
    MDR_POWER->PVDCS = tmpreg;
}

/**
  * @brief  Configures the voltage threshold detected by the Power Battery Voltage Detector(PVBD).
  * @param  POWER_PVBDlevel - @ref PWR_Batt_Voltage_Detector_Lvl - specifies the PVBD detection level.
  * @retval None
  */
void POWER_PVBDlevelConfig(PWR_Batt_Voltage_Detector_Lvl POWER_PVBDlevel)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_POWER_PVBD_LEVEL(POWER_PVBDlevel));

    tmpreg = MDR_POWER->PVDCS;

    /* Clear PBLS[2:1] bits */
    tmpreg &= (uint32_t) ~POWER_PVDCS_PBLS_Msk;

    /* Set PBLS[2:1] bits according to POWER_PVBDlevel value */
    tmpreg |= (POWER_PVBDlevel & POWER_PVDCS_PBLS_Msk);

    /* Store the new value */
    MDR_POWER->PVDCS = tmpreg;
}

/**
  * @brief   Checks whether the specified
  *         POWER detection flag is set or not.
  * @param  POWER_FLAG - @ref PWR_Voltage_Detector_Flags - specifies the flag to check.
  * @retval The state of POWER_FLAG (SET or RESET).
  */
FlagStatus POWER_GetFlagStatus(PWR_Voltage_Detector_Flags POWER_FLAG)
{
    FlagStatus status;

    /* Check the parameters */
    assert_param(IS_POWER_FLAG(POWER_FLAG));

    if ((MDR_POWER->PVDCS & (uint32_t)POWER_FLAG) != (uint32_t)RESET)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }

    /* Return the flag status */
    return status;
}

/**
  * @brief  Clears the POWER's pending flags.
  * @param  POWER_FLAG: specifies the flag to clear.
  *         This parameter can be any combination of the @ref PWR_Voltage_Detector_Flags values.
  * @retval None
  */
void POWER_ClearFlag(uint32_t POWER_FLAG)
{
    /* Check the parameters */
    assert_param(IS_POWER_FLAGS(POWER_FLAG));

    MDR_POWER->PVDCS &= (uint32_t) ~POWER_FLAG;
}

/**
  * @brief  Enables or disables the specified POWER interrupts.
  * @param  POWER_IT: specifies the PVD interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the @ref PWR_Voltage_Detector_ITEnable values.
  * @param  NewState - @ref FunctionalState - new state of the specified POWER interrupts.
  * @retval None
  */
void POWER_PVD_ITconfig(uint32_t POWER_IT, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_POWER_PVD_IT(POWER_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        MDR_POWER->PVDCS |= POWER_IT;
    }
    else
    {
        MDR_POWER->PVDCS &= (uint32_t)~POWER_IT;
    }
}

/**
  * @brief  Enables or disables inversion of the specified POWER voltage detection flags
  * @param  POWER_INV: specifies the PVD interrupts sources to be enabled or disabled.
  *         This parameter can be any combination of the @ref PWR_Voltage_Detector_IT_Inversion values.
  * @param  NewState - @ref FunctionalState - new state of the specified POWER voltage flag inversion.
  * @retval None
  */
void POWER_PVD_INVconfig(uint32_t POWER_INV, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_POWER_PVD_INV(POWER_INV));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        MDR_POWER->PVDCS |= POWER_INV;
    }
    else
    {
        MDR_POWER->PVDCS &= (uint32_t)~POWER_INV;
    }
}

/** @} */ /* End of group POWER_Private_Functions */

/** @} */ /* End of group POWER */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_power.c */
