################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.c \
../Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.o \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_mat_q7_vec_q15_opt.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q15_opt.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7.d \
./Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/arm_fully_connected_q7_opt.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/%.o: ../Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/%.c Drivers/CMSIS_STM32F7xx/NN/Source/FullyConnectedFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


