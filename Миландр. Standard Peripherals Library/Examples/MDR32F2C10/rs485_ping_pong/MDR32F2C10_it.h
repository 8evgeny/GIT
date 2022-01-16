/**
  ******************************************************************************
  * @file    MDR32F2C10_it.h
  * @author  Milandr Application Team
  * @version V1.0.1
  * @date    11/11/2020
  * @brief   Functions prototypes for the interrupt.
  *
  ******************************************************************************
  * <br><br>
  *
  *
  * <h2><center>&copy; COPYRIGHT 2021 Milandr</center></h2>
  */

#include "MDR32F2C10_port.h"
#include "MDR32F2C10_clk.h"
#include "MDR32F2C10_uart.h"

void UartSendBuff( MDR_UART_TypeDef* UARTx, uint8_t* pucData, uint16_t usLenght );
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);

/******************* (C) COPYRIGHT 2021 Milandr *********/

/* END OF FILE MDR32F2C10_it.h */
