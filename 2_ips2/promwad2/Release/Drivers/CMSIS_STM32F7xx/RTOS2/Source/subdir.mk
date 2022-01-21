################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_systick.c \
../Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_tick_gtim.c \
../Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_tick_ptim.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_systick.o \
./Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_tick_gtim.o \
./Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_tick_ptim.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_systick.d \
./Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_tick_gtim.d \
./Drivers/CMSIS_STM32F7xx/RTOS2/Source/os_tick_ptim.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/RTOS2/Source/%.o: ../Drivers/CMSIS_STM32F7xx/RTOS2/Source/%.c Drivers/CMSIS_STM32F7xx/RTOS2/Source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


