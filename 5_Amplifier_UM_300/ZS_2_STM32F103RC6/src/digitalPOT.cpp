#include "main.h"
#include "I2C.h"
#include "delay.h"


void digitaPOT(void *args)
{
    (void)args;

    for(;;)
    {
//        init_I2C1();
//        for (ii = 0; ii < 127 ; ++ii)
//        {
//            char buf[10];
//            sprintf(buf, "%d", ii);
//            send_to_POT(ii);
//            stringToLcd("buf");
//            stringToLcd(buf);


//        }

//        stringToLcd("buf12345");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }


}
