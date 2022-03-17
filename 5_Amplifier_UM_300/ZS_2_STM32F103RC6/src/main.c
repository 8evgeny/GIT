#include "main.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

extern void
vApplicationStackOverflowHook(
	xTaskHandle *pxTask,
	signed portCHAR *pcTaskName);

void
vApplicationStackOverflowHook(
  xTaskHandle *pxTask __attribute((unused)),
  signed portCHAR *pcTaskName __attribute((unused)))
{
	for(;;);	// Loop forever here..
}

static void
uart_task(void *args) {
    int gc;
    char kbuf[256], ch;

    (void)args;

    puts_uart(4, "\n\ruart_task() has begun:\n\r");

    for (;;) {
        if ( (gc = getc_uart_nb(4)) != -1 )
        {
            puts_uart(4, "\r\n\nENTER INPUT: ");

            ch = (char)gc;
            if ( ch != '\r' && ch != '\n' ) {
                /* Already received first character */
                kbuf[0] = ch;
                putc_uart(4, ch);
                getline_uart(4, kbuf+1, sizeof kbuf-1);
            } else	{
                /* Read the entire line */
                getline_uart(4, kbuf, sizeof kbuf);
            }

            puts_uart(4, "\r\nReceived input '");
            puts_uart(4, kbuf);
            puts_uart(4, "'\n\r\nResuming prints...\n\r");
        }

        /* Receive char to be TX */
        if ( xQueueReceive(uart_txq, &ch, 10) == pdPASS )
            putc_uart(4, ch);
        /* Toggle LED to show signs of life */
        gpio_toggle(GPIOD,GPIO2);
//        gpio_toggle(GPIOC,GPIO13);
    }
}

//static void
//uart_task(void *args __attribute__((unused)))
//{
//    char ch;

//    for (;;)
//    {
//        // Receive char to be TX
//        if ( xQueueReceive(uart_txq,&ch,10) == pdPASS ) {
//            while ( !usart_get_flag(UART4, USART_SR_TXE) )
//                taskYIELD();	// Yield until ready
//            usart_send(UART4, ch);
//        }
//        // Toggle LED to show signs of life
////        gpio_toggle(GPIOD,GPIO2);
//    }
//}

static inline void
stringToUart(const char *s) {

    for ( ; *s; ++s ) {
        // blocks when queue is full
        xQueueSend(uart_txq, s, portMAX_DELAY);
    }
}

static void
testUART2(void *args)
{
    (void)args;

    for (;;)
    {
        stringToUart("Now this is a message..\n\r");
        stringToUart("  sent via FreeRTOS queues.\n\n\r");
        vTaskDelay(pdMS_TO_TICKS(1000));
        stringToUart("Just start typing to enter a line, or..\n\r"
                  "hit Enter first, then enter your input.\n\n\r");
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}


//#ifdef __cplusplus
//}
//#endif

static void
uart_setup()
{
//    usart_set_baudrate(UART4,115200);
//    usart_set_databits(UART4,8);
//    usart_set_stopbits(UART4,USART_STOPBITS_1);
//    usart_set_mode(UART4,USART_MODE_TX);
//    usart_set_parity(UART4,USART_PARITY_NONE);
//    usart_set_flow_control(UART4,USART_FLOWCONTROL_NONE);
//    usart_enable(UART4);

    open_uart(4, 115200, "8N1", "rw", 0, 0); //Описание в теле функции

    // Create a queue for data to transmit from UART
    uart_txq = xQueueCreate(256,sizeof(char));

}

int
main(void) {

    gpio_setup();
    uart_setup();

    xTaskCreate(uart_task,"UART",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(testUART1, "testUART", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(testUART2,"DEMO",100,NULL,configMAX_PRIORITIES-2,NULL);

    xTaskCreate(testTask1, "LED1", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(testTask3, "LED3", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(testTask4, "LED4", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(testTask5, "LED5", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(testTask6, "LED6", 100, NULL, configMAX_PRIORITIES - 1, NULL);

	vTaskStartScheduler();

	for (;;);

	return 0;
}

