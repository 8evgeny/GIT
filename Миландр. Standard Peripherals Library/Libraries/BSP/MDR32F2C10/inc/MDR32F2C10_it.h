
/* Includes ------------------------------------------------------------------*/
#include "MDR32F2C10_port.h"
#include "MDR32F2C10_clk.h"
#include "MDR32F2C10_uart.h"

void UartSendBuff( MDR_UART_TypeDef* UARTx, uint8_t* pucData, uint16_t usLenght );
void UART0_IRQHandler(void);
void UART1_IRQHandler(void);