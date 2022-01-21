################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Src_STM32F1xx/stm32f1xx_hal_msp.c \
../Src/Src_STM32F1xx/stm32f1xx_hal_timebase_tim.c \
../Src/Src_STM32F1xx/stm32f1xx_it.c \
../Src/Src_STM32F1xx/system_stm32f1xx.c 

CPP_SRCS += \
../Src/Src_STM32F1xx/main.cpp 

OBJS += \
./Src/Src_STM32F1xx/main.o \
./Src/Src_STM32F1xx/stm32f1xx_hal_msp.o \
./Src/Src_STM32F1xx/stm32f1xx_hal_timebase_tim.o \
./Src/Src_STM32F1xx/stm32f1xx_it.o \
./Src/Src_STM32F1xx/system_stm32f1xx.o 

C_DEPS += \
./Src/Src_STM32F1xx/stm32f1xx_hal_msp.d \
./Src/Src_STM32F1xx/stm32f1xx_hal_timebase_tim.d \
./Src/Src_STM32F1xx/stm32f1xx_it.d \
./Src/Src_STM32F1xx/system_stm32f1xx.d 

CPP_DEPS += \
./Src/Src_STM32F1xx/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Src_STM32F1xx/%.o: ../Src/Src_STM32F1xx/%.cpp Src/Src_STM32F1xx/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/Src_STM32F1xx/%.o: ../Src/Src_STM32F1xx/%.c Src/Src_STM32F1xx/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


