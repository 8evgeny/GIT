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
#include "tlv320aic3254.h"
#include "aic3254_reg.h"

bool volUpPressed;
bool volDownPressed;
bool sensUpPressed;
bool sensDownPressed;
#ifdef __cplusplus
extern "C" {
#endif

osTimerDef(timer7, timerCallback); /*!< Define the attributes of the timer */
extern uint8_t LinkStatus;
extern uint8_t inMcastGroup;
//osMessageQDef(message_q, 1, uint16_t); // Declare a message queue
osMutexDef (mutexRingBufferRx);
osMutexDef (mutexBoard);
osMutexDef (mutexButtons);

// max address number of TLC59116F chips
extern uint8_t TLC59116F_max_address;
// TLC59116F chips addresses
const uint8_t TLC59116F_address[] = {0xC0,0xC2,0xC4,0xC6,0xC8,0xCA};
// TLC59116F chip button registers
const uint8_t TLC59116F_registerLED[] = {0x14,0x15,0x16,0x17};
// Регистры индивидуальной яркости каждого светодиода
const uint8_t TLC59116F_registerBright[] = {0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11};
// TLC59116F chip setup registers values


// max address number of MCP23017 chips
extern uint8_t MCP23017_max_address;
// MCP23017 chips addresses
const uint8_t MCP23017_address[] = {0x40,0x42,0x44,0x4E};
// MCP23017 chip LEDs registers
const uint8_t MCP23017_register[] = {0x12,0x13};
// MCP23017 chip setup registers values
const uint8_t MCP23017_Init_Val[] = {
            //  Reg    Data
                0x04 , 0xFF,	// GPINTENA
                0x05 , 0xFF,	// GPINTENB
                0x0A , 0x42,	// IOCON
                0x0C , 0xFF,	// GPPUA
                0x0D , 0xFF,	// GPPUB
             };

extern osSemaphoreId Netif_LinkSemaphore;
static osTimerId timerId7; /*!< The thread ID of the timer */
constexpr static uint8_t timerDelay = 50;

static GPIO_InitTypeDef GPIO_InitStruct;

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
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET); //Пин Норма
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); //Пин МК Вкл
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET); //Пин UPR1_SP - Включение усилителя

}

#ifdef __cplusplus
}
#endif

GPIO::GPIO()
{
    gpioInit = & GPIO_InitStruct;
#ifndef SC4
    initLEDS_SC2();
#endif

    mutexRingBufferRx_id = osMutexCreate(osMutex(mutexRingBufferRx));
    mutexBoard_id = osMutexCreate(osMutex(mutexBoard));
    mutexButtons_id = osMutexCreate(osMutex(mutexButtons));
    if (mutexRingBufferRx_id == nullptr)
    {
        while(1)
            RS232::getInstance().term << "Failed to create [mutexRingBufferRx]" << "\n";
    }

//    message_q_id = osMessageCreate(osMessageQ(message_q), NULL);

#ifndef SC4
    initBUTTONS_SC2();
#endif
#ifdef SC4
    i2cInitBoard();
    SC4_EXTI_IRQHandler_Config();
#endif
}

GPIO *GPIO::getInstance()
{
    if (!p_instance) p_instance = new GPIO();
    return p_instance;
}

GPIO *GPIO::p_instance = nullptr;

struct Aic3254Configs ConfigureDAC_VOL[] {
    {TLV320AIC3254_REG_PAGE_SELECT, 0x01},
    {TLV320AIC3254_REG_CM_CR, 0x0B},
    {TLV320AIC3254_REG_LOL_SEL, 0x08},
    {TLV320AIC3254_REG_LOR_SEL, 0x08},
    {TLV320AIC3254_REG_OUTDRV_PWR_CR, 0x0C},
    {TLV320AIC3254_REG_LOL_GAIN, 0x0A},
    {TLV320AIC3254_REG_LOR_GAIN, 0x0A},
    // Select Page 0
    {TLV320AIC3254_REG_PAGE_SELECT, 0x00},
    // DAC => 0dB
    {TLV320AIC3254_REG_LDAC_DVOL_CR, 0xD7},   // ГРОМКОСТЬ -20,5dB
    {TLV320AIC3254_REG_RDAC_DVOL_CR, 0xC0},   // ГРОМКОСТЬ -32dB
    // Powerup LDAC/RDAC
    {TLV320AIC3254_REG_DAC_SETUP1, 0xd4},
    // UnmuteLDAC/RDAC
    {TLV320AIC3254_REG_DAC_SETUP2, 0x00},
};

constexpr static uint32_t I2C_ADDRESS = 49;
void GPIO::upVolume(void)
{
    term2("upVolume")
    ConfigureDAC_VOL[8].regVal = GPIO::getInstance()->dacDriverGainValue;

    for (uint32_t i = 0; i < sizeof(ConfigureDAC_VOL) / sizeof(struct Aic3254Configs); i++)
    {
        osMutexWait(GPIO::getInstance()->mutexButtons_id, osWaitForever);
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureDAC_VOL[i].regOffset, ConfigureDAC_VOL[i].regVal, true);
        osMutexRelease(GPIO::getInstance()->mutexButtons_id);
    }

}
void GPIO::downVolume(void)
{
    term2("downVolume")
    ConfigureDAC_VOL[8].regVal = GPIO::getInstance()->dacDriverGainValue;

    for (uint32_t i = 0; i < sizeof(ConfigureDAC_VOL) / sizeof(struct Aic3254Configs); i++)
    {
        osMutexWait(GPIO::getInstance()->mutexButtons_id, osWaitForever);
        I2C::getInstance()->writeRegister(I2C_ADDRESS, ConfigureDAC_VOL[i].regOffset, ConfigureDAC_VOL[i].regVal, true);
        osMutexRelease(GPIO::getInstance()->mutexButtons_id);
    }

}
void GPIO::upSens(void)
{
    term2("upSens")

}
void GPIO::downSens(void)
{
    term2("downSens")
}
void GPIO::test(void)
{

}

void GPIO::configLed(uint8_t ledNumber, bool ledOn, uint32_t timeOn, uint32_t timeOff, uint8_t repeatNum)
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
    for (uint8_t i = 0; i <
#ifdef SC4
                        keysNum;
#endif
#ifndef SC4
                        6;
#endif
         i++) {
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
        for (uint8_t i = 0; i < 6 ; ++i)
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
term("--- switchLEDsThread ---")

    GPIO::getInstance()->initLEDS_SC4();
    GPIO::getInstance()->testLed();

    while(true)
    {
        uint8_t adr, reg, numON, numOFF;
        for(uint8_t i = 0; i < TLC59116F_max_address * 8; ++i)
        {
            if (GPIO::getInstance()->aLeds[i].ledState)
            {// Включаем пин
                std::tie (adr, reg, numON,numOFF)  = GPIO::getInstance()->fromIndexToReg(i, GPIO::getInstance()->GREEN);
                I2C::getInstance()->writeRegister(adr, reg, numON, false);
            } else
            {// Гасим пин
                std::tie (adr, reg, numON,numOFF)  = GPIO::getInstance()->fromIndexToReg(i, GPIO::getInstance()->RED);
                I2C::getInstance()->writeRegister(adr, reg, numOFF, false);
            }
        }

        if ((LinkStatus == 1) && (inMcastGroup == 1))
        {
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET); //Пин Норма
        }
        else
        {
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
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

//    osDelay(8000);
    GPIO::getInstance()->initBUTTONS_SC4();

term("--- readButtonThread ---")
    uint8_t readBoard = 0;
    uint8_t numButton = 0;
    uint32_t tickstart = HAL_GetTick();
    bool keySendingFlag = false;

    while(true)
    {
        //Повторное нажатие воспринимается
        if (HAL_GetTick() - 50 > tickstart)
        {
            keySendingFlag = false;
            numButton = 0;
        }

        for (uint8_t i = 0; i < MCP23017_max_address; ++i)
        {
            for (uint8_t j = 0; j < 2; ++j)
            {
                readBoard = I2C::getInstance()->readRegister(MCP23017_address[i], MCP23017_register[j], false);
                if (readBoard != 255)
                {
                    numButton = GPIO::getInstance()->findBUTTONS_SC4(readBoard, i, j);
                }
            }
        }//Просканировали все клавиши

        for (uint8_t i = 0; i < TLC59116F_max_address * 8 ; ++i)
        {
            if ((numButton-1) == GPIO::getInstance()->buttonArray[i].i) //Похоже здесь buttonArray не нужен
            {

                if ((!keySendingFlag) && (numButton != 0))
                {
                //Передача кнопки в основной код
                    tempPack.payloadData = numButton;
                osMutexWait(GPIO::getInstance()->mutexRingBufferRx_id, osWaitForever);
                    GPIO::getInstance()->ringBufferRx.push(tempPack);
                osMutexRelease(GPIO::getInstance()->mutexRingBufferRx_id);
                    tickstart = HAL_GetTick();
                    keySendingFlag = true;

                    term1("Pressed key") term2(numButton) //Тестовый вывод
                }

            }
            else
            {
                GPIO::getInstance()->buttonArray[i].n = 0;
            }

        }
        if(volDownPressed)
        {
            GPIO::getInstance()->downVolume();
            volDownPressed = false;
        }
        if(volUpPressed)
        {
            GPIO::getInstance()->upVolume();
            volUpPressed = false;
        }
        if(sensDownPressed)
        {
            GPIO::getInstance()->downSens();
            sensDownPressed = false;
        }
        if(sensUpPressed)
        {
            GPIO::getInstance()->upSens();
            sensUpPressed = false;
        }

         osDelay(50); //Возможно нужно убрать

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


#ifdef __cplusplus
extern "C" {
#endif
    auto timeVolPlus = HAL_GetTick();
    void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
    {

        if (GPIO_Pin == GPIO_PIN_5)
        {
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5))
            {
                RS232Puts("Release TEST button\r\n");
            }
            else
            {
                RS232Puts("Pressed TEST button\r\n");
            }
        }
        else if (GPIO_Pin == GPIO_PIN_9)
        {
            if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) && (timeVolPlus + 300 < HAL_GetTick()))
            {
                RS232Puts("Pressed VOL+ button\r\n");

                if (GPIO::getInstance()->dacDriverGainValue < GPIO::getInstance()->dacDriverGainValueMax)
                GPIO::getInstance()->dacDriverGainValue = GPIO::getInstance()->dacDriverGainValue + GPIO::getInstance()->dacDriverGainValueStep;
                volUpPressed = true;
                timeVolPlus = HAL_GetTick();

                term2(GPIO::getInstance()->dacDriverGainValue / 2)
            }
        }
        else if (GPIO_Pin == GPIO_PIN_10)
        {
            if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10))
            {
                RS232Puts("Pressed VOL- button\r\n");
                if (GPIO::getInstance()->dacDriverGainValue > GPIO::getInstance()->dacDriverGainValueMin)
                GPIO::getInstance()->dacDriverGainValue = GPIO::getInstance()->dacDriverGainValue - GPIO::getInstance()->dacDriverGainValueStep;
                volDownPressed = true;

                term2(GPIO::getInstance()->dacDriverGainValue / 2 )
            }
        }
        else if (GPIO_Pin == GPIO_PIN_11)
        {
            RS232Puts("Pressed SENS+ button\r\n");

//            if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11))
//            {
                if (GPIO::getInstance()->dacDriverSensValue < GPIO::getInstance()->dacDriverSensValueMax)
                GPIO::getInstance()->dacDriverSensValue = GPIO::getInstance()->dacDriverSensValue + GPIO::getInstance()->dacDriverSensValueStep;
                sensUpPressed = true;
                term2(GPIO::getInstance()->dacDriverSensValue )
//            }
        }
        else if (GPIO_Pin == GPIO_PIN_12)
        {
            RS232Puts("Pressed SENS- button\r\n");

//            if (!HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12))
//            {
                if (GPIO::getInstance()->dacDriverSensValue > GPIO::getInstance()->dacDriverSensValueMin)
                GPIO::getInstance()->dacDriverSensValue = GPIO::getInstance()->dacDriverSensValue - GPIO::getInstance()->dacDriverSensValueStep;
                sensDownPressed = true;
                term2(GPIO::getInstance()->dacDriverSensValue )
//            }
        }
    }

void EXTI2_IRQHandler(void)
{
RS232Puts("--- EXTI2_IRQHandler ---\r\n");
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI4_IRQHandler(void)
{
    term("Interrupt UI board")
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_9);
}

void EXTI15_10_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_11);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}

void GPIO::SC4_EXTI_IRQHandler_Config() {
  GPIO_InitTypeDef   GPIO_InitStructure;

  //button TEST interrupt pin setup
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = TEST_BUT_Pin;
  HAL_GPIO_Init(TEST_BUT_GPIO_Port, &GPIO_InitStructure);

#ifdef SC4
  //button UI board interrupt pin setup
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStructure.Pull = GPIO_PULLUP;
  GPIO_InitStructure.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  // Configure GPIO for volume and sens buttons ######################*/
  GPIO_InitStructure.Pin       = GPIO_PIN_9| GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStructure.Mode      = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull      = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

#endif

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

#ifdef __cplusplus
}
#endif

std::tuple<u_int8_t, u_int8_t, u_int8_t, u_int8_t> GPIO::fromIndexToReg(u_int8_t i, GPIO::Color color)
{
    uint8_t reg = 0;
    uint8_t regON = 0;
    uint8_t regOFF = 0;

    switch (i)
    { //Возвращаем адрес микросхемы  номер регистра значение для установки и для сброса
        case 0: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(TLC59116F_address[0], TLC59116F_registerLED[0], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[0], TLC59116F_registerLED[0], regON, regOFF );
        case 1: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(TLC59116F_address[0], TLC59116F_registerLED[0], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[0], TLC59116F_registerLED[0], regON, regOFF );
        case 2:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[0], TLC59116F_registerLED[1], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[0], TLC59116F_registerLED[1], regON, regOFF );
        case 3:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[0], TLC59116F_registerLED[1], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[0], TLC59116F_registerLED[1], regON, regOFF );
        case 4:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[0], TLC59116F_registerLED[2], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[0], TLC59116F_registerLED[2], regON, regOFF );
        case 5:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[0], TLC59116F_registerLED[2], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[0], TLC59116F_registerLED[2], regON, regOFF );
        case 6:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[0], TLC59116F_registerLED[3], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[0], TLC59116F_registerLED[3], regON, regOFF );
        case 7:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[0], TLC59116F_registerLED[3], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[0], TLC59116F_registerLED[3], regON, regOFF );

        case 8: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(TLC59116F_address[1], TLC59116F_registerLED[0], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[1], TLC59116F_registerLED[0], regON, regOFF );
        case 9: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(TLC59116F_address[1], TLC59116F_registerLED[0], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[1], TLC59116F_registerLED[0], regON, regOFF );
        case 10:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[1], TLC59116F_registerLED[1], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[1], TLC59116F_registerLED[1], regON, regOFF );
        case 11:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[1], TLC59116F_registerLED[1], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[1], TLC59116F_registerLED[1], regON, regOFF );
        case 12:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[1], TLC59116F_registerLED[2], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[1], TLC59116F_registerLED[2], regON, regOFF );
        case 13:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[1], TLC59116F_registerLED[2], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[1], TLC59116F_registerLED[2], regON, regOFF );
        case 14:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[1], TLC59116F_registerLED[3], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[1], TLC59116F_registerLED[3], regON, regOFF );
        case 15:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[1], TLC59116F_registerLED[3], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[1], TLC59116F_registerLED[3], regON, regOFF );

        case 16: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(TLC59116F_address[2], TLC59116F_registerLED[0], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[2], TLC59116F_registerLED[0], regON, regOFF );
        case 17: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(TLC59116F_address[2], TLC59116F_registerLED[0], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[2], TLC59116F_registerLED[0], regON, regOFF );
        case 18:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[2], TLC59116F_registerLED[1], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[2], TLC59116F_registerLED[1], regON, regOFF );
        case 19:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[2], TLC59116F_registerLED[1], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[2], TLC59116F_registerLED[1], regON, regOFF );
        case 20:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[2], TLC59116F_registerLED[2], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[2], TLC59116F_registerLED[2], regON, regOFF );
        case 21:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[2], TLC59116F_registerLED[2], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[2], TLC59116F_registerLED[2], regON, regOFF );
        case 22:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[2], TLC59116F_registerLED[3], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[2], TLC59116F_registerLED[3], regON, regOFF );
        case 23:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[2], TLC59116F_registerLED[3], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[2], TLC59116F_registerLED[3], regON, regOFF );

        case 24: //Пишем в младшие 4 бита
            reg = I2C::getInstance()->readRegister(TLC59116F_address[3], TLC59116F_registerLED[0], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[3], TLC59116F_registerLED[0], regON, regOFF );
        case 25: //Пишем в старшие 4 бита
            reg = I2C::getInstance()->readRegister(TLC59116F_address[3], TLC59116F_registerLED[0], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[3], TLC59116F_registerLED[0], regON, regOFF );
        case 26:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[3], TLC59116F_registerLED[1], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[3], TLC59116F_registerLED[1], regON, regOFF );
        case 27:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[3], TLC59116F_registerLED[1], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[3], TLC59116F_registerLED[1], regON, regOFF );
        case 28:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[3], TLC59116F_registerLED[2], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[3], TLC59116F_registerLED[2], regON, regOFF );
        case 29:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[3], TLC59116F_registerLED[2], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[3], TLC59116F_registerLED[2], regON, regOFF );
        case 30:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[3], TLC59116F_registerLED[3], false); regOFF = reg & 0xF0; regON = reg & 0xF0; color == GPIO::GREEN ? regON |=0x08 : regON  |=0x01;
            return std::make_tuple(TLC59116F_address[3], TLC59116F_registerLED[3], regON, regOFF );
        case 31:
            reg = I2C::getInstance()->readRegister(TLC59116F_address[3], TLC59116F_registerLED[3], false); regOFF = reg & 0x0F; regON = reg & 0x0F; color == GPIO::GREEN ? regON |=0x80 : regON  |=0x10;
            return std::make_tuple(TLC59116F_address[3], TLC59116F_registerLED[3], regON, regOFF );
        }

}

void GPIO::testLed()
{
    uint8_t testBlink = 0;
    uint8_t testRun = 0;

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
        uint8_t adr, reg, numON, numOFF;
        for (uint8_t i = 0; i < TLC59116F_max_address * 8; i++)
        {
           std::tie (adr, reg, numON, numOFF)  = GPIO::getInstance()->fromIndexToReg(i, GPIO::getInstance()->RED);
           I2C::getInstance()->writeRegister(adr, reg, numON, false);
           osDelay(30);
        }
        for (uint8_t i = 0; i < TLC59116F_max_address * 8; i++)
        {
           std::tie (adr, reg, numON, numOFF)  = GPIO::getInstance()->fromIndexToReg(i, GPIO::getInstance()->GREEN);
           I2C::getInstance()->writeRegister(adr, reg, numON, false);
           osDelay(30);
        }
        ++testRun;
    }//End Бегущий огонек

    //Гасим все
    for (uint8_t i = 0; i < TLC59116F_max_address; i++)
    {
        for (uint8_t j = 0; j < 4; j++)
        {
            I2C::getInstance()->writeRegister(TLC59116F_address[i], TLC59116F_registerLED[j], 0x00, false);
        }
    }
}

void GPIO::initLEDS_SC4()
{
#ifdef SC4
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
#endif
}

void GPIO::initBUTTONS_SC4()
{
#ifdef SC4
    for (uint8_t i = 0; i < MCP23017_max_address * 16; ++i)
    {
        buttonArray[i].i = i;
        buttonArray[i].n = 0;
    }


    for (uint8_t i = 0; i < MCP23017_max_address; ++i)
    {
        for (uint8_t j = 0; j < sizeof(MCP23017_Init_Val); j += 2)
        {
            I2C::getInstance()->writeRegister(MCP23017_address[i], MCP23017_Init_Val[j] , MCP23017_Init_Val[j+1], false);
        }
    }

    //Инициализация регистров телефонного номера


#endif
}

uint8_t GPIO::findBUTTONS_SC4(uint8_t num, uint8_t adr, uint8_t reg)
{
#ifdef SC4
uint8_t ret = 0;
switch (num)
{
    case 254: if(adr == 0) reg == 0? ret = 1:ret = 9;
              if(adr == 1) reg == 0? ret = 17:ret = 25;
              break;
    case 253: if(adr == 0) reg == 0? ret = 2:ret = 10;
              if(adr == 1) reg == 0? ret = 18:ret = 26;
              break;
    case 251: if(adr == 0) reg == 0? ret = 3:ret = 11;
              if(adr == 1) reg == 0? ret = 19:ret = 27;
              break;
    case 247: if(adr == 0) reg == 0? ret = 4:ret = 12;
              if(adr == 1) reg == 0? ret = 20:ret = 28;
              break;
    case 239: if(adr == 0) reg == 0? ret = 5:ret = 13;
              if(adr == 1) reg == 0? ret = 21:ret = 29;
              break;
    case 223: if(adr == 0) reg == 0? ret = 6:ret = 14;
              if(adr == 1) reg == 0? ret = 22:ret = 30;
              break;
    case 191: if(adr == 0) reg == 0? ret = 7:ret = 15;
              if(adr == 1) reg == 0? ret = 23:ret = 31;
              break;
    case 127: if(adr == 0) reg == 0? ret = 8:ret = 16;
              if(adr == 1) reg == 0? ret = 24:ret = 32;
              break;

    default : if(adr == 0) reg == 0? ret = 1:ret = 9;
              if(adr == 1) reg == 0? ret = 17:ret = 25;
              break;
}

return ret;
#endif
}
