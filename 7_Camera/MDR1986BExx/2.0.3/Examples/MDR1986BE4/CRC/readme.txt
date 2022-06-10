  /**
  @addtogroup CRC_Calculation CRC_Calculation for MDR1986BE4 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    11/08/2021
  * @brief   Description of the CRC_Calculation Example.
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

This example describes how to use the CRC block.
The CRC value is calculated for the test array by software and hardware methods. 
The calculated CRC values are compared, and if they are equal, the LED is on.

In the example, the hardware calculation of the CRC is performed for CRC_LITTLE_ENDIAN_MODE. 
In CRC_LITTLE_ENDIAN_MODE, the CRC_DataSize value does not affect the calculation result.

Using the DATA_LOAD_xBIT macros, you can select the size of the data that will be loaded into the FIFO of the CRC block.


@par Directory contains:

    - main.c                   Main program


@par Hardware and Software environment:

    - This example is intended to run on MDR1986BE4 eval board with MDR1986BE4 microcontroller.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F9Qx_config.h
        MDR32F9Qx_port.c
        MDR32F9Qx_rst_clk.c
        MDR32F9Qx_crc.c
        MDR32F9Qx_eeprom.c
        MDR32F9Qx_bkp.c
    - Edit the MDR32F9Qx_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.


 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
