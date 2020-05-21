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
test_eepameti.lss \

FLASH_IMAGE += \
test_eepameti.hex \

EEPROM_IMAGE += \
test_eepameti.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: test_eepameti.elf secondary-outputs

# Tool invocations
test_eepameti.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,test_eepameti.map -mmcu=atmega328p -o "test_eepameti.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

test_eepameti.lss: test_eepameti.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S test_eepameti.elf  >"test_eepameti.lss"
	@echo 'Finished building: $@'
	@echo ' '

test_eepameti.hex: test_eepameti.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex test_eepameti.elf  "test_eepameti.hex"
	@echo 'Finished building: $@'
	@echo ' '

test_eepameti.eep: test_eepameti.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex test_eepameti.elf  "test_eepameti.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: test_eepameti.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p test_eepameti.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) test_eepameti.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets
