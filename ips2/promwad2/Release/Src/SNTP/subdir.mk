################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/SNTP/sntp_client.cpp 

OBJS += \
./Src/SNTP/sntp_client.o 

CPP_DEPS += \
./Src/SNTP/sntp_client.d 


# Each subdirectory must supply rules for building sources it contributes
Src/SNTP/%.o: ../Src/SNTP/%.cpp Src/SNTP/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


