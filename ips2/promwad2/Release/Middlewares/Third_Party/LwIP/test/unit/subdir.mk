################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/test/unit/lwip_unittests.c 

OBJS += \
./Middlewares/Third_Party/LwIP/test/unit/lwip_unittests.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/test/unit/lwip_unittests.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/test/unit/%.o: ../Middlewares/Third_Party/LwIP/test/unit/%.c Middlewares/Third_Party/LwIP/test/unit/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


