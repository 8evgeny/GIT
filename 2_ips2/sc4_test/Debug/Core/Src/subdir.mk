################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/CLI_CmdDeviceFn.c \
../Core/Src/CLI_CmdProcess.c \
../Core/Src/CLI_CmdServiceFn.c \
../Core/Src/CLI_CmdTable.c \
../Core/Src/CLI_CmdTestFn.c \
../Core/Src/CLI_io.c \
../Core/Src/CRC32.c \
../Core/Src/MemTest.c \
../Core/Src/cbuffer.c \
../Core/Src/driver_AIC.c \
../Core/Src/driver_EEPROM.c \
../Core/Src/driver_UI.c \
../Core/Src/driver_pinIO.c \
../Core/Src/i2c_SW.c \
../Core/Src/main.c \
../Core/Src/stm32h7xx_hal_msp.c \
../Core/Src/stm32h7xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32h7xx.c 

OBJS += \
./Core/Src/CLI_CmdDeviceFn.o \
./Core/Src/CLI_CmdProcess.o \
./Core/Src/CLI_CmdServiceFn.o \
./Core/Src/CLI_CmdTable.o \
./Core/Src/CLI_CmdTestFn.o \
./Core/Src/CLI_io.o \
./Core/Src/CRC32.o \
./Core/Src/MemTest.o \
./Core/Src/cbuffer.o \
./Core/Src/driver_AIC.o \
./Core/Src/driver_EEPROM.o \
./Core/Src/driver_UI.o \
./Core/Src/driver_pinIO.o \
./Core/Src/i2c_SW.o \
./Core/Src/main.o \
./Core/Src/stm32h7xx_hal_msp.o \
./Core/Src/stm32h7xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32h7xx.o 

C_DEPS += \
./Core/Src/CLI_CmdDeviceFn.d \
./Core/Src/CLI_CmdProcess.d \
./Core/Src/CLI_CmdServiceFn.d \
./Core/Src/CLI_CmdTable.d \
./Core/Src/CLI_CmdTestFn.d \
./Core/Src/CLI_io.d \
./Core/Src/CRC32.d \
./Core/Src/MemTest.d \
./Core/Src/cbuffer.d \
./Core/Src/driver_AIC.d \
./Core/Src/driver_EEPROM.d \
./Core/Src/driver_UI.d \
./Core/Src/driver_pinIO.d \
./Core/Src/i2c_SW.d \
./Core/Src/main.d \
./Core/Src/stm32h7xx_hal_msp.d \
./Core/Src/stm32h7xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32h7xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H753xx -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

