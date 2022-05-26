#include "main.h"
#include "milandr.h"
#include "outputSig.h"
#include <memory>
#include <vector>
PORT_InitTypeDef PORTB_Init;

void RCC_init() {
    RST_CLK_DeInit();	//Сброс настроек
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);			//включение внешнего кварца 8 МГц (CPU_C1_SEL)
    while (RST_CLK_HSEstatus() != SUCCESS);	//ждем включения HSE
    RST_CLK_CPUclkSelectionC1(RST_CLK_CPU_C1srcHSEdiv1);	//мультиплексор CPU_C1: источник тактирования CPU
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul9); 	//CPU_PLL = 8MHz * 9 = 72 MHz
    RST_CLK_CPU_PLLuse(ENABLE);	//мультиплексор CPU_C2_SEL: используем CPU_PLL для входа CPU_C3_SEL
    RST_CLK_CPU_PLLcmd(ENABLE);	//включаем CPU_PLL
    while (RST_CLK_CPU_PLLstatus() != SUCCESS);	//ждем включения CPU_PLL
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);	//предделитель CPU_C3_SEL = 1
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);	//источник тактирования - выход предделителя
    RST_CLK_HSIcmd(DISABLE);// Выключаем HSI

    /* Enables the HSE clock on PORTB PORTD*/
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB,ENABLE);
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD,ENABLE);
}

void PORT_StructInit_OUT(PORT_InitTypeDef* PORT_InitStruct)
{
 /* Reset PORT initialization structure parameters values */
    PORT_InitStruct->PORT_Pin = PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_5;
    PORT_InitStruct->PORT_OE = PORT_OE_OUT;
    PORT_InitStruct->PORT_PULL_UP = PORT_PULL_UP_OFF;
    PORT_InitStruct->PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PORT_InitStruct->PORT_PD_SHM = PORT_PD_SHM_OFF;
    PORT_InitStruct->PORT_PD = PORT_PD_DRIVER;
    PORT_InitStruct->PORT_GFEN = PORT_GFEN_OFF;
    PORT_InitStruct->PORT_FUNC = PORT_FUNC_PORT;
    PORT_InitStruct->PORT_SPEED = PORT_SPEED_MAXFAST;
    PORT_InitStruct->PORT_MODE = PORT_MODE_DIGITAL;
 }

void GPIO_init()
{
    static PORT_InitTypeDef PORTB_Init;
//    static PORT_InitTypeDef PORTD_Init;
    static UART_InitTypeDef UART_InitStructure1;

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

    //выходы
    PORTB_Init.PORT_FUNC = PORT_FUNC_PORT;
    PORTB_Init.PORT_OE = PORT_OE_OUT;
    PORTB_Init.PORT_Pin = PORT_Pin_10 | PORT_Pin_9 | PORT_Pin_8 | PORT_Pin_7;
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

//    Порт D отключаю

//    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD,ENABLE);
//    PORT_StructInit_OUT(&PORTD_Init);
//    PORT_Init(MDR_PORTD, &PORTD_Init);
}


int main (void)
{
    __disable_irq();
    delay(0x3FFFF);
    RCC_init();
    GPIO_init();
    __enable_irq();


    auto mil = std::shared_ptr<Milandr>(new Milandr());
    auto out = std::shared_ptr<OutputSig>(new OutputSig(mil));

    std::vector<uint8_t> receiveData(7);

    PORT_SetBits(MDR_PORTB, PORT_Pin_9); //54 pin  Дворник выключен
    PORT_ResetBits(MDR_PORTB, PORT_Pin_7); //Разрешение приема через МСХ
    while (1)
    {
        PORT_SetBits(MDR_PORTB, PORT_Pin_8); //53 pin
        delay(0x3FFFF);
        PORT_ResetBits(MDR_PORTB, PORT_Pin_8);
        delay(0x3FFFF);

        for (int i = 0; i < 7; ++i)
        {
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET); //Если буфер FIFO запрещен, бит устанавливается в 1, когда буферный регистр приемника пуст

            receiveData.push_back (UART_ReceiveData(MDR_UART1));
            if((i == 0) && (receiveData[0] != 0xff))
            {
                receiveData.clear ();
                --i;
            }
        }

        if (    receiveData[0] == 0xff &&  //включение дворника
                receiveData[1] == 0x01 &&
                receiveData[2] == 0x00 &&
                receiveData[3] == 0x09 &&
                receiveData[4] == 0x00 &&
                receiveData[5] == 0x01 &&
                receiveData[6] == 0x0b
            )
        {
            PORT_ResetBits(MDR_PORTB, PORT_Pin_9);
//            out->setMoveOn(true);
        }
        if (    receiveData[0] == 0xff &&  //выключение дворника
                receiveData[1] == 0x01 &&
                receiveData[2] == 0x00 &&
                receiveData[3] == 0x0b &&
                receiveData[4] == 0x00 &&
                receiveData[5] == 0x01 &&
                receiveData[6] == 0x0d
            )
        {
            PORT_SetBits(MDR_PORTB, PORT_Pin_9);
//            out->setMoveOn(false);
        }


        receiveData.clear();

    }
}




