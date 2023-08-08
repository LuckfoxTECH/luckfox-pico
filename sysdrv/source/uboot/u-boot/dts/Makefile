#
# Copyright (c) 2011 The Chromium OS Authors.
#
# SPDX-License-Identifier:	GPL-2.0+
#

# This Makefile builds the internal U-Boot fdt if CONFIG_OF_CONTROL is
# enabled. See doc/README.fdt-control for more details.

DEVICE_TREE ?= $(CONFIG_DEFAULT_DEVICE_TREE:"%"=%)
ifeq ($(DEVICE_TREE),)
DEVICE_TREE := unset
endif

ARCH_PATH := arch/$(ARCH)/dts
dtb_depends := arch-dtbs

ifneq ($(EXT_DTB),)
DTB := $(EXT_DTB)
else
DTB := $(ARCH_PATH)/$(DEVICE_TREE).dtb
dtb_depends += $(DTB:.dtb=.dts)
endif

# Pass the original device tree file through fdtgrep twice. The first pass
# removes any unwanted nodes (i.e. those which don't have the
# 'u-boot,dm-pre-reloc' property and thus are not needed by SPL. The second
# pass removes various unused properties from the remaining nodes.
# The output is typically a much smaller device tree file.
ifeq ($(CONFIG_TPL_BUILD),y)
fdtgrep_props := -b u-boot,dm-pre-reloc -b u-boot,dm-tpl
else
fdtgrep_props := -b u-boot,dm-pre-reloc -b u-boot,dm-spl
endif

quiet_cmd_fdtgrep = FDTGREP $@
      cmd_fdtgrep = $(objtree)/tools/fdtgrep $(fdtgrep_props) -RT $< \
                -n /chosen -n /config -O dtb | \
        $(objtree)/tools/fdtgrep -r -O dtb - -o $@ \
                $(addprefix -P ,$(subst $\",,$(CONFIG_OF_SPL_REMOVE_PROPS)))

quiet_cmd_fdtgrep_uboot = FDTGREP $@
      cmd_fdtgrep_uboot = $(objtree)/tools/fdtgrep $(fdtgrep_props) -RT $< \
		-n /chosen -n /config -O dtb | \
	$(objtree)/tools/fdtgrep -r -O dtb - -o $@ \
		$(addprefix -P ,$(subst $\",,$(CONFIG_OF_U_BOOT_REMOVE_PROPS)))

fdtgrep_tpl_props := -b u-boot,dm-pre-reloc -b u-boot,dm-tpl
quiet_cmd_fdtgrep_tpl = FDTGREP $@
      cmd_fdtgrep_tpl = $(objtree)/tools/fdtgrep $(fdtgrep_tpl_props) -RT $< \
                -n /chosen -n config -O dtb | \
        $(objtree)/tools/fdtgrep -r -O dtb - -o $@ \
                $(addprefix -P ,$(subst $\",,$(CONFIG_OF_SPL_REMOVE_PROPS)))

fdtgrep_spl_minimum_props := -b u-boot,dm-spl
quiet_cmd_fdtgrep_spl_minimum = FDTGREP $@
      cmd_fdtgrep_spl_minimum = $(objtree)/tools/fdtgrep $(fdtgrep_spl_minimum_props) -RT $< \
                -n /chosen -n config -O dtb | \
        $(objtree)/tools/fdtgrep -r -O dtb - -o $@ \
                $(addprefix -P ,$(subst $\",,$(CONFIG_OF_SPL_REMOVE_PROPS)))

$(obj)/dt-tpl.dtb: $(DTB) $(objtree)/tools/fdtgrep FORCE
	$(call if_changed,fdtgrep_tpl)

ifeq ($(CONFIG_SPL_DTB_MINIMUM),y)
$(obj)/dt-spl.dtb: $(DTB) $(objtree)/tools/fdtgrep FORCE
	$(call if_changed,fdtgrep_spl_minimum)
else
$(obj)/dt-spl.dtb: $(DTB) $(objtree)/tools/fdtgrep FORCE
	$(call if_changed,fdtgrep)
endif

# Normally, usbplug is enabled with kernel dtb disabled
ifneq ($(CONFIG_USING_KERNEL_DTB)$(CONFIG_SUPPORT_USBPLUG),)
$(obj)/dt.dtb: $(DTB) $(objtree)/tools/fdtgrep FORCE
ifneq ($(CONFIG_OF_U_BOOT_REMOVE_PROPS), "")
	$(call if_changed,fdtgrep_uboot)
else
	$(call if_changed,fdtgrep)
endif
else
$(obj)/dt.dtb: $(DTB) FORCE
	$(call if_changed,shipped)
endif

targets += dt.dtb dt-spl.dtb dt-tpl.dtb

$(DTB): $(dtb_depends)
ifeq ($(EXT_DTB),)
	$(Q)$(MAKE) $(build)=$(ARCH_PATH) $@
endif
	$(Q)test -e $@ || (						\
	echo >&2;							\
	echo >&2 "Device Tree Source is not correctly specified.";	\
	echo >&2 "Please define 'CONFIG_DEFAULT_DEVICE_TREE'";		\
	echo >&2 "or build with 'DEVICE_TREE=<device_tree>' argument";	\
	echo >&2;							\
	/bin/false)

arch-dtbs:
	$(Q)$(MAKE) $(build)=$(ARCH_PATH) dtbs

.SECONDARY: $(obj)/dt.dtb.S $(obj)/dt-spl.dtb.S $(obj)/dt-tpl.dtb.S


ifeq ($(CONFIG_SPL_BUILD),y)
obj-$(CONFIG_OF_EMBED) := dt-spl.dtb.o
# support "out-of-tree" build for dtb-spl
$(obj)/dt-tpl.dtb.o: $(obj)/dt-tpl.dtb.S FORCE
	$(call if_changed_dep,as_o_S)

$(obj)/dt-spl.dtb.o: $(obj)/dt-spl.dtb.S FORCE
	$(call if_changed_dep,as_o_S)
else
obj-$(CONFIG_OF_EMBED) := dt.dtb.o
endif

dtbs: $(obj)/dt.dtb $(obj)/dt-spl.dtb $(obj)/dt-tpl.dtb
	@:

clean-files := dt.dtb.S dt-spl.dtb.S dt-tpl.dtb.S

# Let clean descend into dts directories
subdir- += ../arch/arm/dts ../arch/microblaze/dts ../arch/mips/dts ../arch/sandbox/dts ../arch/x86/dts
