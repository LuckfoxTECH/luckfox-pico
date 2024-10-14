# SPDX-License-Identifier: BSD-3-Clause */

# Copyright (c) 2020 Rockchip Electronics Co., Ltd.

ROOT_PATH	:= ../../..
PROJECT		:= RK1808
SOC		:= RK1808

#############################################################################
# Cross compiler
#############################################################################
CROSS_COMPILE	?= arm-none-eabi-

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

CPU		= -march=armv7-a

ASFLAGS		+= $(CPU) -D__ASSEMBLY__
CFLAGS		+= $(CPU) -std=gnu99 -O2 -g -Wformat=2 -Wall -Wextra -Wno-unused-parameter -Werror
LDFLAGS		+= $(CPU) -Wl,--gc-sections -lm -lgcc -nostartfiles
OCFLAGS		= -R .note -R .note.gnu.build-id -R .comment -S

LINKER_SCRIPT	:= link.lds

#############################################################################
# Output files
#############################################################################
BIN		:= TestDemo.bin
ELF		:= TestDemo.elf
MAP		:= TestDemo.map

#############################################################################
# Options
#############################################################################
QUIET ?= n

ifeq ($(QUIET), y)
  Q := @
  S := -s
endif

#############################################################################
# Head files
#############################################################################
INCLUDES += \
-I"$(ROOT_PATH)/lib/hal/inc" \
-I"$(ROOT_PATH)/lib/bsp/$(PROJECT)" \
-I"$(ROOT_PATH)/lib/CMSIS/Device/$(PROJECT)/Include" \
-I"$(ROOT_PATH)/lib/CMSIS/Core_A/Include" \
-I"../src" \

#############################################################################
# Source Files
#############################################################################
DIRS := $(ROOT_PATH)/lib/hal/src \
	$(ROOT_PATH)/lib/hal/src/* \
	$(ROOT_PATH)/lib/bsp/$(PROJECT) \
	$(ROOT_PATH)/lib/CMSIS/Device/$(PROJECT)/Source/Templates \
	../src \
	. \

SRCS += $(basename $(foreach dir,$(DIRS),$(wildcard $(dir)/*.[cS])))

OBJS := $(addsuffix .o,$(basename $(SRCS)))

CPPFLAGS += $(INCLUDES)

all: $(BIN)

$(ELF): $(OBJS) $(LINKER_SCRIPT)
	$(Q) $(CC) $(OBJS) $(LDFLAGS) $(CFLAGS) -T$(LINKER_SCRIPT) -Wl,-Map=$(MAP),-cref -o $@

$(BIN): $(ELF)
	$(Q) $(OBJCOPY) $(OCFLAGS) -O binary $< $@

clean:
	rm -f $(OBJS)
	rm -f TestDemo*

.PHONY: all clean
