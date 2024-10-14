# --------------------------------------------------------------------------------
# SET compiler {{{ START
# --------------------------------------------------------------------------------
ifneq ($(RK_TOOLCHAIN_CROSS),)
MTD_CROSS_COMPILER_PREFIX ?= $(RK_TOOLCHAIN_CROSS)-
else
MTD_CROSS_COMPILER_PREFIX ?= arm-rockchip830-linux-uclibcgnueabihf-
endif
CC                    := $(MTD_CROSS_COMPILER_PREFIX)gcc
CXX                   := $(MTD_CROSS_COMPILER_PREFIX)g++
AR                    := $(MTD_CROSS_COMPILER_PREFIX)ar
AS                    := $(MTD_CROSS_COMPILER_PREFIX)as
LD                    := $(MTD_CROSS_COMPILER_PREFIX)ld
CPP                   := $(MTD_CROSS_COMPILER_PREFIX)cpp
NM                    := $(MTD_CROSS_COMPILER_PREFIX)nm
STRIP                 := $(MTD_CROSS_COMPILER_PREFIX)strip
OBJCOPY               := $(MTD_CROSS_COMPILER_PREFIX)objcopy
OBJDUMP               := $(MTD_CROSS_COMPILER_PREFIX)objdump
# --------------------------------------------------------------------------------
# SET compiler END }}}
# --------------------------------------------------------------------------------

# $(info MTD_CROSS_COMPILER_PREFIX = $(MTD_CROSS_COMPILER_PREFIX))

CFLAGS         := -L ./ -I ./ -fPIC -w

SOURCES_C     := common.c flash_erase.c libmtd.c libmtd_legacy.c mtd_debug.c mtd_utils_all.c nanddump.c nandwrite.c

OBJECTS_O     = $(addsuffix .o, $(basename $(SOURCES_C)))

LIBNAME := libmtd-tools.a

all:
	@mkdir -p ./out_board
	$(CC) $(CFLAGS) -o out_board/common.o        -c common.c
	$(CC) $(CFLAGS) -o out_board/flash_erase.o   -c flash_erase.c
	$(CC) $(CFLAGS) -o out_board/libmtd.o        -c libmtd.c
	$(CC) $(CFLAGS) -o out_board/libmtd_legacy.o -c libmtd_legacy.c
	$(CC) $(CFLAGS) -o out_board/mtd_debug.o     -c mtd_debug.c
	$(CC) $(CFLAGS) -o out_board/mtd_utils_all.o -c mtd_utils_all.c
	$(CC) $(CFLAGS) -o out_board/nanddump.o      -c nanddump.c
	$(CC) $(CFLAGS) -o out_board/nandwrite.o     -c nandwrite.c
	cd out_board && $(AR) rcs $(LIBNAME) *.o &&  cd ..
	# $(CC) flash_rw_demo.c -o flash_rw_demo $(LIBNAME) $(CFLAGS)

clean:
	@$(RM) -rf $(OBJECTS_O) ./out_board
