#include "main.h"
#include <chrono>

int main (void)
{
    initGPIO();
    initUART();

    typedef std::chrono::system_clock Time;
    typedef std::chrono::microseconds mcs;
    typedef std::chrono::duration<float> fsec;
    auto t0 = Time::now();

    while (1)
    {
        auto t1 = Time::now();
        fsec fs = t1 - t0;
        mcs d = std::chrono::duration_cast<mcs>(fs);

        if (d.count() >1000)
        {
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_RXFE) == SET);  //ждем пока не не установиться флаг по приему байта
            uint8_t ReciveByte = UART_ReceiveData(MDR_UART1);               //считываем принятый байт
    //        delay(0xFF);
    //        PORT_ResetBits(MDR_PORTC, PORT_Pin_1);
            UART_SendData(MDR_UART1, checkReceivedByte(ReciveByte));
            while (UART_GetFlagStatus (MDR_UART1, UART_FLAG_TXFE) != SET);
            t0 = t1;
        }


 


    }//end while(1)

}


