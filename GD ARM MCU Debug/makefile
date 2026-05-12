################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include osal/subdir.mk
-include gcc_startup/subdir.mk
-include app/subdir.mk
-include Firmware/GD32F30x_standard_peripheral/Source/subdir.mk
-include Firmware/CMSIS/GD/GD32F30x/Source/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := source
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH_HEX += \
source.hex \

SECONDARY_FLASH_BIN += \
source.bin \

SECONDARY_LIST += \
source.lst \

SECONDARY_SIZE += \
source.siz \


# All Target
all: main-build

# Main-build Target
main-build: source.elf secondary-outputs

# Tool invocations
source.elf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: GD ARM MCU C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3  -T gd32f30x_flash.ld -Xlinker --gc-sections -L"../ldscripts/" -Wl,-Map,"source.map" --specs=nano.specs --specs=nosys.specs -o "source.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

source.hex: source.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GD ARM MCU Flash Image(Hex)'
	arm-none-eabi-objcopy -O ihex  "source.elf"  "source.hex"
	@echo 'Finished building: $@'
	@echo ' '

source.bin: source.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GD ARM MCU Flash Image(Bin)'
	arm-none-eabi-objcopy -O binary  "source.elf"  "source.bin"
	@echo 'Finished building: $@'
	@echo ' '

source.lst: source.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GD ARM MCU Listing'
	arm-none-eabi-objdump --source --all-headers --demangle --line-numbers --wide "source.elf" > "source.lst"
	@echo 'Finished building: $@'
	@echo ' '

source.siz: source.elf makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: GD ARM MCU Print Size'
	arm-none-eabi-size --format=berkeley "source.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) source.bin source.elf source.hex source.lst source.siz
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH_HEX) $(SECONDARY_FLASH_BIN) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents main-build

-include ../makefile.targets
