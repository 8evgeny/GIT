#include "main.h"

void
testTask1(void *args __attribute((unused)))
{
    for (;;)
    {
        setTestLed1(true);
        vTaskDelay(pdMS_TO_TICKS(700));
        setTestLed1(false);
        vTaskDelay(pdMS_TO_TICKS(700));
    }
}


void
testTask2(void *args __attribute((unused)))
{
    for (;;)
    {
        setTestLed2(true);
        vTaskDelay(pdMS_TO_TICKS(100));
        setTestLed2(false);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void
testTask3(void *args __attribute((unused)))
{
    for (;;)
    {
        setTestLed3(true);
        vTaskDelay(pdMS_TO_TICKS(500));
        setTestLed3(false);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}





