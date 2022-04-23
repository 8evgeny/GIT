################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_audio.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_bus.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_lcd.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_mmc.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_sdram.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_ts.c 

OBJS += \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery.o \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_audio.o \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_bus.o \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_lcd.o \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_mmc.o \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_sdram.o \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_ts.o 

C_DEPS += \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery.d \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_audio.d \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_bus.d \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_lcd.d \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_mmc.d \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_sdram.d \
./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery.c Drivers/BSP/STM32H750B_DISCOVERY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_audio.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_audio.c Drivers/BSP/STM32H750B_DISCOVERY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_bus.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_bus.c Drivers/BSP/STM32H750B_DISCOVERY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_lcd.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_lcd.c Drivers/BSP/STM32H750B_DISCOVERY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_mmc.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_mmc.c Drivers/BSP/STM32H750B_DISCOVERY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_sdram.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_sdram.c Drivers/BSP/STM32H750B_DISCOVERY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_ts.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/STM32H750B-DK/stm32h750b_discovery_ts.c Drivers/BSP/STM32H750B_DISCOVERY/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32H750B_DISCOVERY

clean-Drivers-2f-BSP-2f-STM32H750B_DISCOVERY:
	-$(RM) ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery.d ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery.o ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_audio.d ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_audio.o ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_bus.d ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_bus.o ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_lcd.d ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_lcd.o ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_mmc.d ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_mmc.o ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_sdram.d ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_sdram.o ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_ts.d ./Drivers/BSP/STM32H750B_DISCOVERY/stm32h750b_discovery_ts.o

.PHONY: clean-Drivers-2f-BSP-2f-STM32H750B_DISCOVERY

