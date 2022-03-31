#include "main.h"

void setOutputs(void *args)
{
    (void)args;

    for(;;)
    {
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
