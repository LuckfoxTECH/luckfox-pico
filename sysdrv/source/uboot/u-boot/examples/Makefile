#
# SPDX-License-Identifier:	GPL-2.0+
#

ifndef CONFIG_SANDBOX

ifdef FTRACE
subdir-ccflags-y += -finstrument-functions -DFTRACE
endif

# Here is an compile issue: "undefined reference to `__aeabi_unwind_cpp_pr0'..."
# when -funwind-tables is enabled, so make it compile depends on -funwind-tables.
ifeq (,$(findstring -funwind-tables,$(PLATFORM_CPPFLAGS)))
subdir-y += standalone
subdir-$(CONFIG_API) += api
endif
endif
