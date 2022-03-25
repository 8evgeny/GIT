#include "main.h"
#define VARIANT_INT
//#define VARIANT_NOINT

#ifdef VARIANT_INT
//Попытка через прерывания
PORT_InitTypeDef PortInit;           // определение переменной для инициализации портов ввода вывода
UART_InitTypeDef UART_InitStructure; // определение переменной для инициализации UART
uint32_t uart1_IT_TX_flag = RESET;   // Флаг устанавливается после передачи одного байта
uint32_t uart1_IT_RX_flag = RESET;   // Флаг устанавливается после приема одного байта

void UART1_IRQHandler(void)
{
    PORT_SetBits(MDR_PORTC, PORT_Pin_1);
    delay(0x6FFFF);
    PORT_ResetBits(MDR_PORTC, PORT_Pin_1);

    if (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET) //проверка установки флага прерывания по окончании приема данных
    {
        UART_ClearITPendingBit(MDR_UART1, UART_FLAG_RXFE);        //очистка флага прерывания
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

    initGPIO();
    initUART();

static uint8_t ReciveByte=0x00; //данные для приема



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
        while (uart1_IT_RX_flag != SET);                            //ждем пока не не установиться флаг по приему байта
        uart1_IT_RX_flag = RESET;                                   //очищаем флаг приема
        ReciveByte = UART_ReceiveData (MDR_UART1);                  //считываем принятый байт
        UART_SendData (MDR_UART1, checkReceivedByte(ReciveByte));   //отправляем принятый байт обратно
        while (uart1_IT_TX_flag != SET);                            //ждем пока байт уйдет
        uart1_IT_TX_flag = RESET;                                   //очищаем флаг передачи
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



