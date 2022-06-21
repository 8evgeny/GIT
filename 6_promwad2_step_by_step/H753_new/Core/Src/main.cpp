#include "main.h"
#include "rs232.h"
#include "../Debug/debug.h"
#include "wdt.h"
#include "sai.h"
#include "gpio_sc2_board.h"
#include "testTasks.h"
#include "fsforeeprom.h"
#include "json.h"
#include "udp_multicast.h"
#include "flash/flash.h"
#include "net_sockets.h"
#include "i2ceeprom.h"
#include "lwip.h"
#include "ethernetif.h"
#include "dp83848.h"

char msgOCD[] = "Hello STM32 lovers!\n";
extern void flashErraseBank2();

//static osThreadId readPackageTypeThreadId;
//static osSemaphoreId canSemId;

#ifdef __cplusplus
extern "C" {
#endif

#include "eeprom.h"
#include "stm32h7xx_hal.h"
#include "cmsis_os.h"

[[ noreturn ]] static void trackRingBufferThread(void const *arg);
//[[ noreturn ]] static void readPackageTypeThread(void const *arg);
//[[ noreturn ]] static void createTestTaskThread(void const *arg);
//[[ noreturn ]] static void testThread(void const *arg);
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
//static void MX_FMC_Init(void);
//static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_SAI1_Init(void);
static void MX_UART7_Init(void);
static void MX_TIM3_Init(void);
static void MX_DMA_Init(void);
static void MX_RNG_Init(void);
void StartDefaultTask(void const * argument);

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;
DMA_HandleTypeDef hdma_i2c2_rx;
DMA_HandleTypeDef hdma_i2c2_tx;
RNG_HandleTypeDef hrng;
SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;
DMA_HandleTypeDef hdma_sai1_a;
DMA_HandleTypeDef hdma_sai1_b;
DMA_HandleTypeDef hdma_uart7_rx;
DMA_HandleTypeDef hdma_uart7_tx;
TIM_HandleTypeDef htim3;
UART_HandleTypeDef huart7;
SRAM_HandleTypeDef hsram1;

//Добавил_CUBE_03_05_2022
MDMA_HandleTypeDef hmdma_memtomem_dma2_stream0;
//static void MX_MDMA_Init(void); //Вынес с SRAM


extern struct netif gnetif;
extern DP83848_Object_t DP83848;
void ethernet_link_check_state(struct netif *netif)
{
  ETH_MACConfigTypeDef MACConf;
  int32_t PHYLinkState;
  uint32_t linkchanged = 0, speed = 0, duplex =0;

  PHYLinkState = DP83848_GetLinkState(&DP83848);

  if(netif_is_link_up(netif) && (PHYLinkState <= DP83848_STATUS_LINK_DOWN))
  {
    HAL_ETH_Stop(&heth);
    netif_set_down(netif);
    netif_set_link_down(netif);
  }
  else if(!netif_is_link_up(netif) && (PHYLinkState > DP83848_STATUS_LINK_DOWN))
  {
    switch (PHYLinkState)
    {
    case DP83848_STATUS_100MBITS_FULLDUPLEX:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      linkchanged = 1;
      break;
    case DP83848_STATUS_100MBITS_HALFDUPLEX:
      duplex = ETH_HALFDUPLEX_MODE;
      speed = ETH_SPEED_100M;
      linkchanged = 1;
      break;
    case DP83848_STATUS_10MBITS_FULLDUPLEX:
      duplex = ETH_FULLDUPLEX_MODE;
      speed = ETH_SPEED_10M;
      linkchanged = 1;
      break;
    case DP83848_STATUS_10MBITS_HALFDUPLEX:
      duplex = ETH_HALFDUPLEX_MODE;
      speed = ETH_SPEED_10M;
      linkchanged = 1;
      break;
    default:
      break;
    }

    if(linkchanged)
    {
      /* Get MAC Config MAC */
      HAL_ETH_GetMACConfig(&heth, &MACConf);
      MACConf.DuplexMode = duplex;
      MACConf.Speed = speed;
      HAL_ETH_SetMACConfig(&heth, &MACConf);

      HAL_ETH_Start(&heth);
      netif_set_up(netif);
      netif_set_link_up(netif);
    }
  }

}
uint32_t EthernetLinkTimer;
void Ethernet_Link_Periodic_Handle(struct netif *netif)
{
  /* Ethernet Link every 100ms */
  if (HAL_GetTick() - EthernetLinkTimer >= 100)
  {
    EthernetLinkTimer = HAL_GetTick();
    ethernet_link_check_state(netif);
  }
}


osThreadId defaultTaskHandle;

osThreadDef(trackRingBufferThread, trackRingBufferThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 20);
osThreadDef(readFromUartThread, readFromUartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE );
osThreadDef(StartWdtThread, StartWdtThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 1);
//osThreadDef(recvUdpThread, recvUdpThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 20);
//osThreadDef(audioInitThread, threadAudioInit, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 10);

osThreadDef(switchLEDsThread, switchLEDsThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
osThreadDef(replaceTimerCallback, replaceTimerCallback, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 4);
osThreadDef(readButtonThread, readButtonThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 4 );

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    while (1) {
        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
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
        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }
}

#ifdef __cplusplus
}
#endif

[[ noreturn ]]
static void empty(void const *arg)
{

HAL_Delay(400);
term("****  empty  start  ****")

    (void)arg;
    while (1) {
        osDelay(10);
    }
}

int main(void)
{

    /* Configure the MPU attributes as Device memory for ETH DMA descriptors */
    MPU_Config();

  /* Enable the CPU Cache */

//    SCB_EnableICache();

//    SCB_EnableDCache();

    HAL_Init();
    SystemClock_Config();

/* Configure the peripherals common clocks */
    PeriphCommonClock_Config();

    /* Initialize all configured peripherals */

    MX_GPIO_Init();
//    MX_FMC_Init();  //Вынес в SRAMInit

//    MX_MDMA_Init(); //Вынес в SRAM
//    MX_I2C1_Init(); //Вынесен в EEPROM
    MX_I2C2_Init();
    MX_I2C3_Init();
    MX_SAI1_Init();
    MX_UART7_Init(); //Вынесен в RS232
    debugInit();
    RS232Init();
    MX_TIM3_Init();
    MX_DMA_Init(); //Вынесен в stm32h7xx_hal_msp.c
    MX_RNG_Init();
    GPIOInit();  //Тут остался только таймер - не стартует ??

    SRAMInit();
    BSP_EEPROM_Init();

//    simpleEEPROM_test();
//    simpleEEPROM_test2();
//    littleFsInit();
//    FsForEeprom::getInstance().test();

//    Flash::getInstance().test(); // Не работает


//При включении рычаги перестают работать
    if ((RS232::getInstance().readFromUartThreadId = osThreadCreate(osThread(readFromUartThread), nullptr)) == nullptr)
//   if ((osThreadCreate(osThread(readFromUartThread), nullptr)) == nullptr)
    {
        term("readFromUartThread Error")
//        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }


//    osThreadDef(defaultTask, empty, osPriorityNormal, 0, configMINIMAL_STACK_SIZE*2);
//    defaultTaskHandle = osThreadCreate(osThread(defaultTask), nullptr);

//    Json::getInstance()->configStation();
//    if (Json::getInstance()->deserializeJsonFlag == Json::JsonFlags::OK)
//    {

//term("ip:")      term(Json::getInstance()->thisStation.ip)
//term("mask:")    term(Json::getInstance()->thisStation.mask)
//term("gateway:") term(Json::getInstance()->thisStation.gateway)


//        netInit(Json::getInstance()->thisStation.ip,
//                Json::getInstance()->thisStation.mask,
//                Json::getInstance()->thisStation.gateway);


//    asm volatile (
//                " mov r0, 0x4 \n"
//                " mov r1, %[msg] \n"
//                " bkpt #0xAB"
//                :
//                : [msg] "r" (msgOCD)
//                : "r0", "r1"
//    );


        osThreadDef(emptyThread, StartDefaultTask, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
        defaultTaskHandle = osThreadCreate(osThread(emptyThread), nullptr);

//        SAI::getInstance()->threadAudioInitId = osThreadCreate(osThread(audioInitThread), nullptr);

//        if ((GPIO::getInstance()->trackRingBufferThreadId = osThreadCreate(osThread(trackRingBufferThread), nullptr)) == nullptr)
//        {
//            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
//        }

//        if ((UdpJsonExch::getInstance()->recvUdpThreadId = osThreadCreate(osThread(recvUdpThread), nullptr)) == nullptr)
//        {
//            Debug::getInstance().dbg << __FUNCTION__ << " " << __LINE__ << " " << "\n";
//        }

//        firmwareInitThread();

//    }
//    else
//    {
//        term("deserializeJsonFlag  -  error")
//    }

//     WDTInit();  // не собирается
//    if ((osThreadCreate(osThread(StartWdtThread), nullptr)) == nullptr)
//    {
//        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
//    }


    //Тестовые потоки
    testLed1();
    testLed2();
//    testLed3();
//    testUART();


    //Debug пока не работает - выпилил везде из кода
    Debug::getInstance().dbg << "ee";


//    vTraceEnable(TRC_INIT);  //Разобраться что позволяет

    osKernelStart();

    while (1)
    {
        RS232::getInstance().term << __FUNCTION__ << " " << __LINE__ << " " << "\n";
    }

} //main



[[ noreturn ]]
static void trackRingBufferThread(void const *arg)
{
//    (void)arg;
//    while(true) {
//        osMutexWait(GPIO::getInstance()->mutexRingBufferRx_id, osWaitForever);
//        if (GPIO::getInstance()->ringBufferRx.size() != 0) {

//            GPIO::getInstance()->packageRx = GPIO::getInstance()->ringBufferRx.shift();
//            osMutexRelease(GPIO::getInstance()->mutexRingBufferRx_id);
//            if (!GPIO::getInstance()->testFlag) {
//                osMutexWait(UdpJsonExch::getInstance()->mutexCallControlId, osWaitForever);
//                UdpJsonExch::getInstance()->callControl->button(GPIO::getInstance()->packageRx);
//                osMutexRelease(UdpJsonExch::getInstance()->mutexCallControlId);
//            } else {
//                RS232::getInstance().term << "Button [" << GPIO::getInstance()->packageRx.payloadData << "] was pressed" << "\n";
//                GPIO::getInstance()->configLed(GPIO::getInstance()->packageRx.payloadData, true, 250, 250);
//            }
//        } else osMutexRelease(GPIO::getInstance()->mutexRingBufferRx_id);

//        osDelay(50);
//    }
}




#ifdef __cplusplus
extern "C" {
#endif

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Supply configuration update enable
    */
    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
    /** Configure the main internal regulator output voltage
    */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
    /** Macro to configure the PLL clock source
    */
    __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_LSI
                                |RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 10;
    RCC_OscInitStruct.PLL.PLLN = 384;
    RCC_OscInitStruct.PLL.PLLP = 2;
    RCC_OscInitStruct.PLL.PLLQ = 10;
    RCC_OscInitStruct.PLL.PLLR = 4;
    RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
    RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    RCC_OscInitStruct.PLL.PLLFRACN = 0;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                                |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
    RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSE, RCC_MCODIV_1);
    /** Enables the Clock Security System
    */
    HAL_RCC_EnableCSS();
}

void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SAI1;
  PeriphClkInitStruct.PLL2.PLL2M = 25;
  PeriphClkInitStruct.PLL2.PLL2N = 512;
  PeriphClkInitStruct.PLL2.PLL2P = 125;
  PeriphClkInitStruct.PLL2.PLL2Q = 125;
  PeriphClkInitStruct.PLL2.PLL2R = 125;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.Sai1ClockSelection = RCC_SAI1CLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

//static void MX_I2C1_Init(void)
//{

//  hi2c1.Instance = I2C1;
//  hi2c1.Init.Timing = 0x307075B1;
//  hi2c1.Init.OwnAddress1 = 0;
//  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//  hi2c1.Init.OwnAddress2 = 0;
//  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
//  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure Analogue filter
//  */
//  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure Digital filter
//  */
//  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
//  {
//    Error_Handler();
//  }


//}

static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x307075B1;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.Timing = 0x307075B1;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c3, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c3, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

static void MX_SAI1_Init(void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_8K;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockA1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockA1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockA1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK)
  {
    Error_Handler();
  }
  hsai_BlockB1.Instance = SAI1_Block_B;
  hsai_BlockB1.Init.AudioMode = SAI_MODESLAVE_RX;
  hsai_BlockB1.Init.Synchro = SAI_SYNCHRONOUS;
  hsai_BlockB1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockB1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockB1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockB1.Init.MonoStereoMode = SAI_STEREOMODE;
  hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockB1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

}

static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 15000;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

static void MX_UART7_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_UART7_CLK_ENABLE();

    __HAL_RCC_DMA1_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    /**UART7 GPIO Configuration
    PF6     ------> UART7_RX
    PF7     ------> UART7_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_UART7;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

//    /* UART7 DMA Init */
//    /* UART7_TX Init */
  huart7.Instance = UART7;
  huart7.Init.BaudRate = 115200;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  huart7.hdmarx = &hdma_uart7_rx;
  huart7.hdmatx = &hdma_uart7_tx;

  if (HAL_UART_Init(&huart7) != HAL_OK)
  {
      while (1) {
          RS232::getInstance().term << "UART Init Error!" << "\n";
      }
  }

////  if (HAL_UARTEx_SetTxFifoThreshold(&huart7, UART_TXFIFO_THRESHOLD_1_2) != HAL_OK)
////  {
////    Error_Handler();
////  }
////  if (HAL_UARTEx_SetRxFifoThreshold(&huart7, UART_RXFIFO_THRESHOLD_1_2) != HAL_OK)
////  {
////    Error_Handler();
////  }
////  if (HAL_UARTEx_EnableFifoMode(&huart7) != HAL_OK)
////  {
////    Error_Handler();
////  }
//  /* USER CODE BEGIN UART7_Init 2 */

//  /* USER CODE END UART7_Init 2 */

}

static void MX_DMA_Init(void)
{


}

//static void MX_FMC_Init(void)
//{

//  /* USER CODE BEGIN FMC_Init 0 */

//  /* USER CODE END FMC_Init 0 */

//  FMC_NORSRAM_TimingTypeDef Timing = {0};

//  /* USER CODE BEGIN FMC_Init 1 */

//  /* USER CODE END FMC_Init 1 */

//  /** Perform the SRAM1 memory initialization sequence
//  */
//  hsram1.Instance = FMC_NORSRAM_DEVICE;
//  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
//  /* hsram1.Init */
//  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
//  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
//  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
//  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_32;
//  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
//  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
//  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
//  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
//  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
//  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
//  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
//  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
//  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
//  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
//  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
//  /* Timing */
//  Timing.AddressSetupTime = 15;
//  Timing.AddressHoldTime = 15;
//  Timing.DataSetupTime = 255;
//  Timing.BusTurnAroundDuration = 15;
//  Timing.CLKDivision = 16;
//  Timing.DataLatency = 17;
//  Timing.AccessMode = FMC_ACCESS_MODE_A;
//  /* ExtTiming */

//  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
//  {
//    Error_Handler( );
//  }

//}

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
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, POW_DOWN_Pin|TEST_LED_Pin, GPIO_PIN_SET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOG, L4_Pin|L5_Pin|L6_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, L1_Pin|L2_Pin|L3_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pins : DET_48V_Pin UPR_MIC2_Pin CFG_UI0_Pin */
    GPIO_InitStruct.Pin = DET_48V_Pin|UPR_MIC2_Pin|CFG_UI0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : POW_DOWN_Pin TEST_LED_Pin */
    GPIO_InitStruct.Pin = POW_DOWN_Pin|TEST_LED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*Configure GPIO pins : L4_Pin L5_Pin L6_Pin */
    GPIO_InitStruct.Pin = L4_Pin|L5_Pin|L6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pin : I2C3_INT_Pin */
    GPIO_InitStruct.Pin = I2C3_INT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(I2C3_INT_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pin : PA8 */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : L1_Pin L2_Pin L3_Pin */
  GPIO_InitStruct.Pin = L1_Pin|L2_Pin|L3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : K1_Pin K2_Pin K3_Pin K4_Pin
                           K5_Pin K6_Pin */
    GPIO_InitStruct.Pin = K1_Pin|K2_Pin|K3_Pin|K4_Pin
                          |K5_Pin|K6_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

    /*Configure GPIO pin : TEST_BUT_Pin */
    GPIO_InitStruct.Pin = TEST_BUT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(TEST_BUT_GPIO_Port, &GPIO_InitStruct);

    /* EXTI interrupt init*/
    HAL_NVIC_SetPriority(EXTI2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

//    if ((osThreadCreate(osThread(switchLEDsThread), nullptr)) == nullptr)
//    {
//        RS232::getInstance().term << "Failed to create [switchLEDsThread]" << "\n";
//    }

    if ((osThreadCreate(osThread(readButtonThread), nullptr)) == nullptr)
    {
        RS232::getInstance().term << "Failed to create [readButtonThread]" << "\n";
    }

    if ((osThreadCreate(osThread(replaceTimerCallback), nullptr)) == nullptr)
    {
        RS232::getInstance().term << "Failed to create [switchLEDsThread]" << "\n";
    }

}

void StartDefaultTask(void const * argument)
{
    osDelay(3000);
    term("------- StartDefaultTask ----------")

    term("ip:")      term(Json::getInstance()->thisStation.ip)
    term("mask:")    term(Json::getInstance()->thisStation.mask)
    term("gateway:") term(Json::getInstance()->thisStation.gateway)


//  /* init code for LWIP */
  MX_LWIP_Init(Json::getInstance()->thisStation.ip, Json::getInstance()->thisStation.mask,Json::getInstance()->thisStation.gateway);
//  /* USER CODE BEGIN 5 */


  if (DP83848.Is_Initialized) {
    term("DP83848.Is_Initialized");
  } else {
    term("DP83848.no_Initialized");
  }

  for(;;)
  {
      ethernetif_input(&gnetif);
      sys_check_timeouts();
      Ethernet_Link_Periodic_Handle(&gnetif);

    osDelay(1);
  }
//  /* USER CODE END 5 */
}


//Добавил_CUBE_03_05_2022
/**
  * Enable MDMA controller clock
  * Configure MDMA for global transfers
  *   hmdma_mdma_channel41_dma2_stream3_tc_0
  */
//static void MX_MDMA_Init(void)
//{

//  /* MDMA controller clock enable */
//  __HAL_RCC_MDMA_CLK_ENABLE();
//  /* Local variables */

//  /* Configure MDMA channel MDMA_Channel1 */
//  /* Configure MDMA request hmdma_mdma_channel41_dma2_stream3_tc_0 on MDMA_Channel1 */
//  hmdma_memtomem_dma2_stream0.Instance = MDMA_Channel1;
//  hmdma_memtomem_dma2_stream0.Init.Request = MDMA_REQUEST_DMA2_Stream0_TC;
//  hmdma_memtomem_dma2_stream0.Init.TransferTriggerMode = MDMA_BUFFER_TRANSFER;
//  hmdma_memtomem_dma2_stream0.Init.Priority = MDMA_PRIORITY_HIGH;
//  hmdma_memtomem_dma2_stream0.Init.Endianness = MDMA_LITTLE_ENDIANNESS_PRESERVE;
//  hmdma_memtomem_dma2_stream0.Init.SourceInc = MDMA_SRC_INC_BYTE;
//  hmdma_memtomem_dma2_stream0.Init.DestinationInc = MDMA_DEST_INC_BYTE;
//  hmdma_memtomem_dma2_stream0.Init.SourceDataSize = MDMA_SRC_DATASIZE_BYTE;
//  hmdma_memtomem_dma2_stream0.Init.DestDataSize = MDMA_DEST_DATASIZE_BYTE;
//  hmdma_memtomem_dma2_stream0.Init.DataAlignment = MDMA_DATAALIGN_PACKENABLE;
//  hmdma_memtomem_dma2_stream0.Init.BufferTransferLength = 1;
//  hmdma_memtomem_dma2_stream0.Init.SourceBurst = MDMA_SOURCE_BURST_SINGLE;
//  hmdma_memtomem_dma2_stream0.Init.DestBurst = MDMA_DEST_BURST_SINGLE;
//  hmdma_memtomem_dma2_stream0.Init.SourceBlockAddressOffset = 0;
//  hmdma_memtomem_dma2_stream0.Init.DestBlockAddressOffset = 0;
//  if (HAL_MDMA_Init(&hmdma_memtomem_dma2_stream0) != HAL_OK)
//  {
//    Error_Handler();
//  }

//  /* Configure post request address and data masks */
//  if (HAL_MDMA_ConfigPostRequestMask(&hmdma_memtomem_dma2_stream0, 0, 0) != HAL_OK)
//  {
//    Error_Handler();
//  }

//  /* MDMA interrupt initialization */
//  /* MDMA_IRQn interrupt configuration */
//  HAL_NVIC_SetPriority(MDMA_IRQn, 5, 0);
//  HAL_NVIC_EnableIRQ(MDMA_IRQn);

//}



void MPU_Config(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    /* Disables the MPU */
    HAL_MPU_Disable();
    /** Initializes and configures the Region and the memory to be protected
    */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER0;
    MPU_InitStruct.BaseAddress = 30000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
    MPU_InitStruct.SubRegionDisable = 0x0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /** Initializes and configures the Region and the memory to be protected
    */
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.BaseAddress = 0x30000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_256B;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /** Initializes and configures the Region and the memory to be protected
    */
    MPU_InitStruct.Number = MPU_REGION_NUMBER2;
    MPU_InitStruct.BaseAddress = 0x30040000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    /* Enables the MPU */
    HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

#ifdef __cplusplus
}
#endif
