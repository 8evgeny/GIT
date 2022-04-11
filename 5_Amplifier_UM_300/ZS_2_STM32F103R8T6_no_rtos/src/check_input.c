#include "main.h"
#include "stdio.h"

void checkInputs(void *args)
{
    (void)args;

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
    bool gpio_OVERHEAT_MC;       //вход OVERHEAT_MC
    bool gpio_POWER_OK;          //вход POWER_OK
    bool gpio_ERROR_MC;          //вход ERROR_MC

    bool temp1,temp2,temp3,temp4,temp5,temp6;
    bool temp7,temp8,temp9,temp10,temp11,temp12;
    bool temp13,temp14,temp15;

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
    gpio_OVERHEAT_MC = gpio_get(GPIOA, GPIO6);
    gpio_POWER_OK = gpio_get(GPIOC, GPIO4);
    gpio_ERROR_MC = gpio_get(GPIOC, GPIO5);
    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
//        stringTo_diagnostic_Usart1("checkInputs - 200");

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
        temp13 = gpio_get(GPIOA, GPIO6);
        temp14 = gpio_get(GPIOC, GPIO4);
        temp15 = gpio_get(GPIOC, GPIO5);

    //Кнопка Импеданс
        if (temp1 != gpioImpedance)
        {
            if (temp1)
            {
                if (!btnImpedance)
                {
                    btnImpedance = 1;
#if defined useMilandr
                    stringToUart(toMilandr_BtnImpedance_On);
#endif
//                    stringToLcd("Button Impedance ON");
                    stringTo_diagnostic_Usart1("Button Impedance ON");
                }
            }

            if (!temp1)
            {
                if (btnImpedance)
                {
                    btnImpedance = 0;
#if defined useMilandr
                    stringToUart(toMilandr_BtnImpedance_Off);
#endif
//                    stringToLcd("Button Impedance OFF");
                    stringTo_diagnostic_Usart1("Button Impedance OFF");
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
#if defined useMilandr
                    stringToUart(toMilandr_BtnCalibr_On);
#endif
//                    stringToLcd("Button Calibrov ON");
                    stringTo_diagnostic_Usart1("Button Calibrov ON");

                }
            }

            if (!temp2)
            {
                if (btnCalibrovka)
                {
                    btnCalibrovka = 0;
#if defined useMilandr
                    stringToUart(toMilandr_BtnCalibr_Off);
#endif
//                    stringToLcd("Button Calibrov OFF");
                    stringTo_diagnostic_Usart1("Button Calibrov OFF");
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
#if defined useMilandr
                    stringToUart(toMilandr_BtnReset_On);
#endif
//                    stringToLcd("Button Reset ON");
                    stringTo_diagnostic_Usart1("Button Reset ON");
                }
            }

            if (!temp3)
            {
                if (btnReset)
                {
                    btnReset = 0;
#if defined useMilandr
                    stringToUart(toMilandr_BtnReset_Off);
#endif
//                    stringToLcd("Button Reset OFF");
                    stringTo_diagnostic_Usart1("Button Reset OFF");
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
#if defined useMilandr
                    stringToUart(toMilandr_SignalPowerOn_ON);
#endif
//                    stringToLcd("signalPowerOn ON");
                    stringTo_diagnostic_Usart1("signalPowerOn ON");
                }
            }

            if (!temp4)
            {
                if (signalPowerOn)
                {
                    signalPowerOn = 0;
#if defined useMilandr
                    stringToUart(toMilandr_SignalPowerOn_OFF);
#endif
//                    stringToLcd("signalPowerOn OFF");
                    stringTo_diagnostic_Usart1("signalPowerOn OFF");
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
#if defined useMilandr
                    stringToUart(toMilandr_SignalImpedanse_ON);
#endif

//                    stringToLcd("signal Impedance ON");
                    stringTo_diagnostic_Usart1("signal Impedance ON");
                }
            }

            if (!temp5)
            {
                if (signalImpedance)
                {
                    signalImpedance = 0;
#if defined useMilandr
                    stringToUart(toMilandr_SignalImpedanse_OFF);
#endif
//                    stringToLcd("signal Impedance OFF");
                    stringTo_diagnostic_Usart1("signal Impedance OFF");
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
#if defined useMilandr
                    stringToUart(toMilandr_SignalTranslate_ON);
#endif
//                    stringToLcd("signal Translate ON");
                    stringTo_diagnostic_Usart1("signal Translate ON");
                }
            }

            if (!temp6)
            {
                if (signalTranslate)
                {
                    signalTranslate = 0;
#if defined useMilandr
                    stringToUart(toMilandr_SignalTranslate_OFF);
#endif
//                    stringToLcd("signal Translate OFF");
                    stringTo_diagnostic_Usart1("signal Translate OFF");
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
//                    stringToLcd("input_IMP_UPR ON");
                    stringTo_diagnostic_Usart1("input_IMP_UPR ON");
                }
            }

            if (!temp7)
            {
                if (input_IMP_UPR)
                {
                    input_IMP_UPR = 0;
//                    stringToLcd("input_IMP_UPR OFF");
                    stringTo_diagnostic_Usart1("input_IMP_UPR OFF");
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
//                    stringToLcd("input_VOLT_UPR ON");
                    stringTo_diagnostic_Usart1("input_VOLT_UPR ON");
                }
            }

            if (!temp8)
            {
                if (input_VOLT_UPR)
                {
                    input_VOLT_UPR = 0;
//                    stringToLcd("input_VOLT_UPR OFF");
                    stringTo_diagnostic_Usart1("input_VOLT_UPR OFF");
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
//                    stringToLcd("input_CUR_UPR1 ON");
                    stringTo_diagnostic_Usart1("input_CUR_UPR1 ON");
                }
            }

            if (!temp9)
            {
                if (input_CUR_UPR1)
                {
                    input_CUR_UPR1 = 0;
//                    stringToLcd("input_CUR_UPR1 OFF");
                    stringTo_diagnostic_Usart1("input_CUR_UPR1 OFF");
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
//                    stringToLcd("input_CUR_UPR2 ON");
                    stringTo_diagnostic_Usart1("input_CUR_UPR2 ON");
                }
            }

            if (!temp10)
            {
                if (input_CUR_UPR2)
                {
                    input_CUR_UPR2 = 0;
//                    stringToLcd("input_CUR_UPR2 OFF");
                    stringTo_diagnostic_Usart1("input_CUR_UPR2 OFF");
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
#if defined useMilandr
                    stringToUart(toMilandr_SignalFromOut_ON);
#endif
//                    stringToLcd("signalVnesh ON");
                    stringTo_diagnostic_Usart1("signalVnesh ON");
                }
            }

            if (!temp11)
            {
                if (signalVnesh)
                {
                    signalVnesh = 0;
#if defined useMilandr
                    stringToUart(toMilandr_SignalFromOut_OFF);
#endif
//                    stringToLcd("signalVnesh OFF");
                    stringTo_diagnostic_Usart1("signalVnesh OFF");
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
#if defined useMilandr
                    stringToUart(toMilandr_SignalMic_ON);
#endif
//                    stringToLcd("signalMic ON");
                    stringTo_diagnostic_Usart1("signalMic ON");
                }
            }

            if (!temp12)
            {
                if (signalMic)
                {
                    signalMic = 0;
#if defined useMilandr
                    stringToUart(toMilandr_SignalMic_OFF);
#endif
//                    stringToLcd("signalMic OFF");
                    stringTo_diagnostic_Usart1("signalMic OFF");
                }
            }
            gpioMic = temp12;
        }

        //вход OVERHEAT_MC
        if (temp13 != gpio_OVERHEAT_MC)
        {
            if (temp13)
            {
                if (!OVERHEAT_MC)
                {
                    OVERHEAT_MC = 1;
                    //                    stringToLcd("OVERHEAT_MC ON");
                    stringTo_diagnostic_Usart1("OVERHEAT_MC ON");
                }
            }

            if (!temp13)
            {
                if (OVERHEAT_MC)
                {
                    OVERHEAT_MC = 0;
                    //                    stringToLcd("OVERHEAT_MC OFF");
                    stringTo_diagnostic_Usart1("OVERHEAT_MC OFF");
                }
            }
            gpio_OVERHEAT_MC = temp13;
        }

        //вход POWER_OK
        if (temp14 != gpio_POWER_OK)
        {
            if (temp14)
            {
                if (!POWER_OK)
                {
                    POWER_OK = 1;
                    //                    stringToLcd("POWER_OK ON");
                    stringTo_diagnostic_Usart1("POWER_OK ON");
                }
            }

            if (!temp14)
            {
                if (POWER_OK)
                {
                    POWER_OK = 0;
                    //                    stringToLcd("POWER_OK OFF");
                    stringTo_diagnostic_Usart1("POWER_OK OFF");
                }
            }
            gpio_POWER_OK = temp14;
        }

        //вход ERROR_MC
        if (temp15 != gpio_ERROR_MC)
        {
            if (temp15)
            {
                if (!ERROR_MC)
                {
                    ERROR_MC = 1;
                    //                    stringToLcd("ERROR_MC ON");
                    stringTo_diagnostic_Usart1("ERROR_MC ON");
                }
            }

            if (!temp15)
            {
                if (ERROR_MC)
                {
                    ERROR_MC = 0;
                    //                    stringToLcd("ERROR_MC OFF");
                    stringTo_diagnostic_Usart1("ERROR_MC OFF");
                }
            }
            gpio_ERROR_MC = temp15;
        }
    }
}



const char * checkReceivedByteFromMilandr(char data)
{
#if defined useMilandr
    //Подтверждения принятия команд от Миландра
    if (data == fromMilandr_BtnImpedance_On_OK)     return "BtnImpedance_On_OK";
    if (data == fromMilandr_BtnImpedance_Off_OK)    return "BtnImpedance_Off_OK";
    if (data == fromMilandr_BtnCalibr_On_OK)        return "BtnCalibr_On_OK";
    if (data == fromMilandr_BtnCalibr_Off_OK)       return "BtnCalibr_Off_OK";
    if (data == fromMilandr_BtnReset_On_OK)         return "BtnReset_On_OK";
    if (data == fromMilandr_BtnReset_Off_OK)        return "BtnReset_Off_OK";
    if (data == fromMilandr_SignalPowerOn_ON_OK)    return "SigPowerOn_ON_OK";
    if (data == fromMilandr_SignalPowerOn_OFF_OK)   return "SigPowerOn_OFF_OK";
    if (data == fromMilandr_SignalImpedanse_ON_OK)  return "SigImpedanse_ON_OK";
    if (data == fromMilandr_SignalImpedanse_OFF_OK) return "SigImpedanse_OFF_OK";
    if (data == fromMilandr_SignalTranslate_ON_OK)  return "SigTranslate_ON_OK";
    if (data == fromMilandr_SignalTranslate_OFF_OK) return "SigTranslate_OFF_OK";
    if (data == fromMilandr_SignalFromOut_ON_OK)    return "SigFromOut_ON_OK";
    if (data == fromMilandr_SignalFromOut_OFF_OK)   return "SigFromOut_OFF_OK";
    if (data == fromMilandr_SignalMic_ON_OK)        return "SignalMic_ON_OK";
    if (data == fromMilandr_SignalMic_OFF_OK)       return "SignalMic_OFF_OK";
    if (data == fromMilandr_MilandrNoReceiveCmd) //В Миландре отключил
    {
        MilandrNoReceiveCmd = 1;
        return "MilandrNoReceiveCmd";
    }

    //Команды от Миландра
    if (data == cmdFromMilandr_OVERHEAT_MC_ON)
    {
        OVERHEAT_MC = 1;
        stringToUart(toMilandr_OVERHEAT_MC_ON_OK);
        return "CMD: OVERHEAT_MC_ON";
    }
    if (data == cmdFromMilandr_OVERHEAT_MC_OFF)
    {
        OVERHEAT_MC = 0;
        stringToUart(toMilandr_OVERHEAT_MC_OFF_OK);
        return "CMD: OVERHEAT_MC_OFF";
    }
    if (data == cmdFromMilandr_BOARD_OK_ON)
    {
        POWER_OK = 1;
        stringToUart(toMilandr_BOARD_OK_ON_OK);
        return "CMD: BOARD_OK_ON";
    }
    if (data == cmdFromMilandr_BOARD_OK_OFF)
    {
        POWER_OK = 0;
        stringToUart(toMilandr_BOARD_OK_OFF_OK);
        return "CMD: BOARD_OK_OFF";
    }
    if (data == cmdFromMilandr_ERROR_MC_ON)
    {
        ERROR_MC = 1;
        stringToUart(toMilandr_ERROR_MC_ON_OK);
        return "CMD: ERROR_MC_ON";
    }
    if (data == cmdFromMilandr_ERROR_MC_OFF)
    {
        ERROR_MC = 0;
        stringToUart(toMilandr_ERROR_MC_OFF_OK);
        return "CMD: ERROR_MC_OFF";
    }

    UncnownCmdFromMilandr = 1;
//  return "UncnowCmdFromMilandr";
    return "";
#endif
    return "";
}



