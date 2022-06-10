/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   Main program body.
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
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Examples MDR1986BE4 StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR1986BE4_EVAL MDR1986BE4 Evaluation Board
  * @{
  */

/** @addtogroup UART_Interrupt UART_Interrupt
  * @{
  */

/* Private variables ---------------------------------------------------------*/
static PORT_InitTypeDef PortInit;
static UART_InitTypeDef UART_InitStructure;
uint32_t uart1_IT_TX_flag = RESET;
uint32_t uart2_IT_RX_flag = RESET;
uint8_t DataByte;
uint8_t ReciveByte;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main (void)
{
    /* Enables peripheral clocks */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB,ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC,ENABLE);

    /* Fill PortInit structure */
    PORT_StructInit(&PortInit);

    PortInit.PORT_FUNC = PORT_FUNC_MAIN;
    PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;

    /* Configure PORTB pins 0 (UART1_TXD) */
    PortInit.PORT_Pin = PORT_Pin_0;
    PORT_Init(MDR_PORTB, &PortInit);

    /* Configure PORTC pins 1 (UART2_RXD) */
    PortInit.PORT_Pin = PORT_Pin_1;
    PORT_Init(MDR_PORTC, &PortInit);

    /* Enables the CPU_CLK clock on UART1, UART2 */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);

    /* Set the CPU_CLK division factor = 1 for UART1, UART2 */
    UART_BRGInit(MDR_UART1, UART_HCLKdiv1);
    UART_BRGInit(MDR_UART2, UART_HCLKdiv1);
    
    /* Resets UART1, UART2 to default reset values */
    UART_DeInit(MDR_UART1);
    UART_DeInit(MDR_UART2);

    /* Initialize UART_InitStructure */
    UART_InitStructure.UART_BaudRate                = 115000;
    UART_InitStructure.UART_WordLength              = UART_WordLength8b;
    UART_InitStructure.UART_StopBits                = UART_StopBits1;
    UART_InitStructure.UART_Parity                  = UART_Parity_No;
    UART_InitStructure.UART_FIFOMode                = UART_FIFO_OFF;
    UART_InitStructure.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

    /* Configure UART1 parameters */
    UART_Init(MDR_UART1,&UART_InitStructure);

    /* Enable transmitter interrupt (UARTTXINTR) */
    UART_ITConfig(MDR_UART1, UART_IT_TX, ENABLE);

    /* Enables UART1 peripheral */
    UART_Cmd(MDR_UART1,ENABLE);

    /* Configure UART2 parameters */
    UART_Init(MDR_UART2,&UART_InitStructure);

    /* Enable Receiver interrupt */
    UART_ITConfig(MDR_UART2, UART_IT_RX, ENABLE);

    /* Enables UART2 peripheral */
    UART_Cmd(MDR_UART2,ENABLE);
    
    /* Enables UART1, UART2 interrupt in the NVIC */
    NVIC_EnableIRQ(UART1_IRQn);
    NVIC_EnableIRQ(UART2_IRQn);
    
    /* Send Data from UART1 */
    UART_SendData(MDR_UART1,DataByte);

    while (1)
    {
        /* Wait until uart2_IT_RX being SET */
        while(uart2_IT_RX_flag != SET){}

        /* Clear uart2_IT_RX_flag */
        uart2_IT_RX_flag = RESET;

        /* Recive data */
        ReciveByte = UART_ReceiveData(MDR_UART2);

        /* Wait until uart1_IT_TX being SET */
        while(uart1_IT_TX_flag != SET){}

        /* Clear uart1_IT_TX_flag */
        uart1_IT_TX_flag = RESET;
            
        /* Increment Data */
        DataByte++;

        /* Send Data from UART1 */
        UART_SendData(MDR_UART1,DataByte);
    }
}

/**
  * @brief  Reports the source file name, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @param  expr:
  * @retval None
  */
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the source file name and line number.
       Ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    /* User can add his own implementation to report the source file name, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group UART_Interrupt */

/** @} */ /* End of group __MDR1986BE4_EVAL */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2021 Milandr *******************************/

/* END OF FILE main.c */

