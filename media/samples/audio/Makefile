# Rockchip sample Makefile

all:
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_ai.c -o sample_ai $(LD_FLAGS)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_ai_aenc.c -o sample_ai_aenc $(LD_FLAGS)

install:
	@mv -f sample_ai sample_ai_aenc $(SAMPLE_OUT_DIR)/bin

clean:
	@rm -f sample_ai sample_ai_aenc
