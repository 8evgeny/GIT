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

//    xTaskCreate(testTask1, "TestSignalls", 50, NULL, configMAX_PRIORITIES - 1, NULL);
//    vTaskStartScheduler();
//    for (;;)
//    {};

    while(1)
    {
        gpio_set(GPIOC,GPIO13);
        delay();
        delay();
        delay();
        gpio_clear(GPIOC,GPIO13);
        delay();
        delay();
        delay();
    }


    return 0;

}

