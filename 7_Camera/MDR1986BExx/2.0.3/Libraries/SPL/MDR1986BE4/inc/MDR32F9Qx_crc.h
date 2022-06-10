/**
  ******************************************************************************
  * @file    MDR32F9Qx_crc.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains all the functions prototypes for the CRC
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
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef MDR32F9QX_CRC_H
#define MDR32F9QX_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @addtogroup CRC CRC
  * @{
  */

/** @defgroup CRC_Exported_Types CRC Exported Types
  * @{
  */

/**
  * @brief CRC Data Size
  */
typedef enum
{
    CRC_DATA_SIZE_8BIT  = (uint32_t)(0x00),
    CRC_DATA_SIZE_16BIT = (uint32_t)(0x20),
    CRC_DATA_SIZE_32BIT = (uint32_t)(0x40)
} CRC_Data_Size_TypeDef;

#define IS_CRC_DATA_SIZE(DS)    ((DS == CRC_DATA_SIZE_8BIT)  ||\
                                 (DS == CRC_DATA_SIZE_16BIT) ||\
                                 (DS == CRC_DATA_SIZE_32BIT))

/**
  * @brief CRC Data Load Size
  */
typedef enum
{
    CRC_DATA_LOAD_SIZE_8BIT  = (uint32_t)(0x00),
    CRC_DATA_LOAD_SIZE_16BIT = (uint32_t)(0x08),
    CRC_DATA_LOAD_SIZE_32BIT = (uint32_t)(0x10)
} CRC_Data_Load_Size_TypeDef;

#define IS_CRC_DATA_LOAD_SIZE(DLS)  ((DLS == CRC_DATA_LOAD_SIZE_8BIT)  ||\
                                     (DLS == CRC_DATA_LOAD_SIZE_16BIT) ||\
                                     (DLS == CRC_DATA_LOAD_SIZE_32BIT))

/**
  * @brief CRC Endian Mode
  */
typedef enum
{
    CRC_BIG_ENDIAN_MODE    = (uint32_t)(0x02),
    CRC_LITTLE_ENDIAN_MODE = (uint32_t)(0x00)
} CRC_Endian_Mode_TypeDef;

#define IS_CRC_ENDIAN_MODE(EM)  ((EM == CRC_BIG_ENDIAN_MODE) ||\
                                 (EM == CRC_LITTLE_ENDIAN_MODE))

/**
  * @brief CRC Status flags
  */
typedef enum
{
    CRC_FIFO_OVER  = (uint32_t)(0x08),
    CRC_FIFO_EMPTY = (uint32_t)(0x04),
    CRC_FIFO_FULL  = (uint32_t)(0x02),
    CRC_CONV_COMP  = (uint32_t)(0x01)
} CRC_STATUS_Flag_TypeDef;

#define IS_CRC_STATUS_FLAG(SF)  ((SF == CRC_FIFO_OVER)  ||\
                                 (SF == CRC_FIFO_EMPTY) ||\
                                 (SF == CRC_FIFO_FULL)  ||\
                                 (SF == CRC_CONV_COMP))

/**
  * @brief CRC Init struct defintion
  */
typedef struct
{
    CRC_Data_Size_TypeDef      CRC_DataSize;        /*!< Size of the data in the calculation of CRC.
                                                         This parameter can be a value of @ref CRC_Data_Size_TypeDef. */
    CRC_Data_Load_Size_TypeDef CRC_DataLoadSize;    /*!< Size of loding data in the register DATAI.
                                                         This parameter can be a value of @ref CRC_Data_Load_Size_TypeDef. */
    FunctionalState            CRC_DMARequestState; /*!< Status for DMA request generation.
                                                         This parameter can be a value of @ref FunctionalState. */
    CRC_Endian_Mode_TypeDef    CRC_EndianMode;      /*!< Order of computation of CRC.
                                                         This parameter can be a value of @ref CRC_Endian_Mode_TypeDef. */
    uint32_t                   CRC_Polynom;         /*!< Polynomial for CRC calculation.
                                                         This parameter can be a number between 0x10001 and 0x1FFFF. */
    uint32_t                   CRC_StartValue;      /*!< Start value of the CRC calculation.
                                                         This parameter can be a number between 0x00000 and 0x1FFFF. */
} CRC_InitTypeDef;

/** @} */ /* End of group CRC_Exported_Types */


/** @defgroup CRC_Exported_Constants CRC Exported Constants
  * @{
  */

#define IS_CRC_START_VALUE(SV)  ((SV & (~CRC_VAL_CRCOUT_Msk)) == 0)
#define IS_CRC_POLYNOM(POL)     ((POL & (~CRC_POL_CRC_POL_Msk)) == 0)

/** @} */ /* End of group CRC_Exported_Constants */


/** @defgroup CRC_Exported_Functions CRC Exported Functions
  * @{
  */

void CRC_DeInit(void);
void CRC_Init(CRC_InitTypeDef* CRC_InitStruct);
void CRC_StructInit(CRC_InitTypeDef* CRC_InitStruct);
void CRC_Cmd(FunctionalState NewState);
void CRC_SetDataSize(CRC_Data_Size_TypeDef data_size);
void CRC_SetDataLoadSize(CRC_Data_Load_Size_TypeDef data_load_size);
void CRC_DMARequestCmd(FunctionalState NewState);
void CRC_SetEndianMode(CRC_Endian_Mode_TypeDef endian_mode);
void CRC_SetPolynom(uint16_t polynom);
void CRC_SetStartValue(uint16_t start_value);
FlagStatus CRC_GetFlagStatus(CRC_STATUS_Flag_TypeDef crc_flag);
void CRC_ClearFIFOOverFlag(void);
void CRC_WriteData(uint32_t data);
uint16_t CRC_GetCRC(void);

/** @} */ /* End of group CRC_Exported_Functions */

/** @} */ /* End of group CRC */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* MDR32F9QX_CRC_H */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_crc.h */
