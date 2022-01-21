#ifndef RS232_H
#define RS232_H

#include <cstdint>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_uart.h"
#include "stm32f7xx_hal_dma.h"
#include "stm32f7xx_it.h"
#include <array>
#include "CircularBuffer.h"
#include "sram.h"
#include "cmsis_os.h"
#include "json.h"

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

/*!
 \brief It is designed to automatically destroy RS232

 \class RS232Destroyer rs232.h "rs232.h"
*/
class RS232Destroyer
{
private:
    RS232 *pInstance; /*! Pointer to instance */
public:
    /*!
     \brief Base destructor.

     \fn ~RS232Destroyer
    */
    ~RS232Destroyer();

    /*!
     \brief Initialization for RS232.

     \fn initialize
     \param p A pointer to instance
    */
    void initialize(RS232 *p);
};

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class RS232 rs232.h "rs232.h"
*/
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

    /*!
     \brief Base constructor. A default constructor.

     \fn RS232
    */
    RS232();

    /*!
     \brief Copy constructor.

     \fn RS232
     \param
    */
    RS232(const RS232 &);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return RS232 &operator
    */
    RS232 &operator=(RS232 &);

    /*!
     \brief Base destructor.

     \fn ~RS232
    */
    ~RS232() { }
    friend class RS232Destroyer;
public:
    Console term;
    UART_HandleTypeDef *uartHandle;

    static constexpr uint32_t sizeCircularBuffer = 512;
    CircularBuffer <char, sizeCircularBuffer> ringBuffer;

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return RS232. The public static operation can be accessed easily by using the class name and operation name.
    */
    static RS232 &getInstance();

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

    /*!
     * \brief Simple test for RS232
     *
     * \fn test
     */
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

/*!
 * \brief RS232Init Initialization for UART (RS232)
 */
void RS232Init(void);


#ifdef __cplusplus
}
#endif

#endif // RS232_H
