################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/EEPROM/eeprom.c 

CPP_SRCS += \
../Src/EEPROM/fsforeeprom.cpp \
../Src/EEPROM/i2ceeprom.cpp 

OBJS += \
./Src/EEPROM/eeprom.o \
./Src/EEPROM/fsforeeprom.o \
./Src/EEPROM/i2ceeprom.o 

C_DEPS += \
./Src/EEPROM/eeprom.d 

CPP_DEPS += \
./Src/EEPROM/fsforeeprom.d \
./Src/EEPROM/i2ceeprom.d 


# Each subdirectory must supply rules for building sources it contributes
Src/EEPROM/%.o: ../Src/EEPROM/%.c Src/EEPROM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/EEPROM/%.o: ../Src/EEPROM/%.cpp Src/EEPROM/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


