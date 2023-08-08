#===============================================================================
# export variable
#===============================================================================
ifeq ($(CFG_HI_EXPORT_FLAG),)
SDK_DIR = $(shell cd $(CURDIR)/../.. && /bin/pwd)
include $(SDK_DIR)/base.mak
endif

#CURDIR ?= $(shell pwd)
#CC := $(CROSS_COMPILE)gcc
#===============================================================================
# local variable
#===============================================================================
SAMPLE_IMAGES := vlink_hichannel_main

CFLAGS = -Wno-declaration-after-statement -funsigned-char -freg-struct-return -fno-strict-aliasing -Wtrampolines -Wformat=2 -Wshadow -Wall -fpic -fpie -fno-common -Wfloat-equal -Wextra -fvisibility=hidden
CFLAGS += -D_FORTIFY_SOURCE=2 -O2 -pipe
#规范建议：在Debug版本中实施，Release版本不实施
#CFLAGS += -ftrapv

CFLAGS += -I$(CURDIR)/../../securec
CFLAGS += -I$(CURDIR)/../include
CFLAGS += -D_GNU_SOURCE
$(warning $(CURDIR))
LDFLAGS := -Wl,-z,relro,-z,now -Wl,-z,noexecstack -pie -Wl,-Bsymbolic -rdynamic -Wl,--no-undefined
LIBPATH += -L$(CURDIR)/../../securec

SAMPLE_OBJ := vlink_hichannel_main.o hichannel_host.o hichannel_host_comm.o cJSON.o vlink_hichannel_cmd.o vlink_tcp_socket_main.o vlink_wifi_ota_main.o

DEPEND_LIBS := -lpthread -lhi_securec -lm
.PHONY: all clean
all:$(SAMPLE_IMAGES)

vlink_hichannel_main: $(SAMPLE_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBPATH) $(DEPEND_LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	-@rm -f $(SAMPLE_IMAGES)
	-@rm -f $(SAMPLE_OBJ)
