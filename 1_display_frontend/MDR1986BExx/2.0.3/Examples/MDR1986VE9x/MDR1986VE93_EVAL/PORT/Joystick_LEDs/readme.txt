  /**
  @addtogroup PORT_Joystick_LEDs_93 PORT_Joystick_LEDs for MDR1986VE93 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    28/07/2021
  * @brief   Description of the PORT_Joystick_LEDs Example.
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

This example shows how to use input/output ports.
PORTE is configured for input. Its 3,6 pins are connected to the joystick buttons.
PORTF is configured for output. Its 0,1 pins control LED indicators.


@par Directory contains:

    - main.c          Main program


@par Hardware and Software environment:

    - This example is intended to run on MDR1986VE93 eval board with MDR1986VE93 microcontroller.
    - "LEFT" and "RIGHT" joystick buttons are connected to
      PE.3, PE.6 pins, respectively.
    - VD2, VD3 LEDs are connected to
      PF.0, PF.1 pins, respectively.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F9Qx_config.h
        MDR32F9Qx_port.c
        MDR32F9Qx_rst_clk.c
    - Edit the MDR32F9Qx_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
