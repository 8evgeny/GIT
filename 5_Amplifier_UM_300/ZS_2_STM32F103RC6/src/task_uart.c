#include "main.h"

/*********************************************************************
 * Send one character to the UART
 *********************************************************************/
static inline void
uart_putc(char ch) {
    usart_send_blocking(UART4, ch);
}

/*********************************************************************
 * Send characters to the UART, slowly
 *********************************************************************/
void
testUART1(void *args __attribute__((unused))) {
    int c = '0' - 1;

    for (;;) {
//        gpio_toggle(GPIOD,GPIO2);
        vTaskDelay(pdMS_TO_TICKS(50));
        if ( ++c >= 'Z' ) {
            uart_putc(c);
            uart_putc('\r');
            uart_putc('\n');
            c = '0' - 1;
        } else	{
            uart_putc(c);
        }
    }
}



