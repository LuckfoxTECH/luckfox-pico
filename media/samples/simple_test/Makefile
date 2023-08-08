# Rockchip sample Makefile

SIMPLE_FAST_CLIENT_LD_FLAGS += $(RK_MEDIA_OPTS) -L$(RK_MEDIA_OUTPUT)/lib -Wl,-Bstatic -lpthread -lrockit \
	-lrockchip_mpp -lrksysutils -lsmartIr \
	-lrkaudio_detect -laec_bf_process \
	-lrga -Wl,-Bdynamic \
	-L$(SAMPLE_OUT_DIR)/ -lrkaiq -lstdc++ -lm
SIMPLE_FAST_CLIENT_LD_FLAGS += -L$(SAMPLE_SRC_DIR)/../lib/$(RK_MEDIA_CROSS) -Wl,-Bstatic -lrtsp -Wl,-Bdynamic -Wl,--gc-sections -Wl,--as-needed


SOURCES_C     = $(wildcard *.c)
ifneq ($(RK_MEDIA_CHIP), rv1106)
SOURCES_C     = $(filter-out simple_vi_rockiva.c simple_dual_camera.c simple_fast_client.c, $(wildcard *.c))
endif

OBJECTS       = $(basename $(SOURCES_C))

$(warning "here add the debug info $(OBJECTS)")
all: $(OBJECTS)

simple_adec_ao: simple_adec_ao.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_ai_aenc: simple_ai_aenc.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_ai: simple_ai.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_ao: simple_ao.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_fast_client: simple_fast_client.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(SIMPLE_FAST_CLIENT_LD_FLAGS)

simple_venc_jpeg: simple_venc_jpeg.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_venc_osd: simple_venc_osd.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_get_frame: simple_vi_get_frame.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_ivs: simple_vi_ivs.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_rockiva: simple_vi_rockiva.c
	@make -C $(RK_MEDIA_TOP_DIR)/iva
	$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS) -DROCKIVA -lrockiva -lrknnmrt -I$(RK_MEDIA_OUTPUT)/include/rockiva

simple_vi_venc: simple_vi_venc.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_venc_change_resolution: simple_vi_venc_change_resolution.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_venc_rtsp: simple_vi_venc_rtsp.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_venc_wrap: simple_vi_venc_wrap.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_vpss_venc: simple_vi_vpss_venc.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_rkaiq: simple_vi_rkaiq.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_tde: simple_vi_tde.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_venc_combo: simple_vi_venc_combo.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_vi_eptz_test: simple_vi_eptz_test.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

simple_dual_camera: simple_dual_camera.c
	@$(SAMPLE_CC) $^ -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS) -DROCKIVA -lrockiva -lrknnmrt -I$(RK_MEDIA_OUTPUT)/include/rockiva

install:
	@mv -f $(OBJECTS) $(SAMPLE_OUT_DIR)/bin

clean:
	@echo "clean simple"
