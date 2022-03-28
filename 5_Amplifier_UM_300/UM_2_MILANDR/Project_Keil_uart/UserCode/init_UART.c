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


    /* Enables the CPU_CLK clock on UART1 */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
    /* Set the HCLK division factor = 1 for UART1*/
    UART_BRGInit(MDR_UART1, UART_HCLKdiv1);


    /* Initialize UART_InitStructure1 */
    UART_DeInit(MDR_UART1);

    UART_InitStructure1.UART_BaudRate                = 38400;
    UART_InitStructure1.UART_WordLength              = UART_WordLength8b;
    UART_InitStructure1.UART_StopBits                = UART_StopBits1;
    UART_InitStructure1.UART_Parity                  = UART_Parity_No;
    UART_InitStructure1.UART_FIFOMode                = UART_FIFO_ON;
    UART_InitStructure1.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

    /* Configure UART1 parameters*/
    UART_Init (MDR_UART1,&UART_InitStructure1);

    NVIC_EnableIRQ (UART1_IRQn);

    /* Enables UART1 peripheral */
    UART_ITConfig (MDR_UART1, UART_IT_RX, ENABLE);//Разрешение прерывания по приему
    UART_ITConfig (MDR_UART1, UART_IT_TX, ENABLE);//Разрешение прерывания по окончани передачи
    UART_ITConfig (MDR_UART1, UART_IT_RT, ENABLE);
    UART_ITConfig (MDR_UART1, UART_IT_FE, ENABLE);
    UART_ITConfig (MDR_UART1, UART_IT_OE, ENABLE);
    UART_Cmd(MDR_UART1,ENABLE);

//UART_IT_OE  = ((uint32_t)0x0400), /*!< Buffer overflow interrupt (UARTOEINTR) */
//UART_IT_BE  = ((uint32_t)0x0200), /*!< Line break interrupt (UARTBEINTR) */
//UART_IT_PE  = ((uint32_t)0x0100), /*!< Parity error interrupt (UARTPEINTR) */
//UART_IT_FE  = ((uint32_t)0x0080), /*!< Frame structure error interrupt (UARTFEINTR) */
//UART_IT_RT  = ((uint32_t)0x0040), /*!< Data input timeout interrupt (UARTRTINTR) */
//UART_IT_TX  = ((uint32_t)0x0020), /*!< Transmitter interrupt (UARTTXINTR) */
//UART_IT_RX  = ((uint32_t)0x0010), /*!< Receiver interrupt (UARTRXINTR) */
//UART_IT_DSR = ((uint32_t)0x0008), /*!< Line nUARTDSR change interrupt (UARTDSRINTR) */
//UART_IT_DCD = ((uint32_t)0x0004), /*!< Line nUARTDCD change interrupt (UARTDCDINTR) */
//UART_IT_CTS = ((uint32_t)0x0002), /*!< Line nUARTCTS change interrupt (UARTCTSINTR) */
//UART_IT_RI  = ((uint32_t)0x0001)  /*!< Line nUARTRI change interrupt (UARTRIINTR) */

}
