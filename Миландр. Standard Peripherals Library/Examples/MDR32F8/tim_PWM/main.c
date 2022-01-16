/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure TIMER in PWM mode, output signal with const CCR value
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR1986VE8T.h"
#include "MDR32F8_it.h"
#include "spec.h"

#include "mdr32f8_config.h"
#include "mdr32f8_port.h"
#include "mdr32f8_clkctrl.h"
#include "mdr32f8_uart.h"
#include "mdr32f8_ssp.h"
#include "mdr32f8_tim.h"

/** @addtogroup MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup tim_PWM tim_PWM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CCR2_Val 5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
TIMER_CntInitTypeDef sTIM_CntInit;
TIMER_ChnInitTypeDef sTIM_ChnInit;
TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;

volatile uint32_t ch = 0;
volatile uint32_t c = 0;

uint32_t dat;
			
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void POR_disable(void);
void KEY_reg_accs(void);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
		
int main(void)
{	
	/* ONLY REV2 MCU, errata 0015. Disable Power-on-Reset control. Hold the SW4 button down until operation complete */
	//POR_disable();

	// Key to access clock control 
	UNLOCK_UNIT (CLK_CNTR_key);
	// Key to access fault control
	UNLOCK_UNIT (FT_CNTR_key);
	// Key to access BKP control
	UNLOCK_UNIT (BKP_key); 	

	CLKCTRL_DeInit();

	BKPCNTR_SRILOWconfig (BKPCNTR_SRILOW_upto_50MHz);

	/* Enable HSE0 clock */
	CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);

	/* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
		

	CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSE0div1, PLL_DIVQ_Q_1_DV, 8);//PLL, SRC, Q, N

	if(CLKCTRL_CPU_PLLstatus(0) == ERROR)
		while(1);

	CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_PLL0);			


	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTA_EN, ENABLE);
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_TMR1_EN, ENABLE);

	UNLOCK_UNIT (PORTA_key);


	/*PORT configuration*/
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_20 | PORT_Pin_21);//tmr1_ch1p,n
	PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_7;
	PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
	PORT_Init(PORTA, &PORT_InitStructure);

	//TIMER_BRGInit(MDR_TMR1,TIMER_HCLKdiv1);
	TIM_CLK_en(TIM1, TIM_CLKdiv1);

	/* Initializes the TIMERx Counter ------------------------------------*/
	sTIM_CntInit.TIMER_Prescaler                = 2;
	sTIM_CntInit.TIMER_Period                   = 10;
	sTIM_CntInit.TIMER_CounterMode              = TIMER_CntMode_ClkFixedDir;
	sTIM_CntInit.TIMER_CounterDirection         = TIMER_CntDir_Up;
	sTIM_CntInit.TIMER_EventSource              = TIMER_EvSrc_None;
	sTIM_CntInit.TIMER_FilterSampling           = TIMER_FDTS_TIMER_CLK_div_1;
	sTIM_CntInit.TIMER_ARR_UpdateMode           = TIMER_ARR_Update_Immediately;
	sTIM_CntInit.TIMER_ETR_FilterConf           = TIMER_Filter_1FF_at_TIMER_CLK;
	sTIM_CntInit.TIMER_ETR_Prescaler            = TIMER_ETR_Prescaler_None;
	sTIM_CntInit.TIMER_ETR_Polarity             = TIMER_ETRPolarity_NonInverted;
	sTIM_CntInit.TIMER_BRK_Polarity             = TIMER_BRKPolarity_NonInverted;
	TIMER_CntInit (MDR_TMR1,&sTIM_CntInit);

	/* Initializes the TIMER1 Channel 1,1N-------------------------------------*/
	TIMER_ChnStructInit(&sTIM_ChnInit);

	sTIM_ChnInit.TIMER_CH_Mode                = TIMER_CH_MODE_PWM;
	sTIM_ChnInit.TIMER_CH_REF_Format          = TIMER_CH_REF_Format6;
	sTIM_ChnInit.TIMER_CH_Number              = TIMER_CHANNEL1;
	TIMER_ChnInit(MDR_TMR1, &sTIM_ChnInit);

	TIMER_SetChnCompare(MDR_TMR1, TIMER_CHANNEL1, CCR2_Val);

	/* Initializes the TIMER1 Channel 1,1N-------------------------------*/

	TIMER_ChnOutStructInit(&sTIM_ChnOutInit);

	sTIM_ChnOutInit.TIMER_CH_DirOut_Polarity          = TIMER_CHOPolarity_NonInverted;
	sTIM_ChnOutInit.TIMER_CH_DirOut_Source            = TIMER_CH_OutSrc_REF;
	sTIM_ChnOutInit.TIMER_CH_DirOut_Mode              = TIMER_CH_OutMode_Output;
	sTIM_ChnOutInit.TIMER_CH_NegOut_Polarity          = TIMER_CHOPolarity_NonInverted;
	sTIM_ChnOutInit.TIMER_CH_NegOut_Source            = TIMER_CH_OutSrc_REF;
	sTIM_ChnOutInit.TIMER_CH_NegOut_Mode              = TIMER_CH_OutMode_Output;
	sTIM_ChnOutInit.TIMER_CH_Number                   = TIMER_CHANNEL1;
	TIMER_ChnOutInit(MDR_TMR1, &sTIM_ChnOutInit);

	/* Enable TIMER1 */
	TIMER_Cmd(MDR_TMR1,ENABLE);

	/*Main cycle*/									
	while (1)
	{

	}
}

//-----------------------------assert_param--------------------------------
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint32_t file_id, uint32_t line)
{
  /* User can add his own implementation to report the source file ID and line number.
     Ex: printf("Wrong parameters value: file Id %d on line %d\r\n", file_id, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint32_t file_id, uint32_t line, const uint8_t* expr)
{
  /* User can add his own implementation to report the source file ID, line number and
     expression text.
     Ex: printf("Wrong parameters value (%s): file Id %d on line %d\r\n", expr, file_id, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_ASSERT_INFO */

/**
  * @}
  */ 	

/**
  * @}
  */ 	

/**
  * @}
  */ 	

/******************* (C) COPYRIGHT 2021 Milandr *********/

/* END OF FILE main.c */

