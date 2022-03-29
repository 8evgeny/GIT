#include "main.h"
#include "milandr.h"
#include "SSM2166_MicPreamp.h"
#include "TPA3255_Amplifier.h"
#include <memory>

int main (void)
{
    auto mil = std::shared_ptr<Milandr>(new Milandr);
    auto pre = std::shared_ptr<SSM2166_MicPreamp>(new SSM2166_MicPreamp(mil));
    auto amp = std::shared_ptr<TPA3255_Amplifier>(new TPA3255_Amplifier(mil));

    static uint8_t ReciveByte=0x00; //данные для приема
    char temp;
    char ReplayToCmdFromStm = 0x00;
    bool sendReplayToCmdFromStm = false;

    while (1)
    {
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);  //ждем пока не не установиться флаг по приему байта
        ReciveByte = UART_ReceiveData(MDR_UART1);                       //считываем принятый байт
        temp = mil->checkReceivedByte(ReciveByte);

        if (temp != 0x00)// от STM поступила команда
        {
            ReplayToCmdFromStm = temp;  //Ответ на команду
            sendReplayToCmdFromStm = false;

        }

        if (temp == 0x00)// от STM поступила пустая команда
        {
        //Основная логика (При возникновении команды сбрасываем toStmCmdSend  сама команда - toStmCmd )





        //Питание
            if(mil->isPowerOk())
            {
                mil->setBOARD_OK(true);
            }
            else
            {
                mil->setBOARD_OK(false);
            }




            pre->micPreampON();
            pre->micPreampCompressionON();
            amp->reset();

        //Выключение усилителя
            pre->micPreampOFF();
            pre->micPreampCompressionOFF();

        //Проверка перегрузки
            if(!amp->isFault())
            {
                //Перегрузки нет
                pre->micPreampON();
                mil->setERROR_MC(false);
            }
            else
            {
                //Перегрузка
                pre->micPreampOFF();
                mil->setERROR_MC(true);
                amp->reset();
            }

        //Проверка перегрева
            if(!amp->isOverHeart())
            {
                //Перегрева нет
                pre->micPreampON();
                mil->setOVERHEAT_MC(false);
            }
            else
            {
                //Перегрев
                pre->micPreampOFF();
                mil->setOVERHEAT_MC(true);
                amp->reset();
            }




        }


        //Проверка переменных и если не совпадает с temp - сигнал отправка









        if (!sendReplayToCmdFromStm)//Отправка ответа на команду Stm
        {
            UART_SendData(MDR_UART1, ReplayToCmdFromStm);
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            sendReplayToCmdFromStm = true;
        }

        if (!mil->getToStmCmdSend())//Отправка команды в адрес Stm
        {
            UART_SendData(MDR_UART1, mil->getToStmCmd());
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            mil->setToStmCmdSend(true);
        }


    }
}




