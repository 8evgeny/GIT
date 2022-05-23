#include "main.h"
#include "milandr.h"
#include "outputSig.h"
#include <memory>
#include <vector>

int main (void)
{
    auto mil = std::shared_ptr<Milandr>(new Milandr());
    auto out = std::shared_ptr<OutputSig>(new OutputSig(mil));

    std::vector<uint8_t> receiveData(7);

    while (1)
    {
        for (int i = 0; i <7; ++i)
        {
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET) //Если буфер FIFO запрещен, бит устанавливается в 1, когда буферный регистр приемника пуст
            {
                //Тут сделать проверку времени ожидания
            }
            receiveData.push_back (UART_ReceiveData(MDR_UART1));
            if((i == 0) && (receiveData.at(0) != 0xFF))
        {
            receiveData.clear ();
            break;
        }
        }

        if (receiveData.size () == 7) //Принята команда
        {


        }





    }//end while (1)
}




