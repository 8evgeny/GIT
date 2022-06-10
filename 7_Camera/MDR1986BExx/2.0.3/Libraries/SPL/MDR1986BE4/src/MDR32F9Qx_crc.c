/**
  ******************************************************************************
  * @file    MDR32F9Qx_crc.c
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    23/09/2021
  * @brief   This file contains all the CRC firmware functions.
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

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_crc.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @addtogroup CRC CRC
  * @{
  */

/** @defgroup CRC_Private_Functions CRC Private Functions
  * @{
  */

/**
  * @brief  Resets the CRC peripheral registers to their default reset values.
  * @param  None.
  * @retval None.
  */
void CRC_DeInit(void)
{
    MDR_CRC->CTRL = 0x0000;
    MDR_CRC->STAT = 0x0004; // Reset FIFOOVER bit
    MDR_CRC->VAL  = 0x0000;
    MDR_CRC->POL  = 0x8001;
}

/**
  * @brief  Initializes the CRC peripheral according to the specified
  *         parameters in the CRC_InitStruct.
  * @param  CRC_InitStruct: pointer to a @ref CRC_InitTypeDef structure
  *         that contains the configuration information for the specified CRC peripheral.
  * @retval None.
  */
void CRC_Init(CRC_InitTypeDef* CRC_InitStruct)
{
    /* Check the parameters */
    assert_param(IS_CRC_DATA_SIZE(CRC_InitStruct->CRC_DataSize));
    assert_param(IS_CRC_DATA_LOAD_SIZE(CRC_InitStruct->CRC_DataLoadSize));
    assert_param(IS_FUNCTIONAL_STATE(CRC_InitStruct->CRC_DMARequestState));
    assert_param(IS_CRC_ENDIAN_MODE(CRC_InitStruct->CRC_EndianMode));
    assert_param(IS_CRC_POLYNOM(CRC_InitStruct->CRC_Polynom));
    assert_param(IS_CRC_START_VALUE(CRC_InitStruct->CRC_StartValue));

    /* Set CRC CTRL register */
    MDR_CRC->CTRL = (uint32_t)CRC_InitStruct->CRC_DataSize
                  | CRC_InitStruct->CRC_DataLoadSize
                  | (CRC_InitStruct->CRC_DMARequestState << CRC_CTRL_DMAEN_Pos)
                  | CRC_InitStruct->CRC_EndianMode;

    /* Set initial polynom */
    MDR_CRC->POL = CRC_InitStruct->CRC_Polynom | 0x10001;

    /* Set initial value */
    MDR_CRC->VAL = CRC_InitStruct->CRC_StartValue;
}

/**
  * @brief  Fills each CRC_InitStruct member with its default value.
  * @param  CRC_InitStruct: pointer to a @ref CRC_InitTypeDef structure
  *         that is to be initialized.
  * @retval None.
  */
void CRC_StructInit(CRC_InitTypeDef* CRC_InitStruct)
{
    CRC_InitStruct->CRC_DataSize        = CRC_DATA_SIZE_8BIT;
    CRC_InitStruct->CRC_DataLoadSize    = CRC_DATA_LOAD_SIZE_8BIT;
    CRC_InitStruct->CRC_DMARequestState = DISABLE;
    CRC_InitStruct->CRC_EndianMode      = CRC_LITTLE_ENDIAN_MODE;
    CRC_InitStruct->CRC_Polynom         = 0;
    CRC_InitStruct->CRC_StartValue      = 0;
}

/**
  * @brief  Enables or disables the specified CRC peripheral.
  * @param  NewState - @ref FunctionalState - new state of the CRC peripheral.
  * @retval None.
  */
void CRC_Cmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState != DISABLE)
    {
        MDR_CRC->CTRL |= CRC_CTRL_CRCEN;
    }
    else
    {
        MDR_CRC->CTRL &= ~CRC_CTRL_CRCEN;
    }
}

/**
  * @brief  Set the input data size.
  * @param  data_size - @ref CRC_Data_Size_TypeDef - the size of input data.
  * @retval None.
  */
void CRC_SetDataSize(CRC_Data_Size_TypeDef data_size)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_CRC_DATA_SIZE(data_size));

    /* Set data size */
    tmpreg = MDR_CRC->CTRL & (~CRC_CTRL_DCSIZE_Msk);
    tmpreg |= (uint32_t)data_size;

    MDR_CRC->CTRL = tmpreg;
}

/**
  * @brief  Set the load data size.
  * @param  data_load_size - @ref CRC_Data_Load_Size_TypeDef - the size of loading data.
  * @retval None.
  */
void CRC_SetDataLoadSize(CRC_Data_Load_Size_TypeDef data_load_size)
{
    uint32_t tmpreg;

    /* Check the parameters */
    assert_param(IS_CRC_DATA_LOAD_SIZE(data_load_size));

    /* Set data load size */
    tmpreg = MDR_CRC->CTRL & (~CRC_CTRL_DLSIZE_Msk);
    tmpreg |= (uint32_t)data_load_size;

    MDR_CRC->CTRL = tmpreg;
}

/**
  * @brief  Enables or disables the CRC DMA request.
  * @param  NewState - @ref FunctionalState - new state of the CRC DMA request.
  * @retval None.
  */
void CRC_DMARequestCmd(FunctionalState NewState)
{
    /* Check the parameters */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if(NewState != DISABLE)
    {
        MDR_CRC->CTRL |= CRC_CTRL_DMAEN;
    }
    else
    {
        MDR_CRC->CTRL &= ~CRC_CTRL_DMAEN;
    }
}

/**
  * @brief  Set the endian mode.
  * @param  endian_mode - @ref CRC_Endian_Mode_TypeDef - new value of the CRC endian mode.
  * @retval None.
  */
void CRC_SetEndianMode(CRC_Endian_Mode_TypeDef endian_mode)
{
    /* Check the parameters */
    assert_param(IS_CRC_ENDIAN_MODE(endian_mode));

    if(endian_mode == CRC_BIG_ENDIAN_MODE)
    {
        MDR_CRC->CTRL |= CRC_CTRL_DATAINV;
    }
    else
    {
        MDR_CRC->CTRL &= ~CRC_CTRL_DATAINV;
    }
}

/**
  * @brief  Set the CRC polynom.
  * @param  polynom: new value of the CRC polynom.
  *         This parameter can be a number between 0x10001 and 0x1FFFF.
  * @retval None.
  */
void CRC_SetPolynom(uint16_t polynom)
{
    /* Check the parameters */
    assert_param(IS_CRC_POLYNOM(polynom));

    MDR_CRC->POL = polynom | 0x10001;
}

/**
  * @brief  The initial value for CRC calculation.
  * @param  start_value: the start CRC value.
  *         This parameter can be a number between 0x00000 and 0x1FFFF.
  * @retval None.
  */
void CRC_SetStartValue(uint16_t start_value)
{
    /* Check the parameters */
    assert_param(IS_CRC_START_VALUE(start_value));

    MDR_CRC->VAL = start_value;
}

/**
  * @brief  Checks whether the specified CRC Status flag is set or not.
  * @param  crc_flag - @ref CRC_STATUS_Flag_TypeDef - specifies the flag to check.
  * @retval @ref FlagStatus - Current Status flag state (SET or RESET).
  */
FlagStatus CRC_GetFlagStatus(CRC_STATUS_Flag_TypeDef crc_flag)
{
    FlagStatus flag = RESET;

    /* Check the parameters */
    assert_param(IS_CRC_STATUS_FLAG(crc_flag));

    if(MDR_CRC->STAT & (uint32_t)crc_flag)
    {
        flag = SET;
    }

    return (flag);
}

/**
  * @brief  Clear the FIFO overloaded flag.
  * @param  None.
  * @retval None.
  */
void CRC_ClearFIFOOverFlag(void)
{
    MDR_CRC->STAT |= CRC_STAT_FIFOOVER;
}

/**
  * @brief  Write new data to CRC FIFO.
  * @param  data: data value loaded into FIFO.
  * @retval None.
  */
void CRC_WriteData(uint32_t data)
{
    MDR_CRC->DATAI = data;
}

/**
  * @brief  Returns the CRC result.
  * @param  None.
  * @retval CRC result register value.
  */
uint16_t CRC_GetCRC(void)
{
    return (MDR_CRC->VAL & 0xFFFF);
}

/** @} */ /* End of group CRC_Private_Functions */

/** @} */ /* End of group CRC */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_crc.c */
