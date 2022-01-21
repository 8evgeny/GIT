################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/RTOS2/Template/cmsis_os1.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/RTOS2/Template/cmsis_os1.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/RTOS2/Template/cmsis_os1.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/RTOS2/Template/%.o: ../Drivers/CMSIS_STM32F7xx/RTOS2/Template/%.c Drivers/CMSIS_STM32F7xx/RTOS2/Template/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


