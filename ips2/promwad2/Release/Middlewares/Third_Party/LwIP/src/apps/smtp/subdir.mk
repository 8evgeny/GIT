################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/apps/smtp/smtp.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/apps/smtp/smtp.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/apps/smtp/smtp.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/apps/smtp/%.o: ../Middlewares/Third_Party/LwIP/src/apps/smtp/%.c Middlewares/Third_Party/LwIP/src/apps/smtp/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


