################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Bootloader/AES128/aes.c 

OBJS += \
./Src/Bootloader/AES128/aes.o 

C_DEPS += \
./Src/Bootloader/AES128/aes.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Bootloader/AES128/%.o: ../Src/Bootloader/AES128/%.c Src/Bootloader/AES128/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


