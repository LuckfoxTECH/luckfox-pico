// Copyright 2023 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "video.h"
#include "common.h"
#include "isp.h"
#include "osd.h"
#include "region_clip.h"
#include "rockiva.h"
#include "roi.h"
#include "rtmp.h"
#include "rtsp.h"
#include "storage.h"
#include <pthread.h>
#include <rk_aiq_user_api2_imgproc.h>
#include <rk_aiq_user_api2_sysctl.h>
#include <rk_comm_tde.h>
#include <rk_mpi_cal.h>
#include <rk_mpi_ivs.h>
#include <rk_mpi_mb.h>
#include <rk_mpi_mmz.h>
#include <rk_mpi_tde.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "video.c"

#ifdef TRACE_BEGIN
#undef TRACE_BEGIN
#endif
#define TRACE_BEGIN() LOG_INFO("Enter\n")

#ifdef TRACE_END
#undef TRACE_END
#endif
#define TRACE_END() LOG_INFO("Exit\n")

#define VIDEO_PIPE_0 0
#define VIDEO_PIPE_1 1
#define VIDEO_PIPE_2 2
#define JPEG_VENC_CHN 3

#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"
#define RTSP_URL_2 "/live/2"
#define RTMP_URL_0 "rtmp://127.0.0.1:1935/live/mainstream"
#define RTMP_URL_1 "rtmp://127.0.0.1:1935/live/substream"
#define RTMP_URL_2 "rtmp://127.0.0.1:1935/live/thirdstream"

static int g_do_capture = 0;
static int g_enable_ivs, g_enable_jpeg, g_enable_venc_0, g_enable_venc_1, g_enable_venc_2,
    g_enable_npu, g_enable_aiisp;
static int g_video_run_ = 1;
static int g_pipe_id = 0;
static int g_dev_id = 0;

static const char *tmp_output_data_type = "H.264";
static const char *tmp_rc_mode;
static const char *tmp_h264_profile;
static const char *tmp_smart;
static const char *tmp_gop_mode;
static const char *tmp_rc_quality;

static int thread_stack_top = -1;
static pthread_t thread_id_stack[16];
static pthread_t venc_thread_0, venc_thread_1, venc_thread_2, iva_get_frame_thread_id,
    ivs_get_result_thread_id, jpeg_venc_thread_id, cycle_snapshot_thread_id = 0;

static pthread_cond_t g_capture_cond;
static pthread_mutex_t g_capture_mutex;

static MPP_CHN_S vi_chn, venc_chns[3], ivs_chn, vpss_0_chns[3], vpss_1_chns[1];

extern rk_aiq_sys_ctx_t *rkipc_aiq_get_ctx(int cam_id);

/* after rkaiq init */
static RK_S32 vpss_aiisp_callback(rk_ainr_param *pAinrParam, RK_VOID *pPrivateData) {
	static int last_enable = 0; // default disable
	if (pAinrParam == RK_NULL) {
		return RK_FAILURE;
	}
	memset(pAinrParam, 0, sizeof(rk_ainr_param));
	int ret = 0;
	int camera_id = rk_param_get_int("video.0:camera_id", 0);
	rk_aiq_sys_ctx_t *ctx = rkipc_aiq_get_ctx(camera_id);
	ret = rk_aiq_uapi2_sysctl_getAinrParams(ctx, pAinrParam);
	if (ret != 0) {
		LOG_ERROR("failed to get ainr parameters, ret = %#x\n", ret);
		return ret;
	}
	if (pAinrParam->enable != last_enable) {
		Uapi_ExpSwAttrV2_t stExpSwAttr;
		int src_den, src_num, dst_den, dst_num, fps, stream_id;
		char entry[128] = {'\0'};
		// Adjust ISP framerate first.
		ret = rk_aiq_user_api2_ae_getExpSwAttr(ctx, &stExpSwAttr);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("rk_aiq_user_api2_ae_getExpSwAttr failed %#X\n", ret);
			return ret;
		}
		if (pAinrParam->enable)
			fps = rk_param_get_int("video.source:aiisp_fps", 10);
		else
			fps = rk_param_get_int("isp.0.adjustment:fps", 25);
		stExpSwAttr.stAuto.stFrmRate.isFpsFix = true;
		stExpSwAttr.stAuto.stFrmRate.FpsValue = fps;
		ret = rk_aiq_user_api2_ae_setExpSwAttr(ctx, stExpSwAttr);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("rk_aiq_user_api2_ae_setExpSwAttr failed %#X\n", ret);
			return ret;
		}

		// Then adjust framerate of all venc channels.
		VENC_CHN_ATTR_S venc_chn_attr;
		for (stream_id = 0; stream_id != 3; ++stream_id) {
			if (pAinrParam->enable) {
				dst_num = src_num = fps;
				dst_den = src_den = 1;
			} else {
				snprintf(entry, 127, "video.%d:dst_frame_rate_den", stream_id);
				dst_den = rk_param_get_int(entry, 1);
				snprintf(entry, 127, "video.%d:dst_frame_rate_num", stream_id);
				dst_num = rk_param_get_int(entry, 25);
				snprintf(entry, 127, "video.%d:src_frame_rate_den", stream_id);
				src_den = rk_param_get_int(entry, 1);
				snprintf(entry, 127, "video.%d:src_frame_rate_num", stream_id);
				src_num = rk_param_get_int(entry, 25);
				if (src_num != fps || !src_den || !dst_den ||
				    (src_num / src_den) < (dst_num / dst_den))
					LOG_ERROR("Bad framerate settings! src %d/%d, dst %d/%d\n", src_num, src_den,
					          dst_num, dst_den);
			}

			memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
			RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
			if (venc_chn_attr.stVencAttr.enType == RK_VIDEO_ID_AVC) {
				if (venc_chn_attr.stRcAttr.enRcMode == VENC_RC_MODE_H264CBR) {
					venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen = src_den;
					venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum = src_num;
					venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = dst_den;
					venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = dst_num;
				} else {
					venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen = src_den;
					venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum = src_num;
					venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen = dst_den;
					venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum = dst_num;
				}
			} else if (venc_chn_attr.stVencAttr.enType == RK_VIDEO_ID_HEVC) {
				if (venc_chn_attr.stRcAttr.enRcMode == VENC_RC_MODE_H265CBR) {
					venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateDen = src_den;
					venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateNum = src_num;
					venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen = dst_den;
					venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum = dst_num;
				} else {
					venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen = src_den;
					venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum = src_num;
					venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen = dst_den;
					venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum = dst_num;
				}
			}
			ret = RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);
			if (ret != RK_SUCCESS)
				LOG_ERROR("stream %d set frame rate %d/%d failed!\n", stream_id, dst_num, dst_den);
			else
				LOG_DEBUG("stream %d set frame rate %d/%d success!\n", stream_id, dst_num, dst_den);
		}
		last_enable = pAinrParam->enable;
	}
	// LOG_INFO("enable:%d, gain:%f\n", pAinrParam->enable, pAinrParam->gain);

	return RK_SUCCESS;
}

static int rkipc_rtmp_init() {
	int ret = 0;
	ret |= rk_rtmp_init(0, RTMP_URL_0);
	ret |= rk_rtmp_init(1, RTMP_URL_1);
	ret |= rk_rtmp_init(2, RTMP_URL_2);

	return ret;
}

static int rkipc_rtmp_deinit() {
	int ret = 0;
	ret |= rk_rtmp_deinit(0);
	ret |= rk_rtmp_deinit(1);
	ret |= rk_rtmp_deinit(2);

	return ret;
}

static int rkipc_vi_dev_init() {
	TRACE_BEGIN();
	int ret = 0;
	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	memset(&stDevAttr, 0, sizeof(stDevAttr));
	memset(&stBindPipe, 0, sizeof(stBindPipe));
	// 0. get dev config status
	ret = RK_MPI_VI_GetDevAttr(g_dev_id, &stDevAttr);
	if (ret == RK_ERR_VI_NOT_CONFIG) {
		// 0-1.config dev
		ret = RK_MPI_VI_SetDevAttr(g_dev_id, &stDevAttr);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_MPI_VI_SetDevAttr %x\n", ret);
			return -1;
		}
	} else {
		LOG_ERROR("RK_MPI_VI_SetDevAttr already\n");
	}
	// 1.get dev enable status
	ret = RK_MPI_VI_GetDevIsEnable(g_dev_id);
	if (ret != RK_SUCCESS) {
		// 1-2.enable dev
		ret = RK_MPI_VI_EnableDev(g_dev_id);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_MPI_VI_EnableDev %x\n", ret);
			return -1;
		}
		// 1-3.bind dev/pipe
		stBindPipe.u32Num = g_pipe_id;
		stBindPipe.PipeId[0] = g_pipe_id;
		ret = RK_MPI_VI_SetDevBindPipe(g_dev_id, &stBindPipe);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_MPI_VI_SetDevBindPipe %x\n", ret);
			return -1;
		}
	} else {
		LOG_ERROR("RK_MPI_VI_EnableDev already\n");
	}
	TRACE_END();

	return 0;
}

static int rkipc_vi_dev_deinit() {
	int ret = 0;
	TRACE_BEGIN();
	ret = RK_MPI_VI_DisableDev(g_dev_id);
	if (ret != RK_SUCCESS)
		LOG_ERROR("RK_MPI_VI_DisableDev failed because %#X\n", ret);
	TRACE_END();

	return ret;
}

static int rkipc_vi_chn_init() {
	int ret;
	// Always use full size output VI.
	int video_width = rk_param_get_int("video.0:max_width", 2560);
	int video_height = rk_param_get_int("video.0:max_height", 1440);
	VI_CHN_ATTR_S vi_chn_attr;

	// VI CHN -> VPSS AIISP CHN
	TRACE_BEGIN();
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = rk_param_get_int("video.0:vi_buffer_count", 2);
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;

	ret = RK_MPI_VI_SetChnAttr(g_pipe_id, VIDEO_PIPE_0, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(g_pipe_id, VIDEO_PIPE_0);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%#x\n", ret);
		return ret;
	}

	TRACE_END();
	return ret;
}

static int rkipc_vi_chn_deinit() {
	int ret = 0;
	TRACE_BEGIN();
	ret = RK_MPI_VI_DisableChn(g_pipe_id, VIDEO_PIPE_0);
	if (ret != RK_SUCCESS)
		LOG_ERROR("RK_MPI_VI_DisableChn failed because %#X\n", ret);
	TRACE_END();
	return ret;
}

static int rkipc_vpss_0_init() {
	int ret, i, VpssChn;
	VPSS_GRP VpssGrp;
	VPSS_GRP_ATTR_S stVpssGrpAttr;
	VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
	AIISP_ATTR_S stAIISPAttr;

	TRACE_BEGIN();
	VpssGrp = 0;
	memset(&stVpssGrpAttr, 0, sizeof(stVpssGrpAttr));
	for (i = 0; i < VPSS_MAX_CHN_NUM; ++i)
		memset(&stVpssChnAttr[i], 0, sizeof(stVpssChnAttr[i]));

	stVpssGrpAttr.u32MaxW = rk_param_get_int("video.0:max_width", 2560);
	stVpssGrpAttr.u32MaxH = rk_param_get_int("video.0:max_height", 1440);
	stVpssGrpAttr.enPixelFormat = RK_FMT_YUV420SP;
	stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
	stVpssGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
	ret = RK_MPI_VPSS_CreateGrp(VpssGrp, &stVpssGrpAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_CreateGrp error! ret is %#x\n", ret);
		return ret;
	}

	// Initialize AI ISP.
	if (g_enable_aiisp) {
		memset(&stAIISPAttr, 0, sizeof(AIISP_ATTR_S));
		stAIISPAttr.bEnable = RK_TRUE;
		stAIISPAttr.stAiIspCallback.pfUpdateCallback = (AIISP_CALLBACK)vpss_aiisp_callback;
		stAIISPAttr.stAiIspCallback.pPrivateData = RK_NULL;
		stAIISPAttr.pModelFilePath = "/oem/usr/lib/";
		stAIISPAttr.u32FrameBufCnt = rk_param_get_int("video.source:aiisp_dst_buf_cnt", 1);
		ret = RK_MPI_VPSS_SetGrpAIISPAttr(VpssGrp, &stAIISPAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("ERROR: initialize AI ISP error! ret=%#x\n", ret);
		}
	}

	// VPSS_0_0, link to venc0
	VpssChn = 0;
	if ((rk_param_get_int("video.0:width", -1) == rk_param_get_int("video.0:max_width", -1)) &&
	    (rk_param_get_int("video.0:height", -1) == rk_param_get_int("video.0:max_height", -1)))
		stVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_PASSTHROUGH;
	else
		stVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_AUTO;
	stVpssChnAttr[VpssChn].enDynamicRange = DYNAMIC_RANGE_SDR8;
	stVpssChnAttr[VpssChn].enPixelFormat = RK_FMT_YUV420SP;
	stVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = -1;
	stVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = -1;
	stVpssChnAttr[VpssChn].u32Width = rk_param_get_int("video.0:width", -1);
	stVpssChnAttr[VpssChn].u32Height = rk_param_get_int("video.0:height", -1);
	stVpssChnAttr[VpssChn].enCompressMode = COMPRESS_MODE_NONE;
	stVpssChnAttr[VpssChn].u32Depth = 0;
	stVpssChnAttr[VpssChn].u32FrameBufCnt = rk_param_get_int("video.source:aiisp_dst_buf_cnt", 1);
	ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &stVpssChnAttr[VpssChn]);
	if (ret != RK_SUCCESS)
		LOG_ERROR("%d: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", VpssChn, ret);
	ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
	if (ret != RK_SUCCESS)
		LOG_ERROR("%d: RK_MPI_VPSS_EnableChn error! ret is %#x\n", VpssChn, ret);

	// VPSS_0_1, link to VPSS_1
	if (g_enable_venc_1 || g_enable_venc_2) {
		VpssChn = 1;
		stVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_AUTO;
		stVpssChnAttr[VpssChn].enDynamicRange = DYNAMIC_RANGE_SDR8;
		stVpssChnAttr[VpssChn].enPixelFormat = RK_FMT_YUV420SP;
		stVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = -1;
		stVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = -1;
		stVpssChnAttr[VpssChn].u32Width = rk_param_get_int("video.1:width", -1);
		stVpssChnAttr[VpssChn].u32Height = rk_param_get_int("video.1:height", -1);
		stVpssChnAttr[VpssChn].enCompressMode = COMPRESS_MODE_NONE;
		ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &stVpssChnAttr[VpssChn]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("%d: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", VpssChn, ret);
		ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
		if (ret != RK_SUCCESS)
			LOG_ERROR("%d: RK_MPI_VPSS_EnableChn error! ret is %#x\n", VpssChn, ret);
	}

	// VPSS_0_2, link to IVS
	if (g_enable_ivs || g_enable_npu) {
		VpssChn = 2;
		stVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_AUTO;
		stVpssChnAttr[VpssChn].enDynamicRange = DYNAMIC_RANGE_SDR8;
		stVpssChnAttr[VpssChn].enPixelFormat = RK_FMT_YUV420SP;
		stVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = -1;
		stVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = -1;
		stVpssChnAttr[VpssChn].u32Width = rk_param_get_int("ivs:width", 704);
		stVpssChnAttr[VpssChn].u32Height = rk_param_get_int("ivs:height", 576);
		stVpssChnAttr[VpssChn].enCompressMode = COMPRESS_MODE_NONE;
		stVpssChnAttr[VpssChn].u32FrameBufCnt = 1;
		if (g_enable_npu) {
			stVpssChnAttr[VpssChn].u32Depth = 1;
			stVpssChnAttr[VpssChn].u32FrameBufCnt = 2;
		}
		ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &stVpssChnAttr[VpssChn]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("%d: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", VpssChn, ret);
		ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
		if (ret != RK_SUCCESS)
			LOG_ERROR("%d: RK_MPI_VPSS_EnableChn error! ret is %#x\n", VpssChn, ret);
	}

	ret = RK_MPI_VPSS_EnableBackupFrame(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_EnableBackupFrame error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_StartGrp(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_StartGrp error! ret is %#x\n", ret);
		return ret;
	}

	TRACE_END();
	return ret;
}

static int rkipc_vpss_0_deinit() {
	int ret = RK_SUCCESS;
	VPSS_GRP VpssGrp = 0;

	TRACE_BEGIN();
	ret |= RK_MPI_VPSS_StopGrp(VpssGrp);
	if (g_enable_ivs || g_enable_npu)
		ret |= RK_MPI_VPSS_DisableChn(VpssGrp, 2);
	if (g_enable_venc_1 || g_enable_venc_2)
		ret |= RK_MPI_VPSS_DisableChn(VpssGrp, 1);
	ret |= RK_MPI_VPSS_DisableChn(VpssGrp, 0);
	ret |= RK_MPI_VPSS_DisableBackupFrame(VpssGrp);
	ret |= RK_MPI_VPSS_DestroyGrp(VpssGrp);
	TRACE_END();

	return ret;
}

static int rkipc_vpss_1_init() {
	int ret, i, VpssChn;
	VPSS_GRP VpssGrp;
	VPSS_GRP_ATTR_S stVpssGrpAttr;
	VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
	AIISP_ATTR_S stAIISPAttr;

	// VPSS 1, link to VENC 2
	TRACE_BEGIN();
	VpssGrp = 1;
	memset(&stVpssGrpAttr, 0, sizeof(stVpssGrpAttr));
	memset(&stVpssChnAttr[0], 0, sizeof(stVpssChnAttr));
	stVpssGrpAttr.u32MaxW = rk_param_get_int("video.2:max_width", 640);
	stVpssGrpAttr.u32MaxH = rk_param_get_int("video.2:max_height", 480);
	stVpssGrpAttr.enPixelFormat = RK_FMT_YUV420SP;
	stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
	stVpssGrpAttr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;

	ret |= RK_MPI_VPSS_CreateGrp(VpssGrp, &stVpssGrpAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_CreateGrp error! ret is %#x\n", ret);
		return ret;
	}

	VpssChn = 0;
	stVpssChnAttr[VpssChn].enChnMode = VPSS_CHN_MODE_AUTO;
	stVpssChnAttr[VpssChn].enDynamicRange = DYNAMIC_RANGE_SDR8;
	stVpssChnAttr[VpssChn].enPixelFormat = RK_FMT_YUV420SP;
	stVpssChnAttr[VpssChn].stFrameRate.s32SrcFrameRate = -1;
	stVpssChnAttr[VpssChn].stFrameRate.s32DstFrameRate = -1;
	stVpssChnAttr[VpssChn].u32Width = rk_param_get_int("video.2:width", -1);
	stVpssChnAttr[VpssChn].u32Height = rk_param_get_int("video.2:height", -1);
	stVpssChnAttr[VpssChn].enCompressMode = COMPRESS_MODE_NONE;
	ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn, &stVpssChnAttr[VpssChn]);
	if (ret != RK_SUCCESS)
		LOG_ERROR("%d: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", VpssChn, ret);
	ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn);
	if (ret != RK_SUCCESS)
		LOG_ERROR("%d: RK_MPI_VPSS_EnableChn error! ret is %#x\n", VpssChn, ret);

	ret = RK_MPI_VPSS_EnableBackupFrame(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_EnableBackupFrame error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_StartGrp(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_StartGrp error! ret is %#x\n", ret);
		return ret;
	}
	TRACE_END();

	return ret;
}

static int rkipc_vpss_1_deinit() {
	int ret = RK_SUCCESS;
	VPSS_GRP VpssGrp = 1;

	TRACE_BEGIN();
	ret |= RK_MPI_VPSS_StopGrp(VpssGrp);
	ret |= RK_MPI_VPSS_DisableChn(VpssGrp, 0);
	ret |= RK_MPI_VPSS_DisableBackupFrame(VpssGrp);
	ret |= RK_MPI_VPSS_DestroyGrp(VpssGrp);
	TRACE_END();

	return ret;
}

static int rkipc_vpss_init() {
	int ret = 0;
	ret |= rkipc_vpss_0_init();
	if (g_enable_venc_2)
		ret |= rkipc_vpss_1_init();

	return ret;
}

static int rkipc_vpss_deinit() {
	int ret = 0;
	if (g_enable_venc_2)
		ret |= rkipc_vpss_1_deinit();
	ret |= rkipc_vpss_0_deinit();

	return ret;
}

static int rkipc_ivs_init() {
	int ret;
	int video_width = rk_param_get_int("ivs:width", 704);
	int video_height = rk_param_get_int("ivs:height", 576);
	int buf_cnt = 2;
	int smear = rk_param_get_int("ivs:smear", 1);
	int weightp = rk_param_get_int("ivs:weightp", 1);
	int md = rk_param_get_int("ivs:md", 0);
	int od = rk_param_get_int("ivs:od", 0);

	TRACE_BEGIN();
	if (!smear && !weightp && !md && !od) {
		LOG_INFO("no pp function enabled! end\n");
		return -1;
	}
	// IVS
	IVS_CHN_ATTR_S attr;
	memset(&attr, 0, sizeof(attr));
	attr.enMode = IVS_MODE_MD_OD;
	attr.u32PicWidth = video_width;
	attr.u32PicHeight = video_height;
	attr.enPixelFormat = RK_FMT_YUV420SP;
	attr.s32Gop = rk_param_get_int("video.0:gop", 30);
	attr.bSmearEnable = smear;
	attr.bWeightpEnable = weightp;
	attr.bMDEnable = md;
	attr.s32MDInterval = 5;
	attr.bMDNightMode = RK_TRUE;
	attr.u32MDSensibility = rk_param_get_int("ivs:md_sensibility", 3);
	attr.bODEnable = od;
	attr.s32ODInterval = 1;
	attr.s32ODPercent = 6;
	ret = RK_MPI_IVS_CreateChn(0, &attr);
	if (ret) {
		LOG_ERROR("ERROR: RK_MPI_IVS_CreateChn error! ret=%#x\n", ret);
		return -1;
	}

	IVS_MD_ATTR_S stMdAttr;
	memset(&stMdAttr, 0, sizeof(stMdAttr));
	ret = RK_MPI_IVS_GetMdAttr(0, &stMdAttr);
	if (ret) {
		LOG_ERROR("ERROR: RK_MPI_IVS_GetMdAttr error! ret=%#x\n", ret);
		return -1;
	}
	stMdAttr.s32ThreshSad = 40;
	stMdAttr.s32ThreshMove = 2;
	stMdAttr.s32SwitchSad = 0;
	ret = RK_MPI_IVS_SetMdAttr(0, &stMdAttr);
	if (ret) {
		LOG_ERROR("ERROR: RK_MPI_IVS_SetMdAttr error! ret=%#x\n", ret);
		return -1;
	}
	TRACE_END();

	return 0;
}

static int rkipc_ivs_deinit() {
	int ret;
	// IVS
	ret = RK_MPI_IVS_DestroyChn(0);
	if (ret)
		LOG_ERROR("ERROR: Destroy IVS error! ret=%#x\n", ret);
	else
		LOG_DEBUG("RK_MPI_IVS_DestroyChn success\n");

	return ret;
}

static int rkipc_venc_0_init() {
	int ret = 0;
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	int rotation = rk_param_get_int("video.source:rotation", 0);
	VENC_CHN_ATTR_S venc_chn_attr;

	TRACE_BEGIN();
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	tmp_output_data_type = rk_param_get_string("video.0:output_data_type", NULL);
	tmp_rc_mode = rk_param_get_string("video.0:rc_mode", NULL);
	tmp_h264_profile = rk_param_get_string("video.0:h264_profile", NULL);
	if ((tmp_output_data_type == NULL) || (tmp_rc_mode == NULL)) {
		LOG_ERROR("tmp_output_data_type or tmp_rc_mode is NULL\n");
		return -1;
	}
	LOG_INFO("tmp_output_data_type is %s, tmp_rc_mode is %s, tmp_h264_profile is %s\n",
	         tmp_output_data_type, tmp_rc_mode, tmp_h264_profile);
	if (!strcmp(tmp_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_AVC;

		if (!strcmp(tmp_h264_profile, "high"))
			venc_chn_attr.stVencAttr.u32Profile = 100;
		else if (!strcmp(tmp_h264_profile, "main"))
			venc_chn_attr.stVencAttr.u32Profile = 77;
		else if (!strcmp(tmp_h264_profile, "baseline"))
			venc_chn_attr.stVencAttr.u32Profile = 66;
		else
			LOG_ERROR("tmp_h264_profile is %s\n", tmp_h264_profile);

		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = rk_param_get_int("video.0:max_rate", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.0:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.0:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.0:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.0:src_frame_rate_num", -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
			venc_chn_attr.stRcAttr.stH264Vbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = rk_param_get_int("video.0:max_rate", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.0:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.0:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.0:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.0:src_frame_rate_num", -1);
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_HEVC;
		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = rk_param_get_int("video.0:max_rate", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.0:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.0:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.0:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.0:src_frame_rate_num", -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
			venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = rk_param_get_int("video.0:max_rate", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.0:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.0:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.0:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.0:src_frame_rate_num", -1);
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	tmp_gop_mode = rk_param_get_string("video.0:gop_mode", NULL);
	if (!strcmp(tmp_gop_mode, "normalP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	} else if (!strcmp(tmp_gop_mode, "smartP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		venc_chn_attr.stGopAttr.s32VirIdrLen = rk_param_get_int("video.0:smartp_viridrlen", 25);
	} else if (!strcmp(tmp_gop_mode, "TSVC4")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_TSVC4;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.0:gop", -1);

	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	venc_chn_attr.stVencAttr.u32StreamBufCnt = rk_param_get_int("video.0:buffer_count", 4);
	venc_chn_attr.stVencAttr.u32BufSize = rk_param_get_int("video.0:buffer_size", 202752);
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_0, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
		return -1;
	}

	tmp_smart = rk_param_get_string("video.0:smart", NULL);
	if (!strcmp(tmp_smart, "open")) {
		ret = RK_MPI_VENC_EnableSvc(VIDEO_PIPE_0, 1);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VENC_EnableSvc failed %#X\n", ret);
	}

	tmp_rc_quality = rk_param_get_string("video.0:rc_quality", NULL);
	VENC_RC_PARAM_S venc_rc_param;
	RK_MPI_VENC_GetRcParam(VIDEO_PIPE_0, &venc_rc_param);
	if (!strcmp(tmp_output_data_type, "H.264")) {
		if (!strcmp(tmp_rc_quality, "highest")) {
			venc_rc_param.stParamH264.u32MinQp = 10;
		} else if (!strcmp(tmp_rc_quality, "higher")) {
			venc_rc_param.stParamH264.u32MinQp = 15;
		} else if (!strcmp(tmp_rc_quality, "high")) {
			venc_rc_param.stParamH264.u32MinQp = 20;
		} else if (!strcmp(tmp_rc_quality, "medium")) {
			venc_rc_param.stParamH264.u32MinQp = 25;
		} else if (!strcmp(tmp_rc_quality, "low")) {
			venc_rc_param.stParamH264.u32MinQp = 30;
		} else if (!strcmp(tmp_rc_quality, "lower")) {
			venc_rc_param.stParamH264.u32MinQp = 35;
		} else {
			venc_rc_param.stParamH264.u32MinQp = 40;
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		if (!strcmp(tmp_rc_quality, "highest")) {
			venc_rc_param.stParamH265.u32MinQp = 10;
		} else if (!strcmp(tmp_rc_quality, "higher")) {
			venc_rc_param.stParamH265.u32MinQp = 15;
		} else if (!strcmp(tmp_rc_quality, "high")) {
			venc_rc_param.stParamH265.u32MinQp = 20;
		} else if (!strcmp(tmp_rc_quality, "medium")) {
			venc_rc_param.stParamH265.u32MinQp = 25;
		} else if (!strcmp(tmp_rc_quality, "low")) {
			venc_rc_param.stParamH265.u32MinQp = 30;
		} else if (!strcmp(tmp_rc_quality, "lower")) {
			venc_rc_param.stParamH265.u32MinQp = 35;
		} else {
			venc_rc_param.stParamH265.u32MinQp = 40;
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetRcParam(VIDEO_PIPE_0, &venc_rc_param);

	if (rk_param_get_int("video.0:enable_refer_buffer_share", 0)) {
		VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
		memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
		stVencChnRefBufShare.bEnable = 1;
		ret = RK_MPI_VENC_SetChnRefBufShareAttr(VIDEO_PIPE_0, &stVencChnRefBufShare);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VENC_SetChnRefBufShareAttr failed,ret=%#x\n", ret);
		}
	}
	if (rotation == 0) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_0, ROTATION_0);
	} else if (rotation == 90) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_0, ROTATION_90);
	} else if (rotation == 180) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_0, ROTATION_180);
	} else if (rotation == 270) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_0, ROTATION_270);
	}

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_0, &stRecvParam);

	TRACE_END();
	return 0;
}

static int rkipc_venc_0_deinit() {
	int ret = 0;
	TRACE_BEGIN();
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_0);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");
	TRACE_END();

	return ret;
}

static int rkipc_venc_1_init() {
	int ret = 0;
	int video_width = rk_param_get_int("video.1:width", -1);
	int video_height = rk_param_get_int("video.1:height", -1);
	int rotation = rk_param_get_int("video.source:rotation", 0);

	// VENC[1] init
	TRACE_BEGIN();
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	tmp_output_data_type = rk_param_get_string("video.1:output_data_type", NULL);
	tmp_rc_mode = rk_param_get_string("video.1:rc_mode", NULL);
	tmp_h264_profile = rk_param_get_string("video.1:h264_profile", NULL);
	if ((tmp_output_data_type == NULL) || (tmp_rc_mode == NULL)) {
		LOG_ERROR("tmp_output_data_type or tmp_rc_mode is NULL\n");
		return -1;
	}
	LOG_INFO("tmp_output_data_type is %s, tmp_rc_mode is %s, tmp_h264_profile is %s\n",
	         tmp_output_data_type, tmp_rc_mode, tmp_h264_profile);
	if (!strcmp(tmp_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_AVC;

		if (!strcmp(tmp_h264_profile, "high"))
			venc_chn_attr.stVencAttr.u32Profile = 100;
		else if (!strcmp(tmp_h264_profile, "main"))
			venc_chn_attr.stVencAttr.u32Profile = 77;
		else if (!strcmp(tmp_h264_profile, "baseline"))
			venc_chn_attr.stVencAttr.u32Profile = 66;
		else
			LOG_ERROR("tmp_h264_profile is %s\n", tmp_h264_profile);

		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int("video.1:gop", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = rk_param_get_int("video.1:max_rate", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.1:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.1:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.1:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.1:src_frame_rate_num", -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
			venc_chn_attr.stRcAttr.stH264Vbr.u32Gop = rk_param_get_int("video.1:gop", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = rk_param_get_int("video.1:max_rate", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.1:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.1:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.1:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.1:src_frame_rate_num", -1);
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_HEVC;
		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int("video.1:gop", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = rk_param_get_int("video.1:max_rate", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.1:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.1:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.1:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.1:src_frame_rate_num", -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
			venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = rk_param_get_int("video.1:gop", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = rk_param_get_int("video.1:max_rate", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.1:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.1:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.1:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.1:src_frame_rate_num", -1);
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	tmp_gop_mode = rk_param_get_string("video.1:gop_mode", NULL);
	if (!strcmp(tmp_gop_mode, "normalP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	} else if (!strcmp(tmp_gop_mode, "smartP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		venc_chn_attr.stGopAttr.s32VirIdrLen = rk_param_get_int("video.1:smartp_viridrlen", 25);
	} else if (!strcmp(tmp_gop_mode, "TSVC4")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_TSVC4;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.1:gop", -1);

	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	venc_chn_attr.stVencAttr.u32StreamBufCnt = rk_param_get_int("video.1:buffer_count", 4);
	venc_chn_attr.stVencAttr.u32BufSize = rk_param_get_int("video.1:buffer_size", 202752);
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_1, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
		return -1;
	}

	tmp_smart = rk_param_get_string("video.1:smart", NULL);
	if (!strcmp(tmp_smart, "open")) {
		ret = RK_MPI_VENC_EnableSvc(VIDEO_PIPE_1, 1);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VENC_EnableSvc failed %#X\n", ret);
	}
	tmp_rc_quality = rk_param_get_string("video.1:rc_quality", NULL);
	VENC_RC_PARAM_S venc_rc_param;
	RK_MPI_VENC_GetRcParam(VIDEO_PIPE_1, &venc_rc_param);
	if (!strcmp(tmp_output_data_type, "H.264")) {
		if (!strcmp(tmp_rc_quality, "highest")) {
			venc_rc_param.stParamH264.u32MinQp = 10;
		} else if (!strcmp(tmp_rc_quality, "higher")) {
			venc_rc_param.stParamH264.u32MinQp = 15;
		} else if (!strcmp(tmp_rc_quality, "high")) {
			venc_rc_param.stParamH264.u32MinQp = 20;
		} else if (!strcmp(tmp_rc_quality, "medium")) {
			venc_rc_param.stParamH264.u32MinQp = 25;
		} else if (!strcmp(tmp_rc_quality, "low")) {
			venc_rc_param.stParamH264.u32MinQp = 30;
		} else if (!strcmp(tmp_rc_quality, "lower")) {
			venc_rc_param.stParamH264.u32MinQp = 35;
		} else {
			venc_rc_param.stParamH264.u32MinQp = 40;
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		if (!strcmp(tmp_rc_quality, "highest")) {
			venc_rc_param.stParamH265.u32MinQp = 10;
		} else if (!strcmp(tmp_rc_quality, "higher")) {
			venc_rc_param.stParamH265.u32MinQp = 15;
		} else if (!strcmp(tmp_rc_quality, "high")) {
			venc_rc_param.stParamH265.u32MinQp = 20;
		} else if (!strcmp(tmp_rc_quality, "medium")) {
			venc_rc_param.stParamH265.u32MinQp = 25;
		} else if (!strcmp(tmp_rc_quality, "low")) {
			venc_rc_param.stParamH265.u32MinQp = 30;
		} else if (!strcmp(tmp_rc_quality, "lower")) {
			venc_rc_param.stParamH265.u32MinQp = 35;
		} else {
			venc_rc_param.stParamH265.u32MinQp = 40;
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetRcParam(VIDEO_PIPE_1, &venc_rc_param);

	if (rk_param_get_int("video.1:enable_refer_buffer_share", 0)) {
		VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
		memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
		stVencChnRefBufShare.bEnable = 1;
		ret = RK_MPI_VENC_SetChnRefBufShareAttr(VIDEO_PIPE_1, &stVencChnRefBufShare);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VENC_SetChnRefBufShareAttr failed,ret=%#x\n", ret);
		}
	}
	if (rotation == 0) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_1, ROTATION_0);
	} else if (rotation == 90) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_1, ROTATION_90);
	} else if (rotation == 180) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_1, ROTATION_180);
	} else if (rotation == 270) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_1, ROTATION_270);
	}

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_1, &stRecvParam);

	TRACE_END();

	return 0;
}

static int rkipc_venc_1_deinit() {
	int ret = 0;
	TRACE_BEGIN();
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_1);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_1);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");
	TRACE_END();

	return ret;
}

static int rkipc_venc_2_init() {
	int ret = 0;
	int video_width = rk_param_get_int("video.2:width", -1);
	int video_height = rk_param_get_int("video.2:height", -1);
	int rotation = rk_param_get_int("video.source:rotation", 0);
	VENC_CHN_ATTR_S venc_chn_attr;

	// VENC[2] init
	TRACE_BEGIN();
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	tmp_output_data_type = rk_param_get_string("video.2:output_data_type", NULL);
	tmp_rc_mode = rk_param_get_string("video.2:rc_mode", NULL);
	tmp_h264_profile = rk_param_get_string("video.2:h264_profile", NULL);
	if ((tmp_output_data_type == NULL) || (tmp_rc_mode == NULL)) {
		LOG_ERROR("tmp_output_data_type or tmp_rc_mode is NULL\n");
		return -1;
	}
	LOG_INFO("tmp_output_data_type is %s, tmp_rc_mode is %s, tmp_h264_profile is %s\n",
	         tmp_output_data_type, tmp_rc_mode, tmp_h264_profile);
	if (!strcmp(tmp_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_AVC;

		if (!strcmp(tmp_h264_profile, "high"))
			venc_chn_attr.stVencAttr.u32Profile = 100;
		else if (!strcmp(tmp_h264_profile, "main"))
			venc_chn_attr.stVencAttr.u32Profile = 77;
		else if (!strcmp(tmp_h264_profile, "baseline"))
			venc_chn_attr.stVencAttr.u32Profile = 66;
		else
			LOG_ERROR("tmp_h264_profile is %s\n", tmp_h264_profile);

		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int("video.2:gop", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = rk_param_get_int("video.2:max_rate", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.2:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.2:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.2:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.2:src_frame_rate_num", -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
			venc_chn_attr.stRcAttr.stH264Vbr.u32Gop = rk_param_get_int("video.2:gop", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = rk_param_get_int("video.2:max_rate", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.2:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.2:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.2:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.2:src_frame_rate_num", -1);
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_HEVC;
		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int("video.2:gop", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = rk_param_get_int("video.2:max_rate", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.2:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.2:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.2:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.2:src_frame_rate_num", -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
			venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = rk_param_get_int("video.2:gop", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = rk_param_get_int("video.2:max_rate", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.2:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.2:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.2:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.2:src_frame_rate_num", -1);
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	tmp_gop_mode = rk_param_get_string("video.2:gop_mode", NULL);
	if (!strcmp(tmp_gop_mode, "normalP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	} else if (!strcmp(tmp_gop_mode, "smartP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		venc_chn_attr.stGopAttr.s32VirIdrLen = rk_param_get_int("video.2:smartp_viridrlen", 25);
	} else if (!strcmp(tmp_gop_mode, "TSVC4")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_TSVC4;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.2:gop", -1);

	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	venc_chn_attr.stVencAttr.u32StreamBufCnt = rk_param_get_int("video.2:buffer_count", 4);
	venc_chn_attr.stVencAttr.u32BufSize = rk_param_get_int("video.2:buffer_size", 202752);
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_2, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
		return -1;
	}

	tmp_smart = rk_param_get_string("video.2:smart", NULL);
	if (!strcmp(tmp_smart, "open")) {
		ret = RK_MPI_VENC_EnableSvc(VIDEO_PIPE_2, 1);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VENC_EnableSvc failed %#X\n", ret);
	}
	tmp_rc_quality = rk_param_get_string("video.2:rc_quality", NULL);
	VENC_RC_PARAM_S venc_rc_param;
	RK_MPI_VENC_GetRcParam(VIDEO_PIPE_2, &venc_rc_param);
	if (!strcmp(tmp_output_data_type, "H.264")) {
		if (!strcmp(tmp_rc_quality, "highest")) {
			venc_rc_param.stParamH264.u32MinQp = 10;
		} else if (!strcmp(tmp_rc_quality, "higher")) {
			venc_rc_param.stParamH264.u32MinQp = 15;
		} else if (!strcmp(tmp_rc_quality, "high")) {
			venc_rc_param.stParamH264.u32MinQp = 20;
		} else if (!strcmp(tmp_rc_quality, "medium")) {
			venc_rc_param.stParamH264.u32MinQp = 25;
		} else if (!strcmp(tmp_rc_quality, "low")) {
			venc_rc_param.stParamH264.u32MinQp = 30;
		} else if (!strcmp(tmp_rc_quality, "lower")) {
			venc_rc_param.stParamH264.u32MinQp = 35;
		} else {
			venc_rc_param.stParamH264.u32MinQp = 40;
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		if (!strcmp(tmp_rc_quality, "highest")) {
			venc_rc_param.stParamH265.u32MinQp = 10;
		} else if (!strcmp(tmp_rc_quality, "higher")) {
			venc_rc_param.stParamH265.u32MinQp = 15;
		} else if (!strcmp(tmp_rc_quality, "high")) {
			venc_rc_param.stParamH265.u32MinQp = 20;
		} else if (!strcmp(tmp_rc_quality, "medium")) {
			venc_rc_param.stParamH265.u32MinQp = 25;
		} else if (!strcmp(tmp_rc_quality, "low")) {
			venc_rc_param.stParamH265.u32MinQp = 30;
		} else if (!strcmp(tmp_rc_quality, "lower")) {
			venc_rc_param.stParamH265.u32MinQp = 35;
		} else {
			venc_rc_param.stParamH265.u32MinQp = 40;
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetRcParam(VIDEO_PIPE_2, &venc_rc_param);

	if (rk_param_get_int("video.2:enable_refer_buffer_share", 0)) {
		VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
		memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
		stVencChnRefBufShare.bEnable = 1;
		ret = RK_MPI_VENC_SetChnRefBufShareAttr(VIDEO_PIPE_2, &stVencChnRefBufShare);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VENC_SetChnRefBufShareAttr failed,ret=%#x\n", ret);
		}
	}
	if (rotation == 0) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_2, ROTATION_0);
	} else if (rotation == 90) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_2, ROTATION_90);
	} else if (rotation == 180) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_2, ROTATION_180);
	} else if (rotation == 270) {
		RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_2, ROTATION_270);
	}

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_2, &stRecvParam);

	TRACE_END();

	return 0;
}

static int rkipc_venc_2_deinit() {
	int ret = 0;
	TRACE_BEGIN();
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_2);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_2);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");
	TRACE_END();

	return ret;
}

static int rkipc_jpeg_init() {
	// jpeg resolution same to video.0
	int ret;
	int width = rk_param_get_int("video.jpeg:width", -1);
	int height = rk_param_get_int("video.jpeg:height", -1);
	VENC_CHN_ATTR_S jpeg_chn_attr;

	TRACE_BEGIN();
	memset(&jpeg_chn_attr, 0, sizeof(jpeg_chn_attr));
	jpeg_chn_attr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
	jpeg_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	jpeg_chn_attr.stVencAttr.u32MaxPicWidth = rk_param_get_int("video.0:max_width", 2560);
	jpeg_chn_attr.stVencAttr.u32MaxPicHeight = rk_param_get_int("video.0:max_height", 1440);
	jpeg_chn_attr.stVencAttr.u32PicWidth = width;
	jpeg_chn_attr.stVencAttr.u32PicHeight = height;
	jpeg_chn_attr.stVencAttr.u32VirWidth = width;
	jpeg_chn_attr.stVencAttr.u32VirHeight = height;
	jpeg_chn_attr.stVencAttr.u32StreamBufCnt = 2;
	jpeg_chn_attr.stVencAttr.u32BufSize = rk_param_get_int("video.jpeg:jpeg_buffer_size", 2097152);
	// jpeg_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(JPEG_VENC_CHN, &jpeg_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%d\n", ret);
		return ret;
	}

	VENC_COMBO_ATTR_S stComboAttr;
	memset(&stComboAttr, 0, sizeof(VENC_COMBO_ATTR_S));
	stComboAttr.bEnable = RK_TRUE;
	stComboAttr.s32ChnId = VIDEO_PIPE_0;
	RK_MPI_VENC_SetComboAttr(JPEG_VENC_CHN, &stComboAttr);

	VENC_JPEG_PARAM_S stJpegParam;
	memset(&stJpegParam, 0, sizeof(stJpegParam));
	stJpegParam.u32Qfactor = rk_param_get_int("video.jpeg:jpeg_qfactor", 70);
	RK_MPI_VENC_SetJpegParam(JPEG_VENC_CHN, &stJpegParam);
	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = 1;
	RK_MPI_VENC_StartRecvFrame(JPEG_VENC_CHN,
	                           &stRecvParam); // must, for no streams callback running failed
	RK_MPI_VENC_StopRecvFrame(JPEG_VENC_CHN);

	ret = pthread_mutex_init(&g_capture_mutex, NULL);
	if (ret != 0) {
		LOG_ERROR("Posix thread init error %#X\n", errno);
		return RK_FAILURE;
	}
	pthread_cond_init(&g_capture_cond, NULL);
	if (ret != 0) {
		LOG_ERROR("Posix cond init error %#X\n", errno);
		return RK_FAILURE;
	}
	TRACE_END();

	return ret;
}

static int rkipc_jpeg_deinit() {
	int ret = 0;

	TRACE_BEGIN();
	// Stop cycle snapshot thread.
	pthread_mutex_destroy(&g_capture_mutex);
	pthread_cond_destroy(&g_capture_cond);
	ret = RK_MPI_VENC_StopRecvFrame(JPEG_VENC_CHN);
	ret |= RK_MPI_VENC_DestroyChn(JPEG_VENC_CHN);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");
	TRACE_END();

	return ret;
}

static const char *g_rk_mod_name[RK_ID_BUTT] = {
    [RK_ID_VENC] = RK_MOD_VENC, [RK_ID_VPSS] = RK_MOD_VPSS, [RK_ID_VI] = RK_MOD_VI,
    [RK_ID_VO] = RK_MOD_VO,     [RK_ID_IVS] = RK_MOD_IVS,
};

static int rkipc_bind_helper(MPP_CHN_S *src, MPP_CHN_S *dst) {
	int ret = RK_SUCCESS;
	if (NULL == src || NULL == dst)
		return RK_FAILURE;
	ret = RK_MPI_SYS_Bind(src, dst);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("Bind error! ret=%#x\n", ret);
		LOG_ERROR("Src Mod:%s, Dev:%d, Chn:%d\n", g_rk_mod_name[src->enModId], src->s32DevId,
		          src->s32ChnId);
		LOG_ERROR("Dst Mod:%s, Dev:%d, Chn:%d\n", g_rk_mod_name[dst->enModId], dst->s32DevId,
		          dst->s32ChnId);
	}
	return ret;
}

static int rkipc_bind_init() {
	int ret, i;

	TRACE_BEGIN();
	// vi channel
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = 0;
	for (i = 0; i < 3; i++) {
		// venc
		venc_chns[i].enModId = RK_ID_VENC;
		venc_chns[i].s32DevId = 0;
		venc_chns[i].s32ChnId = i;
		// vpss 0
		vpss_0_chns[i].enModId = RK_ID_VPSS;
		vpss_0_chns[i].s32DevId = 0; // vpss group 0
		vpss_0_chns[i].s32ChnId = i;
	}
	// vpss 1
	vpss_1_chns[0].enModId = RK_ID_VPSS;
	vpss_1_chns[0].s32DevId = 1; // vpss group 1
	vpss_1_chns[0].s32ChnId = 0;
	// ivs channel
	ivs_chn.enModId = RK_ID_IVS;
	ivs_chn.s32DevId = 0;
	ivs_chn.s32ChnId = 0;

	ret |= rkipc_bind_helper(&vi_chn, &vpss_0_chns[0]);

	if (g_enable_venc_0)
		ret |= rkipc_bind_helper(&vpss_0_chns[0], &venc_chns[0]);
	if (g_enable_venc_1)
		ret |= rkipc_bind_helper(&vpss_0_chns[1], &venc_chns[1]);
	if (g_enable_ivs)
		ret |= rkipc_bind_helper(&vpss_0_chns[2], &ivs_chn);
	if (g_enable_venc_2) {
		ret |= rkipc_bind_helper(&vpss_0_chns[1], &vpss_1_chns[0]);
		ret |= rkipc_bind_helper(&vpss_1_chns[0], &venc_chns[2]);
	}

	TRACE_END();

	return ret;
}

static int rkipc_bind_deinit() {
	int ret;

	if (g_enable_venc_2) {
		ret |= RK_MPI_SYS_UnBind(&vpss_1_chns[0], &venc_chns[2]);
		ret |= RK_MPI_SYS_UnBind(&vpss_0_chns[1], &vpss_1_chns[0]);
	}
	if (g_enable_ivs)
		ret |= RK_MPI_SYS_UnBind(&vpss_0_chns[2], &ivs_chn);
	if (g_enable_venc_1)
		ret |= RK_MPI_SYS_UnBind(&vpss_0_chns[1], &venc_chns[1]);
	if (g_enable_venc_0)
		ret |= RK_MPI_SYS_UnBind(&vpss_0_chns[0], &venc_chns[0]);

	ret |= RK_MPI_SYS_UnBind(&vi_chn, &vpss_0_chns[0]);

	return ret;
}

static int rkipc_osd_cover_create(int id, osd_data_s *osd_data) {
	int ret = 0;
	RGN_HANDLE coverHandle = id;
	RGN_ATTR_S stCoverAttr;
	MPP_CHN_S stCoverChn;
	RGN_CHN_ATTR_S stCoverChnAttr;
	int rotation = rk_param_get_int("video.source:rotation", 0);
	int video_0_width = rk_param_get_int("video.0:width", -1);
	int video_0_height = rk_param_get_int("video.0:height", -1);
	int video_0_max_width = rk_param_get_int("video.0:max_width", -1);
	int video_0_max_height = rk_param_get_int("video.0:max_height", -1);
	double video_0_w_h_rate = 1.0;

	// since the coordinates stored in the OSD module are of actual resolution,
	// 1106 needs to be converted back to the maximum resolution
	osd_data->origin_x = osd_data->origin_x * video_0_max_width / video_0_width;
	osd_data->origin_y = osd_data->origin_y * video_0_max_height / video_0_height;
	osd_data->width = osd_data->width * video_0_max_width / video_0_width;
	osd_data->height = osd_data->height * video_0_max_height / video_0_height;

	memset(&stCoverAttr, 0, sizeof(stCoverAttr));
	memset(&stCoverChnAttr, 0, sizeof(stCoverChnAttr));
	// create cover regions
	stCoverAttr.enType = COVER_RGN;
	ret = RK_MPI_RGN_Create(coverHandle, &stCoverAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Create (%d) failed with %#x\n", coverHandle, ret);
		RK_MPI_RGN_Destroy(coverHandle);
		return RK_FAILURE;
	}
	LOG_DEBUG("The handle: %d, create success\n", coverHandle);

	// when cover is attached to VI,
	// coordinate conversion of three angles shall be considered when rotating VENC
	video_0_w_h_rate = (double)video_0_max_width / (double)video_0_max_height;
	if (rotation == 90) {
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X =
		    (double)osd_data->origin_y * video_0_w_h_rate;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y =
		    (video_0_max_height -
		     ((double)(osd_data->width + osd_data->origin_x) / video_0_w_h_rate));
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width =
		    (double)osd_data->height * video_0_w_h_rate;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height =
		    (double)osd_data->width / video_0_w_h_rate;
	} else if (rotation == 270) {
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X =
		    (video_0_max_width -
		     ((double)(osd_data->height + osd_data->origin_y) * video_0_w_h_rate));
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y =
		    (double)osd_data->origin_x / video_0_w_h_rate;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width =
		    (double)osd_data->height * video_0_w_h_rate;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height =
		    (double)osd_data->width / video_0_w_h_rate;
	} else if (rotation == 180) {
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X =
		    video_0_max_width - osd_data->width - osd_data->origin_x;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y =
		    video_0_max_height - osd_data->height - osd_data->origin_y;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = osd_data->width;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = osd_data->height;
	} else {
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = osd_data->origin_x;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = osd_data->origin_y;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = osd_data->width;
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = osd_data->height;
	}
	stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X =
	    UPALIGNTO16(stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X);
	stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y =
	    UPALIGNTO16(stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y);
	stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width =
	    UPALIGNTO16(stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width);
	stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height =
	    UPALIGNTO16(stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height);
	// because the rotation is done in the VENC,
	// and the cover and VI resolution are both before the rotation,
	// there is no need to judge the rotation here
	while (stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X +
	           stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width >
	       video_0_max_width) {
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width -= 16;
	}
	while (stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y +
	           stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height >
	       video_0_max_height) {
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height -= 16;
	}

	// display cover regions to vi
	stCoverChn.enModId = RK_ID_VI;
	stCoverChn.s32DevId = 0;
	stCoverChn.s32ChnId = VI_MAX_CHN_NUM;
	stCoverChnAttr.bShow = osd_data->enable;
	stCoverChnAttr.enType = COVER_RGN;
	stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0xffffffff;
	stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = id;
	LOG_DEBUG("cover region to chn success\n");
	ret = RK_MPI_RGN_AttachToChn(coverHandle, &stCoverChn, &stCoverChnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("vi pipe RK_MPI_RGN_AttachToChn (%d) failed with %#x\n", coverHandle, ret);
		return RK_FAILURE;
	}
	ret = RK_MPI_RGN_SetDisplayAttr(coverHandle, &stCoverChn, &stCoverChnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("vi pipe RK_MPI_RGN_SetDisplayAttr failed with %#x\n", ret);
		return RK_FAILURE;
	}
	LOG_DEBUG("RK_MPI_RGN_AttachToChn to vi 0 success\n");
	return ret;
}

static int rkipc_osd_cover_destroy(int id) {
	int ret = 0;
	MPP_CHN_S stMppChn;
	RGN_HANDLE RgnHandle = id;
	// Detach osd from chn
	stMppChn.enModId = RK_ID_VI;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = VI_MAX_CHN_NUM;
	ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
	if (RK_SUCCESS != ret)
		LOG_DEBUG("RK_MPI_RGN_DetachFrmChn (%d) to vi pipe failed with %#x\n", RgnHandle, ret);

	// destory region
	ret = RK_MPI_RGN_Destroy(RgnHandle);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Destroy [%d] failed with %#x\n", RgnHandle, ret);
	}

	LOG_INFO("Destory handle:%d success\n", RgnHandle);
	return ret;
}

static int rkipc_osd_bmp_create(int id, osd_data_s *osd_data) {
	LOG_DEBUG("%s: id is %d\n", __func__, id);
	int ret = 0;
	RGN_HANDLE RgnHandle = id;
	RGN_ATTR_S stRgnAttr;
	MPP_CHN_S stMppChn;
	RGN_CHN_ATTR_S stRgnChnAttr;
	BITMAP_S stBitmap;

	// create overlay regions
	memset(&stRgnAttr, 0, sizeof(stRgnAttr));
	stRgnAttr.enType = OVERLAY_RGN;
	stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_ARGB8888;
	stRgnAttr.unAttr.stOverlay.stSize.u32Width = osd_data->width;
	stRgnAttr.unAttr.stOverlay.stSize.u32Height = osd_data->height;
	ret = RK_MPI_RGN_Create(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Create (%d) failed with %#x\n", RgnHandle, ret);
		RK_MPI_RGN_Destroy(RgnHandle);
		return RK_FAILURE;
	}
	LOG_DEBUG("The handle: %d, create success\n", RgnHandle);

	// display overlay regions to vpss 1
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = 0;
	memset(&stRgnChnAttr, 0, sizeof(stRgnChnAttr));
	stRgnChnAttr.bShow = osd_data->enable;
	stRgnChnAttr.enType = OVERLAY_RGN;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = osd_data->origin_x;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = osd_data->origin_y;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 64;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 64;
	// 1126 osd alpha not support multi layer, so use layer 0 to draw nn
	stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = id + 1; // 0 1 2 3 6 → 1 2 3 4 5
	// make sure cover layer is higher
	if (id == 6)
		stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = 5;

	if (g_enable_venc_0) {
		stMppChn.s32ChnId = 0;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
		else
			LOG_DEBUG("RK_MPI_RGN_AttachToChn to venc0 success\n");
	}
	if (g_enable_venc_1) {
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X =
		    UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.1:width", 1) /
		                rk_param_get_int("video.0:width", 1));
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y =
		    UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.1:height", 1) /
		                rk_param_get_int("video.0:height", 1));
		stMppChn.s32ChnId = 1;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
		else
			LOG_DEBUG("RK_MPI_RGN_AttachToChn to venc1 success\n");
	}
	if (g_enable_venc_2) {
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X =
		    UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.2:width", 1) /
		                rk_param_get_int("video.0:width", 1));
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y =
		    UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.2:height", 1) /
		                rk_param_get_int("video.0:height", 1));
		stMppChn.s32ChnId = 2;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc2 failed with %#x\n", RgnHandle, ret);
		else
			LOG_DEBUG("RK_MPI_RGN_AttachToChn to venc2 success\n");
	}
	if (g_enable_jpeg) {
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = UPALIGNTO16(osd_data->origin_x);
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = UPALIGNTO16(osd_data->origin_y);
		stMppChn.s32ChnId = JPEG_VENC_CHN;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to jpeg failed with %#x\n", RgnHandle, ret);
		else
			LOG_DEBUG("RK_MPI_RGN_AttachToChn to jpeg success\n");
	}

	// set bitmap
	stBitmap.enPixelFormat = RK_FMT_ARGB8888;
	stBitmap.u32Width = osd_data->width;
	stBitmap.u32Height = osd_data->height;
	stBitmap.pData = (RK_VOID *)osd_data->buffer;
	ret = RK_MPI_RGN_SetBitMap(RgnHandle, &stBitmap);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_RGN_SetBitMap failed with %#x\n", ret);
		return RK_FAILURE;
	}

	return ret;
}

static int rkipc_osd_bmp_destroy(int id) {
	LOG_DEBUG("%s\n", __func__);
	int ret = 0;
	// Detach osd from chn
	MPP_CHN_S stMppChn;
	RGN_HANDLE RgnHandle = id;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = 0;

	if (g_enable_venc_0) {
		stMppChn.s32ChnId = 0;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_DEBUG("RK_MPI_RGN_DetachFrmChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
	}
	if (g_enable_venc_1) {
		stMppChn.s32ChnId = 1;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_DEBUG("RK_MPI_RGN_DetachFrmChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
	}
	if (g_enable_venc_2) {
		stMppChn.s32ChnId = 2;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_DEBUG("RK_MPI_RGN_DetachFrmChn (%d) to venc2 failed with %#x\n", RgnHandle, ret);
	}
	if (g_enable_jpeg) {
		stMppChn.s32ChnId = JPEG_VENC_CHN;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_DEBUG("RK_MPI_RGN_DetachFrmChn (%d) to jpeg failed with %#x\n", RgnHandle, ret);
	}

	// destory region
	ret = RK_MPI_RGN_Destroy(RgnHandle);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Destroy [%d] failed with %#x\n", RgnHandle, ret);
		return RK_FAILURE;
	}
	LOG_INFO("Destory handle:%d success\n", RgnHandle);
	return ret;
}

static int rkipc_osd_bmp_change(int id, osd_data_s *osd_data) {
	// LOG_INFO("%s: id is %d\n", __func__, id);
	int ret = 0;
	RGN_HANDLE RgnHandle = id;
	BITMAP_S stBitmap;

	// set bitmap
	stBitmap.enPixelFormat = RK_FMT_ARGB8888;
	stBitmap.u32Width = osd_data->width;
	stBitmap.u32Height = osd_data->height;
	stBitmap.pData = (RK_VOID *)osd_data->buffer;
	ret = RK_MPI_RGN_SetBitMap(RgnHandle, &stBitmap);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_RGN_SetBitMap failed with %#x\n", ret);
		return RK_FAILURE;
	}

	return ret;
}

static int rkipc_osd_init() {
	rk_osd_cover_create_callback_register(rkipc_osd_cover_create);
	rk_osd_cover_destroy_callback_register(rkipc_osd_cover_destroy);
	rk_osd_bmp_create_callback_register(rkipc_osd_bmp_create);
	rk_osd_bmp_destroy_callback_register(rkipc_osd_bmp_destroy);
	rk_osd_bmp_change_callback_register(rkipc_osd_bmp_change);
	rk_osd_init();

	return 0;
}

static int rkipc_osd_deinit() {
	rk_osd_deinit();
	rk_osd_cover_create_callback_register(NULL);
	rk_osd_cover_destroy_callback_register(NULL);
	rk_osd_bmp_create_callback_register(NULL);
	rk_osd_bmp_destroy_callback_register(NULL);
	rk_osd_bmp_change_callback_register(NULL);

	return 0;
}

// Thread which send video frame to rtmp server.
static void *rkipc_get_venc_0(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcVenc0", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_0, &stFrame, 2500);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);

			rkipc_rtsp_write_video_frame(0, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
			if ((stFrame.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
			    (stFrame.pstPack->DataType.enH264EType == H264E_NALU_ISLICE) ||
			    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_IDRSLICE) ||
			    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_ISLICE)) {
				rk_storage_write_video_frame(0, data, stFrame.pstPack->u32Len,
				                             stFrame.pstPack->u64PTS, 1);
				rk_rtmp_write_video_frame(0, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
				                          1);
			} else {
				rk_storage_write_video_frame(0, data, stFrame.pstPack->u32Len,
				                             stFrame.pstPack->u64PTS, 0);
				rk_rtmp_write_video_frame(0, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
				                          0);
			}
			ret = RK_MPI_VENC_ReleaseStream(VIDEO_PIPE_0, &stFrame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
			}
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VENC_GetStream timeout %x\n", ret);
			VENC_CHN_STATUS_S status;
			RK_MPI_VENC_QueryStatus(VIDEO_PIPE_0, &status);
			LOG_ERROR("LeftPics %d, CurPacks %d, LeftRecvPics %d, LeftEncPics %d\n",
			          status.u32LeftPics, status.u32CurPacks, status.u32LeftRecvPics,
			          status.u32LeftEncPics);
		}
	}
	if (stFrame.pstPack)
		free(stFrame.pstPack);

	return 0;
}

static void *rkipc_get_venc_1(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcVenc1", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int ret = 0;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_1, &stFrame, 2500);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			rkipc_rtsp_write_video_frame(1, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
			if ((stFrame.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
			    (stFrame.pstPack->DataType.enH264EType == H264E_NALU_ISLICE) ||
			    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_IDRSLICE) ||
			    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_ISLICE)) {
				rk_storage_write_video_frame(1, data, stFrame.pstPack->u32Len,
				                             stFrame.pstPack->u64PTS, 1);
				rk_rtmp_write_video_frame(1, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
				                          1);
			} else {
				rk_storage_write_video_frame(1, data, stFrame.pstPack->u32Len,
				                             stFrame.pstPack->u64PTS, 0);
				rk_rtmp_write_video_frame(1, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
				                          0);
			}
			ret = RK_MPI_VENC_ReleaseStream(VIDEO_PIPE_1, &stFrame);
			if (ret != RK_SUCCESS)
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
		} else {
			LOG_ERROR("RK_MPI_VENC_GetStream timeout %x\n", ret);
		}
	}
	if (stFrame.pstPack)
		free(stFrame.pstPack);

	return 0;
}

static void *rkipc_get_venc_2(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcVenc2", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_2, &stFrame, 2500);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			rkipc_rtsp_write_video_frame(2, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
			if ((stFrame.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
			    (stFrame.pstPack->DataType.enH264EType == H264E_NALU_ISLICE) ||
			    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_IDRSLICE) ||
			    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_ISLICE)) {
				rk_storage_write_video_frame(2, data, stFrame.pstPack->u32Len,
				                             stFrame.pstPack->u64PTS, 1);
				rk_rtmp_write_video_frame(2, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
				                          1);
			} else {
				rk_storage_write_video_frame(2, data, stFrame.pstPack->u32Len,
				                             stFrame.pstPack->u64PTS, 0);
				rk_rtmp_write_video_frame(2, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
				                          0);
			}
			ret = RK_MPI_VENC_ReleaseStream(VIDEO_PIPE_2, &stFrame);
			if (ret != RK_SUCCESS)
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VENC_GetStream timeout %x\n", ret);
		}
	}
	if (stFrame.pstPack)
		free(stFrame.pstPack);

	return 0;
}

static void *rkipc_ivs_get_results(void *arg) {
	LOG_INFO("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcGetIVS", 0, 0, 0);
	int ret, i;
	IVS_RESULT_INFO_S stResults;
	int resultscount = 0;
	int count = 0;
	int md = rk_param_get_int("ivs:md", 0);
	int od = rk_param_get_int("ivs:od", 0);
	int width = rk_param_get_int("ivs:width", 704);
	int height = rk_param_get_int("ivs:height", 576);
	int md_area_threshold = width * height * 0.3;

	while (g_video_run_) {
		ret = RK_MPI_IVS_GetResults(0, &stResults, 1000);
		if (ret == RK_SUCCESS) {
			resultscount++;
			if (md == 1) {
				if (stResults.pstResults->stMdInfo.u32Square > md_area_threshold) {
					LOG_INFO("MD: md_area is %d, md_area_threshold is %d\n",
					         stResults.pstResults->stMdInfo.u32Square, md_area_threshold);
				}
			}
			if (od == 1) {
				if (stResults.s32ResultNum > 0) {
					if (stResults.pstResults->stOdInfo.u32Flag)
						LOG_INFO("OD flag:%d\n", stResults.pstResults->stOdInfo.u32Flag);
				}
			}
			RK_MPI_IVS_ReleaseResults(0, &stResults);
		} else {
			LOG_ERROR("get chn %d fail %#X\n", 0, ret);
			usleep(50000llu);
		}
	}
	return NULL;
}

static void *rkipc_iva_get_frame(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcGetVi2", 0, 0, 0);
	int ret;
	int32_t loopCount = 0;
	VIDEO_FRAME_INFO_S stViFrame;
	int npu_cycle_time_ms = 1000 / rk_param_get_int("video.source:npu_fps", 10);

	long long before_time, cost_time;
	while (g_video_run_) {
		before_time = rkipc_get_curren_time_ms();
		ret = RK_MPI_VPSS_GetChnFrame(0, 2, &stViFrame, 1000);
		if (ret == RK_SUCCESS) {
			// FIXME:
			// RK_MPI_VPSS_GetChnFrame loss frame size.
			stViFrame.stVFrame.u32Width = rk_param_get_int("ivs:width", 704);
			stViFrame.stVFrame.u32Height = rk_param_get_int("ivs:height", 576);
			void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
			int32_t fd = RK_MPI_MB_Handle2Fd(stViFrame.stVFrame.pMbBlk);
			ret = rkipc_rockiva_write_nv12_frame_by_fd(stViFrame.stVFrame.u32Width,
			                                           stViFrame.stVFrame.u32Height, loopCount, fd);
			if (ret != RK_SUCCESS)
				LOG_INFO("rockiva failed,width=%d,height=%d,fd=%d,data=%p,ret=%#X\n",
				         stViFrame.stVFrame.u32Width, stViFrame.stVFrame.u32Height, fd, data, ret);
			ret = RK_MPI_VPSS_ReleaseChnFrame(0, 2, &stViFrame);
			if (ret != RK_SUCCESS)
				LOG_ERROR("%s: fail %x\n", __func__, ret);
			loopCount++;
		} else {
			LOG_ERROR("%s: timeout %x\n", __func__, ret);
		}
		cost_time = rkipc_get_curren_time_ms() - before_time;
		if ((cost_time > 0) && (cost_time < npu_cycle_time_ms))
			usleep((npu_cycle_time_ms - cost_time) * 1000);
	}
	return NULL;
}

static int rkipc_resize_jpeg_by_tde() {
	int jpeg_width, jpeg_height, ret;

	TDE_HANDLE hHandle;
	TDE_SURFACE_S pstSrc, pstDst;
	TDE_RECT_S pstSrcRect, pstDstRect;

	VIDEO_FRAME_INFO_S stViFrame, DstFrame;
	PIC_BUF_ATTR_S Dst_stPicBufAttr;
	MB_PIC_CAL_S Dst_stMbPicCalResult;
	VENC_CHN_ATTR_S pstChnAttr;
	MB_BLK dstBlk = RK_NULL;

	Dst_stPicBufAttr.u32Width = rk_param_get_int("video.jpeg:width", 2560);
	Dst_stPicBufAttr.u32Height = rk_param_get_int("video.jpeg:height", 1440);
	Dst_stPicBufAttr.enPixelFormat = RK_FMT_YUV420SP;
	Dst_stPicBufAttr.enCompMode = COMPRESS_MODE_NONE;

	ret = RK_MPI_CAL_TDE_GetPicBufferSize(&Dst_stPicBufAttr, &Dst_stMbPicCalResult);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("get picture buffer size failed. err 0x%x\n", ret);
		return ret;
	}
	ret = RK_MPI_SYS_MmzAlloc(&dstBlk, RK_NULL, RK_NULL, Dst_stMbPicCalResult.u32MBSize);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_SYS_MmzAlloc err 0x%x\n", ret);
		return ret;
	}

	pstSrc.enColorFmt = RK_FMT_YUV420SP;
	pstSrc.enComprocessMode = COMPRESS_MODE_NONE;
	pstSrcRect.s32Xpos = 0;
	pstSrcRect.s32Ypos = 0;

	pstDst.enColorFmt = RK_FMT_YUV420SP;
	pstDst.enComprocessMode = COMPRESS_MODE_NONE;
	pstDstRect.s32Xpos = 0;
	pstDstRect.s32Ypos = 0;

	memset(&DstFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
	DstFrame.stVFrame.enPixelFormat = RK_FMT_YUV420SP;
	DstFrame.stVFrame.enCompressMode = COMPRESS_MODE_NONE;

	ret = RK_TDE_Open();
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_TDE_Open fail %x\n", ret);
		RK_MPI_SYS_Free(dstBlk);
		return ret;
	}

	pstSrc.u32Width = rk_param_get_int("video.0:width", 2688);
	pstSrc.u32Height = rk_param_get_int("video.0:height", 1520);
	pstSrcRect.u32Width = rk_param_get_int("video.0:width", 2688);
	pstSrcRect.u32Height = rk_param_get_int("video.0:height", 1520);
	jpeg_width = rk_param_get_int("video.jpeg:width", 2688);
	jpeg_height = rk_param_get_int("video.jpeg:height", 1520);
	ret = RK_MPI_VPSS_GetChnFrame(0, 0, &stViFrame, 1000);

	if (ret == RK_SUCCESS) {
		// tde begin job
		hHandle = RK_TDE_BeginJob();
		if (RK_ERR_TDE_INVALID_HANDLE == hHandle) {
			LOG_ERROR("start job fail\n");
			RK_MPI_VPSS_ReleaseChnFrame(0, 0, &stViFrame);
			goto __failed;
		}
		// tde quick resize
		pstSrc.pMbBlk = stViFrame.stVFrame.pMbBlk;
		pstDst.pMbBlk = dstBlk;
		pstDst.u32Width = jpeg_width;
		pstDst.u32Height = jpeg_height;
		pstDstRect.u32Width = jpeg_width;
		pstDstRect.u32Height = jpeg_height;
		ret = RK_TDE_QuickResize(hHandle, &pstSrc, &pstSrcRect, &pstDst, &pstDstRect);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_TDE_QuickResize failed. err 0x%x\n", ret);
			RK_TDE_CancelJob(hHandle);
			RK_MPI_VPSS_ReleaseChnFrame(0, 0, &stViFrame);
			goto __failed;
		}
		// tde end job
		ret = RK_TDE_EndJob(hHandle, RK_FALSE, RK_TRUE, 10);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_TDE_EndJob failed. err 0x%x\n", ret);
			RK_TDE_CancelJob(hHandle);
			RK_MPI_VPSS_ReleaseChnFrame(0, 0, &stViFrame);
			goto __failed;
		}
		ret = RK_TDE_WaitForDone(hHandle);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_TDE_WaitForDone fail %x\n", ret);
		// set jpeg venc w,h
		ret = RK_MPI_VENC_GetChnAttr(JPEG_VENC_CHN, &pstChnAttr);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VENC_GetChnAttr fail %x\n", ret);
		pstChnAttr.stVencAttr.u32PicWidth = pstDst.u32Width;
		pstChnAttr.stVencAttr.u32PicHeight = pstDst.u32Height;
		ret = RK_MPI_VENC_SetChnAttr(JPEG_VENC_CHN, &pstChnAttr);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VENC_SetChnAttr fail %x\n", ret);
		// send frame to jpeg venc
		DstFrame.stVFrame.pMbBlk = dstBlk;
		DstFrame.stVFrame.u32Width = pstDst.u32Width;
		DstFrame.stVFrame.u32Height = pstDst.u32Height;
		DstFrame.stVFrame.u32VirWidth = pstDst.u32Width;
		DstFrame.stVFrame.u32VirHeight = pstDst.u32Height;

#if 0
		char file_name[128] = {0};
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		snprintf(file_name, 128, "/userdata/tde_%d%02d%02d%02d%02d%02d.yuv"
					, tm.tm_year + 1900, tm.tm_mon + 1
					, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
		FILE *dump_file = fopen(file_name, "wb");
		if (dump_file) {
			void *data = RK_MPI_MB_Handle2VirAddr(dstBlk);
			fwrite(data, 1, Dst_stMbPicCalResult.u32MBSize, dump_file);
			fflush(dump_file);
			fclose(dump_file);
		}
#endif
		ret = RK_MPI_VENC_SendFrame(JPEG_VENC_CHN, &DstFrame, 1000);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VENC_SendFrame fail %x\n", ret);
		// release the frame
		ret = RK_MPI_VPSS_ReleaseChnFrame(0, 0, &stViFrame);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VPSS_ReleaseChnFrame fail %x\n", ret);
	} else {
		LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", ret);
	}

__failed:
	RK_TDE_Close();
	RK_MPI_SYS_Free(dstBlk);

	return ret;
}

static void *rkipc_get_jpeg(void *arg) {
	LOG_DEBUG("#Start thread, arg:%p\n", arg);
	prctl(PR_SET_NAME, "RkipcGetJpeg", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int ret = 0, loopCount = 0;
	char file_name[128] = {0};
	char file_path[256];
	memset(&file_path, 0, sizeof(file_path));
	strcat(file_path, rk_param_get_string("storage:mount_path", "/userdata"));
	strcat(file_path, "/");
	strcat(file_path, rk_param_get_string("storage.0:folder_name", "video0"));

	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// INFO: This mutex just used for conditional variable, because if there is
		// no mutex offered, invocation of pthread_cond_wait() will be failed.
		pthread_mutex_lock(&g_capture_mutex);
		pthread_cond_wait(&g_capture_cond, &g_capture_mutex);
		pthread_mutex_unlock(&g_capture_mutex);
		if (!g_do_capture)
			continue;
		if (!g_video_run_)
			break;
#if 0 // combo
		ret = rkipc_resize_jpeg_by_tde();
		if (ret != RK_SUCCESS) {
			LOG_ERROR("tde failed %x\n", ret);
		}
#endif
		ret = RK_MPI_VENC_GetStream(JPEG_VENC_CHN, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			LOG_DEBUG("Count:%d, Len:%d, PTS is %" PRId64 ", enH264EType is %d\n", loopCount,
			          stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			          stFrame.pstPack->DataType.enH264EType);
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			snprintf(file_name, 128, "%s/%d%02d%02d%02d%02d%02d.jpeg", file_path, tm.tm_year + 1900,
			         tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			LOG_INFO("file_name is %s\n", file_name);
			FILE *fp = fopen(file_name, "wb");
			if (fp) {
				fwrite(data, 1, stFrame.pstPack->u32Len, fp);
				fflush(fp);
				fclose(fp);
			} else {
				LOG_ERROR("Failed to open file %s\n", file_name);
			}
			g_do_capture = 0;

			ret = RK_MPI_VENC_ReleaseStream(JPEG_VENC_CHN, &stFrame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
			}
			++loopCount;
		} else {
			LOG_ERROR("RK_MPI_VENC_GetStream timeout %x\n", ret);
		}
		RK_MPI_VENC_StopRecvFrame(JPEG_VENC_CHN);
	}
	if (stFrame.pstPack)
		free(stFrame.pstPack);

	return 0;
}

static void *rkipc_do_cycle_snapshot(void *arg) {
	int enable = 0;
	while (g_video_run_) {
		enable = rk_param_get_int("video.jpeg:enable_cycle_snapshot", 0);
		if (!enable)
			break;
		usleep(rk_param_get_int("video.jpeg:snapshot_interval_ms", 1000) * 1000);
		rk_take_photo();
	}
	return NULL;
}

static void start_all_threads() {
	int ret = 0;
	memset(thread_id_stack, 0, sizeof(thread_id_stack));
	thread_stack_top = -1;
	g_video_run_ = 1;
	TRACE_BEGIN();
	if (g_enable_venc_0) {
		venc_thread_0 = 0;
		ret = pthread_create(&venc_thread_0, NULL, rkipc_get_venc_0, NULL);
		if (ret == 0) {
			thread_id_stack[++thread_stack_top] = venc_thread_0;
			LOG_INFO("create venc0 thread tid: %#X\n", venc_thread_0);
		} else {
			LOG_ERROR("create venc0 thread failed: %d\n", ret);
		}
	}
	if (g_enable_venc_1) {
		venc_thread_1 = 0;
		ret = pthread_create(&venc_thread_1, NULL, rkipc_get_venc_1, NULL);
		if (ret == 0) {
			thread_id_stack[++thread_stack_top] = venc_thread_1;
			LOG_INFO("create venc1 thread tid: %#X\n", venc_thread_1);
		} else {
			LOG_ERROR("create venc1 thread failed: %d\n", ret);
		}
	}
	if (g_enable_venc_2) {
		venc_thread_2 = 0;
		ret = pthread_create(&venc_thread_2, NULL, rkipc_get_venc_2, NULL);
		if (ret == 0) {
			thread_id_stack[++thread_stack_top] = venc_thread_2;
			LOG_INFO("create venc2 thread tid: %#X\n", venc_thread_2);
		} else {
			LOG_ERROR("create venc2 thread failed: %d\n", ret);
		}
	}
	if (g_enable_ivs) {
		int md = rk_param_get_int("ivs:md", 0);
		int od = rk_param_get_int("ivs:od", 0);
		if (md || od) {
			ivs_get_result_thread_id = 0;
			ret = pthread_create(&ivs_get_result_thread_id, NULL, rkipc_ivs_get_results, NULL);
			if (ret == 0) {
				thread_id_stack[++thread_stack_top] = ivs_get_result_thread_id;
				LOG_INFO("create ivs thread tid: %#X\n", ivs_get_result_thread_id);
			} else {
				LOG_ERROR("create ivs thread failed: %d\n", ret);
			}
		}
	}
	if (g_enable_npu) {
		iva_get_frame_thread_id = 0;
		ret = pthread_create(&iva_get_frame_thread_id, NULL, rkipc_iva_get_frame, NULL);
		if (ret == 0) {
			thread_id_stack[++thread_stack_top] = iva_get_frame_thread_id;
			LOG_INFO("create iva thread tid: %#X\n", iva_get_frame_thread_id);
		} else {
			LOG_ERROR("create iva thread failed: %d\n", ret);
		}
	}
	if (g_enable_jpeg) {
		jpeg_venc_thread_id = 0;
		ret = pthread_create(&jpeg_venc_thread_id, NULL, rkipc_get_jpeg, NULL);
		if (ret == 0) {
			thread_id_stack[++thread_stack_top] = jpeg_venc_thread_id;
			LOG_INFO("create jpeg thread tid: %#X\n", jpeg_venc_thread_id);
		} else {
			LOG_ERROR("create jpeg thread failed: %d\n", ret);
		}
		if (rk_param_get_int("video.jpeg:enable_cycle_snapshot", 0)) {
			pthread_create(&cycle_snapshot_thread_id, NULL, rkipc_do_cycle_snapshot, NULL);
		}
	}
	TRACE_END();
}

static void stop_all_threads() {
	TRACE_BEGIN();
	g_video_run_ = 0;
	// INFO: cycle snapshot thread may not be destroy by user, so we need to destroy
	// it manually.
	if (g_enable_jpeg) {
		if (cycle_snapshot_thread_id) {
			pthread_join(cycle_snapshot_thread_id, NULL);
			cycle_snapshot_thread_id = 0;
		}
		pthread_cond_signal(&g_capture_cond);
	}
	for (int i = thread_stack_top; i >= 0 && i < 16; --i) {
		pthread_join(thread_id_stack[i], NULL);
		LOG_INFO("destroy thread tid: %#X\n", thread_id_stack[i]);
	}
	thread_stack_top = -1;
	memset(thread_id_stack, 0, sizeof(thread_id_stack));
	TRACE_END();
}

// export API
int rk_video_get_gop(int stream_id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:gop", stream_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_gop(int stream_id, int value) {
	char entry[128] = {'\0'};
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	tmp_output_data_type = rk_param_get_string(entry, "H.264");
	snprintf(entry, 127, "video.%d:rc_mode", stream_id);
	tmp_rc_mode = rk_param_get_string(entry, "CBR");
	if (!strcmp(tmp_output_data_type, "H.264")) {
		if (!strcmp(tmp_rc_mode, "CBR"))
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = value;
		else
			venc_chn_attr.stRcAttr.stH264Vbr.u32Gop = value;
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		if (!strcmp(tmp_rc_mode, "CBR"))
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = value;
		else
			venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = value;
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);
	snprintf(entry, 127, "video.%d:gop", stream_id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_video_get_max_rate(int stream_id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:max_rate", stream_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_max_rate(int stream_id, int value) {
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	tmp_output_data_type = rk_param_get_string(entry, "H.264");
	snprintf(entry, 127, "video.%d:rc_mode", stream_id);
	tmp_rc_mode = rk_param_get_string(entry, "CBR");
	if (!strcmp(tmp_output_data_type, "H.264")) {
		if (!strcmp(tmp_rc_mode, "CBR"))
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = value;
		else
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = value;
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		if (!strcmp(tmp_rc_mode, "CBR"))
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = value;
		else
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = value;
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);
	snprintf(entry, 127, "video.%d:max_rate", stream_id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_video_get_RC_mode(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:rc_mode", stream_id);
	*value = rk_param_get_string(entry, "CBR");

	return 0;
}

int rk_video_set_RC_mode(int stream_id, const char *value) {
	char entry_output_data_type[128] = {'\0'};
	char entry_gop[128] = {'\0'};
	char entry_max_rate[128] = {'\0'};
	char entry_dst_frame_rate_den[128] = {'\0'};
	char entry_dst_frame_rate_num[128] = {'\0'};
	char entry_src_frame_rate_den[128] = {'\0'};
	char entry_src_frame_rate_num[128] = {'\0'};
	char entry_rc_mode[128] = {'\0'};
	snprintf(entry_output_data_type, 127, "video.%d:output_data_type", stream_id);
	snprintf(entry_gop, 127, "video.%d:gop", stream_id);
	snprintf(entry_max_rate, 127, "video.%d:max_rate", stream_id);
	snprintf(entry_dst_frame_rate_den, 127, "video.%d:dst_frame_rate_den", stream_id);
	snprintf(entry_dst_frame_rate_num, 127, "video.%d:dst_frame_rate_num", stream_id);
	snprintf(entry_src_frame_rate_den, 127, "video.%d:src_frame_rate_den", stream_id);
	snprintf(entry_src_frame_rate_num, 127, "video.%d:src_frame_rate_num", stream_id);
	snprintf(entry_rc_mode, 127, "video.%d:rc_mode", stream_id);

	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
	tmp_output_data_type = rk_param_get_string(entry_output_data_type, "H.264");
	if (!strcmp(tmp_output_data_type, "H.264")) {
		if (!strcmp(value, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int(entry_gop, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = rk_param_get_int(entry_max_rate, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen =
			    rk_param_get_int(entry_dst_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum =
			    rk_param_get_int(entry_dst_frame_rate_num, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen =
			    rk_param_get_int(entry_src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum =
			    rk_param_get_int(entry_src_frame_rate_num, -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
			venc_chn_attr.stRcAttr.stH264Vbr.u32Gop = rk_param_get_int(entry_gop, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = rk_param_get_int(entry_max_rate, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen =
			    rk_param_get_int(entry_dst_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum =
			    rk_param_get_int(entry_dst_frame_rate_num, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen =
			    rk_param_get_int(entry_src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum =
			    rk_param_get_int(entry_src_frame_rate_num, -1);
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		if (!strcmp(value, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int(entry_gop, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = rk_param_get_int(entry_max_rate, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen =
			    rk_param_get_int(entry_dst_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum =
			    rk_param_get_int(entry_dst_frame_rate_num, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateDen =
			    rk_param_get_int(entry_src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateNum =
			    rk_param_get_int(entry_src_frame_rate_num, -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
			venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = rk_param_get_int(entry_gop, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = rk_param_get_int(entry_max_rate, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen =
			    rk_param_get_int(entry_dst_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum =
			    rk_param_get_int(entry_dst_frame_rate_num, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen =
			    rk_param_get_int(entry_src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum =
			    rk_param_get_int(entry_src_frame_rate_num, -1);
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);
	rk_param_set_string(entry_rc_mode, value);

	return 0;
}

int rk_video_get_output_data_type(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	*value = rk_param_get_string(entry, "H.265");

	return 0;
}

int rk_video_set_output_data_type(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	rk_param_set_string(entry, value);
	rk_video_restart();

	return 0;
}

int rk_video_get_rc_quality(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:rc_quality", stream_id);
	*value = rk_param_get_string(entry, "high");

	return 0;
}

int rk_video_set_rc_quality(int stream_id, const char *value) {
	char entry_rc_quality[128] = {'\0'};
	char entry_output_data_type[128] = {'\0'};

	snprintf(entry_rc_quality, 127, "video.%d:rc_quality", stream_id);
	snprintf(entry_output_data_type, 127, "video.%d:output_data_type", stream_id);
	tmp_output_data_type = rk_param_get_string(entry_output_data_type, "H.264");

	VENC_RC_PARAM_S venc_rc_param;
	RK_MPI_VENC_GetRcParam(stream_id, &venc_rc_param);
	if (!strcmp(tmp_output_data_type, "H.264")) {
		if (!strcmp(value, "highest")) {
			venc_rc_param.stParamH264.u32MinQp = 10;
		} else if (!strcmp(value, "higher")) {
			venc_rc_param.stParamH264.u32MinQp = 15;
		} else if (!strcmp(value, "high")) {
			venc_rc_param.stParamH264.u32MinQp = 20;
		} else if (!strcmp(value, "medium")) {
			venc_rc_param.stParamH264.u32MinQp = 25;
		} else if (!strcmp(value, "low")) {
			venc_rc_param.stParamH264.u32MinQp = 30;
		} else if (!strcmp(value, "lower")) {
			venc_rc_param.stParamH264.u32MinQp = 35;
		} else {
			venc_rc_param.stParamH264.u32MinQp = 40;
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		if (!strcmp(value, "highest")) {
			venc_rc_param.stParamH265.u32MinQp = 10;
		} else if (!strcmp(value, "higher")) {
			venc_rc_param.stParamH265.u32MinQp = 15;
		} else if (!strcmp(value, "high")) {
			venc_rc_param.stParamH265.u32MinQp = 20;
		} else if (!strcmp(value, "medium")) {
			venc_rc_param.stParamH265.u32MinQp = 25;
		} else if (!strcmp(value, "low")) {
			venc_rc_param.stParamH265.u32MinQp = 30;
		} else if (!strcmp(value, "lower")) {
			venc_rc_param.stParamH265.u32MinQp = 35;
		} else {
			venc_rc_param.stParamH265.u32MinQp = 40;
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetRcParam(stream_id, &venc_rc_param);
	rk_param_set_string(entry_rc_quality, value);

	return 0;
}

int rk_video_get_smart(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:smart", stream_id);
	*value = rk_param_get_string(entry, "close");

	return 0;
}

int rk_video_set_smart(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:smart", stream_id);
	rk_param_set_string(entry, value);
	rk_video_restart();

	return 0;
}

int rk_video_get_gop_mode(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:gop_mode", stream_id);
	*value = rk_param_get_string(entry, "close");

	return 0;
}

int rk_video_set_gop_mode(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:gop_mode", stream_id);
	rk_param_set_string(entry, value);
	rk_video_restart();

	return 0;
}

int rk_video_get_stream_type(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:stream_type", stream_id);
	*value = rk_param_get_string(entry, "mainStream");

	return 0;
}

int rk_video_set_stream_type(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:stream_type", stream_id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_video_get_h264_profile(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:h264_profile", stream_id);
	*value = rk_param_get_string(entry, "high");

	return 0;
}

int rk_video_set_h264_profile(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:h264_profile", stream_id);
	rk_param_set_string(entry, value);
	rk_video_restart();

	return 0;
}

int rk_video_get_resolution(int stream_id, char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:width", stream_id);
	int width = rk_param_get_int(entry, 0);
	snprintf(entry, 127, "video.%d:height", stream_id);
	int height = rk_param_get_int(entry, 0);
	sprintf(*value, "%d*%d", width, height);

	return 0;
}

int rk_video_set_resolution(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	int width, height;

	sscanf(value, "%d*%d", &width, &height);
	LOG_INFO("value is %s, width is %d, height is %d\n", value, width, height);
	snprintf(entry, 127, "video.%d:width", stream_id);
	rk_param_set_int(entry, width);
	snprintf(entry, 127, "video.%d:height", stream_id);
	rk_param_set_int(entry, height);
	// combo
	if (g_enable_jpeg && (stream_id == 0)) {
		snprintf(entry, 127, "video.jpeg:width");
		rk_param_set_int(entry, width);
		snprintf(entry, 127, "video.jpeg:height");
		rk_param_set_int(entry, height);
	}

	rk_video_restart();

	return 0;
}

int rk_video_get_frame_rate(int stream_id, char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:dst_frame_rate_den", stream_id);
	int den = rk_param_get_int(entry, -1);
	snprintf(entry, 127, "video.%d:dst_frame_rate_num", stream_id);
	int num = rk_param_get_int(entry, -1);
	if (den == 1)
		sprintf(*value, "%d", num);
	else
		sprintf(*value, "%d/%d", num, den);

	return 0;
}

int rk_video_set_frame_rate(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	int den, num;
	if (strchr(value, '/') == NULL) {
		den = 1;
		sscanf(value, "%d", &num);
	} else {
		sscanf(value, "%d/%d", &num, &den);
	}
	LOG_DEBUG("num is %d, den is %d\n", num, den);

	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	tmp_output_data_type = rk_param_get_string(entry, "H.264");
	snprintf(entry, 127, "video.%d:rc_mode", stream_id);
	tmp_rc_mode = rk_param_get_string(entry, "CBR");
	if (!strcmp(tmp_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_AVC;
		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = den;
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = num;
		} else {
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen = den;
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum = num;
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_HEVC;
		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen = den;
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum = num;
		} else {
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen = den;
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum = num;
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	int ret = RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);
	if (ret != RK_SUCCESS)
		LOG_ERROR("set frame rate %d/%d failed!\n", num, den);
	else
		LOG_DEBUG("set frame rate %d/%d success!\n", num, den);

	snprintf(entry, 127, "video.%d:dst_frame_rate_den", stream_id);
	rk_param_set_int(entry, den);
	snprintf(entry, 127, "video.%d:dst_frame_rate_num", stream_id);
	rk_param_set_int(entry, num);

	return 0;
}

int rk_video_get_frame_rate_in(int stream_id, char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:src_frame_rate_den", stream_id);
	int den = rk_param_get_int(entry, -1);
	snprintf(entry, 127, "video.%d:src_frame_rate_num", stream_id);
	int num = rk_param_get_int(entry, -1);
	if (den == 1)
		sprintf(*value, "%d", num);
	else
		sprintf(*value, "%d/%d", num, den);

	return 0;
}

int rk_video_reset_frame_rate(int stream_id) {
	int ret = 0;
	char *value = malloc(20);
	ret |= rk_video_get_frame_rate(stream_id, &value);
	ret |= rk_video_set_frame_rate(stream_id, value);
	free(value);

	return 0;
}

int rk_video_set_frame_rate_in(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	int den, num;
	if (strchr(value, '/') == NULL) {
		den = 1;
		sscanf(value, "%d", &num);
	} else {
		sscanf(value, "%d/%d", &num, &den);
	}
	LOG_INFO("num is %d, den is %d\n", num, den);
	snprintf(entry, 127, "video.%d:src_frame_rate_den", stream_id);
	rk_param_set_int(entry, den);
	snprintf(entry, 127, "video.%d:src_frame_rate_num", stream_id);
	rk_param_set_int(entry, num);
	rk_video_restart();

	return 0;
}

int rk_video_get_enable_cycle_snapshot(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:enable_cycle_snapshot");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_enable_cycle_snapshot(int value) {
	TRACE_BEGIN();
	char entry[128] = {'\0'};

	if (!g_enable_jpeg) {
		LOG_INFO("jpeg is not support.\n");
		return RK_FAILURE;
	}
	LOG_INFO("%s\n", value ? "enable" : "disable");
	// Set new value
	snprintf(entry, 127, "video.jpeg:enable_cycle_snapshot");
	rk_param_set_int(entry, value);
	// Create/Destroy thread.
	if (value && cycle_snapshot_thread_id == 0) {
		// Thread is not create, start cycle snapshot thread.
		pthread_create(&cycle_snapshot_thread_id, NULL, rkipc_do_cycle_snapshot, NULL);
	} else if (!value && cycle_snapshot_thread_id != 0) {
		// Thread has been created, stop cycle snapshot thread.
		pthread_join(cycle_snapshot_thread_id, NULL);
		cycle_snapshot_thread_id = 0;
	}
	TRACE_END();

	return 0;
}

int rk_video_get_image_quality(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:jpeg_qfactor");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_image_quality(int value) {
	int ret = 0;
	char entry[128] = {'\0'};

	TRACE_BEGIN();
	LOG_INFO("quality %d\n", value);
	snprintf(entry, 127, "video.jpeg:jpeg_qfactor");
	rk_param_set_int(entry, value);
	VENC_JPEG_PARAM_S stJpegParam;
	memset(&stJpegParam, 0, sizeof(stJpegParam));
	stJpegParam.u32Qfactor = rk_param_get_int("video.jpeg:jpeg_qfactor", -1);
	ret = RK_MPI_VENC_SetJpegParam(JPEG_VENC_CHN, &stJpegParam);
	if (ret != RK_SUCCESS)
		LOG_ERROR("RK_MPI_VENC_SetJpegParam failed because %#X\n", ret);
	TRACE_END();

	return 0;
}

int rk_video_get_snapshot_interval_ms(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:snapshot_interval_ms");
	*value = rk_param_get_int(entry, 0);

	return 0;
}

int rk_video_set_snapshot_interval_ms(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:snapshot_interval_ms");
	rk_param_set_int(entry, value);
	LOG_INFO("inerval %d ms\n", value);

	return 0;
}

int rk_video_get_jpeg_resolution(char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:width");
	int width = rk_param_get_int(entry, 0);
	snprintf(entry, 127, "video.jpeg:height");
	int height = rk_param_get_int(entry, 0);
	sprintf(*value, "%d*%d", width, height);

	return 0;
}

int rk_video_set_jpeg_resolution(const char *value) {
	int width, height, ret;
	char entry[128] = {'\0'};
	VENC_CHN_ATTR_S jpeg_chn_attr;

	TRACE_BEGIN();
#if 0
	sscanf(value, "%d*%d", &width, &height);
	snprintf(entry, 127, "video.jpeg:width");
	rk_param_set_int(entry, width);
	snprintf(entry, 127, "video.jpeg:height");
	rk_param_set_int(entry, height);
	LOG_INFO("width %d, height %d\n", width, height);
	ret = RK_MPI_VENC_GetChnAttr(JPEG_VENC_CHN, &jpeg_chn_attr);
	if (ret == RK_SUCCESS) {
		jpeg_chn_attr.stVencAttr.u32PicWidth = width;
		jpeg_chn_attr.stVencAttr.u32PicHeight = height;
		jpeg_chn_attr.stVencAttr.u32VirWidth = width;
		jpeg_chn_attr.stVencAttr.u32VirHeight = height;
		ret = RK_MPI_VENC_SetChnAttr(JPEG_VENC_CHN, &jpeg_chn_attr);
		if (ret != RK_SUCCESS)
			LOG_ERROR("Failed to set attribute of jpeg channel because %#X\n", ret);
	} else {
		LOG_ERROR("Failed to get attribute of jpeg channel because %#X\n", ret);
	}
#else
	LOG_INFO(
	    "1106 aiisp combo, jpeg resolution must be consistent with the main stream resolution\n");
#endif
	TRACE_END();

	return 0;
}

int rk_take_photo() {
	if (g_do_capture) {
		LOG_WARN("the last photo was not completed\n");
		return RK_FAILURE;
	}
	if (rkipc_storage_dev_mount_status_get() != DISK_MOUNTED) {
		LOG_WARN("dev not mount\n");
		return RK_FAILURE;
	}
	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = 1;
	RK_MPI_VENC_StartRecvFrame(JPEG_VENC_CHN, &stRecvParam);
	g_do_capture = 1;
	pthread_cond_signal(&g_capture_cond);

	return RK_SUCCESS;
}

int rk_roi_set(roi_data_s *roi_data) {
	int ret = 0;
	int venc_chn_num = 0;
	int rotation, video_width, video_height;
	int origin_x, origin_y;
	VENC_ROI_ATTR_S pstRoiAttr;
	char entry[128] = {'\0'};

	pstRoiAttr.u32Index = roi_data->id;
	pstRoiAttr.bEnable = roi_data->enabled;
	pstRoiAttr.bAbsQp = RK_FALSE;
	pstRoiAttr.bIntra = RK_FALSE;
	pstRoiAttr.stRect.s32X = roi_data->position_x;
	pstRoiAttr.stRect.s32Y = roi_data->position_y;
	pstRoiAttr.stRect.u32Width = roi_data->width;
	pstRoiAttr.stRect.u32Height = roi_data->height;
	switch (roi_data->quality_level) {
	case 6:
		pstRoiAttr.s32Qp = -16;
		break;
	case 5:
		pstRoiAttr.s32Qp = -14;
		break;
	case 4:
		pstRoiAttr.s32Qp = -12;
		break;
	case 3:
		pstRoiAttr.s32Qp = -10;
		break;
	case 2:
		pstRoiAttr.s32Qp = -8;
		break;
	case 1:
	default:
		pstRoiAttr.s32Qp = -6;
	}

	if (!strcmp(roi_data->stream_type, "mainStream") &&
	    rk_param_get_int("video.source:enable_venc_0", 0)) {
		venc_chn_num = 0;
	} else if (!strcmp(roi_data->stream_type, "subStream") &&
	           rk_param_get_int("video.source:enable_venc_1", 0)) {
		venc_chn_num = 1;
	} else if (!strcmp(roi_data->stream_type, "thirdStream") &&
	           rk_param_get_int("video.source:enable_venc_2", 0)) {
		venc_chn_num = 2;
	} else {
		LOG_DEBUG("%s is not exit\n", roi_data->stream_type);
		return -1;
	}

	if (pstRoiAttr.stRect.u32Height != 0 && pstRoiAttr.stRect.u32Width != 0 && roi_data->enabled) {
		snprintf(entry, 127, "video.%d:width", venc_chn_num);
		video_width = rk_param_get_int(entry, 0);
		snprintf(entry, 127, "video.%d:height", venc_chn_num);
		video_height = rk_param_get_int(entry, 0);
		rotation = rk_param_get_int("video.source:rotation", 0);
		origin_x = pstRoiAttr.stRect.s32X;
		origin_y = pstRoiAttr.stRect.s32Y;
		if (video_height < (origin_y + pstRoiAttr.stRect.u32Height))
			LOG_ERROR("illegal params! video height(%d) < y(%d) + h(%d)", video_height, origin_y,
			          pstRoiAttr.stRect.u32Height);
		if (video_width < (origin_x + pstRoiAttr.stRect.u32Width))
			LOG_ERROR("illegal params! video width(%d) < x(%d) + w(%d)", video_width, origin_x,
			          pstRoiAttr.stRect.u32Width);
		switch (rotation) {
		case 90:
			pstRoiAttr.stRect.s32X = video_height - origin_y - pstRoiAttr.stRect.u32Height;
			pstRoiAttr.stRect.s32Y = origin_x;
			RKIPC_SWAP(pstRoiAttr.stRect.u32Width, pstRoiAttr.stRect.u32Height);
			break;
		case 270:
			pstRoiAttr.stRect.s32X = origin_y;
			pstRoiAttr.stRect.s32Y = video_width - origin_x - pstRoiAttr.stRect.u32Width;
			RKIPC_SWAP(pstRoiAttr.stRect.u32Width, pstRoiAttr.stRect.u32Height);
			break;
		case 180:
			pstRoiAttr.stRect.s32X = video_width - origin_x - pstRoiAttr.stRect.u32Width;
			pstRoiAttr.stRect.s32Y = video_height - origin_y - pstRoiAttr.stRect.u32Height;
			break;
		default:
			break;
		}
		LOG_INFO("id %d, rotation %d, from [x(%d),y(%d),w(%d),h(%d)] "
		         "to [x(%d),y(%d),w(%d),h(%d)]\n",
		         roi_data->id, rotation, roi_data->position_x, roi_data->position_y,
		         roi_data->width, roi_data->height, pstRoiAttr.stRect.s32X, pstRoiAttr.stRect.s32Y,
		         pstRoiAttr.stRect.u32Width, pstRoiAttr.stRect.u32Height);
	}

	ret = RK_MPI_VENC_SetRoiAttr(venc_chn_num, &pstRoiAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_VENC_SetRoiAttr to venc %d failed with %#x\n", venc_chn_num, ret);
		return RK_FAILURE;
	}

	return ret;
}

int rk_region_clip_set(int venc_chn_num, region_clip_data_s *region_clip_data) {
	int ret = 0;
	VENC_CHN_PARAM_S stParam;

	RK_MPI_VENC_GetChnParam(venc_chn_num, &stParam);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_VENC_GetChnParam to venc failed with %#x\n", ret);
		return RK_FAILURE;
	}
	LOG_INFO("RK_MPI_VENC_GetChnParam to venc success\n");
	LOG_INFO("venc_chn_num is %d\n", venc_chn_num);
	if (region_clip_data->enabled)
		stParam.stCropCfg.enCropType = VENC_CROP_ONLY;
	else
		stParam.stCropCfg.enCropType = VENC_CROP_NONE;
	stParam.stCropCfg.stCropRect.s32X = region_clip_data->position_x;
	stParam.stCropCfg.stCropRect.s32Y = region_clip_data->position_y;
	stParam.stCropCfg.stCropRect.u32Width = region_clip_data->width;
	stParam.stCropCfg.stCropRect.u32Height = region_clip_data->height;
	// LOG_INFO("xywh is %d,%d,%d,%d\n", stParam.stCropCfg.stCropRect.s32X,
	// stParam.stCropCfg.stCropRect.s32Y, stParam.stCropCfg.stCropRect.u32Width,
	// stParam.stCropCfg.stCropRect.u32Height);
	ret = RK_MPI_VENC_SetChnParam(venc_chn_num, &stParam);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_VENC_SetChnParam to venc failed with %#x\n", ret);
		return RK_FAILURE;
	}
	LOG_INFO("RK_MPI_VENC_SetChnParam to venc success\n");

	return ret;
}

int rk_video_get_rotation(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.source:rotation");
	*value = rk_param_get_int(entry, 0);

	return 0;
}

int rk_video_set_rotation(int value) {
	LOG_INFO("value is %d\n", value);
	ROTATION_E rotation = ROTATION_0, cur_rotation = ROTATION_0;
	int ret = 0;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.source:rotation");
	rk_param_set_int(entry, value);

	if (value == 0) {
		rotation = ROTATION_0;
	} else if (value == 90) {
		rotation = ROTATION_90;
	} else if (value == 180) {
		rotation = ROTATION_180;
	} else if (value == 270) {
		rotation = ROTATION_270;
	}

	// ret = RK_MPI_VPSS_SetGrpRotation(1, rotation);
	// if (ret != RK_SUCCESS)
	// 	LOG_ERROR("vpss: set rotation failed because %#X\n", ret);
	// Do rotation just in venc channels.
	if (g_enable_venc_0) {
		ret = RK_MPI_VENC_GetChnRotation(0, &cur_rotation);
		if (ret == RK_SUCCESS && cur_rotation != rotation) {
			ret = RK_MPI_VENC_SetChnRotation(0, rotation);
			if (ret != RK_SUCCESS)
				LOG_ERROR("venc0: set rotation failed because %#X\n", ret);
		} else {
			LOG_ERROR("venc0: get current rotation failed because %#X\n", ret);
		}
	}
	if (g_enable_venc_1) {
		ret = RK_MPI_VENC_GetChnRotation(1, &cur_rotation);
		if (ret == RK_SUCCESS && cur_rotation != rotation) {
			ret = RK_MPI_VENC_SetChnRotation(1, rotation);
			if (ret != RK_SUCCESS)
				LOG_ERROR("venc1: set rotation failed because %#X\n", ret);
		} else {
			LOG_ERROR("venc1: get current rotation failed because %#X\n", ret);
		}
	}
	if (g_enable_venc_2) {
		ret = RK_MPI_VENC_GetChnRotation(2, &cur_rotation);
		if (ret == RK_SUCCESS && cur_rotation != rotation) {
			ret = RK_MPI_VENC_SetChnRotation(2, rotation);
			if (ret != RK_SUCCESS)
				LOG_ERROR("venc2: set rotation failed because %#X\n", ret);
		} else {
			LOG_ERROR("venc2: get current rotation failed because %#X\n", ret);
		}
	}
	if (g_enable_jpeg) {
		ret = RK_MPI_VENC_GetChnRotation(JPEG_VENC_CHN, &cur_rotation);
		if (ret == RK_SUCCESS && cur_rotation != rotation) {
			ret = RK_MPI_VENC_SetChnRotation(JPEG_VENC_CHN, rotation);
			if (ret != RK_SUCCESS)
				LOG_ERROR("jpeg: set rotation failed because %#X\n", ret);
		} else {
			LOG_ERROR("jpeg: get current rotation failed because %#X\n", ret);
		}
	}

	rk_roi_set_all();
	// update osd info, cover currently attaches to VI
	rk_osd_privacy_mask_restart();
	rk_region_clip_set_all();

	return 0;
}

int rk_video_init() {
	LOG_INFO("begin\n");
	int ret = 0;
	g_enable_venc_0 = rk_param_get_int("video.source:enable_venc_0", 1);
	g_enable_venc_1 = rk_param_get_int("video.source:enable_venc_1", 1);
	g_enable_venc_2 = rk_param_get_int("video.source:enable_venc_2", 1);
	g_enable_jpeg = rk_param_get_int("video.source:enable_jpeg", 1);
	g_enable_ivs = rk_param_get_int("video.source:enable_ivs", 1);
	g_enable_npu = rk_param_get_int("video.source:enable_npu", 1);
	g_enable_aiisp = rk_param_get_int("video.source:enable_aiisp", 1);

	ret |= rkipc_vi_dev_init();
	ret |= rkipc_vi_chn_init();
	ret |= rkipc_vpss_init();
	if (g_enable_venc_0)
		ret |= rkipc_venc_0_init();
	if (g_enable_venc_1)
		ret |= rkipc_venc_1_init();
	if (g_enable_venc_2)
		ret |= rkipc_venc_2_init();
	if (g_enable_jpeg)
		ret |= rkipc_jpeg_init();
	if (g_enable_ivs)
		rkipc_ivs_init();
	ret |= rkipc_osd_init();
	ret |= rkipc_bind_init();
	start_all_threads();

	ret |= rkipc_rtsp_init(RTSP_URL_0, RTSP_URL_1, RTSP_URL_2);
	ret |= rkipc_rtmp_init();

	rk_roi_set_callback_register(rk_roi_set);
	ret |= rk_roi_set_all();
	rk_region_clip_set_callback_register(rk_region_clip_set);
	ret |= rk_region_clip_set_all();
	LOG_INFO("over\n");

	return ret;
}

int rk_video_deinit() {
	LOG_INFO("%s\n", __func__);
	int ret = 0;

	rk_region_clip_set_callback_register(NULL);
	rk_roi_set_callback_register(NULL);

	ret |= rkipc_rtmp_deinit();
	ret |= rkipc_rtsp_deinit();

	stop_all_threads();
	ret |= rkipc_bind_deinit();
	ret |= rkipc_osd_deinit();
	if (g_enable_ivs)
		ret |= rkipc_ivs_deinit();
	if (g_enable_jpeg)
		ret |= rkipc_jpeg_deinit();
	if (g_enable_venc_2)
		ret |= rkipc_venc_2_deinit();
	if (g_enable_venc_1)
		ret |= rkipc_venc_1_deinit();
	if (g_enable_venc_0)
		ret |= rkipc_venc_0_deinit();
	ret |= rkipc_vpss_deinit();
	ret |= rkipc_vi_chn_deinit();
	ret |= rkipc_vi_dev_deinit();

	return ret;
}

extern char *rkipc_iq_file_path_;
int rk_video_restart() {
	int ret;
	ret = rk_storage_deinit();
	ret = rk_video_deinit();
	if (rk_param_get_int("video.source:enable_aiq", 1))
		ret |= rk_isp_deinit(0);
	malloc_trim(0);
	if (rk_param_get_int("video.source:enable_aiq", 1)) {
		ret |= rk_isp_init(0, rkipc_iq_file_path_);
		if (rk_param_get_int("isp:init_form_ini", 1))
			ret |= rk_isp_set_from_ini(0);
	}
	ret |= rk_video_init();
	ret |= rk_storage_init();

	return ret;
}
