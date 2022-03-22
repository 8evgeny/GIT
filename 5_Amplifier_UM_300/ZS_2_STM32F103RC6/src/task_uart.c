#include "main.h"

#if 0
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
#endif
