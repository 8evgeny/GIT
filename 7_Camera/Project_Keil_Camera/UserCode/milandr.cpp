#include "main.h"
#include "milandr.h"


Milandr::Milandr()
{
    init();

}

Milandr::~Milandr()
{
}

void Milandr::init()
{
    static PORT_InitTypeDef PORTB_Init;
    static UART_InitTypeDef UART_InitStructure1;

    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    while(RST_CLK_HSEstatus() != SUCCESS);

    /* Configures the CPU_PLL clock source */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);

    /* Enables the CPU_PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);
    if (RST_CLK_CPU_PLLstatus() == ERROR) {
        while (1);
    }

    /* Select the CPU_PLL output as input for CPU_C3_SEL */
    RST_CLK_CPU_PLLuse(ENABLE);
    /* Set CPUClk Prescaler */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

    /* Select the CPU clock source */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

    /* Enables the HSE clock on PORTB */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB,ENABLE);

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

    //выход
    PORTB_Init.PORT_Pin = PORT_Pin_10 | PORT_Pin_9 | PORT_Pin_8 | PORT_Pin_7;
    PORTB_Init.PORT_OE = PORT_OE_OUT;
    PORTB_Init.PORT_FUNC = PORT_FUNC_PORT;
    PORTB_Init.PORT_SPEED = PORT_SPEED_SLOW;
    PORT_Init(MDR_PORTB, &PORTB_Init);


    /* Enables the CPU_CLK clock on UART1 */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
    /* Set the HCLK division factor = 1 for UART1*/
    UART_BRGInit(MDR_UART1, UART_HCLKdiv1);

    /* Initialize UART_InitStructure1 */
    UART_DeInit(MDR_UART1);

    UART_InitStructure1.UART_BaudRate                = 115200;
    UART_InitStructure1.UART_WordLength              = UART_WordLength8b;
    UART_InitStructure1.UART_StopBits                = UART_StopBits1;
    UART_InitStructure1.UART_Parity                  = UART_Parity_No;
//    UART_InitStructure1.UART_FIFOMode                = UART_FIFO_OFF;
    UART_InitStructure1.UART_FIFOMode                = UART_FIFO_ON;
//    UART_InitStructure1.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
    UART_InitStructure1.UART_HardwareFlowControl     = UART_HardwareFlowControl_None ;

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


}





