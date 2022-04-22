#ifndef RS232_H
#define RS232_H

#include <cstdint>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_uart.h"
#include "stm32h7xx_hal_dma.h"
#include "stm32h7xx_it.h"
#include <array>
#include "CircularBuffer.h"
#include "cmsis_os.h"


//#include "sram.h"
//#include "json.h"



/*!
 * \brief The Console class for streaming debug information
 */
class Console
{
public:
    Console() {}
    ~Console() {}

    friend Console &operator<<(Console &console, char data);
    friend Console &operator<<(Console &console, unsigned char data);

    friend Console &operator<<(Console &console, short data);
    friend Console &operator<<(Console &console, unsigned short data);

    friend Console &operator<<(Console &console, int data);
    friend Console &operator<<(Console &console, unsigned int data);

    friend Console &operator<<(Console &console, long int data);
    friend Console &operator<<(Console &console, unsigned long int data);

    friend Console &operator<<(Console &console, const char *data);
    friend Console &operator<<(Console &console, const unsigned char *data);

    friend Console &operator<<(Console &console, const std::string &data);

    Console &operator<<(Console & (*data)(Console &));


    friend Console &operator>>(Console &console, std::string &data);
};

class RS232;



class RS232Destroyer
{
private:
    RS232 *pInstance; /*! Pointer to instance */
public:

    ~RS232Destroyer();


    void initialize(RS232 *p);
};


class RS232
{
private:
    static RS232 *pInstance;
    static RS232Destroyer destroyer;

    enum {
        OK,
        DeInitFail,
        InitFail
    };

protected:
    RS232();

    RS232(const RS232 &);

    RS232 &operator=(RS232 &);


    ~RS232() { }
    friend class RS232Destroyer;

public:
    Console term;
    UART_HandleTypeDef *uartHandle;

    static constexpr uint32_t sizeCircularBuffer = 512;
    CircularBuffer <char, sizeCircularBuffer> ringBuffer;

    static RS232 &getInstance();

    void  C_test(RS232* p);
    static RS232 & C_getInstance(RS232* p);


    /*!
     * \brief read data from UART
     * \param buf Buffer for saving data
     * \param size Size of buffer for saving data
     * \param saveDataToRingBuffer Use a ring buffer
     * \return Status of reading
     */
    HAL_StatusTypeDef read(uint8_t *buf, uint16_t size);

    /*!
    * \brief read data from UART
    * \param buf Buffer for saving data
    * \param size Size of buffer for saving data
    * \param saveDataToRingBuffer Use a ring buffer
    * \return Status of reading
    */
    HAL_StatusTypeDef read(char *buf, uint16_t size);

    /*!
     * \brief read data from UART
     * \param buf Buffer for saving data
     * \param size Size of buffer for saving data
     * \param saveDataToRingBuffer Use a ring buffer
     * \return Status of reading
     */
    HAL_StatusTypeDef read(uint8_t *buf, uint16_t size, uint32_t timeout, bool saveDataToRingBuffer =  false);

    /*!
     * \brief read data from UART
     * \param buf Buffer for saving data
     * \param size Size of buffer for saving data
     * \param saveDataToRingBuffer Use a ring buffer
     * \return Status of reading
     */
    HAL_StatusTypeDef read(char *buf, uint16_t size, uint32_t timeout, bool saveDataToRingBuffer = false);

    HAL_StatusTypeDef read(std::string &str, uint32_t timeout, bool saveDataToRingBuffer =  false);

    /*!
     * \brief write data to UART
     * \param buf Buffer for writing data
     * \param size Size of buffer for writing data
     * \return Status of writing
     */
    HAL_StatusTypeDef write(uint8_t *buf, uint16_t size);

    /*!
     * \brief write data to UART
     * \param buf Buffer for writing data
     * \param size Size of buffer for writing data
     * \return Status of writing
     */
//    HAL_StatusTypeDef write(char *buf, uint16_t size);

    volatile FlagStatus uartReadReady = RESET; /*! Status flag about reading data from UART */
    volatile FlagStatus uartWriteReady = RESET; /*! Status flag about writing data to UART */
    volatile bool uartErrorCallback = false; /*! Status flag about error UART */
    osThreadId readFromUartThreadId,
               terminateThreadsId;

    void test();

};


/*!
 * \brief The OS thread function reads data from the RS232
 *
 * \fn readFromUartThread
 * \return arg not used
 */
void readFromUartThread(void const *arg);

#ifdef __cplusplus
extern "C" {
#endif

void RS232Init(void);


#ifdef __cplusplus
}
#endif

#endif // RS232_H
