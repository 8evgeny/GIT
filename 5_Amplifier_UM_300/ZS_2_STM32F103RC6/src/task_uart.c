#include "main.h"

static inline void
uart_putc(char ch) {
    usart_send_blocking(USART3, ch);
}

void
testUART1(void *args __attribute__((unused)))
{
    int c = '0' - 1;
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(200));

        if ( ++c >= 'Z' )
        {
            uart_putc(c);
            uart_putc('\r');
            uart_putc('\n');
            c = '0' - 1;
        } else
        {
            uart_putc(c);
        }
    }
}

void testUART2(void *args)
{
    (void)args;

    for (;;)
    {
        stringToUart("\r\nNow this is a message..\n\r");
        stringToUart("  sent via FreeRTOS queues.\n\n\r");
        vTaskDelay(pdMS_TO_TICKS(2000));
        stringToUart("Just start typing to enter a line, or..\n\r"
                     "hit Enter first, then enter your input.\n\n\r");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

