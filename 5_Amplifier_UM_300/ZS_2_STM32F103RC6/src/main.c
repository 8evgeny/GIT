#include "main.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

extern void
vApplicationStackOverflowHook(
    xTaskHandle *pxTask,
    signed portCHAR *pcTaskName);

void
vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName)
{
    (void)pxTask;
    (void)pcTaskName;
    for(;;);
}


static void
uart_task(void *args) {
    int gc;
    char kbuf[256], ch;

    (void)args;

//    puts_uart(3, "\n\ruart_task() has begun:\n\r");

    for (;;)
    {
        gc = getc_uart_nb(3);

        if ( gc != -1 )
        {
//            puts_uart(3, "\r\n\nENTER INPUT: ");

            ch = (char)gc;
            if ( ch == '*' )
            {
                stringToLcd("Recrive Cmd");
            }

            if ( ch != '\r' && ch != '\n' )
            {
                /* Already received first character */
                kbuf[0] = ch;
                putc_uart(3, ch);
                getline_uart(3, kbuf+1, sizeof kbuf-1);

            } else
            {
                /* Read the entire line */
                getline_uart(3, kbuf, sizeof kbuf);
            }

            puts_uart(3, "\r\nReceived command: ");
            puts_uart(3, kbuf);
            puts_uart(3, "\r\n");

            stringToLcd(kbuf);

//            puts_uart(3, "'\n\r\nResuming prints...\n\r");
        }

        /* Receive char to be TX */
        if ( xQueueReceive(uart_txq, &ch, 1000) == pdPASS )
        {
            putc_uart(3, ch);
        }

//        gpio_toggle(GPIOD,GPIO2);
    }
}

void stringToUart(const char *s) {

    for ( ; *s; ++s ) {
        // blocks when queue is full
        xQueueSend(uart_txq, s, portMAX_DELAY);
    }
}


//#ifdef __cplusplus
//}
//#endif

static void
uart_setup()
{
//    usart_set_baudrate(USART3,19200);
//    usart_set_databits(USART3,8);
//    usart_set_stopbits(USART3,USART_STOPBITS_1);
//    usart_set_mode(USART3,USART_MODE_TX_RX);
//    usart_set_parity(USART3,USART_PARITY_NONE);
//    usart_set_flow_control(USART3,USART_FLOWCONTROL_NONE);
//    usart_enable(USART3);
    open_uart(3, 19200, "8N1", "rw", 0, 0); //аналог верхним 7 строкам

    // Create a queue for data to transmit from UART
    uart_txq = xQueueCreate(256,sizeof(char));

}



int
main(void) {

    gpio_setup();
    uart_setup();

    xTaskCreate(uart_task,"UART",200,NULL,configMAX_PRIORITIES-1,NULL);

    xTaskCreate(checkButtons,"BUTTONS",100,NULL,configMAX_PRIORITIES-1,NULL);

    xTaskCreate(setStateRele,"StateRele",100,NULL,configMAX_PRIORITIES-1,NULL);

//    xTaskCreate(i2c_main_vers1,"i2c_vers2",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(i2c_main_vers2,"i2c_vers2",100,NULL,configMAX_PRIORITIES-1,NULL);

//    xTaskCreate(testUART2,"USART3",100,NULL,configMAX_PRIORITIES-2,NULL);
//    xTaskCreate(testUART1, "UART4", 100, NULL, configMAX_PRIORITIES - 2, NULL);

//    xTaskCreate(testTask1, "LED1", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask3, "LED3", 100, NULL, configMAX_PRIORITIES - 1, NULL);




    vTaskStartScheduler();

    for (;;);

    return 0;
}

