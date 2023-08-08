#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SAMPLE_DIR ?= $(shell cd $(CURDIR) && /bin/pwd)
endif
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#       variable
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
app_sample = app
libsec = securec
objects := $(libsec) $(app_sample)
objects_clean:=$(addsuffix _clean,$(objects))
#===============================================================================
# rules
# #===============================================================================
.PHONY: all clean

all: $(objects)
clean: $(objects_clean)
#==================================================================
#                          modules
#==================================================================
.PHONY: $(objects) $(objects_clean) $(libsec) $(app_sample)

$(libsec):
	make -C $(libsec) all

$(app_sample):$(libsec)
	make -C $(app_sample) all

$(objects_clean):
	make -C $(SAMPLE_DIR)/$(patsubst %_clean,%,$@) clean