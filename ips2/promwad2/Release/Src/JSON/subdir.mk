################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/JSON/json.cpp 

OBJS += \
./Src/JSON/json.o 

CPP_DEPS += \
./Src/JSON/json.d 


# Each subdirectory must supply rules for building sources it contributes
Src/JSON/%.o: ../Src/JSON/%.cpp Src/JSON/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


