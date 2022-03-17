#include "main.h"

static QueueHandle_t uart_txq;				// TX queue for UART
void
uart_setup()
{
    usart_set_baudrate(UART4,115200);
    usart_set_databits(UART4,8);
    usart_set_stopbits(UART4,USART_STOPBITS_1);
    usart_set_mode(UART4,USART_MODE_TX);
    usart_set_parity(UART4,USART_PARITY_NONE);
    usart_set_flow_control(UART4,USART_FLOWCONTROL_NONE);
    usart_enable(UART4);

//    open_uart(4, 115200, "8N1", "rw", 0, 0); //Описание в теле функции
//    uart_txq = xQueueCreate(256, sizeof(char));

}
