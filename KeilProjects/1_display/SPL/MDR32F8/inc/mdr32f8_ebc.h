/**
  ******************************************************************************
  * @file    mdr32f8_ebc.h
  * @author  Milandr Application Team
  * @version V1.4.0
  * @date    20/07/2015
  * @brief   This file contains all the functions prototypes for the EBC
  *          firmware library.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  ******************************************************************************
  * FILE mdr32f8_ebc.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32F8_EBC_H
#define __MDR32F8_EBC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"	
#include "mdr32f8_config.h"
#include "MDR1986VE8T.h"	

/** @addtogroup __MDR32F8_StdPeriph_Driver MDR32F8 Standard Peripherial Driver
  * @{
  */

/** @addtogroup  EBC EBC
  * @{
  */

/** @defgroup EBC_Exported_Types EBC Exported Types
  * @{
  */

/**
  * @brief  EBC Init structure definition
  */

typedef struct {

	FunctionalState RGN_ECCEN;

	uint32_t RGN_ECCMODE;

	uint32_t RGN_RDYWAIT;

	uint32_t RGN_MODE;

	uint32_t RGN_ROM;

	uint32_t RGN_CPOL;

	uint32_t RGN_WS_ACTIVE;

	uint32_t RGN_WS_SETUP;

	uint32_t RGN_WS_HOLD;

	uint32_t RGN_USESEPAR;

	FunctionalState RGN_CACHEBLE;

	uint32_t RGN_READ32;

	FunctionalState RGN_ENOCLK;

	uint32_t RGN_DIVOCLK;
	
} EBC_RGN_InitTypeDef;

typedef enum { DEEC_CNT=0, SEEC_CNT} Cnt_type;


/** @} */ /* End of group EBC_Exported_Types */


/** @defgroup EBC_Exported_Macros EBC Exported Macros
  * @{
  */

#define 	RGN0      		0
#define 	RGN1			1
#define 	RGN2			2
#define 	RGN3			3
#define 	RGN4			4
#define 	RGN5			5
#define 	RGN6			6
#define 	RGN7			7

#define EBC_WS_ACTIVE_0  ((uint32_t) (0x0 << EBC_WS_ACTIVE_pos)



#define EBC_RGNx_EN		        ((uint32_t)0x00000001)

#define EBC_ECCMODE_PAR 		(uint32_t)(0x00)
#define EBC_ECCMODE_CON 		(uint32_t)(0x01)

#define EBC_RDYWAIT_NOWAIT 		(uint32_t)(0x00)
#define EBC_RDYWAIT_WAIT 		(uint32_t)(0x01)

#define EBC_MODE_32X 			(uint32_t)(0x00)
#define EBC_MODE_16X 			(uint32_t)(0x01)
#define EBC_MODE_8X 			(uint32_t)(0x02)
#define EBC_MODE_64X 			(uint32_t)(0x03)

#define EBC_ROM_RAM 			(uint32_t)(0x00)
#define EBC_ROM_ROM 			(uint32_t)(0x01)

#define EBC_CPOL_UP 			(uint32_t)(0x00)
#define EBC_CPOL_DOWN 			(uint32_t)(0x01)

#define EBC_USESEPAR_USER 		(uint32_t)(0x00)
#define EBC_USESEPAR_FORSOC 	(uint32_t)(0x01)

#define EBC_READ32_8_16 		(uint32_t)(0x00)
#define EBC_READ32_32 			(uint32_t)(0x01)

#define EBC_IT_SEEC					(uint32_t) (0x4)
#define EBC_IT_DEEC					(uint32_t) (0x8)

#define EBC_FIX_SEEC				(uint32_t) (0x10)
#define EBC_FIC_DEEC				(uint32_t) (0x20)

#define EBC_CNT_DEEC_MSK		(uint32_t) (0xFF00)
#define EBC_CNT_SEEC_MSK		(uint32_t) (0xFFFF0000)

#define EBC_CNT_DEEC_Pos			8		
#define EBC_CNT_SEEC_Pos			16

#define IS_EBC_RGNx_WS_SETUP(SRC)            ((SRC)<16)
#define IS_EBC_RGNx_WS_ACTIVE(SRC)           ((SRC)<256)
#define IS_EBC_RGNx_WS_HOLD(SRC)             ((SRC)<16)

#define IS_EBC_RGNx(NUMBER)				     ((NUMBER == RGN0) ||\
                                              (NUMBER == RGN1) ||\
                                              (NUMBER == RGN2) ||\
                                              (NUMBER == RGN3) ||\
                                              (NUMBER == RGN4) ||\
                                              (NUMBER == RGN5) ||\
                                              (NUMBER == RGN6) ||\
                                              (NUMBER == RGN7))
											  
#if defined ( __ICCARM__ )
   #define __RAMFUNC      __ramfunc
#elif defined (__CMCARM__)
  #define __RAMFUNC       __ramfunc
#elif defined ( __CC_ARM )
  #define __RAMFUNC
#elif defined ( __GNUC__)
 #define __RAMFUNC
#endif

/** @} */ /* End of group EBC_Exported_Macros */

/** @defgroup EBC_Exported_Functions EBC Exported Functions
  * @{
  */
	
void EBC_RGNx_Init(uint32_t RGNx, const EBC_RGN_InitTypeDef* EBC_RGN_InitStruct);	
void EBC_RGNx_StructInit(EBC_RGN_InitTypeDef* EBC_RGN_InitStruct);
void EBC_RGNx_Cmd(uint32_t RGNx, FunctionalState NewState);
void EBC_RGNx_Addr_serial_ECC (uint32_t RGNx, uint32_t addr);
void EBC_RGNx_ECCS_Config ( uint32_t RGNx, uint32_t EBC_IT_FIX, FunctionalState NewState);
uint32_t EBC_RGNx_GetErrorCount (uint32_t RGNx, Cnt_type CNT);
__RAMFUNC void EBC_RGNx_WSConfig(uint32_t RGNx, uint32_t RGN_WS_SETUP, uint32_t RGN_WS_ACTIVE, uint32_t RGN_WS_HOLD) __attribute__((section("EXECUTABLE_MEMORY_SECTION")));

/** @} */ /* End of group EBC_Exported_Functions */

/** @} */ /* End of group EBC */

/** @} */ /* End of group __MDR32F8_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32F8_EBC_H */

/******************* (C) COPYRIGHT 2021 Milandr *********************************
*
* END OF FILE mdr32f8_ebc.h */

