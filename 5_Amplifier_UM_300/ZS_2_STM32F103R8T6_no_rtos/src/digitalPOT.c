#include "main.h"
#include "I2C.h"
#include "delay.h"

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

    init_I2C1();
    for(;;)
    {

        for (ii = 0; ii < 127 ; ++ii)
        {
            vTaskDelay(pdMS_TO_TICKS(500));

            char buf[10];
            sprintf(buf, "%d", ii);
            stringTo_diagnostic_Usart1(buf);
            send_to_POT(ii);
        }


        vTaskDelay(pdMS_TO_TICKS(500));
    }


}
