################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Software_configurator/configurator/Lib/UsageEnvironment/HashTable.cpp \
../Software_configurator/configurator/Lib/UsageEnvironment/UsageEnvironment.cpp \
../Software_configurator/configurator/Lib/UsageEnvironment/strDup.cpp 

OBJS += \
./Software_configurator/configurator/Lib/UsageEnvironment/HashTable.o \
./Software_configurator/configurator/Lib/UsageEnvironment/UsageEnvironment.o \
./Software_configurator/configurator/Lib/UsageEnvironment/strDup.o 

CPP_DEPS += \
./Software_configurator/configurator/Lib/UsageEnvironment/HashTable.d \
./Software_configurator/configurator/Lib/UsageEnvironment/UsageEnvironment.d \
./Software_configurator/configurator/Lib/UsageEnvironment/strDup.d 


# Each subdirectory must supply rules for building sources it contributes
Software_configurator/configurator/Lib/UsageEnvironment/%.o: ../Software_configurator/configurator/Lib/UsageEnvironment/%.cpp Software_configurator/configurator/Lib/UsageEnvironment/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


