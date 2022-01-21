################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/Bootloader/bootloader.cpp 

OBJS += \
./Src/Bootloader/bootloader.o 

CPP_DEPS += \
./Src/Bootloader/bootloader.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Bootloader/%.o: ../Src/Bootloader/%.cpp Src/Bootloader/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


