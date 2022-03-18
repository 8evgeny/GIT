#include "main.h"

/*********************************************************************
 * Send one character to the UART
 *********************************************************************/
static inline void
uart_putc(char ch) {
    if(!uart4) usart_send_blocking(USART1, ch);
    if(uart4) usart_send_blocking(UART4, ch);
}

static inline void
uart_getc() {
    uint16_t num;

    {
        num = usart_recv(UART4);
        usart_send(UART4, '\n');
        usart_send(UART4, num);
    }
}
/*********************************************************************
 * Send characters to the UART, slowly
 *********************************************************************/
void
testUART1(void *args __attribute__((unused))) {
    int c = '0' - 1;

    for (;;)
    {
//        gpio_toggle(GPIOD,GPIO2);
        vTaskDelay(pdMS_TO_TICKS(500));

//        if ( ++c >= 'Z' )
//        {
//            uart_putc(c);
//            uart_putc('\r');
//            uart_putc('\n');
//            c = '0' - 1;
//        } else
//        {
//            uart_putc(c);
//        }

        uart_getc();

    }
}



