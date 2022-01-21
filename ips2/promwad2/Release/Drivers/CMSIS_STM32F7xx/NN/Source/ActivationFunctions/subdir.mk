################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_nn_activations_q15.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_nn_activations_q7.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_relu_q15.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_relu_q7.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_nn_activations_q15.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_nn_activations_q7.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_relu_q15.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_relu_q7.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_nn_activations_q15.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_nn_activations_q7.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_relu_q15.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/arm_relu_q7.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/%.o: ../Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/%.c Drivers/CMSIS_STM32F7xx/NN/Source/ActivationFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


