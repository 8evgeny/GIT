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
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include "socket.h"
#include "loopback.h"
#include "my_function.h"
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
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint8_t gDATABUF[DATA_BUF_SIZE];
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
                            .ip = {192, 168, 10, 1},		
                            .sn = {255,255,255,0},			
                            .gw = {192, 168, 10, 2},		
                            .dns = {0,0,0,0},
                            .dhcp = NETINFO_STATIC };

uint8_t tmp;//int32_t ret = 0;
// задание размеров буферов W5500 для сокетов по 2 Кбайта 														
uint8_t memsize[2][8] = { {2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};													
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */
														
/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void UART_Printf(const char* fmt, ...) {
    char buff[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buff, sizeof(buff), fmt, args);
    HAL_UART_Transmit(&huart2, (uint8_t*)buff, strlen(buff),
                      HAL_MAX_DELAY);
    va_end(args);
}

/* Chip selection call back */
void Chip_selection_call_back(void)
{
#if   _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
    reg_wizchip_cs_cbfunc(wizchip_select, wizchip_select);  // CS must be tried with LOW.
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif
    /* SPI Read & Write callback function */
    reg_wizchip_spi_cbfunc(wizchip_read, wizchip_write);

    /* wizchip initialize*/
}


void wizchip_initialize(void)
{
    if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1)
    {
       UART_Printf("WIZCHIP Initialized fail.\r\n");
       while(1);
    }
		else{UART_Printf("WIZCHIP Initialized OK.\r\n");}

    /* PHY link status check */
    do
    {
       if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
          UART_Printf("Unknown PHY Link stauts.\r\n");
    }while(tmp == PHY_LINK_OFF);
}


void network_init(void)
{
   uint8_t tmpstr[6];
	ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);

	// Display Network Information
	ctlwizchip(CW_GET_ID,(void*)tmpstr);
	UART_Printf("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
	UART_Printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
		  gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
	UART_Printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
	UART_Printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
	UART_Printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
	UART_Printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
	UART_Printf("======================\r\n");
	
}
/* USER CODE END 0 */

