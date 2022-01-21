################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/flash/flash.cpp 

OBJS += \
./Src/flash/flash.o 

CPP_DEPS += \
./Src/flash/flash.d 


# Each subdirectory must supply rules for building sources it contributes
Src/flash/%.o: ../Src/flash/%.cpp Src/flash/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


