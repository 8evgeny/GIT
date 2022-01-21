################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/Core_A/Source/irq_ctrl_gic.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/Core_A/Source/irq_ctrl_gic.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/Core_A/Source/irq_ctrl_gic.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/Core_A/Source/%.o: ../Drivers/CMSIS_STM32F7xx/Core_A/Source/%.c Drivers/CMSIS_STM32F7xx/Core_A/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


