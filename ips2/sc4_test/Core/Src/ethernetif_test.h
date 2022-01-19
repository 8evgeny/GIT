/**
 ******************************************************************************
  * File Name          : ethernetif.h
  * Description        : This file provides initialization code for LWIP
  *                      middleWare.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__

/* Within 'USER CODE' section, code will be kept by default at each generation */

/* Exported functions ------------------------------------------------------- */
void ethernetif_init();

void low_level_output(uint8_t *p, uint16_t len);
uint16_t ethernetif_input(ETH_BufferTypeDef *RxBuff);
void ethernet_link_check_state();

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
