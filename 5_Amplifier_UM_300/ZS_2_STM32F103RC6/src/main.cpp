#include "main.h"

//Временно классы отключаю
//#include "CAT5132_digitalPOT.h"
//#include "buttons.h"
//#include "led.h"
//#include "relays.h"
//#include "display.h"
//#include "stm32.h"
//#include "inputSignals.h"
//#include <memory>

#ifdef __cplusplus
extern "C" {
#endif

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
usart1_diadnostic_setup()
{
//    usart_set_baudrate(USART3,19200);
//    usart_set_databits(USART3,8);
//    usart_set_stopbits(USART3,USART_STOPBITS_1);
//    usart_set_mode(USART3,USART_MODE_TX_RX);
//    usart_set_parity(USART3,USART_PARITY_NONE);
//    usart_set_flow_control(USART3,USART_FLOWCONTROL_NONE);
//    usart_enable(USART3);
    open_uart(1, 115200, "8N1", "rw", 0, 0); //аналог верхним 7 строкам

    // Create a queue for data to transmit from UART
    usart_diagnostic_txq = xQueueCreate(256,sizeof(char));

}

static void
usart1_diagnostic_task(void *args) {
    int gc;
    char kbuf[256], ch;

    (void)args;

//    puts_uart(3, "\n\ruart_task() has begun:\n\r");

    for (;;)
    {
        gc = getc_uart_nb(3);

        if ( gc != -1 )
        {
            ch = (char)gc;
            const char* toLcd = checkReceivedByteFromMilandr (ch);
            stringToLcd(toLcd);
            stringTo_diagnostic_Usart1(toLcd);
        }

        /* Receive char to be TX */
        if ( xQueueReceive(usart_diagnostic_txq, &ch, 10) == pdPASS )
        {
            putc_uart(1, ch);
        }
    }
}

static inline void
uart_putc(char ch) {
    usart_send_blocking(USART1, ch);
}

void
test_diadnostic_USART1(void *args __attribute__((unused)))
{
    int c = '0' - 1;
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(200));

        stringTo_diagnostic_Usart1("test\n\r");

        if ( ++c >= 'Z' )
        {
            uart_putc(c);
            uart_putc('\r');
            uart_putc('\n');
            c = '0' - 1;
        } else
        {
            uart_putc(c);
        }
    }
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
            ch = (char)gc;
            const char* toLcd = checkReceivedByteFromMilandr (ch);
            stringToLcd(toLcd);
            stringTo_diagnostic_Usart1(toLcd);
        }

        /* Receive char to be TX */
        if ( xQueueReceive(uart_txq, &ch, 10) == pdPASS )
        {
            putc_uart(3, ch);
        }
    }
}

void stringToUart(const char *s) {

    for ( ; *s; ++s ) {
        // blocks when queue is full
        xQueueSend(uart_txq, s, portMAX_DELAY);
    }
}

void stringTo_diagnostic_Usart1(const char *s) {

    for ( ; *s; ++s ) {
        // blocks when queue is full
        xQueueSend(usart_diagnostic_txq, s, portMAX_DELAY);
    }
    xQueueSend(usart_diagnostic_txq, "\n", portMAX_DELAY);
    xQueueSend(usart_diagnostic_txq, "\r", portMAX_DELAY);
}


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
    open_uart(3, 38400, "8N1", "rw", 0, 0); //аналог верхним 7 строкам

    // Create a queue for data to transmit from UART
    uart_txq = xQueueCreate(256,sizeof(char));

}

void SendUartCommand(void *args __attribute((unused)))
{
    int delay = 500;
    for (;;)
    {
        stringTo_diagnostic_Usart1("test");


        stringToUart(toMilandr_BlankCommand);
        vTaskDelay(pdMS_TO_TICKS(delay));

    }
}

int main() {

//Временно классы отключаю
//    auto pot = std::shared_ptr<CAT5132_digitalPOT>(new CAT5132_digitalPOT);
//    auto but = std::shared_ptr<Buttons>(new Buttons);
//    auto lcd = std::shared_ptr<Led>(new Led);
//    auto rel = std::shared_ptr<Relays>(new Relays);
//    auto dis = std::shared_ptr<Display>(new Display);
//    auto stm = std::shared_ptr<Stm32>(new Stm32);
//    auto inp = std::shared_ptr<InputSignals>(new InputSignals);

    gpio_setup();
    uart_setup();
    usart1_diadnostic_setup();

    xTaskCreate(uart_task,"UART",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(usart1_diagnostic_task,"UART",100,NULL,configMAX_PRIORITIES-1,NULL);

    xTaskCreate(i2c_main_vers2,"i2c_vers2",100,NULL,configMAX_PRIORITIES-2,NULL);
    xTaskCreate(SendUartCommand,"SendUartCommand",100,NULL,configMAX_PRIORITIES-2,NULL);
    xTaskCreate(checkInputs,"+InputSignals",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(setOutputs,"+StateRele",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(digitaPOT,"digitaPOT",200,NULL,configMAX_PRIORITIES-2,NULL);

//    xTaskCreate(test_diadnostic_USART1, "USART1", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testSendUartCommand,"testSendUartCommand",100,NULL,configMAX_PRIORITIES-1,NULL);
//    xTaskCreate(testTask1, "LED1", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask3, "LED3", 100, NULL, configMAX_PRIORITIES - 1, NULL);

    vTaskStartScheduler();

    for (;;)
    {};

    return 0;

#ifdef __cplusplus
}
#endif

}

