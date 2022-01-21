################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/test/fuzz/fuzz.c 

OBJS += \
./Middlewares/Third_Party/LwIP/test/fuzz/fuzz.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/test/fuzz/fuzz.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/test/fuzz/%.o: ../Middlewares/Third_Party/LwIP/test/fuzz/%.c Middlewares/Third_Party/LwIP/test/fuzz/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


