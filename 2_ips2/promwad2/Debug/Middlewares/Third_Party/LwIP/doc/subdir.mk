################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/doc/NO_SYS_SampleCode.c \
../Middlewares/Third_Party/LwIP/doc/ZeroCopyRx.c 

OBJS += \
./Middlewares/Third_Party/LwIP/doc/NO_SYS_SampleCode.o \
./Middlewares/Third_Party/LwIP/doc/ZeroCopyRx.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/doc/NO_SYS_SampleCode.d \
./Middlewares/Third_Party/LwIP/doc/ZeroCopyRx.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/doc/%.o: ../Middlewares/Third_Party/LwIP/doc/%.c Middlewares/Third_Party/LwIP/doc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


