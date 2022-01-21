################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/main_s.c \
../Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/tz_context.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/main_s.o \
./Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/tz_context.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/main_s.d \
./Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/tz_context.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/%.o: ../Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/%.c Drivers/CMSIS_STM32F7xx/Core/Template/ARMv8-M/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


