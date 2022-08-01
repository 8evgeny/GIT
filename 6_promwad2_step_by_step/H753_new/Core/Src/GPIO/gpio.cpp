/*!
 \file gpio.cpp

 \authors Androshchuk A.V.

 \code
    GPIOInit();

    ...

 \endcode



*/
#include "gpio.h"

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

extern I2C_HandleTypeDef hi2c3;

// max address number of TLC59116F chips
uint8_t TLC59116F_max_address;
// TLC59116F chips addresses
const uint8_t TLC59116F_address[] 	= {0xC0,0xC2,0xC4,0xC6,0xC8,0xCA};
// TLC59116F chip button registers
const uint8_t TLC59116F_register[] 	= {0x14,0x15,0x16,0x17};

// max address number of MCP23017 chips
uint8_t MCP23017_max_address;
// MCP23017 chips addresses
const uint8_t MCP23017_address[] = {0x40,0x42,0x44,0x4E};
// MCP23017 chip LEDs registers
const uint8_t MCP23017_register[] = {0x12,0x13};

// TLC59116F chip setup registers values
const uint8_t TLC59116F_Init_Val[] = {
            //  Reg    Data
                0x00 , 0x00,	// MODE1
                0x14 , 0x00,    // LEDOUT0
                0x15 , 0x00,	// LEDOUT1
                0x16 , 0x00,	// LEDOUT2
                0x17 , 0x00		// LEDOUT3
             };

// MCP23017 chip setup registers values
const uint8_t MCP23017_Init_Val[] = {
            //  Reg    Data
                0x04 , 0xFF,	// GPINTENA
                0x05 , 0xFF,	// GPINTENB
                0x0A , 0x42,	// IOCON
                0x0C , 0xFF,	// GPPUA
                0x0D , 0xFF,	// GPPUB
             };


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
    term1("getCFG()") term(getCFG())

    switch (getCFG())
    {
        case 0: TLC59116F_max_address = 2; MCP23017_max_address = 1; break; //УИ-16-1
        case 1: TLC59116F_max_address = 4; MCP23017_max_address = 2; break;	//УИ-32-1
        case 2: TLC59116F_max_address = 6; MCP23017_max_address = 3; break;	//УИ-48-1
        case 3: TLC59116F_max_address = 2; MCP23017_max_address = 1; break;	//УИ-16Н-1
        case 4: TLC59116F_max_address = 4; MCP23017_max_address = 2; break;	//УИ-32Н-1
        default: TLC59116F_max_address = 6; MCP23017_max_address = 3; break;
    }

    TLC59116F_Init();
//    MCP23017_Init();
    initLEDS_SC4();



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

int8_t GPIO::TLC59116F_Init()
{
    uint8_t i, j;
    for (i = 0; i < TLC59116F_max_address; i++)
        for (j = 0; j < sizeof(TLC59116F_Init_Val); j+=2)
        {
term("TLC59116F_Init")
        auto ret = HAL_I2C_Mem_Write(&hi2c3,
                                     TLC59116F_address[i],
                                     TLC59116F_Init_Val[j],
                                     I2C_MEMADD_SIZE_8BIT,
                                     (uint8_t *)&TLC59116F_Init_Val[j+1],
                                     1,
                                     100
                                     );
HAL_Delay(500);
term1("error") term((uint8_t)ret)
            if (ret != HAL_OK)
            {
//                term(i)
                return -i;
            }
        }

    for (uint8_t i =0; i < 100; ++i)
    {
term("writeled")
        GPIO::getInstance()->TLC59116F_writeled(i);
        osDelay(100);
    }


    return 1;
}

int8_t GPIO::MCP23017_Init()
{
term("MCP23017_Init")

//  uint8_t i,j;
//  for (i = 0; i < MCP23017_max_address; i++)
//   for (j = 0; j < sizeof(MCP23017_Init_Val); j+=2) {
//    if (HAL_I2C_Mem_Write_IT(&hi2c3, MCP23017_address[i], MCP23017_Init_Val[j] ,
//    		              I2C_MEMADD_SIZE_8BIT,(uint8_t *)&MCP23017_Init_Val[j+1], 1)!=HAL_OK) return -i;
//   }

//  if ((ui_mode == UI_mode_UI16N)||(ui_mode == UI_mode_UI32N)) { // tel number btn reg
//   for (j = 0; j < sizeof(MCP23017_Init_Val); j+=2) {
//    if (HAL_I2C_Mem_Write_IT(&hi2c3, MCP23017_address[3], MCP23017_Init_Val[j] ,
//    		              I2C_MEMADD_SIZE_8BIT,(uint8_t *)&MCP23017_Init_Val[j+1], 1)!=HAL_OK) return -3;
//   }
//  }
  return 1;
}



uint8_t GPIO::TLC59116F_makeledval(uint8_t led)
{
   uint8_t i,res;
   res = 0;
   for (i=0; i<8; i+=2) {
     res |= (LED_val[led++] & 0x03) << i;
   }

   return res;
}


void GPIO::TLC59116F_writeled(uint8_t led)
{
term( "TLC59116F_writeled")
  led &= 0xFC;
  uint8_t data = TLC59116F_makeledval(led);
  led -= LED_AB1R;
  HAL_I2C_Mem_Write(&hi2c3, TLC59116F_address[led >> 4],
          TLC59116F_register[(led & 0x0F) >> 2],
          I2C_MEMADD_SIZE_8BIT,&data, 1, 100);
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
uint8_t GPIO::getCFG(void)
{
      uint8_t res = 0;
#ifdef SC4
      if (HAL_GPIO_ReadPin(GPIOC, CFG_UI0_Pin)==GPIO_PIN_SET)
          res |= 1;
      if (HAL_GPIO_ReadPin(GPIOC, CFG_UI1_Pin)==GPIO_PIN_SET)
          res |= 2;
      if (HAL_GPIO_ReadPin(GPIOC, CFG_UI2_Pin)==GPIO_PIN_SET)
          res |= 4;
#endif
    return res;
}

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
//                if (i > 2) HAL_GPIO_WritePin(GPIOG, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_SET);
//                else HAL_GPIO_WritePin(GPIOC, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_SET);
            } else
            {
                if(i == 0) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);
                if(i == 1) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET);
                if(i == 2) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_7, GPIO_PIN_RESET);
                if(i == 3) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, GPIO_PIN_RESET);
                if(i == 4) HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8, GPIO_PIN_RESET);
                if(i == 5) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET);
//                if (i > 2) HAL_GPIO_WritePin(GPIOG, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_RESET);
//                else HAL_GPIO_WritePin(GPIOC, GPIO::getInstance()->aLeds[i].ledPin, GPIO_PIN_RESET);
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

#ifdef SC4 //Здесь структуры которые обращаются к плате по I2C3 (as Murom)





#endif

#ifdef SC4 //Два потока для SC4 - светодиоды и кнопки
[[ noreturn ]]
void switchLEDsThread(void const *arg)
{
    (void)arg;
osDelay(200);
term("--- switchLEDsThread ---")
    while(true)
    {
//        for (uint8_t i =0; i<100;++i)
//        {
//            GPIO::getInstance()->TLC59116F_writeled(i);
//            osDelay(100);
//        }

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

void GPIO::initLEDS_SC4()
{
#ifdef SC4


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
