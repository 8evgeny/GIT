################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/SRAM/sram.cpp 

OBJS += \
./Src/SRAM/sram.o 

CPP_DEPS += \
./Src/SRAM/sram.d 


# Each subdirectory must supply rules for building sources it contributes
Src/SRAM/%.o: ../Src/SRAM/%.cpp Src/SRAM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


