#include "main.h"
#include "stdio.h"

#define toMilandr_BtnImpedance_On     "!"
#define toMilandr_BtnImpedance_Off    "@"
#define toMilandr_BtnCalibr_On        "#"
#define toMilandr_BtnCalibr_Off       "$"
#define toMilandr_BtnReset_On         "%"
#define toMilandr_BtnReset_Off        "^"
#define toMilandr_SignalPowerOn_ON    "&"
#define toMilandr_SignalPowerOn_OFF   "*"
#define toMilandr_SignalImpedanse_ON  "("
#define toMilandr_SignalImpedanse_OFF ")"
#define toMilandr_SignalTranslate_ON  "_"
#define toMilandr_SignalTranslate_OFF "+"
#define toMilandr_SignalFromOut_ON    "="
#define toMilandr_SignalFromOut_OFF   "-"
#define toMilandr_SignalMic_ON        "~"
#define toMilandr_SignalMic_OFF       ";"

static bool btnImpedance = 0;
static bool btnCalibrovka = 0;
static bool btnReset = 0;
static bool signalPowerOn = 0;
static bool signalImpedance = 0;
static bool signalTranslate = 0;
static bool input_IMP_UPR = 0;
static bool input_VOLT_UPR = 0;
static bool input_CUR_UPR1 = 0;
static bool input_CUR_UPR2 = 0;
static bool signalVnesh = 0;
static bool signalMic = 0;

void checkButtons()
{
    bool gpioImpedance;          //Кнопка "ИМПЕДАНС"
    bool gpioCalibrovka;         //Кнопка "КАЛИБРОВКА"
    bool gpioReset;              //Кнопка "СБРОС ОШИБКИ"
    bool gpioPowerOn;            //Внешний сигнал «Включение 20 - 72V»
    bool gpioSignalImpedance;    //Внешний сигнал «Импеданс 20 - 72V»
    bool gpioSignalTranslate;    //Внешний сигнал «Трансляция 20 - 72V»
    bool gpio_IMP_UPR;           //вход IMP_UPR
    bool gpio_VOLT_UPR;          //вход VOLT_UPR
    bool gpio_CUR_UPR1;          //вход CUR_UPR1
    bool gpio_CUR_UPR2;          //вход CUR_UPR2
    bool gpioVnesh;              //Внешний сигнал «Внешний сигнал 20 - 72V»
    bool gpioMic;                //Внешний сигнал «Микрофон 20 - 72V»

    bool temp1,temp2,temp3,temp4,temp5,temp6;
    bool temp7,temp8,temp9,temp10,temp11,temp12;

    gpioImpedance = gpio_get(GPIOA, GPIO0);
    gpioCalibrovka = gpio_get(GPIOA, GPIO1);
    gpioReset = gpio_get(GPIOA, GPIO2);
    gpioPowerOn = gpio_get(GPIOA, GPIO3);
    gpioSignalImpedance = gpio_get(GPIOA, GPIO4);
    gpioSignalTranslate = gpio_get(GPIOA, GPIO5);
    gpio_IMP_UPR = gpio_get(GPIOC, GPIO0);
    gpio_VOLT_UPR = gpio_get(GPIOC, GPIO1);
    gpio_CUR_UPR1 = gpio_get(GPIOC, GPIO2);
    gpio_CUR_UPR2 = gpio_get(GPIOC, GPIO3);
    gpioVnesh = gpio_get(GPIOC, GPIO13);
    gpioMic = gpio_get(GPIOD, GPIO2);

    for(;;)
    {
        temp1 = gpio_get(GPIOA, GPIO0);
        temp2 = gpio_get(GPIOA, GPIO1);
        temp3 = gpio_get(GPIOA, GPIO2);
        temp4 = gpio_get(GPIOA, GPIO3);
        temp5 = gpio_get(GPIOA, GPIO4);
        temp6 = gpio_get(GPIOA, GPIO5);
        temp7 = gpio_get(GPIOC, GPIO0);
        temp8 = gpio_get(GPIOC, GPIO1);
        temp9 = gpio_get(GPIOC, GPIO2);
        temp10 = gpio_get(GPIOC, GPIO3);
        temp11 = gpio_get(GPIOC, GPIO13);
        temp12 = gpio_get(GPIOD, GPIO2);

    //Кнопка Импеданс
        if (temp1 != gpioImpedance)
        {
            if (temp1)
            {
                if (!btnImpedance)
                {
                    btnImpedance = 1;
                    stringToUart(toMilandr_BtnImpedance_On);
                    stringToLcd("Button Impedance ON");
                }
            }

            if (!temp1)
            {
                if (btnImpedance)
                {
                    btnImpedance = 0;
                    stringToUart(toMilandr_BtnImpedance_Off);
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
                    stringToUart(toMilandr_BtnCalibr_On);
                    stringToLcd("Button Calibrov ON");
                }
            }

            if (!temp2)
            {
                if (btnCalibrovka)
                {
                    btnCalibrovka = 0;
                    stringToUart(toMilandr_BtnCalibr_Off);
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

    //Внешний сигнал «Включение 20 - 72V»
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

    //Внешний сигнал «Импеданс 20 - 72V»
        if (temp5 != gpioSignalImpedance)
        {
            if (temp5)
            {
                if (!signalImpedance)
                {
                    signalImpedance = 1;
                    stringToUart("\r\nsignal Impedance ON\n\r");
                    stringToLcd("signal Impedance ON");
                }
            }

            if (!temp5)
            {
                if (signalImpedance)
                {
                    signalImpedance = 0;
                    stringToUart("\r\nsignal Impedance OFF\n\r");
                    stringToLcd("signal Impedance OFF");
                }
            }
            gpioSignalImpedance = temp5;
        }

    //Внешний сигнал «Трансляция 20 - 72V»
        if (temp6 != gpioSignalTranslate)
        {
            if (temp6)
            {
                if (!signalTranslate)
                {
                    signalTranslate = 1;
                    stringToUart("\r\nsignal Translate ON\n\r");
                    stringToLcd("signal Translate ON");
                }
            }

            if (!temp6)
            {
                if (signalTranslate)
                {
                    signalTranslate = 0;
                    stringToUart("\r\nsignal Translate OFF\n\r");
                    stringToLcd("signal Translate OFF");
                }
            }
            gpioSignalTranslate = temp6;
        }

    //вход IMP_UPR
        if (temp7 != gpio_IMP_UPR)
        {
            if (temp7)
            {
                if (!input_IMP_UPR)
                {
                    input_IMP_UPR = 1;
                    stringToUart("\r\ninput_IMP_UPR ON\n\r");
                    stringToLcd("input_IMP_UPR ON");
                }
            }

            if (!temp7)
            {
                if (input_IMP_UPR)
                {
                    input_IMP_UPR = 0;
                    stringToUart("\r\ninput_IMP_UPR OFF\n\r");
                    stringToLcd("input_IMP_UPR OFF");
                }
            }
            gpio_IMP_UPR = temp7;
        }

    //вход VOLT_UPR
        if (temp8 != gpio_VOLT_UPR)
        {
            if (temp8)
            {
                if (!input_VOLT_UPR)
                {
                    input_VOLT_UPR = 1;
                    stringToUart("\r\ninput_VOLT_UPR ON\n\r");
                    stringToLcd("input_VOLT_UPR ON");
                }
            }

            if (!temp8)
            {
                if (input_VOLT_UPR)
                {
                    input_VOLT_UPR = 0;
                    stringToUart("\r\ninput_VOLT_UPR OFF\n\r");
                    stringToLcd("input_VOLT_UPR OFF");
                }
            }
            gpio_VOLT_UPR = temp8;
        }

    //вход CUR_UPR1
        if (temp9 != gpio_CUR_UPR1)
        {
            if (temp9)
            {
                if (!input_CUR_UPR1)
                {
                    input_CUR_UPR1 = 1;
                    stringToUart("\r\ninput_CUR_UPR1 ON\n\r");
                    stringToLcd("input_CUR_UPR1 ON");
                }
            }

            if (!temp9)
            {
                if (input_CUR_UPR1)
                {
                    input_CUR_UPR1 = 0;
                    stringToUart("\r\ninput_CUR_UPR1 OFF\n\r");
                    stringToLcd("input_CUR_UPR1 OFF");
                }
            }
            gpio_CUR_UPR1 = temp9;
        }

    //вход CUR_UPR2
        if (temp10 != gpio_CUR_UPR2)
        {
            if (temp10)
            {
                if (!input_CUR_UPR2)
                {
                    input_CUR_UPR2 = 1;
                    stringToUart("\r\ninput_CUR_UPR2 ON\n\r");
                    stringToLcd("input_CUR_UPR2 ON");
                }
            }

            if (!temp10)
            {
                if (input_CUR_UPR2)
                {
                    input_CUR_UPR2 = 0;
                    stringToUart("\r\ninput_CUR_UPR2 OFF\n\r");
                    stringToLcd("input_CUR_UPR2 OFF");
                }
            }
            gpio_CUR_UPR2 = temp10;
        }

    //Внешний сигнал «Внешний сигнал 20 - 72V»
        if (temp11 != gpioVnesh)
        {
            if (temp11)
            {
                if (!signalVnesh)
                {
                    signalVnesh = 1;
                    stringToUart("\r\nsignalVne#sh ON\n\r");
                    stringToLcd("signalVnesh ON");
                }
            }

            if (!temp11)
            {
                if (signalVnesh)
                {
                    signalVnesh = 0;
                    stringToUart("\r\nsignalVne@sh OFF\n\r");
                    stringToLcd("signalVnesh OFF");
                }
            }
            gpioVnesh = temp11;
        }

    //Внешний сигнал «Микрофон 20 - 72V»
        if (temp12 != gpioMic)
        {
            if (temp12)
            {
                if (!signalMic)
                {
                    signalMic = 1;
                    stringToUart("\r\nsignalMic ON\n\r");
                    stringToLcd("signalMic ON");
                }
            }

            if (!temp12)
            {
                if (signalMic)
                {
                    signalMic = 0;
                    stringToUart("\r\nsignalMic OFF\n\r");
                    stringToLcd("signalMic OFF");
                }
            }
            gpioMic = temp12;
        }


        vTaskDelay(pdMS_TO_TICKS(100));
    }


}




void setStateRele()
{
    for(;;)
    {
        if (btnImpedance)
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
