################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/uid/uid.cpp 

OBJS += \
./Src/uid/uid.o 

CPP_DEPS += \
./Src/uid/uid.d 


# Each subdirectory must supply rules for building sources it contributes
Src/uid/%.o: ../Src/uid/%.cpp Src/uid/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


