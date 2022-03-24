#include "main.h"


int main (void)
{
    initGPIO();
    initUART();

    while (1)
    {
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);//ждем пока не не установиться флаг по приему байта
        uint8_t ReciveByte = UART_ReceiveData(MDR_UART1);                     //считываем принятый байт
//        delay(0xFF);
//        PORT_ResetBits(MDR_PORTC, PORT_Pin_1);
        UART_SendData(MDR_UART1, checkReceivedByte(ReciveByte));
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
 
    }//end while(1)

}


