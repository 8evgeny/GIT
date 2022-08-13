#ifndef GPIO_SC2_BOARD_H
#define GPIO_SC2_BOARD_H

#include <cstdint>
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_gpio.h"
#include "stm32h7xx_hal_dma.h"
#include <array>
#include "CircularBuffer.h"
#include "cmsis_os.h"
extern uint8_t keysNum;
struct alignas(4) Keyboard {

    uint16_t ledPin = 0;    /*!< The field stores a pin number to switch LED */
    uint16_t timeOn = 0,    /**< The field stores time in millisec when LED is On*/
             timeOff = 0;   /**< The field stores time in millisec when LED is Off*/
    bool ledState = false,  /**<  The field stores state of the LED (turns on/off LED)*/
         timeStart = false; /**< The field stores the start flag of the switching mode */
    uint8_t count = 0;      /**< The counter is used to control the switching mode of the LEDs*/
    int8_t reiterationNum = -1; /**< Number of reiteration is used to set the reiteration number of the switching mode */
};

struct sButton {
    uint16_t i = 0, /*!< The number of key on the keypad */
             n = 0; /*!< The value of the keypad key */
};

struct PackageRx {

    /*custom protocol fields*/
    uint8_t packetType,     /**< packet type */
            payloadData;
};



class GPIO
{
private:
    static GPIO * p_instance;
    GPIO();
    GPIO(const GPIO&);
    GPIO& operator=(GPIO&);

public:
    enum
    {
        button = 0,
        data,
        errString
    };

    enum Color
    {
        RED = 0,
        GREEN
    };

    std::tuple<u_int8_t, u_int8_t, u_int8_t, u_int8_t> fromIndexToReg(u_int8_t i, Color color);
    static GPIO * getInstance();

    void upVolume(void);
    void downVolume(void);

    void test(void);

    void configLed(uint8_t ledNumber,
                   bool ledOn = true,
                   uint32_t timeOn = 0,
                   uint32_t timeOff = 0,
                   uint8_t repeatNum = 0);

    void initLEDS_SC2();
    void initLEDS_SC4();
    void initBUTTONS_SC2();
    void initBUTTONS_SC4();
    uint8_t findBUTTONS_SC4(uint8_t num, uint8_t adr, uint8_t reg);
    void SC4_EXTI_IRQHandler_Config();
    void testLed();
#ifndef SC4
    constexpr static uint8_t keysNum = 6; /*!< Number of the keys on the keyboard */
#endif

    osMessageQId message_q_id;  /*!< ID for the message queue */

//    alignas(4) std::array <sButton, keysNum> buttonArray; /*!< The array stores sButton struct elements */
    alignas(4) sButton * buttonArray =  new sButton[keysNum];
//    alignas(4) Keyboard aLeds[keysNum]; /*!< The array stores Keyboard struct elements */
    alignas(4) Keyboard * aLeds =  new Keyboard[keysNum];

    GPIO_InitTypeDef *gpioInit; /**< The variable is pointer to GPIO Handle Type */
    bool bedCfgFlag = false; /*! The variable stores forbidden combination presence flag */
    volatile bool testFlag = false;
    osThreadId createTestTaskThreadId, testThreadId, trackRingBufferThreadId;

//Громкость
    volatile int8_t dacDriverGainValue = 0xD7; //-20,5
    volatile int8_t dacDriverGainValueMax = 0xFF; //-0,5
    volatile int8_t dacDriverGainValueMin = 0xC0; //-32
    volatile int8_t dacDriverGainValueStep = 0x02;

    constexpr static uint16_t sizeCircularBuffer = 1040;
    CircularBuffer <PackageRx, sizeCircularBuffer> ringBufferRx;
    osMutexId mutexRingBufferRx_id;
    osMutexId mutexBoard_id;
    PackageRx packageRx;
};

#ifdef __cplusplus
extern "C" {
#endif

void GPIOInit(void);
void timerCallback(void const *arg);
[[ noreturn ]] void switchLEDsThread(void const *arg);
[[ noreturn ]] void replaceTimerCallback(void const *arg);
[[ noreturn ]] void readButtonThread(void const *arg);

#ifdef __cplusplus
}
#endif

#endif // GPIO_SC2_BOARD_H
