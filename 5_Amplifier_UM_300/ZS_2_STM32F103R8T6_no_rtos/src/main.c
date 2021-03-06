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
//        gc = getc_uart_nb(3);
        gc = getc_uart_nb(1);

        if ( gc != -1 )
        {
            ch = (char)gc;
            const char* toLcd = checkReceivedByteFromMilandr (ch);
//            stringToLcd(toLcd);
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
        vTaskDelay(pdMS_TO_TICKS(500));

//        stringTo_diagnostic_Usart1("test_diadnostic_USART1 - 500");

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



#if defined useMilandr
static void
uart_task(void *args)
{
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
//            stringToLcd(toLcd);
            stringTo_diagnostic_Usart1(toLcd);
        }

        /* Receive char to be TX */
        if ( xQueueReceive(uart_txq, &ch, 10) == pdPASS )
        {
            putc_uart(3, ch);
        }
    }
}

void stringToUart(const char *s)
{

    for ( ; *s; ++s ) {
        // blocks when queue is full
        xQueueSend(uart_txq, s, portMAX_DELAY);
    }
}
#endif

void stringTo_diagnostic_Usart1(const char *s) {

    for ( ; *s; ++s ) {
        // blocks when queue is full
        xQueueSend(usart_diagnostic_txq, s, portMAX_DELAY);
    }
    xQueueSend(usart_diagnostic_txq, "\n", portMAX_DELAY);
    xQueueSend(usart_diagnostic_txq, "\r", portMAX_DELAY);
}

#ifdef useMilandr
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
 const int delay = 200;
    for (;;)
    {
//        stringTo_diagnostic_Usart1("SendUartCommand - 200");


        stringToUart(toMilandr_BlankCommand);

        vTaskDelay(pdMS_TO_TICKS(delay));

    }
}
#endif


void initOuts()
{
    //Выходные сигналы
            setLedOverheatOut(0);
            setLedOvercutOut(0);
            setLedShortOut(0);
            setLedBreakOut(0);
            setErrorRele(0);

    //Реле и Led
            setReleLine1(0);
            setRele24V(0);
            setReleLine2(0);
            setReleLine3(0);
            setReleLine4(0);
#ifndef useUSART1
            setReleTr1(0);
            setReleTr2(0);
#endif
            setReleTr3(0);
            setReleTr4(0);
            setReadyLed(true);
            setFan(0);
            setMute(true);
            setGAIN(false);
            if (!isPOWER_OFF) stringTo_diagnostic_Usart1("POWER_OK ON");
            if (isPOWER_OFF) stringTo_diagnostic_Usart1("POWER_OK OFF");
            setUpr_IN_078(true);
            setUpr_IN_ST(false);
            setImpedanceRele(false);
            if (isVOLT_UPR) stringTo_diagnostic_Usart1("U bigger THRESHOLD");
            if (!isVOLT_UPR) stringTo_diagnostic_Usart1("U less THRESHOLD");

            if (isCURR1_UPR) stringTo_diagnostic_Usart1("I_1 bigger THRESHOLD");
            if (!isCURR1_UPR) stringTo_diagnostic_Usart1("I_1 less THRESHOLD");

            if (isCURR2_UPR) stringTo_diagnostic_Usart1("I_2 bigger THRESHOLD");
            if (!isCURR2_UPR) stringTo_diagnostic_Usart1("I_2 less THRESHOLD");

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
#if defined useMilandr
    uart_setup();
#endif
    usart1_diadnostic_setup();
    initOuts();

    xTaskCreate(usart1_diagnostic_task,"UART",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(checkInputs,"+InputSignals",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(setOutputs,"+StateRele",100,NULL,configMAX_PRIORITIES-2,NULL);
//    xTaskCreate(digitaPOT,"digitaPOT",200,NULL,configMAX_PRIORITIES-2,NULL);

#ifdef useDisplay
    xTaskCreate(i2c_main_vers2,"i2c_vers2",100,NULL,configMAX_PRIORITIES-2,NULL);
#endif

#if defined useMilandr
    xTaskCreate(uart_task,"UART",100,NULL,configMAX_PRIORITIES-1,NULL);
    xTaskCreate(SendUartCommand,"SendUartBlankCommand",100,NULL,configMAX_PRIORITIES-2,NULL);
//    xTaskCreate(testTask1, "TestSignalls", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testSendUartCommand,"testSendUartCommand",100,NULL,configMAX_PRIORITIES-1,NULL);
#endif

//    xTaskCreate(testImpuls, "testImpuls", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(test_diadnostic_USART1, "USART1", 100, NULL, configMAX_PRIORITIES - 1, NULL);
    xTaskCreate(testTask2, "LED2", 100, NULL, configMAX_PRIORITIES - 1, NULL);
//    xTaskCreate(testTask3, "LED3", 100, NULL, configMAX_PRIORITIES - 1, NULL);

    vTaskStartScheduler();

    for (;;)
    {};

    return 0;

#ifdef __cplusplus
}
#endif

}

