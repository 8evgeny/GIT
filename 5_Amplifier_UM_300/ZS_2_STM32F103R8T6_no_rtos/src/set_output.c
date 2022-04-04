#include "main.h"

void setOutputs(void *args)
{
    (void)args;

    for(;;)
    {
//stringTo_diagnostic_Usart1("test\n\r");

        if (ERROR_MC)
        {
            setErrorRele(true);
            setLedOvercutOut(true);
        }
        else
        {
            setErrorRele(false);
            setLedOvercutOut(false);
        }

        if (OVERHEAT_MC )
        {
            setErrorRele(true);
            setLedOverheatOut(true);
        }
        else
        {
            setErrorRele(false);
            setLedOverheatOut(false);
        }

        if (BOARD_OK)
        {
            setReadyLed(true);
            setFan(true);
        }
        else
        {
            setReadyLed(false);
            setFan(false);
        }

        if (signalVnesh)
        {
            setRele24V(true);
        }
        else
        {
            setRele24V(false);
        }


        if (signalTranslate)
        {
            setReleTr1(true);
            setRele24V(true);
        }
        else
        {
            setReleTr1(false);
            setRele24V(false);
        }


//Нужно реализовать выходные сигналы






        vTaskDelay(pdMS_TO_TICKS(500));
    }

}
