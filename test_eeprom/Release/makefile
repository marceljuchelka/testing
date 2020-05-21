################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
test_eeprom.lss \

FLASH_IMAGE += \
test_eeprom.hex \

EEPROM_IMAGE += \
test_eeprom.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: test_eeprom.elf secondary-outputs

# Tool invocations
test_eeprom.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,test_eeprom.map -mmcu=atmega328p -o "test_eeprom.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

test_eeprom.lss: test_eeprom.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S test_eeprom.elf  >"test_eeprom.lss"
	@echo 'Finished building: $@'
	@echo ' '

test_eeprom.hex: test_eeprom.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex test_eeprom.elf  "test_eeprom.hex"
	@echo 'Finished building: $@'
	@echo ' '

test_eeprom.eep: test_eeprom.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex test_eeprom.elf  "test_eeprom.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: test_eeprom.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p test_eeprom.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) test_eeprom.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
