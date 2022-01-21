################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Drivers/CMSIS_STM32F7xx/NN/NN_Lib_Tests/nn_test/arm_nnexamples_nn_test.cpp 

OBJS += \
./Drivers/CMSIS_STM32F7xx/NN/NN_Lib_Tests/nn_test/arm_nnexamples_nn_test.o 

CPP_DEPS += \
./Drivers/CMSIS_STM32F7xx/NN/NN_Lib_Tests/nn_test/arm_nnexamples_nn_test.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/NN/NN_Lib_Tests/nn_test/%.o: ../Drivers/CMSIS_STM32F7xx/NN/NN_Lib_Tests/nn_test/%.cpp Drivers/CMSIS_STM32F7xx/NN/NN_Lib_Tests/nn_test/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


