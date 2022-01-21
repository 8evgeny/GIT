################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Debug/debug_printf.c 

CPP_SRCS += \
../Src/Debug/debug.cpp 

OBJS += \
./Src/Debug/debug.o \
./Src/Debug/debug_printf.o 

C_DEPS += \
./Src/Debug/debug_printf.d 

CPP_DEPS += \
./Src/Debug/debug.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Debug/%.o: ../Src/Debug/%.cpp Src/Debug/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/Debug/%.o: ../Src/Debug/%.c Src/Debug/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


