################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/abs_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/add_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_common_data.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/basic_math_test_group.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/dot_prod_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/mult_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/negate_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/offset_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/scale_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/shift_tests.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/sub_tests.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/%.c Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/src/basic_math_tests/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


