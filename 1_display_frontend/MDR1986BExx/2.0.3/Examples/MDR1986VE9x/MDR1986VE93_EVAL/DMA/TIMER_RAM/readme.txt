  /**
  @addtogroup DMA_TIMER_RAM_93 DMA_TIMER_RAM for MDR1986VE93 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    28/07/2021
  * @brief   Description of the DMA_TIMER_RAM Example.
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

This example provides a description of how to use DMA with TIM1 Capture request to transfer Data
from TIM1 Capture Compare Register2 to memory.
The TIM1 Channel1 is configured to generate one pulse when TIM_CNT = CCR1.
TIM1 channel2 is configured to capture events. CCR2 register value transfered to memory using DMA request.
Once the transfer is completed a comparison is done and TransferStatus gives the data transfer status
where it is PASSED if CCR1 register value and transmitted data are the same otherwise it is FAILED.


@par Directory contains:

    - main.c          Main program


@par Hardware and Software environment:

    - This example is intended to run on MDR1986VE93 eval board with MDR1986VE93 microcontroller.
    - Connect TIMER1 CH1 pin (X25.1.12) to TIMER1 CH2 pin (X25.1.10).

@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F9Qx_config.h
        MDR32F9Qx_port.c
        MDR32F9Qx_rst_clk.c
        MDR32F9Qx_timer.c
        MDR32F9Qx_dma.c
    - Edit the MDR32F9Qx_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.


 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
