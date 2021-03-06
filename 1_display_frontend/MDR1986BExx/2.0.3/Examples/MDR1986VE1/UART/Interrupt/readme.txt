  /**
  @addtogroup UART_Interrupt_1T UART_Interrupt for MDR1986VE1T evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    11/05/2021
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
Configure PORTB lines 5,6 as UART1_TX,UART1_RX.
Configure PORTD lines 0,1 as UART2_RX,UART2_TX.
Configure UARTs on an exchange with 115000 Baud rate, 8 bit data size, 1 stop-bit and no parity check.
Enable transmitter interrupt for UART1.
Enable receiver interrupt for UART2.
Get interrupts status.
Clear interrupts pending bits.
UART1 transmit word.
UART2 recive word.


@par Directory contains:

    - main.c                       Main program
    - MDR1986VE1T_IT.c             Main Interrupt Service Routines
    - MDR1986VE1T_IT.h             Functions prototypes for the interrupt


@par Hardware and Software environment:

    - This example is intended to run on MDR1986VE1T eval board with MDR1986VE1T microcontroller.
    - Pin PC3 of XP19. and Pin PD14 XP19. must be connected together.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add MDR1986VE1T_IT.c and MDR1986VE1T_IT.h files.
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
