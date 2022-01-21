################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/RTP/rtp.cpp 

OBJS += \
./Src/RTP/rtp.o 

CPP_DEPS += \
./Src/RTP/rtp.d 


# Each subdirectory must supply rules for building sources it contributes
Src/RTP/%.o: ../Src/RTP/%.cpp Src/RTP/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


