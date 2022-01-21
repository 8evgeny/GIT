################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Software_configurator/configurator/Lib/groupsock/inet.c 

CPP_SRCS += \
../Software_configurator/configurator/Lib/groupsock/GroupEId.cpp \
../Software_configurator/configurator/Lib/groupsock/Groupsock.cpp \
../Software_configurator/configurator/Lib/groupsock/GroupsockHelper.cpp \
../Software_configurator/configurator/Lib/groupsock/IOHandlers.cpp \
../Software_configurator/configurator/Lib/groupsock/NetAddress.cpp \
../Software_configurator/configurator/Lib/groupsock/NetInterface.cpp 

OBJS += \
./Software_configurator/configurator/Lib/groupsock/GroupEId.o \
./Software_configurator/configurator/Lib/groupsock/Groupsock.o \
./Software_configurator/configurator/Lib/groupsock/GroupsockHelper.o \
./Software_configurator/configurator/Lib/groupsock/IOHandlers.o \
./Software_configurator/configurator/Lib/groupsock/NetAddress.o \
./Software_configurator/configurator/Lib/groupsock/NetInterface.o \
./Software_configurator/configurator/Lib/groupsock/inet.o 

C_DEPS += \
./Software_configurator/configurator/Lib/groupsock/inet.d 

CPP_DEPS += \
./Software_configurator/configurator/Lib/groupsock/GroupEId.d \
./Software_configurator/configurator/Lib/groupsock/Groupsock.d \
./Software_configurator/configurator/Lib/groupsock/GroupsockHelper.d \
./Software_configurator/configurator/Lib/groupsock/IOHandlers.d \
./Software_configurator/configurator/Lib/groupsock/NetAddress.d \
./Software_configurator/configurator/Lib/groupsock/NetInterface.d 


# Each subdirectory must supply rules for building sources it contributes
Software_configurator/configurator/Lib/groupsock/%.o: ../Software_configurator/configurator/Lib/groupsock/%.cpp Software_configurator/configurator/Lib/groupsock/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Software_configurator/configurator/Lib/groupsock/%.o: ../Software_configurator/configurator/Lib/groupsock/%.c Software_configurator/configurator/Lib/groupsock/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


