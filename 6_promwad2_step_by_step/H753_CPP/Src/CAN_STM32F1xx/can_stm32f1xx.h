/*!
 \file can_stm32f1xx.h

*/
#ifndef CAN_STM32F1XX_H
#define CAN_STM32F1XX_H

#include <cstdint>
#include "stm32f1xx_hal.h"
#include "CircularBuffer.h"
#include "packageClass.h"
#include "cmsis_os.h"
#include "../Debug/debug.h"
#include <vector>

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class CAN can_stm32f1xx.h "can_stm32f1xx.h"
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

    //! The structure stores value of packet type field.

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
     * \return Status of writing
     */
    HAL_StatusTypeDef sendData();

    /*!
      \brief CAN1 Test Function

      \fn test
      */
    void test();

    /*!
      \brief The function creates CAN package to send
      \param payload is pointer to the payload to send by CAN bus
      \return None
     */
    void createPackage( uint8_t payload );

    /*!
      \brief The function sends created message to the CAN buffer

      \fn sendMessage
      \param None
      \retval None
      */
    void sendMessage();

    /*!
      \brief The function reads data from Mail Queue to the ring buffer

      \fn readToRingBuffer
      \param None
      \return None
     */
    void readToRingBuffer();

    CAN_HandleTypeDef *canHandle; /**< The variable is pointer to CAN Handle Type */
    constexpr static uint16_t sizeCircularBuffer = 10; /**< The variable stores size of CAN ring buffer */
    CircularBuffer <uPackageRx, sizeCircularBuffer> ringBufferRx; /**< RX CAN ring buffer */
    CircularBuffer <uPackageTx, sizeCircularBuffer> ringBufferTx; /**< TX CAN ring buffer */

    volatile FlagStatus canReadReady = RESET; /**< Status flag about reading data from CAN */
    volatile FlagStatus canWriteReady = RESET; /**< Status flag about writing data to CAN */
    volatile bool canErrorCallback = false; /**< Status flag about error CAN */

    uPackageTx packageTx;          /*!< The object of the PackageTx union */
    uPackageRx packageRx;          /*!< The object of the PackageRx union */

    osThreadId sendToCanThreadId; /*!< The thread ID of the sendToCanThread */
    osMailQId  mailID;  /**< Mail queue ID */
    osEvent evt;   /**< Event structure contains detailed information about an event. */
    osMutexId mutexRingBufferRx_id, /**< The variable that stores ID of the mutexRingBufferRx */
              mutexRingBufferTx_id; /**< The variable that stores ID of the mutexRingBufferTx */
};

#ifdef __cplusplus
extern "C" {
#endif


/*!
 \brief Initialization for CAN bus.

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

#endif // CAN_STM32F1XX_H
