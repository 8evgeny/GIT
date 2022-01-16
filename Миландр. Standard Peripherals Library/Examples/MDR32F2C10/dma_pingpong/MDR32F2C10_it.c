/**
  ******************************************************************************
  * @file    MDR32F2C10_it.c
  * @author  Milandr Application Team
  * @version V1.0.1
  * @date    11/11/2020
  * @brief   Main Interrupt Service Routines.
  *
  ******************************************************************************
  * <br><br>
  *
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/

#include "MDR32F2C10_it.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern DMA_ChannelInitTypeDef DMA_InitStr;
extern DMA_CtrlDataInitTypeDef DMA_PriCtrlStr;
extern DMA_CtrlDataInitTypeDef DMA_AltCtrlStr;

void DMA_DONE0_IRQHandler(void)
{
  if((MDR_DMA->CHNL_PRI_ALT_SET &(1<<0)) == (0<<0))
  {
   DMA_CtrlInit(0, DMA_CTRL_DATA_ALTERNATE,  &DMA_AltCtrlStr);
  }
  else if((MDR_DMA->CHNL_PRI_ALT_SET & (1<<0)) == (1<<0))
  {
   DMA_CtrlInit(0, DMA_CTRL_DATA_PRIMARY, &DMA_PriCtrlStr);
  }
}

/******************* (C) COPYRIGHT 2021 Milandr *********/

/* END OF FILE MDR32F2C10_it.c */
