################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/pid.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/sin_cos.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/pid.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/sin_cos.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/pid.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/sin_cos.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/%.c Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/ControllerFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


