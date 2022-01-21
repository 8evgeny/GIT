################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32F7xx_HAL_Driver/Src/Legacy/stm32f7xx_hal_can.c 

OBJS += \
./Drivers/STM32F7xx_HAL_Driver/Src/Legacy/stm32f7xx_hal_can.o 

C_DEPS += \
./Drivers/STM32F7xx_HAL_Driver/Src/Legacy/stm32f7xx_hal_can.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32F7xx_HAL_Driver/Src/Legacy/%.o: ../Drivers/STM32F7xx_HAL_Driver/Src/Legacy/%.c Drivers/STM32F7xx_HAL_Driver/Src/Legacy/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


