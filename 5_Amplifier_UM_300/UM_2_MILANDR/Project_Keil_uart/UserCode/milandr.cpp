#include "main.h"
#include "milandr.h"

Milandr::Milandr()
{
    initGPIO();
    initUART();

    setRESET_UPR(true);
    setFAN_ON(false);
    setGAIN_UPR(true);
    setCOMP_UPR(true);

}

Milandr::~Milandr()
{
}

void Milandr::initGPIO()
{
    //Порт A
    PORT_InitTypeDef PORTA_Init;
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
    PORTA_Init.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PORTA_Init.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PORTA_Init.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PORTA_Init.PORT_PD = PORT_PD_DRIVER;
    PORTA_Init.PORT_GFEN = PORT_GFEN_OFF;
    PORTA_Init.PORT_FUNC =  PORT_FUNC_PORT;
    PORTA_Init.PORT_SPEED = PORT_SPEED_MAXFAST;
    PORTA_Init.PORT_MODE = PORT_MODE_DIGITAL;
    //Выходы
    PORTA_Init.PORT_OE =    PORT_OE_OUT;
    PORTA_Init.PORT_Pin =   PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_7 ;
    PORT_Init(MDR_PORTA, &PORTA_Init);
    //Входы
    PORTA_Init.PORT_OE =    PORT_OE_IN;
    PORTA_Init.PORT_Pin =   PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6;
    PORT_Init(MDR_PORTA, &PORTA_Init);

    //Порт B инициализируется с UART
}

void Milandr::initUART()
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

    //Один пин на выход
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

}

bool Milandr::getMAINPOWER()
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_0))
    {
        return true;
    }
    return false;
}

bool Milandr::getRESPOWER()
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_1))
    {
        return true;
    }
    return false;
}

bool Milandr::getFAULT() //Инверсный
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_4))
    {
        return true;
    }
    return false;
}

bool Milandr::getCLIP_OTW() //Инверсный
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_5))
    {
        return true;
    }
    return false;
}

bool Milandr::getPOW_READY()
{
    if (1 == PORT_ReadInputDataBit(MDR_PORTA, PORT_Pin_6))
    {
        return true;
    }
    return false;
}

void Milandr::setGAIN_UPR (bool set)
{
    if(set)  PORT_SetBits(MDR_PORTA, PORT_Pin_2);
    if(!set) PORT_ResetBits(MDR_PORTA, PORT_Pin_2);
}

void Milandr::setCOMP_UPR (bool set)
{
    if(set)  PORT_SetBits(MDR_PORTA, PORT_Pin_3);
    if(!set) PORT_ResetBits(MDR_PORTA, PORT_Pin_3);
}

void Milandr::setFAN_ON (bool set)
{
    if(set)  PORT_SetBits(MDR_PORTA, PORT_Pin_7);
    if(!set) PORT_ResetBits(MDR_PORTA, PORT_Pin_7);
}

void Milandr::setRESET_UPR (bool set)
{
    if(set)  PORT_SetBits(MDR_PORTB, PORT_Pin_8);
    if(!set) PORT_ResetBits(MDR_PORTB, PORT_Pin_8);
}
