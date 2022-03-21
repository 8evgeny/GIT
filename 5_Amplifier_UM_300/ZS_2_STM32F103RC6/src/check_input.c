#include "main.h"
#include "stdio.h"

static uint16_t btnImpedance = 0;
static uint16_t btnCalibrovka = 0;
static uint16_t btnReset = 0;
static uint16_t signalPowerOn = 0;

void checkButtons()
{
    int gpioImpedance, temp1;
    int gpioCalibrovka, temp2;
    int gpioReset, temp3;
    int gpioPowerOn, temp4;

    gpioImpedance = gpio_get(GPIOA, GPIO0);
    gpioCalibrovka = gpio_get(GPIOA, GPIO1);
    gpioReset = gpio_get(GPIOA, GPIO2);
    gpioPowerOn = gpio_get(GPIOA, GPIO3);
    for(;;)
    {
        temp1 = gpio_get(GPIOA, GPIO0);
        temp2 = gpio_get(GPIOA, GPIO1);
        temp3 = gpio_get(GPIOA, GPIO2);
        temp4 = gpio_get(GPIOA, GPIO3);

    //Кнопка Импеданс
        if (temp1 != gpioImpedance)
        {
            if (temp1 == 1)
            {
                if (btnImpedance != 1)
                {
                    btnImpedance = 1;
                    stringToUart("\r\nButton Impedance ON\n\r");
                    stringToLcd("Button Impedance ON");
                }
            }

            if (temp1 == 0)
            {
                if (btnImpedance == 1)
                {
                    btnImpedance = 0;
                    stringToUart("\r\nButton Impedance OFF\n\r");
                    stringToLcd("Button Impedance OFF");
                }
            }
            gpioImpedance = temp1;
        }

    //Кнопка Калибровка
        if (temp2 != gpioCalibrovka)
        {
            if (temp2 == 1)
            {
                if (btnCalibrovka != 1)
                {
                    btnCalibrovka = 1;
                    stringToUart("\r\nButton Calibrov ON\n\r");
                    stringToLcd("Button Calibrov ON");
                }
            }

            if (temp2 == 0)
            {
                if (btnCalibrovka == 1)
                {
                    btnCalibrovka = 0;
                    stringToUart("\r\nButton Calibrov OFF\n\r");
                    stringToLcd("Button Calibrov OFF");
                }
            }
            gpioCalibrovka = temp2;
        }

    //Кнопка Сброс ошибки
        if (temp3 != gpioReset)
        {
            if (temp3 == 1)
            {
                if (btnReset != 1)
                {
                    btnReset = 1;
                    stringToUart("\r\nButton Reset ON\n\r");
                    stringToLcd("Button Reset ON");
                }
            }

            if (temp3 == 0)
            {
                if (btnReset == 1)
                {
                    btnReset = 0;
                    stringToUart("\r\nButton Reset OFF\n\r");
                    stringToLcd("Button Reset OFF");
                }
            }
            gpioReset = temp3;
        }

    //Сигнал PowerON
        if (temp4 != gpioPowerOn)
        {
            if (temp4 == 1)
            {
                if (signalPowerOn != 1)
                {
                    signalPowerOn = 1;
                    stringToUart("\r\nsignalPowerOn ON\n\r");
                    stringToLcd("signalPowerOn ON");
                }
            }

            if (temp4 == 0)
            {
                if (signalPowerOn == 1)
                {
                    signalPowerOn = 0;
                    stringToUart("\r\nsignalPowerOn OFF\n\r");
                    stringToLcd("signalPowerOn OFF");
                }
            }
            gpioPowerOn = temp4;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }


}




void setStateRele()
{
    for(;;)
    {
        if (btnImpedance == 1)
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
