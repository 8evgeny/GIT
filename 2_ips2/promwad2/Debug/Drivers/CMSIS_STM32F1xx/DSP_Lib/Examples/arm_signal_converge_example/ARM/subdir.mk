################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/arm_signal_converge_data.c \
../Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/arm_signal_converge_example_f32.c \
../Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/math_helper.c 

OBJS += \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/arm_signal_converge_data.o \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/arm_signal_converge_example_f32.o \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/math_helper.o 

C_DEPS += \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/arm_signal_converge_data.d \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/arm_signal_converge_example_f32.d \
./Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/math_helper.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/%.o: ../Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/%.c Drivers/CMSIS_STM32F1xx/DSP_Lib/Examples/arm_signal_converge_example/ARM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


