// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "video.h"
#include "common.h"
#include "isp.h"
#include "rkbar_scan_api.h"
#include "rtsp.h"
#include "tuya_ipc.h"

#include <fcntl.h>
#include <inttypes.h> // PRId64
#include <linux/input.h>
#include <signal.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <rk_debug.h>
#include <rk_mpi_mb.h>
#include <rk_mpi_rgn.h>
#include <rk_mpi_sys.h>
#include <rk_mpi_venc.h>
#include <rk_mpi_vi.h>
#include <rk_mpi_vo.h>
#include <rk_mpi_vpss.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "video.c"

// VI_0-->VENC-->RTSP/tuya_clound-->|preview| phone_app
// VI_0-->QR_code_recognition
#define VIDEO_PIPE_0 0
// VI_1-->face_recognize-->draw-->VO
#define VIDEO_PIPE_1 1
#define VPSS_ROTATE 6

#define RV1126_VO_DEV_MIPI 0
#define RV1126_VOP_LAYER_CLUSTER0 0

#define RTSP_URL_0 "/live/0"

static int capture_one = 0;
static int retry_time = 0;
static int g_video_run_ = 1;
static int pipe_id_ = 0;
static int dev_id_ = 0;
static int g_rtmp_start = 0;
static const char *g_output_data_type;
static const char *g_rc_mode;
static const char *g_h264_profile;
static const char *g_smart;
static pthread_t venc_thread_id;

MPP_CHN_S pipe_0_vi_chn, pipe_0_venc_chn;
MPP_CHN_S pipe_1_vi_chn, vpss_rotate_chn, pipe_1_vo_chn;

static VO_DEV VoLayer = RV1126_VOP_LAYER_CLUSTER0;

static void *rkipc_get_venc_0(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	// FILE *fp = fopen("/tmp/venc.h264", "wb");
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_0, &stFrame, 2500);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			// fwrite(data, 1, stFrame.pstPack->u32Len, fp);
			// fflush(fp);
			// LOG_INFO("Count:%d, Len:%d, PTS is %lld, enH264EType is %d\n",
			// loopCount,
			// stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			// stFrame.pstPack->DataType.enH264EType);

			rkipc_rtsp_write_video_frame(0, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
			if ((stFrame.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
			    (stFrame.pstPack->DataType.enH264EType == H264E_NALU_ISLICE) ||
			    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_IDRSLICE) ||
			    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_ISLICE)) {
				rk_tuya_push_video(data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS, 1);
			} else {
				rk_tuya_push_video(data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS, 0);
			}

			// 7.release the frame
			ret = RK_MPI_VENC_ReleaseStream(VIDEO_PIPE_0, &stFrame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
			}
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VENC_GetStream timeout %x\n", ret);
		}
	}
	if (stFrame.pstPack)
		free(stFrame.pstPack);
	// if (fp)
	//  fclose(fp);

	return 0;
}

int rkipc_vi_dev_init() {
	LOG_INFO("%s\n", __func__);
	int ret = 0;
	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	memset(&stDevAttr, 0, sizeof(stDevAttr));
	memset(&stBindPipe, 0, sizeof(stBindPipe));
	// 0. get dev config status
	ret = RK_MPI_VI_GetDevAttr(dev_id_, &stDevAttr);
	if (ret == RK_ERR_VI_NOT_CONFIG) {
		// 0-1.config dev
		ret = RK_MPI_VI_SetDevAttr(dev_id_, &stDevAttr);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_MPI_VI_SetDevAttr %x\n", ret);
			return -1;
		}
	} else {
		LOG_ERROR("RK_MPI_VI_SetDevAttr already\n");
	}
	// 1.get dev enable status
	ret = RK_MPI_VI_GetDevIsEnable(dev_id_);
	if (ret != RK_SUCCESS) {
		// 1-2.enable dev
		ret = RK_MPI_VI_EnableDev(dev_id_);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_MPI_VI_EnableDev %x\n", ret);
			return -1;
		}
		// 1-3.bind dev/pipe
		stBindPipe.u32Num = pipe_id_;
		stBindPipe.PipeId[0] = pipe_id_;
		ret = RK_MPI_VI_SetDevBindPipe(dev_id_, &stBindPipe);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_MPI_VI_SetDevBindPipe %x\n", ret);
			return -1;
		}
	} else {
		LOG_ERROR("RK_MPI_VI_EnableDev already\n");
	}

	return 0;
}

int rkipc_vi_dev_deinit() {
	RK_MPI_VI_DisableDev(pipe_id_);

	return 0;
}

int rkipc_pipe_0_init() {
	int video_width = rk_param_get_int("video.0:width", 1920);
	int video_height = rk_param_get_int("video.0:height", 1080);
	const char *video_device_name = rk_param_get_string("video.0:src_node", "rkispp_scale0");
	int buf_cnt = 3;
	int ret = 0;

	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	LOG_INFO("video_device_name = %s\n", video_device_name);
	memcpy(vi_chn_attr.stIspOpt.aEntityName, video_device_name, strlen(video_device_name));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.u32Depth = 1; // not bind, can get buffer
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_0, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_0);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%d\n", ret);
		return -1;
	}
	// pthread_t thread_id;
	// pthread_create(&thread_id, NULL, rkipc_get_vi_0, NULL);
	// VENC init
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	g_output_data_type = rk_param_get_string("video.0:output_data_type", NULL);
	g_rc_mode = rk_param_get_string("video.0:rc_mode", NULL);
	g_h264_profile = rk_param_get_string("video.0:h264_profile", NULL);
	if ((g_output_data_type == NULL) || (g_rc_mode == NULL)) {
		LOG_ERROR("g_output_data_type or g_rc_mode is NULL\n");
		return -1;
	}
	LOG_INFO("g_output_data_type is %s, g_rc_mode is %s, g_h264_profile is %s\n",
	         g_output_data_type, g_rc_mode, g_h264_profile);
	if (!strcmp(g_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_AVC;

		if (!strcmp(g_h264_profile, "high"))
			venc_chn_attr.stVencAttr.u32Profile = 100;
		else if (!strcmp(g_h264_profile, "main"))
			venc_chn_attr.stVencAttr.u32Profile = 77;
		else if (!strcmp(g_h264_profile, "baseline"))
			venc_chn_attr.stVencAttr.u32Profile = 66;
		else
			LOG_ERROR("g_h264_profile is %s\n", g_h264_profile);

		if (!strcmp(g_rc_mode, "CBR")) {
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
	} else if (!strcmp(g_output_data_type, "H.265")) {
		venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_HEVC;
		if (!strcmp(g_rc_mode, "CBR")) {
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
		LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
		return -1;
	}
	g_smart = rk_param_get_string("video.0:smart", NULL);
	if (!strcmp(g_rc_mode, "open")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
	} else {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.0:gop", -1);

	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	venc_chn_attr.stVencAttr.u32StreamBufCnt = buf_cnt;
	venc_chn_attr.stVencAttr.u32BufSize = video_width * video_height * 3 / 2;
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_0, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%d\n", ret);
		return -1;
	}
	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_0, &stRecvParam);

	pthread_create(&venc_thread_id, NULL, rkipc_get_venc_0, NULL);

	// rockit可以绑定后get
	pipe_0_venc_chn.enModId = RK_ID_VENC;
	pipe_0_venc_chn.s32DevId = pipe_id_;
	pipe_0_venc_chn.s32ChnId = VIDEO_PIPE_0;

	pipe_0_vi_chn.enModId = RK_ID_VI;
	pipe_0_vi_chn.s32DevId = pipe_id_;
	pipe_0_vi_chn.s32ChnId = VIDEO_PIPE_0;
	ret = RK_MPI_SYS_Bind(&pipe_0_vi_chn, &pipe_0_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: Bind VI and VENC error! ret=%d\n", ret);
		return -1;
	}

	return 0;
}

int rkipc_pipe_0_deinit() {
	int ret = 0;
	// unbind first
	ret = RK_MPI_SYS_UnBind(&pipe_0_vi_chn, &pipe_0_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: UnBind VI and VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("UnBind VI and VENC success\n");
	// destroy venc before vi
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_0);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VENC success\n");
	// destroy vi
	ret = RK_MPI_VI_DisableChn(pipe_id_, VIDEO_PIPE_0);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VI error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VI success\n");

	return 0;
}

int rkipc_pipe_1_init() {
	int video_width = rk_param_get_int("video.1:width", 1280);
	int video_height = rk_param_get_int("video.1:height", 720);
	const char *video_device_name = rk_param_get_string("video.1:src_node", "rkispp_scale1");
	int buf_cnt = 3;
	int ret = 0;

	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	// VI init
	// 2.config channel
	LOG_INFO("video_device_name = %s\n", video_device_name);
	memcpy(vi_chn_attr.stIspOpt.aEntityName, video_device_name, strlen(video_device_name));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.u32Depth = 1; // not bind, can get buffer
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_1, &vi_chn_attr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VI_SetChnAttr %x\n", ret);
		return -1;
	}
	LOG_INFO("RK_MPI_VI_SetChnAttr success\n");
	// 3.enable channel
	ret = RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_1);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VI_EnableChn %x\n", ret);
		return -1;
	}
	LOG_INFO("RK_MPI_VI_EnableChn success\n");

	// VO
	VO_PUB_ATTR_S VoPubAttr;
	VO_VIDEO_LAYER_ATTR_S stLayerAttr;
	VO_CSC_S VideoCSC;
	VO_CHN_ATTR_S VoChnAttr;
	RK_U32 u32DispBufLen;

	memset(&VoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
	memset(&VoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
	memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));
	memset(&VideoCSC, 0, sizeof(VO_CSC_S));
	memset(&VoChnAttr, 0, sizeof(VoChnAttr));

	VoPubAttr.enIntfType = VO_INTF_MIPI;
	VoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;
	ret = RK_MPI_VO_SetPubAttr(RV1126_VO_DEV_MIPI, &VoPubAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetPubAttr %x\n", ret);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_SetPubAttr success\n");

	ret = RK_MPI_VO_Enable(RV1126_VO_DEV_MIPI);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_Enable %x\n", ret);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_Enable success\n");

	ret = RK_MPI_VO_GetLayerDispBufLen(VoLayer, &u32DispBufLen);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("Get display buf len failed with error code %d!\n", ret);
		return ret;
	}
	LOG_INFO("Get VoLayer %d disp buf len is %d.\n", VoLayer, u32DispBufLen);
	u32DispBufLen = 15;
	ret = RK_MPI_VO_SetLayerDispBufLen(VoLayer, u32DispBufLen);
	if (ret != RK_SUCCESS) {
		return ret;
	}
	LOG_INFO("Agin Get VoLayer %d disp buf len is %d.\n", VoLayer, u32DispBufLen);

	/* get vo attribute*/
	ret = RK_MPI_VO_GetPubAttr(RV1126_VO_DEV_MIPI, &VoPubAttr);
	if (ret) {
		LOG_ERROR("RK_MPI_VO_GetPubAttr fail!\n");
		return ret;
	}
	LOG_INFO("RK_MPI_VO_GetPubAttr success\n");

	stLayerAttr.stDispRect.s32X = 0;
	stLayerAttr.stDispRect.s32Y = 0;
	stLayerAttr.stDispRect.u32Width = VoPubAttr.stSyncInfo.u16Hact;
	stLayerAttr.stDispRect.u32Height = VoPubAttr.stSyncInfo.u16Vact;
	stLayerAttr.stImageSize.u32Width = VoPubAttr.stSyncInfo.u16Hact;
	stLayerAttr.stImageSize.u32Height = VoPubAttr.stSyncInfo.u16Vact;
	LOG_INFO("stLayerAttr W=%d, H=%d\n", stLayerAttr.stDispRect.u32Width,
	         stLayerAttr.stDispRect.u32Height);

	// stLayerAttr.u32DispFrmRt = 30;
	stLayerAttr.enPixFormat = RK_FMT_YUV420SP;
	// VideoCSC.enCscMatrix = VO_CSC_MATRIX_IDENTITY;
	// VideoCSC.u32Contrast = 50;
	// VideoCSC.u32Hue = 50;
	// VideoCSC.u32Luma = 50;
	// VideoCSC.u32Satuature = 50;
	RK_S32 u32VoChn = 0;

	/*bind layer0 to device hd0*/
	ret = RK_MPI_VO_BindLayer(VoLayer, RV1126_VO_DEV_MIPI, VO_LAYER_MODE_GRAPHIC);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_BindLayer VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_BindLayer success\n");

	ret = RK_MPI_VO_SetLayerSpliceMode(VoLayer, VO_SPLICE_MODE_RGA);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetLayerSpliceMode error %#x\n", ret);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_SetLayerSpliceMode success\n");

	ret = RK_MPI_VO_SetLayerAttr(VoLayer, &stLayerAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetLayerAttr VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_SetLayerAttr success\n");

	ret = RK_MPI_VO_EnableLayer(VoLayer);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_EnableLayer VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_EnableLayer success\n");

	// ret = RK_MPI_VO_SetLayerCSC(VoLayer, &VideoCSC);
	// if (ret != RK_SUCCESS) {
	// 	LOG_ERROR("RK_MPI_VO_SetLayerCSC error\n");
	// 	return ret;
	// }
	// LOG_INFO("RK_MPI_VO_SetLayerCSC success\n");

	ret = RK_MPI_VO_EnableChn(RV1126_VOP_LAYER_CLUSTER0, u32VoChn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("create RV1126_VOP_LAYER_CLUSTER0 layer %d ch vo failed!\n", u32VoChn);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_EnableChn success\n");

	VoChnAttr.bDeflicker = RK_FALSE;
	VoChnAttr.u32Priority = 1;
	VoChnAttr.stRect.s32X = 0;
	VoChnAttr.stRect.s32Y = 0;
	VoChnAttr.stRect.u32Width = stLayerAttr.stDispRect.u32Width;
	VoChnAttr.stRect.u32Height = stLayerAttr.stDispRect.u32Height;
	VoChnAttr.enRotation = ROTATION_90;

	ret = RT_MPI_VO_SetChnAttr(VoLayer, 0, &VoChnAttr);

	// RK_MPI_VO_SetChnFrameRate(VoLayer, 0, 30);

	pipe_1_vi_chn.enModId = RK_ID_VI;
	pipe_1_vi_chn.s32DevId = 0;
	pipe_1_vi_chn.s32ChnId = VIDEO_PIPE_1;
	pipe_1_vo_chn.enModId = RK_ID_VO;
	pipe_1_vo_chn.s32DevId = RV1126_VOP_LAYER_CLUSTER0;
	pipe_1_vo_chn.s32ChnId = 0;

	ret = RK_MPI_SYS_Bind(&pipe_1_vi_chn, &pipe_1_vo_chn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("vi and vo bind error! ret=%#x\n", ret);
		return ret;
	}
	LOG_INFO("vi and vo bind success!\n");

	return 0;
}

int rkipc_pipe_1_deinit() {
	int ret;
	ret = RK_MPI_SYS_UnBind(&pipe_1_vi_chn, &pipe_1_vo_chn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("vi and vo unbind error! ret=%#x\n", ret);
		return ret;
	}
	// disable vo layer
	ret = RK_MPI_VO_DisableLayer(VoLayer);
	if (ret) {
		LOG_ERROR("RK_MPI_VO_DisableLayer failed\n");
		return -1;
	}
	// disable vo dev
	ret = RK_MPI_VO_Disable(RV1126_VO_DEV_MIPI);
	if (ret) {
		LOG_ERROR("RK_MPI_VO_Disable failed\n");
		return -1;
	}

	VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};
	VPSS_GRP VpssGrp = VPSS_ROTATE;
	ret |= RK_MPI_VPSS_StopGrp(VpssGrp);
	ret |= RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[0]);
	ret |= RK_MPI_VPSS_DisableBackupFrame(VpssGrp);
	ret |= RK_MPI_VPSS_DestroyGrp(VpssGrp);

	ret |= RK_MPI_VI_DisableChn(pipe_id_, VIDEO_PIPE_1);

	return ret;
}

static void *wait_key_event(void *arg) {
	int key_fd;
	key_fd = open("/dev/input/event0", O_RDONLY);
	if (key_fd < 0) {
		LOG_ERROR("can't open /dev/input/event0\n");
		return NULL;
	}
	fd_set rfds;
	int nfds = key_fd + 1;
	struct timeval timeout;
	struct input_event key_event;

	while (g_video_run_) {
		// The rfds collection must be emptied every time,
		// otherwise the descriptor changes cannot be detected
		timeout.tv_sec = 1;
		FD_ZERO(&rfds);
		FD_SET(key_fd, &rfds);
		select(nfds, &rfds, NULL, NULL, &timeout);
		// wait for the key event to occur
		if (FD_ISSET(key_fd, &rfds)) {
			read(key_fd, &key_event, sizeof(key_event));
			LOG_INFO("[timeval:sec:%d,usec:%d,type:%d,code:%d,value:%d]\n", key_event.time.tv_sec,
			         key_event.time.tv_usec, key_event.type, key_event.code, key_event.value);
			if ((key_event.code == 115) && key_event.value) {
				LOG_INFO("start capture\n");
				capture_one = 1;
				retry_time = 30;
			}
		}
	}

	if (key_fd) {
		close(key_fd);
		key_fd = 0;
	}
	LOG_DEBUG("wait key event out\n");
	return NULL;
}

static void *get_vi_send_rkbar(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	VI_FRAME_S vi_frame;
	VI_CHN_STATUS_S stChnStatus;
	int ret = 0;
	char file_name[256];

	while (g_video_run_) {
		if (capture_one == 0) {
			usleep(300 * 1000);
			continue;
		}
		capture_one = 0;
		// rkbar
		image_t *img = NULL;
		img = (image_t *)malloc(sizeof(image_t));
		img->width = rk_param_get_int("video.0:width", -1);
		img->height = rk_param_get_int("video.0:height", -1);
		img->crop_x = 0;
		img->crop_y = 0;
		img->crop_w = img->width;
		img->crop_h = img->height;
		img->bin = (unsigned char *)malloc(img->width * img->height);
		img->tmp = NULL;

		void *rkbar_hand = NULL;
		ret = rkbar_init(&rkbar_hand);
		if (ret == -1) {
			LOG_INFO("rkbar init is err");
			rkbar_deinit(rkbar_hand);
			if (img->bin)
				free(img->bin);
			if (img)
				free(img);
			continue;
		}
		LOG_INFO("rkbar init is success");
		while (retry_time) {
			LOG_INFO("333 retry_time is %d\n", retry_time);
			retry_time--;
			ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_0, &vi_frame, 1000);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %#x\n", ret);
				continue;
			}
			void *buffer = RK_MPI_MB_Handle2VirAddr(vi_frame.pMbBlk);
			LOG_INFO("RK_MPI_VI_GetChnFrame ok:data %p seq:%d pts:%" PRId64 " ms\n", buffer,
			         vi_frame.s32Seq, vi_frame.s64PTS / 1000);
			img->data = (uint8_t *)buffer;
			ret = rkbar_scan(rkbar_hand, img);
			if (ret <= 0) {
				LOG_INFO("scan fail\n");
				ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_0, &vi_frame);
				if (ret != RK_SUCCESS) {
					LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
				}
				continue;
			}
			ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_0, &vi_frame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
			}
			LOG_INFO("scan success\n");
			retry_time = 0;
			const char *test = rkbar_getresult(rkbar_hand);
			char *data = (char *)malloc(strlen(test));
			memcpy(data, test, strlen(test));
			LOG_INFO("rkbar the decoding result is \" %s \" \n", data);
			system("aplay /etc/qr_recognized.wav &");

			tuya_ipc_direct_connect(data, TUYA_IPC_DIRECT_CONNECT_QRCODE);
			// connect wifi
			char ssid[20], psk[20], cmd[100], wifi_ssid[20], wifi_psk[20];
			sscanf(data, "%*[^:]:%[^,],%*[^:]:%[^,]", psk, ssid);
			memset(wifi_ssid, 0, 20);
			memset(wifi_psk, 0, 20);
			memcpy(wifi_ssid, &ssid[1], strlen(ssid) - 2);
			memcpy(wifi_psk, &psk[1], strlen(psk) - 2);
			system("cp /etc/wpa_supplicant.conf /tmp");
			sprintf(cmd, "s/SSID/%s/g", ssid);
			LOG_INFO("cmd is %s\n", cmd);
			system(cmd);
			sprintf(cmd, "s/PASSWORD/%s/g", psk);
			LOG_INFO("cmd is %s\n", cmd);
			system(cmd);
			system("wpa_supplicant -B -i wlan0 -c /tmp/wpa_supplicant.conf");
			usleep(1000 * 1000);
			system("udhcpc -i wlan0");
			if (data)
				free(data);
		}
		// retry_time = 0, or connect success
		rkbar_deinit(rkbar_hand);
		if (img->bin)
			free(img->bin);
		if (img)
			free(img);
	}

	return 0;
}

int rk_video_init() {
	LOG_INFO("%s\n", __func__);
	int ret = 0;
	g_video_run_ = 1;
	ret = RK_MPI_SYS_Init();
	ret |= rkipc_vi_dev_init();
	ret |= rkipc_pipe_0_init(); // for vi-venc-rtsp
	ret |= rkipc_pipe_1_init(); // for vi-vo
	ret |= rkipc_rtsp_init(RTSP_URL_0, NULL, NULL);
#if 1
	pthread_t key_id;
	pthread_t get_vi_id;
	pthread_create(&key_id, NULL, wait_key_event, NULL);
	pthread_create(&get_vi_id, NULL, get_vi_send_rkbar, NULL);
#endif

	return ret;
}

int rk_video_deinit() {
	LOG_INFO("%s\n", __func__);
	g_video_run_ = 0;
	pthread_join(venc_thread_id, NULL);
	int ret = 0;
	ret |= rkipc_rtsp_deinit();
	ret |= rkipc_pipe_0_deinit();
	ret |= rkipc_pipe_1_deinit();
	ret |= rkipc_vi_dev_deinit();
	ret |= RK_MPI_SYS_Exit();

	return ret;
}
