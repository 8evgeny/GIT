/**
  ******************************************************************************
  * @file    dp83848.h
  * @author  dmib@@ETS
  * @brief   This file contains DP83848 PHY controller driver module header
  ******************************************************************************
  */
#ifndef __DP83848_H
#define __DP83848_H

#ifdef __cplusplus
 extern "C" {
#endif   
   
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** @defgroup dp83848 Ethernet PHY controller driver module
  * @brief DP83848 PHY controller driver implementation
  * @{
  */


/* Exported constants --------------------------------------------------------*/

/// PHY controller MII address
#define DP83848_DEV_ADDR       0x01

/** @defgroup dp83848_reg DP83848 register defines
  * @brief DP83848 register defines
  * @{
  */
#define DP83848_BMCR	((uint16_t)0x0000U)
#define DP83848_BMSR	((uint16_t)0x0001U)
#define DP83848_PHYIDR1	((uint16_t)0x0002U)
#define DP83848_PHYIDR2	((uint16_t)0x0003U)
#define DP83848_ANAR	((uint16_t)0x0004U)
#define DP83848_ANLPAR	((uint16_t)0x0005U)
#define DP83848_ANER	((uint16_t)0x0006U)
#define DP83848_ANNPTR	((uint16_t)0x0007U)

#define DP83848_PHYSTS	((uint16_t)0x0010U)
#define DP83848_MICR	((uint16_t)0x0011U)
#define DP83848_MISR	((uint16_t)0x0012U)
#define DP83848_FCSCR	((uint16_t)0x0014U)
#define DP83848_RECR	((uint16_t)0x0015U)
#define DP83848_PCSR	((uint16_t)0x0016U)
#define DP83848_RBR		((uint16_t)0x0017U)
#define DP83848_LEDCR	((uint16_t)0x0018U)
#define DP83848_PHYCR	((uint16_t)0x0019U)
#define DP83848_10BTSCR	((uint16_t)0x001AU)
#define DP83848_CDCTRL1	((uint16_t)0x001BU)
#define DP83848_EDCR	((uint16_t)0x001DU)

// DP83848 BMCR Bit Definition
#define DP83848_BMCR_SOFT_RESET         ((uint16_t)0x8000U)
#define DP83848_BMCR_LOOPBACK           ((uint16_t)0x4000U)
#define DP83848_BMCR_SPEED_SELECT       ((uint16_t)0x2000U)
#define DP83848_BMCR_AUTONEGO_EN        ((uint16_t)0x1000U)
#define DP83848_BMCR_POWER_DOWN         ((uint16_t)0x0800U)
#define DP83848_BMCR_ISOLATE            ((uint16_t)0x0400U)
#define DP83848_BMCR_RESTART_AUTONEGO   ((uint16_t)0x0200U)
#define DP83848_BMCR_DUPLEX_MODE        ((uint16_t)0x0100U)
#define DP83848_BMCR_COLLISION_TEST     ((uint16_t)0x0080U)

// DP83848 BMSR Bit Definition
#define DP83848_BMSR_100BASE_T4       ((uint16_t)0x8000U)
#define DP83848_BMSR_100BASE_TX_FD    ((uint16_t)0x4000U)
#define DP83848_BMSR_100BASE_TX_HD    ((uint16_t)0x2000U)
#define DP83848_BMSR_10BASE_T_FD      ((uint16_t)0x1000U)
#define DP83848_BMSR_10BASE_T_HD      ((uint16_t)0x0800U)
#define DP83848_BMSR_PREAMBLE_SUPPR   ((uint16_t)0x0040U)
#define DP83848_BMSR_AUTONEGO_CPLT    ((uint16_t)0x0020U)
#define DP83848_BMSR_REMOTE_FAULT     ((uint16_t)0x0010U)
#define DP83848_BMSR_AUTONEGO_ABILITY ((uint16_t)0x0008U)
#define DP83848_BMSR_LINK_STATUS      ((uint16_t)0x0004U)
#define DP83848_BMSR_JABBER_DETECT    ((uint16_t)0x0002U)
#define DP83848_BMSR_EXTENDED_CAP     ((uint16_t)0x0001U)

// DP83848 PHYIDR Bit Definition
#define DP83848_PHYIDR1_OUI_3_18      ((uint16_t)0xFFFFU)
#define DP83848_PHYIDR2_OUI_19_24    ((uint16_t)0xFC00U)
#define DP83848_PHYIDR2_MODEL_NBR    ((uint16_t)0x03F0U)
#define DP83848_PHYIDR2_REVISION_NBR ((uint16_t)0x000FU)

// DP83848 ANAR Bit Definition
#define DP83848_ANAR_NEXT_PAGE               ((uint16_t)0x8000U)
#define DP83848_ANAR_REMOTE_FAULT            ((uint16_t)0x2000U)
#define DP83848_ANAR_ASYMMETRIC_PAUSE     	 ((uint16_t)0x0800U)
#define DP83848_ANAR_PAUSE_SUPPORT           ((uint16_t)0x0400U)
#define DP83848_ANAR_100BASE_T4              ((uint16_t)0x0200U)
#define DP83848_ANAR_100BASE_TX_FD           ((uint16_t)0x0100U)
#define DP83848_ANAR_100BASE_TX              ((uint16_t)0x0080U)
#define DP83848_ANAR_10BASE_T_FD             ((uint16_t)0x0040U)
#define DP83848_ANAR_10BASE_T                ((uint16_t)0x0020U)
#define DP83848_ANAR_SELECTOR_FIELD          ((uint16_t)0x001FU)

// DP83848 ANLPAR Bit Definition
#define DP83848_ANLPAR_NEXT_PAGE            ((uint16_t)0x8000U)
#define DP83848_ANLPAR_ACK                  ((uint16_t)0x4000U)
#define DP83848_ANLPAR_REMOTE_FAULT         ((uint16_t)0x2000U)
#define DP83848_ANLPAR_ASYMMETRIC_PAUSE     ((uint16_t)0x0800U)
#define DP83848_ANLPAR_PAUSE_SUPPORT        ((uint16_t)0x0400U)
#define DP83848_ANLPAR_100BASE_T4           ((uint16_t)0x0200U)
#define DP83848_ANLPAR_100BASE_TX_FD        ((uint16_t)0x0100U)
#define DP83848_ANLPAR_100BASE_TX           ((uint16_t)0x0080U)
#define DP83848_ANLPAR_10BASE_T_FD          ((uint16_t)0x0040U)
#define DP83848_ANLPAR_10BASE_T             ((uint16_t)0x0020U)
#define DP83848_ANLPAR_SELECTOR_FIELD       ((uint16_t)0x001FU)

//DP83848 ANER Bit Definition
#define DP83848_ANER_PARALLEL_DETECT_FAULT  ((uint16_t)0x0010U)
#define DP83848_ANER_LP_NP_ABLE             ((uint16_t)0x0008U)
#define DP83848_ANER_NP_ABLE                ((uint16_t)0x0004U)
#define DP83848_ANER_PAGE_RECEIVED          ((uint16_t)0x0002U)
#define DP83848_ANER_LP_AUTONEG_ABLE        ((uint16_t)0x0001U)

// DP83848 ANNPTR Bit Definition
#define DP83848_ANNPTR_NEXT_PAGE         ((uint16_t)0x8000U)
#define DP83848_ANNPTR_MESSAGE_PAGE      ((uint16_t)0x2000U)
#define DP83848_ANNPTR_ACK2              ((uint16_t)0x1000U)
#define DP83848_ANNPTR_TOGGLE            ((uint16_t)0x0800U)
#define DP83848_ANNPTR_MESSAGGE_CODE     ((uint16_t)0x07FFU)

// DP83848 PHYSTS Bit Definition
#define DP83848_PHYSTS_MDIX_MODE		((uint16_t)0x4000U)
#define DP83848_PHYSTS_RECV_ERROR		((uint16_t)0x2000U)
#define DP83848_PHYSTS_POL_STATUS		((uint16_t)0x1000U)
#define DP83848_PHYSTS_FALSE_CARRIER	((uint16_t)0x0800U)
#define DP83848_PHYSTS_SIG_DETECT		((uint16_t)0x0400U)
#define DP83848_PHYSTS_DESCR_LOCK		((uint16_t)0x0200U)
#define DP83848_PHYSTS_PAGE_RECVD 		((uint16_t)0x0100U)
#define DP83848_PHYSTS_MII_INT	 		((uint16_t)0x0080U)
#define DP83848_PHYSTS_REMOTE_FAULT		((uint16_t)0x0040U)
#define DP83848_PHYSTS_JABBER_DET 		((uint16_t)0x0020U)
#define DP83848_PHYSTS_AUTONEG_CPLT		((uint16_t)0x0010U)
#define DP83848_PHYSTS_LOOPBACK			((uint16_t)0x0008U)
#define DP83848_PHYSTS_DUPLEX			((uint16_t)0x0004U)
#define DP83848_PHYSTS_SPEED10			((uint16_t)0x0002U)
#define DP83848_PHYSTS_LINK_STATUS		((uint16_t)0x0001U)

// DP83848 MICR Bit Definition
#define DP83848_MICR_TEST_INT			((uint16_t)0x0004U)
#define DP83848_MICR_INT_EN				((uint16_t)0x0002U)
#define DP83848_MICR_INT_OE				((uint16_t)0x0001U)

// DP83848 MISR Bit Definition
#define DP83848_MISR_ED_INT				((uint16_t)0x4000U)
#define DP83848_MISR_LINK_INT			((uint16_t)0x2000U)
#define DP83848_MISR_SPD_INT			((uint16_t)0x1000U)
#define DP83848_MISR_DUP_INT			((uint16_t)0x0800U)
#define DP83848_MISR_ANC_INT			((uint16_t)0x0400U)
#define DP83848_MISR_FHF_INT			((uint16_t)0x0200U)
#define DP83848_MISR_RHF_INT			((uint16_t)0x0100U)
#define DP83848_MISR_ED_INT_EN			((uint16_t)0x0040U)
#define DP83848_MISR_LINK_INT_EN		((uint16_t)0x0020U)
#define DP83848_MISR_SPD_INT_EN			((uint16_t)0x0010U)
#define DP83848_MISR_DUP_INT_EN			((uint16_t)0x0008U)
#define DP83848_MISR_ANC_INT_EN			((uint16_t)0x0004U)
#define DP83848_MISR_FHF_INT_EN			((uint16_t)0x0002U)
#define DP83848_MISR_RHF_INT_EN			((uint16_t)0x0001U)

// DP83848 PCSR Bit Definition (100 Mb/s PCS Configuration and Status Register)
// ... NOT NEEDED

// DP83848 RBR Bit Definition
#define DP83848_RBR_RMII_MODE			((uint16_t)0x0020U)
#define DP83848_RBR_RMII_REV1			((uint16_t)0x0010U)
#define DP83848_RBR_RX_OVF_STS			((uint16_t)0x0008U)
#define DP83848_RBR_RX_UNF_STS			((uint16_t)0x0004U)
#define DP83848_RBR_ELAST_BUF			((uint16_t)0x0003U)

// DP83848 LEDCR Bit Definition
#define DP83848_LEDCR_DRV_SPDLED		((uint16_t)0x0020U)
#define DP83848_LEDCR_DRV_LNKLED		((uint16_t)0x0010U)
#define DP83848_LEDCR_DRV_ACTLED		((uint16_t)0x0008U)
#define DP83848_LEDCR_SPDLED			((uint16_t)0x0004U)
#define DP83848_LEDCR_LNKLED			((uint16_t)0x0002U)
#define DP83848_LEDCR_ACTLED			((uint16_t)0x0001U)

// DP83848 PHYCR Bit Definition
#define DP83848_PHYCR_MDIX_EN			((uint16_t)0x8000U)
#define DP83848_PHYCR_FORCE_MDIX		((uint16_t)0x4000U)
#define DP83848_PHYCR_PAUSE_RX			((uint16_t)0x2000U)
#define DP83848_PHYCR_PAUSE_TX			((uint16_t)0x1000U)
#define DP83848_PHYCR_BIST_FE			((uint16_t)0x0800U)
#define DP83848_PHYCR_PSR_15			((uint16_t)0x0400U)
#define DP83848_PHYCR_BIST_STATUS		((uint16_t)0x0200U)
#define DP83848_PHYCR_BIST_START		((uint16_t)0x0100U)
#define DP83848_PHYCR_BP_STRETCH		((uint16_t)0x0080U)
#define DP83848_PHYCR_LED_CNFG			((uint16_t)0x0060U)
#define DP83848_PHYCR_PHYADR			((uint16_t)0x001FU)

// DP83848 10BTSCR Bit Definition (10 Base-T Status/Control Register)
// ... NOT NEEDED

// DP83848 CDCTRL1 Bit Definition (CD Test and BIST Extensions Register)
// ... NOT NEEDED

// DP83848 EDCR Bit Definition (Energy Detect Control)
// ... NOT NEEDED
 /**
   * @}
   */

/** @defgroup dp83848_st DP83848 status defines
  * @brief DP83848 status defines
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

 /** @defgroup dp83848_it DP83848 IT flags defines
   * @brief DP83848 IT flags defines
   * @{
   */
#define  DP83848_ED_INT		DP83848_MISR_ED_INT_EN
#define  DP83848_LINK_INT	DP83848_MISR_LINK_INT_EN
#define  DP83848_SPD_INT	DP83848_MISR_SPD_INT_EN
#define  DP83848_DUP_INT	DP83848_MISR_DUP_INT_EN	
#define  DP83848_ANC_INT	DP83848_MISR_ANC_INT_EN	
#define  DP83848_FHF_INT	DP83848_MISR_FHF_INT_EN	
#define  DP83848_RHF_INT	DP83848_MISR_RHF_INT_EN	
 /**
   * @}
   */

/* Exported types ------------------------------------------------------------*/ 
typedef int32_t  (*DP83848_Init_Func) (void);
typedef int32_t  (*DP83848_DeInit_Func) (void);
typedef int32_t  (*DP83848_ReadReg_Func)   (uint32_t, uint32_t, uint32_t *);
typedef int32_t  (*DP83848_WriteReg_Func)  (uint32_t, uint32_t, uint32_t);
typedef int32_t  (*DP83848_GetTick_Func)  (void);

/// Ethernet PHY controller IO description structure
typedef struct 
{                   
  DP83848_Init_Func      Init;
  DP83848_DeInit_Func    DeInit;
  DP83848_WriteReg_Func  WriteReg;
  DP83848_ReadReg_Func   ReadReg;
  DP83848_GetTick_Func   GetTick;
} DP83848_IOCtx_t;

/// Ethernet PHY controller description structure
typedef struct 
{
  uint32_t            DevAddr;
  uint32_t            Is_Initialized;
  DP83848_IOCtx_t     IO;
  void               *pData;
} DP83848_Object_t;

/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Register IO functions to component object
  * @param  [inout] pObj - device object  of DP83848_Object_t.
  * @param  ioctx: holds device IO functions.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_ERROR if missing mandatory function
  */
int32_t DP83848_RegisterBusIO(DP83848_Object_t *pObj, DP83848_IOCtx_t *ioctx);

/**
  * @brief  Initialize the DP83848 and configure the needed hardware resources
  * @param  [inout] pObj - device object DP83848_Object_t.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_ADDRESS_ERROR if cannot find device address
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  *         DP83848_STATUS_RESET_TIMEOUT if cannot perform a software reset
  */
int32_t DP83848_Init(DP83848_Object_t *pObj);

/**
  * @brief  De-Initialize the DP83848 and it's hardware resources
  * @param  [inout] pObj - device object DP83848_Object_t.
  * @retval None
  */
int32_t DP83848_DeInit(DP83848_Object_t *pObj);

/**
  * @brief  Disable the DP83848 power down mode.
  * @param  [in] pObj - device object DP83848_Object_t.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_DisablePowerDownMode(DP83848_Object_t *pObj);

/**
  * @brief  Enable the DP83848 power down mode.
  * @param  [in] pObj - device object DP83848_Object_t.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_EnablePowerDownMode(DP83848_Object_t *pObj);

/**
  * @brief  Start the auto negotiation process.
  * @param  [in] pObj - device object DP83848_Object_t.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_StartAutoNego(DP83848_Object_t *pObj);

/**
  * @brief  Get the link state of DP83848 device.
  * @param  [in] pObj - Pointer to device object.
  * @retval DP83848_STATUS_LINK_DOWN  if link is down
  *         DP83848_STATUS_AUTONEGO_NOTDONE if Auto nego not completed
  *         DP83848_STATUS_100MBITS_FULLDUPLEX if 100Mb/s FD
  *         DP83848_STATUS_100MBITS_HALFDUPLEX if 100Mb/s HD
  *         DP83848_STATUS_10MBITS_FULLDUPLEX  if 10Mb/s FD
  *         DP83848_STATUS_10MBITS_HALFDUPLEX  if 10Mb/s HD
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_GetLinkState(DP83848_Object_t *pObj);

/**
  * @brief  Set the link state of DP83848 device.
  * @param  [in] pObj - pointer to device object.
  * @param  [in] LinkState - link state can be one of the following
  *         DP83848_STATUS_100MBITS_FULLDUPLEX if 100Mb/s FD
  *         DP83848_STATUS_100MBITS_HALFDUPLEX if 100Mb/s HD
  *         DP83848_STATUS_10MBITS_FULLDUPLEX  if 10Mb/s FD
  *         DP83848_STATUS_10MBITS_HALFDUPLEX  if 10Mb/s HD
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_ERROR  if parameter error
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_SetLinkState(DP83848_Object_t *pObj, uint32_t LinkState);

/**
  * @brief  Enable loopback mode.
  * @param  [in] pObj - pointer to device object.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_EnableLoopbackMode(DP83848_Object_t *pObj);

/**
  * @brief  Disable loopback mode.
  * @param  [in] pObj - pointer to device object.
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_DisableLoopbackMode(DP83848_Object_t *pObj);

/**
  * @brief  Enable IT source.
  * @param  [in] pObj - pointer to device object.
  * @param  [in] Interrupt - IT source to be enabled
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_EnableIT(DP83848_Object_t *pObj, uint32_t Interrupt);

/**
  * @brief  Disable IT source.
  * @param  [in] pObj - pointer to device object.
  * @param  [in] Interrupt - IT source to be disabled
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  *         DP83848_STATUS_WRITE_ERROR if connot write to register
  */
int32_t DP83848_DisableIT(DP83848_Object_t *pObj, uint32_t Interrupt);

/**
  * @brief  Clear IT flag.
  * @param  [in] pObj - pointer to device object.
  * @param  [in] Interrupt - IT flag to be cleared
  * @retval DP83848_STATUS_OK  if OK
  *         DP83848_STATUS_READ_ERROR if connot read register
  */
int32_t DP83848_ClearIT(DP83848_Object_t *pObj, uint32_t Interrupt);

/**
  * @brief  Get IT Flag status.
  * @param  [in] pObj - pointer to device object.
  * @param  [in] Interrupt - IT Flag to be checked,
  * @retval 1 IT flag is SET
  *         0 IT flag is RESET
  *         DP83848_STATUS_READ_ERROR if connot read register
  */
int32_t DP83848_GetITStatus(DP83848_Object_t *pObj, uint32_t Interrupt);

/**
  * @brief  Get flag by mask
  * @param  [in] pObj - pointer to device object.
  * @param  [in] reg - register index
  * @param  [in] mask - flag mask
  * @retval 1 if flags corresponds mask
  *         0 if flags not corresponds mask
  *         DP83848_STATUS_READ_ERROR if cannot read register
  */
int32_t DP83848_GetFlagByMask(DP83848_Object_t *pObj, uint32_t reg, uint16_t mask);

/**
  * @brief  Set flag by mask
  * @param  [in] pObj - pointer to device object.
  * @param  [in] reg - register index
  * @param  [in] mask - flag mask
  * @retval result - DP83848 status
  */
int32_t DP83848_SetFlagByMask(DP83848_Object_t *pObj, uint32_t reg, uint16_t mask);

/**
  * @brief  Reset flag by mask
  * @param  [in] pObj - pointer to device object.
  * @param  [in] reg - register index
  * @param  [in] mask - flag mask
  * @retval result - DP83848 status
  */
int32_t DP83848_ResetFlagByMask(DP83848_Object_t *pObj, uint32_t reg, uint16_t mask);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __DP83848_H */

