#include "main.h"

void
testTask1(void *args __attribute((unused)))
{
    for (;;)
    {
        gpio_toggle(GPIOC, GPIO6);
        vTaskDelay(pdMS_TO_TICKS(700));
    }
}


void
testTask2(void *args __attribute((unused)))
{
    for (;;)
    {
        gpio_toggle(GPIOB, GPIO10);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void
testTask3(void *args __attribute((unused)))
{
    for (;;)
    {
        gpio_toggle(GPIOB, GPIO12);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void
testTask4(void *args __attribute((unused)))
{
    for (;;)
    {
        gpio_toggle(GPIOB, GPIO14);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
void
testTask5(void *args __attribute((unused)))
{
    for (;;)
    {
        gpio_toggle(GPIOB, GPIO1);
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}


