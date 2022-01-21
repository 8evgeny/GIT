################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/Call_Control/call_control.cpp \
../Src/Call_Control/callcontrol.cpp \
../Src/Call_Control/callwaiting.cpp \
../Src/Call_Control/circularcall.cpp \
../Src/Call_Control/conferencecall.cpp \
../Src/Call_Control/duplexdirectcall.cpp \
../Src/Call_Control/groupcall.cpp \
../Src/Call_Control/messagedata.cpp \
../Src/Call_Control/missedcall.cpp \
../Src/Call_Control/os_timers.cpp \
../Src/Call_Control/simplexdirectcall.cpp \
../Src/Call_Control/telephonecall.cpp 

OBJS += \
./Src/Call_Control/call_control.o \
./Src/Call_Control/callcontrol.o \
./Src/Call_Control/callwaiting.o \
./Src/Call_Control/circularcall.o \
./Src/Call_Control/conferencecall.o \
./Src/Call_Control/duplexdirectcall.o \
./Src/Call_Control/groupcall.o \
./Src/Call_Control/messagedata.o \
./Src/Call_Control/missedcall.o \
./Src/Call_Control/os_timers.o \
./Src/Call_Control/simplexdirectcall.o \
./Src/Call_Control/telephonecall.o 

CPP_DEPS += \
./Src/Call_Control/call_control.d \
./Src/Call_Control/callcontrol.d \
./Src/Call_Control/callwaiting.d \
./Src/Call_Control/circularcall.d \
./Src/Call_Control/conferencecall.d \
./Src/Call_Control/duplexdirectcall.d \
./Src/Call_Control/groupcall.d \
./Src/Call_Control/messagedata.d \
./Src/Call_Control/missedcall.d \
./Src/Call_Control/os_timers.d \
./Src/Call_Control/simplexdirectcall.d \
./Src/Call_Control/telephonecall.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Call_Control/%.o: ../Src/Call_Control/%.cpp Src/Call_Control/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


