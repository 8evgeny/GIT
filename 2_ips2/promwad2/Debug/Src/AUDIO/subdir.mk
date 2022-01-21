################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/AUDIO/crypt.cpp \
../Src/AUDIO/dtmf_generator.cpp \
../Src/AUDIO/i2c.cpp \
../Src/AUDIO/microphone.cpp \
../Src/AUDIO/sai.cpp \
../Src/AUDIO/tlv320aic3254.cpp 

OBJS += \
./Src/AUDIO/crypt.o \
./Src/AUDIO/dtmf_generator.o \
./Src/AUDIO/i2c.o \
./Src/AUDIO/microphone.o \
./Src/AUDIO/sai.o \
./Src/AUDIO/tlv320aic3254.o 

CPP_DEPS += \
./Src/AUDIO/crypt.d \
./Src/AUDIO/dtmf_generator.d \
./Src/AUDIO/i2c.d \
./Src/AUDIO/microphone.d \
./Src/AUDIO/sai.d \
./Src/AUDIO/tlv320aic3254.d 


# Each subdirectory must supply rules for building sources it contributes
Src/AUDIO/%.o: ../Src/AUDIO/%.cpp Src/AUDIO/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


