#include "main.h"

void delay()
{
    for (int i = 0; i < 500000; i++)
        __asm__("nop");
}

extern void
vApplicationStackOverflowHook(
    xTaskHandle *pxTask,
    signed portCHAR *pcTaskName);

void
vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName)
{
    (void)pxTask;
    (void)pcTaskName;
    for(;;);
}

int main()
{
    gpio_setup();

    xTaskCreate(testTask1, "TestSignalls", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    vTaskStartScheduler();
    for (;;)
    {};

    while(1)
    {
        gpio_set(GPIOD,GPIO2);
        delay();
        gpio_clear(GPIOD,GPIO2);
        delay();
    }


    return 0;

}

