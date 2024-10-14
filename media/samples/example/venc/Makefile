# Rockchip sample Makefile
LD_FLAGS += -L$(COMM_DIR)/lib -lsample_comm
LD_FLAGS += $(RK_MEDIA_OPTS) -L$(RK_MEDIA_OUTPUT)/lib -lrockchip_mpp -lrkaiq -lrtsp
ifeq ($(RK_MEDIA_CHIP), rv1106)
LD_FLAGS += -lrockit_full
else
LD_FLAGS += -lrockit
endif

all:
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_vi_vpss_osd_venc.c -o sample_vi_vpss_osd_venc $(LD_FLAGS)
ifeq ($(HAVE_AVS), y)
	@$(SAMPLE_CC) $(SAMPLE_CFLAGS) sample_multi_vi_avs_osd_venc.c -o sample_multi_vi_avs_osd_venc $(LD_FLAGS)
endif
install:
	@mv -f sample_vi_vpss_osd_venc $(SAMPLE_OUT_DIR)/bin
ifeq ($(HAVE_AVS), y)
	@mv -f sample_multi_vi_avs_osd_venc $(SAMPLE_OUT_DIR)/bin
endif
clean:
	@echo "clean venc"
