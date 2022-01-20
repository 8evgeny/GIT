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
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "i2c_SW.h"
#include "memtest.h"
#include "cli_io.h"
#include "driver_pinIO.h"
#include "driver_ui.h"
#include "driver_AIC.h"
#include "driver_EEPROM.h"
#include "driver_extRAM.h"
#include "CLI_CmdDeviceFn.h"
#include "CLI_CmdTestFn.h"
#include "CLI_CmdServiceFn.h"
#include "audio_process.h"
#include "cbuffer.h"
#include "tone_gen_state_processor.h"
#include "dp83848.h"
#include "ethernetif_test.h"
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
#if defined ( __ICCARM__ ) /*!< IAR Compiler */

#pragma location=0x30040000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x30040060
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */
#pragma location=0x300400c0
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffers */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x30040000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x30040060))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */
__attribute__((at(0x300400c0))) uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE]; /* Ethernet Receive Buffer */

#elif defined ( __GNUC__ ) /* GNU Compiler */

ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDecripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDecripSection")));   /* Ethernet Tx DMA Descriptors */
uint8_t Rx_Buff[ETH_RX_DESC_CNT][ETH_MAX_PACKET_SIZE] __attribute__((section(".RxArraySection"))); /* Ethernet Receive Buffers */

#endif

ETH_TxPacketConfig TxConfig;

ETH_HandleTypeDef heth;

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

extern uint32_t Cur_PHYLinkState;
extern uint32_t aic_smp_cnt;

uint8_t EthTx[256] __attribute__((section(".lwip_pool")));
ETH_BufferTypeDef EthRx[ETH_RX_DESC_CNT] __attribute__((section(".useful_ramd2")));

const uint8_t test_eth_UDP[] = {0x31,0x32,0x33,0x01,0x02,0x03, 0x00,0x00,0x00,0x00,0x00,0x00, 0x08,0x00,
								0x45,0x00, 0x00,0xF2, 0x12,0x34, 0x40,0x00, 0x7F, 0x11, 0x00,0x00, 0xc0,0xa8,0xa9,0x01, 0xc0,0xa8,0xa9,0xfe,
								0x30,0x39, 0x30,0x39, 0x00,0xDE, 0x00,0x00
						};

const uint8_t test_eth_ARP[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, 0x00,0x00,0x00,0x00,0x00,0x00, 0x08,0x06,
								0x00,0x01, 0x08,0x00, 0x06,0x04, 0x00,0x01,
								0x31,0x32,0x33,0x00,0x00,0x00, 0xc0,0xa8,0xa9,0x01,
								0x00,0x00,0x00,0x00,0x00,0x00, 0xc0,0xa8,0xa9,0xfe
						};

void print_ETH_paket(uint8_t *data) {
	CLI_print("------------PACKECT INFO-------------\r\n");
	CLI_print("SRC MAC %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\r\n", data[0],data[1],data[2],data[3],data[4],data[5]);
	CLI_print("DST MAC %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\r\n", data[6],data[7],data[8],data[9],data[10],data[11]);
	CLI_print("TYPE %.2X%.2X\r\n", data[12],data[13]);
	if (data[13]==0) {
	 CLI_print("IP TYPE %.2X\r\n", data[23]);
	 CLI_print("IP SRC %.2X.%.2X.%.2X.%.2X\r\n", data[26],data[27],data[28],data[29]);
	 CLI_print("IP DST %.2X.%.2X.%.2X.%.2X\r\n", data[30],data[31],data[32],data[33]);
	} else if (data[13]==6) {
	 CLI_print("ARP IPSRC %.2X.%.2X.%.2X.%.2X\r\n", data[28],data[29],data[30],data[31]);
	 CLI_print("ARP IPDST %.2X.%.2X.%.2X.%.2X\r\n", data[38],data[39],data[40],data[41]);
	}
}

extern CBuffer audio_mic_input;
extern CBuffer audio_mix_output;
extern CBuffer audio_mix_channels[AUDIO_MAX_MIX_CHANNELS];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ETH_Init(void);
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

	for (i=0;i<LED_NUM;i++) ui_setledstate(i, LED_STATE_FL1);

	ledtimeout = HAL_GetTick();
	while ((HAL_GetTick() - ledtimeout) < TIME_PDO_LED_START) ui_task();

	for (i=0;i<LED_NUM;i++) ui_setledstate(i, LED_STATE_OFF);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t tmp_u8,btn_idx, btn_st;
  //uint8_t temp_buf[10];
  uint32_t loop_cnt, res;
  //uint8_t TEMP_io[8];
  uint32_t TEMP_eth, test_phy, test_eth;
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
  MX_ETH_Init();
  MX_FMC_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_I2C3_Init();
  MX_SAI1_Init();
  MX_UART7_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  pdo_work_mode = WORK_MODE_UDEF;
  pdo_hwerror = 0;

  CLI_init();

  CLI_print("v-----------v\r\n");
  CLI_print("_Hello_World_\r\n");
  CLI_print("GIT-Comm IPS\r\n");
  CLI_print("Test SC4 firmware\r\n");

  //__HAL_RCC_GPIOH_CLK_ENABLE();
  //__HAL_RCC_GPIOC_CLK_ENABLE();
  //I2C_Init();

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
   //aic_setSAIloop(1);
   CLI_print("Test AIC : OK\r\n");
   audio_init();
  }
  tone_genInit();

  pinio_set_UPR_SP(UPR_STATE_OFF);
  pinio_set_UPR_FAN(UPR_STATE_OFF);
  pinio_set_UPR_RELE(UPR_STATE_OFF);

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

  ethernetif_init();
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

  memset(&EthTx[0], 0 , 256);
  //memcpy(&EthTx[0],&test_eth_UDP[0],sizeof(test_eth_UDP));
  memcpy(&EthTx[0],&test_eth_ARP[0],sizeof(test_eth_ARP));

  test_phy = 0;
  test_eth = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  CLI_uart_task();

	  ui_task();

	  test_task();

	  if (pdo_work_mode == WORK_MODE_OK) {
	   if (service_getmode()==svcNone) {
	    aic_task();

	    audio_process();
	   }
	  }

	  tone_genProcess();

	  if (btn_wait_time) {
		if ((HAL_GetTick() - btn_wait_time) > TIME_BTN_WAIT) { ui_set_block_kbd(0); btn_wait_time = 0;}
	  } else
	   while ((btn_idx=ui_getactivebtn())!=BTN_NONE) {
		  btn_st = ui_getbtnval(btn_idx);
		  btn_wait_time =  HAL_GetTick();	// lock keys (fast press protection)
		  ui_set_block_kbd(1);
		  CLI_print_lev(CLI_PRINTLEVEL_SVCDEBUG, "BTN %.2X ",btn_idx,btn_st);
		  if (btn_st == BTN_VAL_DOWN)
			  CLI_print_lev(CLI_PRINTLEVEL_SVCDEBUG, "DOWN\r\n");
		  else
			  CLI_print_lev(CLI_PRINTLEVEL_SVCDEBUG, "UP\r\n");

		  if (btn_idx == BTN_VOLUP) {
			if (btn_st == BTN_VAL_DOWN) {
				aic_setOutVolUp();
			}
		  } else
		  if (btn_idx == BTN_VOLDN) {
			if (btn_st == BTN_VAL_DOWN) {
				aic_setOutVolDown();
			}
		  } else
		  if (btn_idx == BTN_MICUP) {
			if (btn_st == BTN_VAL_DOWN) {
				aic_setInVolUp();
			}
		  } else
		  if (btn_idx == BTN_MICDN) {
			if (btn_st == BTN_VAL_DOWN) {
				aic_setInVolDown();
			}
		  };

		  if (btn_idx == BTN_TEST) {
			if (btn_st == BTN_VAL_DOWN) {
				if (test_get_mode()&TEST_MODE_ON)
					fnStopTestMode(0,0);
				else
					fnStartTestMode(0,0);
			}
		  };

		  if (btn_idx == BTN_AB1) {
			if (btn_st == BTN_VAL_DOWN) {
				test_phy ^= 1;
			}
		  };
		  if (btn_idx == (BTN_AB1+1)) {
			if (btn_st == BTN_VAL_DOWN) {
				test_eth ^= 1;
			}
		  };
	  }

	  tmp_u8 = pinio_getstate_DET_PHONE();
	  if (tmp_u8!=DET_STATE_IDLE) {
		  if (tmp_u8 == DET_STATE_ON) {
			CLI_print("EXT_PHONE ON\r\n");
			aic_setOutDev(AIC_OUTDEV_PHONE);
			pinio_set_UPR_SP(UPR_STATE_OFF);
		  } else {
			CLI_print("EXT_PHONE OFF\r\n");
			aic_setOutDev(AIC_OUTDEV_GR);
		  }
	  }

	  tmp_u8 = pinio_getstate_DET_MIC();
	  if (tmp_u8!=DET_STATE_IDLE) {
		  if (tmp_u8 == DET_STATE_ON) {
			CLI_print("EXT_MIC ON\r\n");
			aic_setInDev(AIC_INDEV_EXTMIC);
		  } else {
			CLI_print("EXT_MIC OFF\r\n");
			aic_setInDev(AIC_INDEV_INTMIC);
		  }
	  }

	  if ((HAL_GetTick() - print_time) > TIME_DEBUG_PRINT)
	  {
		  loop_cnt++;
		  CLI_print("--------------\r\nWHILE LOOP %lu\r\n",loop_cnt);

		  //CLI_print("aic_smp_cnt: %lu\r\n",aic_smp_cnt);
		  aic_smp_cnt = 0;

		  CLI_print("mixer %i %i %i \r\n",audio_mic_input.datalen,audio_mix_output.datalen,audio_mix_channels[0].datalen);

		  ethernet_link_check_state();
		  res = DP83848_GetLinkState(&DP83848);
		  CLI_print("ETH:%lu\r\n",res);

		  if (test_phy)
		  {
		   res = HAL_ETH_ReadPHYRegister(&heth, DP83848_DEV_ADDR, DP83848_BMCR, &TEMP_eth);
		   CLI_print("ETH 01 %.8X %.4X\r\n",res, TEMP_eth);
		   res = HAL_ETH_ReadPHYRegister(&heth, DP83848_DEV_ADDR, DP83848_BMSR, &TEMP_eth);
		   CLI_print("ETH 02 %.8X %.4X\r\n",res, TEMP_eth);
		   res = HAL_ETH_ReadPHYRegister(&heth, DP83848_DEV_ADDR, DP83848_ANAR, &TEMP_eth);
		   CLI_print("ETH 03 %.8X %.4X\r\n",res, TEMP_eth);
		   res = HAL_ETH_ReadPHYRegister(&heth, DP83848_DEV_ADDR, DP83848_ANLPAR, &TEMP_eth);
		   CLI_print("ETH 03 %.8X %.4X\r\n",res, TEMP_eth);
		   res = HAL_ETH_ReadPHYRegister(&heth, DP83848_DEV_ADDR, DP83848_PHYSTS, &TEMP_eth);
		   CLI_print("ETH 03 %.8X %.4X\r\n",res, TEMP_eth);
		   res = HAL_ETH_ReadPHYRegister(&heth, DP83848_DEV_ADDR, DP83848_FCSCR, &TEMP_eth);
		   CLI_print("ETH 03 %.8X %.4X\r\n",res, TEMP_eth);
		   res = HAL_ETH_ReadPHYRegister(&heth, DP83848_DEV_ADDR, DP83848_RECR, &TEMP_eth);
		   CLI_print("ETH 03 %.8X %.4X\r\n",res, TEMP_eth);
		  }

		  if (test_eth) {
		   if ((Cur_PHYLinkState>DP83848_STATUS_LINK_DOWN)&&(Cur_PHYLinkState<DP83848_STATUS_AUTONEGO_NOTDONE)) {
			  //EthTx[19] = loop_cnt & 0xFF;
			  //low_level_output(&EthTx[0], 256);
			  low_level_output(&EthTx[0], sizeof(test_eth_ARP));
		   }
		  }

		  print_time = HAL_GetTick();
	  }

	  if (test_eth) {
	   TEMP_eth = ethernetif_input(&EthRx[0]);
	   if (TEMP_eth) {
		   CLI_print("ETH RX %i\r\n",TEMP_eth);
	       print_ETH_paket(EthRx[0].buffer);
	   }
	  }

	  //-------------------------------------------------
	  /*
	  res = HAL_I2C_Mem_Read(&hi2c3, 0x40, 0x04,I2C_MEMADD_SIZE_8BIT,TEMP_io, 2, 100);
	  printf("MCP1 R %.2X %.2X %.2X\r\n",res, TEMP_io[0], TEMP_io[1]);
	  res = HAL_I2C_Mem_Read(&hi2c3, 0x40, 0x0C,I2C_MEMADD_SIZE_8BIT,TEMP_io, 2, 100);
	  printf("MCP1 R %.2X %.2X %.2X\r\n",res, TEMP_io[0], TEMP_io[1]);
	  res = HAL_I2C_Mem_Read(&hi2c3, 0x40, 0x0A,I2C_MEMADD_SIZE_8BIT,TEMP_io, 1, 100);
	  printf("MCP1 R %.2X %.2X\r\n",res, TEMP_io[0]);
	  */

	  //res = HAL_I2C_Mem_Read(&hi2c3, 0xC0, 0x94,I2C_MEMADD_SIZE_8BIT,TEMP_io, 2, 100);
	  //printf("TLC1 R %.2X %.2X %.2X\r\n",res, TEMP_io[0], TEMP_io[1]);
	  //res = HAL_I2C_Mem_Read(&hi2c3, 0xC0, 0x80,I2C_MEMADD_SIZE_8BIT,TEMP_io, 1, 100);
	  //printf("TLC1 R %.2X %.2X\r\n",res, TEMP_io[0]);
	  //--------------------------------------------------------
	  //HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
	  //res = HAL_I2C_Mem_Read(&hi2c3, 0x40, 0x10,I2C_MEMADD_SIZE_8BIT,TEMP_io, 2, 100);
	  //CLI_print("MCP1 AB %.2X %.2X %.2X\r\n",res, TEMP_io[0], TEMP_io[1]);
	  //res = HAL_I2C_Mem_Read(&hi2c3, 0x40, 0x12,I2C_MEMADD_SIZE_8BIT,TEMP_io, 2, 100);
	  //CLI_print("MCP1 CP %.2X %.2X %.2X\r\n",res, TEMP_io[0], TEMP_io[1]);
	  //HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	  //res = HAL_I2C_Mem_Read(&hi2c3, 0x4E, 0x12,I2C_MEMADD_SIZE_8BIT,TEMP_io, 1, 100);
	  //printf("MCP2 A %.2X %.2X\n",res, TEMP_io[0]);
	  //res = HAL_I2C_Mem_Read(&hi2c3, 0x4E, 0x13,I2C_MEMADD_SIZE_8BIT,TEMP_io, 1, 100);
	  //printf("MCP2 B %.2X %.2X\n",res, TEMP_io[0]);

	  //delay_us(200000);

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
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x30;
  MACAddr[1] = 0x31;
  MACAddr[2] = 0x32;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_MII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

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
//  hi2c3.Init.Timing = 0x0024C3C7;
//	hi2c3.Init.Timing = 0x00300B29;

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

  /* USER CODE BEGIN SAI1_Init 1 */

  hsai_BlockA1.Init.MckOverSampling = SAI_MCK_OVERSAMPLING_ENABLE;

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
  HAL_GPIO_WritePin(GPIOA, UPR1_SP_Pin|MKVKL_UPR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, UPR2_FAN_Pin|RELE_UPR_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(POW_DOWN_GPIO_Port, POW_DOWN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(NORMA_UPR_GPIO_Port, NORMA_UPR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TEST_LED_GPIO_Port, TEST_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DET_48V_Pin UPR_PHONE_Pin UPR_MIC2_Pin CFG_UI0_Pin
                           CFG_UI1_Pin CFG_UI2_Pin INT_BUT_Pin */
  GPIO_InitStruct.Pin = DET_48V_Pin|UPR_PHONE_Pin|UPR_MIC2_Pin|CFG_UI0_Pin
                          |CFG_UI1_Pin|CFG_UI2_Pin|INT_BUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : UPR1_SP_Pin UPR2_FAN_Pin MKVKL_UPR_Pin RELE_UPR_Pin */
  GPIO_InitStruct.Pin = UPR1_SP_Pin|UPR2_FAN_Pin|MKVKL_UPR_Pin|RELE_UPR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
  printf(">>>HAL Error handler<<<< : Critical hardware init fail\r\n");
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

