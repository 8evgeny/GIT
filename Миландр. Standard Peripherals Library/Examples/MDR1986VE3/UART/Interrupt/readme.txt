  /**
  @addtogroup UART_Interrupt_3T UART_Interrupt for MDR1986VE3 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    02/08/2021
  * @brief   Description of the UART_Interrupt Example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
  @endlatexonly

@par Example Description

This example shows how to use UART.
Configure PORTD lines 13, 14 as UART2_TX,UART2_RX.
Configure UARTs on an exchange with 9600 Baud rate, 8 bit data size, 1 stop-bit and no parity check.
Enable transmitter interrupt for UART2.
Enable receiver interrupt for UART2.
Get interrupts status.
Clear interrupts pending bits.
UART2 transmit word.
UART2 recive word.


@par Directory contains:

    - main.c                       Main program
    - MDR1986VE3_IT.c              Main Interrupt Service Routines
    - MDR1986VE3_IT.h              Functions prototypes for the interrupt


@par Hardware and Software environment:

    - This example is intended to run on MDR1986VE3 eval board with MDR1986VE3 microcontroller.
    - Connect UART2 TX (PD13) pin (XP29.2) to UART2 TXD (PD14) pin (XP23.3).


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add MDR1986VE3_IT.c and MDR1986VE3_IT.h files.
    - Add the required files from "Libraries" folder:
        MDR32F9Qx_config.h
        MDR32F9Qx_port.c
        MDR32F9Qx_rst_clk.c
        MDR32F9Qx_uart.c
    - Edit the MDR32F9Qx_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
