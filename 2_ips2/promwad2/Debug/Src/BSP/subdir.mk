################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/BSP/stm32746g_discovery.c \
../Src/BSP/stm32746g_discovery_sdram.c 

OBJS += \
./Src/BSP/stm32746g_discovery.o \
./Src/BSP/stm32746g_discovery_sdram.o 

C_DEPS += \
./Src/BSP/stm32746g_discovery.d \
./Src/BSP/stm32746g_discovery_sdram.d 


# Each subdirectory must supply rules for building sources it contributes
Src/BSP/%.o: ../Src/BSP/%.c Src/BSP/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


