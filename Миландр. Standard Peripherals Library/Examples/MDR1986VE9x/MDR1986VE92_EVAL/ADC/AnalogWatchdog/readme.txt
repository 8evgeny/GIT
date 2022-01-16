  /**
  @addtogroup ADC_AnalogWatchdog_92 ADC_AnalogWatchdog for MDR1986VE92 evaluation board
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2021 Milandr ******************************
  * @file    readme.txt
  * @author  Milandr Application Team
  * @version V2.0.0
  * @date    27/07/2021
  * @brief   Description of the ADC_AnalogWatchdog Example.
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

This example describes how to use the ADC analog watchdog to guard continuously
an ADC channel. The ADC1 is configured to convert continuously ADC channel 7.
The analog watchdog is configured and enabled to guard a single regular channel.
Each time the channel 7 converted value exceed programmed analog watchdog high
threshold (value 0x0900) or goes down analog watchdog low threshold (value 0x0800)
an AWD interrupt is generated and the output pin connected to LED1 is turn on.
The LED1 will bright as long as the AWD interrupt is generated which means
that the converted value of regular ADC channel 7 is outside the range limited by high
and low analog watchdog thresholds.
The LED2 will bright as long as the converted value of regular ADC channel 7 is  above the upper limit.
Regular ADC channel 7 connected to the trim resistor.


@par Directory contains:

    - main.c                         Main program
    - MDR32F9Qx_it.c                 Main Interrupt Service Routines
    - MDR32F9Qx_it.h                 Functions prototypes for the interrupt


@par Hardware and Software environment:

    - This example is intended to run on MDR1986VE92 eval board with MDR1986VE92 microcontroller.
    - ADC_INP_SEL jamper must be switched into a state TRIM.


@par How to use.

To launch the example, you must do the following:
    - Create a project and setup all project configurations.
    - Add main.c file.
    - Add the required files from "Libraries" folder:
        MDR32F9Qx_config.h
        MDR32F9Qx_port.c
        MDR32F9Qx_rst_clk.c
        MDR32F9Qx_adc.c
    - Edit the MDR32F9Qx_config.h to set appropriate run-time parameter checking level.
    - Compile and link together all .c files and load your image into the target board.
    - Run the example.

 * <h3><center>&copy; COPYRIGHT 2021 Milandr</center></h3>
 */
