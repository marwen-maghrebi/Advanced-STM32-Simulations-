################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include util/subdir.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include ECUAL/STEPPER/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Stepper_LAB48.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Stepper_LAB48.list \

OBJCOPY_BIN += \
Stepper_LAB48.bin \


# All Target
all: Stepper_LAB48.elf secondary-outputs

# Tool invocations
Stepper_LAB48.elf: $(OBJS) $(USER_OBJS) C:\Users\Khaled\ Magdy\Desktop\Stepper_LAB48\STM32L432KCUX_FLASH.ld
	arm-none-eabi-gcc -o "Stepper_LAB48.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Khaled Magdy\Desktop\Stepper_LAB48\STM32L432KCUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Stepper_LAB48.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Stepper_LAB48.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Stepper_LAB48.list"
	@echo 'Finished building: $@'
	@echo ' '

Stepper_LAB48.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Stepper_LAB48.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
