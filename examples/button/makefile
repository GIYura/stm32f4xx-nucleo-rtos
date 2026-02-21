##############################################
#			Multi-Platform Build			#
##############################################

##############################################
# Pre-compiled lib for linker
##############################################
LIB=

##############################################
# Source directory
##############################################
SUBMODULES = uSDK
SRCDIR = src
BSP = bsp

##############################################
# APPLICATION
##############################################
APPLICATION = button-rtos

##############################################
# Target platform
##############################################
PLATFORM ?= 

SUPPORTED_PLATFORMS := stm32f4xx cc32xx nrf52
IGNORE_GOAL := clean help ocd gdb flash debug

ifeq ($(MAKECMDGOALS),)
    GOAL := all
else
    GOAL := $(MAKECMDGOALS)
endif

ifeq (,$(filter $(GOAL),$(IGNORE_GOAL)))

    ifeq ($(PLATFORM),)
        $(info Supported platforms: $(SUPPORTED_PLATFORMS))
        $(error PLATFORM is undefined. Usage: make PLATFORM=$(SUPPORTED_PLATFORMS))
    endif

    ifeq (,$(filter $(PLATFORM),$(SUPPORTED_PLATFORMS)))
        $(error Unsupported PLATFORM '$(PLATFORM)'. Supported: $(SUPPORTED_PLATFORMS))
    endif

endif

##############################################
# Load platform-specific configuration
##############################################
.DEFAULT_GOAL := all

ifneq ($(PLATFORM),)
    include $(BSP)/config/$(PLATFORM).mk
endif

##############################################
# Output directory
##############################################
OBJDIR = Obj
BUILD = Build

##############################################
# Binary
##############################################
GCC_PATH=$(HOME)/install/gcc-arm-none-eabi-7-2017-q4-major/bin
PREFIX=arm-none-eabi-
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size

##############################################
# CFLAGS
##############################################
CFLAGS = -mcpu=$(CPU) \
         -mthumb \
         -mfpu=fpv4-sp-d16 \
         -mfloat-abi=hard \
         -std=$(STANDARD) \
         -g3 \
         $(OPT)\
         -MP \
         -MMD \
         -Wall \
         -ffunction-sections \
         -fdata-sections

##############################################
# Source directiries 
##############################################
DIRS = \
    $(SUBMODULES)/hal/include \
    $(PLATFORM_DIRS) \
    $(SRCDIR) \
    $(PLATFORM_BOOT) \
    $(SUBMODULES)/core/assert \
    $(SUBMODULES)/lib/drivers/led \
    $(SUBMODULES)/lib/drivers/button \
    $(SUBMODULES)/lib/drivers/sw-timer \
    $(SUBMODULES)/core/freertos \
    $(SRCDIR)/segger/config \
    $(SRCDIR)/segger/os \
    $(SRCDIR)/segger/segger \
    $(SRCDIR)/segger/segger/syscalls

##############################################
# List of source files (.c)
##############################################
SRC = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.c))

##############################################
# List of ASM files (.s)
##############################################
ASMS = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.s))

##############################################
# List of header files (.h)
##############################################
INC = $(foreach dir, $(DIRS), $(addprefix -I, $(dir)))

##############################################
# Replace all '*.c' and all '.s' files to '.o'
##############################################
COBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))
ASMOBJS = $(patsubst %.s, $(OBJDIR)/%.o, $(ASMS))

##############################################
# List of all relocatable object files (.o)
##############################################
OBJS = $(COBJS) $(ASMOBJS)

##############################################
# Variables
##############################################
STANDARD = c99
OPT = -O0
DEFINES += -DNDEBUG=1\
           -DRTT_USE_ASM=0

TARGET = $(BUILD)/$(PLATFORM)-$(APPLICATION)
EXEC = $(TARGET).elf
RM = rm -rf

##############################################
# LDFLAGS (linker flags)
# NOTE: use newlib-nano link to libc_nano.a
# NOTE: --gc-sections - garbage collection (delete unused data and functions)
##############################################
LDFLAGS = -T $(LINKER_SCRIPT) -Xlinker --gc-sections -Xlinker \
  -Map=$(TARGET).map --specs=nano.specs -u _print_float -Wl,--print-memory-usage -o $(TARGET).elf
LDFLAGS += -mcpu=$(CPU) -mthumb

##############################################
# Default action: build all
##############################################
all: $(TARGET)
$(TARGET): $(EXEC) $(TARGET).bin

$(EXEC): $(OBJS)
	@mkdir -p $(@D)
	@echo '============='
	@echo 'Linking the target $@'
	@echo '============='
	@mkdir -p $(BUILD)
#	$(CC) $(OBJS) $(LDFLAGS) $(LIB) -o $@
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) $(LIB) -o $@
	@echo 'Linking Successful !!!'

$(OBJDIR)/%.o: %.c
	@echo '> Building C file $<'
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEFINES) $(INC) -c $< -o $@

$(OBJDIR)/%.o: %.s
	@echo '> Building ASM file $<'
	@mkdir -p $(dir $@)
	$(AS) $(CFLAGS) $(DEFINES) $(INC) -c $< -o $@
	
$(TARGET).bin: $(TARGET).elf
	@echo '> Building bin file from $<'
	@mkdir -p $(dir $@)
	$(CP) -O binary $< $@
	@echo '!!! ====== DONE =======!!!'
	
##############################################
# Dependency
-include $(COBJS:%.o=%.d)
##############################################

.PHONY: all clean

clean:
	@echo 'Cleaning...Done!'
	@$(RM) $(OBJDIR) $(BUILD)

help:
	@echo "Common targets: clean, all"
ifneq ($(PLATFORM),)
	@$(MAKE) --no-print-directory help-$(PLATFORM)
endif

