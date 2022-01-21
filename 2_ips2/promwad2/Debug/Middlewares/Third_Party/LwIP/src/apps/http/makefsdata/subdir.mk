################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/src/apps/http/makefsdata/makefsdata.c 

OBJS += \
./Middlewares/Third_Party/LwIP/src/apps/http/makefsdata/makefsdata.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/src/apps/http/makefsdata/makefsdata.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/src/apps/http/makefsdata/%.o: ../Middlewares/Third_Party/LwIP/src/apps/http/makefsdata/%.c Middlewares/Third_Party/LwIP/src/apps/http/makefsdata/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


