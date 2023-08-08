# Rockchip sample Makefile

all:
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_camera_stresstest.c -o sample_camera_stresstest $(LD_FLAGS)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_isp_stresstest.c -o sample_isp_stresstest $(LD_FLAGS)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_venc_stresstest.c -o sample_venc_stresstest $(LD_FLAGS)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_rgn_stresstest.c -o sample_rgn_stresstest $(LD_FLAGS)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_mulit_isp_stresstest.c -o sample_mulit_isp_stresstest $(LD_FLAGS)

install:
	@mv -f sample_camera_stresstest $(SAMPLE_OUT_DIR)/bin
	@mv -f sample_isp_stresstest $(SAMPLE_OUT_DIR)/bin
	@mv -f sample_venc_stresstest $(SAMPLE_OUT_DIR)/bin
	@mv -f sample_rgn_stresstest $(SAMPLE_OUT_DIR)/bin
	@mv -f sample_mulit_isp_stresstest $(SAMPLE_OUT_DIR)/bin

clean:
	@echo "clean test"
