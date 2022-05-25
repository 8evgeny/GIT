#include "main.h"
#include "milandr.h"
#include "outputSig.h"
#include <memory>
#include <vector>
PORT_InitTypeDef PORTB_Init;
int main (void)
{
    delay(0x3FFFF);

    auto mil = std::shared_ptr<Milandr>(new Milandr());
    auto out = std::shared_ptr<OutputSig>(new OutputSig(mil));

    std::vector<uint8_t> receiveData(7);

    PORT_ResetBits(MDR_PORTB, PORT_Pin_9); //54 pin
    PORT_ResetBits(MDR_PORTB, PORT_Pin_7);
    while (1)
    {
        PORT_SetBits(MDR_PORTB, PORT_Pin_8); //53 pin
        delay(0x3FFFF);
        PORT_ResetBits(MDR_PORTB, PORT_Pin_8);
        delay(0x3FFFF);

//        while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);
//        PORT_SetBits(MDR_PORTB, PORT_Pin_9); //54 pin

        for (int i = 0; i < 7; ++i)
        {
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET) //Если буфер FIFO запрещен, бит устанавливается в 1, когда буферный регистр приемника пуст
            {
                //Тут сделать проверку времени ожидания
            }


            receiveData.push_back (UART_ReceiveData(MDR_UART1));
//            if((i == 0) && (receiveData.at(0) != 0xFF))
//        {
//            receiveData.clear ();
//            break;
//        }
        }

        if (    receiveData[0] == 0xff &&  //включение дворника
                receiveData[1] == 0x01 &&
                receiveData[2] == 0x00 &&
                receiveData[3] == 0x09 &&
                receiveData[4] == 0x00 &&
                receiveData[5] == 0x01 &&
                receiveData[6] == 0x0b
            )
        {
            PORT_SetBits(MDR_PORTB, PORT_Pin_9);
//            out->setMoveOn(true);
        }
        if (    receiveData[0] == 0xff &&  //выключение дворника
                receiveData[1] == 0x01 &&
                receiveData[2] == 0x00 &&
                receiveData[3] == 0x0b &&
                receiveData[4] == 0x00 &&
                receiveData[5] == 0x01 &&
                receiveData[6] == 0x0d
            )
        {
            PORT_ResetBits(MDR_PORTB, PORT_Pin_9);
//            out->setMoveOn(false);
        }


        receiveData.clear();

    }
}




