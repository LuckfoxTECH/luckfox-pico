# Rockchip sample Makefile
SOURCES_C     = $(wildcard *.c)
OBJECTS       = $(basename $(SOURCES_C))
LD_FLAGS += -L$(COMM_DIR)/lib -lsample_comm
LD_FLAGS += $(RK_MEDIA_OPTS) -L$(RK_MEDIA_OUTPUT)/lib -lrkaiq
all: $(OBJECTS)

ifeq ($(RK_MEDIA_CHIP), rv1106)
LD_FLAGS += -lrockit_full
else
LD_FLAGS += -lrockit
endif

sample_vi_vo: sample_vi_vo.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)
install:
	@mv -f $(OBJECTS) $(SAMPLE_OUT_DIR)/bin
clean:
	@echo "clean sample/vo"
