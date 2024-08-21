# Rockchip sysutils Makefile

SRCS := $(wildcard *.c)
OBJS  := $(SRCS:%.c=%.o)
target_lib_so := librksysutils.so
target_lib_static := librksysutils.a

all: $(OBJS) build-so build-static
	@mkdir -p $(SYSUTILS_OUT_DIR)/lib
	@cp -f $(target_lib_so) $(target_lib_static) $(SYSUTILS_OUT_DIR)/lib

$(OBJS): $(SRCS)
	$(CC) -c $(filter $(basename $@)%, $^) $(LD_FLAGS) $(CFLAGS)

build-so:
	$(CC) $(CFLAGS) -shared $(OBJS) -o $(target_lib_so)

build-static:
	$(AR) -cvr $(target_lib_static) $(OBJS)

clean:
	@rm -f $(OBJS)
