################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include source/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Wetter.lss \

FLASH_IMAGE += \
Wetter.hex \

EEPROM_IMAGE += \
Wetter.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: Wetter.elf secondary-outputs

# Tool invocations
Wetter.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Wetter.map -mmcu=atmega32 -o "Wetter.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Wetter.lss: Wetter.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Wetter.elf  >"Wetter.lss"
	@echo 'Finished building: $@'
	@echo ' '

Wetter.hex: Wetter.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex Wetter.elf  "Wetter.hex"
	@echo 'Finished building: $@'
	@echo ' '

Wetter.eep: Wetter.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex Wetter.elf  "Wetter.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Wetter.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t Wetter.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) Wetter.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
