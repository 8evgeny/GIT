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
uart_task(void *args __attribute__((unused))) {
    char ch;

    for (;;)
    {
        // Receive char to be TX
        if ( xQueueReceive(uart_txq,&ch,10) == pdPASS ) {
            while ( !usart_get_flag(UART4, USART_SR_TXE) )
                taskYIELD();	// Yield until ready
            usart_send(UART4, ch);
        }
        // Toggle LED to show signs of life
//        gpio_toggle(GPIOD,GPIO2);
    }
}

static void
stringToUart(const char *s) {

    for ( ; *s; ++s ) {
        // blocks when queue is full
        xQueueSend(uart_txq,s,portMAX_DELAY);
    }
}

static void
testUART2(void *args __attribute__((unused)))
{
    for (;;)
    {
        stringToUart("\nNow this is a message sent via FreeRTOS queues");
        stringToUart("\n\r");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


//#ifdef __cplusplus
//}
//#endif

static void
uart_setup()
{
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_UART4);

    usart_set_baudrate(UART4,115200);
    usart_set_databits(UART4,8);
    usart_set_stopbits(UART4,USART_STOPBITS_1);
    usart_set_mode(UART4,USART_MODE_TX);
    usart_set_parity(UART4,USART_PARITY_NONE);
    usart_set_flow_control(UART4,USART_FLOWCONTROL_NONE);
    usart_enable(UART4);

    // Create a queue for data to transmit from UART
    uart_txq = xQueueCreate(256,sizeof(char));

//    open_uart(4, 115200, "8N1", "rw", 0, 0); //Описание в теле функции

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

