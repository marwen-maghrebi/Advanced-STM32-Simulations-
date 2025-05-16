################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F1xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := stm32_thermocouple_amplification_AD8495
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
stm32_thermocouple_amplification_AD8495.elf \

MAP_FILES += \
stm32_thermocouple_amplification_AD8495.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
stm32_thermocouple_amplification_AD8495.list \

OBJCOPY_HEX += \
stm32_thermocouple_amplification_AD8495.hex \


# All Target
all: main-build

# Main-build Target
main-build: stm32_thermocouple_amplification_AD8495.elf secondary-outputs

# Tool invocations
stm32_thermocouple_amplification_AD8495.elf stm32_thermocouple_amplification_AD8495.map: $(OBJS) $(USER_OBJS) D:\0-Youtube\stm32_thermocouple_amplification_AD8495\STM32F103C6TX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "stm32_thermocouple_amplification_AD8495.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\0-Youtube\stm32_thermocouple_amplification_AD8495\STM32F103C6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="stm32_thermocouple_amplification_AD8495.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

stm32_thermocouple_amplification_AD8495.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "stm32_thermocouple_amplification_AD8495.list"
	@echo 'Finished building: $@'
	@echo ' '

stm32_thermocouple_amplification_AD8495.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "stm32_thermocouple_amplification_AD8495.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout stm32_thermocouple_amplification_AD8495.elf stm32_thermocouple_amplification_AD8495.hex stm32_thermocouple_amplification_AD8495.list stm32_thermocouple_amplification_AD8495.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
