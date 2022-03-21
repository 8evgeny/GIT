#include "main.h"
#include "stdio.h"

uint16_t releImpedance = 0;


void checkButtons()
{
    uint16_t gpioImpedance, temp;
    gpioImpedance = gpio_get(GPIOA, GPIO0);
    for(;;)
    {
        temp = gpio_get(GPIOA, GPIO0);

        if (temp != gpioImpedance)
        {

            if (temp == 1)
            {
                if (releImpedance != 1)
                {
                    releImpedance = 1;
                    stringToUart("\r\nRele Impedance ON\n\r ");
                    stringToLcd("Rele Impedance ON");
                }

                stringToUart("\r\nButton Impedance ON\n\r");
                stringToLcd("Button Impedance ON");

            }

            if (temp == 0)
            {
                stringToUart("\r\nButton Impedance OFF\n\r");
                stringToLcd("Button Impedance OFF");

            }

            gpioImpedance = temp;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void setStateRele()
{
    for(;;)
    {
        if (releImpedance == 1)
        {
            setImpedanceRele(true);
//setTestLed4(true);

        }
        else
        {
            setImpedanceRele(false);
//setTestLed4(false);
        }


    }

}
