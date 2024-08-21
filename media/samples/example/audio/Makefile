# Rockchip sample Makefile

LD_FLAGS += -L$(COMM_DIR)/lib -lsample_comm
LD_FLAGS += $(RK_MEDIA_OPTS) -L$(RK_MEDIA_OUTPUT)/lib

ifeq ($(RK_MEDIA_CHIP), rv1106)
LD_FLAGS += -lrockit_full
else
LD_FLAGS += -lrockit
endif

all:
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_ai.c -o sample_ai $(LD_FLAGS)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_ai_aenc.c -o sample_ai_aenc $(LD_FLAGS)

install:
	@mv -f sample_ai sample_ai_aenc $(SAMPLE_OUT_DIR)/bin

clean:
	@rm -f sample_ai sample_ai_aenc
