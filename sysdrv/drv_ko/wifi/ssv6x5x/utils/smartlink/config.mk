
CROSS_PREFIX=arm-anykav200-linux-uclibcgnueabi-

CC           = $(CROSS_PREFIX)gcc
AS           = $(CROSS_PREFIX)as
AR           = $(CROSS_PREFIX)ar
LD           = $(CROSS_PREFIX)ld
RM           = rm -f

LIB_NAME=libssv_smartlink.a
LIB_DIR=lib_ssv_smartlink
TARGET=user_smartlink


