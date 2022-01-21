################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/WDT/wdt.cpp 

OBJS += \
./Src/WDT/wdt.o 

CPP_DEPS += \
./Src/WDT/wdt.d 


# Each subdirectory must supply rules for building sources it contributes
Src/WDT/%.o: ../Src/WDT/%.cpp Src/WDT/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


