  /**
  @addtogroup CAN_LoopBack_Interrupt CAN_LoopBack_Interrupt for MDR1986VE91 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    16/07/2021
  * @brief   Description of the CAN_LoopBack_Interrupt Example.
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

This example provides a description of how to set a communication with the CAN in loopback mode.
The CAN cell performs a transmission of a extended data frame at 500 Kbits/s. Reception is done in the interrupt handler. 
The received frame is checked and LED1 light up to indicate what the communication was successful, else LED2 light up.


@par Directory contains:

    - main.c              Main program
    - MDR32F9Qx_it.c      Main Interrupt Service Routines
    - MDR32F9Qx_it.h      Functions prototypes for the interrupt


@par Hardware and Software environment:

    - This example is intended to run on MDR32F9Q1 eval board with MDR32F9Q1 microcontroller.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add MDR32F9Qx_it.c and MDR32F9Qx_it.h files.
    - Add the required files from "Libraries" folder:
        MDR32F9Qx_config.h
        MDR32F9Qx_port.c
        MDR32F9Qx_rst_clk.c
        MDR32F9Qx_can.c
    - Edit the MDR32F9Qx_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
