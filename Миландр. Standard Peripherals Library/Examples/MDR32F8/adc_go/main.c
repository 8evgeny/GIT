/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to configure ADC channels and test them in ADC GO mode.
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
#include "spec.h"

#include "mdr32f8_config.h"
#include "mdr32f8_port.h"
#include "mdr32f8_clkctrl.h"
#include "mdr32f8_adc.h"

/** @addtogroup MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup ADC_GO ADC_GO
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
/* Private define ------------------------------------------------------------*/
#define CH0 0
#define CH1 1
#define CH2 2
#define CH3 3
#define CH4 4
#define CH5 5
#define CH6 6
#define CH7 7
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADCx_InitTypeDef ADCx_IS;
uint32_t res;
/* Private function prototypes -----------------------------------------------*/
uint32_t TestADCch(uint32_t ch);
void TestADC(void);
/* Private functions ---------------------------------------------------------*/

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
	
	/* Set CLKCTRL to default */
	CLKCTRL_DeInit();
	
	BKPCNTR_SRILOWconfig (BKPCNTR_SRILOW_upto_50MHz);
	
	/* Enable HSE0 clock */
	CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);
	
	/* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
	while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}
	
	/* Select PLL0 clk src, PLL0_N, PLL0_Q to get FINT = FIN*(PLLn_N)/(PLLn_Q+1) */
	CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSIdiv1, PLL_DIVQ_Q_1_DV, 8);//PLLn, SRC, Q, N
	while(CLKCTRL_CPU_PLLstatus(0) != SUCCESS){}
	CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_PLL0);
	
	/* Enable PORTx clk */
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);
	CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_ADC1_EN, ENABLE);

	/* Allow write to PORT regs */
	UNLOCK_UNIT (PORTC_key);
			
	/* Configure PortC[6:13] for analog input */
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_6|PORT_Pin_7|PORT_Pin_8|PORT_Pin_9|
									PORT_Pin_10|PORT_Pin_11|PORT_Pin_12|PORT_Pin_13);
	
	PORT_InitStructure.PORT_CANALOG  = PORT_CANALOG_DIGITAL;//Clear dig, set analog
	PORT_Init(PORTC, &PORT_InitStructure);
	
	ADCx_CLK_en(ADC1, ADCx_CLKSRC_MAX_CLK, ADCx_CLKdiv4);
	UNLOCK_UNIT (ADC1_key);
    
    ADCx_StructInit (&ADCx_IS);
	
	ADCx_IS.ADC_RH_MODE = ADCx_CONFIG0_RH_MODE_ADC_POW;
    ADCx_IS.ADC_SELR = ENABLE;
	ADCx_IS.ADC_SETUP = 5;
	ADCx_IS.ADC_PAUSE = 5;
	ADCx_IS.ADC_FIFOEN_0_31 = FIEN0|FIEN1|FIEN2|FIEN3|FIEN4|FIEN5|FIEN6|FIEN7;
	
	ADC_Init(ADC1, &ADCx_IS);
	ADCx_Cmd(ADC1, ENABLE);
	
	TestADC();
	
	while(1);
	
}

uint32_t TestADCch(uint32_t ch)
{
	
	ADCx_SetChannel(ADC1, ch);
	ADCx_Start(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_NE)==RESET){}
	res = ADCx_GetResult(ADC1);
	if (res>0xC4E || res<0xBEA) 
	{
		__NOP();
	}
	
	return 0;
}

void TestADC(void)
{	

	TestADCch(CH0);
	TestADCch(CH1);
	TestADCch(CH2);
	TestADCch(CH3);
	TestADCch(CH4);
	TestADCch(CH5);
	TestADCch(CH6);
	TestADCch(CH7);	

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

