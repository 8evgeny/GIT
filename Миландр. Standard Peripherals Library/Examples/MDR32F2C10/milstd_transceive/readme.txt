  /**
  \addtogroup milstd_transceive readme
  @{
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.2
  * @date    28/01/2021
  * @brief   Description of the milstd_transceive Example.
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

This example shows, how to use MILSTD.
In this demo there are MILSTD initialisation function and MILSTD test function, which transmit data from MDR_MIL1_STD_1553 and receive to MDR_MIL0_STD_1553. 
User can validate communication checking TestData array of received values in debug mode.
For this test user needs to install external MILSTD modules with XS6 and XS7 of demo board and connecting inputs and outputs.

@par Directory contains:

  - main.c              Main program

@par Hardware and Software environment:

  - This example is intended to run on MDR1986VK01 demo board with MDR1986VK01 
  microcontroller.

@par How to use.

To launch the example, you must do the following:
  - Create a project and setup all project configurations.
  - Add main.c file.
  - Add the required files from "Libraries" folder:
        MDR32F2C10_port.h
        MDR32F2C10_clk.h
        MDR32F2C10_milstd.h
        demoboard.h
        milstd.h
        
  - Edit the MDR32F2C10_config.h to set appropriate run-time parameter checking level.
  - Compile and link together all .c files and load your image into the target board.
  - Run the example.

 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
