################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32N6xx_HAL_Driver/subdir.mk
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


BUILD_ARTIFACT_NAME := UART_AppliNonSecure
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
UART_AppliNonSecure.elf \

MAP_FILES += \
UART_AppliNonSecure.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
UART_AppliNonSecure.list \

OBJCOPY_BIN += \
UART_AppliNonSecure.bin \


# All Target
all: main-build

# Main-build Target
main-build: UART_AppliNonSecure.elf secondary-outputs

dependents:
	-cd /home/benjaminavocat-maulaz/Documents/Programs_STM32/N657/UART/AppliSecure/Debug && $(MAKE) clean all

# Tool invocations
UART_AppliNonSecure.elf UART_AppliNonSecure.map: $(OBJS) $(USER_OBJS) /home/benjaminavocat-maulaz/Documents/Programs_STM32/N657/UART/AppliSecure/Debug/UART_AppliSecure.elf /home/benjaminavocat-maulaz/Documents/Programs_STM32/N657/UART/AppliNonSecure/STM32N657X0HXQ_LRUN.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "UART_AppliNonSecure.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m55 -T"/home/benjaminavocat-maulaz/Documents/Programs_STM32/N657/UART/AppliNonSecure/STM32N657X0HXQ_LRUN.ld" --specs=nosys.specs -Wl,-Map="UART_AppliNonSecure.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

UART_AppliNonSecure.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "UART_AppliNonSecure.list"
	@echo 'Finished building: $@'
	@echo ' '

UART_AppliNonSecure.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "UART_AppliNonSecure.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) UART_AppliNonSecure.bin UART_AppliNonSecure.elf UART_AppliNonSecure.list UART_AppliNonSecure.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified
/home/benjaminavocat-maulaz/Documents/Programs_STM32/N657/UART/AppliSecure/Debug/UART_AppliSecure.elf:

-include ../makefile.targets
