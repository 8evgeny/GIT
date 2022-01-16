/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   This examle shows how to configure UART, receive data from terminal and echoing it back
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

/** @addtogroup MDR32F8_StdPeriph_Examples
  * @{
  */

/** @addtogroup MDR32F8_Board_For_MCU
  * @{
  */

/** @addtogroup uart_echo uart_echo
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UART_FREQ     10000000 //10 MHz, Calculate it going through your clock tree, for example: 
/*
*           hse0=12
*              ||
*              \/
*           pll_mul=5
*              ||
*              \/
*           uart_div=6
*
* UART_FREQ = hse0 * pll_mul / uart_div
*           = 12   * 5       / 6
*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
UART_InitTypeDef UART_InitStructure;

volatile uint32_t ch = 0;
volatile uint32_t c = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void POR_disable(void);
void KEY_reg_accs(void);
void BlinkLine(uint32_t Pin);

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
    /* Set CLKCTRL to default */
    CLKCTRL_DeInit();
    /*HSE0 as the clk source*/
    /* Enable HSE0 clock */
    CLKCTRL_HSEconfig(CLKCTRL_HSE0_CLK_ON);

    /* Check HSE success, enable PLL0, check PLL0 success, select MAX_CLK src */
    while(CLKCTRL_HSEstatus(CLKCTRL_HSEn_STAT_HSE0_RDY) != SUCCESS){}

    CLKCTRL_MAX_CLKSelection(CLKCTRL_MAX_CLK_HSE0div1);	

    CLKCTRL_PER0_CLKcmd(CLKCTRL_PER0_CLK_MDR_PORTE_EN, ENABLE);
    CLKCTRL_PER1_CLKcmd(CLKCTRL_PER1_CLK_MDR_UART0_EN, ENABLE);

    UNLOCK_UNIT (PORTE_key);

    /*PORT configuration*/
    PORT_InitStructure.PORT_Pin   = (PORT_Pin_15 | PORT_Pin_16);
    PORT_InitStructure.PORT_SFUNC  = PORT_SFUNC_5;
    PORT_InitStructure.PORT_SANALOG  = PORT_SANALOG_DIGITAL;
    PORT_InitStructure.PORT_SPWR = PORT_SPWR_10;
    PORT_Init(PORTE, &PORT_InitStructure);

    UART_CLK_en(MDR_UART0,UART_CLKSRC_HSE0, UART_CLKdiv1);
    /*UART configuration*/
    UART_InitStructure.UART_BaudRate                = 9600;
    UART_InitStructure.UART_WordLength              = UART_WordLength8b;
    UART_InitStructure.UART_StopBits                = UART_StopBits1;
    UART_InitStructure.UART_Parity                  = UART_Parity_No;
    UART_InitStructure.UART_FIFOMode                = UART_FIFO_OFF;
    UART_InitStructure.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
    UART_Init (MDR_UART0,&UART_InitStructure, UART_FREQ);

    UART_Cmd(MDR_UART0,ENABLE);
    /*Int enable*/
    UART_ITConfig (MDR_UART0, UART_IT_RX, ENABLE);
    NVIC_EnableIRQ(UART0_IRQn);
    /*Main cycle*/
    while (1); 
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

