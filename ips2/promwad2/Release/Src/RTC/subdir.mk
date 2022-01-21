################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/RTC/rtc.cpp 

OBJS += \
./Src/RTC/rtc.o 

CPP_DEPS += \
./Src/RTC/rtc.d 


# Each subdirectory must supply rules for building sources it contributes
Src/RTC/%.o: ../Src/RTC/%.cpp Src/RTC/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


