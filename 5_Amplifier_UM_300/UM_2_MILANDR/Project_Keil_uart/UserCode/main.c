#include "main.h"


int main (void)
{

    initGPIO();
    initUART();

//Временно для теста - все разрешаю
PORT_SetBits(MDR_PORTB, PORT_Pin_8); //RESET_UPR
PORT_SetBits(MDR_PORTA, PORT_Pin_2); //GAIN_UPR
PORT_SetBits(MDR_PORTA, PORT_Pin_3); //COMP_UPR

    static uint8_t ReciveByte=0x00; //данные для приема
    char temp;
    char ReplayToCmdFromStm = 0x00;
    bool sendReplayToCmdFromStm = false;





    while (1)
    {

        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);  //ждем пока не не установиться флаг по приему байта
        ReciveByte = UART_ReceiveData(MDR_UART1);                       //считываем принятый байт
        temp = checkReceivedByte(ReciveByte);

        if (temp != 0x00)// от STM поступила команда
        {
            ReplayToCmdFromStm = temp;  //Ответ на команду
            sendReplayToCmdFromStm = false;
        }

        else //Основная логика (При возникновении команды сбрасываем toStmCmdSend  сама команда - toStmCmd )
        {
            checkPinStates();
            checkGlobalVarables();

        }




        if (!sendReplayToCmdFromStm)//Отправка ответа на команду Stm
        {
            UART_SendData(MDR_UART1, ReplayToCmdFromStm);
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            sendReplayToCmdFromStm = true;
        }

        if (!toStmCmdSend)//Отправка команды в адрес Stm
        {
            UART_SendData(MDR_UART1, toStmCmd);
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            toStmCmdSend = true;
        }


    }
}




