################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_f32.c \
../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_q15.c \
../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_q31.c \
../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_f32.c \
../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_q15.c \
../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_q31.c \
../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_sin_cos_f32.c \
../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_sin_cos_q31.c 

OBJS += \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_f32.o \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_q15.o \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_q31.o \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_f32.o \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_q15.o \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_q31.o \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_sin_cos_f32.o \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_sin_cos_q31.o 

C_DEPS += \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_f32.d \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_q15.d \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_init_q31.d \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_f32.d \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_q15.d \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_pid_reset_q31.d \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_sin_cos_f32.d \
./Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/arm_sin_cos_q31.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/%.o: ../Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/%.c Drivers/CMSIS_STM32F7xx/DSP/Source/ControllerFunctions/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


