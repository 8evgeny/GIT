#ifndef RS232_H
#define RS232_H

#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_uart.h"
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_it.h"
//#include <array>
//#include "CircularBuffer.h"
#include "cmsis_os.h"
#include "main.h"

//#include "sram.h"
//#include "json.h"




struct RS232;

struct RS232
{
//    enum {
//        OK,
//        DeInitFail,
//        InitFail
//    };

//    UART_HandleTypeDef *uartHandle;

//    static constexpr uint32_t sizeCircularBuffer = 512;
//    CircularBuffer <char, sizeCircularBuffer> ringBuffer;


    volatile FlagStatus uartReadReady ; /*! Status flag about reading data from UART */
    volatile FlagStatus uartWriteReady ; /*! Status flag about writing data to UART */
    volatile _Bool uartErrorCallback ; /*! Status flag about error UART */
    osThreadId readFromUartThreadId, terminateThreadsId;

//    void test();

};

HAL_StatusTypeDef RS232_write_u(uint8_t *buf, uint16_t size);
HAL_StatusTypeDef RS232_write_c(char *buf, uint16_t size);
HAL_StatusTypeDef RS232_read_u(uint8_t *buf, uint16_t size);
HAL_StatusTypeDef RS232_read_c(char *buf, uint16_t size);


void readFromUartThread(void const *arg);


void RS232Init(void);



#endif // RS232_H
