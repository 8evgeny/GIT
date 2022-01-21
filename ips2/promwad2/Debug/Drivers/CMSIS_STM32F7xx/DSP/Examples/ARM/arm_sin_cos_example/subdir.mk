################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/Examples/ARM/arm_sin_cos_example/arm_sin_cos_example_f32.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/Examples/ARM/arm_sin_cos_example/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/Examples/ARM/arm_sin_cos_example/%.c Drivers/CMSIS_STM32F7xx/DSP/Examples/ARM/arm_sin_cos_example/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


