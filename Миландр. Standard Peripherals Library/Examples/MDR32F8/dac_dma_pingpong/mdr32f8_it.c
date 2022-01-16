/**
  ******************************************************************************
  * @file    MDR32F8_it.c
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    02/09/2015
  * @brief   Main Interrupt Service Routines.
  *
  ******************************************************************************
  * <br><br>
  *
  *
  * <h2><center>&copy; COPYRIGHT 2015 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/

#include "mdr32f8_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_ChannelInitTypeDef DMA_InitStr;
extern DMA_CtrlDataInitTypeDef DMA_PriCtrlStr;
extern DMA_CtrlDataInitTypeDef DMA_AltCtrlStr;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void DMA_DONE0_Handler(void)
{
  if((MDR_DMA->CHNL_PRI_ALT_SET &(1<<DMA_Channel_TIM1)) == (0<<DMA_Channel_TIM1))
  {
   DMA_CtrlInit(DMA_Channel_TIM1, DMA_CTRL_DATA_ALTERNATE,  &DMA_AltCtrlStr);
  }
  else if((MDR_DMA->CHNL_PRI_ALT_SET & (1<<DMA_Channel_TIM1)) == (1<<DMA_Channel_TIM1))
  {
   DMA_CtrlInit(DMA_Channel_TIM1, DMA_CTRL_DATA_PRIMARY, &DMA_PriCtrlStr);
  }
}
/******************* (C) COPYRIGHT 2015 Milandr *********/

/* END OF FILE MDR32F8_it.c */
