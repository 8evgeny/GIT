#include "main.h"
//#define VARIANT_INT
#define VARIANT_NOINT

#ifdef VARIANT_INT
//Попытка через прерывания
PORT_InitTypeDef PortInit;           // определение переменной для инициализации портов ввода вывода
UART_InitTypeDef UART_InitStructure; // определение переменной для инициализации UART
uint32_t uart1_IT_TX_flag = RESET;   // Флаг устанавливается после передачи одного байта
uint32_t uart1_IT_RX_flag = RESET;   // Флаг устанавливается после приема одного байта

void UART1_IRQHandler(void)
{
    if (UART_GetITStatusMasked(MDR_UART1, UART_IT_RX) == SET) //проверка установки флага прерывания по окончании приема данных
    {
        UART_ClearITPendingBit(MDR_UART1, UART_IT_RX);        //очистка флага прерывания
        uart1_IT_RX_flag = SET;                               //установка флага передача данных завершена
    }
    if (UART_GetITStatusMasked(MDR_UART1, UART_IT_TX) == SET) //проверка установки флага прерывания по окончании передачи данных
    {
        UART_ClearITPendingBit(MDR_UART1, UART_IT_TX);        //очистка флага прерывания
        uart1_IT_TX_flag = SET;                               //установка флага передача данных завершена
    }
}
#endif

int main (void)
{
#ifdef VARIANT_NOINT
    initGPIO();
    initUART();
#endif

    static uint8_t ReciveByte=0x00; //данные для приема
#ifdef VARIANT_INT
//Попытка через прерывания
    // Включение HSE осциллятора (внешнего кварцевого резонатора) для обеспечения стабильной частоты UART
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    if (RST_CLK_HSEstatus() == SUCCESS)
    {
        // Если HSE осциллятор включился и прошел текст
        // Выбор HSE осциллятора в качестве источника тактовых импульсов для CPU_PLL
        // и установка умножителя тактовой частоты CPU_PLL равного 8 = 7+1
        RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, 7);
        // Включение схемы PLL
        RST_CLK_CPU_PLLcmd(ENABLE);
        if (RST_CLK_HSEstatus() == SUCCESS){ //Если включение CPU_PLL прошло успешно
            RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV2); // Установка CPU_C3_prescaler = 2
            RST_CLK_CPU_PLLuse(ENABLE);
            // Установка CPU_C2_SEL от CPU_PLL выхода вместо CPU_C1 такта
            /* Выбор CPU_C3 такта на мультиплексоре тактовых импульсов микропроцессора (CPU clock MUX) */
            RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
        }
        else while(1);// блок CPU_PLL не включился
    }
    else while(1); // кварцевый резонатор HSE не включился
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE); //Разрешение тактирования порта B
    // заполнение полей переменной PortInit для обеспечения работы UART
    PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PortInit.PORT_PD = PORT_PD_DRIVER;
    PortInit.PORT_GFEN = PORT_GFEN_OFF;
    PortInit.PORT_FUNC = PORT_FUNC_ALTER;
    PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;

    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = PORT_Pin_5;
    PORT_Init(MDR_PORTB, &PortInit);

    PortInit.PORT_OE = PORT_OE_IN;
    PortInit.PORT_Pin = PORT_Pin_6;
    PORT_Init(MDR_PORTB, &PortInit);

    //Разрешение тактирования UART1
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
    // Инициализация делителя тактовой частоты для UART1
    UART_BRGInit(MDR_UART1, UART_HCLKdiv1);
    // Разрешение прерывания для UART1
    NVIC_EnableIRQ(UART1_IRQn);
    // Заполнение полей для переменной UART_InitStructure
    UART_InitStructure.UART_BaudRate = 19200; //тактовая частота передачи данных
    UART_InitStructure.UART_WordLength = UART_WordLength8b; //длина символов 8 бит
    UART_InitStructure.UART_StopBits = UART_StopBits1; //1 стоп бит
    UART_InitStructure.UART_Parity = UART_Parity_No; // нет контроля четности
    UART_InitStructure.UART_FIFOMode = UART_FIFO_ON; // выключение FIFO буфера
    /* Аппаратный контроль за передачей и приемом */
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;
    UART_Init (MDR_UART1, &UART_InitStructure); //Инициализация UART1

    UART_ITConfig (MDR_UART1, UART_IT_RX, ENABLE);//Разрешение прерывания по приему
    UART_ITConfig (MDR_UART1, UART_IT_TX, ENABLE);//Разрешение прерывания по окончани передачи
    UART_Cmd(MDR_UART1, ENABLE); //Разрешение работы UART1
#endif

    while (1)
    {
#ifdef VARIANT_NOINT

        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);  //ждем пока не не установиться флаг по приему байта
//        if (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET)
//        {

//        }
        ReciveByte = UART_ReceiveData(MDR_UART1);               //считываем принятый байт
        UART_SendData(MDR_UART1, checkReceivedByte(ReciveByte));
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);

        PORT_SetBits(MDR_PORTC, PORT_Pin_1);
        delay(0x6FFFF);
        PORT_ResetBits(MDR_PORTC, PORT_Pin_1);

#endif

#ifdef VARIANT_INT
//Попытка через прерывания
        while (uart1_IT_RX_flag != SET);           //ждем пока не не установиться флаг по приему байта
        uart1_IT_RX_flag = RESET;                  //очищаем флаг приема
        ReciveByte = UART_ReceiveData (MDR_UART1); //считываем принятый байт
        UART_SendData (MDR_UART1, checkReceivedByte(ReciveByte));     //отправляем принятый байт обратно
        while (uart1_IT_TX_flag != SET);           //ждем пока байт уйдет
        uart1_IT_TX_flag = RESET;                  //очищаем флаг передачи
#endif
    }
}

//Попытка через RTOS
//#include "MDR32Fx.h"
//#include "MDR32F9Qx_config.h"
//#include "MDR32F9Qx_can.h"
//#include "MDR32F9Qx_rst_clk.h"
//#include "MDR32F9Qx_port.h"
//#include "MDR32F9Qx_it.h"
//#include "init.h" /**<  */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "semphr.h"

//void vLed1(void *argument)
//{
//    while(1)
//    {
//        PORT_SetBits(MDR_PORTB, PORT_Pin_0);
//        vTaskDelay(5000);
//        PORT_ResetBits(MDR_PORTB, PORT_Pin_0);
//        vTaskDelay(5000);
//    }
//}

//void vLed2(void *argument)
//{
//    while(1)
//    {
//        PORT_SetBits(MDR_PORTB, PORT_Pin_1);
//        vTaskDelay(500);
//        PORT_ResetBits(MDR_PORTB, PORT_Pin_1);
//        vTaskDelay(500);
//    }
//}

//void vLed3(void *argument)
//{
//    while(1)
//    {
//        PORT_SetBits(MDR_PORTB, PORT_Pin_2);
//        vTaskDelay(1000);
//        PORT_ResetBits(MDR_PORTB, PORT_Pin_2);
//        vTaskDelay(1000);
//    }
//}

//void vLed4(void *argument)
//{
//    while(1)
//    {
//        PORT_SetBits(MDR_PORTB, PORT_Pin_3);
//        vTaskDelay(2000);
//        PORT_ResetBits(MDR_PORTB, PORT_Pin_3);
//        vTaskDelay(2000);
//    }
//}

//void vApplicationIdleHook (void)
//{
//}

//int main(void)
//{
//    init ();
//    xTaskCreate(vLed1, "1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(vLed2, "2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(vLed3, "3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
//    xTaskCreate(vLed4, "4", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

//    // Запускаем диспетчер и понеслась.
//    vTaskStartScheduler();
//}



