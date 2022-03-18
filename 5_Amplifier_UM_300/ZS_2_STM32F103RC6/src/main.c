#include "main.h"

//#ifdef __cplusplus
//extern "C" {
//#endif

extern void
vApplicationStackOverflowHook(
	xTaskHandle *pxTask,
	signed portCHAR *pcTaskName);

static QueueHandle_t uart_txq;				// TX queue for UART

void
vApplicationStackOverflowHook(xTaskHandle *pxTask,signed portCHAR *pcTaskName)
{
    (void)pxTask;
    (void)pcTaskName;
    for(;;);
}



static void
gpio_setup(void) {

    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);

    if(uart4) rcc_periph_clock_enable(RCC_UART4);

    if(!uart4) gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO_USART1_TX);
    if(uart4) gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO_UART4_TX);
//	gpio_set_mode(GPIOA,GPIO_MODE_OUTPUT_50_MHZ,GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO11);

    if(!uart4) gpio_set_mode(GPIOA,GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT,GPIO_USART1_RX);
    if(uart4) gpio_set_mode(GPIOC,GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT,GPIO_UART4_RX);
//	gpio_set_mode(GPIOA,GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT,GPIO12);

    //Выходы
//        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8); //OUT11 IMP_RELE
//        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9); //OUT12 RELE_TR1
//        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10); //OUT13 RELE_TR2
//        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO11); //OUT14 RELE_TR3
//        gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12); //OUT15 RELE_TR4

//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO0); //OUT18 READY_LED
//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO1); //OUT19 UPR1
//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO2); //OUT20 UPR2

//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9); //OUT17 FAN_UPR
//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10); //OUT1 LED Короткое замыкание
//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO11); //OUT2 LED Обрыв
//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12); //OUT3 LED Перегрузка
//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13); //OUT4 LED Перегрев
//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO14); //OUT5 ERROR_RELE
//        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO15); //OUT6 RELE_LINE1
//        gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9); //OUT10 RELE_24V

    //тестовые задачи - blink
        gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO6);
        gpio_set_mode(GPIOD, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO2);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO1);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
        gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO14);
        gpio_set_mode(GPIOD,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO2);
//        gpio_set_mode(GPIOC,GPIO_MODE_OUTPUT_2_MHZ,GPIO_CNF_OUTPUT_PUSHPULL,GPIO13);

    //Входы
//        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0); //Кнопка "ИМПЕДАНС"
//        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO1); //Кнопка "КАЛИБРОВКА"
//        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO2); //Кнопка "СБРОС ОШИБКИ"
//        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO3); //Внешний сигнал «Включение 20 - 72V»
//        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO4); //Внешний сигнал «Импеданс 20 - 72V»
//        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO5); //Внешний сигнал «Трансляция 20 - 72V»


}


static void
uart_task(void *args) {
    int gc;
    char kbuf[256], ch;

    (void)args;

    if(!uart4) puts_uart(1, "\n\ruart_task() has begun:\n\r");

    for (;;) {
        if(!uart4) gc = getc_uart_nb(1);

        if ( gc != -1 )
        {
            if(!uart4) puts_uart(1, "\r\n\nENTER INPUT: ");

            ch = (char)gc;
            if ( ch != '\r' && ch != '\n' ) {
                /* Already received first character */
                kbuf[0] = ch;
                if(!uart4) putc_uart(1, ch);

                if(!uart4) getline_uart(1, kbuf+1, sizeof kbuf-1);

            } else	{
                /* Read the entire line */
                if(!uart4) getline_uart(1, kbuf, sizeof kbuf);

            }
            if(!uart4)
            {
                puts_uart(1, "\r\nReceived input '");
                puts_uart(1, kbuf);
                puts_uart(1, "'\n\r\nResuming prints...\n\r");
            }

        }

        /* Receive char to be TX */
        if ( xQueueReceive(uart_txq, &ch, 10) == pdPASS )
        {
            if(!uart4) putc_uart(1, ch);

        }

        /* Toggle LED to show signs of life */
        gpio_toggle(GPIOD,GPIO2);
//        gpio_toggle(GPIOC,GPIO13);
    }
}


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

    if(!uart4) open_uart(1, 115200, "8N1", "rw", 0, 0); //Описание в теле функции

    // Create a queue for data to transmit from UART
    uart_txq = xQueueCreate(256,sizeof(char));

}

int
main(void) {

    gpio_setup();
    uart_setup();

    xTaskCreate(uart_task,"UART",200,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(testUART1, "testUART", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(testUART2,"DEMO",100,NULL,configMAX_PRIORITIES-2,NULL);

    xTaskCreate(testTask1, "LED1", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask3, "LED3", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask4, "LED4", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask5, "LED5", 100, NULL, configMAX_PRIORITIES - 1, NULL);


	vTaskStartScheduler();

	for (;;);

	return 0;
}

