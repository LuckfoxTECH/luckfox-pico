ifeq ($(SYSDRV_PARAM), )
	SYSDRV_PARAM:=../../../Makefile.param
	include $(SYSDRV_PARAM)
endif

CC := $(CROSS_COMPILE)gcc

target1 := motor_24byj48_test
target2 := rk_motor_test

all : $(target1) $(target2)

$(target1): motor_24byj48_test.c
	$(CC) $^ -o $@

$(target2): rk_motor_test.o $(OBJ)
	$(CC) $^ -o $@

rk_motor_test.o : rk_motor_test.c 
	$(CC) -c $(INC_FLAGS_MOTOR) rk_motor_test.c -o rk_motor_test.o

clean:
	rm -f $(target1) $(target2)
	rm -f  rk_motor_test.o rk_motor_test motor_24byj48_test

info:
	@echo "$(INC_FLAGS_MOTOR)"; 