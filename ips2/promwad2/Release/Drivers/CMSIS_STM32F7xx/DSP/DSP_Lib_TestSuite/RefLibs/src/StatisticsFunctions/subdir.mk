################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/max.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/mean.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/min.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/power.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/rms.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/std.c \
../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/var.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/max.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/mean.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/min.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/power.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/rms.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/std.o \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/var.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/max.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/mean.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/min.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/power.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/rms.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/std.d \
./Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/var.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/%.c Drivers/CMSIS_STM32F7xx/DSP/DSP_Lib_TestSuite/RefLibs/src/StatisticsFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


