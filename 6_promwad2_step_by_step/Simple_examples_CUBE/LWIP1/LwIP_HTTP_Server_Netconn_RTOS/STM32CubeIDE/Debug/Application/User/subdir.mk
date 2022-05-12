################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/app_ethernet.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/ethernetif.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/httpserver_netconn.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/main.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/stm32h7xx_hal_timebase_tim.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/stm32h7xx_it.c \
../Application/User/syscalls.c \
../Application/User/sysmem.c 

OBJS += \
./Application/User/app_ethernet.o \
./Application/User/ethernetif.o \
./Application/User/httpserver_netconn.o \
./Application/User/main.o \
./Application/User/stm32h7xx_hal_timebase_tim.o \
./Application/User/stm32h7xx_it.o \
./Application/User/syscalls.o \
./Application/User/sysmem.o 

C_DEPS += \
./Application/User/app_ethernet.d \
./Application/User/ethernetif.d \
./Application/User/httpserver_netconn.d \
./Application/User/main.d \
./Application/User/stm32h7xx_hal_timebase_tim.d \
./Application/User/stm32h7xx_it.d \
./Application/User/syscalls.d \
./Application/User/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/app_ethernet.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/app_ethernet.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32H735G_DISCO -DDEBUG -DSTM32H735xx -c -I../../Src -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/LwIP/system -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/BSP/Components -I../../Middlewares/Third_Party/FreeRTOS/Source -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/BSP/STM32H735G-DK -I../../Utilities/lcd -I../../Inc -I../../Utilities/Lcd_Trace -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/ethernetif.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/ethernetif.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32H735G_DISCO -DDEBUG -DSTM32H735xx -c -I../../Src -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/LwIP/system -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/BSP/Components -I../../Middlewares/Third_Party/FreeRTOS/Source -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/BSP/STM32H735G-DK -I../../Utilities/lcd -I../../Inc -I../../Utilities/Lcd_Trace -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/httpserver_netconn.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/httpserver_netconn.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32H735G_DISCO -DDEBUG -DSTM32H735xx -c -I../../Src -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/LwIP/system -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/BSP/Components -I../../Middlewares/Third_Party/FreeRTOS/Source -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/BSP/STM32H735G-DK -I../../Utilities/lcd -I../../Inc -I../../Utilities/Lcd_Trace -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/main.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/main.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32H735G_DISCO -DDEBUG -DSTM32H735xx -c -I../../Src -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/LwIP/system -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/BSP/Components -I../../Middlewares/Third_Party/FreeRTOS/Source -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/BSP/STM32H735G-DK -I../../Utilities/lcd -I../../Inc -I../../Utilities/Lcd_Trace -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32h7xx_hal_timebase_tim.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/stm32h7xx_hal_timebase_tim.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32H735G_DISCO -DDEBUG -DSTM32H735xx -c -I../../Src -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/LwIP/system -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/BSP/Components -I../../Middlewares/Third_Party/FreeRTOS/Source -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/BSP/STM32H735G-DK -I../../Utilities/lcd -I../../Inc -I../../Utilities/Lcd_Trace -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/stm32h7xx_it.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/LWIP1/LwIP_HTTP_Server_Netconn_RTOS/Src/stm32h7xx_it.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32H735G_DISCO -DDEBUG -DSTM32H735xx -c -I../../Src -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/LwIP/system -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/BSP/Components -I../../Middlewares/Third_Party/FreeRTOS/Source -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/BSP/STM32H735G-DK -I../../Utilities/lcd -I../../Inc -I../../Utilities/Lcd_Trace -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/User/%.o: ../Application/User/%.c Application/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_STM32H735G_DISCO -DDEBUG -DSTM32H735xx -c -I../../Src -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/LwIP/system -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/BSP/Components -I../../Middlewares/Third_Party/FreeRTOS/Source -I../../Middlewares/Third_Party/LwIP/src/include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Drivers/BSP/STM32H735G-DK -I../../Utilities/lcd -I../../Inc -I../../Utilities/Lcd_Trace -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User

clean-Application-2f-User:
	-$(RM) ./Application/User/app_ethernet.d ./Application/User/app_ethernet.o ./Application/User/ethernetif.d ./Application/User/ethernetif.o ./Application/User/httpserver_netconn.d ./Application/User/httpserver_netconn.o ./Application/User/main.d ./Application/User/main.o ./Application/User/stm32h7xx_hal_timebase_tim.d ./Application/User/stm32h7xx_hal_timebase_tim.o ./Application/User/stm32h7xx_it.d ./Application/User/stm32h7xx_it.o ./Application/User/syscalls.d ./Application/User/syscalls.o ./Application/User/sysmem.d ./Application/User/sysmem.o

.PHONY: clean-Application-2f-User

