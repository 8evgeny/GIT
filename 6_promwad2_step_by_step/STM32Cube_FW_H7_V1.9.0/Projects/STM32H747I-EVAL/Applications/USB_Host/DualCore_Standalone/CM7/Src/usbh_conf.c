/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/CM7/Src/usbh_conf.c
  * @author  MCD Application Team
  * @brief   USB Host configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------ */
#include "stm32h7xx_hal.h"
#include "usbh_core.h"
#include "stm32h747i_eval_io.h"

HCD_HandleTypeDef hhcd_FS;
HCD_HandleTypeDef hhcd_HS;

/*******************************************************************************
                       HCD BSP Routines
*******************************************************************************/
/**
  * @brief  Initializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_MspInit(HCD_HandleTypeDef * hhcd)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  BSP_IO_Init_t io_init_structure;


  if (hhcd->Instance == USB2_OTG_FS)
  {
    /* Configure USB FS GPIOs */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure DM DP Pins */
    GPIO_InitStruct.Pin = (GPIO_PIN_11 | GPIO_PIN_12);
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure ID pin */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG2_FS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure VBUS Pin */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Initialize the IO */
    io_init_structure.Pin  =IO_PIN_7;
    io_init_structure.Pull = IO_PULLUP;
    io_init_structure.Mode = IO_MODE_OUTPUT_PP;
    
    BSP_IO_Init(0, &io_init_structure);

    /* Enable USB FS Clocks */
    __HAL_RCC_USB2_OTG_FS_CLK_ENABLE();

    /* Disable USB clock during CSleep mode */
    __HAL_RCC_USB2_OTG_FS_ULPI_CLK_SLEEP_DISABLE();

    /* Set USBFS Interrupt priority */
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0x05, 0);

    /* Enable USBFS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
  }
  else if (hhcd->Instance == USB1_OTG_HS)
  {
    /* Configure USB FS GPIOs */
    __GPIOA_CLK_ENABLE();
    __GPIOB_CLK_ENABLE();
    __GPIOC_CLK_ENABLE();
    __GPIOH_CLK_ENABLE();
    __GPIOI_CLK_ENABLE();

    /* CLK */
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* D0 */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* D1 D2 D3 D4 D5 D6 D7 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_5 |
      GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* STP */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* NXT */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

    /* DIR */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF10_OTG1_HS;
    HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
    __HAL_RCC_USB1_OTG_HS_ULPI_CLK_ENABLE();

    /* Enable USB HS Clocks */
    __HAL_RCC_USB1_OTG_HS_CLK_ENABLE();

    /* Set USBHS Interrupt priority */
    HAL_NVIC_SetPriority(OTG_HS_IRQn, 7, 0);

    /* Enable USBHS Interrupt */
    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);
  }
}

/**
  * @brief  DeInitializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_MspDeInit(HCD_HandleTypeDef * hhcd)
{
  if (hhcd->Instance == USB2_OTG_FS)
  {
    /* Disable USB FS Clocks */
    __HAL_RCC_USB2_OTG_FS_CLK_DISABLE();
  }
  else if (hhcd->Instance == USB1_OTG_HS)
  {
    /* Disable USB HS Clocks */
    __HAL_RCC_USB1_OTG_HS_CLK_DISABLE();
    __HAL_RCC_USB1_OTG_HS_ULPI_CLK_DISABLE();
  }
}

/*******************************************************************************
                       LL Driver Callbacks (HCD -> USB Host Library)
*******************************************************************************/

/**
  * @brief  SOF callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef * hhcd)
{
  USBH_LL_IncTimer(hhcd->pData);
}

/**
  * @brief  Connect callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef * hhcd)
{
  USBH_LL_Connect(hhcd->pData);
}

/**
  * @brief  Disconnect callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef * hhcd)
{
  USBH_LL_Disconnect(hhcd->pData);
}

/**
  * @brief  Port Port Enabled callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_PortEnabled(hhcd->pData);
}


/**
  * @brief  Port Port Disabled callback.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *hhcd)
{
  USBH_LL_PortDisabled(hhcd->pData);
}

/**
  * @brief  Notify URB state change callback.
  * @param  hhcd: HCD handle
  * @param  chnum: Channel number
  * @param  urb_state: URB State
  * @retval None
  */
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef * hhcd,
                                         uint8_t chnum,
                                         HCD_URBStateTypeDef urb_state)
{
  /* To be used with OS to sync URB state with the global state machine */
}

/*******************************************************************************
                       LL Driver Interface (USB Host Library --> HCD)
*******************************************************************************/
/**
  * @brief  USBH_LL_Init
  *         Initialize the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef * phost)
{
  if(phost->id == 0)
  {
    /* Set the LL Driver parameters */
    hhcd_FS.Instance = USB2_OTG_FS;
    hhcd_FS.Init.Host_channels = 11;
    hhcd_FS.Init.dma_enable = 0;
    hhcd_FS.Init.low_power_enable = 0;
    hhcd_FS.Init.phy_itface = HCD_PHY_EMBEDDED;
    hhcd_FS.Init.Sof_enable = 0;
    hhcd_FS.Init.speed = HCD_SPEED_FULL;
    hhcd_FS.Init.vbus_sensing_enable = 0;
    hhcd_FS.Init.lpm_enable = 0;

    /* Link the driver to the stack */
    hhcd_FS.pData = phost;
    phost->pData = &hhcd_FS;

    /* Initialize the LL Driver */
    HAL_HCD_Init(&hhcd_FS);
  }
  else /*phost->id == 1*/
  {
    /* Set the LL driver parameters */
    hhcd_HS.Instance = USB1_OTG_HS;
    hhcd_HS.Init.Host_channels = 11;
    hhcd_HS.Init.dma_enable = 0;
    hhcd_HS.Init.low_power_enable = 0;
    hhcd_HS.Init.phy_itface = HCD_PHY_ULPI;
    hhcd_HS.Init.Sof_enable = 0;
    hhcd_HS.Init.speed = HCD_SPEED_HIGH;
    hhcd_HS.Init.vbus_sensing_enable = 0;
    hhcd_HS.Init.use_external_vbus = 1;
    hhcd_HS.Init.lpm_enable = 0;

    /* Link the driver to the stack */
    hhcd_HS.pData = phost;
    phost->pData = &hhcd_HS;

    /* Initialize the LL driver */
    HAL_HCD_Init(&hhcd_HS);
  }

  USBH_LL_SetTimer (phost, HAL_HCD_GetCurrentFrame(&hhcd_HS));
  USBH_LL_SetTimer (phost, HAL_HCD_GetCurrentFrame(&hhcd_FS));

  return USBH_OK;
}

/**
  * @brief  De-Initializes the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef * phost)
{
  HAL_HCD_DeInit(phost->pData);
  return USBH_OK;
}

/**
  * @brief  Starts the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef * phost)
{
  HAL_HCD_Start(phost->pData);
  return USBH_OK;
}

/**
  * @brief  Stops the Low Level portion of the Host driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef * phost)
{
  HAL_HCD_Stop(phost->pData);
  return USBH_OK;
}

/**
  * @brief  Returns the USB Host Speed from the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Speeds
  */
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef * phost)
{
  USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

  switch (HAL_HCD_GetCurrentSpeed(phost->pData))
  {
  case 0:
    speed = USBH_SPEED_HIGH;
    break;

  case 1:
    speed = USBH_SPEED_FULL;
    break;

  case 2:
    speed = USBH_SPEED_LOW;
    break;

  default:
    speed = USBH_SPEED_FULL;
    break;
  }
  return speed;
}

/**
  * @brief  Resets the Host Port of the Low Level Driver.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef * phost)
{
  HAL_HCD_ResetPort(phost->pData);
  return USBH_OK;
}

/**
  * @brief  Returns the last transferred packet size.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval Packet Size
  */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef * phost, uint8_t pipe)
{
  return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

/**
  * @brief  Opens a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  epnum: Endpoint Number
  * @param  dev_address: Device USB address
  * @param  speed: Device Speed
  * @param  ep_type: Endpoint Type
  * @param  mps: Endpoint Max Packet Size
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef * phost,
                                    uint8_t pipe,
                                    uint8_t epnum,
                                    uint8_t dev_address,
                                    uint8_t speed,
                                    uint8_t ep_type, uint16_t mps)
{
  HAL_HCD_HC_Init(phost->pData, pipe, epnum, dev_address, speed, ep_type, mps);
  return USBH_OK;
}

/**
  * @brief  Closes a pipe of the Low Level Driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef * phost, uint8_t pipe)
{
  HAL_HCD_HC_Halt(phost->pData, pipe);
  return USBH_OK;
}

/**
  * @brief  Submits a new URB to the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *          This parameter can be a value from 1 to 15
  * @param  direction: Channel number
  *          This parameter can be one of these values:
  *           0: Output
  *           1: Input
  * @param  ep_type: Endpoint Type
  *          This parameter can be one of these values:
  *            @arg EP_TYPE_CTRL: Control type
  *            @arg EP_TYPE_ISOC: Isochronous type
  *            @arg EP_TYPE_BULK: Bulk type
  *            @arg EP_TYPE_INTR: Interrupt type
  * @param  token: Endpoint Type
  *          This parameter can be one of these values:
  *            @arg 0: PID_SETUP
  *            @arg 1: PID_DATA
  * @param  pbuff: pointer to URB data
  * @param  length: length of URB data
  * @param  do_ping: activate do ping protocol (for high speed only)
  *          This parameter can be one of these values:
  *           0: do ping inactive
  *           1: do ping active
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef * phost,
                                     uint8_t pipe,
                                     uint8_t direction,
                                     uint8_t ep_type,
                                     uint8_t token,
                                     uint8_t * pbuff,
                                     uint16_t length, uint8_t do_ping)
{
  HAL_HCD_HC_SubmitRequest(phost->pData,
                           pipe,
                           direction, ep_type, token, pbuff, length, do_ping);
  return USBH_OK;
}

/**
  * @brief  Gets a URB state from the low level driver.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  *          This parameter can be a value from 1 to 15
  * @retval URB state
  *          This parameter can be one of these values:
  *            @arg URB_IDLE
  *            @arg URB_DONE
  *            @arg URB_NOTREADY
  *            @arg URB_NYET
  *            @arg URB_ERROR
  *            @arg URB_STALL
  */
USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef * phost,
                                         uint8_t pipe)
{
  return (USBH_URBStateTypeDef) HAL_HCD_HC_GetURBState(phost->pData, pipe);
}

/**
  * @brief  Drives VBUS.
  * @param  phost: Host handle
  * @param  state: VBUS state
  *          This parameter can be one of these values:
  *           0: VBUS Active
  *           1: VBUS Inactive
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef * phost, uint8_t state)
{
#ifdef USE_USB_FS
  if (state == 0)
  {
    /* Configure Low Charge pump */
    BSP_IO_WritePin(0, IO_PIN_7,IO_PIN_RESET);  }
  else
  {
    /* Drive High Charge pump */
    BSP_IO_WritePin(0, IO_PIN_7,IO_PIN_SET);
  }
#endif
  HAL_Delay(200);
  return USBH_OK;
}

/**
  * @brief  Sets toggle for a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @param  toggle: toggle (0/1)
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef * phost, uint8_t pipe,
                                     uint8_t toggle)
{
  if(phost->id == 0)
  {
    if(hhcd_FS.hc[pipe].ep_is_in)
    {
      hhcd_FS.hc[pipe].toggle_in = toggle;
    }
    else
    {
      hhcd_FS.hc[pipe].toggle_out = toggle;
    }
  }
  else
  {
    if(hhcd_HS.hc[pipe].ep_is_in)
    {
      hhcd_HS.hc[pipe].toggle_in = toggle;
    }
    else
    {
      hhcd_HS.hc[pipe].toggle_out = toggle;
    }
  }
  return USBH_OK;
}

/**
  * @brief  Returns the current toggle of a pipe.
  * @param  phost: Host handle
  * @param  pipe: Pipe index
  * @retval toggle (0/1)
  */
uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef * phost, uint8_t pipe)
{
  uint8_t toggle = 0;
  if(phost->id == 0)
  {
    if(hhcd_FS.hc[pipe].ep_is_in)
    {
      toggle = hhcd_FS.hc[pipe].toggle_in;
    }
    else
    {
      toggle = hhcd_FS.hc[pipe].toggle_out;
    }
  }
  else
  {
    if(hhcd_HS.hc[pipe].ep_is_in)
    {
      toggle = hhcd_HS.hc[pipe].toggle_in;
    }
    else
    {
      toggle = hhcd_HS.hc[pipe].toggle_out;
    }
  }
  return toggle;
}

/**
  * @brief  Delay routine for the USB Host Library
  * @param  Delay: Delay in ms
  * @retval None
  */
void USBH_Delay(uint32_t Delay)
{
#if (USBH_USE_OS == 1)
  osDelay(Delay);
#else
  HAL_Delay(Delay);
#endif
}

