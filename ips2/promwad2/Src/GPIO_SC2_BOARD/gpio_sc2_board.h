/*!
 \file gpio_stm32f7xx.h

*/

#ifndef GPIO_SC2_BOARD_H
#define GPIO_SC2_BOARD_H

#include <cstdint>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_dma.h"
#include <array>
#include "CircularBuffer.h"
#include "cmsis_os.h"
#include "../AUDIO/tlv320aic3254.h"

/*!
 \brief The structure that stores variables for configuration keys of the keyboard

 \class Keyboard gpio_stm32f7xx.h "gpio_stm32f7xx.h"
*/
struct alignas(4) Keyboard {

    uint16_t ledPin = 0;    /*!< The field stores a pin number to switch LED */
    uint16_t timeOn = 0,    /**< The field stores time in millisec when LED is On*/
             timeOff = 0;   /**< The field stores time in millisec when LED is Off*/
    bool ledState = false,  /**<  The field stores state of the LED (turns on/off LED)*/
         timeStart = false; /**< The field stores the start flag of the switching mode */
    uint8_t count = 0;      /**< The counter is used to control the switching mode of the LEDs*/
    int8_t reiterationNum = -1; /**< Number of reiteration is used to set the reiteration number of the switching mode */
};

/*!
 \brief The struct stores numbers of key on the keypad and values those ones

 \class sPin gpio_stm32f7xx.h "gpio_stm32f7xx.h"
*/
struct sPin {
    uint16_t i = 0, /*!< The number of key on the keypad */
             n = 0; /*!< The value of the keypad key */
};

/*!
  \brief The structure stores custom protocol fields

  \class Package can_stm32f1xx.h "can_stm32f1xx.h"
 */
struct PackageRx {

    /*custom protocol fields*/
    uint8_t packetType,     /**< packet type */
            payloadData;
};

/*!
 \brief Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class GPIO gpio.h "gpio.h"
*/

class GPIO
{
private:
    static GPIO * p_instance; /*!< TODO: describe */

    /*!
     \brief Base constructor. A default constructor.

     \fn GPIO
    */
    GPIO();

    /*!
     \brief Copy constructor.

     \fn GPIO
     \param
    */
    GPIO(const GPIO&);

    /*!
     \brief Copy assignment operator

     \fn operator =
     \param
     \return GPIO &operator
    */
    GPIO& operator=(GPIO&);

public:

    /*!
     \brief The enumeration stores value of packet type field

     \enum
    */
    enum {
        button = 0,
        data,
        errString
    };

    /*!
     \brief It returns the sole instance of the class.

     \fn getInstance
     \return GPIO. The public static operation can be accessed easily by using the class name and operation name.
    */
    static GPIO * getInstance();

    /*!
     * \brief The function allow to get data from CFG resistors
     * \param none
     * \return none
     */
    uint16_t getCFG(void);

    /*!
     * \brief The function allow to increase volume
     * \param none
     * \return none
     */
    void upVolume(void);

    /*!
     * \brief The function allow to reduce volume
     * \param none
     * \return none
     */
    void downVolume(void);

    /*!
      \brief GPIO Test Function
      \param None
      \retval None
      */
    void test(void);

    /*!
     \brief The function allow to configure LEDs of the keyboard

     \fn configLed
     \param ledNumber LED number of the keyboard
     \param ledOn controls the LED switching
     \param timeOn allows to set the LED light time
     \param timeOff allows to set time when LED doesn't light
     \param repeatNum allows to set repetitions number
    */
    void configLed(uint8_t ledNumber,
                   bool ledOn = true,
                   uint32_t timeOn = 0,
                   uint32_t timeOff = 0,
                   uint8_t repeatNum = 0);

    /*!
     \brief The function initializes LEDs of the keyboard

     \fn initLEDs
    */
    void initLEDs();

    constexpr static uint8_t keysNum = 6; /*!< Number of the keys on the keyboard */
    osMessageQId message_q_id;  /*!< ID for the message queue */
    alignas(4) std::array <sPin, keysNum>sPinArray; /*!< The array stores sPin struct elements */
    alignas(4) Keyboard aLeds[keysNum]; /*!< The array stores Keyboard struct elements */

    GPIO_InitTypeDef *gpioInit; /**< The variable is pointer to GPIO Handle Type */
    bool bedCfgFlag = false; /*! The variable stores forbidden combination presence flag */
    volatile bool testFlag = false; /*! The variable stores flag of the test start mode*/
    osThreadId createTestTaskThreadId, /*!< The variable stores ID of the createTestTaskThread */
               testThreadId, /*!< The variable stores ID of the testThread */
               trackRingBufferThreadId;
    volatile int8_t dacDriverGainValue = 0;  /*!< The variable stores a value of the driver gain */

    constexpr static uint16_t sizeCircularBuffer = 1040; /**< The variable stores size of CAN ring buffer */

    CircularBuffer <PackageRx, sizeCircularBuffer> ringBufferRx; /**< RX CAN ring buffer */
//    CircularBuffer <PackageRx, sizeCircularBuffer> ringBufferTx; /**< TX CAN ring buffer */

    osMutexId mutexRingBufferRx_id; /**< The variable that stores ID of the mutexRingBufferRx */
//              mutexRingBufferTx_id;

    PackageRx packageRx;


};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 \brief The function initializes GPIO

 \fn GPIOInit
*/
void GPIOInit(void);
/*!
 \brief The callback function for the timer

 \fn timerCallback
 \param arg not used
*/
void timerCallback(void const *arg);
/*!
 \brief The OS thread function that switches LEDs depending on
        operating mode

 \fn switchLEDsThread
 \param arg not used
*/
[[ noreturn ]] void switchLEDsThread(void const *arg);
/*!
 \brief The OS thread function that reads button click data from the keyboard

 \fn readButtonThread
 \param arg not used
*/
[[ noreturn ]] void readButtonThread(void const *arg);

#ifdef __cplusplus
}
#endif

#endif // GPIO_SC2_BOARD_H
