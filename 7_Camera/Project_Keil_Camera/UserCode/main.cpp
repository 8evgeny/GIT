#include "main.h"
#include "milandr.h"
#include "outputSig.h"
#include <memory>
#include <vector>
PORT_InitTypeDef PORTB_Init;
int main (void)
{
    auto mil = std::shared_ptr<Milandr>(new Milandr());
    auto out = std::shared_ptr<OutputSig>(new OutputSig(mil));

    std::vector<uint8_t> receiveData(7);

    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);

    PORTB_Init.PORT_Pin = PORT_Pin_10 ;
    PORTB_Init.PORT_OE = PORT_OE_OUT;           // Конфигурация группы выводов как выход
    PORTB_Init.PORT_FUNC = PORT_FUNC_PORT;      // Работа а режиме порта ввода-вывода
    PORTB_Init.PORT_MODE = PORT_MODE_DIGITAL;   // Цифровой режим
    PORTB_Init.PORT_SPEED = PORT_SPEED_SLOW;    // Низкая частота тактования порта
    PORT_Init(MDR_PORTC, &PORTB_Init);          // Инициализация порта C объявленной структурой





    while (1)
    {

        PORT_SetBits(MDR_PORTB, PORT_Pin_10);
        delay(0xFFFF);                         // Задержка

        PORT_ResetBits(MDR_PORTB, PORT_Pin_10);
        delay(0xFFFF);




//        for (int i = 0; i <7; ++i)
//        {
//            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET) //Если буфер FIFO запрещен, бит устанавливается в 1, когда буферный регистр приемника пуст
//            {
//                //Тут сделать проверку времени ожидания
//            }
//            receiveData.push_back (UART_ReceiveData(MDR_UART1));
//            if((i == 0) && (receiveData.at(0) != 0xFF))
//        {
//            receiveData.clear ();
//            break;
//        }
//        }

//        if (receiveData.size () == 7) //Принята команда
//        {
//            PORT_ResetBits(MDR_PORTB, PORT_Pin_10);
////            out->setMoveOn(false);
//        }





    }//end while (1)
}




