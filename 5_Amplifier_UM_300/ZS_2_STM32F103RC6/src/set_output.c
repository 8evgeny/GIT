#include "main.h"

void setOutputs()
{
    for(;;)
    {
        if (btnImpedance)
        {
            setImpedanceRele(true);
        }
        else
        {
            setImpedanceRele(false);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }

}
