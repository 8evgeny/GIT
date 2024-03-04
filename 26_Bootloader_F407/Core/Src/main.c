/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "config.h"
#include <string.h>

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
CRC_HandleTypeDef hcrc;

RNG_HandleTypeDef hrng;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi3;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart6;
DMA_HandleTypeDef hdma_usart6_tx;

/* USER CODE BEGIN PV */

FATFS fs;
FIL file;
//FIL destIP;
//FIL maskIP;
//FIL gateIP;
//FIL newFirmware;
extern uint32_t * _smem;
extern uint32_t * _sapp;
extern uint32_t * _eapp;
#define FLASH_APP_START_ADDESS (uint32_t) & _sapp
#define FLASH_APP_END_ADDRESS (uint32_t) & _eapp
#define FLASH_MEM_ADDRESS (uint32_t) & _smem
/* Function pointer for jumping to user application. */
typedef void (*pFunction)(void);
#define FLASH_SECTOR_0_ADDR      0x08000000          // Sector 0, 16 Kbytes
#define FLASH_SECTOR_1_ADDR      0x08004000          // Sector 1, 16 Kbytes
#define FLASH_SECTOR_2_ADDR      0x08008000          // Sector 2, 16 Kbytes
#define FLASH_SECTOR_3_ADDR      0x0800C000          // Sector 3, 16 Kbytes
#define FLASH_SECTOR_4_ADDR      0x08010000          // Sector 4, 64 Kbytes
#define FLASH_SECTOR_5_ADDR      0x08020000          // Sector 5, 128 Kbytes
#define FLASH_SECTOR_6_ADDR      0x08040000          // Sector 6, 128 Kbytes
#define FLASH_SECTOR_7_ADDR      0x08060000          // Sector 7, 128 Kbytes
#define FLASH_SECTOR_8_ADDR      0x08080000          // Sector 8, 128 Kbytes
#define FLASH_SECTOR_9_ADDR      0x080A0000          // Sector 9, 128 Kbytes
#define FLASH_SECTOR_10_ADDR     0x080C0000          // Sector 10, 128 Kbytes
#define FLASH_SECTOR_11_ADDR     0x080E0000          // Sector 11, 128 Kbytes
#define FLASH_LAST_ADDR          0x080FFFFE
#define APP_ADDR             FLASH_SECTOR_4_ADDR
#define FIRMWARE_SECTION_ADDR    APP_ADDR + 0x188
#define  NUM_WORLDS              12288
uint32_t bufFW[NUM_WORLDS];         // 12288 * 4 байт
#define   numSectorsErase        3

uint32_t flashReadWorld(uint32_t address){
    return (*(__IO uint32_t*)address);
}

void printFlash(uint32_t address, uint32_t numByte){
    for (int i = 0; i < numByte/4 ; ++i){
        printf("address: %X value: %.8X\r\n", address + i*4, flashReadWorld(address + i*4));
    }
}

void readDirSD(char * nameFirmware, uint32_t * sizeFirmware ){
    puts("\r\nDisplay Directory on SD\r");
      {
        DIR dir;
        char *path;
        char string[128];
        path = ""; // where you want to list
        uint8_t res = f_opendir(&dir, path);
        if (res != FR_OK){
//          printf("res = %d f_opendir\r\n", res);
            printf("SD not found\r\n");
        }
        if (res == FR_OK)
        {
          while(1)
          {
            FILINFO fno;
            res = f_readdir(&dir, &fno);

            if (res != FR_OK){
              printf("res = %d f_readdir\r\n", res);
            }

            if ((res != FR_OK) || (fno.fname[0] == 0))
              break;

            sprintf(string, "%c%c%c%c %10d %s/%s\r",
              ((fno.fattrib & AM_DIR) ? 'D' : '-'),
              ((fno.fattrib & AM_RDO) ? 'R' : '-'),
              ((fno.fattrib & AM_SYS) ? 'S' : '-'),
              ((fno.fattrib & AM_HID) ? 'H' : '-'),
              (int)fno.fsize, path, fno.fname);

            if (strstr(fno.fname, ".bin") != NULL) {
                strcpy(nameFirmware, fno.fname);
                *sizeFirmware = (int)fno.fsize;
            }
            puts(string);
          }
        }
      }
    puts("");
}
void firmware_run(void) {

    uint32_t appStack = (uint32_t) *((uint32_t *) APP_ADDR);
    pFunction appEntry = (pFunction) *(uint32_t *) (APP_ADDR + 4);
    SCB->VTOR = APP_ADDR;
    __set_MSP(appStack);
    appEntry();
}
int fw_check(void)
{
    extern void* _estack; // Это из линкера, генерируется автоматически и указывает на конец RAM (или стек)
//    printf("_estack:\t\t\t %p\r\n", (uint32_t*)&_estack);
    // Проверка первого адреса прошивки, значение в нем должно быть размером RAM (регистр SP)
//    if (((*(uint32_t*) FLASH_APP_START_ADDESS) & 0x2FFF8000) != &_estack) //Непонятно зачем &
    if ((*(uint32_t*) FLASH_APP_START_ADDESS) != &_estack)
    {
        printf("not found firmware on %p\r\n", (uint32_t*)&_sapp);
        return -1;
    }
    printf("in address %p found main Firmware\r\n", (uint32_t*)&_sapp);
    return 0;
}
int _write(int fd, char *str, int len)
{
    for(int i=0; i<len; i++)
    {
        HAL_UART_Transmit(&huart6, (uint8_t *)&str[i], 1, 0xFFFF);
    }
    return len;
}
uint8_t writeFlash (uint32_t addr, uint32_t* buf, uint32_t numWorlds){
    printf("invoke writeFlash\r\n");
    HAL_StatusTypeDef status = HAL_OK;

    __disable_irq();
    HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < numWorlds; i++) {
        status += HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, addr, buf[i]);
//        printf("HAL_FLASH_Program addr: %lX data: %lX\r\n", addr, buf[i]);
        addr += 4;
    }
    __enable_irq();
     HAL_FLASH_Lock();

    return status;
}
uint8_t eraseFlashSectors(uint32_t sector, uint32_t number){
    printf("invoke eraseFlashSectors\r\n");
//    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
    FLASH_EraseInitTypeDef FlashErase;                     // структура для функции стирания страницы
    uint32_t sectorError = 0;                              // переменная для записи информации об ошибках в процессе стирания
    __disable_irq();
    HAL_FLASH_Unlock();
    FlashErase.TypeErase = FLASH_TYPEERASE_SECTORS;
    FlashErase.NbSectors = number;
    FlashErase.Sector = sector;
    FlashErase.VoltageRange = VOLTAGE_RANGE_3;
    if (HAL_FLASHEx_Erase(&FlashErase, &sectorError) != HAL_OK) {  // вызов функции стирания
        __enable_irq();
        HAL_FLASH_Lock();
        return HAL_ERROR;
    }
    __enable_irq();
    return HAL_OK;
}
void startUpdateFirmware(FIL* fp, const TCHAR* path, uint32_t len){
    printf("\r\n************* Start update Firmware ************\r\n");

    f_open(fp, path, FA_READ );
    if (fp->obj.objsize > 0){
        if (eraseFlashSectors(FLASH_SECTOR_4, numSectorsErase) == HAL_OK){
            printf ("erased %d flash sectors from %d to %d \r\n", numSectorsErase, FLASH_SECTOR_4, FLASH_SECTOR_4 + numSectorsErase -1);
        //Вычисляем n (раз по BLOCK_SIZE) и m  (остаток байт)
            uint8_t n = len / (NUM_WORLDS * 4);

            uint32_t m = len % (NUM_WORLDS * 4);
            printf ("firmware - %d part (%d) + %d byte\r\n", n, NUM_WORLDS * 4, m);

            UINT bytesRead;
            uint32_t addresFlashWrite = FLASH_APP_START_ADDESS;
            for (uint8_t i = 0; i < n; ++i){
                f_read(fp, bufFW, NUM_WORLDS * 4, &bytesRead);
                printf ("read %d byte\r\n", bytesRead);
                printf ("flash address 0x%X \r\n", addresFlashWrite);
//Запись во FLASH
                if (writeFlash(addresFlashWrite, bufFW, NUM_WORLDS) == HAL_OK){
                    printf ("WRITE FLASH to address %X %d byte\r\n", addresFlashWrite, NUM_WORLDS * 4);
                }
                else {
                    printf ("ERROR WRITE FLASH to address %X %d byte\r\n", addresFlashWrite, NUM_WORLDS * 4);
                }

                addresFlashWrite += NUM_WORLDS * 4;
            }
//Остаток m байт
            f_read(fp, bufFW, m, &bytesRead);
            printf ("read %d byte\r\n", bytesRead);
            printf ("flash address 0x%X \r\n", addresFlashWrite);

            if (writeFlash(addresFlashWrite, bufFW, m / 4) == HAL_OK){
                printf ("WRITE FLASH to address %X %d byte\r\n", addresFlashWrite, m);
            }
            else {
                printf ("ERROR WRITE FLASH to address %X %d byte\r\n", addresFlashWrite, m);
            }

        }//if (eraseFlashSectors(FLASH_SECTOR_4, numSectorsErase) == HAL_OK)
        else {
            printf ("error erased flash sectors \r\n");
        }
    }//if (fp->obj.objsize > 0)
    f_close(fp);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_SPI3_Init(void);
static void MX_RNG_Init(void);
static void MX_RTC_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_CRC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART6_UART_Init();
  MX_SPI3_Init();
  MX_FATFS_Init();
  MX_RNG_Init();
  MX_RTC_Init();
  MX_USART2_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
    printf("************************************************\r\n");
    printf("version bootloader: %.2d_%.2d\r\n", main_FW, patch_FW);
    printf("\r\nstart address SHARED_MEMORY:\t %p\r\n", (uint32_t*)&_smem);
    printf("start address APP:\t\t %p\r\n", (uint32_t*)&_sapp);
    printf("end   address APP:\t\t %p\r\n", (uint32_t*)&_eapp);
    f_mount(&fs, "", 0);
    char nameFw[100];
    uint32_t sizeFw = 0;
    readDirSD(nameFw, &sizeFw);
    if (sizeFw > 0){
        printf ("fount firmware file: %s size: %d\r\n",nameFw, sizeFw);
        startUpdateFirmware(&file, nameFw, sizeFw);
    }

    if (0 == fw_check()){
        //Читаем данные по адресу APP_ADDR + 0x188 (таблица векторов)
        printFlash(FIRMWARE_SECTION_ADDR, 4 * 10); //Передается 10 слов в новой секции
        printf("num version: %d\r\n",flashReadWorld(FIRMWARE_SECTION_ADDR));
        printf("num patch: %d\r\n",flashReadWorld(FIRMWARE_SECTION_ADDR + 4));

        printf("\r\n******* Send control for main Firmware *********\r\n");
        firmware_run();
    }



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_MAY;
  sDate.Date = 0x9;
  sDate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI3_Init(void)
{

  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */

  /* USER CODE END SPI3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
printf("\r\n\r\n*************** Start bootloader ***************\r\n\n");
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 115200;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_EEPROM_Pin|HOLD_EEPROM_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WP_EEPROM_GPIO_Port, WP_EEPROM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, Green_Led_Pin|Orange_Led_Pin|Red_Led_Pin|Blue_Led_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);

  /*Configure GPIO pin : CS_EEPROM_Pin */
  GPIO_InitStruct.Pin = CS_EEPROM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(CS_EEPROM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : WP_EEPROM_Pin HOLD_EEPROM_Pin */
  GPIO_InitStruct.Pin = WP_EEPROM_Pin|HOLD_EEPROM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : OTG_FS_PowerSwitchOn_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OTG_FS_PowerSwitchOn_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BOOT1_Pin */
  GPIO_InitStruct.Pin = BOOT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BOOT1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Green_Led_Pin Orange_Led_Pin Red_Led_Pin Blue_Led_Pin */
  GPIO_InitStruct.Pin = Green_Led_Pin|Orange_Led_Pin|Red_Led_Pin|Blue_Led_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PD2 */
  GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

