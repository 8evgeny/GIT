/*!
 \file gpio.cpp

 \authors Androshchuk A.V.

 \code
    GPIOInit();

    ...

 \endcode



*/
#include "gpio.h"
#include "i2c.h"
#include <algorithm>
#include <cstring>
#include "rs232.h"
#include "rs232_printf.h"

#ifdef __cplusplus
extern "C" {
#endif

osTimerDef(timer7, timerCallback); /*!< Define the attributes of the timer */

//osMessageQDef(message_q, 1, uint16_t); // Declare a message queue
osMutexDef (mutexRingBufferRx);

// max address number of TLC59116F chips
extern uint8_t TLC59116F_max_address;
// TLC59116F chips addresses
const uint8_t TLC59116F_address[] = {0xC0,0xC2,0xC4,0xC6,0xC8,0xCA};
// TLC59116F chip button registers
const uint8_t TLC59116F_registerLED[] = {0x14,0x15,0x16,0x17};
// Индивидуальная яркость каждого светодиода
const uint8_t TLC59116F_registerBright[] = {0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11};

// max address number of MCP23017 chips
extern uint8_t MCP23017_max_address;
// MCP23017 chips addresses
extern uint8_t MCP23017_address[];
// MCP23017 chip LEDs registers
extern  uint8_t MCP23017_register[];
// TLC59116F chip setup registers values
extern uint8_t TLC59116F_Init_Val[];
// MCP23017 chip setup registers values
extern uint8_t MCP23017_Init_Val[];


//extern osSemaphoreId Netif_LinkSemaphore;
static osTimerId timerId7; /*!< The thread ID of the timer */
constexpr static uint8_t timerDelay = 50;


static GPIO_InitTypeDef GPIO_InitStruct;

/*!
  \brief GPIO Initialization Function
  \param None
  \retval None
  */
void GPIOInit(void)
{

// timerCallback  каллбек
    timerId7 = osTimerCreate( osTimer(timer7), osTimerPeriodic, nullptr); // create timer thread
    if (timerId7)
    {
        osStatus status = osTimerStart (timerId7, timerDelay);   // start timer
        if (status != osOK)  {
            RS232::getInstance().term << "Failed to start [timer]" << "\n";
            while(1);
        }
    }

    HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

}

#ifdef __cplusplus
}
#endif

GPIO::GPIO()
{
    gpioInit = & GPIO_InitStruct;

//ИНИЦИАЛИЗАЦИЯ светодиодов
#ifndef SC4
    initLEDS_SC2();
#endif
#ifdef SC4


#endif

    mutexRingBufferRx_id = osMutexCreate(osMutex(mutexRingBufferRx));
    if (mutexRingBufferRx_id == nullptr)
    {
        while(1)
            RS232::getInstance().term << "Failed to create [mutexRingBufferRx]" << "\n";
    }

//ИНИЦИАЛИЗАЦИЯ РЫЧАГОВ (кнопок)
//    message_q_id = osMessageCreate(osMessageQ(message_q), NULL);

#ifndef SC4
    initBUTTONS_SC2();
#endif
#ifdef SC4
    initBUTTONS_SC4();
#endif

}

GPIO *GPIO::getInstance()
{
    if (!p_instance) p_instance = new GPIO();
    return p_instance;
}

GPIO *GPIO::p_instance = nullptr;

/*!
  \brief Function of getting board configuration
  \param None
  \retval None
  */

/*!
 * \brief Volume up function
 * \param none
 * \return none
 */
void GPIO::upVolume(void)
{
    //Empty
}

/*!
 * \brief Volume down function
 * \param none
 * \return none
 */
void GPIO::downVolume(void)
{
    //Empty
}

/*!
  \brief GPIO Test Function
  \param None
  \retval None
  */
void GPIO::test(void)
{

}

void GPIO::configLed(uint8_t ledNumber,
                     bool ledOn,
                     uint32_t timeOn,
                     uint32_t timeOff,
                     uint8_t repeatNum)
{
term1("**** configLed ledNumber") term(ledNumber)
    --ledNumber;
    if (timeOn < 50 && timeOff < 50)
    {
        aLeds[ledNumber].ledState = ledOn;
        aLeds[ledNumber].timeStart = false;
        aLeds[ledNumber].timeOn = timeOn;
        aLeds[ledNumber].timeOff = timeOff;
    } else
    {
        aLeds[ledNumber].timeOn = timeOn;
        aLeds[ledNumber].timeOff = timeOff;
        aLeds[ledNumber].timeStart = ledOn;
        aLeds[ledNumber].ledState = ledOn;
        if (repeatNum > 0)
            aLeds[ledNumber].reiterationNum = repeatNum * 2;
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void timerCallback(void const *arg)
{
    (void)arg;
    for (uint8_t i = 0; i < GPIO::keysNum; i++) {
        if (GPIO::getInstance()->aLeds[i].timeStart)
        {
            GPIO::getInstance()->aLeds[i].count += 1;
            if(GPIO::getInstance()->aLeds[i].ledState == false && GPIO::getInstance()->aLeds[i].count >= GPIO::getInstance()->aLeds[i].timeOff/timerDelay)
            {
                GPIO::getInstance()->aLeds[i].ledState = true;
                GPIO::getInstance()->aLeds[i].count = 0;
                if (GPIO::getInstance()->aLeds[i].reiterationNum > 0)
                    GPIO::getInstance()->aLeds[i].reiterationNum -= 1;
                if (GPIO::getInstance()->aLeds[i].reiterationNum == 0) {
                    GPIO::getInstance()->aLeds[i].timeStart = false;
                    GPIO::getInstance()->aLeds[i].reiterationNum -= 1;
                }
            } else if (GPIO::getInstance()->aLeds[i].ledState == true && GPIO::getInstance()->aLeds[i].count >= GPIO::getInstance()->aLeds[i].timeOn/timerDelay)
            {
                GPIO::getInstance()->aLeds[i].ledState = false;
                GPIO::getInstance()->aLeds[i].count = 0;
                if (GPIO::getInstance()->aLeds[i].reiterationNum > 0)
                    GPIO::getInstance()->aLeds[i].reiterationNum -= 1;
                if (GPIO::getInstance()->aLeds[i].reiterationNum == 0) {
                    GPIO::getInstance()->aLeds[i].timeStart = false;
                    GPIO::getInstance()->aLeds[i].reiterationNum -= 1;
                }
            }
        }
    }
}

#ifndef SC4 //Два потока для SC2 - светодиоды и рычаги
[[ noreturn ]]
void switchLEDsThread(void const *arg)
{
    (void)arg;
osDelay(200);
term("--- switchLEDsThread ---")
    while(true)
    {
        for(uint8_t i = 0; i < 6; ++i)
        {
            if (GPIO::getInstance()->aLeds[i].ledState) // Включаем пин
            {
                if(i == 0) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_SET);
                if(i == 1) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET);
                if(i == 2) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_SET);
                if(i == 3) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_SET);
                if(i == 4) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_SET);
                if(i == 5) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET);
            } else
            {
                if(i == 0) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
                if(i == 1) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
                if(i == 2) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
                if(i == 3) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
                if(i == 4) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
                if(i == 5) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
            }
        }
        osDelay(1);
    }
}

[[ noreturn ]]
void readButtonThread(void const *arg)
{
    (void)arg;
    PackageRx tempPack;
    tempPack.packetType = GPIO::getInstance()->button;

osDelay(4000);
term("--- readButtonThread ---")
    while(true)
    {
        for (uint8_t i = 0; i < GPIO::getInstance()->buttonArray.size() ; ++i)
        {
            uint16_t n = GPIO::getInstance()->buttonArray[i].n;
//            k = GPIO::getInstance()->buttonArray[i].i;
            if (HAL_GPIO_ReadPin(GPIOG, GPIO::getInstance()->buttonArray[i].n) == GPIO_PIN_SET)
            {
                osDelay(50);
                if (HAL_GPIO_ReadPin(GPIOG, GPIO::getInstance()->buttonArray[i].n)  == GPIO_PIN_SET)
                {

//term1("Pressed button: ") term(std::to_string(i + 1))

                //Включаю Led
//                if (i < 3 )  HAL_GPIO_WritePin(GPIOG, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_SET);
//                if (i >= 3 ) HAL_GPIO_WritePin(GPIOC, GPIO::getInstance()->aLeds[i ].ledPin, GPIO_PIN_SET);

                    n = GPIO::getInstance()->buttonArray[i].i;
                    tempPack.payloadData = n;

                    osMutexWait(GPIO::getInstance()->mutexRingBufferRx_id, osWaitForever);
                    GPIO::getInstance()->ringBufferRx.push(tempPack);
                    osMutexRelease(GPIO::getInstance()->mutexRingBufferRx_id);

//                    osMessagePut(GPIO::getInstance()->message_q_id, n, osWaitForever);
                }
            }
            else
            {
                //Гашу Led
//                if (i < 3 ) HAL_GPIO_WritePin(GPIOG, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_RESET);
//                if (i >= 3 ) HAL_GPIO_WritePin(GPIOC, GPIO::getInstance()->aLeds[i ].ledPin, GPIO_PIN_RESET);
            }
        }
        osDelay(1);
    }
}
#endif

#ifdef SC4 //Два потока для SC4 - светодиоды и кнопки
[[ noreturn ]]
void switchLEDsThread(void const *arg)
{
    (void)arg;
osDelay(200);
term("--- switchLEDsThread ---")

    i2cInitBoard();
    for (uint8_t i = 0; i < TLC59116F_max_address; i++)
    {
        I2C::getInstance()->writeRegister(TLC59116F_address[i], 0x00, 0x00, false);
    }

    //Яркость
    for (uint8_t i = 0; i < TLC59116F_max_address; i++)
    {
       // зеленый  TLC59116F_registerBright
         for (uint8_t j = 0; j < 16; j++)
         {//Индивидуальная яркость
             I2C::getInstance()->writeRegister(TLC59116F_address[i], TLC59116F_registerBright[j], 0x30, false);
         }
    }
    uint8_t testBlink = 0;
    uint8_t testRun = 0;
    bool endTests = false;
    while(true)
    {
        while(testBlink < 2)
        {//Тестовое попеременное моргание
            for (uint8_t i = 0; i < TLC59116F_max_address; i++)
            {
                for (uint8_t j = 0; j < 4; j++)
                { // красный
                I2C::getInstance()->writeRegister(TLC59116F_address[i], TLC59116F_registerLED[j], 0x11, false);
                //                               4 столбца LED (для 32)  4 регистра по 2*2 светодиода
                }
            }
            osDelay(150);
            for (uint8_t i = 0; i < TLC59116F_max_address; i++)
            {
                for (uint8_t j = 0; j < 4; j++)
                { // зеленый  TLC59116F_registerBright
                I2C::getInstance()->writeRegister(TLC59116F_address[i], TLC59116F_registerLED[j], 0x88, false);
                }
            }
            osDelay(150);
            ++testBlink;
        }//end тестовое моргание


        while(testRun < 1)
        { //Бегущий огонек
            uint8_t adr,reg,num;
            for (uint8_t i = 0; i < TLC59116F_max_address * 8; i++)
            {
               std::tie (adr, reg, num)  = GPIO::getInstance()->fromIndexToReg(i, GPIO::getInstance()->RED);
               I2C::getInstance()->writeRegister(adr, reg, num, false);
               osDelay(30);
            }
            for (uint8_t i = 0; i < TLC59116F_max_address * 8; i++)
            {
               std::tie (adr, reg, num)  = GPIO::getInstance()->fromIndexToReg(i, GPIO::getInstance()->GREEN);
               I2C::getInstance()->writeRegister(adr, reg, num, false);
               osDelay(30);
            }
            ++testRun;
        }//End Бегущий огонек

        //Гасим все
        if(!endTests)
        {
            for (uint8_t i = 0; i < TLC59116F_max_address; i++)
            {
                for (uint8_t j = 0; j < 4; j++)
                {
                I2C::getInstance()->writeRegister(TLC59116F_address[i], TLC59116F_registerLED[j], 0x00, false);
                }
            }
            endTests = true;
        }

        //Начало основного цикла управления LED
        for(uint8_t i = 0; i < TLC59116F_max_address * 8; ++i)
        {
            if (GPIO::getInstance()->aLeds[i].ledState)
            {// Включаем пин



            } else
            {// Гасим пин


            }
        }

        osDelay(1);
    }
}

[[ noreturn ]]
void readButtonThread(void const *arg)
{
    (void)arg;
    PackageRx tempPack;
    tempPack.packetType = GPIO::getInstance()->button;

osDelay(4000);
term("--- readButtonThread ---")
    while(true)
    {


        osDelay(1);
    }
}
#endif

#ifdef __cplusplus
}
#endif

void GPIO::initLEDS_SC2()
{
#ifndef SC4
//    for (uint8_t i = 0, j = 6; i < 6; i++, j++) {
//        if (i == 3) j = 10;
//        aLeds[i].ledPin = aPin[j];
//    }
    aLeds[0].ledPin = GPIO_PIN_6;
    aLeds[1].ledPin = GPIO_PIN_10;
    aLeds[2].ledPin = GPIO_PIN_7;
    aLeds[3].ledPin = GPIO_PIN_11;
    aLeds[4].ledPin = GPIO_PIN_8;
    aLeds[5].ledPin = GPIO_PIN_12;
#endif
}


void GPIO::initBUTTONS_SC2()
{
#ifndef SC4
    buttonArray[0].i = 1;     buttonArray[0].n = GPIO_PIN_11;
    buttonArray[1].i = 2;     buttonArray[1].n = GPIO_PIN_12;
    buttonArray[2].i = 3;     buttonArray[2].n = GPIO_PIN_10;
    buttonArray[3].i = 4;     buttonArray[3].n = GPIO_PIN_13;
    buttonArray[4].i = 5;     buttonArray[4].n = GPIO_PIN_9;
    buttonArray[5].i = 6;     buttonArray[5].n = GPIO_PIN_14;
#endif
}

void GPIO::initBUTTONS_SC4()
{
#ifdef SC4


#endif
}

#ifdef __cplusplus
extern "C" {
#endif

/*!
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {
        if (GPIO_Pin == GPIO_PIN_5)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
            GPIO::getInstance()->testFlag = true;
            osSignalSet(GPIO::getInstance()->createTestTaskThreadId, 0x03);
        }

    //    else if (GPIO_Pin == GPIO_PIN_14) {
    //        if (GPIO::getInstance()->dacDriverGainValue < 29)
    //            ++GPIO::getInstance()->dacDriverGainValue;
    //    } else if (GPIO_Pin == GPIO_PIN_15) {
    //        if (GPIO::getInstance()->dacDriverGainValue > -6)
    //            --GPIO::getInstance()->dacDriverGainValue;
    //    }

//        if (GPIO_Pin == GPIO_PIN_2) {
//            osSemaphoreRelease(Netif_LinkSemaphore);
//        }

    }

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
/*!
  * @brief  This function handles external lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
//    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}

#ifdef __cplusplus
}
#endif

std::tuple<u_int8_t, u_int8_t, u_int8_t> GPIO::fromIndexToReg(u_int8_t i, GPIO::Color color)
{
    uint8_t reg = 0;
    std::tuple<u_int8_t, u_int8_t, u_int8_t> ret{0,0,0};
    switch (i)
    { //Возвращаем адрес микросхемы и номер регистра
        case 0: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC0, 0x14, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC0, 0x14, reg ); break;
        case 1: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC0, 0x14, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC0, 0x14, reg ); break;
        case 2:
            reg = I2C::getInstance()->readRegister(0xC0, 0x15, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC0, 0x15, reg ); break;
        case 3:
            reg = I2C::getInstance()->readRegister(0xC0, 0x15, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC0, 0x15, reg ); break;
        case 4:
            reg = I2C::getInstance()->readRegister(0xC0, 0x16, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC0, 0x16, reg ); break;
        case 5:
            reg = I2C::getInstance()->readRegister(0xC0, 0x16, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC0, 0x16, reg ); break;
        case 6:
            reg = I2C::getInstance()->readRegister(0xC0, 0x17, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC0, 0x17, reg ); break;
        case 7:
            reg = I2C::getInstance()->readRegister(0xC0, 0x17, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC0, 0x17, reg ); break;

        case 8: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC2, 0x14, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC2, 0x14, reg ); break;
        case 9: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC2, 0x14, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC2, 0x14, reg ); break;
        case 10:
            reg = I2C::getInstance()->readRegister(0xC2, 0x15, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC2, 0x15, reg ); break;
        case 11:
            reg = I2C::getInstance()->readRegister(0xC2, 0x15, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC2, 0x15, reg ); break;
        case 12:
            reg = I2C::getInstance()->readRegister(0xC2, 0x16, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC2, 0x16, reg ); break;
        case 13:
            reg = I2C::getInstance()->readRegister(0xC2, 0x16, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC2, 0x16, reg ); break;
        case 14:
            reg = I2C::getInstance()->readRegister(0xC2, 0x17, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC2, 0x17, reg ); break;
        case 15:
            reg = I2C::getInstance()->readRegister(0xC2, 0x17, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC2, 0x17, reg ); break;

        case 16: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC4, 0x14, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC4, 0x14, reg ); break;
        case 17: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC4, 0x14, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC4, 0x14, reg ); break;
        case 18:
            reg = I2C::getInstance()->readRegister(0xC4, 0x15, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC4, 0x15, reg ); break;
        case 19:
            reg = I2C::getInstance()->readRegister(0xC4, 0x15, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC4, 0x15, reg ); break;
        case 20:
            reg = I2C::getInstance()->readRegister(0xC4, 0x16, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC4, 0x16, reg ); break;
        case 21:
            reg = I2C::getInstance()->readRegister(0xC4, 0x16, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC4, 0x16, reg ); break;
        case 22:
            reg = I2C::getInstance()->readRegister(0xC4, 0x17, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC4, 0x17, reg ); break;
        case 23:
            reg = I2C::getInstance()->readRegister(0xC4, 0x17, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC4, 0x17, reg ); break;

        case 24: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC6, 0x14, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC6, 0x14, reg ); break;
        case 25: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC6, 0x14, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC6, 0x14, reg ); break;
        case 26:
            reg = I2C::getInstance()->readRegister(0xC6, 0x15, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC6, 0x15, reg ); break;
        case 27:
            reg = I2C::getInstance()->readRegister(0xC6, 0x15, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC6, 0x15, reg ); break;
        case 28:
            reg = I2C::getInstance()->readRegister(0xC6, 0x16, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC6, 0x16, reg ); break;
        case 29:
            reg = I2C::getInstance()->readRegister(0xC6, 0x16, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC6, 0x16, reg ); break;
        case 30:
            reg = I2C::getInstance()->readRegister(0xC6, 0x17, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC6, 0x17, reg ); break;
        case 31:
            reg = I2C::getInstance()->readRegister(0xC6, 0x17, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC6, 0x17, reg ); break;
                //48
        case 32: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC8, 0x14, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC8, 0x14, reg ); break;
        case 33: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(0xC8, 0x14, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC8, 0x14, reg ); break;
        case 34:
            reg = I2C::getInstance()->readRegister(0xC8, 0x15, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC8, 0x15, reg ); break;
        case 35:
            reg = I2C::getInstance()->readRegister(0xC8, 0x15, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC8, 0x15, reg ); break;
        case 36:
            reg = I2C::getInstance()->readRegister(0xC8, 0x16, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC8, 0x16, reg ); break;
        case 37:
            reg = I2C::getInstance()->readRegister(0xC8, 0x16, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC8, 0x16, reg ); break;
        case 38:
            reg = I2C::getInstance()->readRegister(0xC8, 0x17, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xC8, 0x17, reg ); break;
        case 39:
            reg = I2C::getInstance()->readRegister(0xC8, 0x17, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xC8, 0x17, reg ); break;

        case 40: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(0xCA, 0x14, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xCA, 0x14, reg ); break;
        case 41: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(0xCA, 0x14, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xCA, 0x14, reg ); break;
        case 42:
            reg = I2C::getInstance()->readRegister(0xCA, 0x15, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xCA, 0x15, reg ); break;
        case 43:
            reg = I2C::getInstance()->readRegister(0xCA, 0x15, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xCA, 0x15, reg ); break;
        case 44:
            reg = I2C::getInstance()->readRegister(0xCA, 0x16, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xCA, 0x16, reg ); break;
        case 45:
            reg = I2C::getInstance()->readRegister(0xCA, 0x16, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xCA, 0x16, reg ); break;
        case 46:
            reg = I2C::getInstance()->readRegister(0xCA, 0x17, false); reg &= 0xF0; color == GPIO::GREEN ? reg |=0x08 : reg |=0x01;
            ret = std::make_tuple(0xCA, 0x17, reg ); break;
        case 47:
            reg = I2C::getInstance()->readRegister(0xCA, 0x17, false); reg &= 0x0F;  color == GPIO::GREEN ? reg |=0x80 : reg |=0x10;
            ret = std::make_tuple(0xCA, 0x17, reg ); break;

        }
    return ret;
}
