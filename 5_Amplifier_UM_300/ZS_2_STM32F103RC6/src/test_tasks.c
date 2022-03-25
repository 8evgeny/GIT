#include "main.h"

void testTask1(void *args __attribute((unused)))
{
    for (;;)
    {
        setTestLed1(true);
        vTaskDelay(pdMS_TO_TICKS(700));
        setTestLed1(false);
        vTaskDelay(pdMS_TO_TICKS(700));
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
    int delay = 300;
    for (;;)
    {
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
    }
}




