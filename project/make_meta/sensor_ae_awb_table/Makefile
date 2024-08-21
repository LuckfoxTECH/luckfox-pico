		  
target_cflag=-I../include
ifneq ($(SENSOR_TARGET),)
target=$(SENSOR_TARGET)_2a.c
else
target=sc230ai_2a.c
endif
target_bin=$(patsubst %.c,%,$(target))_table
target_bin_file=ae_awb_table.bin

all:
	echo "#include \"$(target)\"" > config.h 
	gcc -m32 -O2 -Wall $(target_cflag) main.c -o $(target_bin)
	./$(target_bin) $(target_bin_file)

clean:
	@rm -f $(target_bin) $(target_bin_file)

distclean: clean
