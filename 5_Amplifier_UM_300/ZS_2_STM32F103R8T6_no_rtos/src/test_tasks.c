#include "main.h"

void testTaski2C(void *args __attribute((unused)))
{
    for (;;)
    {
        setTestSCL(true);
        setTestSDA(true);
        vTaskDelay(pdMS_TO_TICKS(300));
        setTestSCL(false);
        setTestSDA(false);
        vTaskDelay(pdMS_TO_TICKS(300));
    }
}

void testTask1(void *args __attribute((unused)))
{
    for (;;)
    {
//        setTestLed1(true);
//        vTaskDelay(pdMS_TO_TICKS(700));
//        setTestLed1(false);
        vTaskDelay(pdMS_TO_TICKS(700));
        setLedOverheatOut(0);
        setLedOvercutOut(0);
        setLedShortOut(0);
        setLedBreakOut(0);
        setErrorRele(0);

//Реле и Led
        setReleLine1(0);
        setRele24V(0);
        setReleLine2(0);
        setReleLine3(0);
        setReleLine4(0);
        setReleTr1(0);
        setReleTr2(0);
        setReleTr3(0);
        setReleTr4(0);
        setReadyLed(0);
        setFan(0);

vTaskDelay(pdMS_TO_TICKS(700));
        setLedOverheatOut(1);
        setLedOvercutOut(1);
        setLedShortOut(1);
        setLedBreakOut(1);
        setErrorRele(1);

//Реле и Led
        setReleLine1(1);
        setRele24V(1);
        setReleLine2(1);
        setReleLine3(1);
        setReleLine4(1);
        setReleTr1(1);
        setReleTr2(1);
        setReleTr3(1);
        setReleTr4(1);
        setReadyLed(1);
        setFan(1);


    }
}


void testTask2(void *args __attribute((unused)))
{
    for (;;)
    {
        setTestLed2(true);
        vTaskDelay(pdMS_TO_TICKS(100));
        setTestLed2(false);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void testTask3(void *args __attribute((unused)))
{
    for (;;)
    {
        setTestLed3(true);
        vTaskDelay(pdMS_TO_TICKS(500));
        setTestLed3(false);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void testSendUartCommand(void *args __attribute((unused)))
{
    const int delay = 1000;
    for (;;)
    {
#if defined useUSART3
        stringToUart(toMilandr_BtnImpedance_On);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_BtnImpedance_Off);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_BtnCalibr_On);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_BtnCalibr_Off);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_BtnReset_On);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_BtnReset_Off);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalPowerOn_ON);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalPowerOn_OFF);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalImpedanse_ON);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalImpedanse_OFF);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalTranslate_ON);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalTranslate_OFF);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalFromOut_ON);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalFromOut_OFF);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalMic_ON);
        vTaskDelay(pdMS_TO_TICKS(delay));
        stringToUart(toMilandr_SignalMic_OFF);
        vTaskDelay(pdMS_TO_TICKS(delay));
#endif
    }
}




