################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/CRC16_CCITT/crc16_ccitt.cpp 

OBJS += \
./Src/CRC16_CCITT/crc16_ccitt.o 

CPP_DEPS += \
./Src/CRC16_CCITT/crc16_ccitt.d 


# Each subdirectory must supply rules for building sources it contributes
Src/CRC16_CCITT/%.o: ../Src/CRC16_CCITT/%.cpp Src/CRC16_CCITT/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


