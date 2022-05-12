################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/mbedTLS/subdir.mk
-include Middlewares/libwebsockets/core-net/subdir.mk
-include Middlewares/libwebsockets/subdir.mk
-include Middlewares/SQLite/src/subdir.mk
-include Middlewares/SQLite/subdir.mk
-include Middlewares/LwIP/subdir.mk
-include Middlewares/FreeRTOS/subdir.mk
-include Middlewares/FatFs/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/subdir.mk
-include Drivers/CMSIS/subdir.mk
-include Drivers/BSP/STM32H743I_EVAL/subdir.mk
-include Drivers/BSP/Components/subdir.mk
-include Application/User/subdir.mk
-include Application/TrueSTUDIO/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := STM32H7-RTOS-mbedTLS-SQL-Atollic
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
STM32H7-RTOS-mbedTLS-SQL-Atollic.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
STM32H7-RTOS-mbedTLS-SQL-Atollic.list \

OBJCOPY_HEX += \
STM32H7-RTOS-mbedTLS-SQL-Atollic.hex \

OBJCOPY_BIN += \
STM32H7-RTOS-mbedTLS-SQL-Atollic.bin \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: STM32H7-RTOS-mbedTLS-SQL-Atollic.elf secondary-outputs

# Tool invocations
STM32H7-RTOS-mbedTLS-SQL-Atollic.elf: $(OBJS) $(USER_OBJS) ../STM32H753XIHx_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "STM32H7-RTOS-mbedTLS-SQL-Atollic.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"../STM32H753XIHx_FLASH.ld" -Wl,-Map="STM32H7-RTOS-mbedTLS-SQL-Atollic.map" -Wl,--gc-sections -static -Wl,--wrap=_malloc_r -Wl,--wrap=_free_r -Wl,--wrap=_realloc_r -Wl,--wrap=_calloc_r --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

STM32H7-RTOS-mbedTLS-SQL-Atollic.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "STM32H7-RTOS-mbedTLS-SQL-Atollic.list"
	@echo 'Finished building: $@'
	@echo ' '

STM32H7-RTOS-mbedTLS-SQL-Atollic.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "STM32H7-RTOS-mbedTLS-SQL-Atollic.hex"
	@echo 'Finished building: $@'
	@echo ' '

STM32H7-RTOS-mbedTLS-SQL-Atollic.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "STM32H7-RTOS-mbedTLS-SQL-Atollic.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) STM32H7-RTOS-mbedTLS-SQL-Atollic.bin STM32H7-RTOS-mbedTLS-SQL-Atollic.elf STM32H7-RTOS-mbedTLS-SQL-Atollic.hex STM32H7-RTOS-mbedTLS-SQL-Atollic.list default.size.stdout
	-@echo ' '

post-build:
	arm-none-eabi-objcopy.exe -O ihex "STM32H7-RTOS-mbedTLS-SQL-Atollic.elf" "STM32H7-RTOS-mbedTLS-SQL-Atollic.hex"
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified post-build

-include ../makefile.targets
