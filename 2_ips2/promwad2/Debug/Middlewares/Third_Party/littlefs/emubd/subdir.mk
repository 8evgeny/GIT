################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/littlefs/emubd/lfs_emubd.c 

OBJS += \
./Middlewares/Third_Party/littlefs/emubd/lfs_emubd.o 

C_DEPS += \
./Middlewares/Third_Party/littlefs/emubd/lfs_emubd.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/littlefs/emubd/%.o: ../Middlewares/Third_Party/littlefs/emubd/%.c Middlewares/Third_Party/littlefs/emubd/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


