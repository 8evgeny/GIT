################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/Call_control_for_SC2_board/call_control_sc2.cpp 

OBJS += \
./Src/Call_control_for_SC2_board/call_control_sc2.o 

CPP_DEPS += \
./Src/Call_control_for_SC2_board/call_control_sc2.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Call_control_for_SC2_board/%.o: ../Src/Call_control_for_SC2_board/%.cpp Src/Call_control_for_SC2_board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


