CFLAGS := -Wall -g
CC := arm-rockchip830-linux-uclibcgnueabihf-gcc

all: rkwifi_server

OBJS += \
	rkwifi_cli.o \
	main.o \
	rk_wifi_test.o \
	softap/softap.o

ARCH=arm
CFLAGS += -I ../include/ -I ./ -fpic -rdynamic
#CFLAGS += -Wno-declaration-after-statement -funsigned-char -freg-struct-return -fno-strict-aliasing -Wtrampolines -Wformat=2 -Wshadow -Wall -fpic -fpie -fno-common -Wfloat-equal -Wextra -fvisibility=hidden
#CFLAGS += -D_FORTIFY_SOURCE=2 -O2 -pipe
#LDFLAGS += -Wl,-z,relro,-z,now -Wl,-z,noexecstack -pie -Wl,-Bsymbolic -rdynamic -Wl,--no-undefined -fpic
LDFLAGS += -lpthread -lrkwifibt -lwpa_client -L ../../out/lib/ -L ../

rkwifi_server: $(OBJS)
	$(info "$(PWD)")
	$(CC) -o rkwifi_server $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) rkwifi_server
