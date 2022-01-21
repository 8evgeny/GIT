################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/gru/arm_nnexamples_gru.cpp 

OBJS += \
./Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/gru/arm_nnexamples_gru.o 

CPP_DEPS += \
./Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/gru/arm_nnexamples_gru.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/gru/%.o: ../Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/gru/%.cpp Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/gru/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


