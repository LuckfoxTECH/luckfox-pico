# Rockchip sysutils Makefile

SRCS := $(wildcard *.c)
OBJS  := $(SRCS:%.c=%.o)
TARGET := $(SRCS:%.c=%)

.PHONY : clean all

TARGET1 = rk_event_test
TARGET2 = rk_gpio_test
TARGET3 = rk_adc_test
TARGET4 = rk_pwm_test
TARGET5 = rk_led_test
TARGET6 = rk_watchdog_test
TARGET7 = rk_time_test
TARGET8 = rk_system_test

all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) $(TARGET7) $(TARGET8)

$(TARGET1): $(TARGET1).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET1).o $(OBJ) -o $@ -lpthread

$(TARGET2):  $(TARGET2).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET2).o $(OBJ) -o $@

$(TARGET3):  $(TARGET3).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET3).o $(OBJ) -o $@

$(TARGET4):  $(TARGET4).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET4).o $(OBJ) -o $@

$(TARGET5):  $(TARGET5).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET5).o $(OBJ) -o $@

$(TARGET6):  $(TARGET6).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET6).o $(OBJ) -o $@

$(TARGET7):  $(TARGET7).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET7).o $(OBJ) -o $@

$(TARGET8):  $(TARGET8).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET8).o $(OBJ) -o $@

install:
	@mv -f $(TARGET1) $(SYSUTILS_OUT_DIR)/bin
	@mv -f $(TARGET2) $(SYSUTILS_OUT_DIR)/bin
	@mv -f $(TARGET3) $(SYSUTILS_OUT_DIR)/bin
	@mv -f $(TARGET4) $(SYSUTILS_OUT_DIR)/bin
	@mv -f $(TARGET5) $(SYSUTILS_OUT_DIR)/bin
	@mv -f $(TARGET6) $(SYSUTILS_OUT_DIR)/bin
	@mv -f $(TARGET7) $(SYSUTILS_OUT_DIR)/bin
	@mv -f $(TARGET8) $(SYSUTILS_OUT_DIR)/bin

clean:
	@rm -f $(TARGET1)
	@rm -f $(TARGET2)
	@rm -f $(TARGET3)
	@rm -f $(TARGET4)
	@rm -f $(TARGET5)
	@rm -f $(TARGET6)
	@rm -f $(TARGET7)
	@rm -f $(TARGET8)
	@rm -f $(OBJS)
