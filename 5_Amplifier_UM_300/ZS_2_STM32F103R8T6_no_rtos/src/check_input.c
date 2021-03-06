#include "main.h"
#include "stdio.h"

void fsignalPowerOnON()
{
    if(!isPOWER_OFF)
    {
        signalPowerOn = 1;
    #if defined useMilandr
        stringToUart(toMilandr_SignalPowerOn_ON);
    #endif
    //    stringToLcd("signalPowerOn ON");
        stringTo_diagnostic_Usart1("signalPowerOn ON");
        setFan(true);
    //    setUpr_IN_078(true); stringTo_diagnostic_Usart1("setUpr_IN_078 ON");
        setReleLine1(true);
        setMute(false); stringTo_diagnostic_Usart1("MUTE OFF");
        setGAIN(true);  stringTo_diagnostic_Usart1("GAIN ON");
        if (!RESET_AMP )
        {
            setRESET(true);
            vTaskDelay(pdMS_TO_TICKS(500));
            setRESET(false);
            RESET_AMP = true; stringTo_diagnostic_Usart1("RESETTING AMP");
        }
    }
    else
    {
        stringTo_diagnostic_Usart1("ERROR POWER");
    }

}
void fsignalPowerOnOFF()
{
    signalPowerOn = 0;
#if defined useMilandr
    stringToUart(toMilandr_SignalPowerOn_OFF);
#endif
//    stringToLcd("signalPowerOn OFF");
    stringTo_diagnostic_Usart1("signalPowerOn OFF");
    setFan(false);
//    setUpr_IN_078(false); stringTo_diagnostic_Usart1("setUpr_IN_078 OFF");
    setReleLine1(false);
    setMute(true); stringTo_diagnostic_Usart1("MUTE ON");
    setGAIN(false); stringTo_diagnostic_Usart1("GAIN OFF");
    RESET_AMP = false;
}
void fPOWER_OK_ON()
{
    POWER_OK = 1;
    setReadyLed(true);
//    stringToLcd("POWER_OK ON");
    stringTo_diagnostic_Usart1("POWER_OK ON");
}
void fPOWER_OK_OFF()
{
    POWER_OK = 0;
    setReadyLed(false);
//    stringToLcd("POWER_OK OFF");
    stringTo_diagnostic_Usart1("POWER_OK OFF");
}
void fOVERHEAT_MC_ON()
{
    OVERHEAT_MC = 1;
//    stringToLcd("OVERHEAT_MC ON");
    stringTo_diagnostic_Usart1("OVERHEAT_MC ON");
    setLedOverheatOut(true);
    setErrorRele(true);
}
void fOVERHEAT_MC_OFF()
{
//    OVERHEAT_MC = 0;
////    stringToLcd("OVERHEAT_MC OFF");
//    stringTo_diagnostic_Usart1("OVERHEAT_MC OFF");
//    setErrorRele(false);
//    setLedOverheatOut(false);
}
void fERROR_MC_ON()
{
    ERROR_MC = 1;
//    stringToLcd("ERROR_MC ON");
    stringTo_diagnostic_Usart1("ERROR_MC ON");
    setLedOvercutOut(true);
    setErrorRele(true);
}
void fERROR_MC_OFF()
{
//    ERROR_MC = 0;
////    stringToLcd("ERROR_MC OFF");
//    stringTo_diagnostic_Usart1("ERROR_MC OFF");
//    setLedOvercutOut(false);
//    setErrorRele(false);
}
void fsignalVneshON()
{
    signalVnesh = 1;
#if defined useMilandr
    stringToUart(toMilandr_SignalFromOut_ON);
#endif
//    stringToLcd("signalVnesh ON");
    setUpr_IN_ST(true); stringTo_diagnostic_Usart1("setUpr_IN_ST ON");
    setRele24V(true);
    stringTo_diagnostic_Usart1("signalVnesh ON");
}
void fsignalVneshOFF()
{
    signalVnesh = 0;
#if defined useMilandr
    stringToUart(toMilandr_SignalFromOut_OFF);
#endif
//    stringToLcd("signalVnesh OFF");
    setUpr_IN_ST(false); stringTo_diagnostic_Usart1("setUpr_IN_ST OFF");
    setRele24V(false);
    stringTo_diagnostic_Usart1("signalVnesh OFF");
}
void fsignalMicON()
{
    signalMic = 1;
#if defined useMilandr
    stringToUart(toMilandr_SignalMic_ON);
#endif
//    stringToLcd("signalMic ON");
    setUpr_IN_078(false);
#ifndef useUSART1
    setReleTr1(true);
#endif
    setRele24V(true);
    stringTo_diagnostic_Usart1("signalMic ON");
}
void fsignalMicOFF()
{
    signalMic = 0;
#if defined useMilandr
    stringToUart(toMilandr_SignalMic_OFF);
#endif
//    stringToLcd("signalMic OFF");
    setUpr_IN_078(true);
#ifndef useUSART1
    setReleTr1(false);
#endif
    setRele24V(false);
    stringTo_diagnostic_Usart1("signalMic OFF");
}
void fsignalImpedanceON()
{
    signalImpedance = 1;
#if defined useMilandr
    stringToUart(toMilandr_SignalImpedanse_ON);
#endif

//    stringToLcd("signal Impedance ON");
    stringTo_diagnostic_Usart1("signal Impedance ON");
    setImpedanceRele(true);
}
void fsignalImpedanceOFF()
{
    signalImpedance = 0;
#if defined useMilandr
    stringToUart(toMilandr_SignalImpedanse_OFF);
#endif

//    stringToLcd("signal Impedance OFF");
    stringTo_diagnostic_Usart1("signal Impedance OFF");
    setImpedanceRele(false);
}
void fsignalTranslateON()
{
    signalTranslate = 1;
#if defined useMilandr
    stringToUart(toMilandr_SignalTranslate_ON);
#endif
//    stringToLcd("signal Translate ON");
#ifndef useUSART1
    setReleTr1(true);
#endif
    setRele24V(true);
    stringTo_diagnostic_Usart1("signal Translate ON");
}
void fsignalTranslateOFF()
{
    signalTranslate = 0;
#if defined useMilandr
    stringToUart(toMilandr_SignalTranslate_OFF);
#endif
//    stringToLcd("signal Translate OFF");
#ifndef useUSART1
    setReleTr1(false);
#endif
    setRele24V(false);
    stringTo_diagnostic_Usart1("signal Translate OFF");
}
void fbtnRESET_ON()
{
    btnReset = 1;    stringTo_diagnostic_Usart1("Button Reset ON");
    OVERHEAT_MC = 0; stringTo_diagnostic_Usart1("OVERHEAT_MC OFF");
    ERROR_MC = 0;    stringTo_diagnostic_Usart1("ERROR_MC OFF");
#if defined useMilandr
    stringToUart(toMilandr_BtnReset_On);
#endif

    setErrorRele(false);
    setLedOverheatOut(false);
    setLedOvercutOut(false);

    setRESET(true);
    vTaskDelay(pdMS_TO_TICKS(500));
    setRESET(false);
    RESET_AMP = true; stringTo_diagnostic_Usart1("RESETTING AMP");
}
void fbtnRESET_OFF()
{
    btnReset = 0;
#if defined useMilandr
    stringToUart(toMilandr_BtnReset_Off);
#endif
    stringTo_diagnostic_Usart1("Button Reset OFF");
}
void finput_VOLT_UPR_ON()
{
    input_VOLT_UPR = 1;
//    stringToLcd("input_VOLT_UPR ON");
    stringTo_diagnostic_Usart1("U less THRESHOLD");
//?????? ???????????????? ?????????????????? ???????????????? ???????????????????? ???????????? ???????????????????? ????????????????, ????????????????
//?????? ?????????????????? ???????????? (????????????) ??/?????? ???????????? (??????????????) ?????????????????? ????????????????. ?????????????? ????????
//?????????????????? ???????????????? ???????????????????? ?????? ?????????????????????? ?????????????????????? ?????????????????? ???????????????????? ??
//???????????????? ?????????????????? ???? ?????????? ?? ?????????????? ?????????????? ???????????????????? ???? ???????????? ??????????????????

//    if(!input_CUR_UPR1)
//    {//????
//        setLedShortOut(true); stringTo_diagnostic_Usart1("ShortOut ON");
//        setErrorRele(true); stringTo_diagnostic_Usart1("ERROR ON");
//        setMute(true); stringTo_diagnostic_Usart1("MUTE ON");
//        RESET_AMP = false;
//    }


}
void finput_VOLT_UPR_OFF()
{
    input_VOLT_UPR = 0;
    stringTo_diagnostic_Usart1("U bigger THRESHOLD");

    //?????? ???????????????????? ???????????? ???????????????? ???????????????????? ???????? ???????????????????? ????????????????, ???????????????? ??????
    //?????????????????? ???????????? (??????????????) ?????????????????? ????????????????.
//    if(!input_CUR_UPR2)
//    {//????????????????????
//        setLedOvercutOut(true); stringTo_diagnostic_Usart1("OvercutOut");
//        setErrorRele(true); stringTo_diagnostic_Usart1("ERROR ON");
//        setMute(true); stringTo_diagnostic_Usart1("MUTE ON");
//        RESET_AMP = false;
//    }

}
void fCURRENT1_ON()
{
    input_CUR_UPR1 = 1;
//    stringToLcd("input_CUR_UPR1 ON");
    stringTo_diagnostic_Usart1("I_2 less THRESHOLD");
}
void fCURRENT1_OFF()
{
    input_CUR_UPR1 = 0;
//    stringToLcd("input_CUR_UPR1 OFF");
    stringTo_diagnostic_Usart1("I_2 bigger THRESHOLD");
}
void fCURRENT2_ON()
{
    input_CUR_UPR2 = 1;
//    stringToLcd("input_CUR_UPR2 ON");
    stringTo_diagnostic_Usart1("I_2 less THRESHOLD");
}
void fCURRENT2_OFF()
{
    input_CUR_UPR2 = 0;
//    stringToLcd("input_CUR_UPR2 OFF");
    stringTo_diagnostic_Usart1("I_2 bigger THRESHOLD");
}


void checkInputs(void *args)
{
    (void)args;

    bool gpioImpedance;          //???????????? "????????????????"
    bool gpioCalibrovka;         //???????????? "????????????????????"
    bool gpioReset;              //???????????? "?????????? ????????????"
    bool gpioPowerOn;            //?????????????? ???????????? ???????????????????? 20 - 72V??
    bool gpioSignalImpedance;    //?????????????? ???????????? ?????????????????? 20 - 72V??
    bool gpioSignalTranslate;    //?????????????? ???????????? ?????????????????????? 20 - 72V??
    bool gpio_IMP_UPR;           //???????? IMP_UPR
    bool gpio_VOLT_UPR;          //???????? VOLT_UPR
    bool gpio_CUR_UPR1;          //???????? CUR_UPR1
    bool gpio_CUR_UPR2;          //???????? CUR_UPR2
    bool gpioVnesh;              //?????????????? ???????????? ???????????????? ???????????? 20 - 72V??
    bool gpioMic;                //?????????????? ???????????? ?????????????????? 20 - 72V??
    bool gpio_OVERHEAT_MC;       //???????? OVERHEAT_MC
    bool gpio_POWER_OK;          //???????? POWER_OK
    bool gpio_ERROR_MC;          //???????? ERROR_MC

    bool temp1,temp2,temp3,temp4,temp5,temp6;
    bool temp7,temp8,temp9,temp10,temp11,temp12;
    bool temp13,temp14,temp15;

    gpioImpedance = !isBtnIMPEDANSE;  //???????????? - ??????????????????
    gpioCalibrovka = !isBtnCALIBROVKA;//???????????? - ??????????????????
    gpioReset = !isBtnRESET;          //???????????? - ??????????????????
    gpioPowerOn = gpio_get(GPIOA, GPIO3);
    gpioSignalImpedance = gpio_get(GPIOA, GPIO4);
    gpioSignalTranslate = gpio_get(GPIOA, GPIO5);
    gpio_IMP_UPR = gpio_get(GPIOC, GPIO0);
    gpio_VOLT_UPR = isVOLT_UPR;
    gpio_CUR_UPR1 = isCURR1_UPR;
    gpio_CUR_UPR2 = isCURR2_UPR;
    gpioVnesh = gpio_get(GPIOC, GPIO13);
    gpioMic = gpio_get(GPIOD, GPIO2);
    gpio_OVERHEAT_MC = gpio_get(GPIOA, GPIO6);
    gpio_POWER_OK = isPOWER_OFF;
    gpio_ERROR_MC = gpio_get(GPIOC, GPIO5);

    for(;;)
    {
        vTaskDelay(pdMS_TO_TICKS(50));

        temp1 = !isBtnIMPEDANSE;  //???????????? - ??????????????????
        temp2 = !isBtnCALIBROVKA; //???????????? - ??????????????????
        temp3 = !isBtnRESET;      //???????????? - ??????????????????
        temp4 = gpio_get(GPIOA, GPIO3);
        temp5 = gpio_get(GPIOA, GPIO4);
        temp6 = gpio_get(GPIOA, GPIO5);
        temp7 = gpio_get(GPIOC, GPIO0);
        temp8 = isVOLT_UPR;
        temp9 = isCURR1_UPR;
        temp10 = isCURR2_UPR;
        temp11 = gpio_get(GPIOC, GPIO13);
        temp12 = gpio_get(GPIOD, GPIO2);
        temp13 = gpio_get(GPIOA, GPIO6);
        temp14 = isPOWER_OFF;
        temp15 = gpio_get(GPIOC, GPIO5);

    //???????????? ????????????????
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

    //???????????? ????????????????????
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

    //???????????? ?????????? ????????????
        if (temp3 != gpioReset)
        {
            if (temp3)
            {
                if (!btnReset)
                {
                    fbtnRESET_ON();
                }
            }

            if (!temp3)
            {
                if (btnReset)
                {
                    fbtnRESET_OFF();
                }
            }
            gpioReset = temp3;
        }

    //+?????????????? ???????????? ???????????????????? 20 - 72V??
        if (temp4 != gpioPowerOn)
        {
            if (temp4)
            {
                if (!signalPowerOn)
                {
                    fsignalPowerOnON();
                }
            }

            if (!temp4)
            {
                if (signalPowerOn)
                {
                    fsignalPowerOnOFF();
                }
            }
            gpioPowerOn = temp4;
        }

    //+?????????????? ???????????? ?????????????????? 20 - 72V??
        if (temp5 != gpioSignalImpedance)
        {
            if (temp5)
            {
                if (!signalImpedance)
                {
                    fsignalImpedanceON();
                }
            }

            if (!temp5)
            {
                if (signalImpedance)
                {
                    fsignalImpedanceOFF();
                }
            }
            gpioSignalImpedance = temp5;
        }

    //+?????????????? ???????????? ?????????????????????? 20 - 72V??
        if (temp6 != gpioSignalTranslate)
        {
            if (temp6)
            {
                if (!signalTranslate)
                {
                    fsignalTranslateON();
                }
            }

            if (!temp6)
            {
                if (signalTranslate)
                {
                    fsignalTranslateOFF();
                }
            }
            gpioSignalTranslate = temp6;
        }

    //???????? IMP_UPR
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

    //???????? VOLT_UPR
        if (temp8 != gpio_VOLT_UPR)
        {
            if (temp8)
            {
                if (!input_VOLT_UPR)
                {
                    finput_VOLT_UPR_ON();
                }
            }

            if (!temp8)
            {
                if (input_VOLT_UPR)
                {
                    finput_VOLT_UPR_OFF();
                }
            }
            gpio_VOLT_UPR = temp8;
        }

    //???????? CUR_UPR1
        if (temp9 != gpio_CUR_UPR1)
        {
            if (temp9)
            {
                if (!input_CUR_UPR1)
                {
                    fCURRENT1_ON();
                }
            }

            if (!temp9)
            {
                if (input_CUR_UPR1)
                {
                    fCURRENT1_OFF();
                }
            }
            gpio_CUR_UPR1 = temp9;
        }

    //???????? CUR_UPR2
        if (temp10 != gpio_CUR_UPR2)
        {
            if (temp10)
            {
                if (!input_CUR_UPR2)
                {
                    fCURRENT2_ON();
                }
            }

            if (!temp10)
            {
                if (input_CUR_UPR2)
                {
                    fCURRENT2_OFF();
                }
            }
            gpio_CUR_UPR2 = temp10;
        }

    //+?????????????? ???????????? ???????????????? ???????????? 20 - 72V??
        if (temp11 != gpioVnesh)
        {
            if (temp11)
            {
                if (!signalVnesh)
                {
                    fsignalVneshON();
                }
            }

            if (!temp11)
            {
                if (signalVnesh)
                {
                    fsignalVneshOFF();
                }
            }
            gpioVnesh = temp11;
        }

    //+?????????????? ???????????? ?????????????????? 20 - 72V??
        if (temp12 != gpioMic)
        {
            if (temp12)
            {
                if (!signalMic)
                {
                    fsignalMicON();
                }
            }

            if (!temp12)
            {
                if (signalMic)
                {
                    fsignalMicOFF();
                }
            }
            gpioMic = temp12;
        }

    //+???????? OVERHEAT_MC
        if (temp13 != gpio_OVERHEAT_MC)
        {
            if (temp13)
            {
                if (!OVERHEAT_MC)
                {
                    fOVERHEAT_MC_ON();
                }
            }

            if (!temp13)
            {
                if (OVERHEAT_MC)
                {
                    fOVERHEAT_MC_OFF();
                }
            }
            gpio_OVERHEAT_MC = temp13;
        }

    //+???????? POWER_OK
        if (temp14 != gpio_POWER_OK)
        {
            if (temp14)
            {
                if (!POWER_OK)
                {
                    fPOWER_OK_ON();
                }
            }

            if (!temp14)
            {
                if (POWER_OK)
                {
                    fPOWER_OK_OFF();
                }
            }
            gpio_POWER_OK = temp14;
        }

    //+???????? ERROR_MC
        if (temp15 != gpio_ERROR_MC)
        {
            if (temp15)
            {
                if (!ERROR_MC)
                {
                    fERROR_MC_ON();
                }
            }

            if (!temp15)
            {
                if (ERROR_MC)
                {
                    fERROR_MC_OFF();
                }
            }
            gpio_ERROR_MC = temp15;
        }
    }
}



const char * checkReceivedByteFromMilandr(char data)
{
#if defined useMilandr
    //?????????????????????????? ???????????????? ???????????? ???? ????????????????
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
    if (data == fromMilandr_MilandrNoReceiveCmd) //?? ???????????????? ????????????????
    {
        MilandrNoReceiveCmd = 1;
        return "MilandrNoReceiveCmd";
    }

    //?????????????? ???? ????????????????
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



