################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/cifar10/arm_nnexamples_cifar10.cpp 

OBJS += \
./Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/cifar10/arm_nnexamples_cifar10.o 

CPP_DEPS += \
./Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/cifar10/arm_nnexamples_cifar10.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/cifar10/%.o: ../Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/cifar10/%.cpp Drivers/CMSIS_STM32F7xx/NN/Examples/ARM/arm_nn_examples/cifar10/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


