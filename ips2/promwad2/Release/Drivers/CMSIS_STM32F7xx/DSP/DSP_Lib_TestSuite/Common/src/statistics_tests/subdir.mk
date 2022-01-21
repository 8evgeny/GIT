################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/max_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/mean_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/min_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/power_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/rms_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/statistics_test_common_data.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/statistics_test_group.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/std_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/var_tests.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/max_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/mean_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/min_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/power_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/rms_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/statistics_test_common_data.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/statistics_test_group.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/std_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/var_tests.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/max_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/mean_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/min_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/power_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/rms_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/statistics_test_common_data.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/statistics_test_group.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/std_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/var_tests.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/%.c Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/statistics_tests/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


