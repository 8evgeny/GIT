################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Software_configurator/configurator/appcore.cpp \
../Software_configurator/configurator/assignment_list.cpp \
../Software_configurator/configurator/audioplayer.cpp \
../Software_configurator/configurator/audiorecoder.cpp \
../Software_configurator/configurator/firmware.cpp \
../Software_configurator/configurator/groups.cpp \
../Software_configurator/configurator/main.cpp \
../Software_configurator/configurator/network.cpp \
../Software_configurator/configurator/programmer.cpp \
../Software_configurator/configurator/receiver.cpp \
../Software_configurator/configurator/rtpsession.cpp \
../Software_configurator/configurator/subscriber.cpp \
../Software_configurator/configurator/terminal.cpp \
../Software_configurator/configurator/verification.cpp 

OBJS += \
./Software_configurator/configurator/appcore.o \
./Software_configurator/configurator/assignment_list.o \
./Software_configurator/configurator/audioplayer.o \
./Software_configurator/configurator/audiorecoder.o \
./Software_configurator/configurator/firmware.o \
./Software_configurator/configurator/groups.o \
./Software_configurator/configurator/main.o \
./Software_configurator/configurator/network.o \
./Software_configurator/configurator/programmer.o \
./Software_configurator/configurator/receiver.o \
./Software_configurator/configurator/rtpsession.o \
./Software_configurator/configurator/subscriber.o \
./Software_configurator/configurator/terminal.o \
./Software_configurator/configurator/verification.o 

CPP_DEPS += \
./Software_configurator/configurator/appcore.d \
./Software_configurator/configurator/assignment_list.d \
./Software_configurator/configurator/audioplayer.d \
./Software_configurator/configurator/audiorecoder.d \
./Software_configurator/configurator/firmware.d \
./Software_configurator/configurator/groups.d \
./Software_configurator/configurator/main.d \
./Software_configurator/configurator/network.d \
./Software_configurator/configurator/programmer.d \
./Software_configurator/configurator/receiver.d \
./Software_configurator/configurator/rtpsession.d \
./Software_configurator/configurator/subscriber.d \
./Software_configurator/configurator/terminal.d \
./Software_configurator/configurator/verification.d 


# Each subdirectory must supply rules for building sources it contributes
Software_configurator/configurator/%.o: ../Software_configurator/configurator/%.cpp Software_configurator/configurator/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


