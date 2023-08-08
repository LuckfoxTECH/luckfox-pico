# Rockchip sample Makefile

all:
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_vi.c         -o sample_vi          $(LD_FLAGS)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_multi_vi.c    -o sample_multi_vi    $(LD_FLAGS)

install:
	@mv -f sample_vi sample_multi_vi $(SAMPLE_OUT_DIR)/bin

clean:
	@rm -f sample_vi
	@rm -f sample_multi_vi
