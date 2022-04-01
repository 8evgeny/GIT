#include "main.h"
#include "I2C.h"
#include "delay.h"


void digitaPOT(void *args)
{
    (void)args;

    init_I2C1();
    for(;;)
    {

        for (ii = 0; ii < 127 ; ++ii)
        {
            vTaskDelay(pdMS_TO_TICKS(200));

            char buf[10];
            sprintf(buf, "%d", ii);
//            send_to_POT(ii);
            stringTo_diagnostic_Usart1(buf);

        }


        vTaskDelay(pdMS_TO_TICKS(500));
    }


}
