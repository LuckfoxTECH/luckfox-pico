CC = gcc
CFLAGS = -c -Wall 
LDFLAGS =

FLASH_BIN_UTIL_SOURCES:=flash_bin_util.c 
FLASH_BIN_UTIL_OBJECTS=$(FLASH_BIN_UTIL_SOURCES:.c=.o)
FLASH_BIN_UTIL_EXECUTABLE=flash_bin_util
    

do_all:
	make all 2>&1 

all: flash

flash: $(FLASH_BIN_UTIL_SOURCES) $(FLASH_BIN_UTIL_EXECUTABLE)

$(FLASH_BIN_UTIL_EXECUTABLE): $(FLASH_BIN_UTIL_OBJECTS) 
	$(CC) $(CC_INCLUDE) $(LDFLAGS) $(FLASH_BIN_UTIL_OBJECTS) -o $(FLASH_BIN_UTIL_EXECUTABLE) -ldl

.c.o:
	$(CC) $(CFLAGS) $(CC_INCLUDE) $(LDFLAGS) $< -o $@

clean:
	rm *.o flash_bin_util



