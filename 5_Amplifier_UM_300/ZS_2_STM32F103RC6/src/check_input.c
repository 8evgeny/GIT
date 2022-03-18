#include "main.h"
#include "stdio.h"
void checkImpedance()
{
    uint16_t gpioImpedance;
    gpioImpedance = gpio_get(GPIOA, GPIO0);
    char buf[80];
    sprintf(buf, "%d", gpioImpedance);
//    write_uart(3, buf, sizeof buf);
    stringToUart(buf);
//    close_uart(3);
    vTaskDelay(pdMS_TO_TICKS(1000));

}
