#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"


PORT_InitTypeDef PortInitUART1; // определение переменной для инициализации портов ввода вывода
UART_InitTypeDef UART_InitStructure; // определение переменной для инициализации UART

uint32_t uart1_IT_TX_flag = RESET; // Флаг устанавливается после передачи одного байта
uint32_t uart1_IT_RX_flag = RESET; // Флаг устанавливается после приема одного байта

void UART1_IRQHandler(void)
{
    if (UART_GetITStatusMasked(MDR_UART1, UART_IT_RX) == SET) //проверка установки флага прерывания по окончании приема данных
    {
        UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);//очистка флага прерывания
        uart1_IT_RX_flag = SET; //установка флага передача данных завершена
    }
    if (UART_GetITStatusMasked(MDR_UART1, UART_IT_TX) == SET) //проверка установки флага прерывания по окончании передачи данных
    {
        UART_ClearITPendingBit(MDR_UART1, UART_IT_TX); //очистка флага прерывания
        uart1_IT_TX_flag = SET; //установка флага передача данных завершена
    }
}

int main (void)
{
    static uint8_t ReciveByte=0x00; //данные для приема
    // Включение HSE осциллятора (внешнего кварцевого резонатора) для обеспечения стабильной частоты UART
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    if (RST_CLK_HSEstatus() == SUCCESS)
    {
        // Если HSE осциллятор включился и прошел тест - выбор HSE осциллятора в качестве источника тактовых
        // импульсов для CPU_PLL  и установка умножителя тактовой частоты CPU_PLL равного 8 = 7+1
        RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, 7);
//        RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10);
//        RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSIdiv2,0);

        // Включение схемы PLL
        RST_CLK_CPU_PLLcmd(ENABLE);

        if (RST_CLK_HSEstatus() == SUCCESS)
        {   //Если включение CPU_PLL прошло успешно
            RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV2); // Установка CPU_C3_prescaler = 2
            RST_CLK_CPU_PLLuse(ENABLE);
            // Установка CPU_C2_SEL от CPU_PLL выхода вместо CPU_C1 такта
            // Выбор CPU_C3 такта на мультиплексоре тактовых импульсов микропроцессора (CPU clock MUX)
            RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
        }
        else while(1);// блок CPU_PLL не включился
    }
    else while(1); // кварцевый резонатор HSE не включился

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE); //Разрешение тактирования порта B
    // заполнение полей переменной PortInit для обеспечения работы UART
    PortInitUART1.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PortInitUART1.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PortInitUART1.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PortInitUART1.PORT_PD = PORT_PD_DRIVER;
    PortInitUART1.PORT_GFEN = PORT_GFEN_OFF;
    PortInitUART1.PORT_FUNC = PORT_FUNC_ALTER;
    PortInitUART1.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInitUART1.PORT_MODE = PORT_MODE_DIGITAL;
    /* Configure PORTB pins 5 (UART1_TX) as output  */
    PortInitUART1.PORT_OE = PORT_OE_OUT;
    PortInitUART1.PORT_Pin = PORT_Pin_5;
    PORT_Init(MDR_PORTB, &PortInitUART1);
    /* Configure PORTB pins 6 (UART1_RX) as input  */
    PortInitUART1.PORT_OE = PORT_OE_IN;
    PortInitUART1.PORT_Pin = PORT_Pin_6;
    PORT_Init(MDR_PORTB, &PortInitUART1);

    //Разрешение тактирования UART1
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
    // Инициализация делителя тактовой частоты для UART1
    UART_BRGInit(MDR_UART1, UART_HCLKdiv1);
    // Разрешение прерывания для UART1
    NVIC_EnableIRQ(UART1_IRQn);

    // Заполнение полей для переменной UART_InitStructure
    UART_InitStructure.UART_BaudRate                = 115000;
    UART_InitStructure.UART_WordLength              = UART_WordLength8b;
    UART_InitStructure.UART_StopBits                = UART_StopBits1;
    UART_InitStructure.UART_Parity                  = UART_Parity_No;
//    UART_InitStructure.UART_FIFOMode                = UART_FIFO_ON;
    UART_InitStructure.UART_FIFOMode                = UART_FIFO_OFF; // выключение FIFO буфера
    UART_InitStructure.UART_HardwareFlowControl     = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

    UART_Init (MDR_UART1, &UART_InitStructure); //Инициализация UART1
    UART_ITConfig (MDR_UART1, UART_IT_RX, ENABLE);//Разрешение прерывания по приему
    UART_ITConfig (MDR_UART1, UART_IT_TX, ENABLE);//Разрешение прерывания по окончани передачи

    UART_Cmd(MDR_UART1, ENABLE); //Разрешение работы UART1

  uint8_t tmp_data;

  while (1)
  {
      while (uart1_IT_RX_flag != SET); //ждем пока не не установиться флаг по приему байта
      uart1_IT_RX_flag = RESET; //очищаем флаг приема
      ReciveByte = UART_ReceiveData (MDR_UART1); //считываем принятый байт
      UART_SendData (MDR_UART1, ReciveByte); //отправляем принятый байт обратно
      while (uart1_IT_TX_flag != SET); //ждем пока байт уйдет
      uart1_IT_TX_flag = RESET; //очищаем флаг передачи

//      while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);
//      tmp_data = UART_ReceiveData(MDR_UART1);

//      UART_SendData(MDR_UART1, tmp_data);
//      while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
 

  }
}

