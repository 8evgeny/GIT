/**
  ******************************************************************************
  * @file    mdr32f8_clkctrl.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    20/07/2015
  * @brief   This file contains all the CLKCTRL firmware functions.
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
  * FILE mdr32f8_clkctrl.c
  */

/* Includes ------------------------------------------------------------------*/
#include "mdr32f8_clkctrl.h"
#include "MDR1986VE8T.h"


#define ASSERT_INFO_FILE_ID FILEID__MDR32F8_CLKCTRL_C

/** @addtogroup __MDR32F8_StdPeriph_Driver MDR32F8 Standard Peripherial Driver
  * @{
  */

/** @addtogroup CLKCTRL CLKCTRL
  * @{
  */

/** @defgroup CLKCTRL_Exported_Types CLKCTRL Exported Types
  * @{
  */

/** @brief CLKCTRL driver modules
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/

/** @defgroup CLKCTRL_Private_Variables CLKCTRL Private Variables
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define CLKCTRL_OFFSET              (CLK_CNTR_BASE - PERIPH_BASE)
#define BKP_OFFSET                  (BKP_BASE - PERIPH_BASE)
#define SFR_OFFSET(TP, MOD, SFR)    ((uint32_t)&((TP*)MOD)->SFR)
#define BB_ADDR(TP, MOD, SFR, BIT)  (PERIPH_BB_BASE + SFR_OFFSET(TP, MOD, SFR)*32 + BIT*4)
#define CLKCTRL_BB(SFR, BIT)        BB_ADDR(CLKControl, CLKCTRL_OFFSET, SFR, BIT)
#define BKP_BB(SFR, BIT)            BB_ADDR(BKPControl, BKP_OFFSET, SFR, BIT)

#define CPUCLKdiv_msk				(~(uint32_t)0x0000FFFF)

#define PLL_MULoffset				8
#define PLL_SELoffset				29
#define PLL_MULclr 					(~(uint32_t)0x00004100)
#define PLL_SELclr 					(~(uint32_t)0xE0007F1F)

#define FLAG_MASK                   ((uint8_t)0x1F)
#define FLAG_SFR_MASK               ((uint8_t)0xE0)

#define HSEonTimeOut               ((uint16_t)0x0600)
#define LSEonTimeOut               ((uint16_t)0x0600)
#define HSIonTimeOut               ((uint16_t)0x0600)
#define LSIonTimeOut               ((uint16_t)0x0600)
#define PLLCPUonTimeOut            ((uint16_t)0x0600)

#define HSITRIM_OFFSET              24
#define HSITRIM_MASK                ((uint32_t)(0x3F << HSITRIM_OFFSET))

#define HSION_OFFSET                22
#define HSION_MASK                  ((uint32_t)(1 << HSION_OFFSET))

#define LSITRIM_OFFSET              16
#define LSITRIM_MASK                ((uint32_t)(0x1F << LSITRIM_OFFSET))

#define LSION_OFFSET                15
#define LSION_MASK                  ((uint32_t)(1 << LSION_OFFSET))

#define BKPCNTR_REG_63_TMR0_HSION_BB      BKP_BB(REG_63_TMR0, HSION_OFFSET)
#define BKPCNTR_REG_63_TMR1_HSION_BB      BKP_BB(REG_63_TMR1, HSION_OFFSET)
#define BKPCNTR_REG_63_TMR2_HSION_BB      BKP_BB(REG_63_TMR2, HSION_OFFSET)
	
#define BKPCNTR_REG_63_TMR0_LSION_BB      BKP_BB(REG_63_TMR0, LSION_OFFSET)
#define BKPCNTR_REG_63_TMR1_LSION_BB      BKP_BB(REG_63_TMR1, LSION_OFFSET)
#define BKPCNTR_REG_63_TMR2_LSION_BB      BKP_BB(REG_63_TMR2, LSION_OFFSET)
	
#define PLL0CPUON_BB                 CLKCTRL_BB(PLL0_CLK, 27)
#define PLL0CPURLD_BB                CLKCTRL_BB(PLL0_CLK, 28)
#define PLL1CPUON_BB                 CLKCTRL_BB(PLL1_CLK, 27)
#define PLL1CPURLD_BB                CLKCTRL_BB(PLL1_CLK, 28)
#define PLL2CPUON_BB                 CLKCTRL_BB(PLL2_CLK, 27)
#define PLL2CPURLD_BB                CLKCTRL_BB(PLL2_CLK, 28)


/** @} */ /* End of group CLKCTRL_Private_Variables */
	
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup RCC_Private_Functions
  * @{
  */ 

/**
  * @brief  Set the CLKCTRL clock configuration to the default reset state.
  * @param  None
  * @retval None
  */
void CLKCTRL_DeInit(void)
{
	/* Prepare HSI clk */
	BKP->REG_63_TMR0 |= (1<<22);
	BKP->REG_63_TMR1 |= (1<<22);
	BKP->REG_63_TMR2 |= (1<<22);

	/* Reset MAX_CLK bits, MAX_CLK = HSI */
	CLK_CNTR->MAX_CLK = 0x00000000;

	/* Reset PLLn_CONTROL bits */
	CLK_CNTR->PLL0_CLK = 0x00000000;
	CLK_CNTR->PLL1_CLK = 0x00000000;
	CLK_CNTR->PLL2_CLK = 0x00000000;

	/* Reset HSEn_CONTROL bits */	
	CLK_CNTR->HSE0_CLK = 0x00000000;
	CLK_CNTR->HSE1_CLK = 0x00000000;

	/* Reset PERn_CLK bits except CLKCTRL and BKPCTRL bits */	
	CLK_CNTR->PER0_CLK = 0x00000003;
	CLK_CNTR->PER1_CLK = 0x00000000;
}



/**
  * @brief  BKPCNTR_REG_63_TMRx_HSIcmd:  HSI (High Speed Internal) clock mode selection
  * @param  NewState: mode selector - DISABLE, ENABLE
  *         @arg DISABLE:   switch off HSI clock generator
  *         @arg ENABLE:    switch on HSI clock generator
  * @retval None
  */
void BKPCNTR_REG_63_TMRx_HSIcmd(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	*(__IO uint32_t *) BKPCNTR_REG_63_TMR0_HSION_BB = (uint32_t)NewState;
	*(__IO uint32_t *) BKPCNTR_REG_63_TMR1_HSION_BB = (uint32_t)NewState;
	*(__IO uint32_t *) BKPCNTR_REG_63_TMR2_HSION_BB = (uint32_t)NewState;
}

/**
  * @brief  BKPCTRL_HSIstatus: HSI clock status
  * @param  None
  * @retval enum ErrorStatus: SUCCESS if HSI clock is ready, else ERROR
  */
ErrorStatus BKPCNTR_REG_63_TMRx_HSIstatus(void)
{
	__IO uint32_t startCounter = 0;
	ErrorStatus state;
	FlagStatus flag0;
	FlagStatus flag1;
	FlagStatus flag2;

	/* Wait until HSI is ready or time out is occure */
	do
	{
		flag0 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR0_HSI_RDY);
		flag1 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR1_HSI_RDY);
		flag2 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR2_HSI_RDY);
		startCounter++;
	} while ((startCounter < HSIonTimeOut) && (flag0 == RESET) && (flag1 == RESET) && (flag2 == RESET));

	if (CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR0_HSI_RDY) != RESET && 
		CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR1_HSI_RDY) != RESET && 
		CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR2_HSI_RDY) != RESET)
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
  * @brief  BKPCTRL_HSIadjust: HSI clock frequency adjustment by HSITRIM setting
  * @param  HSItrimValue
  *         This parameter must be a number between 0 and 0x3F.
  * @retval None
  */
void BKPCNTR_REG_63_TMRx_HSIadjust(uint32_t HSItrimValue)
{
	uint32_t temp;
	/* Check the parameters */
	assert_param(IS_BKPCTRL_REG_63_HSI_TRIM_VALUE(HSItrimValue));
	temp = BKP->REG_63_TMR0;
	/* Clear HSITRIM[5:0] bits */
	temp &= ~HSITRIM_MASK;
	/* Set the HSITRIM[5:0] bits according to HSItrimValue value */
	temp |= (uint32_t)HSItrimValue << HSITRIM_OFFSET;
	/* Store the new value */
	BKP->REG_63_TMR0 = temp;
	BKP->REG_63_TMR1 = temp;
	BKP->REG_63_TMR2 = temp;
}

/**
  * @brief  Checks whether the specified CLKCTRL_BKPCTRL flag is set or not.
  * @param  CLKCTRL_BKPCTRL_FLAG: specifies the flag to check.
  *         This parameter can be one of the following values:
  *         @arg BKPCTRL_REG_63_TMRx_LSE_RDY
  *              BKPCTRL_REG_63_TMRx_LSI_RDY
  *              BKPCTRL_REG_63_TMRx_HSI_RDY
  *							 CLKCTRL_HSEn_STAT_HSEn_RDY
  *							 CLKCTRL_PLLn_STAT_PLLn_RDY
  * @retval The new state of CLKCTRL_BKPCTRL_FLAG (SET or RESET).
  */
FlagStatus CLKCTRL_BKPCTRL_GetFlagStatus(uint32_t CLKCTRL_BKPCTRL_FLAG)
{
	uint32_t tmp = 0;
	uint32_t tmp_E = 0;
	uint32_t tmp_P = 0;
	uint32_t tmp_I = 0;

	uint32_t statusreg;
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_CLKCTRL_BKPCTRL_FLAG(CLKCTRL_BKPCTRL_FLAG));

	tmp_E = (CLKCTRL_BKPCTRL_FLAG>>5)&0x03;
	tmp_P = (CLKCTRL_BKPCTRL_FLAG>>7)&0x0F;
	tmp_I = CLKCTRL_BKPCTRL_FLAG>>11;

	/* Get the register */
	//--------------tmp_E-----------------
  if (tmp_E==2)                                                  
  {
    statusreg = CLK_CNTR->HSE0_STAT;
  }
		else if (tmp_E==3)                                                   
		{
		statusreg = CLK_CNTR->HSE1_STAT;
		}
	//--------------tmp_P-----------------
			else if (tmp_P==1)                                                   
			{
				statusreg = CLK_CNTR->PLL0_STAT;
			}
				else if (tmp_P==2)                                                   
				{
					statusreg = CLK_CNTR->PLL1_STAT;
				}
					else if (tmp_P==3)                                                   
					{
						statusreg = CLK_CNTR->PLL2_STAT;
					}
	//--------------tmp_I-----------------
	else if (tmp_I==1) 
	{
	statusreg = BKP->REG_63_TMR0;
	}			
		else if (tmp_I==2) 
		{
		statusreg = BKP->REG_63_TMR1;
		}		
			else if (tmp_I==3) 
			{
			statusreg = BKP->REG_63_TMR2;
			}		
	/* Get the flag status on proper position */
	tmp = CLKCTRL_BKPCTRL_FLAG & FLAG_MASK;	
	if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
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

/**
  * @brief  CLKCTRL_MAX_CLKSelection: select the CPU clock source.
  * @param  MAX_CLK: specifies the CPU clock source.
  *         @arg CLKCTRL_MAX_CLK_HSIdiv1
  *         @arg CLKCTRL_MAX_CLK_HSIdiv2
  *         @arg CLKCTRL_MAX_CLK_HSE0div1
  *         @arg CLKCTRL_MAX_CLK_HSE0div2
  *         @arg CLKCTRL_MAX_CLK_HSE1div1
  *         @arg CLKCTRL_MAX_CLK_HSE1div2
  *         @arg CLKCTRL_MAX_CLK_LSI
  *         @arg CLKCTRL_MAX_CLK_LSE
  *         @arg CLKCTRL_MAX_CLK_PLLn
  * @retval None
  */
void CLKCTRL_MAX_CLKSelection(uint32_t MAX_CLK)
{
	/* Check the parameters */
	assert_param(IS_CLKCTRL_MAX_CLK(MAX_CLK));
	/* Store the new value */
	CLK_CNTR->MAX_CLK = MAX_CLK;
}

/**
  * @brief  CLKCTRL_CPUclkPrescaler: configures the CPU_C3_SEL division factor switch.
  * @param  CPUclkDivValue: specifies the CPU_PLL clock division factor.
  *         @arg RST_CLK_CPUclkDIV1
  *         @arg RST_CLK_CPUclkDIV2
  *         @arg RST_CLK_CPUclkDIV4
  *         @arg RST_CLK_CPUclkDIV8
  *         @arg RST_CLK_CPUclkDIV16
  *         @arg RST_CLK_CPUclkDIV32
  *         @arg RST_CLK_CPUclkDIV64
  *         @arg RST_CLK_CPUclkDIV128
  *         @arg RST_CLK_CPUclkDIV256
  * @retval None
  */
void CLKCTRL_CPUclkPrescaler(uint32_t CPUclkDivValue)
{
	uint32_t temp;

	/* Check the parameters */
	assert_param(IS_CLKCTRL_CPU_CLK_CPUclkDIV(CPUclkDivValue));

	temp = CLK_CNTR->CPU_CLK;
	/* Clear CPU_C3_SEL bits */
	temp &= CPUCLKdiv_msk;
	/* Set the CPU_C3_SEL bits */
	temp |= CPUclkDivValue;
	/* Store the new value */
	CLK_CNTR->CPU_CLK = temp;
}

/**
  * @brief	CLKCTRL_PER0_CLKcmd: enables or disables clock of peripherals.
  * @param  CLKCTRL_PER0_CLK: specifies the peripheral to gates its clock.
  *         This parameter can be any combination of the following values:
  *           @arg CLKCTRL_PER0_CLK
  * @param  NewState: new state of the specified peripheral clock.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CLKCTRL_PER0_CLKcmd(uint32_t CLKCTRL_PER0_CLK, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	assert_param(IS_CLKCTRL_PER0_CLK(CLKCTRL_PER0_CLK));

	if (NewState != DISABLE)
	{
		CLK_CNTR->PER0_CLK |= CLKCTRL_PER0_CLK;
	}
	else
	{
		CLK_CNTR->PER0_CLK &= ~CLKCTRL_PER0_CLK;
	}
}

/**
  * @brief	CLKCTRL_PER1_CLKcmd: enables or disables clock of peripherals.
  * @param  CLKCTRL_PER1_CLK: specifies the peripheral to gates its clock.
  *         This parameter can be any combination of the following values:
  *           @arg CLKCTRL_PER1_CLK
  * @param  NewState: new state of the specified peripheral clock.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CLKCTRL_PER1_CLKcmd(uint32_t CLKCTRL_PER1_CLK, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));
	assert_param(IS_CLKCTRL_PER1_CLK(CLKCTRL_PER1_CLK));

	if (NewState != DISABLE)
	{
		CLK_CNTR->PER1_CLK |= CLKCTRL_PER1_CLK;
	}
	else
	{
		CLK_CNTR->PER1_CLK &= ~CLKCTRL_PER1_CLK;
	}
}

/**
  * @brief  CLKCTRL_HSEconfig - HSE (High Speed External) clock mode and source selection
  * @param  CLKCTRL_HSE - mode selector
  *         @arg CLKCTRL_HSE_OFF    -  switch off HSE clock generator
  *         @arg CLKCTRL_HSE_ON     -  switch on HSE clock generator
  *         @arg CLKCTRL_HSE_Bypass -  use external clock source
  * @retval None
  */
void CLKCTRL_HSEconfig(uint32_t CLKCTRL_HSE)
{
  /* Reset HSEON and HSEBYP bits before configuring the HSE */
  /* Configure HSE */
  switch (CLKCTRL_HSE)
  {
    case CLKCTRL_HSE0_CLK_ON:
			CLK_CNTR->HSE0_CLK &= ~((uint32_t)(CLKCTRL_HSE0_CLK_ON | CLKCTRL_HSE0_CLK_BYP));
      /* Set HSEON bit */
      CLK_CNTR->HSE0_CLK |= CLKCTRL_HSE0_CLK_ON;
      break;

    case CLKCTRL_HSE0_CLK_BYP:
			CLK_CNTR->HSE0_CLK &= ~((uint32_t)(CLKCTRL_HSE0_CLK_ON | CLKCTRL_HSE0_CLK_BYP));
      /* Set HSEBYP and HSEON bits */
      CLK_CNTR->HSE0_CLK |= CLKCTRL_HSE0_CLK_ON | CLKCTRL_HSE0_CLK_BYP;
      break;
		
		case CLKCTRL_HSE1_CLK_ON:
			CLK_CNTR->HSE1_CLK &= ~((uint32_t)(CLKCTRL_HSE1_CLK_ON | CLKCTRL_HSE1_CLK_BYP));
      /* Set HSEON bit */
      CLK_CNTR->HSE1_CLK |= CLKCTRL_HSE0_CLK_ON;
      break;

    case CLKCTRL_HSE1_CLK_BYP:
			CLK_CNTR->HSE1_CLK &= ~((uint32_t)(CLKCTRL_HSE1_CLK_ON | CLKCTRL_HSE1_CLK_BYP));
      /* Set HSEBYP and HSEON bits */
      CLK_CNTR->HSE1_CLK |= CLKCTRL_HSE0_CLK_ON | CLKCTRL_HSE0_CLK_BYP;
      break;
		
		case CLKCTRL_HSE0_CLK_OFF:
			CLK_CNTR->HSE0_CLK &= ~((uint32_t)(CLKCTRL_HSE0_CLK_ON | CLKCTRL_HSE0_CLK_BYP));
			break;
		
		case CLKCTRL_HSE1_CLK_OFF:
			CLK_CNTR->HSE1_CLK &= ~((uint32_t)(CLKCTRL_HSE1_CLK_ON | CLKCTRL_HSE1_CLK_BYP));
			break;
		
    default:
      break;
  }
}

/**
  * @brief  CLKCTRL_HSEstatus - HSE clock status
  * @param  None
  * @retval enum ErrorStatus - SUCCESS if HSE clock is ready, else ERROR
  */
ErrorStatus CLKCTRL_HSEstatus(uint32_t CLKCTRL_HSE)
{
	__IO uint32_t startCounter = 0;
	ErrorStatus state;
	FlagStatus flag;


	if(CLKCTRL_HSE==CLKCTRL_HSEn_STAT_HSE0_RDY){
	/* Wait until HSE is ready or time out is occure */
		do
		{
			flag = CLKCTRL_BKPCTRL_GetFlagStatus(CLKCTRL_HSEn_STAT_HSE0_RDY);
			startCounter++;
		} while ((startCounter < HSEonTimeOut) && (flag == RESET));

		if (CLKCTRL_BKPCTRL_GetFlagStatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != RESET)
		{
			state = SUCCESS;
		}
		else
		{
			state = ERROR;
		}
	}
	else if(CLKCTRL_HSE==CLKCTRL_HSEn_STAT_HSE1_RDY){
	/* Wait until HSE is ready or time out is occure */
		do
		{
			flag = CLKCTRL_BKPCTRL_GetFlagStatus(CLKCTRL_HSEn_STAT_HSE1_RDY);
			startCounter++;
		} while ((startCounter < HSEonTimeOut) && (flag == RESET));

		if (CLKCTRL_BKPCTRL_GetFlagStatus(CLKCTRL_HSEn_STAT_HSE1_RDY) != RESET)
		{
			state = SUCCESS;
		}
		else
		{
			state = ERROR;
		}
	}
	return state;
}

/**
  * @brief  BKPCTRL_REG_63_LSEconfig - LSE (Low Speed External) clock mode and source selection
  * @param  CLKCTRL_BKPCTRL_LSE - mode selector
  *         @arg CLKCTRL_BKPCTRL_LSE_OFF    -  switch off LSE clock generator
  *         @arg CLKCTRL_BKPCTRL_LSE_ON     -  switch on LSE clock generator
  *         @arg CLKCTRL_BKPCTRL_LSE_Bypass -  use external clock source
  * @retval None
  */
void BKPCTRL_REG_63_LSEconfig(uint32_t CLKCTRL_BKPCTRL_LSE)
{
	/* Reset LSEON and LSEBYP bits before configuring the LSE */
	BKP->REG_63_TMR0 &= ~((uint32_t)(BKPCTRL_REG_63_TMR0_LSE_ON | BKPCTRL_REG_63_TMR0_LSE_BYP));
	BKP->REG_63_TMR1 &= ~((uint32_t)(BKPCTRL_REG_63_TMR1_LSE_ON | BKPCTRL_REG_63_TMR1_LSE_BYP));
	BKP->REG_63_TMR2 &= ~((uint32_t)(BKPCTRL_REG_63_TMR2_LSE_ON | BKPCTRL_REG_63_TMR2_LSE_BYP));
	switch (CLKCTRL_BKPCTRL_LSE)
	{
	case BKPCTRL_REG_63_TMR0_LSE_ON:
		/* Set LSEON bit */
		BKP->REG_63_TMR0 |= BKPCTRL_REG_63_TMR0_LSE_ON;
		BKP->REG_63_TMR1 |= BKPCTRL_REG_63_TMR1_LSE_ON;
		BKP->REG_63_TMR2 |= BKPCTRL_REG_63_TMR2_LSE_ON;
		break;

	case BKPCTRL_REG_63_TMR0_LSE_BYP:
		/* Set LSEBYP and LSEON bits */
		BKP->REG_63_TMR0 |= (BKPCTRL_REG_63_TMR0_LSE_ON | BKPCTRL_REG_63_TMR0_LSE_BYP);
		BKP->REG_63_TMR1 |= (BKPCTRL_REG_63_TMR1_LSE_ON | BKPCTRL_REG_63_TMR1_LSE_BYP);
		BKP->REG_63_TMR2 |= (BKPCTRL_REG_63_TMR2_LSE_ON | BKPCTRL_REG_63_TMR2_LSE_BYP);
		break;

	default:
		break;
	}
}

/**
  * @brief  BKPCTRL_REG_63_LSEstatus - LSE clock status
  * @param  None
  * @retval enum ErrorStatus - SUCCESS if LSE clock is ready, else ERROR
  */
ErrorStatus BKPCTRL_REG_63_LSEstatus(void)
{
	__IO uint32_t startCounter = 0;
	ErrorStatus state;
	FlagStatus flag0;
	FlagStatus flag1;
	FlagStatus flag2;

	/* Wait until LSE is ready or time out is occure */
	do
	{
	flag0 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR0_LSE_RDY);
	flag1 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR1_LSE_RDY);
	flag2 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR2_LSE_RDY);
	startCounter++;
	} while ((startCounter < LSEonTimeOut) && (flag0 == RESET) && (flag1 == RESET) && (flag2 == RESET));

	if (CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR0_LSE_RDY) != RESET && 
		CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR1_LSE_RDY) != RESET && 
	    CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR2_LSE_RDY) != RESET)
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
  * @brief  BKPCNTR_REG_63_LSIcmd:  LSI (High Speed Internal) clock mode selection
  * @param  NewState: mode selector - DISABLE, ENABLE
  *         @arg DISABLE:   switch off LSI clock generator
  *         @arg ENABLE:    switch on LSI clock generator
  * @retval None
  */
void BKPCNTR_REG_63_TMRx_LSIcmd(FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	*(__IO uint32_t *) BKPCNTR_REG_63_TMR0_LSION_BB = (uint32_t) NewState;
	*(__IO uint32_t *) BKPCNTR_REG_63_TMR1_LSION_BB = (uint32_t) NewState;
	*(__IO uint32_t *) BKPCNTR_REG_63_TMR2_LSION_BB = (uint32_t) NewState;
}

/**
  * @brief  BKPCTRL_REG_63_HSIadjust: LSI clock frequency adjustment by LSITRIM setting
  * @param  LSItrimValue
  *         This parameter must be a number between 0 and 0x1F.
  * @retval None
  */
void BKPCTRL_REG_63_LSIadjust(uint32_t LSItrimValue)
{
	uint32_t temp;
	/* Check the parameters */
	assert_param(IS_BKPCTRL_REG_63_LSI_TRIM_VALUE(LSItrimValue));
	temp = BKP->REG_63_TMR0;
	/* Clear LSITRIM[4:0] bits */
	temp &= ~LSITRIM_MASK;
	/* Set the LSITRIM[4:0] bits according to LSItrimValue value */
	temp |= (uint32_t)LSItrimValue << LSITRIM_OFFSET;
	/* Store the new value */
	BKP->REG_63_TMR0 = temp;
	BKP->REG_63_TMR1 = temp;
	BKP->REG_63_TMR2 = temp;
}

/**
  * @brief  BKPCTRL_REG_63_LSIstatus - LSI clock status
  * @param  None
  * @retval enum ErrorStatus - SUCCESS if LSI clock is ready, else ERROR
  */
ErrorStatus BKPCTRL_REG_63_LSIstatus(void)
{
  __IO uint32_t startCounter = 0;
  ErrorStatus state;
  FlagStatus flag0;
	FlagStatus flag1;
	FlagStatus flag2;

  /* Wait until LSI is ready or time out is occure */
  do
  {
    flag0 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR0_LSI_RDY);
		flag1 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR1_LSI_RDY);
		flag2 = CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR2_LSI_RDY);
    startCounter++;
  } while ((startCounter < LSIonTimeOut) && (flag0 == RESET) && (flag1 == RESET) && (flag2 == RESET));

  if (CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR0_LSI_RDY) != RESET && 
		CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR1_LSI_RDY) != RESET && 
	CLKCTRL_BKPCTRL_GetFlagStatus(BKPCTRL_REG_63_TMR2_LSI_RDY) != RESET)
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
  * @param  PLLn:
  *         @arg PLL0: use PLL0
  *         @arg PLL1: use PLL1
  *         @arg PLL2: use PLL2
  * @param  CLKCTRL_CPU_PLLsource: specifies the PLL entry clock source.
  *         @arg CLKCTRL_PLLn_CLK_SELECT_HSIdiv1: HSI oscillator clock is selected as CPU_PLL clock entry
  *         @arg CLKCTRL_PLLn_CLK_SELECT_HSIdiv2: HSI oscillator clock divided by 2 is selected as CPU_PLL clock entry
  *         @arg CLKCTRL_PLLn_CLK_SELECT_HSE0div1: HSE oscillator clock is selected as CPU_PLL clock entry
  *         @arg CLKCTRL_PLLn_CLK_SELECT_HSE0div2: HSE oscillator clock divided by 2 is selected as CPU_PLL clock entry
  * @param  CLKCTRL_PLLn_CLK_PLLn_Q: specifies the PLL multiplication factor.
  *         @arg PLL_DIVQ_Q_1
  *         @arg PLL_DIVQ_Q_2
  *         @arg PLL_DIVQ_Q_3
  *         @arg PLL_DIVQ_Q_4
  *         @arg PLL_DIVQ_Q_5
  *         @arg PLL_DIVQ_Q_6
  *         @arg PLL_DIVQ_Q_7
  *         @arg PLL_DIVQ_Q_8
  *         @arg PLL_DIVQ_Q_9
  *         @arg PLL_DIVQ_Q_10
  *         @arg PLL_DIVQ_Q_11
  *         @arg PLL_DIVQ_Q_12
  *         @arg PLL_DIVQ_Q_13
  *         @arg PLL_DIVQ_Q_14
  *         @arg PLL_DIVQ_Q_15
  *         @arg PLL_DIVQ_Q_16
  *         @arg PLL_DIVQ_Q_1_DV	
  *         @arg PLL_DIVQ_Q_2_DV	
  *         @arg PLL_DIVQ_Q_3_DV	
  *         @arg PLL_DIVQ_Q_4_DV	
  *         @arg PLL_DIVQ_Q_5_DV	
  *         @arg PLL_DIVQ_Q_6_DV	
  *         @arg PLL_DIVQ_Q_7_DV	
  *         @arg PLL_DIVQ_Q_8_DV	
  *         @arg PLL_DIVQ_Q_9_DV	
  *         @arg PLL_DIVQ_Q_10_DV	
  *         @arg PLL_DIVQ_Q_11_DV	
  *         @arg PLL_DIVQ_Q_12_DV	
  *         @arg PLL_DIVQ_Q_13_DV	
  *         @arg PLL_DIVQ_Q_14_DV	
  *         @arg PLL_DIVQ_Q_15_DV	
  *         @arg PLL_DIVQ_Q_16_DV	
  * @param  CLKCTRL_PLLn_CLK_PLLn_N:
  *         This parameter must be a number between 3 and 75.
  * @retval None
  */
void CLKCTRL_CPU_PLLconfig (uint32_t PLLn, uint32_t CLKCTRL_CPU_PLLsource,
	uint32_t CLKCTRL_PLLn_CLK_PLLn_Q, uint32_t CLKCTRL_PLLn_CLK_PLLn_N)
{
  uint32_t temp;
	uint32_t PLLON_def;
	uint32_t PLLPLD_def;
	
	switch (PLLn)
  {
    case PLL0:
		temp = CLK_CNTR->PLL0_CLK;
		temp &= PLL_SELclr;
		temp |= (CLKCTRL_CPU_PLLsource<<PLL_SELoffset) | (CLKCTRL_PLLn_CLK_PLLn_Q) | (CLKCTRL_PLLn_CLK_PLLn_N<<PLL_MULoffset);
		CLK_CNTR->PLL0_CLK=temp;
		
		PLLON_def = PLL0CPUON_BB;
		PLLPLD_def = PLL0CPURLD_BB;
      break;
		case 1:
    temp = CLK_CNTR->PLL1_CLK;
		temp &= PLL_SELclr;
		temp |= (CLKCTRL_CPU_PLLsource<<PLL_SELoffset) | (CLKCTRL_PLLn_CLK_PLLn_Q) | (CLKCTRL_PLLn_CLK_PLLn_N<<PLL_MULoffset) ;
		CLK_CNTR->PLL1_CLK=temp;
		PLLON_def = PLL1CPUON_BB;
		PLLPLD_def = PLL1CPURLD_BB;
      break;
    case 2:
    temp = CLK_CNTR->PLL2_CLK;
		temp &= PLL_SELclr;
		temp |= (CLKCTRL_CPU_PLLsource<<PLL_SELoffset) | (CLKCTRL_PLLn_CLK_PLLn_Q) | (CLKCTRL_PLLn_CLK_PLLn_N<<PLL_MULoffset);
		CLK_CNTR->PLL2_CLK=temp;
		PLLON_def = PLL2CPUON_BB;
		PLLPLD_def = PLL2CPURLD_BB;
      break;
    default:
      break;
  }	
	
	*(__IO uint32_t *) PLLON_def = (uint32_t) 0x01;
	
	*(__IO uint32_t *) PLLPLD_def = (uint32_t) 0x01;
  *(__IO uint32_t *) PLLPLD_def = (uint32_t) 0x00;
	
}

/**
  * @brief  Enables or disables the CPU_PLL.
  * @param  NewState: new state of the CPU_PLL. This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void CLKCTRL_CPU_PLLcmd ( FunctionalState NewState )
{
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) PLL0CPUON_BB = (uint32_t) NewState;
}

/**
  * @brief  CLKCTRL_CPU_PLLstatus: CPU_PLL status
  * @param  None
  * @retval enum ErrorStatus: SUCCESS if CPU_PLL output clock is ready, else ERROR
  */
ErrorStatus CLKCTRL_CPU_PLLstatus(uint32_t PLLn)
{
  __IO uint32_t startCounter = 0;
  ErrorStatus state;
  FlagStatus flag;
	uint32_t fl;
	
		switch (PLLn)
  {
    case 0:
			fl = CLKCTRL_PLLn_STAT_PLL0_RDY;
      break;
		case 1:
			fl = CLKCTRL_PLLn_STAT_PLL1_RDY;
      break;
    case 2:
			fl = CLKCTRL_PLLn_STAT_PLL2_RDY;
      break;		
    default:
      break;
  }
  /* Wait until CPUPLL is ready or time out is occure */
  do
  {
    flag = CLKCTRL_BKPCTRL_GetFlagStatus(fl);
    startCounter++;
  } while ((startCounter < PLLCPUonTimeOut) && (flag == RESET));

  if (CLKCTRL_BKPCTRL_GetFlagStatus(fl) != RESET)
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
  * @brief  BKPCNTR_SRILOWconfig - Select the internal voltage regulators (LDOx) mode
  * @param  SRILOW: specifies the LDO mode.
  *   This parameter can be one of the following values:
  *     @arg BKPCNTR_SRILOW_upto_10MHz:  system clock is up to 10 MHz
  *     @arg BKPCNTR_SRILOW_upto_30MHz:  system clock is up to 30 MHz
  *     @arg BKPCNTR_SRILOW_upto_50MHz:  system clock is up to 50 MHz
  *     @arg BKPCNTR_SRILOW_upto_100MHz: system clock is up to 100 MHz
  *     @arg BKPCNTR_SRILOW_upto_1MHz:   system clock is up to 1 MHz
  *     @arg BKPCNTR_SRILOW_upto_500kHz: system clock is up to 500 kHz
  *     @arg BKPCNTR_SRILOW_upto_100kHz: system clock is up to 100 kHz
  *     @arg BKPCNTR_SRILOW_over_10kHz:  system clock is over 40 kHz (Recommended at system clock over 40 MHz when the MCU is installed in a contact device)
  * @retval None
  */
void BKPCNTR_SRILOWconfig(uint32_t SRILOW)
{
    uint32_t tmpreg_TMR0, tmpreg_TMR1, tmpreg_TMR2;
    /* Check the parameters */
    assert_param(IS_BKPCNTR_SRILOW(SRILOW));
    
    /* Select the LDO0-3 mode (DUcc) */
    /* Clear SRILOWx[2:0] bits */
    tmpreg_TMR0 = BKP->REG_61_TMR0 & (~BKPCTRL_REG_61_TMRx_SRILOWx_Msk);
    tmpreg_TMR1 = BKP->REG_61_TMR1 & (~BKPCTRL_REG_61_TMRx_SRILOWx_Msk);
    tmpreg_TMR2 = BKP->REG_61_TMR2 & (~BKPCTRL_REG_61_TMRx_SRILOWx_Msk);
    
    /* Set SRILOWx[2:0] bits according to SRILOW */
    tmpreg_TMR0 |= SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW3_Pos | SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW2_Pos |
                   SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW1_Pos | SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW0_Pos;
    tmpreg_TMR1 |= SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW3_Pos | SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW2_Pos |
                   SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW1_Pos | SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW0_Pos; 
    tmpreg_TMR2 |= SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW3_Pos | SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW2_Pos |
                   SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW1_Pos | SRILOW<<BKPCTRL_REG_61_TMRx_SRILOW0_Pos;
    BKP->REG_61_TMR0 = tmpreg_TMR0;
    BKP->REG_61_TMR1 = tmpreg_TMR1;
    BKP->REG_61_TMR2 = tmpreg_TMR2;
    
    /* Select the LDO4-5 mode (DUccB, DUcc_PLL)*/
    /* Clear SRILOWx[2:0] bits */
    tmpreg_TMR0 = BKP->REG_62_TMR0 & (~BKPCTRL_REG_62_TMRx_SRILOWx_Msk);
    tmpreg_TMR1 = BKP->REG_62_TMR1 & (~BKPCTRL_REG_62_TMRx_SRILOWx_Msk);
    tmpreg_TMR2 = BKP->REG_62_TMR2 & (~BKPCTRL_REG_62_TMRx_SRILOWx_Msk);
    
    /* Set SRILOWx[2:0] bits according to SRILOW */
    tmpreg_TMR0 |= SRILOW<<BKPCTRL_REG_62_TMRx_SRILOW5_Pos | SRILOW<<BKPCTRL_REG_62_TMRx_SRILOW4_Pos;
    tmpreg_TMR1 |= SRILOW<<BKPCTRL_REG_62_TMRx_SRILOW5_Pos | SRILOW<<BKPCTRL_REG_62_TMRx_SRILOW4_Pos;
    tmpreg_TMR2 |= SRILOW<<BKPCTRL_REG_62_TMRx_SRILOW5_Pos | SRILOW<<BKPCTRL_REG_62_TMRx_SRILOW4_Pos;
    BKP->REG_62_TMR0 = tmpreg_TMR0;
    BKP->REG_62_TMR1 = tmpreg_TMR1;
    BKP->REG_62_TMR2 = tmpreg_TMR2;
}

/** @} */

/** @} */ /* End of group CLKCTRL */

/** @} */ /* End of group __MDR32F8_StdPeriph_Driver */

/******************* (C) COPYRIGHT 2021 Milandr *********************************
*
* END OF FILE mdr32f8_clkctrl.c */


