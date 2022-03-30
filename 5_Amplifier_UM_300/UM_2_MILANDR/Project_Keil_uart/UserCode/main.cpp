#include "main.h"
#include "milandr.h"
#include "SSM2166_MicPreamp.h"
#include "TPA3255_Amplifier.h"
#include "inputSig.h"
#include "outputSig.h"
#include <memory>

int main (void)
{
    auto inp = std::shared_ptr<InputSig>(new InputSig());
    auto mil = std::shared_ptr<Milandr>(new Milandr(inp));
    auto pre = std::shared_ptr<SSM2166_MicPreamp>(new SSM2166_MicPreamp(mil));
    auto amp = std::shared_ptr<TPA3255_Amplifier>(new TPA3255_Amplifier(mil));
    auto out = std::shared_ptr<OutputSig>(new OutputSig(mil));

    static uint8_t ReciveByte=0x00; //данные для приема
    char temp;
    char ReplayToCmdFromStm = 0x00;
    bool sendReplayToCmdFromStm = false;

    while (1)
    {
        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);  //ждем пока не не установиться флаг по приему байта
        ReciveByte = UART_ReceiveData(MDR_UART1);                       //считываем принятый байт
        temp = inp->checkReceivedByte(ReciveByte);

        if (temp != 0x00)// от STM поступила команда
        {
            ReplayToCmdFromStm = temp;  //Ответ на команду
            sendReplayToCmdFromStm = false;

        }

        if (temp == 0x00)// от STM поступила пустая команда
        {
        //Основная логика
        //Последовательно обрабатываем полученные ранее входные сигналы

            if(inp->getBtnImpedance())
            {
    //Не реализовано

            }

            if(inp->getBtnCalibrovka())
            {
    //Не реализовано

            }

            if(inp->getBtnReset())
            {
    //Не реализовано

            }

            if(inp->getSignalPowerOn())
            {
                if(mil->isPowerOk())
                {
                    out->setBOARD_OK(true);
                    mil->setFAN_ON(true);
                }
                else
                {
                    out->setBOARD_OK(false);
                    mil->setFAN_ON(false);
                }
            }

            if(inp->getSignalImpedance())
            {
    //Не реализовано

            }

            if(inp->getSignalTranslate())
            {
                if (out->getBOARD_OK())
                {
                    pre->micPreampON();
//                    pre->micPreampCompressionON(); //Для микрофона
                }
                else
                {
                    pre->micPreampOFF();
//                    pre->micPreampCompressionOFF();
                }
            }

            if(inp->getSignalVnesh())
            {
    //Не реализовано

            }

            if(inp->getSignalMic())
            {
                if (out->getBOARD_OK())
                {
                    pre->micPreampON();
                    pre->micPreampCompressionON();
                }
                else
                {
                    pre->micPreampOFF();
                    pre->micPreampCompressionOFF();
                }
            }

          //Последовательно формируем выходные сигналы
            //BOARD_OK
            if(mil->isPowerOk())
            {
                out->setBOARD_OK(true);
            }
            else
            {
                out->setBOARD_OK(false);
            }

            //OVERHEAT_MC
            if(!amp->isOverHeart())
            {
                //Перегрева нет
                pre->micPreampON();
                out->setOVERHEAT_MC(false);
            }
            else
            {
                //Перегрев
                pre->micPreampOFF();
                out->setOVERHEAT_MC(true);
                amp->reset();
            }

            //ERROR_MC
            if(!amp->isFault())
            {
                //Перегрузки нет
                pre->micPreampON();
                out->setERROR_MC(false);
            }
            else
            {
                //Перегрузка
                pre->micPreampOFF();
                out->setERROR_MC(true);
                amp->reset();
            }

            //Проверка переменных и если не совпадает с temp отправка сигнала
            out->checkBOARD_OK();
            out->checkOVERHEAT_MC();
            out->checkERROR_MC();

        }//end if


        if (!sendReplayToCmdFromStm)//Отправка ответа на команду Stm
        {
            UART_SendData(MDR_UART1, ReplayToCmdFromStm);
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            sendReplayToCmdFromStm = true;
        }

        if (!out->getToStmCmdSend())//Отправка команды в адрес Stm
        {
            UART_SendData(MDR_UART1, out->getToStmCmd());
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            out->setToStmCmdSend(true);
        }

    }//end while (1)
}




