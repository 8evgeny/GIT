################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/11/Middlewares/Third_Party/SQLite/sqlite_os_freertos.c 

OBJS += \
./Middlewares/SQLite/sqlite_os_freertos.o 

C_DEPS += \
./Middlewares/SQLite/sqlite_os_freertos.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/SQLite/sqlite_os_freertos.o: /home/evg/SOFT/Github/GIT/6_promwad2_step_by_step/Simple_examples_CUBE/11/Middlewares/Third_Party/SQLite/sqlite_os_freertos.c Middlewares/SQLite/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32H753xx -DUSE_IOEXPANDER -D_HAVE_SQLITE_CONFIG_H -c -I../../../Inc -I../../../Inc/common/include -I../../../Middlewares/Third_Party/LwIP/src/include -I../../../Middlewares/Third_Party/LwIP/system -I../../../Middlewares/Third_Party/mbedTLS/include -I../../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../../Middlewares/Third_Party/FatFs/src -I../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../../Drivers/BSP/STM32H743I_EVAL -I../../../Drivers/BSP/Components/lan8742 -I../../../Drivers/BSP/Components/mfxstm32l152 -I../../../Drivers/BSP/Components/Common -I../../../Middlewares/Third_Party/SQLite -I../../../Middlewares/Third_Party/SQLite/src -I../../../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../../Middlewares/Third_Party/LwIP/src/include/lwip -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../../../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../../../Middlewares/Third_Party/LwIP/src/include/netif -I../../../Middlewares/Third_Party/LwIP/src/include/compat/posix -I../../../Middlewares/Third_Party/LwIP/src/include/compat/posix/sys -I../../../Middlewares/Third_Party/LwIP/src/include/compat/posix/arpa -I../../../Middlewares/Third_Party/LwIP/src/include/compat/posix/net -I../../../Middlewares/Third_Party/LwIP/src/include/compat/stdc -I../../../Middlewares/Third_Party/LwIP/system/arch -I../../../Middlewares/Third_Party/libwebsockets/include -I../../../Middlewares/Third_Party/libwebsockets/include/libwebsockets/abstract -I../../../Middlewares/Third_Party/libwebsockets/lib -I../../../Middlewares/Third_Party/libwebsockets/lib/core -I../../../Middlewares/Third_Party/libwebsockets/lib/core-net -I../../../Middlewares/Third_Party/libwebsockets/lib/event-libs -I../../../Middlewares/Third_Party/libwebsockets/lib/event-libs/libuv -I../../../Middlewares/Third_Party/libwebsockets/lib/event-libs/poll -I../../../Middlewares/Third_Party/libwebsockets/lib/event-libs/libevent -I../../../Middlewares/Third_Party/libwebsockets/lib/event-libs/glib -I../../../Middlewares/Third_Party/libwebsockets/lib/event-libs/libev -I../../../Middlewares/Third_Party/libwebsockets/lib/jose -I../../../Middlewares/Third_Party/libwebsockets/lib/jose/jwe -I../../../Middlewares/Third_Party/libwebsockets/lib/jose/jws -I../../../Middlewares/Third_Party/libwebsockets/lib/misc -I../../../Middlewares/Third_Party/libwebsockets/lib/roles -I../../../Middlewares/Third_Party/libwebsockets/lib/roles/http -I../../../Middlewares/Third_Party/libwebsockets/lib/roles/http/compression -I../../../Middlewares/Third_Party/libwebsockets/lib/roles/h1 -I../../../Middlewares/Third_Party/libwebsockets/lib/roles/h2 -I../../../Middlewares/Third_Party/libwebsockets/lib/roles/ws -I../../../Middlewares/Third_Party/libwebsockets/lib/roles/cgi -I../../../Middlewares/Third_Party/libwebsockets/lib/roles/dbus -I../../../Middlewares/Third_Party/libwebsockets/lib/roles/raw-proxy -I../../../Middlewares/Third_Party/libwebsockets/lib/abstract -I../../../Middlewares/Third_Party/libwebsockets/lib/system/async-dns -I../../../Middlewares/Third_Party/libwebsockets/lib/plat/freertos -I../../../Middlewares/Third_Party/libwebsockets/lib/tls -I../../../Middlewares/Third_Party/libwebsockets/lib/tls/mbedtls/wrapper/include -I../../../Middlewares/Third_Party/libwebsockets/lib/tls/mbedtls/wrapper/include/platform -I../../../Middlewares/Third_Party/libwebsockets/lib/tls/mbedtls/wrapper/include/internal -I../../../Middlewares/Third_Party/libwebsockets/lib/tls/mbedtls/wrapper/include/openssl -I../../../Middlewares/Third_Party/mbedTLS/include/mbedtls -I../../../Drivers/CMSIS/Include -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-SQLite

clean-Middlewares-2f-SQLite:
	-$(RM) ./Middlewares/SQLite/sqlite_os_freertos.d ./Middlewares/SQLite/sqlite_os_freertos.o

.PHONY: clean-Middlewares-2f-SQLite

