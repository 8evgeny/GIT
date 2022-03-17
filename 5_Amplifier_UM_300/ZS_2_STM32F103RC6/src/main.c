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

/*********************************************************************
 * USART Task:
 *********************************************************************/
static void
uart_task(void *args __attribute__((unused))) {
    char ch;

    for (;;) {
        // Receive char to be TX
        if ( xQueueReceive(uart_txq,&ch,10) == pdPASS ) {
            while ( !usart_get_flag(UART4, USART_SR_TXE) )
                taskYIELD();	// Yield until ready
            usart_send(UART4, ch);
        }
        // Toggle LED to show signs of life
        gpio_toggle(GPIOD,GPIO2);
    }
}

/*********************************************************************
 * Queue a string of characters to be TX
 *********************************************************************/
static void
uart_puts(const char *s) {

    for ( ; *s; ++s ) {
        // blocks when queue is full
        xQueueSend(uart_txq,s,portMAX_DELAY);
    }
}

/*********************************************************************
 * Demo Task:
 *	Simply queues up two line messages to be TX, one second
 *	apart.
 *********************************************************************/

static void
demo_task(void *args __attribute__((unused)))
{

    for (;;)
    {

        uart_puts("Now this is a message sent via FreeRTOS queues");
        uart_puts("\n\r");
        vTaskDelay(pdMS_TO_TICKS(1000));

    }
}



//#ifdef __cplusplus
//}
//#endif


int
main(void) {

    gpio_setup();
    uart_setup();

    xTaskCreate(uart_task,"UART",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(demo_task,"DEMO",100,NULL,configMAX_PRIORITIES-2,NULL);


    xTaskCreate(testTask1, "LED1", 100, NULL, configMAX_PRIORITIES - 1, NULL);

//    xTaskCreate(taskUART, "testUART", 100, NULL, configMAX_PRIORITIES - 1, NULL);

//    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask3, "LED3", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask4, "LED4", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask5, "LED5", 100, NULL, configMAX_PRIORITIES - 1, NULL);

	vTaskStartScheduler();

	for (;;);

	return 0;
}

