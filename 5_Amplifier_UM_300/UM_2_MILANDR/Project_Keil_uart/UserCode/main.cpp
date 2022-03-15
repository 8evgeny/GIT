#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_iwdg.h"

#include "main.h"

#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
//#include "MDR32F9Qx_eeprom.h"


static PORT_InitTypeDef PortInit;
static UART_InitTypeDef UART_InitStructure1;
static UART_InitTypeDef UART_InitStructure2;

int main (void)
{
   RST_CLK_HSEconfig(RST_CLK_HSE_ON);
   while(RST_CLK_HSEstatus() != SUCCESS);

   /* Configures the CPU_PLL clock source */
   RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);

   /* Enables the CPU_PLL */
   RST_CLK_CPU_PLLcmd(ENABLE);
   if (RST_CLK_CPU_PLLstatus() == ERROR) {
      while (1);
   }

   /* Enables the RST_CLK_PCLK_EEPROM */
   RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
   /* Sets the code latency value */

//   EEPROM_SetLatency(EEPROM_Latency_3);

   /* Select the CPU_PLL output as input for CPU_C3_SEL */
   RST_CLK_CPU_PLLuse(ENABLE);
   /* Set CPUClk Prescaler */
   RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

   /* Select the CPU clock source */
   RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

  /* Enables the HSE clock on PORTF */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF,ENABLE);

  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA,ENABLE);

  /* Fill PortInit structure*/
  PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
  PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
  PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
  PortInit.PORT_PD = PORT_PD_DRIVER;
  PortInit.PORT_GFEN = PORT_GFEN_OFF;
  PortInit.PORT_FUNC = PORT_FUNC_OVERRID;
  PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
  PortInit.PORT_MODE = PORT_MODE_DIGITAL;

  /* Configure PORTF pins 1 (UART2_TX) as output */
  PortInit.PORT_OE = PORT_OE_OUT;
  PortInit.PORT_Pin = PORT_Pin_1;
  PORT_Init(MDR_PORTF, &PortInit);

  /* Configure PORTF pins 0 (UART2_RX) as input */
  PortInit.PORT_OE = PORT_OE_IN;
  PortInit.PORT_Pin = PORT_Pin_0;
  PORT_Init(MDR_PORTF, &PortInit);

  /* Configure PORTF pins 6 (UART1_TX) as output */
  PortInit.PORT_OE = PORT_OE_OUT;
  PortInit.PORT_Pin = PORT_Pin_6;
  PORT_Init(MDR_PORTA, &PortInit);

  /* Configure PORTF pins 5 (UART1_RX) as input */
  PortInit.PORT_OE = PORT_OE_IN;
  PortInit.PORT_Pin = PORT_Pin_5;
  PORT_Init(MDR_PORTA, &PortInit);


  /* Select HSI/2 as CPU_CLK source*/
  //RST_CLK_CPU_PLLconfig (RST_CLK_CPU_PLLsrcHSIdiv2,0);

  /* Enables the CPU_CLK clock on UART2 */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);

  /* Enables the CPU_CLK clock on UART1 */
  RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);


  /* Set the HCLK division factor = 1 for UART2*/
  UART_BRGInit(MDR_UART2, UART_HCLKdiv1);

  /* Set the HCLK division factor = 1 for UART1*/
  UART_BRGInit(MDR_UART1, UART_HCLKdiv1);


  /* Initialize UART_InitStructure2 */
  UART_InitStructure2.UART_BaudRate                = 115000;
  UART_InitStructure2.UART_WordLength              = UART_WordLength8b;
  UART_InitStructure2.UART_StopBits                = UART_StopBits1;
  UART_InitStructure2.UART_Parity                  = UART_Parity_No;
  UART_InitStructure2.UART_FIFOMode                = UART_FIFO_ON;
  UART_InitStructure2.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

  /* Configure UART2 parameters*/
  UART_Init (MDR_UART2,&UART_InitStructure2);

  /* Configure UART1 parameters*/
  UART_Init (MDR_UART2,&UART_InitStructure2);

  /* Initialize UART_InitStructure1 */
  UART_InitStructure1.UART_BaudRate                = 115000;
  UART_InitStructure1.UART_WordLength              = UART_WordLength8b;
  UART_InitStructure1.UART_StopBits                = UART_StopBits1;
  UART_InitStructure1.UART_Parity                  = UART_Parity_No;
  UART_InitStructure1.UART_FIFOMode                = UART_FIFO_ON;
  UART_InitStructure1.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

  /* Configure UART1 parameters*/
  UART_Init (MDR_UART1,&UART_InitStructure1);

  /* Configure UART1 parameters*/
  UART_Init (MDR_UART1,&UART_InitStructure1);

  /* Enables UART1 peripheral */
  UART_Cmd(MDR_UART1,ENABLE);


  uint8_t tmp_data;

  while (1)
  {
    /* Check TXFE flag */
    while (UART_GetFlagStatus (MDR_UART2, UART_FLAG_RXFE) == SET);

    tmp_data = UART_ReceiveData(MDR_UART2);

    UART_SendData(MDR_UART2, tmp_data); // обратно во 2 uart
    while (UART_GetFlagStatus (MDR_UART2, UART_FLAG_TXFE) != SET);

    UART_SendData(MDR_UART1, tmp_data); // обратно в 1 uart
    while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);

  }
}

