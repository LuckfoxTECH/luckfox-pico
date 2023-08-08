CC = gcc
CFLAGS = -c -Wall 
LDFLAGS =

CLI_SOURCES:=icomm_rawdata_util.c 
CLI_OBJECTS=$(CLI_SOURCES:.c=.o)
CLI_EXECUTABLE=icomm_rawdata_util
    

do_all:
	make all 2>&1 |tee test.log

all:  client

client: $(CLI_SOURCES) $(CLI_EXECUTABLE)

$(CLI_EXECUTABLE): $(CLI_OBJECTS) 
	$(CC) $(LDFLAGS) $(CLI_OBJECTS) -lpthread -o $(CLI_EXECUTABLE) -ldl

.c.o:
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

clean:
	rm *o icomm_rawdata_util test.log



