obj-m += pwm_motor.o

KDIR := /home/phamvannhat/luckfox-pico/sysdrv/source/kernel

ARCH := arm
CROSS_COMPILE := arm-rockchip830-linux-uclibcgnueabihf-
JOBS ?= $(shell nproc)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -j$(JOBS) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
