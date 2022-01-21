################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/test/unit/tcp/tcp_helper.c \
../Middlewares/Third_Party/LwIP/test/unit/tcp/test_tcp.c \
../Middlewares/Third_Party/LwIP/test/unit/tcp/test_tcp_oos.c 

OBJS += \
./Middlewares/Third_Party/LwIP/test/unit/tcp/tcp_helper.o \
./Middlewares/Third_Party/LwIP/test/unit/tcp/test_tcp.o \
./Middlewares/Third_Party/LwIP/test/unit/tcp/test_tcp_oos.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/test/unit/tcp/tcp_helper.d \
./Middlewares/Third_Party/LwIP/test/unit/tcp/test_tcp.d \
./Middlewares/Third_Party/LwIP/test/unit/tcp/test_tcp_oos.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/test/unit/tcp/%.o: ../Middlewares/Third_Party/LwIP/test/unit/tcp/%.c Middlewares/Third_Party/LwIP/test/unit/tcp/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


