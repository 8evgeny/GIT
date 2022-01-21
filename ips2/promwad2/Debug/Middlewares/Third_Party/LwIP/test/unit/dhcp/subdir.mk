################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/test/unit/dhcp/test_dhcp.c 

OBJS += \
./Middlewares/Third_Party/LwIP/test/unit/dhcp/test_dhcp.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/test/unit/dhcp/test_dhcp.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/test/unit/dhcp/%.o: ../Middlewares/Third_Party/LwIP/test/unit/dhcp/%.c Middlewares/Third_Party/LwIP/test/unit/dhcp/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


