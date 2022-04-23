################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/SW4STM32/startup_stm32h750xx.s 

C_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/SW4STM32/syscalls.c 

OBJS += \
./Example/SW4STM32/startup_stm32h750xx.o \
./Example/SW4STM32/syscalls.o 

S_DEPS += \
./Example/SW4STM32/startup_stm32h750xx.d 

C_DEPS += \
./Example/SW4STM32/syscalls.d 


# Each subdirectory must supply rules for building sources it contributes
Example/SW4STM32/startup_stm32h750xx.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/SW4STM32/startup_stm32h750xx.s Example/SW4STM32/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Example/SW4STM32/syscalls.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/SW4STM32/syscalls.c Example/SW4STM32/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Example-2f-SW4STM32

clean-Example-2f-SW4STM32:
	-$(RM) ./Example/SW4STM32/startup_stm32h750xx.d ./Example/SW4STM32/startup_stm32h750xx.o ./Example/SW4STM32/syscalls.d ./Example/SW4STM32/syscalls.o

.PHONY: clean-Example-2f-SW4STM32

