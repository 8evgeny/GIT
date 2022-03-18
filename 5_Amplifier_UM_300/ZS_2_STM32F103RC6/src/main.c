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
gpio_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_USART3);

    gpio_primary_remap(AFIO_MAPR, AFIO_MAPR_USART3_REMAP_PARTIAL_REMAP);
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_PR_TX);
    gpio_set_mode(GPIOC, GPIO_MODE_INPUT,GPIO_CNF_INPUT_FLOAT, GPIO_USART3_PR_RX);

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

    //Входы
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO0); //Кнопка "ИМПЕДАНС"
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO1); //Кнопка "КАЛИБРОВКА"
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO2); //Кнопка "СБРОС ОШИБКИ"
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO3); //Внешний сигнал «Включение 20 - 72V»
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO4); //Внешний сигнал «Импеданс 20 - 72V»
        gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, GPIO5); //Внешний сигнал «Трансляция 20 - 72V»

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
//            puts_uart(3, "'\n\r\nResuming prints...\n\r");
        }

        /* Receive char to be TX */
        if ( xQueueReceive(uart_txq, &ch, 10) == pdPASS )
        {
            putc_uart(3, ch);
        }

        gpio_toggle(GPIOD,GPIO2);
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
        vTaskDelay(pdMS_TO_TICKS(2000));
        stringToUart("Just start typing to enter a line, or..\n\r"
                  "hit Enter first, then enter your input.\n\n\r");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}


//#ifdef __cplusplus
//}
//#endif

static void
uart_setup()
{
//    usart_set_baudrate(USART3,115200);
//    usart_set_databits(USART3,8);
//    usart_set_stopbits(USART3,USART_STOPBITS_1);
//    usart_set_mode(USART3,USART_MODE_TX_RX);
//    usart_set_parity(USART3,USART_PARITY_NONE);
//    usart_set_flow_control(USART3,USART_FLOWCONTROL_NONE);
//    usart_enable(USART3);
    open_uart(3, 115200, "8N1", "rw", 0, 0); //аналог верхним 7 строкам

    // Create a queue for data to transmit from UART
    uart_txq = xQueueCreate(256,sizeof(char));

}

int
main(void) {

    gpio_setup();
    uart_setup();

    xTaskCreate(uart_task,"UART",200,NULL,configMAX_PRIORITIES-1,NULL);
//    xTaskCreate(testUART2,"USART3",100,NULL,configMAX_PRIORITIES-1,NULL);

//    xTaskCreate(testUART1, "UART4", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask1, "LED1", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask3, "LED3", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask4, "LED4", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask5, "LED5", 100, NULL, configMAX_PRIORITIES - 1, NULL);


	vTaskStartScheduler();

	for (;;);

	return 0;
}

