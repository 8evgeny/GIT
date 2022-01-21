################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM0.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM23.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM3.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM33.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM4.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM7.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC000.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC300.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MBL.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MML.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_generic.c 

S_UPPER_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/startup_generic.S 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/startup_generic.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM0.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM23.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM3.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM33.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM4.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM7.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC000.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC300.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MBL.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MML.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_generic.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM0.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM23.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM3.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM33.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM4.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMCM7.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC000.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMSC300.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MBL.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_ARMv8MML.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/system_generic.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/%.S Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/%.c Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/Common/platform/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


