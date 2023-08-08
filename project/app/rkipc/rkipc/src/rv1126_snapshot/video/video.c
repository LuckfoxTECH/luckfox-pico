// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//
// snapshot flow chart:
//
// real-time stream : vi_0 → rga_0 → vo_0(primary)
//                         → venc_0(h264) → real.h264
//
// offline frame :   vi_1  → venc_1(h264) → fake.h264
//                         → venc_2(jpeg) → linear amplification(TODO) → jpeg file
//					       → rga_1 → vo(overlay)
//
// output.h264 = fake.h264 + real.h264

#include "video.h"
#include "common.h"
#include "fake_isp.h"
#include "isp.h"
#include "timestamp.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "video.c"

#define VIDEO_PIPE_0 0
#define VIDEO_PIPE_1 1
#define FAKE_FRAME_COUNT 5
#define REAL_FRAME_COUNT 100
#define FAKE_H264_FILE_PATH "/tmp/fake.h264"
#define REAL_H264_FILE_PATH "/tmp/real.h264"
#define OUT_H264_FILE_PATH "/tmp/out.h264"

extern int rkipc_dump_num;
extern char *rkipc_iq_file_path_;
extern char *rkipc_subdev_;
extern char *rkipc_device_;

static rk_aiq_sys_ctx_t *g_fake_aiq_ctx = NULL;
static int g_video_run_ = 1;
static int index_raw = 0;
static int real_index = 0;
static int jpeg_index = 0;
static int real_h264_file_over = 0;
static int fake_rga_over = 0;

int fake_width = 2688;
int fake_height = 1520;
int video_width = 2688;
int video_height = 1520;
int disp_width = 720;
int disp_height = 1280;
int disp1_width = 360;
int disp1_height = 640;

static uint8_t *rkraws[FAKE_FRAME_COUNT];
struct mcu_rkaiq_rkraw mcu_rkraws[FAKE_FRAME_COUNT];
FILE *fake_h264_file;
FILE *real_h264_file;
FILE *out_h264_file;

MPP_CHN_S stSrcChn = {0};
MPP_CHN_S stDestChn = {0};
MPP_CHN_S stEncChn;
VENC_RECV_PIC_PARAM_S stRecvParam;
static char timestamp_char[3] = {2, 0, 0};
VENC_CHN_ATTR_S venc_chn_attr;

void rga_draw_to_vo_cb(MEDIA_BUFFER mb) {
	LOG_DEBUG("fake_rga_over is %d\n", fake_rga_over);
	if (fake_rga_over)
		return;
	LOG_INFO("Get RGA packet, ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
	         "timestamp:%lld\n",
	         RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb), RK_MPI_MB_GetSize(mb),
	         RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));

#if 0 // TODO, STM32 no timestamp
	read_timestamp_from_uart(&timestamp_char);
	draw_buffer(RK_MPI_MB_GetPtr(timestamp_mb), &timestamp_char);

	// Use rga's API to do rotate and overlay
	MEDIA_BUFFER timestamp_mb = NULL;
	MB_IMAGE_INFO_S stImageInfo = {160, 32, 160, 32, IMAGE_TYPE_ARGB8888};
	timestamp_mb = RK_MPI_MB_CreateImageBuffer(&stImageInfo, RK_TRUE, 0);
	if (!timestamp_mb) {
		LOG_INFO("ERROR: RK_MPI_MB_CreateImageBuffer get null buffer!\n");
		return;
	}

#if 0
  // rga_buffer_t timestamp_rga;
  // rga_buffer_t dst_rga;
  // timestamp_rga = wrapbuffer_fd(RK_MPI_MB_GetFD(timestamp_mb), 160, 32,
  //                               RK_FORMAT_BGRA_8888);
  // dst_rga = wrapbuffer_fd(RK_MPI_MB_GetFD(mb), disp1_width, disp1_height,
  //                         RK_FORMAT_BGRA_8888);
  // rga_buffer_t pat = {0};
  // im_rect src_rect = {0, 0, 160, 32};
  // im_rect dst_rect = {0, 0, 32, 160};
  // im_rect pat_rect = {0};
  // ret = improcess(timestamp_rga, dst_rga, pat, src_rect, dst_rect, pat_rect,
  //                 IM_HAL_TRANSFORM_ROT_270 | IM_ALPHA_BLEND_SRC_OVER);
  // LOG_INFO("improcess ret is %d, reason: %s\n", ret, imStrError(ret));
#else
	for (int i = 0; i < 32; i++) {
		memcpy(RK_MPI_MB_GetPtr(mb) + i * disp1_width * 4,
		       RK_MPI_MB_GetPtr(timestamp_mb) + i * 160 * 4, 160 * 4);
	}
#endif
#endif

	RK_MPI_SYS_SendMediaBuffer(RK_ID_VO, 1, mb);
	RK_MPI_MB_ReleaseBuffer(mb);
	fake_rga_over = 1;
}

void vi_0_packet_cb(MEDIA_BUFFER mb) {
	// LOG_INFO("Get NV12 packet:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
	//          "timestamp:%lld\n",
	//          RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb), RK_MPI_MB_GetSize(mb),
	//          RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));
	RK_MPI_SYS_SendMediaBuffer(RK_ID_RGA, 0, mb);
	RK_MPI_SYS_SendMediaBuffer(RK_ID_VENC, 0, mb);
	RK_MPI_MB_ReleaseBuffer(mb);
}

void vi_1_packet_cb(MEDIA_BUFFER mb) {
	LOG_INFO("Get NV12 packet:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
	         "timestamp:%lld\n",
	         RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb), RK_MPI_MB_GetSize(mb),
	         RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));
	RK_MPI_SYS_SendMediaBuffer(RK_ID_RGA, 1, mb);
	RK_MPI_SYS_SendMediaBuffer(RK_ID_VENC, 1, mb);
	RK_MPI_SYS_SendMediaBuffer(RK_ID_VENC, 2, mb);
	RK_MPI_MB_ReleaseBuffer(mb);
}

void venc_0_packet_cb(MEDIA_BUFFER mb) {
	// LOG_INFO("Get H264 packet:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
	// 			"timestamp:%lld\n",
	// 			RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb), RK_MPI_MB_GetSize(mb),
	// 			RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));
	if (real_h264_file && real_index < REAL_FRAME_COUNT) {
		fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), real_h264_file);
		real_index++;
		if (real_index >= REAL_FRAME_COUNT) {
			LOG_INFO("real_index >= REAL_FRAME_COUNT\n");
			real_h264_file_over = 1;
		}
	}
	RK_MPI_MB_ReleaseBuffer(mb);
}

void venc_1_packet_cb(MEDIA_BUFFER mb) {
	LOG_INFO("RK_MPI_MB_GetFlag(mb) is %d\n", RK_MPI_MB_GetFlag(mb));
	LOG_INFO("Get H264 packet:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
	         "timestamp:%lld\n",
	         RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb), RK_MPI_MB_GetSize(mb),
	         RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));
	if (fake_h264_file) {
		fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), fake_h264_file);
	}
	// RK_MPI_VENC_RequestIDR(1, true); // all i frame
	RK_MPI_MB_ReleaseBuffer(mb);
}

void venc_2_packet_cb(MEDIA_BUFFER mb) {
	unsigned int random;
	LOG_INFO("Get JPEG packet[%d]:ptr:%p, fd:%d, size:%zu, mode:%d, channel:%d, "
	         "timestamp:%lld\n",
	         jpeg_index, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetFD(mb), RK_MPI_MB_GetSize(mb),
	         RK_MPI_MB_GetModeID(mb), RK_MPI_MB_GetChannelID(mb), RK_MPI_MB_GetTimestamp(mb));

	char jpeg_path[64];
	getrandom(&random, sizeof(random), GRND_NONBLOCK);
	sprintf(jpeg_path, "/tmp/%08x-%d.jpeg", random, jpeg_index);
	jpeg_index++;
	FILE *file = fopen(jpeg_path, "w");
	LOG_INFO("write jpeg start, jpeg_path=%s\n", jpeg_path);
	if (file) {
		fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), file);
		LOG_INFO("write jpeg over\n");
		fclose(file);
	}
	RK_MPI_MB_ReleaseBuffer(mb);
}

void release_buffer_hunt(void *addr) { LOG_INFO(">>>>release buffer called: addr=%p\n", addr); }

int rkipc_pipe_0_init() {
	// real-time stream
	int ret;
	rk_isp_init(0, rkipc_iq_file_path_);

	LOG_INFO("Create vi[0] \n");
	VI_CHN_ATTR_S vi_chn_attr;
	vi_chn_attr.pcVideoNode = "rkispp_scale0";
	vi_chn_attr.u32BufCnt = 6;
	vi_chn_attr.u32Width = video_width;
	vi_chn_attr.u32Height = video_height;
	vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12;
	vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
	ret = RK_MPI_VI_SetChnAttr(0, 0, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(0, 0);
	if (ret) {
		LOG_INFO("#############Create vi[0] failed! ret=%d\n", ret);
		return -1;
	}
	MPP_CHN_S stViChn;
	stViChn.enModId = RK_ID_VI;
	stViChn.s32DevId = 0;
	stViChn.s32ChnId = 0;
	ret = RK_MPI_SYS_RegisterOutCb(&stViChn, vi_0_packet_cb);
	ret = RK_MPI_VI_StartStream(0, 0);

	LOG_INFO("Create rga[0]  ret=%d\n", ret);
	RGA_ATTR_S stRgaAttr;
	memset(&stRgaAttr, 0, sizeof(stRgaAttr));
	stRgaAttr.bEnBufPool = RK_TRUE;
	stRgaAttr.u16BufPoolCnt = 3;
	stRgaAttr.u16Rotaion = 270;
	stRgaAttr.stImgIn.u32X = 0;
	stRgaAttr.stImgIn.u32Y = 0;
	stRgaAttr.stImgIn.imgType = IMAGE_TYPE_NV12;
	stRgaAttr.stImgIn.u32Width = video_width;
	stRgaAttr.stImgIn.u32Height = video_height;
	stRgaAttr.stImgIn.u32HorStride = video_width;
	stRgaAttr.stImgIn.u32VirStride = video_height;
	stRgaAttr.stImgOut.u32X = 0;
	stRgaAttr.stImgOut.u32Y = 0;
	stRgaAttr.stImgOut.imgType = IMAGE_TYPE_RGB888;
	stRgaAttr.stImgOut.u32Width = disp_width;
	stRgaAttr.stImgOut.u32Height = disp_height;
	stRgaAttr.stImgOut.u32HorStride = disp_width;
	stRgaAttr.stImgOut.u32VirStride = disp_height;
	ret = RK_MPI_RGA_CreateChn(0, &stRgaAttr);
	if (ret) {
		LOG_INFO("###############Create rga[0] falied! ret=%d\n", ret);
		return -1;
	}

	LOG_INFO("Create vo[0]\n");
	VO_CHN_ATTR_S stVoAttr = {0};
	// VO[0] for primary plane
	memset(&stVoAttr, 0, sizeof(stVoAttr));
	stVoAttr.pcDevNode = "/dev/dri/card0";
	stVoAttr.emPlaneType = VO_PLANE_PRIMARY;
	stVoAttr.enImgType = IMAGE_TYPE_RGB888;
	stVoAttr.u16Zpos = 0;
	stVoAttr.stDispRect.s32X = 0;
	stVoAttr.stDispRect.s32Y = 0;
	stVoAttr.stDispRect.u32Width = disp_width;
	stVoAttr.stDispRect.u32Height = disp_height;
	ret = RK_MPI_VO_CreateChn(0, &stVoAttr);
	if (ret) {
		LOG_INFO("###############Create vo[0] failed! ret=%d\n", ret);
		return -1;
	}

	// VENC 0 h264
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264;
	venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
	venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = 30;
	venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = video_width * video_height * 2;
	// frame rate: in 30/1, out 30/1.
	venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 1;
	venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = 30;
	venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 1;
	venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = 30;
	venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomWidth = video_width;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomHeight = video_height;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirWidth = video_width;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirHeight = video_height;
	venc_chn_attr.stVencAttr.enRotation = VENC_ROTATION_0;
	ret = RK_MPI_VENC_CreateChn(0, &venc_chn_attr);
	if (ret) {
		LOG_INFO("Create Venc 0 failed! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Create callback[0]  ret=%d\n", ret);
	stEncChn.enModId = RK_ID_VENC;
	stEncChn.s32ChnId = 0;
	ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, venc_0_packet_cb);
	if (ret) {
		LOG_INFO("Create callback[0] failed! ret=%d\n", ret);
		return -1;
	}

	LOG_INFO("Bind RGA[0] to VO[0]....\n");
	stSrcChn.enModId = RK_ID_RGA;
	stSrcChn.s32ChnId = 0;
	stDestChn.enModId = RK_ID_VO;
	stDestChn.s32ChnId = 0;
	ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (ret) {
		LOG_INFO("###############Bind rga[0] to vo[0] failed! ret=%d\n", ret);
		return -1;
	}

	return 0;
}

int rkipc_pipe_0_deinit() {
	// real-time stream
	int ret;
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32ChnId = 0;
	stDestChn.enModId = RK_ID_RGA;
	stDestChn.s32ChnId = 0;
	ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (ret) {
		LOG_INFO("UnBind vi[0] to rga[0] failed! ret=%d\n", ret);
		return -1;
	}

	stSrcChn.enModId = RK_ID_RGA;
	stSrcChn.s32ChnId = 0;
	stDestChn.enModId = RK_ID_VO;
	stDestChn.s32ChnId = 0;
	ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (ret) {
		LOG_INFO("UnBind rga[0] to vo[0] failed! ret=%d\n", ret);
		return -1;
	}

	RK_MPI_VO_DestroyChn(0);
	RK_MPI_RGA_DestroyChn(0);
	RK_MPI_VI_DisableChn(0, 0);
	rk_isp_deinit(0);

	return 0;
}

int rkipc_pipe_1_init() {
	// offline frame
	int ret;
	// offline frame init
	LOG_INFO("rkipc_subdev_ =%s.\n", rkipc_subdev_);
	parse_mcu_rkraws(rkipc_subdev_, mcu_rkraws);
	alloc_rkraws(rkraws);
	// if (rkipc_subdev_)
	make_rkraws(mcu_rkraws, rkraws);

	rk_aiq_raw_prop_t prop;
	prop.format = RK_PIX_FMT_SBGGR10;
	prop.frame_width = fake_width;
	prop.frame_height = fake_height;
	prop.rawbuf_type = RK_AIQ_RAW_FD;

	g_fake_aiq_ctx =
	    aiq_fake_init(RK_AIQ_WORKING_MODE_NORMAL, rkipc_iq_file_path_, prop, release_buffer_hunt);
	// offline frame init over

	// memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	VI_CHN_ATTR_S vi_chn_attr1;
	vi_chn_attr1.pcVideoNode = rkipc_device_;
	vi_chn_attr1.u32BufCnt = 6;
	vi_chn_attr1.u32Width = fake_width;
	vi_chn_attr1.u32Height = fake_height;
	vi_chn_attr1.enPixFmt = IMAGE_TYPE_NV12;
	vi_chn_attr1.enWorkMode = VI_WORK_MODE_NORMAL;
	ret = RK_MPI_VI_SetChnAttr(1, 1, &vi_chn_attr1);
	ret |= RK_MPI_VI_EnableChn(1, 1);
	if (ret) {
		LOG_INFO("Create vi[1] failed! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Create vi[1] success\n");
	MPP_CHN_S stViChn;
	stViChn.enModId = RK_ID_VI;
	stViChn.s32DevId = 1;
	stViChn.s32ChnId = 1;
	ret = RK_MPI_SYS_RegisterOutCb(&stViChn, vi_1_packet_cb);
	if (ret) {
		LOG_INFO("Register Output callback vi[1] failed! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Register Output callback vi[1] success\n");
	ret = RK_MPI_VI_StartStream(1, 1);

	// VENC 1 h264
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264;
	venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
	venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = 30;
	venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = fake_width * fake_height * 2;
	// frame rate: in 30/1, out 30/1.
	venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = 1;
	venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = 30;
	venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = 1;
	venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = 30;
	venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
	venc_chn_attr.stVencAttr.u32PicWidth = fake_width;
	venc_chn_attr.stVencAttr.u32PicHeight = fake_height;
	venc_chn_attr.stVencAttr.u32VirWidth = fake_width;
	venc_chn_attr.stVencAttr.u32VirHeight = fake_height;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomWidth = fake_width;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomHeight = fake_height;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirWidth = fake_width;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirHeight = fake_height;
	venc_chn_attr.stVencAttr.enRotation = VENC_ROTATION_0;
	ret = RK_MPI_VENC_CreateChn(1, &venc_chn_attr);
	if (ret) {
		LOG_INFO("Create Venc 1 failed! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Create callback[1]  ret=%d\n", ret);
	stEncChn.enModId = RK_ID_VENC;
	stEncChn.s32ChnId = 1;
	ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, venc_1_packet_cb);
	if (ret) {
		LOG_INFO("Create callback[1] failed! ret=%d\n", ret);
		return -1;
	}

	// VENC 2 jpeg
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_JPEG;
	venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
	venc_chn_attr.stVencAttr.u32PicWidth = fake_width;
	venc_chn_attr.stVencAttr.u32PicHeight = fake_height;
	venc_chn_attr.stVencAttr.u32VirWidth = fake_width;
	venc_chn_attr.stVencAttr.u32VirHeight = fake_height;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomWidth = fake_width;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomHeight = fake_height;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirWidth = fake_width;
	venc_chn_attr.stVencAttr.stAttrJpege.u32ZoomVirHeight = fake_height;
	venc_chn_attr.stVencAttr.enRotation = VENC_ROTATION_0;
	ret = RK_MPI_VENC_CreateChn(2, &venc_chn_attr);
	if (ret) {
		LOG_INFO("Create Venc 2 failed! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Create callback[2]  ret=%d\n", ret);
	stEncChn.enModId = RK_ID_VENC;
	stEncChn.s32ChnId = 2;
	ret = RK_MPI_SYS_RegisterOutCb(&stEncChn, venc_2_packet_cb);
	if (ret) {
		LOG_INFO("Create callback[2] failed! ret=%d\n", ret);
		return -1;
	}

	stRecvParam.s32RecvPicNum = 0;
	RK_MPI_VENC_StartRecvFrame(1, &stRecvParam);
	RK_MPI_VENC_StartRecvFrame(2, &stRecvParam);

	// rga1 for overlay plane
	RGA_ATTR_S stRgaAttr1;
	LOG_INFO("Create rga[1]  ret=%d\n", ret);
	stRgaAttr1.bEnBufPool = RK_TRUE;
	stRgaAttr1.u16BufPoolCnt = 2;
	stRgaAttr1.u16Rotaion = 90;
	stRgaAttr1.stImgIn.u32X = 0;
	stRgaAttr1.stImgIn.u32Y = 0;
	stRgaAttr1.stImgIn.imgType = IMAGE_TYPE_NV12;
	stRgaAttr1.stImgIn.u32Width = fake_width;
	stRgaAttr1.stImgIn.u32Height = fake_height;
	stRgaAttr1.stImgIn.u32HorStride = fake_width;
	stRgaAttr1.stImgIn.u32VirStride = fake_height;
	stRgaAttr1.stImgOut.u32X = 0;
	stRgaAttr1.stImgOut.u32Y = 0;
	stRgaAttr1.stImgOut.imgType = IMAGE_TYPE_ARGB8888;
	stRgaAttr1.stImgOut.u32Width = disp1_width;
	stRgaAttr1.stImgOut.u32Height = disp1_height;
	stRgaAttr1.stImgOut.u32HorStride = disp1_width;
	stRgaAttr1.stImgOut.u32VirStride = disp1_height;
	ret = RK_MPI_RGA_CreateChn(1, &stRgaAttr1);
	if (ret) {
		LOG_INFO("###########Create rga[1] falied! ret=%d\n", ret);
		return -1;
	}

	MPP_CHN_S stRgaChn;
	stRgaChn.enModId = RK_ID_RGA;
	stRgaChn.s32ChnId = 1;
	ret = RK_MPI_SYS_RegisterOutCb(&stRgaChn, rga_draw_to_vo_cb);
	if (ret) {
		LOG_INFO("###############Register Output callback failed! ret=%d\n", ret);
		return -1;
	}

	// VO[1] for overlay plane
	VO_CHN_ATTR_S stVoAttr1 = {0};
	LOG_INFO("Create vo[1]\n");
	memset(&stVoAttr1, 0, sizeof(stVoAttr1));
	stVoAttr1.pcDevNode = "/dev/dri/card0";
	stVoAttr1.emPlaneType = VO_PLANE_OVERLAY;
	stVoAttr1.enImgType = IMAGE_TYPE_ARGB8888;
	stVoAttr1.u16Zpos = 1;
	stVoAttr1.stImgRect.s32X = 0;
	stVoAttr1.stImgRect.s32Y = 0;
	stVoAttr1.stImgRect.u32Width = disp1_width;
	stVoAttr1.stImgRect.u32Height = disp1_height;
	stVoAttr1.stDispRect.s32X = 0;
	stVoAttr1.stDispRect.s32Y = 0;
	stVoAttr1.stDispRect.u32Width = disp1_width;
	stVoAttr1.stDispRect.u32Height = disp1_height;
	ret = RK_MPI_VO_CreateChn(1, &stVoAttr1);
	if (ret) {
		LOG_INFO("#############Create vo[1] failed! ret=%d\n", ret);
		return -1;
	}

	return 0;
}

int rkipc_pipe_1_deinit() {
	// offline frame
	int ret;
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32ChnId = 0;
	ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (ret) {
		LOG_INFO("UnBind vi[1] to venc[0] failed! ret=%d\n", ret);
		return -1;
	}

	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_RGA;
	stDestChn.s32ChnId = 1;
	ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (ret) {
		LOG_INFO("UnBind vi[1] to rga[1] failed! ret=%d\n", ret);
		return -1;
	}

	RK_MPI_VO_DestroyChn(1);
	// offline frame free memory
	free_rkraws(rkraws);
	free_rkisp_reserve_mem(rkipc_subdev_);
	rk_aiq_uapi_sysctl_stop(g_fake_aiq_ctx, false);
	LOG_INFO("rk_aiq_uapi_sysctl_deinit enter\n");
	rk_aiq_uapi_sysctl_deinit(g_fake_aiq_ctx);
	LOG_INFO("rk_aiq_uapi_sysctl_deinit exit\n");
	g_fake_aiq_ctx = NULL;

	return 0;
}

int rk_video_init() {
	LOG_INFO("%s\n", __func__);
	int ret = 0;
	fake_h264_file = fopen(FAKE_H264_FILE_PATH, "wb+");
	real_h264_file = fopen(REAL_H264_FILE_PATH, "wb+");
	ret = RK_MPI_SYS_Init();
	ret |= rkipc_pipe_0_init();
	ret |= rkipc_pipe_1_init();

	stRecvParam.s32RecvPicNum = FAKE_FRAME_COUNT;
	ret = RK_MPI_VENC_StartRecvFrame(1, &stRecvParam);
	ret |= RK_MPI_VENC_StartRecvFrame(2, &stRecvParam);
	if (ret)
		LOG_INFO("RK_MPI_VENC_StartRecvFrame failed!\n");
	else
		LOG_INFO("RK_MPI_VENC_StartRecvFrame success!\n");

	// The algorithm requires three frames to be input as reference frames,
	// if you don’t lose these three frames,
	// the first three frames will be swallowed and vi won’t get it.
	for (int p = 0; p < 3; p++) {
		ret = rk_aiq_uapi_sysctl_enqueueRkRawBuf(g_fake_aiq_ctx, (void *)rkraws[0], false);
		LOG_INFO(">>>>>>>>> #### enqueue reference raw. ret = %d.\n", ret);
	}
	for (; index_raw < rkipc_dump_num; index_raw++) {
		ret = rk_aiq_uapi_sysctl_enqueueRkRawBuf(g_fake_aiq_ctx, (void *)rkraws[index_raw], false);
		LOG_INFO("index_raw is %d, >>>>>>>>> enqueue raw. ret = %d.\n", index_raw, ret);
		// usleep(33 * 10000);
	}

	while (!real_h264_file_over || index_raw != FAKE_FRAME_COUNT) {
		usleep(1000 * 1000);
	}
	LOG_INFO("real_h264_file_over && index_raw == FAKE_FRAME_COUNT\n");

	long int fake_size = ftell(fake_h264_file);
	long int real_size = ftell(real_h264_file);
	fseek(fake_h264_file, 0, SEEK_SET);
	fseek(real_h264_file, 0, SEEK_SET);
	LOG_INFO("fake_size is %ld, real_size is %ld\n", fake_size, real_size);

	out_h264_file = fopen(OUT_H264_FILE_PATH, "wb");
	char ch;
	while (ftell(fake_h264_file) < fake_size) {
		ch = fgetc(fake_h264_file);
		fprintf(out_h264_file, "%c", ch);
	}
	while (ftell(real_h264_file) < real_size) {
		ch = fgetc(real_h264_file);
		fprintf(out_h264_file, "%c", ch);
	}
	fclose(out_h264_file);

	return ret;
}

int rk_video_deinit() {
	int ret = 0;
	LOG_INFO("%s\n", __func__);
	g_video_run_ = 0;
	fclose(fake_h264_file);
	fclose(real_h264_file);
	ret |= rkipc_pipe_0_deinit();
	ret |= rkipc_pipe_1_deinit();

	return ret;
}
