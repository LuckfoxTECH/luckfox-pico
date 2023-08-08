#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(shell cd $(CURDIR)/.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean

all:
	make -C link all
	make -C vlinktest all
	make -C client all

clean:
	make -C link clean
	make -C vlinktest clean
	make -C client clean


