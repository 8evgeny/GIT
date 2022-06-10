/**
  ******************************************************************************
  * @file    MDR32F9Qx_dac.h
  * @author  Milandr Application Team
  * @version V2.0.2
  * @date    22/09/2021
  * @brief   This file contains all the functions prototypes for the DAC
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
#ifndef __MDR32F9QX_DAC_H
#define __MDR32F9QX_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "MDR32F9Qx_config.h"

/** @addtogroup __MDR1986BE4_StdPeriph_Driver MDR1986BE4 Standard Peripherial Driver
  * @{
  */

/** @addtogroup DAC
  * @{
  */

/** @defgroup DAC_Exported_Types DAC Exported Types
  * @{
  */

/**
 * @brief DAC1 Reference Voltage source
 */
typedef enum
{
    DAC1_AVCC = (((uint32_t)0x0) << DAC_CFG_M_REF0_Pos), /*!< Selects AVcc as Reference Voltage. */
    DAC1_REF  = (((uint32_t)0x1) << DAC_CFG_M_REF0_Pos)  /*!< Selects DAC1_REF input as Reference Voltage. */
} DAC1_Ref_Src_TypeDef;

#define IS_DAC1_REF_CONFIG(CONFIG) (((CONFIG) == DAC1_AVCC) || \
                                    ((CONFIG) == DAC1_REF ))

/** @} */ /* End of group DAC_Exported_Types */


/** @defgroup DAC_Exported_Constants DAC Exported Constants
  * @{
  */

/** @defgroup DAC_Data DAC Data
  * @{
  */

#define DAC_LO_DATA_MSK                       ((uint32_t)0x0FFF)

#define IS_DAC_DATA(DATA) (((DATA) & ~(DAC_LO_DATA_MSK)) == 0 )

/** @} */ /* End of group DAC_Data */

/** @} */ /* End of group DAC_Exported_Constants */


/** @defgroup DAC_Exported_Functions DAC Exported Functions
  * @{
  */

void DAC_DeInit(void);
void DAC1_Init(DAC1_Ref_Src_TypeDef DAC1_Ref);
void DAC1_Cmd(FunctionalState NewState);
void DAC1_SetData(uint32_t Data);
uint32_t DAC1_GetData(void);

/** @} */ /* End of group DAC_Exported_Functions */

/** @} */ /* End of group DAC */

/** @} */ /* End of group __MDR1986BE4_StdPeriph_Driver */

#ifdef __cplusplus
} // extern "C" block end
#endif

#endif /* __MDR32F9QX_DAC_H */

/*********************** (C) COPYRIGHT 2021 Milandr ****************************
*
* END OF FILE MDR32F9Qx_dac.h */
