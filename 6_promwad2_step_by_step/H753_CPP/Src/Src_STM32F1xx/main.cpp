/*!
  ******************************************************************************
  * \file    main.cpp
  * \author  Androshchuk A.V.
  * \version V1.0.0
  * \date    16-September-2019
  * \brief   Main program body
  ******************************************************************************
  *
  *
  *
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio_stm32f1xx.h"
#include "../Debug/debug.h"
//#include "command.h"
#include "can_stm32f1xx.h"
#include "wdt.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
constexpr static uint8_t timerDelay = 50; /*!< The variable that stores time delay value of the timer */
static osTimerId timerId; /*!< The thread ID of the timer */
static osThreadId readPackageTypeThreadId, /*!< The thread ID of the readPackageTypeThread */
       configLedsThreadId, /*!< The thread ID of the configLedsThread */
       trackRingBufferThreadId; /*!< TODO: describe */
static GPIO *gpio = GPIO::getInstance(); /*!< The pointer that stores GPIO instance */
static Debug *debug = &Debug::getInstance(); /*!< The pointer that stores Debug instance */
static CAN *can = CAN::getInstance(); /*!< The pointer that stores CAN instance */
static WDT *wdt = WDT::getInstance();

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
[[ noreturn ]] static void runZeroThread(void const *arg);
[[ noreturn ]] static void switchLEDThread(void const *arg);
[[ noreturn ]] static void readButtonThread(void const *arg);
[[ noreturn ]] static void testThread(void const *args);
//[[ noreturn ]] static void sendToCanThread(void const *arg);
[[ noreturn ]] static void cteateTestThread(void const *arg);
[[ noreturn ]] static void readToRingBufferThread( void const *arg);
static void timerCallback(void const *arg);
[[ noreturn ]] void trackRingBufferThread(void const *arg);
//[[ noreturn ]] void configLedsThread(void const *arg);

/* Define the attributes of a thread functions ---------------------------------*/
osThreadDef(runZeroThread, runZeroThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the runZeroThread functions */
osThreadDef(switchLEDThread, switchLEDThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the switchLEDThread functions */
osThreadDef(readButtonThread, readButtonThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the readButtonThread functions */
//osThreadDef(sendToCanThread, sendToCanThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the sendToCanThread functions */
osThreadDef(createTestThread, cteateTestThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the cteateTestThread functions */
osThreadDef(readToRingBufferThread, readToRingBufferThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the readToRingBufferThread functions */
osThreadDef(trackRingBufferThread, trackRingBufferThread, osPriorityNormal, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the trackRingBufferThread functions */
osThreadDef(StartWdtThread, StartWdtThread, osPriorityRealtime, 1, configMINIMAL_STACK_SIZE*1); /*!< Define the attributes of the StartWdtThread functions */
osTimerDef(timer, timerCallback); /*!< Define the attributes of the timer */

/*!
  * \brief  Main program
  * \param  None
  * \retval None
  */
int main()
{
    HAL_Init();
    SystemClock_Config();   // Configure the system clock to 72 MHz
    WDTInit();
    GPIOInit();
    CANInit();
    debugInit();
    debug->dbg << "Failed to start [timer]" << "\n";
    /*Checking configure of CFG resistors*/
    if (gpio->bedCfgFlag) {
        std::string errorMessage = "[Error!] Forbidden combination of CFG resistors";
//        can->createPackage(errorMessage);
//        sendToCan();
        while(1);
    }

    /*Creating threads*/
    if ((gpio->runZeroThreadId = osThreadCreate( osThread(runZeroThread), nullptr)) == nullptr) {
        debug->dbg << "Failed to create [runZeroThread]" << "\n";
        while(1);
    }

    if (osThreadCreate( osThread(switchLEDThread), nullptr) == nullptr) {
        debug->dbg << "Failed to create [initLedThread]" << "\n";
        while(1);
    }

    if ((gpio->readButtonThreadId = osThreadCreate( osThread(readButtonThread), nullptr)) == nullptr) {
        debug->dbg << "Failed to create [readKeyThread]" << "\n";
        while(1);
    }

    timerId = osTimerCreate( osTimer(timer), osTimerPeriodic, nullptr); // create timer thread

    if (timerId) {
        osStatus status = osTimerStart (timerId, timerDelay);   // start timer
        if (status != osOK)  {
            debug->dbg << "Failed to start [timer]" << "\n";
            while(1);
        }
    }

    if (osThreadCreate( osThread(readToRingBufferThread), nullptr) == nullptr) {
        debug->dbg << "Failed to create [readToRingBufferThread]" << "\n";
        while(1);
    }

    if ((gpio->createTestThreadId = osThreadCreate( osThread(createTestThread), nullptr)) == nullptr) {
        debug->dbg << "Failed to create [createTestThread]" << "\n";
        while(1);
    }

    if ((trackRingBufferThreadId = osThreadCreate( osThread(trackRingBufferThread), nullptr)) == nullptr) {
        debug->dbg << "Failed to create [trackRingBufferThread]" << "\n";
        while(1);
    }


    if ((osThreadCreate( osThread(StartWdtThread), nullptr)) == nullptr) {
        debug->dbg << "Failed to create [StartWdtThread]" << "\n";
        while(1);
    }

    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET);
    osKernelStart();

}

/*!
   \brief System Clock Configuration
           The system Clock is configured as follow :
              System Clock source            = PLL (HSE)
              SYSCLK(Hz)                     = 72000000
              HCLK(Hz)                       = 72000000
              AHB Prescaler                  = 1
              APB1 Prescaler                 = 2
              APB2 Prescaler                 = 1
              HSE Frequency(Hz)              = 25000000
              HSE PREDIV1                    = 2
              PLLMUL                         = 9
              Flash Latency(WS)              = 2
    \param None
    \return None
*/
static void SystemClock_Config(void)
{
    RCC_ClkInitTypeDef clkinitstruct;
    RCC_OscInitTypeDef oscinitstruct;

    /* Enable HSE Oscillator and activate PLL with HSE as source */
    oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
    oscinitstruct.HSEState        = RCC_HSE_ON;
    oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
    oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
    oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
    oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK) {
        /* Initialization Error */
        debug->dbg << "RCC Init Errore [oscinitstruct init erore]" << "\n";
        while(1);
    }

    /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
       clocks dividers */
    clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
    clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;
    if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK) {
        /* Initialization Error */
        debug->dbg << "RCC Init Errore [clkinitstruct init error]" << "\n";
        while(1);
    }
}

/*!
  \brief The thread of the combined passage in columns of the keyboard
  \param arg is not used
  \return None
  */
[[ noreturn ]]
void runZeroThread(void const *arg)
{
    (void) arg;
    while(true) {
        gpio->runZero();
    }
}



/*!
 \brief The timer call back function.
        It sets keyboard LEDs state ON or OFF depending on the operating mode.
 \param arg is not used
 \return None
*/
static void timerCallback(void const *arg)
{
    (void)arg;
    for (uint8_t i = 0; i < 32; i++) {
        if (gpio->aLeds[i].timeStart) {
            gpio->aLeds[i].count += 1;
            if(gpio->aLeds[i].ledState == false && gpio->aLeds[i].count >= gpio->aLeds[i].timeOff/timerDelay) {
                gpio->aLeds[i].ledState = true;
                gpio->aLeds[i].count = 0;
                if (gpio->aLeds[i].reiterationNum > 0)
                    gpio->aLeds[i].reiterationNum -= 1;
                if (gpio->aLeds[i].reiterationNum == 0) {
                    gpio->aLeds[i].timeStart = false;
                    gpio->aLeds[i].reiterationNum -= 1;
                }
            } else if (gpio->aLeds[i].ledState == true && gpio->aLeds[i].count >= gpio->aLeds[i].timeOn/timerDelay) {
                gpio->aLeds[i].ledState = false;
                gpio->aLeds[i].count = 0;
                if (gpio->aLeds[i].reiterationNum > 0)
                    gpio->aLeds[i].reiterationNum -= 1;
                if (gpio->aLeds[i].reiterationNum == 0) {
                    gpio->aLeds[i].timeStart = false;
                    gpio->aLeds[i].reiterationNum -= 1;
                }
            }
        }
    }
}


/*!
 \brief The thread toggles keyboard LEDs
        depending on the ledState value of the Keyboard structure
 \param arg is not used
 \return None
*/
[[ noreturn ]]
static void switchLEDThread(void const *arg)
{
    (void)arg;
    while(true) {
        gpio->switchLEDs();
    }
}

/*!
 \brief The thread reads button state pressed or not pressed.
 \param arg is not used
 \return None
*/
[[ noreturn ]]
static void readButtonThread(void const *arg)
{
    (void)arg;
    while(true) {
        gpio->readButton();
        osDelay(1);
    }

}

/*!
 \brief The thread creates the testThread
        after the test button has been pressed
 \param arg is not used
 \return None
*/
[[ noreturn ]]
static void cteateTestThread(void const *arg)
{
    (void)arg;
    osSignalWait (0x06, osWaitForever);
    while(true) {
        gpio->test();
        can->test();
        osThreadTerminate(gpio->createTestThreadId);
    }
}

/*!
 \brief The thread reads data from the CAN bus to the ring buffer
 \param arg is not used
 \return None
*/
[[ noreturn ]]
static void readToRingBufferThread( void const *arg)
{
    (void)arg;
    while(true) {
        can->readToRingBuffer();
    }
}


/*!
 \brief The thread tracks ring buffer
        and sends signal to the thread
        that reads data from the that one
 \param arg is not used
 \return None
*/
[[ noreturn ]]
void trackRingBufferThread(void const *arg)
{
    (void)arg;
    while(true) {
        osMutexWait(can->mutexRingBufferRx_id, osWaitForever);
        if (can->ringBufferRx.size() != 0) {

            can->packageRx = can->ringBufferRx.shift();
            osMutexRelease(can->mutexRingBufferRx_id);

            if (!gpio->testFlag) {
                if (can->packageRx.s.packetType == CAN::data) {

                    gpio->ledConfig(can->packageRx.s.payloadData[0],
                                    can->packageRx.s.payloadData[1],
                                    can->packageRx.s.payloadData[2],
                                    can->packageRx.s.payloadData[3],
                                    can->packageRx.s.payloadData[4]);

                } else if (can->packageRx.s.packetType == CAN::mikeLed) {
                    if (can->packageRx.s.payloadData[1]) {
                        //    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET); /*UPR1*/
                        //    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET); /*UPR3*/
                        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_RESET); /*UPR2*/
                    } else {
                        //    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7,GPIO_PIN_RESET); /*UPR1*/
                        //    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET); /*UPR3*/
                        HAL_GPIO_WritePin(GPIOB,GPIO_PIN_6,GPIO_PIN_SET); /*UPR2*/
                    }
                }
            } else {
                debug->dbg << "[TEST] Button [" << can->packageRx.s.payloadData[0] << "] was pressed" << "\n";
                GPIO::getInstance()->ledConfig(can->packageRx.s.payloadData[0], true, 300, 300, 0);
            }

        } else osMutexRelease(can->mutexRingBufferRx_id);

        osDelay(50);
    }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1) {
    }
}
#endif
