/*!
 \file gpio_stm32f7xx.h

*/

#ifndef GPIO_H
#define GPIO_H

#include <cstdint>
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_gpio.h"
#include "stm32f7xx_hal_dma.h"
#include <array>
#include "CircularBuffer.h"
#include "cmsis_os.h"
#include "../AUDIO/tlv320aic3254.h"

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

    GPIO_InitTypeDef *gpioInit; /**< The variable is pointer to GPIO Handle Type */
    bool bedCfgFlag = false; /*! The variable stores forbidden combination presence flag */
    volatile bool testFlag = false; /*! The variable stores flag of the test start mode*/
    osThreadId createTestTaskThreadId, /*!< The variable stores ID of the createTestTaskThread */
               testThreadId; /*!< The variable stores ID of the testThread */
    volatile int8_t dacDriverGainValue = 0;  /*!< The variable stores a value of the driver gain */


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
/*!
 \brief The OS thread function that changes the speaker volume

 \fn changeVolumeThread
 \param arg not used
*/
void changeVolumeThread(void const *arg);

#ifdef __cplusplus
}
#endif

#endif // GPIO_H
