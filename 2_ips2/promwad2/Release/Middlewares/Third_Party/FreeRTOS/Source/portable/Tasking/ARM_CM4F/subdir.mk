################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/port_asm.asm 

C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/port.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/port.o \
./Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/port_asm.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/port.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/%.c Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/%.asm Middlewares/Third_Party/FreeRTOS/Source/portable/Tasking/ARM_CM4F/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Assembler'
	as  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


