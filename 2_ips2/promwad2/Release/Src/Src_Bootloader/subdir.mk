################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/Src_Bootloader/stm32f7xx_hal_msp.c \
../Src/Src_Bootloader/stm32f7xx_hal_timebase_tim.c \
../Src/Src_Bootloader/stm32f7xx_it.c \
../Src/Src_Bootloader/system_stm32f7xx.c 

CPP_SRCS += \
../Src/Src_Bootloader/flash_diskio.cpp \
../Src/Src_Bootloader/main.cpp 

OBJS += \
./Src/Src_Bootloader/flash_diskio.o \
./Src/Src_Bootloader/main.o \
./Src/Src_Bootloader/stm32f7xx_hal_msp.o \
./Src/Src_Bootloader/stm32f7xx_hal_timebase_tim.o \
./Src/Src_Bootloader/stm32f7xx_it.o \
./Src/Src_Bootloader/system_stm32f7xx.o 

C_DEPS += \
./Src/Src_Bootloader/stm32f7xx_hal_msp.d \
./Src/Src_Bootloader/stm32f7xx_hal_timebase_tim.d \
./Src/Src_Bootloader/stm32f7xx_it.d \
./Src/Src_Bootloader/system_stm32f7xx.d 

CPP_DEPS += \
./Src/Src_Bootloader/flash_diskio.d \
./Src/Src_Bootloader/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/Src_Bootloader/%.o: ../Src/Src_Bootloader/%.cpp Src/Src_Bootloader/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/Src_Bootloader/%.o: ../Src/Src_Bootloader/%.c Src/Src_Bootloader/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


