  /**
  @page ADCUI_DMA example readme for MDR32F02 demo board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ****************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    14/07/2021
  * @brief   Description of the ADCUI_DMA Example.
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

This example shows, how to use DMA with ADCUI.

The program configures DMA controller to read data from ADCUI channel and place it into a buffer.
After that, data is sent to PC by UART.
The data is sent in a tab-separated format, ready to copy-paste to MS Excel and build a plot


@par Directory contains:

    - MLDR187_SPL							SPL library v0.29
    - src/adcui_dma.c						ADCUI DMA wrapper. Will be included in SPL in the next release
    - src/adcui_dma.h
    - src/init.c							Common initialisation of used peripherals
    - src/init.h
    - src/main.c							Main program
    - src/startup_MLDR187.S					default bootloader
    - src/syscalls.c						syscalls wrappers needed for printf to send data to UART
    - src/link_Flash.ld						linker script for building program to be stored in flash
    - src/link_RAM.ld						linker script for debugging in RAM
    - MLDR187-ft2232hl.cfg					OpenOCD script to connect to ft2232hl (interface 0) USB-UART bridge tied to JTAG on demo board
    - MLDR187-jlink.cfg						OpenOCD script to connect to Jlink probe attached to JTAG socket on demo board
    - MLDR187.svd							Peripherals memory-mapped registers definitions


@par Hardware and Software environment:

    - This example is intended to run on MLDR187B_Ver2 demo board with MDR32F02 microcontroller.


@par How to use.

To launch the example, you must do the following:
    - Connect V0 signal source to V0_P, V0_N pins on demo board
    - Connect I0 signal source to I0_P, I0_N pins on demo board
    - Connect I3 signal source to I3_P, I3_N pins on demo board
    - Connect serial terminal to COM port, associated with ft2232hl (Interface 1) device
    - Configure Eclipse IDE with RISC-V toolchain
    - Import this project: File -> Import... -> Existing Projects into Workspace
    - Create launch configuration
    - Build and Run the example.
    - Check serial port output

 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
