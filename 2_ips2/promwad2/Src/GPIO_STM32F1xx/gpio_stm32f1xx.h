/*!
 \file gpio_stm32f1xx.h

*/

#ifndef GPIO_H
#define GPIO_H

/* Includes ------------------------------------------------------------------*/
#include <cstdint>
#include <array>
#include "CircularBuffer.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "cmsis_os.h"

/*!
 \brief
        The structure that conteins objects to control LEDs

 \class Keyboard
*/
struct Keyboard {
    uint16_t row;       /**< LEDs row*/
    uint8_t col;        /**< LEDs column*/
    uint32_t timeOn,    /**< time of LED is On*/
             timeOff;   /**< time of LED is Off*/
    bool ledState = false, /**< state of LED (turns on/off a LED)*/
         timeStart = false; /**< state of LED (turns on/off a LED operation mode)*/
    uint8_t count = 0; /**< counter*/
    int8_t reiterationNum = -1; /**< Number of reiteration*/
};

/*!
 \brief
        The structure that conteins column number
        and row number of buttons

 \class ButtonStruct
*/
struct ButtonStruct {
    uint16_t row; /**< Buttons row */
    uint8_t col; /**< Buttons column */
};

/*!
 \brief
        Implementation: ensure that only one instance of the singleton class ever exists
        and provide global access to that instance.

 \class GPIO gpio_stm32f1xx.h "gpio_stm32f1xx.h"
*/

class GPIO
{
private:
    static GPIO * p_instance; /**< The static pointer to the CAN class */

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
     * \brief The function gets config data from CFG resistors
     *
     * \fn getCFG
     * \param None
     * \return configuration resistor data code
     */
    uint8_t getCFG();

    /*!
      \brief GPIO test module

      \fn test
      \param None
      \retval None
      */
    void test();

    /*!
      \brief The function of the combined passage in columns

      \fn runZero
      \param None
      \retval None
      */
    void runZero();

    /*!
     * \brief The function fills the array in order to link the rows and columns
     *
     * \fn initLEDsArray
     * \param none
     * \retval none
     */
    void initLEDsArray();

    /*!
     * \brief The function sets LEDs options
     *
     * \fn ledConfig
     * \param ledNumber number of configurable LED
     * \param ledOn turns On/Off LED
     * \param timeOn sets time when LED is On
     * \param timeOff sets time when LED is Off
     * \retval
     */
    void ledConfig(uint8_t ledNumber,
                   bool ledOn = true,
                   uint32_t timeOn = 0,
                   uint32_t timeOff = 0,
                   uint8_t repeatNum = 0);
    /*!
     * \brief The function turns On the LED
     * depending on the parameter ledOn of ledConfig() function
     *
     * \fn turnOnLeds
     * \param None
     * \retval none
     */
    void switchLEDs();

    /*!
      \brief The function mode detection

      \fn detectMode
      \param None
      \retval None
      */
    void detectMode();

    /*!
      \brief The function reads button state

      \fn readButton
      \param None
      \retval None
      */
    void readButton();

    /*!
      \brief The function sets mode and power to the next board

      \fn modeMaker
      \param None
      \retval None
      */
    void modeMaker(void);

    GPIO_InitTypeDef *gpioInit; /**< The variable is pointer to WWDG Handle Type */
    std::array<Keyboard, 32> aLeds; /*!< The array conteins the parameters of 32 buttons */

    int8_t iter = 0;   /*!< Iterator for the keyboard columns */
    uint8_t mode = 0;           /*!< The variable to store mode setting  */
    ButtonStruct button;

    osMessageQId  queue;        /*!< ID for the message queue declaration */
    osEvent evt;               /*!< Variable to get access to the message queue data   */

    /* IDs for the threads declaration */
    osThreadId runZeroThreadId, /*!< The thread ID of the runZeroThread */
               readButtonThreadId, /*!< The thread ID of the readButtonThread */
               createTestThreadId, /*!< The thread ID of the createTestThread */
               testThreadId; /*!< The thread ID of the testThread */

    uint8_t buttonIsPressed; /*!< The variable stores number of pressed button*/
    bool bedCfgFlag = false, /*!< The variable stores forbidden combination presence flag */
         testFlag = false; /*!< The variable stores flag of the test start mode*/

};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Initialization for GPIO
 *
 * \fn GPIOInit
 */
void GPIOInit(void);

#ifdef __cplusplus
}
#endif

#endif // GPIO_H
