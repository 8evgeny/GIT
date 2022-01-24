  /**
  \addtogroup spi_flash1636 readme
  @{
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.2
  * @date    28/01/2021
  * @brief   Description of the spi_flash1636 Example.
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

This example shows, how to use SPI.
In this demo there are SPI initialisation function and SPI test function, which write data to 1636 flash and read it back. 
User can validate communication with rtrn value of memcmp function of SPI test function in debug mode.
For this test 1636 flash external module needs to be installed with XS6 on demo board.

@par Directory contains:

  - main.c              Main program
  - MDR32F2C10_it.c     Main Interrupt Service Routines
  - MDR32F2C10_it.h     Functions prototypes for the interrupt

@par Hardware and Software environment:

  - This example is intended to run on MDR1986VK01 demo board with MDR1986VK01
  microcontroller.

@par How to use.

To launch the example, you must do the following:
  - Create a project and setup all project configurations.
  - Add main.c file.
  - Add the required files from "Libraries" folder:
        MDR32F2C10_port.h
        MDR32F2C10_ssp.h
        MDR32F2C10_clk.h
        demoboard.h
        spi.h
        
  - Edit the MDR32F2C10_config.h to set appropriate run-time parameter checking level.
  - Compile and link together all .c files and load your image into the target board.
  - Run the example.

 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
