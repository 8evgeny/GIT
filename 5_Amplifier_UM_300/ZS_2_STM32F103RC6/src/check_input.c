#include "main.h"
#include "stdio.h"

static uint16_t btnImpedance = 0;
static uint16_t btnCalibrovka = 0;
static uint16_t btnReset = 0;
static uint16_t signalPowerOn = 0;

static uint16_t signalVnesh = 0;

void checkButtons()
{
    bool gpioImpedance;
    bool gpioCalibrovka;
    bool gpioReset;
    bool gpioPowerOn;
    bool gpioVnesh;
    bool temp1,temp2,temp3,temp4,temp5;

    gpioImpedance = gpio_get(GPIOA, GPIO0);
    gpioCalibrovka = gpio_get(GPIOA, GPIO1);
    gpioReset = gpio_get(GPIOA, GPIO2);
    gpioPowerOn = gpio_get(GPIOA, GPIO3);
    gpioVnesh = gpio_get(GPIOC, GPIO13);

    for(;;)
    {
        temp1 = gpio_get(GPIOA, GPIO0);
        temp2 = gpio_get(GPIOA, GPIO1);
        temp3 = gpio_get(GPIOA, GPIO2);
        temp4 = gpio_get(GPIOA, GPIO3);
        temp5 = gpio_get(GPIOC, GPIO13);

    //Кнопка Импеданс
        if (temp1 != gpioImpedance)
        {
            if (temp1)
            {
                if (!btnImpedance)
                {
                    btnImpedance = 1;
                    stringToUart("\r\nButton Impedance ON\n\r");
                    stringToLcd("Button Impedance ON");
                }
            }

            if (!temp1)
            {
                if (btnImpedance)
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
            if (temp2)
            {
                if (!btnCalibrovka)
                {
                    btnCalibrovka = 1;
                    stringToUart("\r\nButton Calibrov ON\n\r");
                    stringToLcd("Button Calibrov ON");
                }
            }

            if (!temp2)
            {
                if (btnCalibrovka)
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
            if (temp3)
            {
                if (!btnReset)
                {
                    btnReset = 1;
                    stringToUart("\r\nButton Reset ON\n\r");
                    stringToLcd("Button Reset ON");
                }
            }

            if (!temp3)
            {
                if (btnReset)
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
            if (temp4)
            {
                if (!signalPowerOn)
                {
                    signalPowerOn = 1;
                    stringToUart("\r\nsignalPowerOn ON\n\r");
                    stringToLcd("signalPowerOn ON");
                }
            }

            if (!temp4)
            {
                if (signalPowerOn)
                {
                    signalPowerOn = 0;
                    stringToUart("\r\nsignalPowerOn OFF\n\r");
                    stringToLcd("signalPowerOn OFF");
                }
            }
            gpioPowerOn = temp4;
        }

    //Сигнал Внешний
        if (temp5 != gpioVnesh)
        {
            if (temp5)
            {
                if (!signalVnesh)
                {
                    signalVnesh = 1;
                    stringToUart("\r\nsignalVnesh ON\n\r");
                    stringToLcd("signalVnesh ON");
                }
            }

            if (!temp5)
            {
                if (signalVnesh)
                {
                    signalVnesh = 0;
                    stringToUart("\r\nsignalVnesh OFF\n\r");
                    stringToLcd("signalVnesh OFF");
                }
            }
            gpioVnesh = temp5;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
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
