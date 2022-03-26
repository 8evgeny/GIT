#include "main.h"

void initUART()
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
    //   RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);
    /* Sets the code latency value */

    //   EEPROM_SetLatency(EEPROM_Latency_3);

    /* Select the CPU_PLL output as input for CPU_C3_SEL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* Set CPUClk Prescaler */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

    /* Select the CPU clock source */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

    /* Enables the HSE clock on PORTB */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB,ENABLE);

    /* Enables the HSE clock on PORTF */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF,ENABLE);
    
#ifdef UART_1
    
    /* Fill PortInit UART1 structure*/
    PORTB_Init.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PORTB_Init.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PORTB_Init.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PORTB_Init.PORT_PD = PORT_PD_DRIVER;
    PORTB_Init.PORT_GFEN = PORT_GFEN_OFF;
    PORTB_Init.PORT_FUNC = PORT_FUNC_ALTER;
    PORTB_Init.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORTB_Init.PORT_MODE = PORT_MODE_DIGITAL;

    /* Configure PORTB pins 5 (UART1_TX) as output  */
    PORTB_Init.PORT_OE = PORT_OE_OUT;
    PORTB_Init.PORT_Pin = PORT_Pin_5;
    PORT_Init(MDR_PORTB, &PORTB_Init);
    /* Configure PORTB pins 6 (UART1_RX) as input  */
    PORTB_Init.PORT_OE = PORT_OE_IN;
    PORTB_Init.PORT_Pin = PORT_Pin_6;
    PORT_Init(MDR_PORTB, &PORTB_Init);

    PORTB_Init.PORT_FUNC = PORT_FUNC_PORT;
    PORTB_Init.PORT_OE = PORT_OE_OUT;
    PORTB_Init.PORT_Pin = PORT_Pin_8;
    PORT_Init(MDR_PORTB, &PORTB_Init);
#endif

#ifdef UART_2
    /* Fill PortInit UART2 structure*/
    PortInitUART2.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PortInitUART2.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PortInitUART2.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PortInitUART2.PORT_PD = PORT_PD_DRIVER;
    PortInitUART2.PORT_GFEN = PORT_GFEN_OFF;
    PortInitUART2.PORT_FUNC = PORT_FUNC_OVERRID;
    PortInitUART2.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInitUART2.PORT_MODE = PORT_MODE_DIGITAL;

    //UART2 использую для прошивки в Linux
    /* Configure PORTF pins 1 (UART2_TX) as output */
    PortInitUART2.PORT_OE = PORT_OE_OUT;
    PortInitUART2.PORT_Pin = PORT_Pin_1;
    PORT_Init(MDR_PORTF, &PortInitUART2);
    /* Configure PORTF pins 0 (UART2_RX) as input */
    PortInitUART2.PORT_OE = PORT_OE_IN;
    PortInitUART2.PORT_Pin = PORT_Pin_0;
    PORT_Init(MDR_PORTF, &PortInitUART2);
#endif

    /* Select HSI/2 as CPU_CLK source*/
    //RST_CLK_CPU_PLLconfig (RST_CLK_CPU_PLLsrcHSIdiv2,0);
#ifdef UART_1
    /* Enables the CPU_CLK clock on UART1 */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
    /* Set the HCLK division factor = 1 for UART1*/
    UART_BRGInit(MDR_UART1, UART_HCLKdiv1);

#endif
#ifdef UART_2
    /* Enables the CPU_CLK clock on UART2 */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART2, ENABLE);
    /* Set the HCLK division factor = 1 for UART2*/
    UART_BRGInit(MDR_UART2, UART_HCLKdiv1);

#endif

#ifdef UART_1
    /* Initialize UART_InitStructure1 */
    UART_DeInit(MDR_UART1);

    UART_InitStructure1.UART_BaudRate                = 19200;
    UART_InitStructure1.UART_WordLength              = UART_WordLength8b;
    UART_InitStructure1.UART_StopBits                = UART_StopBits1;
    UART_InitStructure1.UART_Parity                  = UART_Parity_No;
    UART_InitStructure1.UART_FIFOMode                = UART_FIFO_OFF;
    UART_InitStructure1.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
    /* Configure UART1 parameters*/
    UART_Init (MDR_UART1,&UART_InitStructure1);

    NVIC_SetPriority (UART1_IRQn, 5);
    NVIC_EnableIRQ (UART1_IRQn);

    /* Enables UART1 peripheral */
    UART_ITConfig (MDR_UART1, UART_IT_RX, ENABLE);//Разрешение прерывания по приему
    UART_ITConfig (MDR_UART1, UART_IT_TX, ENABLE);//Разрешение прерывания по окончани передачи

    UART_Cmd(MDR_UART1,ENABLE);

#endif
#ifdef UART_2
    /* Initialize UART_InitStructure2 */
    UART_InitStructure2.UART_BaudRate                = 115000;
    UART_InitStructure2.UART_WordLength              = UART_WordLength8b;
    UART_InitStructure2.UART_StopBits                = UART_StopBits1;
    UART_InitStructure2.UART_Parity                  = UART_Parity_No;
    UART_InitStructure2.UART_FIFOMode                = UART_FIFO_ON;
    UART_InitStructure2.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
    /* Configure UART2 parameters*/
    UART_Init (MDR_UART2,&UART_InitStructure2);

    /* Enables UART2 peripheral */
    UART_ITConfig (MDR_UART2, UART_IT_RX, ENABLE);//Разрешение прерывания по приему
    UART_ITConfig (MDR_UART2, UART_IT_TX, ENABLE);//Разрешение прерывания по окончани передачи
    UART_Cmd(MDR_UART2,ENABLE);
#endif


}
