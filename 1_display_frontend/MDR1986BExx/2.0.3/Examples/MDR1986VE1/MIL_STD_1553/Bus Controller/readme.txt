  /**
  @addtogroup MIL_STD_1553_Bus_Controller_1T MIL_STD_1553 Bus Controller for MDR1986VE1T evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    18/06/2021
  * @brief   Description work of MIL_STD_1553 in Bus Controller mode.
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

This example provides a description of how to use MIL STD 1553 in Bus Controller
mode. Connect MDR1986VE1T Opora Evaluation Board to PC by UART. Select one
of the support command vy pressing key in terminal window:
    - 1 - Write data to terminal device
    - 2 - Read data from terminal device
    - 3 - Get status word
    - 4 - Get last command word


@par Directory contains:

    - main.c                          Main program
    - MDR1986VE1T_IT.c                Main Interrupt Service Routines
    - MIL_STD_1553_IT_Handler.c       Main function for processing MIL_STD_1553 interrupt.
    - MDR1986VE1T_IT.h                Functions prototypes for the interrupt.
    - MIL_STD_1553_BusController.h    Functions prototypes.
    - MIL_STD_1553_IT_Handler.h       Functions prototypes for the MIL_STD_1553 interrupt.


@par Hardware and Software environment:

    - This example is intended to run on MDR1986VE1T eval board with MDR1986VE1T microcontroller.
    - XP18 must be enable in position UART.
    - XP19 must be enable in position UART1.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F9Qx_config.h
        MDR32F9Qx_port.c
        MDR32F9Qx_rst_clk.c
        MDR32F9Qx_timer.c
        MDR32F9Qx_mil_std_1553.c
        MDR32F9Qx_eeprom.c
        MDR32F9Qx_uart.c
    - Edit the MDR32F9Qx_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.


 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
