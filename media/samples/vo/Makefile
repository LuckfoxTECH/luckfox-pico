# Rockchip sample Makefile
SOURCES_C     = $(wildcard *.c)
OBJECTS       = $(basename $(SOURCES_C))
all: $(OBJECTS)

sample_vi_vo: sample_vi_vo.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)
install:
	@mv -f $(OBJECTS) $(SAMPLE_OUT_DIR)/bin
clean:
	@echo "clean sample/vo"
