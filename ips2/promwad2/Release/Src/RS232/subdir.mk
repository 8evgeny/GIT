################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/RS232/rs232_printf.c 

CPP_SRCS += \
../Src/RS232/rs232.cpp 

OBJS += \
./Src/RS232/rs232.o \
./Src/RS232/rs232_printf.o 

C_DEPS += \
./Src/RS232/rs232_printf.d 

CPP_DEPS += \
./Src/RS232/rs232.d 


# Each subdirectory must supply rules for building sources it contributes
Src/RS232/%.o: ../Src/RS232/%.cpp Src/RS232/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/RS232/%.o: ../Src/RS232/%.c Src/RS232/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


