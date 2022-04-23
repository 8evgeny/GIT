################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/ft5336/ft5336.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/ft5336/ft5336_reg.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/mt48lc4m32b2/mt48lc4m32b2.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/wm8994/wm8994.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/wm8994/wm8994_reg.c 

OBJS += \
./Drivers/BSP/Components/ft5336.o \
./Drivers/BSP/Components/ft5336_reg.o \
./Drivers/BSP/Components/mt48lc4m32b2.o \
./Drivers/BSP/Components/wm8994.o \
./Drivers/BSP/Components/wm8994_reg.o 

C_DEPS += \
./Drivers/BSP/Components/ft5336.d \
./Drivers/BSP/Components/ft5336_reg.d \
./Drivers/BSP/Components/mt48lc4m32b2.d \
./Drivers/BSP/Components/wm8994.d \
./Drivers/BSP/Components/wm8994_reg.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/ft5336.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/ft5336/ft5336.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/ft5336_reg.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/ft5336/ft5336_reg.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/mt48lc4m32b2.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/mt48lc4m32b2/mt48lc4m32b2.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/wm8994.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/wm8994/wm8994.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/Components/wm8994_reg.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/EXAMPLES_H753/BSP/BSP/Drivers/BSP/Components/wm8994/wm8994_reg.c Drivers/BSP/Components/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32H750xx -DUSE_STM32H750B_DISCO -DTS_MULTI_TOUCH_SUPPORTED -DAPPLICATION_ADDRESS=0x90000000 -c -I../../../Inc -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/ST/STM32_Audio/Addons/PDM/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/BSP/STM32H750B-DK -I../../../Drivers/BSP/Components/Common -I../../../Utilities/lcd -I../../../Utilities/Fonts -I../../../Utilities/CPU -I../../../Drivers/CMSIS/Include -O0 -ffunction-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components

clean-Drivers-2f-BSP-2f-Components:
	-$(RM) ./Drivers/BSP/Components/ft5336.d ./Drivers/BSP/Components/ft5336.o ./Drivers/BSP/Components/ft5336_reg.d ./Drivers/BSP/Components/ft5336_reg.o ./Drivers/BSP/Components/mt48lc4m32b2.d ./Drivers/BSP/Components/mt48lc4m32b2.o ./Drivers/BSP/Components/wm8994.d ./Drivers/BSP/Components/wm8994.o ./Drivers/BSP/Components/wm8994_reg.d ./Drivers/BSP/Components/wm8994_reg.o

.PHONY: clean-Drivers-2f-BSP-2f-Components

