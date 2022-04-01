#include "main.h"

void setOutputs(void *args)
{
    (void)args;

    for(;;)
    {
//stringTo_diagnostic_Usart1("test\n\r");

        if (ERROR_MC  || OVERHEAT_MC )
        {
            setErrorRele(true);
        }
        else
        {
            setErrorRele(false);
        }

//Нужно реализовать выходные сигналы






        vTaskDelay(pdMS_TO_TICKS(100));
    }

}
