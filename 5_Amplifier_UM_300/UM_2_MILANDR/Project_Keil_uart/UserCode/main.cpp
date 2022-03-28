#include "main.h"
#include "milandr.h"
#include "SSM2166_MicPreamp.h"
#include "TPA3255_Amplifier.h"
#include <memory>

int main (void)
{
    auto m = std::shared_ptr<Milandr>(new Milandr);
    auto p = std::shared_ptr<SSM2166_MicPreamp>(new SSM2166_MicPreamp(m));
    auto a = std::shared_ptr<TPA3255_Amplifier>(new TPA3255_Amplifier(m));

    static uint8_t ReciveByte=0x00; //данные для приема
    char temp;
    char ReplayToCmdFromStm = 0x00;
    bool sendReplayToCmdFromStm = false;

    while (1)
    {
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);  //ждем пока не не установиться флаг по приему байта
        ReciveByte = UART_ReceiveData(MDR_UART1);                       //считываем принятый байт
        temp = m->checkReceivedByte(ReciveByte);

        if (temp != 0x00)// от STM поступила команда
        {
            ReplayToCmdFromStm = temp;  //Ответ на команду
            sendReplayToCmdFromStm = false;

        }

        if (temp == 0x00)// от STM поступила пустая команда
        {
        //Основная логика (При возникновении команды сбрасываем toStmCmdSend  сама команда - toStmCmd )




        }



        if (!sendReplayToCmdFromStm)//Отправка ответа на команду Stm
        {
            UART_SendData(MDR_UART1, ReplayToCmdFromStm);
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            sendReplayToCmdFromStm = true;
        }

        if (!m->getToStmCmdSend())//Отправка команды в адрес Stm
        {
            UART_SendData(MDR_UART1, m->getToStmCmd());
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            m->setToStmCmdSend(true);
        }


    }
}




