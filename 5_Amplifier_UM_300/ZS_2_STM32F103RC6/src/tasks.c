#include "main.h"

void
testTask1(void *args __attribute((unused)))
{

    for (;;) {
        gpio_toggle(GPIOC, GPIO6);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}


void
testTask2(void *args __attribute((unused)))
{

    for (;;) {
        gpio_toggle(GPIOD, GPIO2);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
