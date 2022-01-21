################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/intrinsics_tests_common_data.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/%.c Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/intrinsics_tests/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


