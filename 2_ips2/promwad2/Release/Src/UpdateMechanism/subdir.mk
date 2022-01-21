################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/UpdateMechanism/updatemechanism.cpp 

OBJS += \
./Src/UpdateMechanism/updatemechanism.o 

CPP_DEPS += \
./Src/UpdateMechanism/updatemechanism.d 


# Each subdirectory must supply rules for building sources it contributes
Src/UpdateMechanism/%.o: ../Src/UpdateMechanism/%.cpp Src/UpdateMechanism/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


