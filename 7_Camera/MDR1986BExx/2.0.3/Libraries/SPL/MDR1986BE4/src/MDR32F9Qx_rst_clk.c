/**
  ******************************************************************************
  * @file    MDR32F9Qx_rst_clk.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   This file contains all the RST_CLK firmware functions.
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
#include "MDR32F9Qx_rst_clk.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @defgroup RST_CLK RST_CLK
  * @{
  */

/** @defgroup RST_CLK_Private_Constants RST_CLK Private Constants
  * @{
  */
#define FLAG_MASK                   ((uint8_t)0x1F)
#define FLAG_SFR_MASK               ((uint8_t)0xE0)

#if defined (MDR_LEGACY_SUPPORT) && (MDR_LEGACY_SUPPORT) == 153
    #warning "Legacy v1.5.3: Clock frequencies and timeouts will be removed from MDR32F9Qx_rst_clk.c. Use definitions in MDR32F9Qx_config.h"

    #ifndef HSI_Value
        /* Typical Value of the HSI in Hz */
        #define HSI_Value                  ((uint32_t)8000000)
    #endif /* #ifndef HSI_Value */

    #ifndef HSE_Value
        /* Typical Value of the HSE in Hz */
        #define HSE_Value                  ((uint32_t)8000000)
    #endif /* #ifndef HSE_Value */

    #ifndef LSI_Value
        /* Typical Value of the LSI in Hz */
        #define LSI_Value                  ((uint32_t)40000)
    #endif /* #ifndef LSI_Value */

    #ifndef LSE_Value
        /* Typical Value of the LSE in Hz */
        #define LSE_Value                  ((uint32_t)32768)
    #endif /* #ifndef LSE_Value */

    #ifndef HSEonTimeOut
        /* Time out for HSE start up */
        #define HSEonTimeOut               ((uint16_t)0x0600)
    #endif /* #ifndef HSEonTimeOut */

    #ifndef LSEonTimeOut
        /* Time out for LSE start up */
        #define LSEonTimeOut               ((uint16_t)0x0600)
    #endif /* #ifndef LSEonTimeOut */

    #ifndef HSIonTimeOut
        /* Time out for HSI start up */
        #define HSIonTimeOut               ((uint16_t)0x0600)
    #endif /* #ifndef HSIonTimeOut */

    #ifndef LSIonTimeOut
        /* Time out for LSI start up */
        #define LSIonTimeOut               ((uint16_t)0x0600)
    #endif /* #ifndef LSIonTimeOut */

    #ifndef PLLCPUonTimeOut
        /* Time out for PLLCPU start up */
        #define PLLCPUonTimeOut            ((uint16_t)0x0600)
    #endif /* #ifndef PLLCPUonTimeOut */

#endif /* MDR_LEGACY_SUPPORT 153 */

/** @} */ /* End of group RST_CLK_Private_Constants */


/** @defgroup RST_CLK_Private_Functions RST_CLK Private Functions
  * @{
  */

/**
  * @brief  Set the RST_CLK clock configuration to the default reset state.
  * @param  None
  * @retval None
  */
void RST_CLK_DeInit(void)
{
    RST_CLK_WarmDeInit();
    /* Reset REG_0F bits to zero but HSION bit */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_BKP, ENABLE);
    MDR_BKP->REG_0F |= (uint32_t)((1 << 22) | (1 << 15)); /* HSION & LSION */
    MDR_BKP->REG_0F &= (uint32_t)((1 << 22) | (1 << 15));
}

/**
  * @brief  Set the RST_CLK clock configuration to the default reset state.
  * @param  None
  * @retval None
  */
void RST_CLK_WarmDeInit(void)
{
    /* Prepare HSI clk */
    RST_CLK_HSIcmd(ENABLE);
    RST_CLK_HSIstatus();
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkHSI);

    /* Reset MDR_RST_CLK bits */
    MDR_RST_CLK->CPU_CLOCK   &= (uint32_t)0x00000000;
    MDR_RST_CLK->PLL_CONTROL &= (uint32_t)0x00000000;
    MDR_RST_CLK->HS_CONTROL  &= (uint32_t)0x00000000;
    MDR_RST_CLK->ADC_CLOCK   &= (uint32_t)0x00000000;
    MDR_RST_CLK->RTC_CLOCK   &= (uint32_t)0x00000000;

    /* Reset all but DMA_EN and DEBUG_EN bits */
    MDR_RST_CLK->PER1_CLOCK  = RST_CLK_PER1_CLOCK_DMA_EN | RST_CLK_PER1_CLOCK_DEBUG_EN;

    /* Reset all periph clock but RST_CLK and BKP_CLK bits */
    MDR_RST_CLK->PER2_CLOCK  = RST_CLK_PER2_CLOCK_PCLK_EN_RST_CLK;

    MDR_RST_CLK->TIM_CLOCK   &= (uint32_t)0x00000000;
    MDR_RST_CLK->UART_CLOCK  &= (uint32_t)0x00000000;
    MDR_RST_CLK->SSP_CLOCK   &= (uint32_t)0x00000000;
}

/**
  * @brief  HSE (High Speed External) clock mode and source selection
  * @param  RST_CLK_HSE - @ref RST_CLK_HSE_Mode - mode selector.
  * @retval None
  */
void RST_CLK_HSEconfig(RST_CLK_HSE_Mode RST_CLK_HSE)
{
    /* Check the parameters */
    assert_param(IS_RST_CLK_HSE(RST_CLK_HSE));

    /* Reset HSEON and HSEBYP bits before configuring the HSE */
    MDR_RST_CLK->HS_CONTROL &= ~((uint32_t)(RST_CLK_HSE_ON | RST_CLK_HSE_Bypass));

    /* Configure HSE (RCC_HSE_OFF is already covered by the code section above) */
    switch (RST_CLK_HSE)
    {
        case RST_CLK_HSE_ON:
            /* Set HSEON bit */
            MDR_RST_CLK->HS_CONTROL |= RST_CLK_HSE_ON;
            break;

        case RST_CLK_HSE_Bypass:
            /* Set HSEBYP and HSEON bits */
            MDR_RST_CLK->HS_CONTROL |= RST_CLK_HSE_ON | RST_CLK_HSE_Bypass;
            break;

        default:
            break;
    }
}

/**
  * @brief  HSE clock status
  * @param  None
  * @retval @ref ErrorStatus - SUCCESS if HSE clock is ready, else ERROR
  */
ErrorStatus RST_CLK_HSEstatus(void)
{
    __IO uint32_t startCounter = 0;
    ErrorStatus state;
    FlagStatus flag;

    /* Wait until HSE is ready or time out is occure */
    do
    {
        flag = RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSERDY);
        startCounter++;
    } while ((startCounter < HSEonTimeOut) && (flag == RESET));

    if (RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSERDY) != RESET)
    {
        state = SUCCESS;
    }
    else
    {
        state = ERROR;
    }

    return state;
}

/**
  * @brief  LSE (Low Speed External) clock mode and source selection
  * @param  RST_CLK_LSE - @ref RST_CLK_LSE_Mode - mode selector.
  * @retval None
  */
void RST_CLK_LSEconfig(RST_CLK_LSE_Mode RST_CLK_LSE)
{
    /* Check the parameters */
    assert_param(IS_RST_CLK_LSE(RST_CLK_LSE));

    /* Reset LSEON and LSEBYP bits before configuring the LSE */
    MDR_BKP->REG_0F &= ~((uint32_t)(RST_CLK_LSE_ON | RST_CLK_LSE_Bypass));

    switch (RST_CLK_LSE)
    {
        case RST_CLK_LSE_ON:
            /* Set LSEON bit */
            MDR_BKP->REG_0F |= RST_CLK_LSE_ON;
            break;

        case RST_CLK_LSE_Bypass:
            /* Set LSEBYP and LSEON bits */
            MDR_BKP->REG_0F |= (RST_CLK_LSE_ON | RST_CLK_LSE_Bypass);
            break;

        default:
            break;
    }
}

/**
  * @brief  LSE clock status
  * @param  None
  * @retval @ref ErrorStatus - SUCCESS if LSE clock is ready, else ERROR
  */
ErrorStatus RST_CLK_LSEstatus(void)
{
    __IO uint32_t startCounter = 0;
    ErrorStatus state;
    FlagStatus flag;

    /* Wait until LSE is ready or time out is occure */
    do
    {
        flag = RST_CLK_GetFlagStatus(RST_CLK_FLAG_LSERDY);
        startCounter++;
    } while ((startCounter < LSEonTimeOut) && (flag == RESET));

    if (RST_CLK_GetFlagStatus(RST_CLK_FLAG_LSERDY) != RESET)
    {
        state = SUCCESS;
    }
    else
    {
        state = ERROR;
    }

    return state;
}

/**
  * @brief  HSI (High Speed Internal) clock mode selection
  * @param  NewState - @ref FunctionalState - mode selector (switch off (DISABLE) or on (ENABLE)).
  * @retval None
  */
void RST_CLK_HSIcmd(FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp = MDR_BKP->REG_0F;
    /* Form new value */
    if (NewState != DISABLE)
    {
        /* Enable HSI clk by setting the HSE_ON bit in the BKP_REG_0F register */
        temp |= BKP_REG_0F_HSI_ON;
    }
    else
    {
        /* Disable HSI clk by resetting the HSE_ON bit in the BKP_REG_0F register */
        temp &= ~BKP_REG_0F_HSI_ON;
    }

    /* Configure REG_0F register with new value */
    MDR_BKP->REG_0F = temp;
}

/**
  * @brief  HSI clock frequency adjustment by HSITRIM setting
  * @param  HSItrimValue: HSI frequency trim value.
  *         This parameter must be a number between 0 and 0x3F.
  * @retval None
  */
void RST_CLK_HSIadjust(uint32_t HSItrimValue)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RCC_CLK_HSI_TRIM_VALUE(HSItrimValue));

    temp = MDR_BKP->REG_0F;

    /* Clear HSITRIM[5:0] bits */
    temp &= ~BKP_REG_0F_HSI_TRIM_Msk;

    /* Set the HSITRIM[5:0] bits according to HSItrimValue value */
    temp |= (uint32_t)HSItrimValue << BKP_REG_0F_HSI_TRIM_Pos;

    /* Store the new value */
    MDR_BKP->REG_0F = temp;
}

/**
  * @brief  HSI clock status
  * @param  None
  * @retval @ref ErrorStatus: SUCCESS if HSI clock is ready, else ERROR
  */
ErrorStatus RST_CLK_HSIstatus(void)
{
    __IO uint32_t startCounter = 0;
    ErrorStatus state;
    FlagStatus flag;

    /* Wait until HSI is ready or time out is occure */
    do
    {
        flag = RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSIRDY);
        startCounter++;
    } while ((startCounter < HSIonTimeOut) && (flag == RESET));

    if (RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSIRDY) != RESET)
    {
        state = SUCCESS;
    }
    else
    {
        state = ERROR;
    }

    return state;
}

/**
  * @brief  LSI (Low Speed Internal) clock mode selection
  * @param  NewState - @ref FunctionalState - mode selector (switch off (DISABLE) or on (ENABLE)).
  * @retval None
  */
void RST_CLK_LSIcmd(FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp = MDR_BKP->REG_0F;
    /* Form new value */
    if (NewState != DISABLE)
    {
        /* Enable LSI clk by setting the LSE_ON bit in the BKP_REG_0F register */
        temp |= BKP_REG_0F_LSI_ON;
    }
    else
    {
        /* Disable LSI clk by resetting the LSE_ON bit in the BKP_REG_0F register */
        temp &= ~BKP_REG_0F_LSI_ON;
    }

    /* Configure REG_0F register with new value */
    MDR_BKP->REG_0F = temp;
}

/**
  * @brief  LSI clock frequency adjustment by LSITRIM setting
  * @param  LSItrimValue: LSI frequency trim value.
  *         This parameter must be a number between 0 and 0x1F.
  * @retval None
  */
void RST_CLK_LSIadjust(uint32_t LSItrimValue)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RCC_CLK_LSI_TRIM_VALUE(LSItrimValue));

    temp = MDR_BKP->REG_0F;

    /* Clear LSITRIM[4:0] bits */
    temp &= ~BKP_REG_0F_LSI_TRIM_Msk;

    /* Set the LSITRIM[4:0] bits according to LSItrimValue value */
    temp |= (uint32_t)LSItrimValue << BKP_REG_0F_LSI_TRIM_Pos;

    /* Store the new value */
    MDR_BKP->REG_0F = temp;
}

/**
  * @brief  LSI clock status
  * @param  None
  * @retval @ref ErrorStatus - SUCCESS if LSI clock is ready, else ERROR
  */
ErrorStatus RST_CLK_LSIstatus(void)
{
    __IO uint32_t startCounter = 0;
    ErrorStatus state;
    FlagStatus flag;

    /* Wait until LSI is ready or time out is occure */
    do
    {
        flag = RST_CLK_GetFlagStatus(RST_CLK_FLAG_LSIRDY);
        startCounter++;
    } while ((startCounter < LSIonTimeOut) && (flag == RESET));

    if (RST_CLK_GetFlagStatus(RST_CLK_FLAG_LSIRDY) != RESET)
    {
        state = SUCCESS;
    }
    else
    {
        state = ERROR;
    }

    return (state);
}

/**
  * @brief  Configures the CPU_PLL clock source and multiplication factor.
  * @param  RST_CLK_CPU_PLLsource - @ref RST_CLK_CPU_PLL_Source - specifies the PLL entry clock source.
  * @param  RST_CLK_CPU_PLLmul - @ref RST_CLK_CPU_PLL_Multiplier - specifies the PLL multiplication factor.
  * @retval None
  */
void RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLL_Source RST_CLK_CPU_PLLsource, uint32_t RST_CLK_CPU_PLLmul)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RST_CLK_CPU_PLL_SOURCE(RST_CLK_CPU_PLLsource));
    assert_param(IS_RST_CLK_CPU_PLL_MUL(RST_CLK_CPU_PLLmul));

    /* Select CPUPLL source */
    temp = MDR_RST_CLK->CPU_CLOCK;
    /* Clear CPU_C1_SEL bits */
    temp &= ~RST_CLK_CPU_CLOCK_CPU_C1_SEL_Msk;
    /* Set the CPU_C1_SEL bits */
    temp |= RST_CLK_CPU_PLLsource;
    /* Store the new value */
    MDR_RST_CLK->CPU_CLOCK = temp;

    /* Set CPUPLL multiplier */
    temp = MDR_RST_CLK->PLL_CONTROL;
    /* Clear PLLMUL[3:0] bits */
    temp &= ~RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Msk;
    /* Set the PLLMUL[3:0] bits */
    temp |= (RST_CLK_CPU_PLLmul << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos);
    /* Store the new value */
    MDR_RST_CLK->PLL_CONTROL = temp;

    if((MDR_RST_CLK->PLL_CONTROL & RST_CLK_PLL_CONTROL_PLL_CPU_ON) == RST_CLK_PLL_CONTROL_PLL_CPU_ON)
    {
        temp = MDR_RST_CLK->PLL_CONTROL;
        temp |= RST_CLK_PLL_CONTROL_PLL_CPU_RLD;
        MDR_RST_CLK->PLL_CONTROL = temp;
        temp &= ~RST_CLK_PLL_CONTROL_PLL_CPU_RLD;
        MDR_RST_CLK->PLL_CONTROL = temp;
    }

}

/**
  * @brief  Select the CPU_PLL output as input for CPU_C3_SEL or bypass the CPU_PLL.
  * @param  UsePLL - @ref FunctionalState - usage state of the CPU_PLL.
  * @retval None
  */
void RST_CLK_CPU_PLLuse(FunctionalState UsePLL)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(UsePLL));

    temp = MDR_RST_CLK->CPU_CLOCK;

    /* Form new value */
    if (UsePLL != DISABLE)
    {
        /* Use the CPU PLL output as input for CPU_C3_SEL
        * by setting the CPU_C2_SEL bit in the CPU_CLOCK register */
        temp |= (1<< RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos);
    }
    else
    {
        /* Use the CPU_C1 output as input for CPU_C3_SEL
        * by resetting the CPU_C2_SEL bit in the CPU_CLOCK register */
        temp &= ~(1<< RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos);
    }

    /* Configure CPU_CLOCK register with new value */
    MDR_RST_CLK->CPU_CLOCK = temp;
}

/**
  * @brief  Enables or disables the CPU_PLL.
  * @param  NewState - @ref FunctionalState - new state of the CPU_PLL.
  * @retval None
  */
void RST_CLK_CPU_PLLcmd(FunctionalState NewState)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp = MDR_RST_CLK->PLL_CONTROL;

    if(NewState != DISABLE)
    {
        temp |= RST_CLK_PLL_CONTROL_PLL_CPU_ON;
    }
    else
    {
        temp &= ~RST_CLK_PLL_CONTROL_PLL_CPU_ON;
    }

    MDR_RST_CLK->PLL_CONTROL = temp;
}

/**
  * @brief  CPU_PLL status
  * @param  None
  * @retval @ref ErrorStatus: SUCCESS if CPU_PLL output clock is ready, else ERROR
  */
ErrorStatus RST_CLK_CPU_PLLstatus(void)
{
    __IO uint32_t startCounter = 0;
    ErrorStatus state;
    FlagStatus flag;

    /* Wait until CPUPLL is ready or time out is occure */
    do
    {
        flag = RST_CLK_GetFlagStatus(RST_CLK_FLAG_PLLCPURDY);
        startCounter++;
    } while ((startCounter < PLLCPUonTimeOut) && (flag == RESET));

    if (RST_CLK_GetFlagStatus(RST_CLK_FLAG_PLLCPURDY) != RESET)
    {
        state = SUCCESS;
    }
    else
    {
        state = ERROR;
    }

    return state;
}

/**
  * @brief  Сonfigures the CPU_C3_SEL division factor switch.
  * @param  CPUclkDivValue - @ref RST_CLK_CPU_C3_Divisor - specifies the CPU_PLL clock division factor.
  * @retval None
  */
void RST_CLK_CPUclkPrescaler(RST_CLK_CPU_C3_Divisor CPUclkDivValue)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RST_CLK_CPUclkDIV(CPUclkDivValue));

    temp = MDR_RST_CLK->CPU_CLOCK;

    /* Clear CPU_C3_SEL bits */
    temp &= ~RST_CLK_CPU_CLOCK_CPU_C3_SEL_Msk;

    /* Set the CPU_C3_SEL bits */
    temp |= ((uint32_t)CPUclkDivValue << RST_CLK_CPU_CLOCK_CPU_C3_SEL_Pos);

    /* Store the new value */
    MDR_RST_CLK->CPU_CLOCK = temp;
}

/**
  * @brief  Select the CPU clock source.
  * @param  CPU_CLK - @ref RST_CLK_HCLK_Source - specifies the CPU clock source.
  * @retval None
  */
void RST_CLK_CPUclkSelection(RST_CLK_HCLK_Source CPU_CLK)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RST_CPU_CLK(CPU_CLK));

    temp = MDR_RST_CLK->CPU_CLOCK;

    /* Clear CPU_C3_SEL bits */
    temp &= ~RST_CLK_CPU_CLOCK_HCLK_SEL_Msk;

    /* Set the CPU_C3_SEL bits */
    temp |= (uint32_t)CPU_CLK;

    /* Store the new value */
    MDR_RST_CLK->CPU_CLOCK = temp;
}

/**
  * @brief  Select the ADC clock source.
  * @param  ADC_CLK - @ref RST_CLK_ADC_Source - specifies the ADC_C2 clock source.
  * @retval None
  */
void RST_CLK_ADCclkSelection(RST_CLK_ADC_Source ADC_CLK)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RST_CLK_ADCclk(ADC_CLK));

    /* Select ADC_CLK source */
    temp = MDR_RST_CLK->ADC_CLOCK;

    /* Clear ADC_C1,C2 SEL bits */
    temp &= ~(RST_CLK_ADC_CLOCK_ADC_C1_SEL_Msk);

    /* Set the ADC_C1, SEL bits */
    temp |= ADC_CLK;

    /* Store the new value */
    MDR_RST_CLK->ADC_CLOCK = temp;
}

/**
  * @brief  Сonfigures the ADC_C3_SEL division factor switch.
  * @param  ADCclkDivValue - @ref RST_CLK_ADC_C3_Divisor - specifies the ADC_C2 clock division factor.
  * @retval None
  */
void RST_CLK_ADCclkPrescaler(RST_CLK_ADC_C3_Divisor ADCclkDivValue)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RST_CLK_ADCclkDivValue(ADCclkDivValue));

    temp = MDR_RST_CLK->ADC_CLOCK;

    /* Clear ADC_C3_SEL bits */
    temp &= ~RST_CLK_ADC_CLOCK_ADC_C3_SEL_Msk;

    /* Set the ADC_C3_SEL bits */
    temp |= ADCclkDivValue << RST_CLK_ADC_CLOCK_ADC_C3_SEL_Pos;

    /* Store the new value */
    MDR_RST_CLK->ADC_CLOCK = temp;
}

/**
  * @brief  Enables or disables the ADC_CLK clock.
  * @param  NewState - @ref FunctionalState - new state of the ADC_CLK.
  * @retval None
  */
void RST_CLK_ADCclkEnable(FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp = MDR_RST_CLK->ADC_CLOCK;

    /* Form new value */
    if (NewState != DISABLE)
    {
        /* Enable the ADC CLK by setting the ADC_CLK_EN bit in the ADC_CLOCK register */
        temp |= RST_CLK_ADC_CLOCK_ADC_CLK_EN;
    }
    else
    {
        /* Disable the ADC CLK by resetting the ADC_CLK_EN bit in the ADC_CLOCK register */
        temp &= ~RST_CLK_ADC_CLOCK_ADC_CLK_EN;
    }

    /* Configure CPU_CLOCK register with new value */
    MDR_RST_CLK->ADC_CLOCK = temp;
}

/**
  * @brief  Set the RST_CLK_ADCIU clock configuration to the default reset state.
  * @param  None.
  * @retval None.
  */
void RST_CLK_ADCIUclkDeInit(void)
{
    MDR_RST_CLK->ADC_CLOCK &= ~(RST_CLK_ADC_CLOCK_ADCIU_C1_SEL_Msk |
                                RST_CLK_ADC_CLOCK_ADCIU_C3_SEL_Msk |
                                RST_CLK_ADC_CLOCK_ADCIU_CLK_EN);

}

/**
  * @brief  Select the ADCIU clock source.
  * @param  ADCIU_CLK_C1 - @ref RST_CLK_ADCIU_Source - specifies the ADCIU_C1 clock source.
  * @retval None
  */
void RST_CLK_ADCIUclkSelection(RST_CLK_ADCIU_Source ADCIU_CLK_C1)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_RST_CLK_ADCIUclk(ADCIU_CLK_C1));

    /* Select ADC_CLK source */
    temp = MDR_RST_CLK->ADC_CLOCK;

    /* Clear ADCIU_C1,C2 SEL bits */
    temp &= ~RST_CLK_ADC_CLOCK_ADCIU_C1_SEL_Msk;

    /* Set the ADCIU_C1 SEL bits */
    temp |= (uint32_t)ADCIU_CLK_C1;

    /* Store the new value */
    MDR_RST_CLK->ADC_CLOCK = temp;
}

/**
 * @brief  Configures the ADCIU_C3_SEL division factor switch.
 * @param  ADCIUclkDIVValue - @ref RST_CLK_ADCIU_C3_Divisor - specifies the ADCIU_VIV clock division factor.
 * @retval None
 */
void RST_CLK_ADCIUclkPrescaler(RST_CLK_ADCIU_C3_Divisor ADCIUclkDIVValue)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_RST_CLK_ADCIUclkDIV(ADCIUclkDIVValue));

    temp = MDR_RST_CLK->ADC_CLOCK;

    /* Clear ADCIU_C3_SEL bits */
    temp &= ~RST_CLK_ADC_CLOCK_ADCIU_C3_SEL_Msk;

    /* Set the ADCIU_C3_SEL bits */
    temp |= (uint32_t)ADCIUclkDIVValue;

    /* Store the new value */
    MDR_RST_CLK->ADC_CLOCK = temp;
}

/**
 * @brief  Enables or disables the ADCIU clk.
 * @param  NewState - @ref FunctionalState - new state of the ADCIU clk.
 * @retval None
 */
void RST_CLK_ADCIUclkCMD(FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp = MDR_RST_CLK->ADC_CLOCK;

    /* Form new value */
    if (NewState != DISABLE)
    {
        /* Enable ADCIU clk by setting the ADCIU_CLK_EN bit in the ADC_CLOCK register */
        temp |= RST_CLK_ADC_CLOCK_ADCIU_CLK_EN;
    }
    else
    {
        /* Disable ADCIU clk by resetting the ADCIU_CLK_EN bit in the ADC_CLOCK register */
        temp &= ~RST_CLK_ADC_CLOCK_ADCIU_CLK_EN;
    }

    /* Configure ADC_CLOCK register with new value */
    MDR_RST_CLK->ADC_CLOCK = temp;
}


/**
  * @brief  Configures the HSI_C1_SEL division factor switch.
  * @param  HSIclkDivValue - @ref RST_CLK_HSI_C1_Divisor - specifies the HSI clock division factor.
  * @retval None
  */
void RST_CLK_HSIclkPrescaler(RST_CLK_HSI_C1_Divisor HSIclkDivValue)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RST_CLK_HSIclkDivValue(HSIclkDivValue));

    temp = MDR_RST_CLK->RTC_CLOCK;

    /* Clear HSI_SEL bits */
    temp &= ~RST_CLK_RTC_CLOCK_HSI_SEL_Msk;

    /* Set the HSI_SEL bits */
    temp |= (uint32_t)HSIclkDivValue << RST_CLK_RTC_CLOCK_HSI_SEL_Pos;

    /* Store the new value */
    MDR_RST_CLK->RTC_CLOCK = temp;
}

/**
  * @brief  Enables or disables the RTCHSI clock.
  * @param  NewState - @ref FunctionalState - new state of the HSI_RTC_EN.
  * @retval None
  */
void RST_CLK_RTC_HSIclkEnable(FunctionalState NewState)
{
    uint32_t temp = 0;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp = MDR_RST_CLK->RTC_CLOCK;

    /* Form new value */
    if (NewState != DISABLE)
    {
        /* Enable HSI RTC clk by setting the HSE_RTC_EN bit in the RTC_CLOCK register */
        temp |= RST_CLK_RTC_CLOCK_HSI_RTC_EN;
    }
    else
    {
        /* Disable HSI RTC clk by resetting the HSE_RTC_EN bit in the RTC_CLOCK register */
        temp &= ~RST_CLK_RTC_CLOCK_HSI_RTC_EN;
    }

    /* Configure RTC_CLOCK register with new value */
    MDR_RST_CLK->RTC_CLOCK = temp;
}

/**
  * @brief  Configures the HSE_C1_SEL division factor switch.
  * @param  HSEclkDivValue - @ref RST_CLK_HSE_C1_Divisor - specifies the HSE clock division factor.
  * @retval None
  */
void RST_CLK_HSEclkPrescaler(RST_CLK_HSE_C1_Divisor HSEclkDivValue)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RST_CLK_HSEclkDivValue(HSEclkDivValue));

    temp = MDR_RST_CLK->RTC_CLOCK;

    /* Clear HSE_SEL bits */
    temp &= ~RST_CLK_RTC_CLOCK_HSE_SEL_Msk;

    /* Set the HSE_SEL bits */
    temp |= (uint32_t)HSEclkDivValue << RST_CLK_RTC_CLOCK_HSE_SEL_Pos;

    /* Store the new value */
    MDR_RST_CLK->RTC_CLOCK = temp;
}

/**
  * @brief  Enables or disables the RTCHSE clock.
  * @param  NewState - @ref FunctionalState - new state of the HSE_RTC_EN.
  * @retval None
  */
void RST_CLK_RTC_HSEclkEnable(FunctionalState NewState)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    temp = MDR_RST_CLK->RTC_CLOCK;

    /* Form new value */
    if (NewState != DISABLE)
    {
        /* Enable HSI RTC clk by setting the HSE_RTC_EN bit in the RTC_CLOCK register */
        temp |= RST_CLK_RTC_CLOCK_HSE_RTC_EN;
    }
    else
    {
        /* Disable HSI RTC clk by resetting the HSE_RTC_EN bit in the RTC_CLOCK register */
        temp &= ~RST_CLK_RTC_CLOCK_HSE_RTC_EN;
    }

    /* Configure RTC_CLOCK register with new value */
    MDR_RST_CLK->RTC_CLOCK = temp;
}

/**
 * @brief  Select the clock source for PER1_C1
 * @param  CLK_SRC - @ref RST_CLK_PER1_C1_TypeDef - specifies the PER1_C1 clock source.
 * @retval None
 */
void RST_CLK_PCLKPer1_C1_CLKSelection(RST_CLK_PER1_C1_TypeDef CLK_SRC)
{
    /* Check the parameters */
    assert_param(IS_RST_CLK_PER1_C1_CLK_SRC(CLK_SRC));

    /* Cleare old clk src and set the new clk src */
    MDR_RST_CLK->PER1_CLOCK = (MDR_RST_CLK->PER1_CLOCK & (~RST_CLK_PER1_CLOCK_C1_SEL_Msk)) | (uint32_t)CLK_SRC;
}

/**
 * @brief  Select the clock source for PER1_C2
 * @param  CLK_SRC - @ref RST_CLK_PER1_C2_TypeDef - specifies the PER1_C2 clock source.
 * @retval None
 */
void RST_CLK_PCLKPer1_C2_CLKSelection(RST_CLK_PER1_C2_TypeDef CLK_SRC)
{
    /* Check the parameters */
    assert_param(IS_RST_CLK_PER1_C2_CLK_SRC(CLK_SRC));

    MDR_RST_CLK->PER1_CLOCK = (MDR_RST_CLK->PER1_CLOCK & (~RST_CLK_PER1_CLOCK_C2_SEL_Msk)) | (uint32_t)CLK_SRC;
}

/**
  * @brief  Select the CPU clock source.
  * @param  CPU_CLK - @ref RST_CLK_CPU_C1_Source - specifies the CPU clock source.
  * @retval None
  */
void RST_CLK_CPUclkSelectionC1(RST_CLK_CPU_C1_Source CPU_CLK)
{
    uint32_t temp;

    /* Check the parameters */
    assert_param(IS_RST_CLK_CPU_C1_SOURCE(CPU_CLK));

    temp = MDR_RST_CLK->CPU_CLOCK;

    /* Clear CPU_C3_SEL bits */
    temp &= ~RST_CLK_CPU_CLOCK_CPU_C1_SEL_Msk;

    /* Set the CPU_C3_SEL bits */
    temp |= CPU_CLK;

    /* Store the new value */
    MDR_RST_CLK->CPU_CLOCK = temp;
}

/**
  * @brief  Set the DMA clock state.
  * @param  NewState - @ref FunctionalState - new state of the DMA clock.
  * @retval None
  */
void RST_CLK_DMACmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        MDR_RST_CLK->PER1_CLOCK |= RST_CLK_PER1_CLOCK_DMA_EN;
    }
    else
    {
        MDR_RST_CLK->PER1_CLOCK &= ~RST_CLK_PER1_CLOCK_DMA_EN;
    }
}

/**
  * @brief  Get the DMA clock state.
  * @param  None
  * @retval @ref FunctionalState - the state of the DMA clock.
  */
FunctionalState RST_CLK_DMAState(void)
{
    FunctionalState dma_state;

    if(MDR_RST_CLK->PER1_CLOCK & RST_CLK_PER1_CLOCK_DMA_EN)
    {
        dma_state = ENABLE;
    }
    else
    {
        dma_state = DISABLE;
    }

    return (FunctionalState)dma_state;
}

/**
  * @brief  Set the DEBUG clock state.
  * @param  NewState - @ref FunctionalState - new state of the DEBUG clock.
  * @retval None
  */
void RST_CLK_DEBUGCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        MDR_RST_CLK->PER1_CLOCK |= RST_CLK_PER1_CLOCK_DEBUG_EN;
    }
    else
    {
        MDR_RST_CLK->PER1_CLOCK &= ~RST_CLK_PER1_CLOCK_DEBUG_EN;
    }
}

/**
  * @brief  Get the DEBUG clock state.
  * @param  None
  * @retval @ref FunctionalState - the state of the DEBUG clock.
  */
FunctionalState RST_CLK_DEBUGState(void)
{
    FunctionalState debug_state;

    if(MDR_RST_CLK->PER1_CLOCK & RST_CLK_PER1_CLOCK_DEBUG_EN)
    {
        debug_state = ENABLE;
    }
    else
    {
        debug_state = DISABLE;
    }

    return (FunctionalState)debug_state;
}

/**
  * @brief  Enables or disables clock of peripherals.
  * @param  RST_CLK_PCLK: specifies the peripheral to gates its clock.
  *         This parameter can be any combination of the @ref RST_CLK_PCLK_TypeDef values.
  * @param  NewState - @ref FunctionalState - new state of the specified peripheral clock.
  * @retval None
  */
void RST_CLK_PCLKcmd(uint32_t RST_CLK_PCLK, FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_RST_CLK_PCLK(RST_CLK_PCLK));

    if (NewState != DISABLE)
    {
        MDR_RST_CLK->PER2_CLOCK |= RST_CLK_PCLK;
    }
    else
    {
        MDR_RST_CLK->PER2_CLOCK &= ~RST_CLK_PCLK;
    }
}

/**
  * @brief  Put the frequencies of different on chip clocks in *RST_CLK_Clocks structure.
  * @param  RST_CLK_Clocks: pointer to a @ref RST_CLK_ClocksTypeDef structure that will hold
  *         the clocks frequencies.
  * @retval None
  */
void RST_CLK_GetClocksFreq(RST_CLK_FreqTypeDef* RST_CLK_Clocks)
{
    uint32_t cpu_c1_freq, cpu_c2_freq, cpu_c3_freq;
    uint32_t adc_c1_freq, adc_c3_freq;
    uint32_t adciu_c1_freq, adciu_c3_freq;
    uint32_t hsi_c1_freq, hse_c1_freq;
    uint32_t per_c1_freq, per_c2_freq;
    uint32_t pll_mul;
    uint32_t temp;

    /* Check the parameter */
    assert_param(RST_CLK_Clocks != 0);

    /* Compute CPU_CLK, ADC_CLK, ADCIU_CLK, RTCHSI, RTCHSE clocks frequencies */

    /* Compute CPU_CLK frequency */

    /* Determine CPU_C1 frequency */
    if( MDR_RST_CLK->CPU_CLOCK & (2 << RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos) )
    {
        cpu_c1_freq = HSE_Value;
    }
    else
    {
        cpu_c1_freq = HSI_Value;
    }

    if( MDR_RST_CLK->CPU_CLOCK & (1 << RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos) )
    {
        cpu_c1_freq /= 2;
    }

    /* Determine CPU_C2 frequency */
    cpu_c2_freq = cpu_c1_freq;

    /* Determine CPU PLL output frequency */
    pll_mul = ((MDR_RST_CLK->PLL_CONTROL >> RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos) & (uint32_t) 0x0F) + 1;

    if (MDR_RST_CLK->CPU_CLOCK & (1 << RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos))
    {
        cpu_c2_freq *= pll_mul;
    }

    /*Select CPU_CLK from HSI, CPU_C3, LSE, LSI cases */
    switch ((MDR_RST_CLK->CPU_CLOCK >> RST_CLK_CPU_CLOCK_HCLK_SEL_Pos) & (uint32_t) 0x03)
    {
        case 0:
            /* HSI */
            RST_CLK_Clocks->CPU_CLK_Frequency = HSI_Value;
            break;
        case 1:
            /* CPU_C3 */
            /* Determine CPU_C3 frequency */
            if ((MDR_RST_CLK->CPU_CLOCK >> RST_CLK_CPU_CLOCK_CPU_C3_SEL_Pos & (uint32_t) 0x08) == 0x00)
            {
                cpu_c3_freq = cpu_c2_freq;
            }
            else
            {
                cpu_c3_freq = cpu_c2_freq / (1 << ((MDR_RST_CLK->CPU_CLOCK >> RST_CLK_CPU_CLOCK_CPU_C3_SEL_Pos & (uint32_t) 0x07) + 1));
            }
            RST_CLK_Clocks->CPU_CLK_Frequency = cpu_c3_freq;
            break;
        case 2:
            /* LSE */
            RST_CLK_Clocks->CPU_CLK_Frequency = LSE_Value;
            break;
        default: /* case 3 */
            /* LSI */
            RST_CLK_Clocks->CPU_CLK_Frequency = LSI_Value;
            break;
    }

    /* Compute RTCHSI_CLK frequency */
    temp = MDR_RST_CLK->RTC_CLOCK >> RST_CLK_RTC_CLOCK_HSI_SEL_Pos;
    if ((temp & 0x08U) == 0)
    {
        hsi_c1_freq = HSI_Value;
    }
    else
    {
        temp = 1 << ((temp & 0x07U) + 1);
        hsi_c1_freq = HSI_Value / temp;
    }
    RST_CLK_Clocks->RTCHSI_Frequency = hsi_c1_freq;

    /* Compute RTCHSE_CLK frequency */
    temp = MDR_RST_CLK->RTC_CLOCK >> RST_CLK_RTC_CLOCK_HSE_SEL_Pos;
    if ((temp & 0x08U) == 0)
    {
        hse_c1_freq = HSE_Value;
    }
    else
    {
        temp = 1 << ((temp & 0x07U) + 1);
        hse_c1_freq = HSE_Value / temp;
    }
    RST_CLK_Clocks->RTCHSE_Frequency = hse_c1_freq;

    /* Determine clock frequency per_c1 and per_c2 */
    if(MDR_RST_CLK->PER1_CLOCK & 0x02)
    {
        per_c1_freq = LSE_Value;
    }
    else
    {
        per_c1_freq = LSI_Value;
    }

    if(MDR_RST_CLK->PER1_CLOCK & 0x01)
    {
        per_c1_freq /= 2;
    }

    switch((MDR_RST_CLK->PER1_CLOCK >> 2) & 0x03)
    {
        case 0:
            per_c2_freq = cpu_c1_freq;
            break;
        case 1:
            per_c2_freq = per_c1_freq;
            break;
        case 2:
            per_c2_freq = cpu_c1_freq * pll_mul;
            break;
        case 3:
            per_c2_freq = HSI_Value;
            break;
    }

    /* Compute UARTx_CLK frequencies */
    temp = 1 << (MDR_RST_CLK->UART_CLOCK & 0x07); // UART1 prescaler
    temp = per_c2_freq / temp;
    RST_CLK_Clocks->UART_Frequency[0] = temp;

    temp = 1 << ((MDR_RST_CLK->UART_CLOCK >> 8) & 0x07); // UART2 prescaler
    temp = per_c2_freq / temp;
    RST_CLK_Clocks->UART_Frequency[1] = temp;

    /* Compute TIMx_CLK frequencies */
    temp = 1 << (MDR_RST_CLK->TIM_CLOCK & 0x07); // TIM1 prescaler
    temp = per_c2_freq / temp;
    RST_CLK_Clocks->TIMER_Frequency[0] = temp;

    temp = 1 << ((MDR_RST_CLK->TIM_CLOCK >> 8) & 0x07); // TIM2 prescaler
    temp = per_c2_freq / temp;
    RST_CLK_Clocks->TIMER_Frequency[1] = temp;

    /* Compute SSP1_CLK frequency */
    temp = 1 << (MDR_RST_CLK->SSP_CLOCK & 0x07); // SSP1 prescaler
    temp = per_c2_freq / temp;
    RST_CLK_Clocks->SSP_Frequency = temp;

    /* Compute ADC_CLK frequency */

    /* Select ADC_C1 from cpu_c1, per_c1_freq, cpu_c2_freq, HSI_Value cases */
    switch ((MDR_RST_CLK->ADC_CLOCK >> RST_CLK_ADC_CLOCK_ADC_C1_SEL_Pos) & (uint32_t) 0x03)
    {
        case 0:
            /* ADC_C1 */
            adc_c1_freq = cpu_c1_freq;
            break;
        case 1:
            /* ADC_C1 */
            adc_c1_freq = per_c1_freq;
            break;
        case 2:
            /* ADC_C2 */
            adc_c1_freq = cpu_c2_freq;
            break;
        default: /* case 3 */
            /* ADC_C2 */
            adc_c1_freq = HSI_Value;
            break;
    }

    temp = MDR_RST_CLK->ADC_CLOCK >> RST_CLK_ADC_CLOCK_ADC_C3_SEL_Pos;
    if ((temp & 0x08U) == 0)
    {
        adc_c3_freq = adc_c1_freq;
    }
    else
    {
        temp = 1 << ((temp & 0x07U) + 1);
        adc_c3_freq = adc_c1_freq / temp;
    }

    RST_CLK_Clocks->ADC_CLK_Frequency = adc_c3_freq;

    /* Compute ADCIU_CLK frequency */

    /* Select ADCIU_C1 from cpu_c1, per_c1_freq, cpu_c2_freq, HSI_Value cases */
    switch ((MDR_RST_CLK->ADC_CLOCK >> RST_CLK_ADC_CLOCK_ADCIU_C1_SEL_Pos) & (uint32_t) 0x03)
    {
        case 0:
            /* ADCIU_C1 */
            adciu_c1_freq = cpu_c1_freq;
            break;
        case 1:
            /* ADCIU_C1 */
            adciu_c1_freq = per_c1_freq;
            break;
        case 2:
            /* ADCIU_C1 */
            adciu_c1_freq = cpu_c2_freq;
            break;
        default: /* case 3 */
            /* ADCIU_C1 */
            adciu_c1_freq = HSI_Value;
            break;
    }

    temp = MDR_RST_CLK->ADC_CLOCK >> RST_CLK_ADC_CLOCK_ADCIU_C3_SEL_Pos;
    if ((temp & 0x08U) == 0)
    {
        adciu_c3_freq = adciu_c1_freq;
    }
    else
    {
        temp = 1 << ((temp & 0x07U) + 1);
        adciu_c3_freq = adciu_c1_freq / temp;
    }

    RST_CLK_Clocks->ADCIU_CLK_Frequency = adciu_c3_freq;

}

/**
  * @brief  Checks whether the specified RCC flag is set or not.
  * @param  RST_CLK_FLAG - @ref RST_CLK_Flags - specifies the flag to check.
  * @retval @ref FlagStatus - The state of RCC_FLAG (SET or RESET).
  */
FlagStatus RST_CLK_GetFlagStatus(RST_CLK_Flags RST_CLK_FLAG)
{
    uint32_t statusreg;
    FlagStatus bitstatus;

    /* Check the parameters */
    assert_param(IS_RST_CLK_FLAG(RST_CLK_FLAG));

    /* Get the register */
    if ((FLAG_SFR_MASK & RST_CLK_FLAG) == (uint32_t)0x20)
    {
        /* The flag to check is in CLOCK_STATUS register */
        statusreg = MDR_RST_CLK->CLOCK_STATUS;
    }
    else
    {
        /* The flag to check is in REG_0F register */
        statusreg = MDR_BKP->REG_0F;
    }

    /* Get the flag status on proper position */
    if ((statusreg & ((uint32_t)1 << (RST_CLK_FLAG & FLAG_MASK))) != (uint32_t)0x00)
    {
        bitstatus = SET;
    }
    else
    {
        bitstatus = RESET;
    }

    /* Return the flag status */
    return bitstatus;
}

/** @} */ /* End of group RST_CLK_Private_Functions */

/** @} */ /* End of group RST_CLK */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_rst_clk.c */
