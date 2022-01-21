################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicHashTable.cpp \
../Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicTaskScheduler.cpp \
../Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicTaskScheduler0.cpp \
../Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicUsageEnvironment.cpp \
../Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicUsageEnvironment0.cpp \
../Software_configurator/configurator/Lib/BasicUsageEnvironment/DelayQueue.cpp 

OBJS += \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicHashTable.o \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicTaskScheduler.o \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicTaskScheduler0.o \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicUsageEnvironment.o \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicUsageEnvironment0.o \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/DelayQueue.o 

CPP_DEPS += \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicHashTable.d \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicTaskScheduler.d \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicTaskScheduler0.d \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicUsageEnvironment.d \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/BasicUsageEnvironment0.d \
./Software_configurator/configurator/Lib/BasicUsageEnvironment/DelayQueue.d 


# Each subdirectory must supply rules for building sources it contributes
Software_configurator/configurator/Lib/BasicUsageEnvironment/%.o: ../Software_configurator/configurator/Lib/BasicUsageEnvironment/%.cpp Software_configurator/configurator/Lib/BasicUsageEnvironment/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


