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

//setErrorRele(bool set)




        vTaskDelay(pdMS_TO_TICKS(100));
    }

}
