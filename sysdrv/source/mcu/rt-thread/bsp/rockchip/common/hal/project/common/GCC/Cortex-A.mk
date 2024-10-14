# SPDX-License-Identifier: BSD-3-Clause */

# Copyright (c) 2021 Rockchip Electronics Co., Ltd.

ROOT_PATH	:= ../../..

#############################################################################
# Cross compiler
#############################################################################
ifneq ($(wildcard ${ROOT_PATH}/../prebuilts/gcc/linux-x86/arm/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux),)
CROSS_COMPILE	?= ${ROOT_PATH}/../prebuilts/gcc/linux-x86/arm/gcc-arm-none-eabi-10-2020-q4-major-x86_64-linux/bin/arm-none-eabi-
else
CROSS_COMPILE	?= arm-none-eabi-
endif

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

CPU		+= -ftree-vectorize -ffast-math -marm
ASFLAGS		+= $(CPU) -c -x assembler-with-cpp -D__ASSEMBLY__
CFLAGS		+= $(CPU) -std=gnu99 -O2 -g
CFLAGS		+= -Wformat=2 -Wall -Wno-unused-parameter
CFLAGS		+= -Wstrict-prototypes -Wmissing-prototypes -nostartfiles
LDFLAGS		+= $(CPU) -Wl,--gc-sections --specs=nosys.specs -lm -lgcc
OCFLAGS		= -R .note -R .note.gnu.build-id -R .comment -S

HAL_CFLAGS	+= -Werror

LINKER_SCRIPT	?= $(ROOT_PATH)/lib/CMSIS/Device/$(SOC)/Source/Templates/GCC/gcc_arm.ld

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
# Source code and include
#############################################################################
INCLUDES += \
-I"../src" \
-I"$(ROOT_PATH)/lib/CMSIS/Core_A/Include" \
-I"$(ROOT_PATH)/lib/CMSIS/Device/$(SOC)/Source/Templates/GCC" \

SRC_DIRS += \
    ../src \
    $(ROOT_PATH)/lib/CMSIS/Device/$(SOC)/Source/Templates/GCC \

export HAL_PATH := $(ROOT_PATH)
include $(HAL_PATH)/tools/build_lib.mk
SRC_DIRS += $(HAL_LIB_SRC)
INCLUDES += $(HAL_LIB_INC)
-include $(HAL_PATH)/test/build_test.mk
CFLAGS += -DUNITY_INCLUDE_CONFIG_H

SRCS += $(basename $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.[cS])))
OBJS += $(addsuffix .o,$(basename $(SRCS)))
CFLAGS += $(INCLUDES)
ASFLAGS += $(INCLUDES)

HAL_SRCS += $(basename $(foreach dir,$(HAL_LIB_SRC) $(ROOT_PATH)/lib/CMSIS/Device/$(SOC)/Source/Templates/GCC,$(wildcard $(dir)/*.[cS])))
HAL_OBJS += $(addsuffix .o,$(basename $(HAL_SRCS)))
$(HAL_OBJS): CFLAGS += $(HAL_CFLAGS)

all: $(BIN)

%.ld: %.ld.S
	$(Q) $(CPP) -P $(CFLAGS) -o $@ $<

$(ELF): $(OBJS) $(LINKER_SCRIPT)
	$(Q) $(CC) $(OBJS) $(LDFLAGS) $(CFLAGS) -T$(LINKER_SCRIPT) -Wl,-Map=$(MAP),-cref -o $@

$(BIN): $(ELF)
	$(Q) $(OBJCOPY) $(OCFLAGS) -O binary $< $@

CLEAN_FILES += $(OBJS) TestDemo*

clean:
	rm -f $(CLEAN_FILES)

.PHONY: all clean

