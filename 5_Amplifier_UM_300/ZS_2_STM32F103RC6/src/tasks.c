#include "main.h"

void
task1(void *args __attribute((unused)))
{

    for (;;) {
        gpio_toggle(GPIOC, GPIO6);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void
task2(void *args __attribute((unused)))
{

    for (;;) {
        gpio_toggle(GPIOB, GPIO14);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void
task3(void *args __attribute((unused)))
{

    for (;;) {
        gpio_toggle(GPIOD, GPIO2);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
