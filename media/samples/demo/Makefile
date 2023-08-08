# Rockchip sample Makefile

all:
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_demo_vi_venc.c -o sample_demo_vi_venc $(LD_FLAGS)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_demo_multi_camera_eptz.c -o sample_demo_multi_camera_eptz $(LD_FLAGS)

install:
	@mv -f sample_demo_vi_venc $(SAMPLE_OUT_DIR)/bin
	@mv -f sample_demo_multi_camera_eptz $(SAMPLE_OUT_DIR)/bin

clean:
	@echo "clean demo"
