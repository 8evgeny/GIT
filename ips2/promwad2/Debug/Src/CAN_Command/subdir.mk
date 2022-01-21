################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/CAN_Command/command.cpp 

OBJS += \
./Src/CAN_Command/command.o 

CPP_DEPS += \
./Src/CAN_Command/command.d 


# Each subdirectory must supply rules for building sources it contributes
Src/CAN_Command/%.o: ../Src/CAN_Command/%.cpp Src/CAN_Command/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


