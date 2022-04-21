#include "../Debug/debug.h"
#include "../AUDIO/sai.h"
#include "../RS232/rs232.h"
#include "wdt.h"
#include "../Debug/debug_printf.h"

#ifdef SC2BOARD
#include "gpio_sc2_board.h"
#else
#include "gpio_stm32f7xx.h"
#include "../CAN_STM32F7xx/can_stm32f7xx.h"
#endif


#include "fsforeeprom.h"

#ifndef SC2BOARD
//#include "command.h"
#include "callcontrol.h"
#endif

#include "json.h"
#include "udp_multicast.h"

#include "flash/flash.h"
#include "net_sockets.h"

extern void flashErraseBank2();


#ifndef SC2BOARD
//static CAN *can = CAN::getInstance();
#endif
//static GPIO *gpio = GPIO::getInstance();
//static SRAM *sram = SRAM::getInstance();
//static Debug *debug = &Debug::getInstance();
//static RS232 *rs232 = &RS232::getInstance();
//static Json *json = Json::getInstance();
//static UdpJsonExch *udpJsonExch = UdpJsonExch::getInstance();
//static WDT *wdt = WDT::getInstance();
//static Call_Control *callControl = Call_Control::getInstance();

static osThreadId readPackageTypeThreadId;
static osSemaphoreId canSemId;

#ifdef __cplusplus
extern "C" {
#endif

#include "eeprom.h"
#include "stm32f7xx_hal.h"
#include "cmsis_os.h"

/* Private function prototypes -----------------------------------------------*/
#ifndef SC2BOARD
[[ noreturn ]] static void readToRingBufferThread(void const *arg);
#endif
[[ noreturn ]] static void trackRingBufferThread(void const *arg);
//[[ noreturn ]] static void readPackageTypeThread(void const *arg);
//[[ noreturn ]] static void createTestTaskThread(void const *arg);
//[[ noreturn ]] static void testThread(void const *arg);


/* Define the attributes of a thread functions ---------------------------------*/
#ifndef SC2BOARD
osThreadDef(trackRingBufferThread, trackRingBufferThread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 20);
#else
osThreadDef(trackRingBufferThread, trackRingBufferThread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 20);
#endif

#ifndef SC2BOARD
osThreadDef(changeVolumeThread, changeVolumeThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
#endif

osThreadDef(readFromUartThread, readFromUartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 10);
osThreadDef(StartWdtThread, StartWdtThread, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE * 1);
osThreadDef(recvUdpThread, recvUdpThread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 20);

osThreadDef(audioInitThread, threadAudioInit, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 10);

/**
  * @brief  vApplicationStackOverflowHook
  * @param  xTask : task handle
  * @param  pcTaskName : pointer task name
  * @retval None
  */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    while (1) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}

/* This default malloc failed hook does nothing and is declared as a weak symbol
to allow the application writer to override this default by providing their own
implementation in the application code. */
void vApplicationMallocFailedHook(void)
{

    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    while (1) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}


#ifdef __cplusplus
}
#endif

/**
 * @brief  Configure the MPU attributes .
 * @param  None
 * @retval None
 */
static void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct;

    /* Disable the MPU */
    HAL_MPU_Disable();

    /* Configure the MPU as Normal Non Cacheable for Ethernet Buffers in the SRAM2
     */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x2007C000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_16KB;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Configure the MPU as Device for Ethernet Descriptors in the SRAM2 */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.BaseAddress = 0x2007C000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.SubRegionDisable = 0x00;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);

    /* Enable the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 432;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    /** Activate the Over-Drive mode
    */
    if (HAL_PWREx_EnableOverDrive() != HAL_OK) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    /** Initializes the CPU, AHB and APB busses clocks
    */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_15) != HAL_OK) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_UART7
            | RCC_PERIPHCLK_SAI1 | RCC_PERIPHCLK_I2C1
            | RCC_PERIPHCLK_I2C2 | RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.PLLI2S.PLLI2SN = 344;
    PeriphClkInitStruct.PLLI2S.PLLI2SP = RCC_PLLP_DIV2;
    PeriphClkInitStruct.PLLI2S.PLLI2SR = 2;
    PeriphClkInitStruct.PLLI2S.PLLI2SQ = 7;
    PeriphClkInitStruct.PLLI2SDivQ = 1;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLLI2S;
    PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
    PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void)
{
    /* Enable I-Cache */
    SCB_EnableICache();

    /* Enable D-Cache */
    SCB_EnableDCache();
}

[[ noreturn ]]
static void empty(void const *arg)
{
    (void)arg;
    while (1) {
        osDelay(10);
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();


    /*Configure GPIO pin : ETH_MCO_Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

#include "Bootloader/bootloader.h"

int main()
{


    /* Configure the MPU attributes as Device memory for ETH DMA descriptors */
    MPU_Config();
    /* Enable the CPU Cache */
    // CPU_CACHE_Enable();

    HAL_Init();
    SystemClock_Config();
    GPIOInit();
    MX_GPIO_Init();

#ifndef SC2BOARD
    CANInit();
#endif
    SRAMInit();
    debugInit();
    BSP_EEPROM_Init();
    littleFsInit();
    RS232Init();


    if ((RS232::getInstance().readFromUartThreadId = osThreadCreate(osThread(readFromUartThread), nullptr)) == nullptr) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }



    Json::getInstance()->configStation();
    if (Json::getInstance()->deserializeJsonFlag == Json::JsonFlags::OK) {


        netInit(Json::getInstance()->thisStation.ip, Json::getInstance()->thisStation.mask, Json::getInstance()->thisStation.gateway);


        osThreadDef(emptyThread, empty, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
        osThreadCreate(osThread(emptyThread), nullptr);


        SAI::getInstance()->threadAudioInitId = osThreadCreate(osThread(audioInitThread), nullptr);

#ifndef SC2BOARD
//        if ((can->readToRingBufferThreadId = osThreadCreate(osThread(readToRingBufferThread), nullptr)) == nullptr) {
//            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
//        }

        if ((CAN::getInstance()->trackRingBufferThreadId = osThreadCreate(osThread(trackRingBufferThread), nullptr)) == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
        }

#else
        if ((GPIO::getInstance()->trackRingBufferThreadId = osThreadCreate(osThread(trackRingBufferThread), nullptr)) == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
        }
#endif

        if ((UdpJsonExch::getInstance()->recvUdpThreadId = osThreadCreate(osThread(recvUdpThread), nullptr)) == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
        }

#ifndef SC2BOARD
        if ((osThreadCreate(osThread(changeVolumeThread), nullptr)) == nullptr) {
            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
        }
#endif

        firmwareInitThread();

    }

    WDTInit();

    if ((osThreadCreate(osThread(StartWdtThread), nullptr)) == nullptr) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

//    vTraceEnable(TRC_INIT);

    osKernelStart();

    while (1) {
        Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}

//#ifndef SC2BOARD
///*!
//  * \brief  The thread that reads CAN data from mailbox to the RX ring buffer
//  * \param  pvParameters not used
//  * \retval None
//  */
//[[ noreturn ]]
//static void readToRingBufferThread(void const *arg)
//{
//    (void)arg;
//    while (true) {
//        can->readToRingBuffer();
//        osDelay(1);
//    }
//}
//#endif

#ifdef SC2BOARD
/*!
  * \brief  The thread that tracks the RX ring buffer changes
  * \param  pvParameters not used
  * \retval None
  */
[[ noreturn ]]
static void trackRingBufferThread(void const *arg)
{
    (void)arg;
    while(true) {
        osMutexWait(GPIO::getInstance()->mutexRingBufferRx_id, osWaitForever);
        if (GPIO::getInstance()->ringBufferRx.size() != 0) {

            GPIO::getInstance()->packageRx = GPIO::getInstance()->ringBufferRx.shift();
            osMutexRelease(GPIO::getInstance()->mutexRingBufferRx_id);
            if (!GPIO::getInstance()->testFlag) {
                osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
                UdpJsonExch::getInstance()->callControl->button(GPIO::getInstance()->packageRx);
                osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);
            } else {
                Debug::getInstance().dbg << "Button [" << GPIO::getInstance()->packageRx.payloadData << "] was pressed" << "\n";
                GPIO::getInstance()->configLed(GPIO::getInstance()->packageRx.payloadData, true, 250, 250);
            }
        } else osMutexRelease(GPIO::getInstance()->mutexRingBufferRx_id);

        osDelay(50);
    }
}
#else


/*!
  * \brief  The thread that tracks the RX ring buffer changes
  * \param  pvParameters not used
  * \retval None
  */
[[ noreturn ]]
static void trackRingBufferThread(void const *arg)
{
    (void)arg;
    while(true) {
        osMutexWait(CAN::getInstance()->mutexRingBufferRx_id, osWaitForever);
        if (CAN::getInstance()->ringBufferRx.size() != 0) {

            CAN::getInstance()->packageRx = CAN::getInstance()->ringBufferRx.shift();
            osMutexRelease(CAN::getInstance()->mutexRingBufferRx_id);
            if (!GPIO::getInstance()->testFlag) {
                osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
                UdpJsonExch::getInstance()->callControl->button(CAN::getInstance()->packageRx);
                osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);
            } else {
                Debug::getInstance().dbg << "Button [" << CAN::getInstance()->packageRx.s.payloadData << "] was pressed" << "\n";
                CAN::getInstance()->configLed(CAN::getInstance()->packageRx.s.payloadData, true, 250, 250);
            }
        } else osMutexRelease(CAN::getInstance()->mutexRingBufferRx_id);

        osDelay(50);
    }
}
#endif


