#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif

static QueueHandle_t uart_txq;				// TX queue for UART

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
uart_setup(void)
{
    usart_set_baudrate(UART4, 115200);
    usart_set_databits(UART4,8);
    usart_set_stopbits(UART4,USART_STOPBITS_1);
    usart_set_mode(UART4,USART_MODE_TX);
    usart_set_parity(UART4,USART_PARITY_NONE);
    usart_set_flow_control(UART4,USART_FLOWCONTROL_NONE);
    usart_enable(UART4);

//Описание в теле функции
//    open_uart(4, 115200, "8N1", "rw", 0, 0);
//    uart_txq = xQueueCreate(256,sizeof(char));
}

/*********************************************************************
 * USART Task:
 *********************************************************************/
static void
uart_task(void *args)
{
    int gc;
    char kbuf[256], ch;

    (void)args;

    puts_uart(1,"\n\ruart_task() has begun:\n\r");

    for (;;) {
        if ( (gc = getc_uart_nb(1)) != -1 ) {
            puts_uart(1,"\r\n\nENTER INPUT: ");

            ch = (char)gc;
            if ( ch != '\r' && ch != '\n' ) {
                /* Already received first character */
                kbuf[0] = ch;
                putc_uart(1,ch);
                getline_uart(1,kbuf+1,sizeof kbuf-1);
            } else	{
                /* Read the entire line */
                getline_uart(1,kbuf,sizeof kbuf);
            }

            puts_uart(1,"\r\nReceived input '");
            puts_uart(1,kbuf);
            puts_uart(1,"'\n\r\nResuming prints...\n\r");
        }

        /* Receive char to be TX */
        if ( xQueueReceive(uart_txq,&ch,10) == pdPASS )
            putc_uart(1,ch);
        /* Toggle LED to show signs of life */
        gpio_toggle(GPIOC,GPIO13);
    }
}

/*********************************************************************
 * Queue a string of characters to be TX
 *********************************************************************/
static inline void
uart_puts(const char *s)
{
    for ( ; *s; ++s )
        xQueueSend(uart_txq,s,portMAX_DELAY); /* blocks when queue is full */
}

/*********************************************************************
 * Demo Task:
 *	Simply queues up two line messages to be TX, one seconds
 *	apart.
 *********************************************************************/
static void
demo_task(void *args)
{
    (void)args;

    for (;;) {
        uart_puts("Now this is a message..\n\r");
        uart_puts("  sent via FreeRTOS queues.\n\n\r");
        vTaskDelay(pdMS_TO_TICKS(1000));
        uart_puts("Just start typing to enter a line, or..\n\r"
                  "hit Enter first, then enter your input.\n\n\r");
        vTaskDelay(pdMS_TO_TICKS(1500));
    }
}


///*********************************************************************
// * Send one character to the UART
// *********************************************************************/
//static inline void
//uart_putc(char ch) {
//    usart_send_blocking(UART4, ch);
//}

///*********************************************************************
// * Send characters to the UART, slowly
// *********************************************************************/
//static void
//taskUART(void *args __attribute__((unused))) {
//    int c = '0' - 1;

//    for (;;) {
////        gpio_toggle(GPIOD,GPIO2);
//        vTaskDelay(pdMS_TO_TICKS(50));
//        if ( ++c >= 'Z' ) {
//            uart_putc(c);
//            uart_putc('\r');
//            uart_putc('\n');
//            c = '0' - 1;
//        } else	{
//            uart_putc(c);
//        }
//    }
//}





#ifdef __cplusplus
}
#endif


int
main(void) {

    gpio_setup();
    uart_setup();

//    xTaskCreate(uart_task,"UART", 200, NULL, configMAX_PRIORITIES - 1, NULL);	/* Highest priority */
//    xTaskCreate(demo_task,"DEMO", 100, NULL, configMAX_PRIORITIES - 1, NULL);	/* Lower priority */

    xTaskCreate(testTask1, "LED1", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(taskUART, "testUART", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask3, "LED3", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask4, "LED4", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask5, "LED5", 100, NULL, configMAX_PRIORITIES - 1, NULL);

	vTaskStartScheduler();

	for (;;);

	return 0;
}

