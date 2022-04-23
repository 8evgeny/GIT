################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/audio_play.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/audio_record.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/lcd.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/main.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/mmc.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/sdram.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/stm32h7xx_it.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/touchscreen.c 

OBJS += \
./Example/User/audio_play.o \
./Example/User/audio_record.o \
./Example/User/lcd.o \
./Example/User/main.o \
./Example/User/mmc.o \
./Example/User/sdram.o \
./Example/User/stm32h7xx_it.o \
./Example/User/touchscreen.o 

C_DEPS += \
./Example/User/audio_play.d \
./Example/User/audio_record.d \
./Example/User/lcd.d \
./Example/User/main.d \
./Example/User/mmc.d \
./Example/User/sdram.d \
./Example/User/stm32h7xx_it.d \
./Example/User/touchscreen.d 


# Each subdirectory must supply rules for building sources it contributes
Example/User/audio_play.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/audio_play.c Example/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Example/User/audio_record.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/audio_record.c Example/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Example/User/lcd.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/lcd.c Example/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Example/User/main.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/main.c Example/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Example/User/mmc.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/mmc.c Example/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Example/User/sdram.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/sdram.c Example/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Example/User/stm32h7xx_it.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/stm32h7xx_it.c Example/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Example/User/touchscreen.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Src/touchscreen.c Example/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Example-2f-User

clean-Example-2f-User:
	-$(RM) ./Example/User/audio_play.d ./Example/User/audio_play.o ./Example/User/audio_record.d ./Example/User/audio_record.o ./Example/User/lcd.d ./Example/User/lcd.o ./Example/User/main.d ./Example/User/main.o ./Example/User/mmc.d ./Example/User/mmc.o ./Example/User/sdram.d ./Example/User/sdram.o ./Example/User/stm32h7xx_it.d ./Example/User/stm32h7xx_it.o ./Example/User/touchscreen.d ./Example/User/touchscreen.o

.PHONY: clean-Example-2f-User

