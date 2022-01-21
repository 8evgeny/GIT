################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/TRNG/trng.cpp 

OBJS += \
./Src/TRNG/trng.o 

CPP_DEPS += \
./Src/TRNG/trng.d 


# Each subdirectory must supply rules for building sources it contributes
Src/TRNG/%.o: ../Src/TRNG/%.cpp Src/TRNG/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


