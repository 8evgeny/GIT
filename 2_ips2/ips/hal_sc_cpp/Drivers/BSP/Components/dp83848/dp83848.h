/**
  ******************************************************************************
  * @file    DP83848.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    08-March-2017
  * @brief   This file contains all the functions prototypes for the
  *          DP83848.c PHY driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DP83848_H
#define __DP83848_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @addtogroup BSP
  * @{
  */ 
#define DP83848_TRUE			0x1
#define DP83848_FALSE			0x0
#define DP83848_CAST2BOOL(x)	((x)?DP83848_TRUE:DP83848_FALSE)
/** @addtogroup Component
  * @{
  */
    
/** @defgroup DP83848
  * @{
  */    
/* Exported constants --------------------------------------------------------*/
/** @defgroup DP83848_Exported_Constants DP83848 Exported Constants
  * @{
  */ 
  
/** @defgroup DP83848_Registers_Mapping DP83848 Registers Mapping
  * @{
  */ 

#define DP83848_BCR      ((uint16_t)0x0000U)
#define DP83848_BSR      ((uint16_t)0x0001U)
#define DP83848_PHYI1R   ((uint16_t)0x0002U)
#define DP83848_PHYI2R   ((uint16_t)0x0003U)
#define DP83848_ANAR     ((uint16_t)0x0004U)
#define DP83848_ANLPAR   ((uint16_t)0x0005U)
#define DP83848_ANER     ((uint16_t)0x0006U)
#define DP83848_ANNPTR   ((uint16_t)0x0007U)
#define DP83848_ANNPRR   ((uint16_t)0x0008U)
#define DP83848_MMDACR   ((uint16_t)0x000DU)
#define DP83848_MMDAADR  ((uint16_t)0x000EU)
#define DP83848_ENCTR    ((uint16_t)0x0010U)
#define DP83848_MCSR     ((uint16_t)0x0011U)
#define DP83848_SMR      ((uint16_t)0x0012U)

#define DP83848_PHYCR	((uint16_t)0x0019U)
#define DP83848_PHYSTS	((uint16_t)0x0010U)
#define DP83848_SR			((uint16_t)0x1FU)

#define DP83848_TPDCR    ((uint16_t)0x0018U)
#define DP83848_TCSR     ((uint16_t)0x0019U)
#define DP83848_SECR     ((uint16_t)0x001AU)
#define DP83848_SCSIR    ((uint16_t)0x001BU)
#define DP83848_CLR      ((uint16_t)0x001CU)
#define DP83848_ISFR     ((uint16_t)0x001DU)
#define DP83848_IMR      ((uint16_t)0x001EU)
#define DP83848_PHYSCSR  ((uint16_t)0x001FU)
/**
  * @}
  */

/** @defgroup DP83848_BCR_Bit_Definition DP83848 BCR Bit Definition
  * @{
  */    
#define DP83848_BCR_SOFT_RESET         ((uint16_t)0x8000U)
#define DP83848_BCR_LOOPBACK           ((uint16_t)0x4000U)
#define DP83848_BCR_SPEED_SELECT       ((uint16_t)0x2000U)
#define DP83848_BCR_AUTONEGO_EN        ((uint16_t)0x1000U)
#define DP83848_BCR_POWER_DOWN         ((uint16_t)0x0800U)
#define DP83848_BCR_ISOLATE            ((uint16_t)0x0400U)
#define DP83848_BCR_RESTART_AUTONEGO   ((uint16_t)0x0200U)
#define DP83848_BCR_DUPLEX_MODE        ((uint16_t)0x0100U)
/**
  * @}
  */

/** @defgroup DP83848_BSR_Bit_Definition DP83848 BSR Bit Definition
  * @{
  */   
#define DP83848_BSR_100BASE_T4       ((uint16_t)0x8000U)
#define DP83848_BSR_100BASE_TX_FD    ((uint16_t)0x4000U)
#define DP83848_BSR_100BASE_TX_HD    ((uint16_t)0x2000U)
#define DP83848_BSR_10BASE_T_FD      ((uint16_t)0x1000U)
#define DP83848_BSR_10BASE_T_HD      ((uint16_t)0x0800U)
#define DP83848_BSR_100BASE_T2_FD    ((uint16_t)0x0400U)
#define DP83848_BSR_100BASE_T2_HD    ((uint16_t)0x0200U)
#define DP83848_BSR_EXTENDED_STATUS  ((uint16_t)0x0100U)
#define DP83848_BSR_AUTONEGO_CPLT    ((uint16_t)0x0020U)
#define DP83848_BSR_REMOTE_FAULT     ((uint16_t)0x0010U)
#define DP83848_BSR_AUTONEGO_ABILITY ((uint16_t)0x0008U)
#define DP83848_BSR_LINK_STATUS      ((uint16_t)0x0004U)
#define DP83848_BSR_JABBER_DETECT    ((uint16_t)0x0002U)
#define DP83848_BSR_EXTENDED_CAP     ((uint16_t)0x0001U)
/**
  * @}
  */

/** @defgroup DP83848_PHYI1R_Bit_Definition DP83848 PHYI1R Bit Definition
  * @{
  */
#define DP83848_PHYI1R_OUI_3_18           ((uint16_t)0xFFFFU)
/**
  * @}
  */

/** @defgroup DP83848_PHYI2R_Bit_Definition DP83848 PHYI2R Bit Definition
  * @{
  */
#define DP83848_PHYI2R_OUI_19_24          ((uint16_t)0xFC00U)
#define DP83848_PHYI2R_MODEL_NBR          ((uint16_t)0x03F0U)
#define DP83848_PHYI2R_REVISION_NBR       ((uint16_t)0x000FU)
/**
  * @}
  */

/** @defgroup DP83848_ANAR_Bit_Definition DP83848 ANAR Bit Definition
  * @{
  */
#define DP83848_ANAR_NEXT_PAGE               ((uint16_t)0x8000U)
#define DP83848_ANAR_REMOTE_FAULT            ((uint16_t)0x2000U)
#define DP83848_ANAR_PAUSE_OPERATION         ((uint16_t)0x0C00U)
#define DP83848_ANAR_PO_NOPAUSE              ((uint16_t)0x0000U)
#define DP83848_ANAR_PO_SYMMETRIC_PAUSE      ((uint16_t)0x0400U)
#define DP83848_ANAR_PO_ASYMMETRIC_PAUSE     ((uint16_t)0x0800U)
#define DP83848_ANAR_PO_ADVERTISE_SUPPORT    ((uint16_t)0x0C00U)
#define DP83848_ANAR_100BASE_TX_FD           ((uint16_t)0x0100U)
#define DP83848_ANAR_100BASE_TX              ((uint16_t)0x0080U)
#define DP83848_ANAR_10BASE_T_FD             ((uint16_t)0x0040U)
#define DP83848_ANAR_10BASE_T                ((uint16_t)0x0020U)
#define DP83848_ANAR_SELECTOR_FIELD          ((uint16_t)0x000FU)
/**
  * @}
  */

/** @defgroup DP83848_ANLPAR_Bit_Definition DP83848 ANLPAR Bit Definition
  * @{
  */
#define DP83848_ANLPAR_NEXT_PAGE            ((uint16_t)0x8000U)
#define DP83848_ANLPAR_REMOTE_FAULT         ((uint16_t)0x2000U)
#define DP83848_ANLPAR_PAUSE_OPERATION      ((uint16_t)0x0C00U)
#define DP83848_ANLPAR_PO_NOPAUSE           ((uint16_t)0x0000U)
#define DP83848_ANLPAR_PO_SYMMETRIC_PAUSE   ((uint16_t)0x0400U)
#define DP83848_ANLPAR_PO_ASYMMETRIC_PAUSE  ((uint16_t)0x0800U)
#define DP83848_ANLPAR_PO_ADVERTISE_SUPPORT ((uint16_t)0x0C00U)
#define DP83848_ANLPAR_100BASE_TX_FD        ((uint16_t)0x0100U)
#define DP83848_ANLPAR_100BASE_TX           ((uint16_t)0x0080U)
#define DP83848_ANLPAR_10BASE_T_FD          ((uint16_t)0x0040U)
#define DP83848_ANLPAR_10BASE_T             ((uint16_t)0x0020U)
#define DP83848_ANLPAR_SELECTOR_FIELD       ((uint16_t)0x000FU)
/**
  * @}
  */

/** @defgroup DP83848_ANER_Bit_Definition DP83848 ANER Bit Definition
  * @{
  */
#define DP83848_ANER_RX_NP_LOCATION_ABLE    ((uint16_t)0x0040U)
#define DP83848_ANER_RX_NP_STORAGE_LOCATION ((uint16_t)0x0020U)
#define DP83848_ANER_PARALLEL_DETECT_FAULT  ((uint16_t)0x0010U)
#define DP83848_ANER_LP_NP_ABLE             ((uint16_t)0x0008U)
#define DP83848_ANER_NP_ABLE                ((uint16_t)0x0004U)
#define DP83848_ANER_PAGE_RECEIVED          ((uint16_t)0x0002U)
#define DP83848_ANER_LP_AUTONEG_ABLE        ((uint16_t)0x0001U)
/**
  * @}
  */

/** @defgroup DP83848_ANNPTR_Bit_Definition DP83848 ANNPTR Bit Definition
  * @{
  */
#define DP83848_ANNPTR_NEXT_PAGE         ((uint16_t)0x8000U)
#define DP83848_ANNPTR_MESSAGE_PAGE      ((uint16_t)0x2000U)
#define DP83848_ANNPTR_ACK2              ((uint16_t)0x1000U)
#define DP83848_ANNPTR_TOGGLE            ((uint16_t)0x0800U)
#define DP83848_ANNPTR_MESSAGGE_CODE     ((uint16_t)0x07FFU)
/**
  * @}
  */

/** @defgroup DP83848_ANNPRR_Bit_Definition DP83848 ANNPRR Bit Definition
  * @{
  */
#define DP83848_ANNPTR_NEXT_PAGE         ((uint16_t)0x8000U)
#define DP83848_ANNPRR_ACK               ((uint16_t)0x4000U)
#define DP83848_ANNPRR_MESSAGE_PAGE      ((uint16_t)0x2000U)
#define DP83848_ANNPRR_ACK2              ((uint16_t)0x1000U)
#define DP83848_ANNPRR_TOGGLE            ((uint16_t)0x0800U)
#define DP83848_ANNPRR_MESSAGGE_CODE     ((uint16_t)0x07FFU)
/**
  * @}
  */

/** @defgroup DP83848_MMDACR_Bit_Definition DP83848 MMDACR Bit Definition
  * @{
  */
#define DP83848_MMDACR_MMD_FUNCTION       ((uint16_t)0xC000U)
#define DP83848_MMDACR_MMD_FUNCTION_ADDR  ((uint16_t)0x0000U)
#define DP83848_MMDACR_MMD_FUNCTION_DATA  ((uint16_t)0x4000U)
#define DP83848_MMDACR_MMD_DEV_ADDR       ((uint16_t)0x001FU)
/**
  * @}
  */

/** @defgroup DP83848_ENCTR_Bit_Definition DP83848 ENCTR Bit Definition
  * @{
  */
#define DP83848_ENCTR_TX_ENABLE             ((uint16_t)0x8000U)
#define DP83848_ENCTR_TX_TIMER              ((uint16_t)0x6000U)
#define DP83848_ENCTR_TX_TIMER_1S           ((uint16_t)0x0000U)
#define DP83848_ENCTR_TX_TIMER_768MS        ((uint16_t)0x2000U)
#define DP83848_ENCTR_TX_TIMER_512MS        ((uint16_t)0x4000U)
#define DP83848_ENCTR_TX_TIMER_265MS        ((uint16_t)0x6000U)
#define DP83848_ENCTR_RX_ENABLE             ((uint16_t)0x1000U)
#define DP83848_ENCTR_RX_MAX_INTERVAL       ((uint16_t)0x0C00U)
#define DP83848_ENCTR_RX_MAX_INTERVAL_64MS  ((uint16_t)0x0000U)
#define DP83848_ENCTR_RX_MAX_INTERVAL_256MS ((uint16_t)0x0400U)
#define DP83848_ENCTR_RX_MAX_INTERVAL_512MS ((uint16_t)0x0800U)
#define DP83848_ENCTR_RX_MAX_INTERVAL_1S    ((uint16_t)0x0C00U)
#define DP83848_ENCTR_EX_CROSS_OVER         ((uint16_t)0x0002U)
#define DP83848_ENCTR_EX_MANUAL_CROSS_OVER  ((uint16_t)0x0001U)
/**
  * @}
  */

/** @defgroup DP83848_MCSR_Bit_Definition DP83848 MCSR Bit Definition
  * @{
  */
#define DP83848_MCSR_EDPWRDOWN        ((uint16_t)0x2000U)
#define DP83848_MCSR_FARLOOPBACK      ((uint16_t)0x0200U)
#define DP83848_MCSR_ALTINT           ((uint16_t)0x0040U)
#define DP83848_MCSR_ENERGYON         ((uint16_t)0x0002U)
/**
  * @}
  */

/** @defgroup DP83848_SMR_Bit_Definition DP83848 SMR Bit Definition
  * @{
  */
#define DP83848_SMR_MODE       ((uint16_t)0x00E0U)
#define DP83848_SMR_PHY_ADDR   ((uint16_t)0x001FU)

#define DP83848_PHYCR_PHY_ADDR   ((uint16_t)0x001FU)
/**
  * @}
  */

/** @defgroup DP83848_TPDCR_Bit_Definition DP83848 TPDCR Bit Definition
  * @{
  */
#define DP83848_TPDCR_DELAY_IN                 ((uint16_t)0x8000U)
#define DP83848_TPDCR_LINE_BREAK_COUNTER       ((uint16_t)0x7000U)
#define DP83848_TPDCR_PATTERN_HIGH             ((uint16_t)0x0FC0U)
#define DP83848_TPDCR_PATTERN_LOW              ((uint16_t)0x003FU)
/**
  * @}
  */

/** @defgroup DP83848_TCSR_Bit_Definition DP83848 TCSR Bit Definition
  * @{
  */
#define DP83848_TCSR_TDR_ENABLE           ((uint16_t)0x8000U)
#define DP83848_TCSR_TDR_AD_FILTER_ENABLE ((uint16_t)0x4000U)
#define DP83848_TCSR_TDR_CH_CABLE_TYPE    ((uint16_t)0x0600U)
#define DP83848_TCSR_TDR_CH_CABLE_DEFAULT ((uint16_t)0x0000U)
#define DP83848_TCSR_TDR_CH_CABLE_SHORTED ((uint16_t)0x0200U)
#define DP83848_TCSR_TDR_CH_CABLE_OPEN    ((uint16_t)0x0400U)
#define DP83848_TCSR_TDR_CH_CABLE_MATCH   ((uint16_t)0x0600U)
#define DP83848_TCSR_TDR_CH_STATUS        ((uint16_t)0x0100U)
#define DP83848_TCSR_TDR_CH_LENGTH        ((uint16_t)0x00FFU)
/**
  * @}
  */

/** @defgroup DP83848_SCSIR_Bit_Definition DP83848 SCSIR Bit Definition
  * @{
  */
#define DP83848_SCSIR_AUTO_MDIX_ENABLE    ((uint16_t)0x8000U)
#define DP83848_SCSIR_CHANNEL_SELECT      ((uint16_t)0x2000U)
#define DP83848_SCSIR_SQE_DISABLE         ((uint16_t)0x0800U)
#define DP83848_SCSIR_XPOLALITY           ((uint16_t)0x0010U)
/**
  * @}
  */

/** @defgroup DP83848_CLR_Bit_Definition DP83848 CLR Bit Definition
  * @{
  */
#define DP83848_CLR_CABLE_LENGTH       ((uint16_t)0xF000U)
/**
  * @}
  */

/** @defgroup DP83848_IMR_ISFR_Bit_Definition DP83848 IMR ISFR Bit Definition
  * @{
  */
#define DP83848_INT_8       ((uint16_t)0x0100U)
#define DP83848_INT_7       ((uint16_t)0x0080U)
#define DP83848_INT_6       ((uint16_t)0x0040U)
#define DP83848_INT_5       ((uint16_t)0x0020U)
#define DP83848_INT_4       ((uint16_t)0x0010U)
#define DP83848_INT_3       ((uint16_t)0x0008U)
#define DP83848_INT_2       ((uint16_t)0x0004U)
#define DP83848_INT_1       ((uint16_t)0x0002U)
/**
  * @}
  */

/** @defgroup DP83848_PHYSCSR_Bit_Definition DP83848 PHYSCSR Bit Definition
  * @{
  */
#define DP83848_PHYSCSR_AUTONEGO_DONE   ((uint16_t)0x1000U)
#define DP83848_PHYSCSR_HCDSPEEDMASK    ((uint16_t)0x001CU)
#define DP83848_PHYSCSR_10BT_HD         ((uint16_t)0x0004U)
#define DP83848_PHYSCSR_10BT_FD         ((uint16_t)0x0014U)
#define DP83848_PHYSCSR_100BTX_HD       ((uint16_t)0x0008U)
#define DP83848_PHYSCSR_100BTX_FD       ((uint16_t)0x0018U)


#define DP83848_PHYSTS_AUTONEGO_DONE	((uint16_t)0x0004U)
#define DP83848_PHYSTS_SPEEDMASK		((uint16_t)0x0006U)
#define DP83848_PHYSTS_10BT_HD			((uint16_t)0x0002U)
#define DP83848_PHYSTS_10BT_FD			((uint16_t)0x0006U)
#define DP83848_PHYSTS_100BTX_HD		((uint16_t)0x0000U)
#define DP83848_PHYSTS_100BTX_FD		((uint16_t)0x0004U)
#define DP83848_PHYSTS_LINKUP			((uint16_t)0x0001U)
/**
  * @}
  */
    
/** @defgroup DP83848_Status DP83848 Status
  * @{
  */    

#define  DP83848_STATUS_READ_ERROR            ((int32_t)-5)
#define  DP83848_STATUS_WRITE_ERROR           ((int32_t)-4)
#define  DP83848_STATUS_ADDRESS_ERROR         ((int32_t)-3)
#define  DP83848_STATUS_RESET_TIMEOUT         ((int32_t)-2)
#define  DP83848_STATUS_ERROR                 ((int32_t)-1)
#define  DP83848_STATUS_OK                    ((int32_t) 0)
#define  DP83848_STATUS_LINK_DOWN             ((int32_t) 1)
#define  DP83848_STATUS_100MBITS_FULLDUPLEX   ((int32_t) 2)
#define  DP83848_STATUS_100MBITS_HALFDUPLEX   ((int32_t) 3)
#define  DP83848_STATUS_10MBITS_FULLDUPLEX    ((int32_t) 4)
#define  DP83848_STATUS_10MBITS_HALFDUPLEX    ((int32_t) 5)
#define  DP83848_STATUS_AUTONEGO_NOTDONE      ((int32_t) 6)
/**
  * @}
  */

/** @defgroup DP83848_IT_Flags DP83848 IT Flags
  * @{
  */     
#define  DP83848_WOL_IT                        DP83848_INT_8
#define  DP83848_ENERGYON_IT                   DP83848_INT_7
#define  DP83848_AUTONEGO_COMPLETE_IT          DP83848_INT_6
#define  DP83848_REMOTE_FAULT_IT               DP83848_INT_5
#define  DP83848_LINK_DOWN_IT                  DP83848_INT_4
#define  DP83848_AUTONEGO_LP_ACK_IT            DP83848_INT_3
#define  DP83848_PARALLEL_DETECTION_FAULT_IT   DP83848_INT_2
#define  DP83848_AUTONEGO_PAGE_RECEIVED_IT     DP83848_INT_1
/**
  * @}
  */

/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/ 
/** @defgroup DP83848_Exported_Types DP83848 Exported Types
  * @{
  */
typedef int32_t  (*DP83848_Init_Func) (void);
typedef int32_t  (*DP83848_DeInit_Func) (void);
typedef int32_t  (*DP83848_ReadReg_Func)   (uint32_t, uint32_t, uint32_t *);
typedef int32_t  (*DP83848_WriteReg_Func)  (uint32_t, uint32_t, uint32_t);
typedef int32_t  (*DP83848_GetTick_Func)  (void);

typedef struct 
{                   
  DP83848_Init_Func      Init;
  DP83848_DeInit_Func    DeInit;
  DP83848_WriteReg_Func  WriteReg;
  DP83848_ReadReg_Func   ReadReg;
  DP83848_GetTick_Func   GetTick;
} DP83848_IOCtx_t;

  
typedef struct 
{
  uint32_t            DevAddr;
  uint32_t            Is_Initialized;
  DP83848_IOCtx_t     IO;
  void               *pData;
}DP83848_Object_t;
/**
  * @}
  */ 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @defgroup DP83848_Exported_Functions DP83848 Exported Functions
  * @{
  */
int32_t DP83848_RegisterBusIO(DP83848_Object_t *pObj, DP83848_IOCtx_t *ioctx);
int32_t DP83848_Init(DP83848_Object_t *pObj);
int32_t DP83848_DeInit(DP83848_Object_t *pObj);
int32_t DP83848_DisablePowerDownMode(DP83848_Object_t *pObj);
int32_t DP83848_EnablePowerDownMode(DP83848_Object_t *pObj);
int32_t DP83848_StartAutoNego(DP83848_Object_t *pObj);
int32_t DP83848_GetLinkState(DP83848_Object_t *pObj);
int32_t DP83848_SetLinkState(DP83848_Object_t *pObj, uint32_t LinkState);
int32_t DP83848_EnableLoopbackMode(DP83848_Object_t *pObj);
int32_t DP83848_DisableLoopbackMode(DP83848_Object_t *pObj);
int32_t DP83848_EnableIT(DP83848_Object_t *pObj, uint32_t Interrupt);
int32_t DP83848_DisableIT(DP83848_Object_t *pObj, uint32_t Interrupt);
int32_t DP83848_ClearIT(DP83848_Object_t *pObj, uint32_t Interrupt);
int32_t DP83848_GetITStatus(DP83848_Object_t *pObj, uint32_t Interrupt);
int32_t DP83848_DisableAutoNego(DP83848_Object_t *pObj);

int32_t DP83848_Isolate(DP83848_Object_t *pObj);
int32_t DP83848_DeIsolate(DP83848_Object_t *pObj);

int32_t readPHYDevice(DP83848_Object_t *pObj);

int32_t DP83848_GetFlag(DP83848_Object_t *pObj,uint32_t reg,uint8_t pos);
int32_t DP83848_SetFlag(DP83848_Object_t *pObj,uint32_t reg,uint8_t pos);
int32_t DP83848_SetFlagByMask(DP83848_Object_t *pObj,uint32_t reg,uint16_t mask);
int32_t DP83848_ResetFlag(DP83848_Object_t *pObj,uint32_t reg,uint8_t pos);
int32_t DP83848_ResetFlagByMask(DP83848_Object_t *pObj,uint32_t reg,uint16_t mask);
int32_t DP83848_Wait4Flag(DP83848_Object_t *pObj,uint32_t reg,uint8_t pos,int32_t state4wait);

/**
  * @}
  */ 

#ifdef __cplusplus
}
#endif
#endif /* __DP83848_H */


/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */       
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
