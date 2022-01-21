################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Software_configurator/configurator/Program/database.cpp \
../Software_configurator/configurator/Program/model.cpp 

OBJS += \
./Software_configurator/configurator/Program/database.o \
./Software_configurator/configurator/Program/model.o 

CPP_DEPS += \
./Software_configurator/configurator/Program/database.d \
./Software_configurator/configurator/Program/model.d 


# Each subdirectory must supply rules for building sources it contributes
Software_configurator/configurator/Program/%.o: ../Software_configurator/configurator/Program/%.cpp Software_configurator/configurator/Program/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


