################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/test/unit/arch/sys_arch.c 

OBJS += \
./Middlewares/Third_Party/LwIP/test/unit/arch/sys_arch.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/test/unit/arch/sys_arch.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/test/unit/arch/%.o: ../Middlewares/Third_Party/LwIP/test/unit/arch/%.c Middlewares/Third_Party/LwIP/test/unit/arch/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


