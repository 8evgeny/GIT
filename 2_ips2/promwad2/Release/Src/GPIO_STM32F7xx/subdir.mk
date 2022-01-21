################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/GPIO_STM32F7xx/gpio_stm32f7xx.cpp 

OBJS += \
./Src/GPIO_STM32F7xx/gpio_stm32f7xx.o 

CPP_DEPS += \
./Src/GPIO_STM32F7xx/gpio_stm32f7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Src/GPIO_STM32F7xx/%.o: ../Src/GPIO_STM32F7xx/%.cpp Src/GPIO_STM32F7xx/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


