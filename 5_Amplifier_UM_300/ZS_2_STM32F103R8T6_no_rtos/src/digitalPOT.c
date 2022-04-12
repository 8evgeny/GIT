#include "main.h"
#ifdef I2C_vers2
#include "I2C.h"
#include "delay.h"
#endif

#if 0
The following is the required sequence in master mode.
- Program the peripheral input clock in I2C_CR2 register in order to generate correct timings
- Configure the clock control registers
- Configure the rise time register
- Program the I2C_CR1 register to enable the peripheral
- Set the START bit in the I2C_CR1 register to generate a Start condition
#endif

void digitaPOT(void *args)
{
    (void)args;

#ifdef useI2C_vers2
#ifndef useProgI2C1
    init_I2C1();
#endif
#endif

#ifdef useProgI2C1
    i2c_init();
#endif

    for(;;)
    {

        for (ii = 0; ii < 127 ; ++ii)
        {
            vTaskDelay(pdMS_TO_TICKS(1500));

            char buf[10];
            sprintf(buf, "%d", ii);
            stringTo_diagnostic_Usart1(buf);
#ifndef useProgI2C1
#ifdef useI2C_vers2
            send_to_POT(ii);
#endif
#endif

#ifdef useProgI2C1
            send_Programm_to_POT(ii);//DR WRITE OPERATION
//            send_Programm_to_POT1(ii);//WCR WRITE OPERATION
#endif
        }


        vTaskDelay(pdMS_TO_TICKS(500));
    }


}
