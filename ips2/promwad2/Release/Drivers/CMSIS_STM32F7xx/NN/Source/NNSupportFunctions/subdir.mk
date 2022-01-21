################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nn_mult_q15.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nn_mult_q7.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nntables.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nn_mult_q15.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nn_mult_q7.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nntables.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nn_mult_q15.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nn_mult_q7.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_nntables.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_q7_to_q15_no_shift.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/arm_q7_to_q15_reordered_no_shift.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/%.o: ../Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/%.c Drivers/CMSIS_STM32F7xx/NN/Source/NNSupportFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


