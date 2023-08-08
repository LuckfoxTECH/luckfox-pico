CFLAGS := -Wall -g
CC := arm-rockchip830-linux-uclibcgnueabihf-gcc

all: librkwifibt rkwifi_server

$(warning "wifi chip" $(RK_ENABLE_WIFI_CHIP))

#hisi
ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"HI3861L"),)
OBJS := src/hisi_tools/securec/fscanf_s.o src/hisi_tools/securec/memmove_s.o src/hisi_tools/securec/secureinput_a.o src/hisi_tools/securec/snprintf_s.o \
	src/hisi_tools/securec/strcpy_s.o src/hisi_tools/securec/swprintf_s.o src/hisi_tools/securec/vscanf_s.o src/hisi_tools/securec/vswprintf_s.o \
	src/hisi_tools/securec/wcscpy_s.o src/hisi_tools/securec/wmemcpy_s.o src/hisi_tools/securec/fwscanf_s.o src/hisi_tools/securec/memset_s.o src/hisi_tools/securec/secureinput_w.o \
	src/hisi_tools/securec/sprintf_s.o src/hisi_tools/securec/strncat_s.o src/hisi_tools/securec/swscanf_s.o src/hisi_tools/securec/vsnprintf_s.o src/hisi_tools/securec/vswscanf_s.o \
	src/hisi_tools/securec/wcsncat_s.o src/hisi_tools/securec/wmemmove_s.o src/hisi_tools/securec/gets_s.o src/hisi_tools/securec/scanf_s.o src/hisi_tools/securec/secureprintoutput_a.o src/hisi_tools/securec/sscanf_s.o \
	src/hisi_tools/securec/strncpy_s.o src/hisi_tools/securec/vfscanf_s.o src/hisi_tools/securec/vsprintf_s.o src/hisi_tools/securec/vwscanf_s.o src/hisi_tools/securec/wcsncpy_s.o src/hisi_tools/securec/wscanf_s.o \
	src/hisi_tools/securec/memcpy_s.o src/hisi_tools/securec/securecutil.o src/hisi_tools/securec/secureprintoutput_w.o src/hisi_tools/securec/strcat_s.o src/hisi_tools/securec/strtok_s.o \
	src/hisi_tools/securec/vfwscanf_s.o src/hisi_tools/securec/vsscanf_s.o src/hisi_tools/securec/wcscat_s.o src/hisi_tools/securec/wcstok_s.o \
	src/hisi_tools/vlinktest/hichannel_host.o src/hisi_tools/vlinktest/hichannel_host_comm.o src/hisi_tools/vlinktest/vlink_hichannel_cmd.o src/hisi_tools/vlinktest/vlink_hichannel_main.o \
	src/hisi_tools/vlinktest/vlink_tcp_socket_main.o src/hisi_tools/vlinktest/vlink_wifi_ota_main.o src/hisi_tools/vlinktest/cJSON.o\
	src/rk_hisi_wifi.o
CFLAGS += -I src/hisi_tools/include/ -I src/hisi_tools/securec/ -DHI3861L
endif

#atbm
ifneq ($(findstring $(RK_ENABLE_WIFI_CHIP),"ATBM6441"),)
OBJS += \
	src/atbm6441/tools.o \
	src/rk_atbm_wifi.o
CFLAGS += -I src/atbm6441/ -DATBM6441
endif

#wpa_supplicant
ifeq ($(RK_ENABLE_FASTBOOT),)
OBJS += \
	src/Rk_wifi.o \
	utility/encode/gbk_to_utf8.o \
	utility/encode/utf8_to_gbk.o \
	utility/encode/RK_encode.o
CFLAGS += -lwpa_client -DWPA_WIFI
endif

#common
OBJS += \
	src/rk_wifi_hal.o \
	utility/utility.o
CFLAGS += -I utility/

#test
#TEST_OBJS += \
	test/main.o \
	test/rk_wifi_test.o \
	test/softap/softap.o

ARCH=arm
CFLAGS += -lpthread -L ../out/lib/ -I include/ -I ./
#CFLAGS += -funsigned-char -freg-struct-return -fno-strict-aliasing -Wtrampolines -Wformat=2 -Wshadow -Wall -fpic -fpie -fno-common -Wfloat-equal -Wextra -fvisibility=hidden
CFLAGS += -funsigned-char -freg-struct-return -fno-strict-aliasing -Wtrampolines -Wformat=2 -Wshadow -Wall -fpic -fno-common -Wfloat-equal -Wextra
CFLAGS += -D_FORTIFY_SOURCE=2 -O2 -pipe
#规范建议：在Debug版本中实施，Release版本不实施
#CFLAGS += -ftrapv
#CFLAGS += -Werror

#ARCH=arm64
#CFLAGS += -lpthread -lasound -L lib64/ -lrkwifibt -I include/

librkwifibt: $(OBJS)
	$(CC) -o librkwifibt.so $(OBJS) $(CFLAGS) -shared

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

rkwifi_server: librkwifibt
	make -C test/

clean:
	rm -f $(OBJS) test/rkwifi_server test/*.o src/hisi_tools/vlinktest/*.o src/hisi_tools/securec/*.o
