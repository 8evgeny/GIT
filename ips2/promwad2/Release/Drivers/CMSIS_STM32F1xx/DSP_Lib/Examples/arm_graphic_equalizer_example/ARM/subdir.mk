################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/arm_graphic_equalizer_data.c \
../Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/arm_graphic_equalizer_example_q31.c \
../Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/math_helper.c 

OBJS += \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/arm_graphic_equalizer_data.o \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/arm_graphic_equalizer_example_q31.o \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/math_helper.o 

C_DEPS += \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/arm_graphic_equalizer_data.d \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/arm_graphic_equalizer_example_q31.d \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/math_helper.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/%.o: ../Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/%.c Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_graphic_equalizer_example/ARM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


