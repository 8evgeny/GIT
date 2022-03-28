#include "main.h"


int main (void)
{

    initGPIO();
    initUART();

    PORT_SetBits(MDR_PORTB, PORT_Pin_8); //RESET_UPR
    PORT_SetBits(MDR_PORTA, PORT_Pin_2); //GAIN_UPR
    PORT_SetBits(MDR_PORTA, PORT_Pin_3); //COMP_UPR

static uint8_t ReciveByte=0x00; //данные для приема


    while (1)
    {

        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);  //ждем пока не не установиться флаг по приему байта
        ReciveByte = UART_ReceiveData(MDR_UART1);                       //считываем принятый байт

        //Тестовые команды
        if( PORT_ReadInputDataBit(MDR_PORTE, PORT_Pin_0) == 0)
        {
             UART_SendData(MDR_UART1, cmdToStm_OVERHEAT_60_ON);
        }

        if( PORT_ReadInputDataBit(MDR_PORTE, PORT_Pin_1) == 0)
        {
            UART_SendData(MDR_UART1, cmdToStm_OVERHEAT_85_ON);
        }

        char temp = checkReceivedByte(ReciveByte);
        if (temp != 0x00)
        {
            UART_SendData(MDR_UART1, temp);
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
        }


    }
}




