#include "main.h"


int main (void)
{

    initGPIO();
    initUART();

static uint8_t ReciveByte=0x00; //данные для приема


    while (1)
    {

        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);  //ждем пока не не установиться флаг по приему байта
        ReciveByte = UART_ReceiveData(MDR_UART1);                       //считываем принятый байт


//Виснет
//        PORT_SetBits(MDR_PORTC, PORT_Pin_1);
//        delay(0x1FFFF);
//        PORT_ResetBits(MDR_PORTC, PORT_Pin_1);
//        delay(0x1FFFF);

        char temp = checkReceivedByte(ReciveByte);
        if (temp != 0x00)
        {
            UART_SendData(MDR_UART1, checkReceivedByte(ReciveByte));
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
        }


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



