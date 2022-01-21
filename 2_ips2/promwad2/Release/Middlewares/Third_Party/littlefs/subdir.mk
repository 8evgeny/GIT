################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/littlefs/lfs.c \
../Middlewares/Third_Party/littlefs/lfs_util.c 

OBJS += \
./Middlewares/Third_Party/littlefs/lfs.o \
./Middlewares/Third_Party/littlefs/lfs_util.o 

C_DEPS += \
./Middlewares/Third_Party/littlefs/lfs.d \
./Middlewares/Third_Party/littlefs/lfs_util.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/littlefs/%.o: ../Middlewares/Third_Party/littlefs/%.c Middlewares/Third_Party/littlefs/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


