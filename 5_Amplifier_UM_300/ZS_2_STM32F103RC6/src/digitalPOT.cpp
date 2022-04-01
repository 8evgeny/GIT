#include "main.h"
#include "I2C.h"
#include "delay.h"


void digitaPOT(void *args)
{
    (void)args;

    init_I2C1();
    for(;;)
    {

//        for (ii = 0; ii < 127 ; ++ii)
//        {
//            char buf[10];
//            sprintf(buf, "%d", ii);
//            send_to_POT(ii);
////            stringToLcd("buf");
////            stringToLcd(buf);

//stringTo_diagnostic_Usart1("test\n\r");

//        }

//stringTo_diagnostic_Usart1("test\n\r");
        vTaskDelay(pdMS_TO_TICKS(500));
    }


}
