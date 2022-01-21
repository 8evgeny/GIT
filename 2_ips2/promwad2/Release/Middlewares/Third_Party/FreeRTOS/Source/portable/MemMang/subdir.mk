################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_1.c \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_2.c \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_3.c \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.c \
../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_5.c 

OBJS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_1.o \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_2.o \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_3.o \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.o \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_5.o 

C_DEPS += \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_1.d \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_2.d \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_3.d \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_4.d \
./Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/heap_5.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.o: ../Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/%.c Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


