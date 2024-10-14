# Rockchip simple Makefile
ifeq ($(MEDIA_PARAM), )
    MEDIA_PARAM:=../../Makefile.param
    include $(MEDIA_PARAM)
endif

SIMPLE_CC := $(RK_MEDIA_CROSS)-gcc

CURRENT_DIR := $(shell pwd)
SIMPLE_OUT_DIR :=$(CURRENT_DIR)/out

SIMPLE_PKG_CONF_OPTS += -DRKPLATFORM=ON

ifeq ($(RK_MEDIA_CHIP), rv1126)
SIMPLE_PKG_CONF_OPTS += -DCMAKE_SYSTEM_PROCESSOR=armv7l
SIMPLE_PKG_CONF_OPTS += -DARCH64=OFF
endif

ifeq ($(RK_MEDIA_CHIP), rv1106)
SIMPLE_PKG_CONF_OPTS += -DARCH64=OFF
endif

SIMPLE_OPTS += -Wl,-rpath-link,${RK_MEDIA_OUTPUT}/lib

INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq
ifeq ($(RK_MEDIA_CHIP), rv1126)
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/uAPI
SIMPLE_LD_FLAGS += -DUAPI
SIMPLE_LD_FLAGS += -DRV1126_RV1109
else
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/uAPI2
SIMPLE_LD_FLAGS += -DUAPI2
SIMPLE_LD_FLAGS += -DRV1106_RV1103
endif
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/common
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/xcore
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/algos
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/iq_parser
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/iq_parser_v2
INC_FLAGS += -I$(RK_MEDIA_OUTPUT)/include/rkaiq/smartIr

SIMPLE_CFLAGS += -g -Wall $(INC_FLAGS) $(SIMPLE_PKG_CONF_OPTS) $(RK_MEDIA_CROSS_CFLAGS)

SIMPLE_LD_FLAGS += $(SIMPLE_OPTS) -L$(RK_MEDIA_OUTPUT)/lib -L$(RK_MEDIA_OUTPUT)/root/usr/lib \
					-lpthread -lm -lrockit \
					-lrockchip_mpp -DRKAIQ \
					-lrga -lstdc++
SIMPLE_LD_FLAGS += -Wl,--gc-sections -Wl,--as-needed

SIMPLE_VO_LD_FLAGS += $(RK_MEDIA_OPTS) -L$(RK_MEDIA_OUTPUT)/lib -L$(RK_MEDIA_OUTPUT)/root/usr/lib -Wl,-Bdynamic \
						-lrockchip_mpp -lrga -ldrm -lpthread  -lstdc++
ifneq ($(RK_MEDIA_CHIP), rv1106)
SIMPLE_VO_LD_FLAGS += -lrockit
else
SIMPLE_VO_LD_FLAGS += -lrockit_full
endif

ifeq ($(RK_MEDIA_CHIP), rv1126)
SIMPLE_LD_FLAGS += -lasound
SIMPLE_VO_LD_FLAGS += -lasound
endif

SOURCES_C     = $(wildcard *.c)
ifneq ($(RK_MEDIA_CHIP), rv1106)
SOURCES_C     = $(filter-out simple_vi_get_frame_send_vo_rv1106.c simple_vi_bind_venc_change_resolution_rv1106.c simple_vi_bind_venc_rtsp_dev_chn_sharebuf_rv1106.c simple_vi_bind_venc_chn_sharebuf_rv1106.c simple_vi_bind_venc_rtsp_three_camera_rv1106.c simple_vi_bind_venc_wrap_rv1106 simple_vi_bind_venc_combo_rv1106, $(wildcard *.c))
endif

OBJECTS       = $(basename $(SOURCES_C))

$(warning "here add the debug info $(OBJECTS)")
all:
	@test -f $(RK_MEDIA_OUTPUT)/lib/librockit.so  || make -C $(RK_MEDIA_TOP_DIR) media_libs
	@make simple_build

simple_build: $(OBJECTS)
	@mkdir -p $(SIMPLE_OUT_DIR)/bin
	@mv -f $(OBJECTS) $(SIMPLE_OUT_DIR)/bin
	@cp simple_cmd.txt $(SIMPLE_OUT_DIR)/bin/
	$(call MAROC_COPY_PKG_TO_MEDIA_OUTPUT, $(RK_MEDIA_OUTPUT)/bin, $(SIMPLE_OUT_DIR)/bin)

simple_adec_bind_ao: simple_adec_bind_ao.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_ai_bind_aenc: simple_ai_bind_aenc.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_adec_bind_ao_external_decoder: simple_adec_bind_ao_external_decoder.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaudio -lrkdemuxer

simple_ai_bind_aenc_external_encoder: simple_ai_bind_aenc_external_encoder.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaudio

simple_ai_get_frame: simple_ai_get_frame.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_ao_send_frame: simple_ao_send_frame.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_bind_venc_jpeg: simple_vi_bind_venc_jpeg.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_bind_venc_osd: simple_vi_bind_venc_osd.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_get_frame: simple_vi_get_frame.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_get_frame_send_vo_rv1106: simple_vi_get_frame_send_vo_rv1106.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_VO_LD_FLAGS)

simple_vi_bind_ivs: simple_vi_bind_ivs.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_bind_venc: simple_vi_bind_venc.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_bind_venc_chn_sharebuf_rv1106: simple_vi_bind_venc_chn_sharebuf_rv1106.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_bind_venc_change_resolution_rv1106: simple_vi_bind_venc_change_resolution_rv1106.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_bind_venc_rtsp: simple_vi_bind_venc_rtsp.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaiq -lrtsp

simple_vi_bind_venc_rtsp_dev_chn_sharebuf_rv1106: simple_vi_bind_venc_rtsp_dev_chn_sharebuf_rv1106.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaiq -lrtsp

simple_vi_bind_venc_rtsp_three_camera_rv1106: simple_vi_bind_venc_rtsp_three_camera_rv1106.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaiq -lrtsp

simple_vi_bind_venc_svc_rtsp: simple_vi_bind_venc_svc_rtsp.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaiq -lrtsp

simple_vi_bind_venc_wrap_rv1106: simple_vi_bind_venc_wrap_rv1106.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_bind_vpss_bind_venc: simple_vi_bind_vpss_bind_venc.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS)

simple_vi_get_frame_rkaiq: simple_vi_get_frame_rkaiq.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaiq

simple_vi_get_frame_tde: simple_vi_get_frame_tde.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaiq

simple_vi_bind_venc_combo_rv1106: simple_vi_bind_venc_combo_rv1106.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaiq

simple_vi_bind_venc_rtsp_eptz: simple_vi_bind_venc_rtsp_eptz.c
	@$(SIMPLE_CC) $^ -o $@ $(SIMPLE_CFLAGS) $(SIMPLE_LD_FLAGS) -lrkaiq -lrtsp

clean:
	@echo "clean simple"
	@rm -rf $(SIMPLE_OUT_DIR)
