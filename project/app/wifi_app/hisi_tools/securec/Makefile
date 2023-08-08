#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR ?= $(CURDIR)/..

include $(SDK_DIR)/base.mak
endif
#CURDIR ?= $(shell pwd)
#CC := $(CROSS_COMPILE)gcc

#===============================================================================
# local variable
#===============================================================================
CFLAGS = -Werror -funsigned-char -freg-struct-return -fno-strict-aliasing -Wtrampolines -Wformat=2 -Wshadow -Wall -fpic -fpie -fno-common -Wfloat-equal -Wextra -fvisibility=hidden
CFLAGS += -D_FORTIFY_SOURCE=2 -O2 -pipe
#规范建议：在Debug版本中实施，Release版本不实施
#CFLAGS += -ftrapv

CFLAGS += -I$(CURDIR)

#ifeq (y,$(CFG_HI_TEST_SUPPORT))
#CFLAGS += -I$(COMMON_DIR)/drv/test
#endif

LIBS := libhi_securec

OBJS := fscanf_s.o memmove_s.o secureinput_a.o snprintf_s.o strcpy_s.o swprintf_s.o vscanf_s.o vswprintf_s.o wcscpy_s.o wmemcpy_s.o \
	    fwscanf_s.o memset_s.o secureinput_w.o sprintf_s.o strncat_s.o swscanf_s.o vsnprintf_s.o vswscanf_s.o wcsncat_s.o wmemmove_s.o \
	    gets_s.o scanf_s.o secureprintoutput_a.o sscanf_s.o strncpy_s.o vfscanf_s.o vsprintf_s.o vwscanf_s.o wcsncpy_s.o wscanf_s.o \
	    memcpy_s.o securecutil.o secureprintoutput_w.o strcat_s.o strtok_s.o vfwscanf_s.o vsscanf_s.o wcscat_s.o wcstok_s.o

PUB_HEADERS  := $(CURDIR)/securec.h

#===============================================================================
# rules
#===============================================================================
.PHONY: all clean install uninstall

all: $(DEPENDS) $(LIBS)

clean:
	$(AT)-rm -f $(LIBS).a
	$(AT)-rm -f $(OBJS)
#	$(AT)find $(COMMON_DIR)/ -name "*.[dios]" -exec rm {} \;
#	$(AT)find $(COMMON_DIR)/ -name "*.d.*" -exec rm {} \;

install:all
	$(AT)cp -f $(PUB_HEADERS) $(INCLUDE_DIR)/
	$(AT)cp -f $(LIBS).a  $(STATIC_LIB_DIR)
	$(AT)echo "Install common resources completed."

uninstall:
	$(AT)cd $(INCLUDE_DIR) && rm -f $(notdir $(PUB_HEADERS)) ; cd - 1>/dev/null
	$(AT)rm -f $(STATIC_LIB_DIR)/$(LIBS).a
	$(AT)rm -f $(INCLUDE_DIR)/hi_flash.h
	$(AT)echo "Uninstall common resources completed."

#################################################################################
## Building library
$(LIBS): $(OBJS)
	$(AT)$(AR) -rcs $@.a $^
	$(AT)echo "Build $@ has completed."

## Building api objects
## using: -save-temps for temporary files
#${OBJS}: %.o : %.c
#	$(AT)echo "Compiling $@..."
#	$(AT)$(CC) $(CFLAGS) -c $< -o $@

$(DEPENDS):%.d:%.c
	$(AT)echo "[$(CC)] $@..."
	$(AT)set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($(notdir $*)\)\.o[ :]*,$(dir $@)\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(DEPENDS)
