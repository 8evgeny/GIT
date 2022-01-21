/*!
 \file can_stm32f7xx.h

*/

#ifndef CAN_STM32F7XX_H
#define CAN_STM32F7XX_H

#include <cstdint>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#include <array>
#include <vector>
#include "CircularBuffer.h"
#ifndef SC2BOARD
#include "packageClass.h"
#endif
#include "cmsis_os.h"
#include "main.h"



/*!
 \brief Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class CAN can_stm32f7xx.h "can_stm32f7xx.h"
*/

class CAN
{
private:
    static CAN * pInstance; /**< The static pointer to the CAN class */

    /*!
     \brief Base constructor. A default constructor.

     \fn CAN
    */
    CAN();

    /*!
     \brief Copy constructor.

     \fn CAN
     \param
    */
    CAN(const CAN&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return CAN &operator
    */
    CAN& operator=(CAN&);

    uPackageRx tempRxPackage_;

public:

    /*!
     \brief The enumeration stores value of packet type field

     \enum
    */
    enum {
        button = 0,
        data,
        mikeLed,
        errString
    };

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return CAN. The public static operation can be accessed easily by using the class name and operation name.
    */
    static CAN * getInstance();

    /*!
      \brief The function reads data from CAN bus

      \fn read
      \return Status of reading
     */
    HAL_StatusTypeDef read();

    /*!
     * \brief The function sends data by CAN bus
     *
     * \fn sendData
     * \param buf   Buffer for writing data
     * \param size  Size of buffer for writing data
     * \return Status of writing
     */
    HAL_StatusTypeDef sendData();

    /*!
      \brief CAN1 Test Function

      \fn test
      */
    [[ noreturn ]]
    void test();

    /*!
      \brief The function creates CAN message to send by CAN bus
      \param payload is pointer to the payload to send by CAN bus
      \retval None
      */
    void createPackage(uint16_t* payload, uint8_t messageType);

    /*!
      \brief The function creates CAN message and sends data by CAN bus

      \fn sendMessage
      \param None
      \retval None
      */
    void sendMessage();

//    /*!
//      \brief The function reads data from Mail Queue to the ring buffer

//      \fn readToRingBuffer
//      \param None
//      \return None
//     */
//    void readToRingBuffer();

    /*!
      * \brief  The function that sets a LEDs mode of keyboard
      * and sends that to the UI32 board through CAN bus
      * \param  ledNumber
      * \param  ledOn
      * \param  timeOn
      * \param  timeOff
      * \param repeatNum
      * \retval None
      */
    void configLed(uint8_t ledNumber,
                   bool ledOn = true,
                   uint16_t timeOn = 0,
                   uint16_t timeOff = 0,
                   uint8_t repeatNum = 0,
                   uint8_t messageType = data);

    CAN_HandleTypeDef *canHandle; /**< The variable is pointer to the CAN Handle Type */
    constexpr static uint16_t sizeCircularBuffer = 1040; /**< The variable stores size of the CAN ring buffer */

    CircularBuffer <uPackageRx, sizeCircularBuffer> ringBufferRx; /**< RX CAN ring buffer */
    CircularBuffer <uPackageTx, sizeCircularBuffer> ringBufferTx; /**< TX CAN ring buffer */



    volatile FlagStatus canReadReady = RESET; /**< Status flag about reading data from CAN */
    volatile FlagStatus canWriteReady = RESET; /**< Status flag about writing data to CAN */
    volatile bool canErrorCallback = false; /**< Status flag about error CAN */

    uPackageTx packageTx; /*!< The object of the PackageTx type */
    uPackageRx packageRx; /*!< The object of the PackageRx type */

    osThreadId sendToCanThreadId,           /*!< The variable that stores ID of the sendToCanThread */
               trackRingBufferThreadId,     /*!< The variable that stores ID of the trackRingBufferThread */
               readToRingBufferThreadId;    /*!< The variable that stores ID of the readToRingBufferThread */

//    osMailQId  mailID;  /**< The variable that stores ID of the OS mail */
//    osEvent evt; /**< Event structure contains detailed information about an event. */
    osMutexId mutexRingBufferRx_id, /**< The variable that stores ID of the mutexRingBufferRx */
              mutexRingBufferTx_id; /**< The variable that stores ID of the mutexRingBufferTx */
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 \brief The function initializes the CAN bus.

 \fn CANInit
*/
void CANInit();

/*!
 \brief The thread creates a CAN message and sends it by CAN bus.
 \param arg is not used
 \return None
*/
[[ noreturn ]]
void sendCanThread(void const *arg);

/*!
 \brief The thread that reads data from the CAN bus.
 \param arg is not used
 \return None
*/
[[ noreturn ]]
void readCanThread(void const *arg);

#ifdef __cplusplus
}
#endif

#endif // CAN_STM32F7XX_H
