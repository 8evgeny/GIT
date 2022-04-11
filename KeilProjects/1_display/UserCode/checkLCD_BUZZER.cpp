#include "main.h"
#include <chrono>

void checkLCD1()
{
    if(Buffer[0] == 49) //если 1 то горит первый светодиод
    {
        PORT_SetBits(MDR_PORTB, LED2_REC);
    }
    if(Buffer[0] == 48) //если 0 то гасим
    {
        PORT_ResetBits(MDR_PORTB, LED2_REC);
    }

}

void checkLCD2()
{
    if(Buffer[1] == 49) //если 1 то горит второй светодиод
    {
        PORT_SetBits(MDR_PORTB, LED1_ERROR);
    }
    if(Buffer[1] == 48) //если 0 то гасим
    {
        PORT_ResetBits(MDR_PORTB, LED1_ERROR);
    }
}

void checkBUZZER()
{
#if 0
Плата индикации должна обеспечивать выдачу звуковых сигналов в следующих случаях:
0. Выключен
1. Постоянный сигнал
2. Два сигнала (звучание в течение 0,5с с интервалом 0,5с).
3. Три сигнала (звучание в течение 0,5с с интервалом 0,5с).
4. Четыре сигнала (звучание в течение 0,5с с интервалом 0,5с).
5. Один сигнал (звучание в течение 0,5с с интервалом 60с).
6. Непрерывная последовательность (звучание в течение 0,5с с интервалом 0,5с).
#endif
    if (Buffer[2] == 49) //Постоянный сигнал
    {
        PORT_SetBits(MDR_PORTB, BUZZER);
    }
    if(Buffer[2] == 48) //если 0 то гасим
    {
        PORT_ResetBits(MDR_PORTB, BUZZER);
    }

}

void checkCMD()
{
    if((Buffer[0] == 55) && (Buffer[1] == 55) && (Buffer[2] == 55))
    {
        firstScreen();
    }

}
