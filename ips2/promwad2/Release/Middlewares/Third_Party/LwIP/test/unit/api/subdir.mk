################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/test/unit/api/test_sockets.c 

OBJS += \
./Middlewares/Third_Party/LwIP/test/unit/api/test_sockets.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/test/unit/api/test_sockets.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/test/unit/api/%.o: ../Middlewares/Third_Party/LwIP/test/unit/api/%.c Middlewares/Third_Party/LwIP/test/unit/api/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


