################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/LwIP/test/unit/core/test_def.c \
../Middlewares/Third_Party/LwIP/test/unit/core/test_mem.c \
../Middlewares/Third_Party/LwIP/test/unit/core/test_netif.c \
../Middlewares/Third_Party/LwIP/test/unit/core/test_pbuf.c \
../Middlewares/Third_Party/LwIP/test/unit/core/test_timers.c 

OBJS += \
./Middlewares/Third_Party/LwIP/test/unit/core/test_def.o \
./Middlewares/Third_Party/LwIP/test/unit/core/test_mem.o \
./Middlewares/Third_Party/LwIP/test/unit/core/test_netif.o \
./Middlewares/Third_Party/LwIP/test/unit/core/test_pbuf.o \
./Middlewares/Third_Party/LwIP/test/unit/core/test_timers.o 

C_DEPS += \
./Middlewares/Third_Party/LwIP/test/unit/core/test_def.d \
./Middlewares/Third_Party/LwIP/test/unit/core/test_mem.d \
./Middlewares/Third_Party/LwIP/test/unit/core/test_netif.d \
./Middlewares/Third_Party/LwIP/test/unit/core/test_pbuf.d \
./Middlewares/Third_Party/LwIP/test/unit/core/test_timers.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/LwIP/test/unit/core/%.o: ../Middlewares/Third_Party/LwIP/test/unit/core/%.c Middlewares/Third_Party/LwIP/test/unit/core/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


