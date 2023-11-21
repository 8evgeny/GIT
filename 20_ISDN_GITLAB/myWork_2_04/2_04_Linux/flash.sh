#!/usr/bin/bash

#st-flash erase 
#st-flash --reset write build_release/20_Build_code_Krasnov.bin 0x8000000

st-flash --reset write  ../2_04_Keil/MDK-ARM/ObjectFiles/R_1_DTS_011M_STM32F429ZG.hex 0x8000000
