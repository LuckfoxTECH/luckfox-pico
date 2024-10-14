HOST_CC = gcc
CFLAGS = -O2 -Wall -I../include

ifeq ($(IS_NAND_FLASH), YES)
	CFLAGS += -DIS_NAND_FLASH -I ../mtd_updateEngeen/
endif

target=make_meta.c
target_bin_host=$(patsubst %.c,%,$(target))_host
target_bin=$(patsubst %.c,%,$(target))
CROSS_COMPILE_CC ?= arm-rockchip830-linux-uclibcgnueabihf-gcc

all: 
	$(HOST_CC) $(CFLAGS) -o $(target_bin_host) $(target) -I ../zlib/out_host_pc/include/ ../zlib/out_host_pc/lib/libz.a 
ifneq ($(CROSS_COMPILE_CC),)
	$(CROSS_COMPILE_CC) $(CFLAGS) -o $(target_bin) $(target) -I ../zlib/out_board/include/ ../zlib/out_board/lib/libz.a \
		-I ../mtd_updateEngeen/ -Wl,--whole-archive ../mtd_updateEngeen/out_board/libmtd-tools.a -Wl,--no-whole-archive
endif

clean:
	@rm -f *.o $(target_bin) $(target_bin_host)
