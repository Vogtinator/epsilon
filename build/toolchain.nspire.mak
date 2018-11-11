CC = nspire-gcc
CXX = nspire-g++
LD = nspire-gcc
GDB = arm-none-eabi-gdb
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

# Always generate debug information
SFLAGS += -ggdb3

# LTO ?= NOT(DEBUG)
ifeq ($(DEBUG),1)
LTO ?= 0
else
LTO ?= 1
endif

ifeq ($(LTO),1)
# Use link-time optimization if LTO=1
SFLAGS += -flto
else
# Otherwise, just get rid of unused symbols
SFLAGS += -fdata-sections -ffunction-sections
LDFLAGS += -Wl,--gc-sections
endif

LDFLAGS += $(SFLAGS)
