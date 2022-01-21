################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/arm_common_tables.c \
../Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/arm_const_structs.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/arm_common_tables.o \
./Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/arm_const_structs.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/arm_common_tables.d \
./Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/arm_const_structs.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/%.c Drivers/CMSIS_STM32F7xx/DSP/Source/CommonTables/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


