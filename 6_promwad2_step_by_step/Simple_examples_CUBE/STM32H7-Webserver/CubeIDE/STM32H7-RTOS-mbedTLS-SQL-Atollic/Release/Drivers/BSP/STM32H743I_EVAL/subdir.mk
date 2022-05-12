################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/11/Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/11/Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_io.c \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/11/Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_sd.c 

OBJS += \
./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval.o \
./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_io.o \
./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_sd.o 

C_DEPS += \
./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval.d \
./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_io.d \
./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_sd.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/11/Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval.c Drivers/BSP/STM32H743I_EVAL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 '-DMBEDTLS_CONFIG_FILE='"'"'"mbedtls_config.h"'"'"'' '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32H753xx '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' -c -I../../../Inc -I../../../Middlewares/Third_Party/LwIP/src/include -I../../../Middlewares/Third_Party/LwIP/system -I../../../Middlewares/Third_Party/mbedTLS/include -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/Third_Party/FatFs/src -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../../Middlewares/Third_Party/LwIP/src/include/netif -I../../../Middlewares/Third_Party/LwIP/src/include/posix -I../../../Middlewares/Third_Party/LwIP/src/include/posix/sys -I../../../Middlewares/Third_Party/LwIP/system/arch -I../../../Middlewares/Third_Party/mbedTLS/include/mbedtls -I../../../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_io.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/11/Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_io.c Drivers/BSP/STM32H743I_EVAL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 '-DMBEDTLS_CONFIG_FILE='"'"'"mbedtls_config.h"'"'"'' '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32H753xx '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' -c -I../../../Inc -I../../../Middlewares/Third_Party/LwIP/src/include -I../../../Middlewares/Third_Party/LwIP/system -I../../../Middlewares/Third_Party/mbedTLS/include -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/Third_Party/FatFs/src -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../../Middlewares/Third_Party/LwIP/src/include/netif -I../../../Middlewares/Third_Party/LwIP/src/include/posix -I../../../Middlewares/Third_Party/LwIP/src/include/posix/sys -I../../../Middlewares/Third_Party/LwIP/system/arch -I../../../Middlewares/Third_Party/mbedTLS/include/mbedtls -I../../../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_sd.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/11/Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_sd.c Drivers/BSP/STM32H743I_EVAL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 '-DMBEDTLS_CONFIG_FILE='"'"'"mbedtls_config.h"'"'"'' '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32H753xx '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' -c -I../../../Inc -I../../../Middlewares/Third_Party/LwIP/src/include -I../../../Middlewares/Third_Party/LwIP/system -I../../../Middlewares/Third_Party/mbedTLS/include -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/Third_Party/FatFs/src -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/BSP/Components/lan8742 -I../../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../../Middlewares/Third_Party/LwIP/src/include/netif -I../../../Middlewares/Third_Party/LwIP/src/include/posix -I../../../Middlewares/Third_Party/LwIP/src/include/posix/sys -I../../../Middlewares/Third_Party/LwIP/system/arch -I../../../Middlewares/Third_Party/mbedTLS/include/mbedtls -I../../../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32H743I_EVAL

clean-Drivers-2f-BSP-2f-STM32H743I_EVAL:
	-$(RM) ./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval.d ./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval.o ./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_io.d ./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_io.o ./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_sd.d ./Drivers/BSP/STM32H743I_EVAL/stm32h743i_eval_sd.o

.PHONY: clean-Drivers-2f-BSP-2f-STM32H743I_EVAL

