/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hal_init.h"
#include "i2cflash.h"
#include "codec.h"
#include "fttcpproto.h"
#include "string.h"

//#include <ipsUartDev.h>
#include <ipsUdp.h>
#include <monitor.h>
#include <telnetServer.h>
#include <ips_globals.hpp>
#include <debug3.h>

#include <ipssystem.hpp>


#include <apps.hpp>
#include <baseapp.hpp>
#include <ipc_pipe.hpp>
#include <ipcCLI.hpp>

#include <cli_apps.hpp>

#include <audio.hpp>
#include <audio_generators.hpp>

//#include <ITMacrocell.hpp>
#include <locatorUDP.hpp>
#include <system_c.h>
#include <system/System.h>


#include <system/configs/ip_setings.h>
#include <test_system.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

extern CRC_HandleTypeDef hcrc;
extern CRYP_HandleTypeDef hcryp;
extern HASH_HandleTypeDef hhash;

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;
#ifdef SC4BOPLA
extern I2C_HandleTypeDef hi2c3;
#endif
extern RNG_HandleTypeDef hrng;
extern RTC_HandleTypeDef hrtc;
extern SAI_HandleTypeDef hsai_BlockA1;
extern SAI_HandleTypeDef hsai_BlockB1;
extern DMA_HandleTypeDef hdma_sai1_a;
extern DMA_HandleTypeDef hdma_sai1_b;

extern TIM_HandleTypeDef htim5;

//extern UART_HandleTypeDef huart7;
//extern DMA_HandleTypeDef hdma_uart7_rx;
//extern DMA_HandleTypeDef hdma_uart7_tx;

extern SRAM_HandleTypeDef hsram1;

int counteee_g = 0;

//ipcSystem system_cpp;

ufw::System& system_cpp=ufw::System::instance();

/* Definitions for startupTask */
osThreadId_t startupTaskHandle;


void ipsStartup(void *argument);

uint8_t is_lwip_ready = 0;

//ipsUART uart7(&huart7);
ufw::System& ipsSystem::sys=ufw::System::instance();
ttY *ipsSystem::seial = ufw::ioITM::Instance().pointer();
ttY *ipsSystem::systerm = ipsSystem::seial;
ttY *ipsSystem::debug = ipsSystem::seial;
ttY *ipsSystem::monterm = ipsSystem::systerm;
monitorTask *ipsSystem::sysmonitor = NULL;

#ifdef UFW_TELNET_ON
ipcCLI ips_cli_processor;

sineAudioGenerator *test_sine_gen_ptr;
testUDPAudio *test_udp_audio_ptr;
#endif
audioDevice *h_adevice = nullptr;


/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
    {
    /* USER CODE BEGIN 1 */

    //perf_t perf_system;
    //_t_test_serialize_crc();

//    heap_check(1000);
//    flash_check();
    //ufw::TestSystem::Instance().TestAll();
    HAL_Delay(100);
    sysLedTestON();
    sysLedNormOFF();

    //system_cpp.memoryInit();
//  HAL_FLASH_OB_UnloINADDR_ANY;//ck();
    /* USER CODE END 1 */

    /* MPU Configuration--------------------------------------------------------*/
    //MPU_Config();
    /* Enable I-Cache---------------------------------------------------------*/
//    SCB_EnableICache();

    /* Enable D-Cache---------------------------------------------------------*/
//    SCB_EnableDCache();

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
//    HAL_Init();

    /* Configure the system clock */
    //SystemClock_Config();
    /* Migrate to HAL_BSP_Init */

    /* Initialize all configured peripherals */
//    HAL_BSP_Init();

    sysLedTestON();
    codecSetup();

//    sysAmpPwrOn();
    sysLedTestOFF();
    HAL_Delay(2000);
    /*
     uint32_t *ram_ptr_test=0x60000000;
     uint32_t ram_test_data[64];
     memset(&ram_test_data,0,64*sizeof(uint32_t));
     PERFCHECK_START
     for(uint32_t i=0;i<(1024*256);i++){
     *(ram_ptr_test+i)=0x00000000;
     sysLedTestToggle();
     }
     PERFCHECK_STOP(perf_system)
     for (uint32_t i=0;i<16;i++)
     {
     *(ram_ptr_test+(1<<i))=0xAAAAAAAA;
     HAL_Delay(1);
     ram_test_data[i]=*(ram_ptr_test+(1<<i));
     sysLedTestToggle();
     }
     */


//  uint8_t stick1,stick2,stick3,stat1,stat2;
//  codecReadFault(&stick1, &stick2, &stick3);
//  codecReadDACStatus(&stat1, &stat2);
//  stopPlaying();
    /* USER CODE END 2 */
    osThreadAttr_t startupTask_attributes;
    memset(&startupTask_attributes, 0, sizeof(osThreadAttr_t));
    startupTask_attributes.name = "Startup";
    startupTask_attributes.priority = (osPriority_t) osPriorityLow;
    startupTask_attributes.stack_size = 256 * 8;
    /* Init scheduler */
    osKernelInitialize();


    /* Create the thread(s) */
    /* creation of startupTask */
    startupTaskHandle = osThreadNew(ipsStartup, NULL, &startupTask_attributes);
#ifdef UFW_TELNET_ON
    static ipsTelnet telnet_server("Telnet TCP", &ips_cli_processor, 23, 8);
    cliApplications::makeAppList(&ips_cli_processor);
#endif

//  static sineAudioGenerator test_sine_gen;
//  test_sine_gen_ptr=&test_sine_gen;
    h_adevice = (audioDevice::getInstance()).getPtr();


    /* Start scheduler */
    osKernelStart();

    /* We should never get here as control is now taken by the scheduler */
    /* Infinite loop */

    while (1)
	{
	/* USER CODE END WHILE */
	}
    }

/* USER CODE BEGIN Header_ipsStartup */
/**
 * @brief  Function implementing the startupTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_ipsStartup */
void ipsStartup(void *argument)
    {
    HAL_TIM_Base_Start(&htim5);
    system_cpp.osStartedCallback();
    /* init code for LWIP */
    MX_LWIP_Init();
    system_cpp.ipStackReadyCallback();
    /* USER CODE BEGIN 5 */
    sysLedTestOFF();
    sysLedNormON();
    //is_lwip_ready=1;
//    locatorUDP::getInstance();
//    createFTransferServer();
    /* Infinite loop */
    //test_sine_audio();
    for (;;)
	{
//	system_cpp.servicesNet().announce_test->announce();
	osDelay(1000);
	}
    /* USER CODE END 5 */
    }

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM7 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
    {
    /* USER CODE BEGIN Callback 0 */

    /* USER CODE END Callback 0 */
    if (htim->Instance == TIM7)
	{
	HAL_IncTick();
	}
    /* USER CODE BEGIN Callback 1 */

    /* USER CODE END Callback 1 */
    }

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
    {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    sysLedTestON();
    sysLedNormOFF();
    int i=0;
    while (100<(i++))
	{
	HAL_Delay(100);
	sysLedNormToggle();
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
