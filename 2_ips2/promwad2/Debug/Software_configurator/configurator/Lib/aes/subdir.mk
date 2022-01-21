################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Software_configurator/configurator/Lib/aes/aestest.cpp \
../Software_configurator/configurator/Lib/aes/qaesencryption.cpp 

OBJS += \
./Software_configurator/configurator/Lib/aes/aestest.o \
./Software_configurator/configurator/Lib/aes/qaesencryption.o 

CPP_DEPS += \
./Software_configurator/configurator/Lib/aes/aestest.d \
./Software_configurator/configurator/Lib/aes/qaesencryption.d 


# Each subdirectory must supply rules for building sources it contributes
Software_configurator/configurator/Lib/aes/%.o: ../Software_configurator/configurator/Lib/aes/%.cpp Software_configurator/configurator/Lib/aes/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


