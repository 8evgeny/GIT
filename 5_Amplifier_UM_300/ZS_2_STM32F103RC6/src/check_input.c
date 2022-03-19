#include "main.h"
#include "stdio.h"


void checkButtons()
{
    uint16_t gpioImpedance, temp;
    gpioImpedance = gpio_get(GPIOA, GPIO0);
    //    char buf[80];
    for(;;)
    {
        temp = gpio_get(GPIOA, GPIO0);

        if (temp != gpioImpedance)
        {

            if (temp == 1)
            {
                stringToUart("\r\nbtnImpedance ");
                stringToUart("ON\n\r");
                gpio_set(GPIOC,GPIO6);
            }

            if (temp == 0)
            {
                stringToUart("\r\nbtnImpedance ");
                stringToUart("OFF\n\r");
                gpio_clear(GPIOC,GPIO6);
            }

            // sprintf подвешивает !!
            //            sprintf(buf, "%d", gpioImpedance);
            //            write_uart(3, buf, sizeof buf);
            //            stringToUart("\r\n\r");

            gpioImpedance = temp;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
