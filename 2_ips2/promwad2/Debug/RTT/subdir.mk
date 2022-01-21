################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../RTT/SEGGER_RTT.c 

OBJS += \
./RTT/SEGGER_RTT.o 

C_DEPS += \
./RTT/SEGGER_RTT.d 


# Each subdirectory must supply rules for building sources it contributes
RTT/%.o: ../RTT/%.c RTT/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


