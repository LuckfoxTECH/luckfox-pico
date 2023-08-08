// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

//
// vi(fbcpath) → vpss → venc_0(fbc) → rtsp/rtmp/vo
//             		  → venc_1(fbc)
//             		  → venc_2(fbc)
//             		  → venc_3(nv12 jpeg) → jpeg

#include "video.h"
#include "common.h"
#include "isp.h"
#include "osd.h"
#include "region_clip.h"
#include "roi.h"
#include "rtmp.h"
#include "rtsp_demo.h"
#include "storage.h"

#include "rk_mpi_mmz.h"
#include <rk_debug.h>
#include <rk_mpi_mb.h>
#include <rk_mpi_rgn.h>
#include <rk_mpi_sys.h>
#include <rk_mpi_venc.h>
#include <rk_mpi_vi.h>
#include <rk_mpi_vo.h>
#include <rk_mpi_vpss.h>

#include <inttypes.h> // PRId64

#include <rga/im2d.h>
#include <rga/rga.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "video.c"

#define RKISP_MAINPATH 0
#define RKISP_SELFPATH 1
#define RKISP_FBCPATH 2
#define VIDEO_PIPE_0 0
#define VIDEO_PIPE_1 1
#define VIDEO_PIPE_2 2
#define JPEG_VENC_CHN 3
#define VPSS_ROTATE 6

#define RV1126_VO_DEV_HDMI 0
#define RV1126_VO_DEV_MIPI 3
#define RV1126_VOP_LAYER_CLUSTER0 0

#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"
#define RTSP_URL_2 "/live/2"
#define RTMP_URL_0 "rtmp://127.0.0.1:1935/live/mainstream"
#define RTMP_URL_1 "rtmp://127.0.0.1:1935/live/substream"
#define RTMP_URL_2 "rtmp://127.0.0.1:1935/live/thirdstream"

static pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
static int take_photo_one = 0;
static int enable_jpeg, enable_venc_0, enable_venc_1, enable_venc_2;
static int g_enable_vo, g_vo_dev_id, g_vi_chn_id;
static int g_video_run_ = 1;
static int pipe_id_ = 0;
static int dev_id_ = 0;
static rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session_0, g_rtsp_session_1, g_rtsp_session_2;
static const char *tmp_output_data_type = "H.264";
static const char *tmp_rc_mode;
static const char *tmp_h264_profile;
static const char *tmp_smart;
static const char *tmp_gop_mode;
static const char *tmp_rc_quality;
static pthread_t venc_thread_0, venc_thread_1, venc_thread_2, jpeg_venc_thread_id;

static MPP_CHN_S vi_chn, vpss_in_chn, vpss_rotate_chn, vo_chn, vpss_out_chn[4], venc_chn[4];
static VO_DEV VoLayer = RV1126_VOP_LAYER_CLUSTER0;

// static void *test_get_vi(void *arg) {
// 	printf("#Start %s thread, arg:%p\n", __func__, arg);
// 	VI_FRAME_S stViFrame;
// 	VI_CHN_STATUS_S stChnStatus;
// 	int loopCount = 0;
// 	int ret = 0;

// 	while (g_video_run_) {
// 		// 5.get the frame
// 		ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_2, &stViFrame, 1000);
// 		if (ret == RK_SUCCESS) {
// 			void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.pMbBlk);
// 			LOG_ERROR("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%" PRId64 " ms\n", data,
// 			          loopCount, stViFrame.s32Seq, stViFrame.s64PTS / 1000);
// 			// 6.get the channel status
// 			ret = RK_MPI_VI_QueryChnStatus(pipe_id_, VIDEO_PIPE_2, &stChnStatus);
// 			LOG_ERROR("RK_MPI_VI_QueryChnStatus ret %x, "
// 			          "w:%d,h:%d,enable:%d,lost:%d,framerate:%d,vbfail:%d\n",
// 			          ret, stChnStatus.stSize.u32Width, stChnStatus.stSize.u32Height,
// 			          stChnStatus.bEnable, stChnStatus.u32LostFrame, stChnStatus.u32FrameRate,
// 			          stChnStatus.u32VbFail);
// 			// 7.release the frame
// 			ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_2, &stViFrame);
// 			if (ret != RK_SUCCESS) {
// 				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
// 			}
// 			loopCount++;
// 		} else {
// 			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", ret);
// 		}
// 		usleep(10 * 1000);
// 	}

// 	return 0;
// }

static void *rkipc_get_venc_0(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	// FILE *fp = fopen("/data/venc.h265", "wb");
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_0, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			// fwrite(data, 1, stFrame.pstPack->u32Len, fp);
			// fflush(fp);
			// LOG_INFO("Count:%d, Len:%d, PTS is %" PRId64", enH264EType is %d\n", loopCount,
			// stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			// stFrame.pstPack->DataType.enH264EType);

			if (g_rtsplive && g_rtsp_session_0) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session_0, data, stFrame.pstPack->u32Len,
				              stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			}
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
	// fclose(fp);

	return 0;
}

static void *rkipc_get_venc_1(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_1, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			// LOG_INFO("Count:%d, Len:%d, PTS is %" PRId64", enH264EType is %d\n", loopCount,
			// stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			// stFrame.pstPack->DataType.enH264EType);
			if (g_rtsplive && g_rtsp_session_1) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session_1, data, stFrame.pstPack->u32Len,
				              stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			}
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
			// 7.release the frame
			ret = RK_MPI_VENC_ReleaseStream(VIDEO_PIPE_1, &stFrame);
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

static void *rkipc_get_venc_2(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_2, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			// LOG_INFO("Count:%d, Len:%d, PTS is %" PRId64", enH264EType is %d\n", loopCount,
			// stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			// stFrame.pstPack->DataType.enH264EType);
			if (g_rtsplive && g_rtsp_session_2) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session_2, data, stFrame.pstPack->u32Len,
				              stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			}
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
			// 7.release the frame
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

static void *rkipc_get_jpeg(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	char file_name[128] = {0};
	const char *file_path = rk_param_get_string("storage:file_path", "/userdata");
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		usleep(300 * 1000);
		if (!take_photo_one)
			continue;
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(JPEG_VENC_CHN, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			LOG_INFO("Count:%d, Len:%d, PTS is %" PRId64 ", enH264EType is %d\n", loopCount,
			         stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			         stFrame.pstPack->DataType.enH264EType);
			// save jpeg file
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			snprintf(file_name, 128, "%s/%d%02d%02d%02d%02d%02d.jpeg", file_path, tm.tm_year + 1900,
			         tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			LOG_INFO("file_name is %s\n", file_name);
			FILE *fp = fopen(file_name, "wb");
			fwrite(data, 1, stFrame.pstPack->u32Len, fp);
			fflush(fp);
			fclose(fp);
			// 7.release the frame
			ret = RK_MPI_VENC_ReleaseStream(JPEG_VENC_CHN, &stFrame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
			}
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VENC_GetStream timeout %x\n", ret);
		}
		// usleep(33 * 1000);
	}
	if (stFrame.pstPack)
		free(stFrame.pstPack);

	return 0;
}

int rkipc_rtsp_init() {
	LOG_INFO("start\n");
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session_0 = rtsp_new_session(g_rtsplive, RTSP_URL_0);
	g_rtsp_session_1 = rtsp_new_session(g_rtsplive, RTSP_URL_1);
	g_rtsp_session_2 = rtsp_new_session(g_rtsplive, RTSP_URL_2);
	tmp_output_data_type = rk_param_get_string("video.0:output_data_type", "H.264");
	if (!strcmp(tmp_output_data_type, "H.264"))
		rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	else if (!strcmp(tmp_output_data_type, "H.265"))
		rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	else
		LOG_ERROR("0 tmp_output_data_type is %s, not support\n", tmp_output_data_type);

	tmp_output_data_type = rk_param_get_string("video.1:output_data_type", "H.264");
	if (!strcmp(tmp_output_data_type, "H.264"))
		rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	else if (!strcmp(tmp_output_data_type, "H.265"))
		rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	else
		LOG_ERROR("1 tmp_output_data_type is %s, not support\n", tmp_output_data_type);

	tmp_output_data_type = rk_param_get_string("video.2:output_data_type", "H.264");
	if (!strcmp(tmp_output_data_type, "H.264"))
		rtsp_set_video(g_rtsp_session_2, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	else if (!strcmp(tmp_output_data_type, "H.265"))
		rtsp_set_video(g_rtsp_session_2, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	else
		LOG_ERROR("2 tmp_output_data_type is %s, not support\n", tmp_output_data_type);

	rtsp_sync_video_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session_1, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session_2, rtsp_get_reltime(), rtsp_get_ntptime());
	LOG_INFO("end\n");

	return 0;
}

int rkipc_rtsp_deinit() {
	LOG_INFO("%s\n", __func__);
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
	g_rtsplive = NULL;
	return 0;
}

int rkipc_rtmp_init() {
	int ret = 0;
	ret |= rk_rtmp_init(0, RTMP_URL_0);
	ret |= rk_rtmp_init(1, RTMP_URL_1);
	ret |= rk_rtmp_init(2, RTMP_URL_2);

	return ret;
}

int rkipc_rtmp_deinit() {
	int ret = 0;
	ret |= rk_rtmp_deinit(0);
	ret |= rk_rtmp_deinit(1);
	ret |= rk_rtmp_deinit(2);

	return ret;
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

int rkipc_vi_chn_init() {
	int ret;
	int video_width = rk_param_get_int("video.source:width", -1);
	int video_height = rk_param_get_int("video.source:height", -1);
	int buf_cnt = 6;
	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	if (g_vi_chn_id == 2)                                // FBCPATH
		vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;
	vi_chn_attr.u32Depth = 2;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, g_vi_chn_id, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, g_vi_chn_id);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}
	// pthread_t thread_id;
	// pthread_create(&thread_id, NULL, test_get_vi, NULL);

	return ret;
}

int rkipc_vi_chn_deinit() {
	int ret = 0;
	ret = RK_MPI_VI_DisableChn(pipe_id_, g_vi_chn_id);
	if (ret)
		LOG_ERROR("ERROR: Destroy VI error! ret=%x\n", ret);

	return ret;
}

int rkipc_vpss_init() {
	int ret;
	// VPSS for 1 → 4
	VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};
	VPSS_GRP VpssGrp = 0;
	VPSS_GRP_ATTR_S stVpssGrpAttr;
	VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
	memset(&stVpssGrpAttr, 0, sizeof(stVpssGrpAttr));
	memset(&stVpssChnAttr[0], 0, sizeof(stVpssChnAttr[0]));
	memset(&stVpssChnAttr[1], 0, sizeof(stVpssChnAttr[1]));
	memset(&stVpssChnAttr[2], 0, sizeof(stVpssChnAttr[2]));
	memset(&stVpssChnAttr[3], 0, sizeof(stVpssChnAttr[3]));
	stVpssGrpAttr.u32MaxW = 4096;
	stVpssGrpAttr.u32MaxH = 4096;
	stVpssGrpAttr.enPixelFormat = RK_FMT_YUV420SP;
	stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
	stVpssGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
	ret = RK_MPI_VPSS_CreateGrp(VpssGrp, &stVpssGrpAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_CreateGrp error! ret is %#x\n", ret);
		return ret;
	}

	if (enable_venc_0) {
		stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
		stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
		stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
		stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
		stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
		stVpssChnAttr[0].u32Width = rk_param_get_int("video.0:width", -1);
		stVpssChnAttr[0].u32Height = rk_param_get_int("video.0:height", -1);
		stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
		ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[0], &stVpssChnAttr[0]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("0: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
		ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[0]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("0: RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);
	}
	if (enable_venc_1) {
		stVpssChnAttr[1].enChnMode = VPSS_CHN_MODE_USER;
		stVpssChnAttr[1].enDynamicRange = DYNAMIC_RANGE_SDR8;
		stVpssChnAttr[1].enPixelFormat = RK_FMT_YUV420SP;
		stVpssChnAttr[1].stFrameRate.s32SrcFrameRate = -1;
		stVpssChnAttr[1].stFrameRate.s32DstFrameRate = -1;
		stVpssChnAttr[1].u32Width = rk_param_get_int("video.1:width", 0);
		stVpssChnAttr[1].u32Height = rk_param_get_int("video.1:height", 0);
		stVpssChnAttr[1].enCompressMode = COMPRESS_MODE_NONE;
		ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[1], &stVpssChnAttr[1]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("1: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
		ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[1]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("1: RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);
	}
	if (enable_venc_2) {
		stVpssChnAttr[2].enChnMode = VPSS_CHN_MODE_USER;
		stVpssChnAttr[2].enDynamicRange = DYNAMIC_RANGE_SDR8;
		stVpssChnAttr[2].enPixelFormat = RK_FMT_YUV420SP;
		stVpssChnAttr[2].stFrameRate.s32SrcFrameRate = -1;
		stVpssChnAttr[2].stFrameRate.s32DstFrameRate = -1;
		stVpssChnAttr[2].u32Width = rk_param_get_int("video.2:width", 0);
		stVpssChnAttr[2].u32Height = rk_param_get_int("video.2:height", 0);
		stVpssChnAttr[2].enCompressMode = COMPRESS_MODE_NONE;
		ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[2], &stVpssChnAttr[2]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("2: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
		ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[2]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("2: RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);
	}
	if (enable_jpeg) {
		stVpssChnAttr[3].enChnMode = VPSS_CHN_MODE_USER;
		stVpssChnAttr[3].enDynamicRange = DYNAMIC_RANGE_SDR8;
		stVpssChnAttr[3].enPixelFormat = RK_FMT_YUV420SP;
		stVpssChnAttr[3].stFrameRate.s32SrcFrameRate = -1;
		stVpssChnAttr[3].stFrameRate.s32DstFrameRate = -1;
		stVpssChnAttr[3].u32Width = rk_param_get_int("video.0:width", 0);
		stVpssChnAttr[3].u32Height = rk_param_get_int("video.0:height", 0);
		stVpssChnAttr[3].enCompressMode = COMPRESS_MODE_NONE;
		ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[3], &stVpssChnAttr[3]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("3: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
		ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[3]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("3: RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);
	}
	ret = RK_MPI_VPSS_EnableBackupFrame(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_EnableBackupFrame error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_SetVProcDev(VpssGrp, VIDEO_PROC_DEV_RGA);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_SetVProcDev error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_StartGrp(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_StartGrp error! ret is %#x\n", ret);
		return ret;
	}

	return ret;
}

int rkipc_vpss_deinit() {
	int ret = 0;
	VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};
	VPSS_GRP VpssGrp = 0;
	ret |= RK_MPI_VPSS_StopGrp(VpssGrp);
	if (enable_venc_0)
		ret |= RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[0]);
	if (enable_venc_1)
		ret |= RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[1]);
	if (enable_venc_2)
		ret |= RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[2]);
	if (enable_jpeg)
		ret |= RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[3]);
	ret |= RK_MPI_VPSS_DisableBackupFrame(VpssGrp);
	ret |= RK_MPI_VPSS_DestroyGrp(VpssGrp);

	return ret;
}

int rkipc_venc_0_init() {
	int ret = 0;
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);

	// VENC[0] init
	VENC_CHN_ATTR_S venc_chn_attr;
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
	tmp_smart = rk_param_get_string("video.0:smart", NULL);
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
	venc_chn_attr.stVencAttr.u32StreamBufCnt = 5;
	venc_chn_attr.stVencAttr.u32BufSize = video_width * video_height * 3 / 2;
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_0, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%d\n", ret);
		return -1;
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

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_0, &stRecvParam);
	pthread_create(&venc_thread_0, NULL, rkipc_get_venc_0, NULL);

	return ret;
}

int rkipc_venc_0_deinit() {
	int ret = 0;
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_0);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");

	return ret;
}

int rkipc_venc_1_init() {
	int ret = 0;
	int video_width = rk_param_get_int("video.1:width", -1);
	int video_height = rk_param_get_int("video.1:height", -1);

	// VENC[1] init
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
	tmp_smart = rk_param_get_string("video.1:smart", NULL);
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
	venc_chn_attr.stVencAttr.u32StreamBufCnt = 5;
	venc_chn_attr.stVencAttr.u32BufSize = video_width * video_height * 3 / 2;
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_1, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%d\n", ret);
		return -1;
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

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_1, &stRecvParam);
	pthread_create(&venc_thread_1, NULL, rkipc_get_venc_1, NULL);

	return ret;
}

int rkipc_venc_1_deinit() {
	int ret = 0;
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_1);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_1);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");

	return ret;
}

int rkipc_venc_2_init() {
	int ret = 0;
	int video_width = rk_param_get_int("video.2:width", -1);
	int video_height = rk_param_get_int("video.2:height", -1);

	// VENC[2] init
	VENC_CHN_ATTR_S venc_chn_attr;
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
	tmp_smart = rk_param_get_string("video.2:smart", NULL);
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
	venc_chn_attr.stVencAttr.u32StreamBufCnt = 5;
	venc_chn_attr.stVencAttr.u32BufSize = video_width * video_height * 3 / 2;
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_2, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%d\n", ret);
		return -1;
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

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_2, &stRecvParam);
	pthread_create(&venc_thread_2, NULL, rkipc_get_venc_2, NULL);

	return ret;
}

int rkipc_venc_2_deinit() {
	int ret = 0;
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_2);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_2);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");

	return ret;
}

int rkipc_venc_3_init() {
	// jpeg resolution same to video.0
	int ret;
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	// VENC[3] init
	VENC_CHN_ATTR_S jpeg_chn_attr;
	memset(&jpeg_chn_attr, 0, sizeof(jpeg_chn_attr));
	jpeg_chn_attr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
	jpeg_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	jpeg_chn_attr.stVencAttr.u32PicWidth = video_width;
	jpeg_chn_attr.stVencAttr.u32PicHeight = video_height;
	jpeg_chn_attr.stVencAttr.u32VirWidth = video_width;
	jpeg_chn_attr.stVencAttr.u32VirHeight = video_height;
	jpeg_chn_attr.stVencAttr.u32StreamBufCnt = 2;
	jpeg_chn_attr.stVencAttr.u32BufSize = video_width * video_height * 3 / 2;
	// jpeg_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(JPEG_VENC_CHN, &jpeg_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%d\n", ret);
		return -1;
	}
	VENC_JPEG_PARAM_S stJpegParam;
	memset(&stJpegParam, 0, sizeof(stJpegParam));
	stJpegParam.u32Qfactor = 95;
	RK_MPI_VENC_SetJpegParam(JPEG_VENC_CHN, &stJpegParam);
	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = 1;
	RK_MPI_VENC_StartRecvFrame(JPEG_VENC_CHN,
	                           &stRecvParam); // must, for no streams callback running failed
	RK_MPI_VENC_StopRecvFrame(JPEG_VENC_CHN);
	pthread_create(&jpeg_venc_thread_id, NULL, rkipc_get_jpeg, NULL);

	return ret;
}

int rkipc_venc_3_deinit() {
	int ret = 0;
	ret = RK_MPI_VENC_StopRecvFrame(JPEG_VENC_CHN);
	ret |= RK_MPI_VENC_DestroyChn(JPEG_VENC_CHN);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");

	return ret;
}

int rkipc_bind_init() {
	int ret;
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = g_vi_chn_id;
	vpss_in_chn.enModId = RK_ID_VPSS;
	vpss_in_chn.s32DevId = 0;
	vpss_in_chn.s32ChnId = 0;
	for (int i = 0; i < 4; i++) {
		vpss_out_chn[i].enModId = RK_ID_VPSS;
		vpss_out_chn[i].s32DevId = 0;
		vpss_out_chn[i].s32ChnId = i;
		venc_chn[i].enModId = RK_ID_VENC;
		venc_chn[i].s32DevId = 0;
		venc_chn[i].s32ChnId = i;
	}

	ret = RK_MPI_SYS_Bind(&vi_chn, &vpss_in_chn);
	if (ret)
		LOG_ERROR("Bind VI and vpss_in_chn error! ret=%#x\n", ret);
	else
		LOG_INFO("Bind VI and vpss_in_chn success\n");

	if (enable_venc_0) {
		ret = RK_MPI_SYS_Bind(&vpss_out_chn[0], &venc_chn[0]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_out_chn[0].s32DevId, vpss_out_chn[0].s32ChnId, venc_chn[0].s32DevId,
			          venc_chn[0].s32ChnId);
		}
	}
	if (enable_venc_1) {
		ret = RK_MPI_SYS_Bind(&vpss_out_chn[1], &venc_chn[1]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_out_chn[1].s32DevId, vpss_out_chn[1].s32ChnId, venc_chn[1].s32DevId,
			          venc_chn[1].s32ChnId);
		}
	}
	if (enable_venc_2) {
		ret = RK_MPI_SYS_Bind(&vpss_out_chn[2], &venc_chn[2]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_out_chn[2].s32DevId, vpss_out_chn[2].s32ChnId, venc_chn[2].s32DevId,
			          venc_chn[2].s32ChnId);
		}
	}
	if (enable_jpeg) {
		ret = RK_MPI_SYS_Bind(&vpss_out_chn[3], &venc_chn[3]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_out_chn[3].s32DevId, vpss_out_chn[3].s32ChnId, venc_chn[3].s32DevId,
			          venc_chn[3].s32ChnId);
		}
	}

	return ret;
}

int rkipc_bind_deinit() {
	int ret;

	// unbind vpss and venc
	if (enable_venc_0) {
		ret = RK_MPI_SYS_UnBind(&vpss_out_chn[0], &venc_chn[0]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_out_chn[0].s32DevId, vpss_out_chn[0].s32ChnId, venc_chn[0].s32DevId,
			          venc_chn[0].s32ChnId);
			return ret;
		}
	}
	if (enable_venc_1) {
		ret = RK_MPI_SYS_UnBind(&vpss_out_chn[1], &venc_chn[1]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_out_chn[1].s32DevId, vpss_out_chn[1].s32ChnId, venc_chn[1].s32DevId,
			          venc_chn[1].s32ChnId);
			return ret;
		}
	}
	if (enable_venc_2) {
		ret = RK_MPI_SYS_UnBind(&vpss_out_chn[2], &venc_chn[2]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_out_chn[2].s32DevId, vpss_out_chn[2].s32ChnId, venc_chn[2].s32DevId,
			          venc_chn[2].s32ChnId);
			return ret;
		}
	}
	if (enable_jpeg) {
		ret = RK_MPI_SYS_UnBind(&vpss_out_chn[3], &venc_chn[3]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_out_chn[3].s32DevId, vpss_out_chn[3].s32ChnId, venc_chn[3].s32DevId,
			          venc_chn[3].s32ChnId);
			return ret;
		}
	}
	// unbind vi and vpss
	ret = RK_MPI_SYS_UnBind(&vi_chn, &vpss_in_chn);
	if (ret)
		LOG_ERROR("UnBind VI and VPSS[0] error! ret=%#x\n", ret);
	else
		LOG_INFO("UnBind VI and VPSS[0] success\n");

	return ret;
}

int rkipc_pipe_vpss_vo_init() {
	int ret = 0;

	// VPSS for rotate
	VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};
	VPSS_GRP VpssGrp = VPSS_ROTATE;
	VPSS_GRP_ATTR_S stVpssGrpAttr;
	VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
	memset(&stVpssGrpAttr, 0, sizeof(stVpssGrpAttr));
	memset(&stVpssChnAttr[0], 0, sizeof(stVpssChnAttr));
	stVpssGrpAttr.u32MaxW = 4096;
	stVpssGrpAttr.u32MaxH = 4096;
	stVpssGrpAttr.enPixelFormat = RK_FMT_YUV420SP;
	stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
	stVpssGrpAttr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;

	stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
	stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
	stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	stVpssChnAttr[0].u32Width = 720;
	stVpssChnAttr[0].u32Height = 1280;
	stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ret |= RK_MPI_VPSS_CreateGrp(VpssGrp, &stVpssGrpAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_CreateGrp error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[0], &stVpssChnAttr[0]);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
		return ret;
	}
	if (g_vo_dev_id == 3)
		ret = RK_MPI_VPSS_SetChnRotation(VpssGrp, VpssChn[0], ROTATION_90);
	// if (g_vo_dev_id == 0)
	// 	ret = RK_MPI_VPSS_SetChnRotation(VpssGrp, VpssChn[0], ROTATION_180);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_SetChnRotation error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[0]);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_EnableBackupFrame(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_EnableBackupFrame error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_SetVProcDev(VpssGrp, VIDEO_PROC_DEV_RGA);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_SetVProcDev error! ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_VPSS_StartGrp(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_StartGrp error! ret is %#x\n", ret);
		return ret;
	}

	VO_PUB_ATTR_S VoPubAttr;
	VO_VIDEO_LAYER_ATTR_S stLayerAttr;
	VO_CSC_S VideoCSC;
	VO_CHN_ATTR_S VoChnAttr;
	RK_U32 u32DispBufLen;
	memset(&VoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
	memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));
	memset(&VideoCSC, 0, sizeof(VO_CSC_S));
	memset(&VoChnAttr, 0, sizeof(VoChnAttr));
	LOG_INFO("dev id:%d\n", g_vo_dev_id);
	if (g_vo_dev_id == 3) {
		VoPubAttr.enIntfType = VO_INTF_HDMI;
		VoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
	} else {
		VoPubAttr.enIntfType = VO_INTF_MIPI;
		VoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;
	}
	ret = RK_MPI_VO_SetPubAttr(g_vo_dev_id, &VoPubAttr); // g_vo_dev_id
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetPubAttr err is %x\n", ret);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_SetPubAttr success\n");

	ret = RK_MPI_VO_Enable(g_vo_dev_id);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_Enable err is %x\n", ret);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_Enable success\n");

	ret = RK_MPI_VO_GetLayerDispBufLen(VoLayer, &u32DispBufLen);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("Get display buf len failed with error code %d!\n", ret);
		return ret;
	}
	LOG_INFO("Get VoLayer %d disp buf len is %d.\n", VoLayer, u32DispBufLen);
	u32DispBufLen = 3;
	ret = RK_MPI_VO_SetLayerDispBufLen(VoLayer, u32DispBufLen);
	if (ret != RK_SUCCESS) {
		return ret;
	}
	LOG_INFO("Agin Get VoLayer %d disp buf len is %d.\n", VoLayer, u32DispBufLen);

	/* get vo attribute*/
	ret = RK_MPI_VO_GetPubAttr(g_vo_dev_id, &VoPubAttr);
	if (ret) {
		LOG_ERROR("RK_MPI_VO_GetPubAttr fail!\n");
		return ret;
	}
	LOG_INFO("RK_MPI_VO_GetPubAttr success\n");
	if ((VoPubAttr.stSyncInfo.u16Hact == 0) || (VoPubAttr.stSyncInfo.u16Vact == 0)) {
		if (g_vo_dev_id == 3) { // RK3588_VO_DEV_HDMI
			VoPubAttr.stSyncInfo.u16Hact = 1920;
			VoPubAttr.stSyncInfo.u16Vact = 1080;
		} else {
			VoPubAttr.stSyncInfo.u16Hact = 720;  // 1080;
			VoPubAttr.stSyncInfo.u16Vact = 1280; // 1920;
		}
	}

	stLayerAttr.stDispRect.s32X = 0;
	stLayerAttr.stDispRect.s32Y = 0;
	stLayerAttr.stDispRect.u32Width = VoPubAttr.stSyncInfo.u16Hact;
	stLayerAttr.stDispRect.u32Height = VoPubAttr.stSyncInfo.u16Vact;
	stLayerAttr.stImageSize.u32Width = VoPubAttr.stSyncInfo.u16Hact;
	stLayerAttr.stImageSize.u32Height = VoPubAttr.stSyncInfo.u16Vact;
	LOG_INFO("stLayerAttr W=%d, H=%d\n", stLayerAttr.stDispRect.u32Width,
	         stLayerAttr.stDispRect.u32Height);

	stLayerAttr.u32DispFrmRt = 25;
	stLayerAttr.enPixFormat = RK_FMT_RGB888;
	VideoCSC.enCscMatrix = VO_CSC_MATRIX_IDENTITY;
	VideoCSC.u32Contrast = 50;
	VideoCSC.u32Hue = 50;
	VideoCSC.u32Luma = 50;
	VideoCSC.u32Satuature = 50;
	RK_S32 u32VoChn = 0;

	/*bind layer0 to device hd0*/
	ret = RK_MPI_VO_BindLayer(VoLayer, g_vo_dev_id, VO_LAYER_MODE_GRAPHIC);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_BindLayer VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_BindLayer success\n");

	ret = RK_MPI_VO_SetLayerAttr(VoLayer, &stLayerAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetLayerAttr VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_SetLayerAttr success\n");

	ret = RK_MPI_VO_SetLayerSpliceMode(VoLayer, VO_SPLICE_MODE_RGA); /////////
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetLayerSpliceMode VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_SetLayerSpliceMode success\n");

	ret = RK_MPI_VO_EnableLayer(VoLayer); //
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_EnableLayer VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_EnableLayer success\n");

	ret = RK_MPI_VO_SetLayerCSC(VoLayer, &VideoCSC);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetLayerCSC error\n");
		return ret;
	}
	LOG_INFO("RK_MPI_VO_SetLayerCSC success\n");

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
	ret = RK_MPI_VO_SetChnAttr(VoLayer, 0, &VoChnAttr);

	vpss_rotate_chn.enModId = RK_ID_VPSS;
	vpss_rotate_chn.s32DevId = VPSS_ROTATE;
	vpss_rotate_chn.s32ChnId = 0;

	vo_chn.enModId = RK_ID_VO;
	vo_chn.s32DevId = RV1126_VOP_LAYER_CLUSTER0;
	vo_chn.s32ChnId = 0;

	ret = RK_MPI_SYS_Bind(&vi_chn, &vpss_rotate_chn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("vi and vpss bind error! ret=%#x\n", ret);
		return ret;
	}
	ret = RK_MPI_SYS_Bind(&vpss_rotate_chn, &vo_chn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("vpss and vo bind error! ret=%#x\n", ret);
		return ret;
	}

	return 0;
}

int rkipc_pipe_vi_vo_deinit() {
	int ret;
	ret = RK_MPI_SYS_UnBind(&vpss_rotate_chn, &vo_chn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("vpss and vo unbind error! ret=%#x\n", ret);
		return ret;
	}
	ret = RK_MPI_SYS_UnBind(&vi_chn, &vpss_rotate_chn);
	if (ret) {
		LOG_ERROR("vi and vpss unbind error! ret=%#x\n", ret);
		return -1;
	}

	// disable vo layer
	ret = RK_MPI_VO_DisableLayer(VoLayer);
	if (ret) {
		LOG_ERROR("RK_MPI_VO_DisableLayer failed\n");
		return -1;
	}
	// disable vo dev
	ret = RK_MPI_VO_Disable(g_vo_dev_id);
	if (ret) {
		LOG_ERROR("RK_MPI_VO_Disable failed\n");
		return -1;
	}

	return 0;
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
	LOG_INFO("num is %d, den is %d\n", num, den);

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
	RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);

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
int rkipc_osd_cover_create(int id, osd_data_s *osd_data) {
	LOG_INFO("id is %d\n", id);
	int ret = 0;
	RGN_HANDLE coverHandle = id;
	RGN_ATTR_S stCoverAttr;
	MPP_CHN_S stCoverChn;
	RGN_CHN_ATTR_S stCoverChnAttr;

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
	LOG_INFO("The handle: %d, create success\n", coverHandle);

	// display cover regions to venc groups
	stCoverChn.enModId = RK_ID_VENC;
	stCoverChn.s32DevId = 0;
	memset(&stCoverChnAttr, 0, sizeof(stCoverChnAttr));
	stCoverChnAttr.bShow = osd_data->enable;
	stCoverChnAttr.enType = COVER_RGN;
	stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X = osd_data->origin_x;
	stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y = osd_data->origin_y;
	stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width = osd_data->width;
	stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height = osd_data->height;
	stCoverChnAttr.unChnAttr.stCoverChn.u32Color = 0xffffff;
	stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = id;
	LOG_INFO("cover region to chn success\n");
	if (enable_venc_0) {
		stCoverChn.s32ChnId = 0;
		ret = RK_MPI_RGN_AttachToChn(coverHandle, &stCoverChn, &stCoverChnAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) failed with %#x\n", coverHandle, ret);
			return RK_FAILURE;
		}
		LOG_INFO("RK_MPI_RGN_AttachToChn to venc0 success\n");
	}
	if (enable_venc_1) {
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X =
		    UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.1:width", 1) /
		                rk_param_get_int("video.0:width", 1));
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y =
		    UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.1:height", 1) /
		                rk_param_get_int("video.0:height", 1));
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width =
		    UPALIGNTO16(osd_data->width * rk_param_get_int("video.1:width", 1) /
		                rk_param_get_int("video.0:width", 1));
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height =
		    UPALIGNTO16(osd_data->height * rk_param_get_int("video.1:height", 1) /
		                rk_param_get_int("video.0:height", 1));
		stCoverChn.s32ChnId = 1;
		ret = RK_MPI_RGN_AttachToChn(coverHandle, &stCoverChn, &stCoverChnAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) failed with %#x\n", coverHandle, ret);
			return RK_FAILURE;
		}
		LOG_INFO("RK_MPI_RGN_AttachToChn to venc0 success\n");
	}
	if (enable_venc_2) {
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32X =
		    UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.2:width", 1) /
		                rk_param_get_int("video.0:width", 1));
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.s32Y =
		    UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.2:height", 1) /
		                rk_param_get_int("video.0:height", 1));
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Width =
		    UPALIGNTO16(osd_data->width * rk_param_get_int("video.2:width", 1) /
		                rk_param_get_int("video.0:width", 1));
		stCoverChnAttr.unChnAttr.stCoverChn.stRect.u32Height =
		    UPALIGNTO16(osd_data->height * rk_param_get_int("video.2:height", 1) /
		                rk_param_get_int("video.0:height", 1));
		stCoverChn.s32ChnId = 2;
		ret = RK_MPI_RGN_AttachToChn(coverHandle, &stCoverChn, &stCoverChnAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) failed with %#x\n", coverHandle, ret);
			return RK_FAILURE;
		}
		LOG_INFO("RK_MPI_RGN_AttachToChn to venc0 success\n");
	}

	return ret;
}

int rkipc_osd_cover_destroy(int id) {
	LOG_INFO("%s\n", __func__);
	int ret = 0;
	// Detach osd from chn
	MPP_CHN_S stMppChn;
	RGN_HANDLE RgnHandle = id;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	if (enable_venc_0) {
		stMppChn.s32ChnId = 0;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
	}
	if (enable_venc_1) {
		stMppChn.s32ChnId = 1;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
	}
	if (enable_venc_2) {
		stMppChn.s32ChnId = 2;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc2 failed with %#x\n", RgnHandle, ret);
	}

	// destory region
	ret = RK_MPI_RGN_Destroy(RgnHandle);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Destroy [%d] failed with %#x\n", RgnHandle, ret);
	}
	LOG_INFO("Destory handle:%d success\n", RgnHandle);

	return ret;
}

int rkipc_osd_bmp_create(int id, osd_data_s *osd_data) {
	LOG_INFO("id is %d\n", id);
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
	LOG_INFO("The handle: %d, create success\n", RgnHandle);

	// display overlay regions to venc groups
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = 0;
	memset(&stRgnChnAttr, 0, sizeof(stRgnChnAttr));
	stRgnChnAttr.bShow = osd_data->enable;
	stRgnChnAttr.enType = OVERLAY_RGN;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = osd_data->origin_x;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = osd_data->origin_y;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = id;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	if (enable_venc_0) {
		stMppChn.s32ChnId = 0;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
			return RK_FAILURE;
		}
		LOG_INFO("RK_MPI_RGN_AttachToChn to venc0 success\n");
	}
	if (enable_venc_1) {
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X =
		    UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.1:width", 1) /
		                rk_param_get_int("video.0:width", 1));
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y =
		    UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.1:height", 1) /
		                rk_param_get_int("video.0:height", 1));
		stMppChn.s32ChnId = 1;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
			return RK_FAILURE;
		}
		LOG_INFO("RK_MPI_RGN_AttachToChn to venc1 success\n");
	}
	if (enable_venc_2) {
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X =
		    UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.2:width", 1) /
		                rk_param_get_int("video.0:width", 1));
		stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y =
		    UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.2:height", 1) /
		                rk_param_get_int("video.0:height", 1));
		stMppChn.s32ChnId = 2;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc2 failed with %#x\n", RgnHandle, ret);
			return RK_FAILURE;
		}
		LOG_INFO("RK_MPI_RGN_AttachToChn to venc2 success\n");
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

int rkipc_osd_bmp_destroy(int id) {
	LOG_INFO("%s\n", __func__);
	int ret = 0;
	// Detach osd from chn
	MPP_CHN_S stMppChn;
	RGN_HANDLE RgnHandle = id;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	if (enable_venc_0) {
		stMppChn.s32ChnId = 0;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
	}
	if (enable_venc_1) {
		stMppChn.s32ChnId = 1;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
	}
	if (enable_venc_2) {
		stMppChn.s32ChnId = 2;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc2 failed with %#x\n", RgnHandle, ret);
	}

	// destory region
	ret = RK_MPI_RGN_Destroy(RgnHandle);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Destroy [%d] failed with %#x\n", RgnHandle, ret);
	}
	LOG_INFO("Destory handle:%d success\n", RgnHandle);

	return ret;
}

int rkipc_osd_bmp_change(int id, osd_data_s *osd_data) {
	// LOG_INFO("id is %d\n", id);
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

int rkipc_osd_init() {
	rk_osd_cover_create_callback_register(rkipc_osd_cover_create);
	rk_osd_cover_destroy_callback_register(rkipc_osd_cover_destroy);
	rk_osd_bmp_create_callback_register(rkipc_osd_bmp_create);
	rk_osd_bmp_destroy_callback_register(rkipc_osd_bmp_destroy);
	rk_osd_bmp_change_callback_register(rkipc_osd_bmp_change);
	rk_osd_init();

	return 0;
}

int rkipc_osd_deinit() {
	rk_osd_deinit();
	rk_osd_cover_create_callback_register(NULL);
	rk_osd_cover_destroy_callback_register(NULL);
	rk_osd_bmp_create_callback_register(NULL);
	rk_osd_bmp_destroy_callback_register(NULL);
	rk_osd_bmp_change_callback_register(NULL);

	return 0;
}

// jpeg

int rk_video_get_enable_cycle_snapshot(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:enable_cycle_snapshot");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_enable_cycle_snapshot(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:enable_cycle_snapshot");
	rk_param_set_int(entry, value);

	return 0;
}

int rk_video_get_image_quality(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:jpeg_qfactor");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_image_quality(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.jpeg:jpeg_qfactor");
	rk_param_set_int(entry, value);

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
	sscanf(value, "%d*%d", &width, &height);
	snprintf(entry, 127, "video.jpeg:width");
	rk_param_set_int(entry, width);
	snprintf(entry, 127, "video.jpeg:height");
	rk_param_set_int(entry, height);

	return 0;
}

int rk_take_photo() {
	LOG_INFO("start\n");
	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = 1;
	RK_MPI_VENC_StartRecvFrame(JPEG_VENC_CHN, &stRecvParam);
	take_photo_one = 1;

	return 0;
}

int rk_roi_set(roi_data_s *roi_data) {
	// LOG_INFO("id is %d\n", id);
	int ret = 0;
	int venc_chn = 0;
	VENC_ROI_ATTR_S pstRoiAttr;
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
		venc_chn = 0;
	} else if (!strcmp(roi_data->stream_type, "subStream") &&
	           rk_param_get_int("video.source:enable_venc_1", 0)) {
		venc_chn = 1;
	} else if (!strcmp(roi_data->stream_type, "thirdStream") &&
	           rk_param_get_int("video.source:enable_venc_2", 0)) {
		venc_chn = 2;
	} else {
		LOG_DEBUG("%s is not exit\n", roi_data->stream_type);
		return -1;
	}

	ret = RK_MPI_VENC_SetRoiAttr(venc_chn, &pstRoiAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_VENC_SetRoiAttr to venc %d failed with %#x\n", venc_chn, ret);
		return RK_FAILURE;
	}
	LOG_DEBUG("RK_MPI_VENC_SetRoiAttr to venc %d success\n", venc_chn);

	return ret;
}

int rk_region_clip_set(int venc_chn, region_clip_data_s *region_clip_data) {
	int ret = 0;
	VENC_CHN_PARAM_S stParam;

	RK_MPI_VENC_GetChnParam(venc_chn, &stParam);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_VENC_GetChnParam to venc failed with %#x\n", ret);
		return RK_FAILURE;
	}
	LOG_INFO("RK_MPI_VENC_GetChnParam to venc success\n");
	LOG_INFO("venc_chn is %d\n", venc_chn);
	if (region_clip_data->enabled)
		stParam.stCropCfg.enCropType = VENC_CROP_ONLY;
	else
		stParam.stCropCfg.enCropType = VENC_CROP_NONE;
	stParam.stCropCfg.stCropRect.s32X = region_clip_data->position_x;
	stParam.stCropCfg.stCropRect.s32Y = region_clip_data->position_y;
	stParam.stCropCfg.stCropRect.u32Width = region_clip_data->width;
	stParam.stCropCfg.stCropRect.u32Height = region_clip_data->height;
	// LOG_INFO("xywh is %d,%d,%d,%d\n", stParam.stCropCfg.stCropRect.s32X,
	// stParam.stCropCfg.stCropRect.s32Y, 			stParam.stCropCfg.stCropRect.u32Width,
	// stParam.stCropCfg.stCropRect.u32Height);
	ret = RK_MPI_VENC_SetChnParam(venc_chn, &stParam);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_VENC_SetChnParam to venc failed with %#x\n", ret);
		return RK_FAILURE;
	}
	LOG_INFO("RK_MPI_VENC_SetChnParam to venc success\n");

	return ret;
}

int rk_video_get_rotation(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.source:rotaion");
	*value = rk_param_get_int(entry, 0);

	return 0;
}

int rk_video_set_rotation(int value) {
	LOG_INFO("value is %d\n", value);
	int rotation = 0;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.source:rotaion");
	rk_param_set_int(entry, value);

	return 0;
}

int rk_video_init() {
	LOG_INFO("begin\n");
	int ret = 0;
	enable_jpeg = rk_param_get_int("video.source:enable_jpeg", 1);
	enable_venc_0 = rk_param_get_int("video.source:enable_venc_0", 1);
	enable_venc_1 = rk_param_get_int("video.source:enable_venc_1", 1);
	enable_venc_2 = rk_param_get_int("video.source:enable_venc_2", 1);
	LOG_INFO("enable_jpeg is %d, enable_venc_0 is %d, enable_venc_1 is %d, enable_venc_2 is %d\n",
	         enable_jpeg, enable_venc_0, enable_venc_1, enable_venc_2);

	g_vi_chn_id = rk_param_get_int("video.source:vi_chn_id", 0);
	g_enable_vo = rk_param_get_int("video.source:enable_vo", 1);
	g_vo_dev_id = rk_param_get_int("video.source:vo_dev_id", 3);
	LOG_INFO("g_vi_chn_id is %d, g_enable_vo is %d, g_vo_dev_id is %d\n", g_vi_chn_id, g_enable_vo,
	         g_vo_dev_id);
	g_video_run_ = 1;
	ret = RK_MPI_SYS_Init();
	ret |= rkipc_vi_dev_init();
	ret |= rkipc_rtsp_init();
	ret |= rkipc_rtmp_init();
	ret |= rkipc_vi_chn_init();
	ret |= rkipc_vpss_init();
	if (enable_venc_0)
		ret |= rkipc_venc_0_init();
	if (enable_venc_1)
		ret |= rkipc_venc_1_init();
	if (enable_venc_2)
		ret |= rkipc_venc_2_init();
	if (enable_jpeg)
		ret |= rkipc_venc_3_init();
	ret |= rkipc_bind_init();
	if (g_enable_vo)
		ret |= rkipc_pipe_vpss_vo_init();
	ret |= rkipc_osd_init();
	rk_roi_set_callback_register(rk_roi_set);
	ret |= rk_roi_set_all();
	rk_region_clip_set_callback_register(rk_region_clip_set);
	rk_region_clip_set_all();
	LOG_INFO("over\n");

	return ret;
}

int rk_video_deinit() {
	LOG_INFO("%s\n", __func__);
	g_video_run_ = 0;
	int ret = 0;
	rk_region_clip_set_callback_register(NULL);
	rk_roi_set_callback_register(NULL);
	ret |= rkipc_osd_deinit();
	if (g_enable_vo)
		ret |= rkipc_pipe_vi_vo_deinit();
	ret |= rkipc_bind_deinit();
	if (enable_venc_0) {
		pthread_join(venc_thread_0, NULL);
		ret |= rkipc_venc_0_deinit();
	}
	if (enable_venc_1) {
		pthread_join(venc_thread_1, NULL);
		ret |= rkipc_venc_1_deinit();
	}
	if (enable_venc_2) {
		pthread_join(venc_thread_2, NULL);
		ret |= rkipc_venc_2_deinit();
	}
	if (enable_jpeg) {
		pthread_join(jpeg_venc_thread_id, NULL);
		ret |= rkipc_venc_3_deinit();
	}
	ret |= rkipc_vpss_deinit();
	ret |= rkipc_vi_chn_deinit();
	ret |= rkipc_vi_dev_deinit();
	ret |= RK_MPI_SYS_Exit();
	ret |= rkipc_rtmp_deinit();
	ret |= rkipc_rtsp_deinit();

	return ret;
}

extern char *rkipc_iq_file_path_;
int rk_video_restart() {
	int ret;
	ret = rk_video_deinit();
	rk_isp_deinit(0);
	usleep(100 * 1000);
	rk_isp_init(0, rkipc_iq_file_path_);
	ret |= rk_video_init();

	return ret;
}
