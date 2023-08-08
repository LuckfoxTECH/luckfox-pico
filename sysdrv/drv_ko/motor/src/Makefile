ifeq ($(SYSDRV_PARAM), )
	SYSDRV_PARAM:=../../../Makefile.param
	include $(SYSDRV_PARAM)
endif

CC := $(CROSS_COMPILE)gcc

rk_motor.o: rk_motor.h
	$(CC) -c rk_motor.c -o $@

clean:
	rm -f  rk_motor.o