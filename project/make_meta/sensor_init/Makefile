
target_cflag=-I../include
ifneq ($(main_sensor),)
target=$(main_sensor)_init.c
else
target=sc230ai_init.c
endif
target_bin=$(patsubst %.c,%,$(target)) 
target_bin_file=init_sensor.bin

ifneq ($(secondary_sensor),)
secondary_target=$(secondary_sensor)_init.c
secondary_target_bin=$(patsubst %.c,%,$(secondary_target)) 
secondary_target_bin_file=init_secondary_sensor.bin
secondary_build:= secondary_build_target
endif

all: $(secondary_build)
	gcc -m32 -O2 -Wall $(target_cflag) $(target) -o $(patsubst %.c,%,$(target))
	./$(target_bin)  $(target_bin_file)

secondary_build_target:
	gcc -m32 -O2 -Wall $(target_cflag) $(secondary_target) -o $(patsubst %.c,%,$(secondary_target))
	./$(secondary_target_bin)  $(secondary_target_bin_file)

clean:
	@rm -f $(target_bin)  $(target_bin_file) $(secondary_target_bin)  $(secondary_target_bin_file)
