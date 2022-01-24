/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This example shows how to use external int from port configuring in input mode and output to other port
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

#include <rtx_os.h>

/** @addtogroup MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup rtos_led rtos_led
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;

/* Private define ------------------------------------------------------------*/
#define VD7 PORT_Pin_16
#define VD8 PORT_Pin_17
#define VD9 PORT_Pin_18
#define VD10 PORT_Pin_19
#define VD11 PORT_Pin_20
#define VD12 PORT_Pin_21
#define VD13 PORT_Pin_22
#define VD14 PORT_Pin_23

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/			
/* Private function prototypes -----------------------------------------------*/
void THREAD_LED0 (void *argument);
void THREAD_LED1 (void *argument);
void Blinky(uint32_t VD);
/* Private functions ---------------------------------------------------------*/
void Delay(__IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

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
	CLKCTRL_CPU_PLLconfig(PLL0, CLKCTRL_PLLn_CLK_SELECT_HSE0div1, PLL_DIVQ_Q_1_DV, 8);//PLLn, SRC, Q, N
		while(CLKCTRL_CPU_PLLstatus(0) != SUCCESS){}
			CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_PLL0);	
			
	/* Enable PORTx clk */
	CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTC_EN, ENABLE);

	/* Allow write to PORT regs */
	UNLOCK_UNIT (PORTC_key);

	/* Configure PORTC pins [16:23] for output */
	PORT_InitStructure.PORT_Pin   = (PORT_Pin_16|PORT_Pin_17|PORT_Pin_18|PORT_Pin_19|
																	 PORT_Pin_20|PORT_Pin_21|PORT_Pin_22|PORT_Pin_23);

	PORT_InitStructure.PORT_SOE    = PORT_SOE_OUT;
	PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
	PORT_InitStructure.PORT_SPD = PORT_SPD_OFF;
	PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;

	PORT_Init(PORTC, &PORT_InitStructure);

	osKernelInitialize ();
	  
	osThreadNew(THREAD_LED0, NULL, NULL); 
	osThreadNew(THREAD_LED1, NULL, NULL); 

	osKernelStart();
  
}
void THREAD_LED0 (void *argumenrt)
{
	while (1)
	{
		Blinky(VD7);
		Blinky(VD8);
		Blinky(VD9);
		Blinky(VD10);
	}
}
void THREAD_LED1 (void *argument)
{
	while (1)
	{
		Blinky(VD11);
		Blinky(VD12);
		Blinky(VD13);
		Blinky(VD14);

	}
}

void Blinky(uint32_t VD)
{	
		PORT_SetBits(PORTC, VD);
		Delay(100000);
		PORT_ResetBits(PORTC, VD);
		Delay(100000);
}

void SVCall_Handler (void)
{
  
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

