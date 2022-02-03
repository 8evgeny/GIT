/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/** @defgroup main Main program module
  * @brief Main program implementation
  * @{
  */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lwip.h"
#include "driver_AIC.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "CLI_io.h"
#include "driver_pinIO.h"
#include "driver_UI.h"
#include "driver_AIC.h"
#include "driver_EEPROM.h"
#include "driver_extRAM.h"
#include "CLI_CmdDeviceFn.h"
#include "CLI_CmdTestFn.h"
#include "CLI_CmdServiceFn.h"
#include "audio_process.h"
#include "tone_gen_state_processor.h"
#include "dp83848.h"
#include "system_settings.h"
#include "udp_exchange.h"
#include "connect_manager.h"
#include "cbuffer.h"
#include "system_settings.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c2;
I2C_HandleTypeDef hi2c3;

SAI_HandleTypeDef hsai_BlockA1;
SAI_HandleTypeDef hsai_BlockB1;
DMA_HandleTypeDef hdma_sai1_a;
DMA_HandleTypeDef hdma_sai1_b;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart7;

SRAM_HandleTypeDef hsram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FMC_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C2_Init(void);
static void MX_I2C3_Init(void);
static void MX_SAI1_Init(void);
static void MX_UART7_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/// debug print time
#define TIME_DEBUG_PRINT	1000
/// button delay time
#define TIME_BTN_WAIT		200

/// time for debug print
uint32_t print_time;
/// time for button delay
uint32_t btn_wait_time;

/// link to PHY controller control/status structure
extern DP83848_Object_t DP83848;
//extern unsigned int received_pcount; // for debug

/// buffer for processing Ethernet packets
uint8_t eth_pkt_cmd_data[1200];

#define PDO_HWERROR_MEM		1
#define PDO_HWERROR_AIC		2
#define PDO_HWERROR_EEPROM	4
#define PDO_HWERROR_PHY		8

/// PDO working mode
uint8_t pdo_work_mode;
/// PDO hardware error flags
uint8_t pdo_hwerror;

/**
  * @brief  get PDO work mode
  * @retval result - PDO work mode
  */
uint8_t get_pdo_work_mode(){
	return pdo_work_mode;
}

/**
  * @brief  set LED to display hardware error
  * @retval none
  */
void pdo_led_hwerror(){
	if (pdo_hwerror&PDO_HWERROR_MEM) ui_setledstate(LED_AB1R + 4, LED_STATE_FL1);
	if (pdo_hwerror&PDO_HWERROR_AIC) ui_setledstate(LED_AB1R + 6, LED_STATE_FL1);
	if (pdo_hwerror&PDO_HWERROR_EEPROM) ui_setledstate(LED_AB1R + 8, LED_STATE_FL1);
	if (pdo_hwerror&PDO_HWERROR_PHY) ui_setledstate(LED_AB1R + 10, LED_STATE_FL1);
}

/// time to display start sequence
#define TIME_PDO_LED_START	1000

/**
  * @brief  set LED to display start sequence
  * @retval none
  */
void pdo_led_start_task()
{
	uint8_t i;
	uint32_t ledtimeout;

	for (i=0;i<LED_NUM;i++) ui_setledstate(i, LED_STATE_FL2);

	ledtimeout = HAL_GetTick();
	while ((HAL_GetTick() - ledtimeout) < TIME_PDO_LED_START) ui_task();

	for (i=0;i<LED_NUM;i++) ui_setledstate(i, LED_STATE_OFF);
}

//extern uint32_t ec_max_energy;

/* USER CODE END 0 */


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t btn_idx, btn_st;
  uint32_t loop_cnt;
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FMC_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_SAI1_Init();
  MX_UART7_Init();
  MX_TIM3_Init();
  //MX_LWIP_Init();
  /* USER CODE BEGIN 2 */

  pdo_work_mode = WORK_MODE_UDEF;
  pdo_hwerror = 0;

  CLI_init();

  CLI_print("v----------v\r\n");
  CLI_print("GIT-Comm IPS\r\n");
  CLI_print("Starting PDO SC2 firmware...\r\n");

  ui_init();

  ui_setledstate(LED_TEST, LED_STATE_ON);

  test_init();
  
  printDeviceID();

  if (extram_init()<0) {
	  pdo_work_mode = WORK_MODE_HWFAIL;
	  pdo_hwerror |= PDO_HWERROR_MEM;
  }

  if (aic_init()<0) {
	  pdo_work_mode = WORK_MODE_HWFAIL;
	  pdo_hwerror |= PDO_HWERROR_AIC;
  } else {
      //aic_setSAIloop(1);	// disable to enable audiomixer out
	  CLI_print("Test AIC : OK\r\n");
	  audio_init();
  }
  tone_genInit();
#ifdef SC_4
  pinio_set_UPR_SP(UPR_STATE_ON);
  pinio_set_UPR_FAN(UPR_STATE_OFF);
  pinio_set_UPR_RELE_SC4(UPR_STATE_OFF);
#endif
  if (eeprom_init() == -1) {
	  pdo_work_mode = WORK_MODE_HWFAIL;
	  pdo_hwerror |= PDO_HWERROR_EEPROM;
  } else {
	  CLI_print("Test EEPROM : OK\r\n");
	  if (sysset_init()<0) {
		  if (pdo_work_mode != WORK_MODE_HWFAIL) pdo_work_mode = WORK_MODE_NOCFG;
	  } else {
		  CLI_print("Settings loaded from EEPROM\r\n");
		  checkFirmware();
		  if (pdo_work_mode != WORK_MODE_HWFAIL) pdo_work_mode = WORK_MODE_OK;
	  }
  }

  MX_LWIP_Init();
  if (DP83848.Is_Initialized) {
	CLI_print("Test PHY : OK\r\n");
  } else {
	CLI_print("Test PHY : phy init fail\r\n");
	pdo_work_mode = WORK_MODE_HWFAIL;
	pdo_hwerror |= PDO_HWERROR_PHY;
  }

  if (pdo_work_mode == WORK_MODE_OK) {
	  pdo_led_start_task();
	  audio_start();
  } else
  if (pdo_work_mode == WORK_MODE_NOCFG) {
	  ui_setledstate(LED_AB1R, LED_STATE_FL1);
	  ui_setledstate(LED_AB1R + 2, LED_STATE_FL1);
  } else {
	  ui_setledstate(LED_AB1R, LED_STATE_FL1);
	  ui_setledstate(LED_AB1R + 2, LED_STATE_FL3);
	  pdo_led_hwerror();
  }

  ui_setledstate(LED_TEST, LED_STATE_OFF);
  ui_setledstate(LED_NORMA, LED_STATE_FL3);
  ui_setledstate(LED_MKVKL, LED_STATE_OFF);

  print_time = HAL_GetTick();
  loop_cnt = 0;
  btn_wait_time = 0;

  udp_ips_init(UDP_AUDIO_PORT, UDP_CTRL_PORT, UDP_CTRL_PORT);
  cmanager_init(CM_ARRAY_COUNT);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  aic_setADCInVolume(0x74); //Микрофон -6db
  aic_setDACOutVolume(0xB8);//Динамик  -36db
  while (1)
  {

	  CLI_uart_task();

	  ui_task();

	  test_task();

      #ifdef SC_2
      vol_GR = VOL_OUT_SC_2;
      vol_Phone = VOL_OUT_SC_2;
      vol_Mic = VOL_MIC_SC_2;
      #endif

	  if (pdo_work_mode == WORK_MODE_OK) {
	   if (service_getmode()==svcNone) {
	    aic_task();

	    audio_process();
	   }
	  }

	  tone_genProcess();

	  MX_LWIP_Process();

      if (btn_wait_time)
      {
	    if ((HAL_GetTick() - btn_wait_time) > TIME_BTN_WAIT) { ui_set_block_kbd(0); btn_wait_time = 0;}
	  } else


      while ((btn_idx=ui_getactivebtn())!=BTN_NONE)
      {
		  btn_st = ui_getbtnval(btn_idx);
		  btn_wait_time =  HAL_GetTick();
		  ui_set_block_kbd(1);
		  CLI_print_lev(CLI_PRINTLEVEL_TEST, "BTN %.2X ",btn_idx);
		  if (btn_st == BTN_VAL_DOWN)
			  CLI_print_lev(CLI_PRINTLEVEL_TEST, "DOWN\r\n");
		  else
			  CLI_print_lev(CLI_PRINTLEVEL_TEST, "UP\r\n");

		  if ((btn_idx >= (BTN_AB1))&&(btn_idx <= (BTN_AB48)))
		  {
		  	cmanager_process_button(btn_idx-8, btn_st);
		  } else


          #ifdef SC_4
          if (btn_idx == BTN_VOLUP)
          {
            if (btn_st == BTN_VAL_DOWN)
            {
		  	 aic_setOutVolUp();
		    }
		  } else
          if (btn_idx == BTN_VOLDN)
          {
            if (btn_st == BTN_VAL_DOWN)
            {
		  	 aic_setOutVolDown();
		    }
		  } else
          if (btn_idx == BTN_MICUP)
          {
            if (btn_st == BTN_VAL_DOWN)
            {
		  	 aic_setInVolUp();
		    }
		  } else
          if (btn_idx == BTN_MICDN)
          {
            if (btn_st == BTN_VAL_DOWN)
            {
		  	 aic_setInVolDown();
		  	}
          } else
          #endif

          if (btn_idx == BTN_TEST)
          {
            if (btn_st == BTN_VAL_DOWN)
            {
				if (test_get_mode()&TEST_MODE_ON)
					fnStopTestMode(0,0);
				else
					fnStartTestMode(0,0);
			}
		  }
	  }

	  btn_idx = pinio_getstate_DET_PHONE();
	  	  if (btn_idx!=DET_STATE_IDLE) {
	  		  if (btn_idx == DET_STATE_ON) {
	  			CLI_print_lev(CLI_PRINTLEVEL_SERVICE, "EXT_PHONE ON\r\n");
	  			//aic_setOutDev(AIC_OUTDEV_PHONE);
                #ifdef SC_4
	  			pinio_set_UPR_SP(UPR_STATE_OFF);
                #endif
	  		  } else {
	  			CLI_print_lev(CLI_PRINTLEVEL_SERVICE, "EXT_PHONE OFF\r\n");
	  			//aic_setOutDev(AIC_OUTDEV_GR);
                #ifdef SC_4
	  			pinio_set_UPR_SP(UPR_STATE_ON);
                #endif
	  		  }
	  	  }

	  	btn_idx = pinio_getstate_DET_MIC();
		  if (btn_idx!=DET_STATE_IDLE) {
			  if (btn_idx == DET_STATE_ON) {
				CLI_print_lev(CLI_PRINTLEVEL_SERVICE, "EXT_MIC ON\r\n");
				aic_setInDev(AIC_INDEV_EXTMIC);
			  } else {
				CLI_print_lev(CLI_PRINTLEVEL_SERVICE, "EXT_MIC OFF\r\n");
				aic_setInDev(AIC_INDEV_INTMIC);
			  }
		  }

	  if ((HAL_GetTick() - print_time) > TIME_DEBUG_PRINT)
	  {
	  	loop_cnt++;
	  	//CLI_print("--------------\r\nWHILE LOOP %lu\r\n",loop_cnt);
	  	//CLI_print("Packet freq: %d \r\n", received_pcount);

	  	//CLI_print("GR Max: %d \r\n", ec_GR_max_energy);
	  	//CLI_print("MIC Max: %d \r\n", ec_MIC_max_energy);
	  	//ec_GR_max_energy = 0;
		//ec_MIC_max_energy = 0;

	  	//received_pcount = 0;

	  	print_time = HAL_GetTick();
	  }

	  uint16_t cmdcfg_next_size = CmdCfgBuffer_GetNextPktSize();

	  if (cmdcfg_next_size)
	  {
		cmdcfg_pckt_read(&eth_pkt_cmd_data[0], cmdcfg_next_size); // <---reading next cmd packet to cmd_data
		eth_pkt_cmd_data[cmdcfg_next_size] = 0;
		if (memcmp(&eth_pkt_cmd_data[0], "CMD : ", 6) == 0) // <--- condition if cmd package arrived
		{
			//CLI_print_lev(CLI_PRINTLEVEL_SVCDEBUG, "IP CMD pktsize: %i \r\n", cmdcfg_next_size);
			cmanager_process_cmd(&eth_pkt_cmd_data[6], cmdcfg_next_size - 6);
		}
		else if (memcmp(&eth_pkt_cmd_data[0], "CONFIG : ", 9) == 0) // <--- condition if cfg package arrived
		{
			//CLI_print_lev(CLI_PRINTLEVEL_SVCDEBUG, "IP CONFIG pktsize: %i \r\n", cmdcfg_next_size);
			CLI_ethernet_input(&eth_pkt_cmd_data[0], cmdcfg_next_size, 0);
		}
	  }

	  cmanager_task();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
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

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
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

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x307075B1;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
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

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
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

/**
  * @brief SAI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SAI1_Init(void)
{

  /* USER CODE BEGIN SAI1_Init 0 */

  /* USER CODE END SAI1_Init 0 */

  hsai_BlockA1.Init.MckOverSampling = SAI_MCK_OVERSAMPLING_ENABLE;

  /* USER CODE BEGIN SAI1_Init 1 */

  /* USER CODE END SAI1_Init 1 */
  hsai_BlockA1.Instance = SAI1_Block_A;
  hsai_BlockA1.Init.AudioMode = SAI_MODEMASTER_TX;
  hsai_BlockA1.Init.Synchro = SAI_ASYNCHRONOUS;
  hsai_BlockA1.Init.OutputDrive = SAI_OUTPUTDRIVE_DISABLE;
  hsai_BlockA1.Init.NoDivider = SAI_MASTERDIVIDER_ENABLE;
  hsai_BlockA1.Init.FIFOThreshold = SAI_FIFOTHRESHOLD_EMPTY;
  hsai_BlockA1.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_MCKDIV;
  hsai_BlockA1.Init.Mckdiv = 1;
  hsai_BlockA1.Init.SynchroExt = SAI_SYNCEXT_DISABLE;
  hsai_BlockA1.Init.MonoStereoMode = SAI_MONOMODE;
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
  hsai_BlockB1.Init.MonoStereoMode = SAI_MONOMODE;
  hsai_BlockB1.Init.CompandingMode = SAI_NOCOMPANDING;
  hsai_BlockB1.Init.TriState = SAI_OUTPUT_NOTRELEASED;
  if (HAL_SAI_InitProtocol(&hsai_BlockB1, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SAI1_Init 2 */

  /* USER CODE END SAI1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
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

/**
  * @brief UART7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART7_Init(void)
{

  /* USER CODE BEGIN UART7_Init 0 */

  /* USER CODE END UART7_Init 0 */

  /* USER CODE BEGIN UART7_Init 1 */

  /* USER CODE END UART7_Init 1 */
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
  if (HAL_UART_Init(&huart7) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart7, UART_TXFIFO_THRESHOLD_1_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart7, UART_RXFIFO_THRESHOLD_1_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_EnableFifoMode(&huart7) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART7_Init 2 */

  /* USER CODE END UART7_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FMC_NORSRAM_DEVICE;
  hsram1.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_32;
  hsram1.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
  hsram1.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
  hsram1.Init.WriteFifo = FMC_WRITE_FIFO_ENABLE;
  hsram1.Init.PageSize = FMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 15;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 255;
  Timing.BusTurnAroundDuration = 15;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
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
  #ifdef SC_4
  HAL_GPIO_WritePin(GPIOA, UPR1_SP_Pin|MKVKL_UPR_Pin, GPIO_PIN_RESET);
  #endif

  /*Configure GPIO pin Output Level */
  #ifdef SC_4
  HAL_GPIO_WritePin(GPIOA, UPR2_FAN_Pin|RELE_UPR_Pin, GPIO_PIN_SET);
  #endif

  /*Configure GPIO pin Output Level */
  #ifdef SC_2
  HAL_GPIO_WritePin(RELE1_UPR_Port, RELE1_UPR_Pin, GPIO_PIN_SET);
  #endif


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(POW_DOWN_GPIO_Port, POW_DOWN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  #ifdef SC_4
  HAL_GPIO_WritePin(NORMA_UPR_GPIO_Port, NORMA_UPR_Pin, GPIO_PIN_RESET);
  #endif

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED1, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, LED2, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, LED3, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOG, LED4, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOG, LED5, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOG, LED6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DET_48V_Pin UPR_PHONE_Pin UPR_MIC2_Pin */
  GPIO_InitStruct.Pin = DET_48V_Pin|UPR_PHONE_Pin|UPR_MIC2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : POW_DOWN_Pin TEST_LED_Pin */
  GPIO_InitStruct.Pin = POW_DOWN_Pin|TEST_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  #ifdef SC_4
  /*Configure GPIO pins : INT_BUT_Pin CFG_UI0_Pin CFG_UI1_Pin CFG_UI2_Pin*/
  GPIO_InitStruct.Pin = INT_BUT_Pin|CFG_UI0_Pin|CFG_UI1_Pin|CFG_UI2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : UPR1_SP_Pin UPR2_FAN_Pin MKVKL_UPR_Pin RELE_UPR_Pin */
  GPIO_InitStruct.Pin = UPR1_SP_Pin|UPR2_FAN_Pin|MKVKL_UPR_Pin|RELE_UPR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : VOL_UP_Pin VOL_DOWN_Pin SENS_UP_Pin SENS_DOWN_Pin */
  GPIO_InitStruct.Pin = VOL_UP_Pin|VOL_DOWN_Pin|SENS_UP_Pin|SENS_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : NORMA_UPR_Pin */
  GPIO_InitStruct.Pin = NORMA_UPR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(NORMA_UPR_GPIO_Port, &GPIO_InitStruct);
  #endif

  #ifdef SC_2
  /*Configure GPIO pins : TANG_UPR_Pin PEDAL_UPR_Pin */
  GPIO_InitStruct.Pin = TANG_UPR_Pin|PEDAL_UPR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : K1 - K6 */
  GPIO_InitStruct.Pin = K1|K2|K3|K4|K5|K6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : I2C3_INT */
  GPIO_InitStruct.Pin = I2C3_INT;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(I2C3_INT_PORT, &GPIO_InitStruct);

  /*Configure GPIO pins : RELE1_UPR_Pin AMP_UPR_Pin */
  GPIO_InitStruct.Pin = RELE1_UPR_Pin|AMP_UPR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LED1 */
  GPIO_InitStruct.Pin = LED1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED2 */
  GPIO_InitStruct.Pin = LED2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED3 */
  GPIO_InitStruct.Pin = LED3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LED4 */
  GPIO_InitStruct.Pin = LED4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : LED5 */
  GPIO_InitStruct.Pin = LED5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : LED6 */
  GPIO_InitStruct.Pin = LED6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
  #endif

  /*Configure GPIO pin : TEST_BUT_Pin */
  GPIO_InitStruct.Pin = TEST_BUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TEST_BUT_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x30000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512B;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x30040000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32KB;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
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

/* USER CODE BEGIN End file */

/**
  * @}
  */

/* USER CODE END End file */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
