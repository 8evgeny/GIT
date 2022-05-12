################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Drivers/BSP/Components/lan8742/lan8742.c 

OBJS += \
./Drivers/BSP/Components/LAN8742/lan8742.o 

C_DEPS += \
./Drivers/BSP/Components/LAN8742/lan8742.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/LAN8742/lan8742.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Drivers/BSP/Components/lan8742/lan8742.c Drivers/BSP/Components/LAN8742/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DUSE_HAL_DRIVER -DUSE_STM32H735G_DISCO -DSTM32H735xx -c -I../../Src -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/LwIP/system -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/BSP/Components -I../../Middlewares/Third_Party/FreeRTOS/Source -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/BSP/STM32H735G-DK -I../../Utilities/lcd -I../../Inc -I../../Utilities/Lcd_Trace -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-Components-2f-LAN8742

clean-Drivers-2f-BSP-2f-Components-2f-LAN8742:
	-$(RM) ./Drivers/BSP/Components/LAN8742/lan8742.d ./Drivers/BSP/Components/LAN8742/lan8742.o

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-LAN8742

