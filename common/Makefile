#
# This Makefile is intended to be used in the following layout
#
# /foo/common/Makefile [THIS FILE]
# /foo/common/bar.c
# /foo/src/project1/Makefile -> ../../common/Makefile [SYMLINK]
# /foo/src/project1/main.c
#
# It will compile c files in common and in the project specific
# directory and link everything together.
#


# Project name, just use the directry name we're in
PROJECT_NAME = $(notdir $(CURDIR))

# Build directory, where we put .o etc.
BUILDDIR = $(abspath $(CURDIR)/build)

# Top level directory with common/, src/ etc.
BASEDIR = $(abspath $(CURDIR)/../..)

# Directory with stlink
LOADERPATH = $(abspath $(BASEDIR)/stlink)

# Common directory
COMMONDIR =  $(abspath $(BASEDIR)/common)

# Path to he arm-none-eabi compiler
DETECTCOMPILER = $(shell command -v arm-none-eabi-gcc 2> /dev/null)
ifneq ($(strip $(DETECTCOMPILER)),)
  COMPILERPATH = $(dir $(DETECTCOMPILER))
  $(info Using compiler found at: $(DETECTCOMPILER))
else
  COMPILERPATH = /usr/bin
endif

# Check whether we have a local config.h. If so, -imacros it first so it can override common/config.h
ifneq ("$(wildcard $(CURDIR)/config.h)","")
  CONFIG_INCLUDE = -imacros config.h
else
  CONFIG_INCLUDE =
endif

# Compiler flags
# We'll use the Thumb instruction set and no stdlib
CFLAGS = -Wall -Os -mthumb -ffunction-sections -fdata-sections -fstack-usage -nostdlib -MMD $(CONFIG_INCLUDE) -I$(COMMONDIR) -I$(COMMONDIR)/arm -I$(COMMONDIR)/stmicro

# linker options
LDFLAGS = -Os -Wl,--gc-sections -Wl,-Map=$(BUILDDIR)/ld.map -mthumb

# additional libraries to link
LIBS = -lm

# MCU specific options
CFLAGS += -mcpu=cortex-m3
LDSCRIPT = $(COMMONDIR)/stm32f103c8t6.ld
LDFLAGS += -mcpu=cortex-m3 -T$(LDSCRIPT)

# Different GCC binaries
CC = $(COMPILERPATH)/arm-none-eabi-gcc
OBJCOPY = $(COMPILERPATH)/arm-none-eabi-objcopy
SIZE = $(COMPILERPATH)/arm-none-eabi-size

# cflow options
CFLOWFLAGS = --cpp='$(CC) $(CFLAGS) -E' --omit-arguments

# Get c files
SRC_FILES := $(wildcard $(CURDIR)/*.c)
COMMON_FILES := $(wildcard $(COMMONDIR)/*.c)

# Transform c sources to object files in the build dir
COMMON_OBJS := $(foreach obj,$(notdir $(COMMON_FILES:.c=.o)), $(BUILDDIR)/common/$(obj))
SRC_OBJS := $(foreach obj,$(notdir $(SRC_FILES:.c=.o)), $(BUILDDIR)/src/$(obj))

# Get files in common that are generated from *.py
GEN_COMMON_FILES := $(basename $(wildcard $(COMMONDIR)/*.py))

all: hex

generated: $(GEN_COMMON_FILES)

build: $(BUILDDIR)/$(PROJECT_NAME).elf

hex: $(BUILDDIR)/$(PROJECT_NAME).hex

load_sipeed: $(BUILDDIR)/$(PROJECT_NAME).elf
	@echo "Running OpenOCD with sipeed.cfg"
	openocd -f $(BASEDIR)/extra/sipeed.cfg -c "program $< verify reset exit"

load: $(BUILDDIR)/$(PROJECT_NAME).hex $(LOADERPATH)/build/Release/st-flash
	@echo "Running st-flash."
	@$(LOADERPATH)/build/Release/st-flash --format=ihex --flash=64k write $<

$(LOADERPATH)/build/Release/st-flash:
	@echo "Making st-flash"
	$(MAKE) -C $(LOADERPATH) release

$(COMMONDIR)/%: $(COMMONDIR)/%.py
	@echo "[PY] \t$<"
	@python "$<" > "$@"

$(BUILDDIR)/common/%.o: $(COMMONDIR)/%.c $(GEN_COMMON_FILES)
	@echo "[CC] \t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"

$(BUILDDIR)/src/%.o: $(CURDIR)/%.c $(GEN_COMMON_FILES)
	@echo "[CC] \t$<"
	@mkdir -p "$(dir $@)"
	@$(CC) $(CFLAGS) -o "$@" -c "$<"


$(BUILDDIR)/$(PROJECT_NAME).elf: $(SRC_OBJS) $(COMMON_OBJS) $(LDSCRIPT)
	@echo "[LD] \t$@"
	@$(CC) $(LDFLAGS) $(LIBS) -o "$@" $(SRC_OBJS) $(COMMON_OBJS)

$(BUILDDIR)/%.hex: $(BUILDDIR)/%.elf
	@echo "[HEX] \t$@"
	@$(SIZE) "$<"
	@$(OBJCOPY) -O ihex -R .eeprom "$<" "$@"

$(BUILDDIR)/$(PROJECT_NAME).cflow: $(SRC_FILES) $(COMMON_FILES) $(GEN_COMMON_FILES)
	@echo "[CFLOW] \t$@"
	@cflow -o$@ $(CFLOWFLAGS) $(SRC_FILES) $(COMMON_FILES) 2> /dev/null

# compiler generated dependency info
-include $(OBJS:.o=.d)

clean:
	@echo Cleaning...
	@rm -rf "$(BUILDDIR)"

.PHONY: all build hex load load_sipeed clean generated
