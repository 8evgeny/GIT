################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/test/unit/ip4/test_ip4.c 

OBJS += \
./Middlewares/Third_Party/LwIP/test/unit/ip4/test_ip4.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/test/unit/ip4/test_ip4.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/test/unit/ip4/%.o: ../Middlewares/Third_Party/LwIP/test/unit/ip4/%.c Middlewares/Third_Party/LwIP/test/unit/ip4/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


