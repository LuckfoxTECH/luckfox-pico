# Rockchip sysutils Makefile

SRCS := $(wildcard *.c)
OBJS  := $(SRCS:%.c=%.o)
TARGET := $(SRCS:%.c=%)

.PHONY : clean all

TARGET1 = luckfox_adc_test
TARGET2 = luckfox_gpio_test
TARGET3 = luckfox_pwm_test
TARGET4 = luckfox_i2c_test
TARGET5 = luckfox_uart_test
TARGET6 = luckfox_spi_test


all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6)

$(TARGET1):  $(TARGET1).o $(OBJ)
	@$(CC) $(CFLAGS) $(TARGET1).o $(OBJ) -o $@

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

install:
	@mv -f $(TARGET1) $(LUCKFOX_OUT_DIR)/bin
	@mv -f $(TARGET2) $(LUCKFOX_OUT_DIR)/bin
	@mv -f $(TARGET3) $(LUCKFOX_OUT_DIR)/bin
	@mv -f $(TARGET4) $(LUCKFOX_OUT_DIR)/bin
	@mv -f $(TARGET5) $(LUCKFOX_OUT_DIR)/bin
	@mv -f $(TARGET6) $(LUCKFOX_OUT_DIR)/bin


clean:
	@rm -f $(TARGET1)
	@rm -f $(TARGET2)
	@rm -f $(TARGET3)
	@rm -f $(TARGET4)
	@rm -f $(TARGET5)
	@rm -f $(TARGET6)
	@rm -f $(OBJS)
