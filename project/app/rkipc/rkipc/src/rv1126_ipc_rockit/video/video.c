// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "video.h"
#include "color_table.h"
#include "common.h"
#include "isp.h"
#include "osd.h"
#include "region_clip.h"
#include "rockiva.h"
#include "roi.h"
#include "rtmp.h"
#include "rtsp.h"
#include "storage.h"

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
#define DRAW_NN_VENC_CHN_ID 0
#define DRAW_NN_OSD_ID 7
#define RED_COLOR 0x0000FF
#define BLUE_COLOR 0xFF0000

#define RV1126_VO_DEV_HDMI 0
#define RV1126_VO_DEV_MIPI 3
#define RV1126_VOP_LAYER_CLUSTER0 0

#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"
#define RTSP_URL_2 "/live/2"
#define RTMP_URL_0 "rtmp://127.0.0.1:1935/live/mainstream"
#define RTMP_URL_1 "rtmp://127.0.0.1:1935/live/substream"
#define RTMP_URL_2 "rtmp://127.0.0.1:1935/live/thirdstream"

#define DRAW_NN_USE_VENC 0

static int get_jpeg_cnt = 0;
static int enable_ivs, enable_jpeg, enable_venc_0, enable_venc_1, enable_venc_2, enable_npu;
static int g_enable_vo, g_vo_dev_id;
static int g_video_run_ = 1;
static int g_nn_osd_run_ = 1;
static int pipe_id_ = 0;
static int dev_id_ = 0;
static const char *tmp_output_data_type = "H.264";
static const char *tmp_rc_mode;
static const char *tmp_h264_profile;
static const char *tmp_smart;
static const char *tmp_gop_mode;
static const char *tmp_rc_quality;
static pthread_t venc_thread_0, venc_thread_1, venc_thread_2, jpeg_venc_thread_id,
    get_vi_0_send_thread, get_vi_2_send_thread, get_nn_update_osd_thread_id, get_ivs_result_thread;

static MPP_CHN_S vi_chn[3], vpss_chn[3], vo_chn, vpss_out_chn[4], venc_chn[4], ivs_chn;
static VO_DEV VoLayer = RV1126_VOP_LAYER_CLUSTER0;
typedef enum rkCOLOR_INDEX_E {
	RGN_COLOR_LUT_INDEX_0 = 0,
	RGN_COLOR_LUT_INDEX_1 = 1,
} COLOR_INDEX_E;

#define CLUT_TABLE_8BPP_NUM 256
const uint32_t clut_table_8bpp[CLUT_TABLE_8BPP_NUM] = {
    0x00000000, 0xff000000, 0xff800000, 0xff008000, 0xff808000, 0xff000080, 0xff800080, 0xff008080,
    0xffc0c0c0, 0xff808080, 0xffff0000, 0xff00ff00, 0xffffff00, 0xff0000ff, 0xffff00ff, 0xff00ffff,
    0xffffffff, 0xff000000, 0xff00005f, 0xff000087, 0xff0000af, 0xff0000d7, 0xff0000ff, 0xff005f00,
    0xff005f5f, 0xff005f87, 0xff005faf, 0xff005fd7, 0xff005fff, 0xff008700, 0xff00875f, 0xff0087af,
    0xff0087d7, 0xff0087ff, 0xff00af00, 0xff00af5f, 0xff00af87, 0xff00afaf, 0xff00afd7, 0xff00afff,
    0xff00d700, 0xff00d75f, 0xff00d787, 0xff00d7af, 0xff00d7d7, 0xff00d7ff, 0xff00ff00, 0xff00ff5f,
    0xff00ff87, 0xff00ffaf, 0xff00ffd7, 0xff00ffff, 0xff5f0000, 0xff5f005f, 0xff5f0087, 0xff5f00af,
    0xff5f00d7, 0xff5f00ff, 0xff5f5f00, 0xff5f5f5f, 0xff5f5f87, 0xff5f5faf, 0xff5f5fd7, 0xff5f5fff,
    0xff5f8700, 0xff5f875f, 0xff5f8787, 0xff5f87af, 0xff5f87d7, 0xff5f87ff, 0xff5faf00, 0xff5faf5f,
    0xff5faf87, 0xff5fafaf, 0xff5fafd7, 0xff5fafff, 0xff5fd700, 0xff5fd75f, 0xff5fd787, 0xff5fd7af,
    0xff5fd7d7, 0xff5fd7ff, 0xff5fff00, 0xff5fff5f, 0xff5fff87, 0xff5fffaf, 0xff5fffd7, 0xff5fffff,
    0xff870000, 0xff87005f, 0xff870087, 0xff8700af, 0xff8700d7, 0xff8700ff, 0xff875f00, 0xff875f5f,
    0xff875f87, 0xff875faf, 0xff875fd7, 0xff875fff, 0xff878700, 0xff87875f, 0xff878787, 0xff8787af,
    0xff8787d7, 0xff8787ff, 0xff87af00, 0xff87af5f, 0xff87af87, 0xff87afaf, 0xff87afd7, 0xff87afff,
    0xff87d700, 0xff87d75f, 0xff87d787, 0xff87d7af, 0xff87d7d7, 0xff87d7ff, 0xff87ff00, 0xff87ff5f,
    0xff87ff87, 0xff87ffaf, 0xff87ffd7, 0xff87ffff, 0xffaf0000, 0xffaf005f, 0xffaf0087, 0xffaf00af,
    0xffaf00d7, 0xffaf00ff, 0xffaf5f00, 0xffaf5f5f, 0xffaf5f87, 0xffaf5faf, 0xffaf5fd7, 0xffaf5fff,
    0xffaf8700, 0xffaf875f, 0xffaf8787, 0xffaf87af, 0xffaf87d7, 0xffaf87ff, 0xffafaf00, 0xffafaf5f,
    0xffafaf87, 0xffafafaf, 0xffafafd7, 0xffafafff, 0xffafd700, 0xffafd75f, 0xffafd787, 0xffafd7af,
    0xffafd7d7, 0xffafd7ff, 0xffafff00, 0xffafff5f, 0xffafff87, 0xffafffaf, 0xffafffd7, 0xffafffff,
    0xffd70000, 0xffd7005f, 0xffd70087, 0xffd700af, 0xffd700d7, 0xffd700ff, 0xffd75f00, 0xffd75f5f,
    0xffd75f87, 0xffd75faf, 0xffd75fd7, 0xffd75fff, 0xffd78700, 0xffd7875f, 0xffd78787, 0xffd787af,
    0xffd787d7, 0xffd787ff, 0xffd7af00, 0xffd7af5f, 0xffd7af87, 0xffd7afaf, 0xffd7afd7, 0xffd7afff,
    0xffd7d700, 0xffd7d75f, 0xffd7d787, 0xffd7d7af, 0xffd7d7d7, 0xffd7d7ff, 0xffd7ff00, 0xffd7ff5f,
    0xffd7ff87, 0xffd7ffaf, 0xffd7ffd7, 0xffd7ffff, 0xffff0000, 0xffff005f, 0xffff0087, 0xffff00af,
    0xffff00d7, 0xffff00ff, 0xffff5f00, 0xffff5f5f, 0xffff5f87, 0xffff5faf, 0xffff5fd7, 0xffff5fff,
    0xffff8700, 0xffff875f, 0xffff8787, 0xffff87af, 0xffff87d7, 0xffff87ff, 0xffffaf00, 0xffffaf5f,
    0xffffaf87, 0xffffafaf, 0xffffafd7, 0xffffafff, 0xffffd700, 0xffffd75f, 0xffffd787, 0xffffd7af,
    0xffffd7d7, 0xffffd7ff, 0xffffff00, 0xffffff5f, 0xffffff87, 0xffffffaf, 0xffffffd7, 0xffffffff,
    0xff080808, 0xff121212, 0xff1c1c1c, 0xff262626, 0xff303030, 0xff3a3a3a, 0xff444444, 0xff4e4e4e,
    0xff585858, 0xff626262, 0xff6c6c6c, 0xff767676, 0xff808080, 0xff8a8a8a, 0xff949494, 0xff9e9e9e,
    0xffa8a8a8, 0xffb2b2b2, 0xffbcbcbc, 0xffc6c6c6, 0xffd0d0d0, 0xffdadada, 0xffe4e4e4, 0xffeeeeee,
};

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
	prctl(PR_SET_NAME, "RkipcVenc0", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	// FILE *fp = fopen("/data/venc.h265", "wb");
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_0, &stFrame, 2500);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			// fwrite(data, 1, stFrame.pstPack->u32Len, fp);
			// fflush(fp);
			// LOG_INFO("Count:%d, Len:%d, PTS is %" PRId64", enH264EType is %d\n", loopCount,
			// stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			// stFrame.pstPack->DataType.enH264EType);

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
	prctl(PR_SET_NAME, "RkipcVenc1", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_1, &stFrame, 2500);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			// LOG_INFO("Count:%d, Len:%d, PTS is %" PRId64", enH264EType is %d\n", loopCount,
			// stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			// stFrame.pstPack->DataType.enH264EType);
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
	prctl(PR_SET_NAME, "RkipcVenc2", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_2, &stFrame, 2500);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			// LOG_INFO("Count:%d, Len:%d, PTS is %" PRId64", enH264EType is %d\n", loopCount,
			// stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			// stFrame.pstPack->DataType.enH264EType);
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
	prctl(PR_SET_NAME, "RkipcGetJpeg", 0, 0, 0);
	VENC_STREAM_S stFrame;
	int loopCount = 0;
	int ret = 0;
	char file_name[128] = {0};
	char record_path[256];

	memset(&record_path, 0, sizeof(record_path));
	strcat(record_path, rk_param_get_string("storage:mount_path", "/userdata"));
	strcat(record_path, "/");
	strcat(record_path, rk_param_get_string("storage.0:folder_name", "video0"));

	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));
	// drop first frame
	ret = RK_MPI_VENC_GetStream(JPEG_VENC_CHN, &stFrame, 1000);
	if (ret == RK_SUCCESS)
		RK_MPI_VENC_ReleaseStream(JPEG_VENC_CHN, &stFrame);
	else
		LOG_ERROR("RK_MPI_VENC_GetStream timeout %x\n", ret);
	while (g_video_run_) {
		if (!get_jpeg_cnt) {
			usleep(300 * 1000);
			continue;
		}
		// get the frame
		ret = RK_MPI_VENC_GetStream(JPEG_VENC_CHN, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			LOG_INFO("Count:%d, Len:%d, PTS is %" PRId64 ", enH264EType is %d\n", loopCount,
			         stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			         stFrame.pstPack->DataType.enH264EType);
			// save jpeg file
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			snprintf(file_name, 128, "%s/%d%02d%02d%02d%02d%02d.jpeg", record_path,
			         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
			         tm.tm_sec);
			LOG_INFO("file_name is %s\n", file_name);
			FILE *fp = fopen(file_name, "wb");
			if (fp == NULL) {
				LOG_ERROR("fp is NULL\n");
			} else {
				fwrite(data, 1, stFrame.pstPack->u32Len, fp);
			}
			// release the frame
			ret = RK_MPI_VENC_ReleaseStream(JPEG_VENC_CHN, &stFrame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
			}
			if (fp) {
				fflush(fp);
				fclose(fp);
			}
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VENC_GetStream timeout %x\n", ret);
		}
		get_jpeg_cnt--;
		RK_MPI_VENC_StopRecvFrame(JPEG_VENC_CHN);
	}
	if (stFrame.pstPack)
		free(stFrame.pstPack);

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

RK_S32 draw_rect_nv12(int fd, RK_U32 width, RK_U32 height, int rgn_x, int rgn_y, int rgn_w,
                      int rgn_h, int line_pixel, COLOR_INDEX_E color_index) {
	RK_U32 color = 0;
	if (color_index == RGN_COLOR_LUT_INDEX_0)
		color = 0xff0000ff;
	if (color_index == RGN_COLOR_LUT_INDEX_1)
		color = 0xffff0000;
	// LOG_INFO("YUV %dx%d, rgn (%d,%d,%d,%d), line pixel %d\n", width, height, rgn_x, rgn_y, rgn_w,
	//          rgn_h, line_pixel);
	rga_buffer_t rga_buffer =
	    wrapbuffer_fd_t(fd, width, height, width, height, RK_FORMAT_YCbCr_420_SP);
	im_rect rect_up = {rgn_x, rgn_y, rgn_w, line_pixel};
	im_rect rect_buttom = {rgn_x, rgn_y + rgn_h - line_pixel, rgn_w, line_pixel};
	im_rect rect_left = {rgn_x, rgn_y, line_pixel, rgn_h};
	im_rect rect_right = {rgn_x + rgn_w - line_pixel, rgn_y, line_pixel, rgn_h};
	IM_STATUS STATUS = imfill(rga_buffer, rect_up, color);
	STATUS |= imfill(rga_buffer, rect_buttom, color);
	STATUS |= imfill(rga_buffer, rect_left, color);
	STATUS |= imfill(rga_buffer, rect_right, color);
	if (STATUS != IM_STATUS_SUCCESS)
		LOG_ERROR("STATUS is %d\n", STATUS);
	return 0;
}

static void *rkipc_get_vi_0_send(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcGetVi0", 0, 0, 0);
	int ret;
	int line_pixel = 6;
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	int32_t loopCount = 0;
	long long last_ba_result_time, before_time, cost_time;
	RockIvaBaResult ba_result;
	RockIvaBaObjectInfo *object;
	VIDEO_FRAME_INFO_S stViFrame;

	memset(&ba_result, 0, sizeof(ba_result));
	while (g_video_run_) {
		ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_0, &stViFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
			int32_t fd = RK_MPI_MB_Handle2Fd(stViFrame.stVFrame.pMbBlk);
			// draw
			// before_time = rkipc_get_curren_time_ms();

			ret = rkipc_rknn_object_get(&ba_result);
			// LOG_DEBUG("ret is %d, ba_result.objNum is %d\n", ret, ba_result.objNum);
			if ((ret == -1) && (rkipc_get_curren_time_ms() - last_ba_result_time > 300))
				ba_result.objNum = 0;
			if (ret == 0)
				last_ba_result_time = rkipc_get_curren_time_ms();
			for (int i = 0; i < ba_result.objNum; i++) {
				int x, y, w, h;
				object = &ba_result.triggerObjects[i];
				// LOG_INFO("topLeft:[%d,%d], bottomRight:[%d,%d],"
				// 			"objId is %d, frameId is %d, score is %d, type is %d\n",
				// 			object->objInfo.rect.topLeft.x, object->objInfo.rect.topLeft.y,
				// 			object->objInfo.rect.bottomRight.x,
				// 			object->objInfo.rect.bottomRight.y, object->objInfo.objId,
				// 			object->objInfo.frameId, object->objInfo.score, object->objInfo.type);
				x = video_width * object->objInfo.rect.topLeft.x / 10000;
				y = video_height * object->objInfo.rect.topLeft.y / 10000;
				w = video_width *
				    (object->objInfo.rect.bottomRight.x - object->objInfo.rect.topLeft.x) / 10000;
				h = video_height *
				    (object->objInfo.rect.bottomRight.y - object->objInfo.rect.topLeft.y) / 10000;
				x = x / 16 * 16;
				y = y / 16 * 16;
				w = (w + 3) / 16 * 16;
				h = (h + 3) / 16 * 16;

				while (x + w + line_pixel >= video_width) {
					w -= 8;
				}
				while (y + h + line_pixel >= video_height) {
					h -= 8;
				}
				if (x < 0 || y < 0 || w <= 0 || h <= 0) {
					continue;
				}
				// LOG_DEBUG("i is %d, x,y,w,h is %d,%d,%d,%d\n", i, x, y, w, h);
				if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_PERSON) {
					draw_rect_nv12(fd, video_width, video_height, x, y, w, h, line_pixel,
					               RGN_COLOR_LUT_INDEX_0);
				} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_FACE) {
					draw_rect_nv12(fd, video_width, video_height, x, y, w, h, line_pixel,
					               RGN_COLOR_LUT_INDEX_1);
				} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_VEHICLE) {
					draw_rect_nv12(fd, video_width, video_height, x, y, w, h, line_pixel,
					               RGN_COLOR_LUT_INDEX_1);
				} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_NON_VEHICLE) {
					draw_rect_nv12(fd, video_width, video_height, x, y, w, h, line_pixel,
					               RGN_COLOR_LUT_INDEX_1);
				}
				// LOG_INFO("draw rect time-consuming is %lld\n",(rkipc_get_curren_time_ms() -
				// 	last_ba_result_time));
				// LOG_INFO("triggerRules is %d, ruleID is %d, triggerType is %d\n",
				// 			object->triggerRules,
				// 			object->firstTrigger.ruleID,
				// 			object->firstTrigger.triggerType);
			}

			// cost_time = rkipc_get_curren_time_ms() - before_time;
			// LOG_INFO("cost time is %lld\n", cost_time);

			RK_MPI_VENC_SendFrame(VIDEO_PIPE_0, &stViFrame, 1000);
			ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_0, &stViFrame);
			if (ret != RK_SUCCESS)
				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x", ret);
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x", ret);
		}
	}

	return NULL;
}

int rkipc_pipe_0_init() {
	int ret = 0;
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	int video_max_width = rk_param_get_int("video.0:max_width", -1);
	int video_max_height = rk_param_get_int("video.0:max_height", -1);
	LOG_INFO("video_width is %d, video_height is %d\n", video_width, video_height);

	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = 4;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	if ((video_width == video_max_width) && (video_height == video_max_height) && (!enable_jpeg)) {
		LOG_INFO("w,h is %d,%d , enable_jpeg is %d, need use rkispp_m_bypass and fbc format\n",
		         video_width, video_height, enable_jpeg);
		vi_chn_attr.enCompressMode = COMPRESS_AFBC_16x16;
		memcpy(vi_chn_attr.stIspOpt.aEntityName, "rkispp_m_bypass", strlen("rkispp_m_bypass"));
	} else {
		vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
		memcpy(vi_chn_attr.stIspOpt.aEntityName, "rkispp_scale0", strlen("rkispp_scale0"));
	}
	vi_chn_attr.u32Depth = 1;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_0, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_0);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%#x\n", ret);
		return ret;
	}
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
		LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
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

	vi_chn[0].enModId = RK_ID_VI;
	vi_chn[0].s32DevId = 0;
	vi_chn[0].s32ChnId = VIDEO_PIPE_0;
#if DRAW_NN_USE_VENC
	venc_chn[0].enModId = RK_ID_VENC;
	venc_chn[0].s32DevId = 0;
	venc_chn[0].s32ChnId = VIDEO_PIPE_0;

	ret = RK_MPI_SYS_Bind(&vi_chn[0], &venc_chn[0]);
	if (ret)
		LOG_ERROR("Bind VI[0] and VENC[0] error! ret=%#x\n", ret);
	else
		LOG_INFO("Bind VI[0] and VENC[0] success\n");
#else
	// use rga draw nn result
	pthread_create(&get_vi_0_send_thread, NULL, rkipc_get_vi_0_send, NULL);
#endif

	// JPEG
	if (enable_jpeg) {
		rkipc_venc_jpeg_init();
		venc_chn[JPEG_VENC_CHN].enModId = RK_ID_VENC;
		venc_chn[JPEG_VENC_CHN].s32DevId = 0;
		venc_chn[JPEG_VENC_CHN].s32ChnId = JPEG_VENC_CHN;
		ret = RK_MPI_SYS_Bind(&vi_chn[0], &venc_chn[JPEG_VENC_CHN]);
		if (ret)
			LOG_ERROR("Bind VI[0] and VENC[JPEG_VENC_CHN] error! ret=%#x\n", ret);
		else
			LOG_INFO("Bind VI[0] and VENC[JPEG_VENC_CHN] success\n");
	}

	return ret;
}

int rkipc_pipe_0_deinit() {
	int ret = 0;
	// JPEG
	if (enable_jpeg) {
		pthread_join(jpeg_venc_thread_id, NULL);
		ret = RK_MPI_SYS_UnBind(&vi_chn[0], &venc_chn[JPEG_VENC_CHN]);
		if (ret)
			LOG_ERROR("Unbind VI[0] and VENC[JPEG_VENC_CHN] error! ret=%#x\n", ret);
		else
			LOG_INFO("Unbind VI[0] and VENC[JPEG_VENC_CHN] success\n");
		rkipc_venc_jpeg_deinit();
	}
	pthread_join(venc_thread_0, NULL);

#if DRAW_NN_USE_VENC
	// unbind
	ret = RK_MPI_SYS_UnBind(&vi_chn[0], &venc_chn[0]);
	if (ret)
		LOG_ERROR("Unbind VI and VENC error! ret=%#x\n", ret);
	else
		LOG_DEBUG("Unbind VI and VENC success\n");
#else
	pthread_join(get_vi_0_send_thread, NULL);
#endif

	// VENC
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_0);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");
	// VI
	ret = RK_MPI_VI_DisableChn(0, VIDEO_PIPE_0);
	if (ret)
		LOG_ERROR("ERROR: Destroy VI error! ret=%#x\n", ret);

	return ret;
}

int rkipc_pipe_1_init() {
	int ret = 0;
	int video_width = rk_param_get_int("video.1:width", -1);
	int video_height = rk_param_get_int("video.1:height", -1);

	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = 4;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
	memcpy(vi_chn_attr.stIspOpt.aEntityName, "rkispp_scale1", strlen("rkispp_scale1"));
	vi_chn_attr.u32Depth = 1;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_1, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_1);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%#x\n", ret);
		return ret;
	}
	// VENC[1] and VENC[2]
	if (enable_venc_1) {
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
				venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate =
				    rk_param_get_int("video.1:max_rate", -1);
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
				venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate =
				    rk_param_get_int("video.1:max_rate", -1);
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
				venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate =
				    rk_param_get_int("video.1:max_rate", -1);
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
				venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate =
				    rk_param_get_int("video.1:max_rate", -1);
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
			LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
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

		vi_chn[1].enModId = RK_ID_VI;
		vi_chn[1].s32DevId = 0;
		vi_chn[1].s32ChnId = VIDEO_PIPE_1;
		venc_chn[1].enModId = RK_ID_VENC;
		venc_chn[1].s32DevId = 0;
		venc_chn[1].s32ChnId = VIDEO_PIPE_1;

		ret = RK_MPI_SYS_Bind(&vi_chn[1], &venc_chn[1]);
		if (ret)
			LOG_ERROR("Bind VI[1] and VENC[1] error! ret=%#x\n", ret);
		else
			LOG_INFO("Bind VI[1] and VENC[1] success\n");
	}
	if (enable_venc_2) {
		video_width = rk_param_get_int("video.2:width", -1);
		video_height = rk_param_get_int("video.2:height", -1);
		// VPSS
		VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};
		VPSS_GRP VpssGrp = VIDEO_PIPE_1;
		VPSS_GRP_ATTR_S stVpssGrpAttr;
		VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
		memset(&stVpssGrpAttr, 0, sizeof(stVpssGrpAttr));
		memset(&stVpssChnAttr[0], 0, sizeof(stVpssChnAttr[0]));
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
		stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
		stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
		stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
		stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
		stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
		stVpssChnAttr[0].u32Width = video_width;
		stVpssChnAttr[0].u32Height = video_height;
		stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
		ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[0], &stVpssChnAttr[0]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("0: RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
		ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[0]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("0: RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);
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
				venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate =
				    rk_param_get_int("video.2:max_rate", -1);
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
				venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate =
				    rk_param_get_int("video.2:max_rate", -1);
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
				venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate =
				    rk_param_get_int("video.2:max_rate", -1);
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
				venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate =
				    rk_param_get_int("video.2:max_rate", -1);
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
			LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
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

		vi_chn[1].enModId = RK_ID_VI;
		vi_chn[1].s32DevId = 0;
		vi_chn[1].s32ChnId = VIDEO_PIPE_1;

		vpss_chn[1].enModId = RK_ID_VPSS;
		vpss_chn[1].s32DevId = VIDEO_PIPE_1;
		vpss_chn[1].s32ChnId = 0;

		venc_chn[2].enModId = RK_ID_VENC;
		venc_chn[2].s32DevId = 0;
		venc_chn[2].s32ChnId = VIDEO_PIPE_2;

		ret = RK_MPI_SYS_Bind(&vi_chn[1], &vpss_chn[1]);
		if (ret)
			LOG_ERROR("Bind VI[1] and VPSS[1] error! ret=%#x\n", ret);
		else
			LOG_INFO("Bind VI[1] and VPSS[1] success\n");

		ret = RK_MPI_SYS_Bind(&vpss_chn[1], &venc_chn[2]);
		if (ret)
			LOG_ERROR("Bind VPSS[1] and VENC[2] error! ret=%#x\n", ret);
		else
			LOG_INFO("Bind VPSS[1] and VENC[2] success\n");
	}

	return ret;
}

int rkipc_pipe_1_deinit() {
	int ret = 0;

	if (enable_venc_2) {
		pthread_join(venc_thread_2, NULL);
		ret = RK_MPI_SYS_UnBind(&vpss_chn[1], &venc_chn[2]);
		if (ret)
			LOG_ERROR("Unbind VPSS[1] and VENC[2] error! ret=%#x\n", ret);
		ret = RK_MPI_SYS_UnBind(&vi_chn[1], &vpss_chn[1]);
		if (ret)
			LOG_ERROR("Unbind VI[1] and VPSS[1] error! ret=%#x\n", ret);

		ret |= RK_MPI_VPSS_StopGrp(1);
		ret |= RK_MPI_VPSS_DisableChn(1, 0);
		ret |= RK_MPI_VPSS_DestroyGrp(1);

		ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_2);
		ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_2);
		if (ret)
			LOG_ERROR("ERROR: Destroy VENC[2] error! ret=%#x\n", ret);
	}

	if (enable_venc_1) {
		pthread_join(venc_thread_1, NULL);
		ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_1);
		ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_1);
		if (ret)
			LOG_ERROR("ERROR: Destroy VENC[1] error! ret=%#x\n", ret);
	}

	// VI
	ret = RK_MPI_VI_DisableChn(0, VIDEO_PIPE_1);
	if (ret)
		LOG_ERROR("ERROR: Destroy VI[1] error! ret=%#x\n", ret);

	return ret;
}

int rkipc_venc_jpeg_init() {
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
		LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
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

	pthread_create(&jpeg_venc_thread_id, NULL, rkipc_get_jpeg, NULL);

	return ret;
}

int rkipc_venc_jpeg_deinit() {
	int ret = 0;
	ret = RK_MPI_VENC_StopRecvFrame(JPEG_VENC_CHN);
	ret |= RK_MPI_VENC_DestroyChn(JPEG_VENC_CHN);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");

	return ret;
}

static void *rkipc_get_vi_2_send(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcGetVi2", 0, 0, 0);
	int ret;
	int32_t loopCount = 0;
	VIDEO_FRAME_INFO_S stViFrame;
	int npu_cycle_time_ms = 1000 / rk_param_get_int("video.source:npu_fps", 10);

	long long before_time, cost_time;
	while (g_video_run_) {
		before_time = rkipc_get_curren_time_ms();
		ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_2, &stViFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
			int32_t fd = RK_MPI_MB_Handle2Fd(stViFrame.stVFrame.pMbBlk);
			rkipc_rockiva_write_nv12_frame_by_fd(stViFrame.stVFrame.u32Width,
			                                     stViFrame.stVFrame.u32Height, loopCount, fd);
			ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_2, &stViFrame);
			if (ret != RK_SUCCESS)
				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x", ret);
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x", ret);
		}
		cost_time = rkipc_get_curren_time_ms() - before_time;
		if ((cost_time > 0) && (cost_time < npu_cycle_time_ms))
			usleep((npu_cycle_time_ms - cost_time) * 1000);
	}
	return NULL;
}

int rkipc_vo_init() {
	int ret;
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

	ret = RK_MPI_VO_SetLayerSpliceMode(VoLayer, VO_SPLICE_MODE_RGA);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetLayerSpliceMode VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_INFO("RK_MPI_VO_SetLayerSpliceMode success\n");

	ret = RK_MPI_VO_EnableLayer(VoLayer);
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

	return ret;
}

static void *rkipc_ivs_get_results(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcGetIVS", 0, 0, 0);
	int ret, i;
	IVS_RESULT_INFO_S stResults;
	int resultscount = 0;
	int count = 0;
	int md = rk_param_get_int("ivs:md", 0);
	int od = rk_param_get_int("ivs:od", 0);
	int width = rk_param_get_int("video.2:width", 960);
	int height = rk_param_get_int("video.2:height", 540);
	int md_area_threshold = width * height * 0.3;

	while (g_video_run_) {
		ret = RK_MPI_IVS_GetResults(0, &stResults, 1000);
		if (ret >= 0) {
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
			LOG_ERROR("get chn %d fail %d\n", 0, ret);
			usleep(50000llu);
		}
	}
	return NULL;
}

int rkipc_ivs_init() {
	int ret;
	int video_width = rk_param_get_int("video.2:width", -1);
	int video_height = rk_param_get_int("video.2:height", -1);
	int buf_cnt = 2;
	int smear = rk_param_get_int("ivs:smear", 1);
	int weightp = rk_param_get_int("ivs:weightp", 1);
	int md = rk_param_get_int("ivs:md", 0);
	int od = rk_param_get_int("ivs:od", 0);
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

	if (md == 1 || od == 1)
		pthread_create(&get_ivs_result_thread, NULL, rkipc_ivs_get_results, NULL);

	return 0;
}

int rkipc_ivs_deinit() {
	int ret;
	pthread_join(get_ivs_result_thread, NULL);
	// IVS
	ret = RK_MPI_IVS_DestroyChn(0);
	if (ret)
		LOG_ERROR("ERROR: Destroy IVS error! ret=%#x\n", ret);
	else
		LOG_DEBUG("RK_MPI_IVS_DestroyChn success\n");

	return 0;
}

int rkipc_pipe_2_init() {
	int ret = 0;
	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = 4;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = rk_param_get_int("video.2:width", -1);
	vi_chn_attr.stSize.u32Height = rk_param_get_int("video.2:height", -1);
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
	memcpy(vi_chn_attr.stIspOpt.aEntityName, "rkispp_scale2", strlen("rkispp_scale2"));
	vi_chn_attr.u32Depth = 1;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_2, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_2);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%#x\n", ret);
		return ret;
	}
	// VPSS for rotate
	VPSS_CHN VpssChn[VPSS_MAX_CHN_NUM] = {VPSS_CHN0, VPSS_CHN1, VPSS_CHN2, VPSS_CHN3};
	VPSS_GRP VpssGrp = VIDEO_PIPE_2;
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
	if (g_vo_dev_id == 0) {
		ret = RK_MPI_VPSS_SetChnRotation(VpssGrp, VpssChn[0], ROTATION_90);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("RK_MPI_VPSS_SetChnRotation error! ret is %#x\n", ret);
			return ret;
		}
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

	vi_chn[2].enModId = RK_ID_VI;
	vi_chn[2].s32DevId = 0;
	vi_chn[2].s32ChnId = VIDEO_PIPE_2;
	vpss_chn[2].enModId = RK_ID_VPSS;
	vpss_chn[2].s32DevId = 2;
	vpss_chn[2].s32ChnId = 0;
	vo_chn.enModId = RK_ID_VO;
	vo_chn.s32DevId = RV1126_VOP_LAYER_CLUSTER0;
	vo_chn.s32ChnId = 0;

	if (g_enable_vo) {
		ret = rkipc_vo_init();
		ret = RK_MPI_SYS_Bind(&vi_chn[2], &vpss_chn[2]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("vi[2] and vpss[2] bind error! ret=%#x\n", ret);
			return ret;
		}
		ret = RK_MPI_SYS_Bind(&vpss_chn[2], &vo_chn);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("vpss[2] and vo bind error! ret=%#x\n", ret);
			return ret;
		}
	}

	if (enable_npu) {
		pthread_create(&get_vi_2_send_thread, NULL, rkipc_get_vi_2_send, NULL);
#if DRAW_NN_USE_VENC
		rkipc_osd_draw_nn_init();
#endif
	}
	if (enable_ivs) {
		rkipc_ivs_init();
		// bind
		vi_chn[2].enModId = RK_ID_VI;
		vi_chn[2].s32DevId = 0;
		vi_chn[2].s32ChnId = VIDEO_PIPE_2;
		ivs_chn.enModId = RK_ID_IVS;
		ivs_chn.s32DevId = 0;
		ivs_chn.s32ChnId = 0;
		ret = RK_MPI_SYS_Bind(&vi_chn[2], &ivs_chn);
		if (ret)
			LOG_ERROR("Bind VI and IVS error! ret=%#x\n", ret);
		else
			LOG_DEBUG("Bind VI and IVS success\n");
	}

	return 0;
}

int rkipc_pipe_2_deinit() {
	int ret;
	if (g_enable_vo) {
		ret = RK_MPI_SYS_UnBind(&vpss_chn[2], &vo_chn);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("vpss and vo unbind error! ret=%#x\n", ret);
			return ret;
		}
		ret = RK_MPI_SYS_UnBind(&vi_chn[2], &vpss_chn[2]);
		if (ret) {
			LOG_ERROR("vi and vpss unbind error! ret=%#x\n", ret);
			return -1;
		}
		// disable vo layer
		ret = RK_MPI_VO_DisableLayer(VoLayer);
		if (ret) {
			LOG_ERROR("RK_MPI_VO_DisableLayer failed, ret=%#x\n", ret);
			return -1;
		}
		// disable vo dev
		ret = RK_MPI_VO_Disable(g_vo_dev_id);
		if (ret) {
			LOG_ERROR("RK_MPI_VO_Disable failed, ret=%#x\n", ret);
			return -1;
		}
		ret = RK_MPI_VO_UnBindLayer(VoLayer, g_vo_dev_id);
		if (ret) {
			LOG_ERROR("RK_MPI_VO_UnBindLayer failed, ret=%#x\n", ret);
			return -1;
		}
		// disable VPSS
		ret |= RK_MPI_VPSS_StopGrp(VIDEO_PIPE_2);
		ret |= RK_MPI_VPSS_DisableChn(VIDEO_PIPE_2, 0);
		ret |= RK_MPI_VPSS_DestroyGrp(VIDEO_PIPE_2);
	}
	if (enable_npu) {
		pthread_join(get_vi_2_send_thread, NULL);
#if DRAW_NN_USE_VENC
		rkipc_osd_draw_nn_deinit();
#endif
	}
	if (enable_ivs) {
		// unbind
		vi_chn[2].enModId = RK_ID_VI;
		vi_chn[2].s32DevId = 0;
		vi_chn[2].s32ChnId = VIDEO_PIPE_2;
		ivs_chn.enModId = RK_ID_IVS;
		ivs_chn.s32DevId = 0;
		ivs_chn.s32ChnId = 0;
		ret = RK_MPI_SYS_UnBind(&vi_chn[2], &ivs_chn);
		if (ret)
			LOG_ERROR("Unbind VI and IVS error! ret=%#x\n", ret);
		else
			LOG_DEBUG("Unbind VI and IVS success\n");
		rkipc_ivs_deinit();
	}
	// disable VI
	ret = RK_MPI_VI_DisableChn(0, VIDEO_PIPE_2);
	if (ret)
		LOG_ERROR("ERROR: Destroy VI error! ret=%#x\n", ret);

	return 0;
}

RK_S32 draw_rect_argb8888(RK_U64 buffer, RK_U32 width, RK_U32 height, int rgn_x, int rgn_y,
                          int rgn_w, int rgn_h, int line_pixel, COLOR_INDEX_E color_index) {
	RK_U32 color = 0;
	if (color_index == RGN_COLOR_LUT_INDEX_0)
		color = 0xff0000ff;
	if (color_index == RGN_COLOR_LUT_INDEX_1)
		color = 0xffff0000;

	rga_buffer_t rga_buffer =
	    wrapbuffer_virtualaddr_t((void *)buffer, width, height, width, height, RK_FORMAT_ARGB_8888);
	im_rect rect_up = {rgn_x, rgn_y, rgn_w, line_pixel};
	im_rect rect_buttom = {rgn_x, rgn_y + rgn_h - line_pixel, rgn_w, line_pixel};
	im_rect rect_left = {rgn_x, rgn_y, line_pixel, rgn_h};
	im_rect rect_right = {rgn_x + rgn_w - line_pixel, rgn_y, line_pixel, rgn_h};
	IM_STATUS STATUS = imfill(rga_buffer, rect_up, color);
	STATUS |= imfill(rga_buffer, rect_buttom, color);
	STATUS |= imfill(rga_buffer, rect_left, color);
	STATUS |= imfill(rga_buffer, rect_right, color);
	if (STATUS != IM_STATUS_SUCCESS)
		LOG_ERROR("STATUS is %d\n", STATUS);

	return 0;
}

RK_S32 draw_rect_8bpp(RK_U64 buffer, RK_U32 width, RK_U32 height, int rgn_x, int rgn_y, int rgn_w,
                      int rgn_h, int line_pixel, COLOR_INDEX_E color_index) {
	// actual draw color index, need set color table, and rga dst not support RK_FORMAT_BPP8
	int i, j;
	int pixel_format_byte = 1;
	RK_U8 *ptr = buffer;
	RK_U8 color = 0;

	if (color_index == RGN_COLOR_LUT_INDEX_0)
		color = 10;
	if (color_index == RGN_COLOR_LUT_INDEX_1)
		color = 11;
	LOG_DEBUG("YUV %dx%d, rgn (%d,%d,%d,%d), line pixel %d\n", width, height, rgn_x, rgn_y, rgn_w,
	          rgn_h, line_pixel);

	//  draw top line
	ptr += (width * rgn_y + rgn_x);
	for (i = 0; i < line_pixel; i++) {
		memset(ptr, color,
		       (rgn_w + line_pixel) * pixel_format_byte); // memset is byte
		ptr += width;
	}
	// draw letft/right line
	for (i = 0; i < (rgn_h - line_pixel * 2); i++) {
		for (j = 0; j < line_pixel; j++) {
			*(ptr + j) = color;
			*(ptr + rgn_w + j) = color;
		}
		ptr += width;
	}
	// draw bottom line
	for (i = 0; i < line_pixel; i++) {
		memset(ptr, color,
		       (rgn_w + line_pixel) * pixel_format_byte); // memset is byte
		ptr += width;
	}

	return 0;
}

static void *rkipc_get_nn_update_osd(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcNpuOsd", 0, 0, 0);

	int ret = 0;
	int line_pixel = 8;
	int change_to_nothing_flag = 0;
	int video_width = 0;
	int video_height = 0;
	int rotation = 0;
	long long last_ba_result_time;
	RockIvaBaResult ba_result;
	RockIvaBaObjectInfo *object;
	RGN_HANDLE RgnHandle = DRAW_NN_OSD_ID;
	RGN_CANVAS_INFO_S stCanvasInfo;

	memset(&stCanvasInfo, 0, sizeof(RGN_CANVAS_INFO_S));
	memset(&ba_result, 0, sizeof(ba_result));
	while (g_nn_osd_run_) {
		usleep(1000 * 1000);
		rotation = rk_param_get_int("video.source:rotation", 0);
		if (rotation == 90 || rotation == 270) {
			video_width = rk_param_get_int("video.0:height", -1);
			video_height = rk_param_get_int("video.0:width", -1);
		} else {
			video_width = rk_param_get_int("video.0:width", -1);
			video_height = rk_param_get_int("video.0:height", -1);
		}
		ret = rkipc_rknn_object_get(&ba_result);
		// LOG_DEBUG("ret is %d, ba_result.objNum is %d\n", ret, ba_result.objNum);

		if ((ret == -1) && (rkipc_get_curren_time_ms() - last_ba_result_time > 300))
			ba_result.objNum = 0;
		if (ret == 0)
			last_ba_result_time = rkipc_get_curren_time_ms();

		ret = RK_MPI_RGN_GetCanvasInfo(RgnHandle, &stCanvasInfo);
		if (ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_RGN_GetCanvasInfo failed with %#x!", ret);
			continue;
		}
		memset((void *)stCanvasInfo.u64VirAddr, 0,
		       stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight);
		// draw
		for (int i = 0; i < ba_result.objNum; i++) {
			int x, y, w, h;
			object = &ba_result.triggerObjects[i];
			// LOG_INFO("topLeft:[%d,%d], bottomRight:[%d,%d],"
			// 			"objId is %d, frameId is %d, score is %d, type is %d\n",
			// 			object->objInfo.rect.topLeft.x, object->objInfo.rect.topLeft.y,
			// 			object->objInfo.rect.bottomRight.x,
			// 			object->objInfo.rect.bottomRight.y, object->objInfo.objId,
			// 			object->objInfo.frameId, object->objInfo.score, object->objInfo.type);
			x = video_width * object->objInfo.rect.topLeft.x / 10000;
			y = video_height * object->objInfo.rect.topLeft.y / 10000;
			w = video_width *
			    (object->objInfo.rect.bottomRight.x - object->objInfo.rect.topLeft.x) / 10000;
			h = video_height *
			    (object->objInfo.rect.bottomRight.y - object->objInfo.rect.topLeft.y) / 10000;
			x = x / 16 * 16;
			y = y / 16 * 16;
			w = (w + 3) / 16 * 16;
			h = (h + 3) / 16 * 16;

			while (x + w + line_pixel >= video_width) {
				w -= 8;
			}
			while (y + h + line_pixel >= video_height) {
				h -= 8;
			}
			if (x < 0 || y < 0 || w <= 0 || h <= 0) {
				continue;
			}
			// LOG_DEBUG("i is %d, x,y,w,h is %d,%d,%d,%d\n", i, x, y, w, h);
			if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_PERSON) {
				draw_rect_8bpp(stCanvasInfo.u64VirAddr, stCanvasInfo.u32VirWidth,
				               stCanvasInfo.u32VirHeight, x, y, w, h, line_pixel,
				               RGN_COLOR_LUT_INDEX_0);
			} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_FACE) {
				draw_rect_8bpp(stCanvasInfo.u64VirAddr, stCanvasInfo.u32VirWidth,
				               stCanvasInfo.u32VirHeight, x, y, w, h, line_pixel,
				               RGN_COLOR_LUT_INDEX_1);
			} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_VEHICLE) {
				draw_rect_8bpp(stCanvasInfo.u64VirAddr, stCanvasInfo.u32VirWidth,
				               stCanvasInfo.u32VirHeight, x, y, w, h, line_pixel,
				               RGN_COLOR_LUT_INDEX_1);
			} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_NON_VEHICLE) {
				draw_rect_8bpp(stCanvasInfo.u64VirAddr, stCanvasInfo.u32VirWidth,
				               stCanvasInfo.u32VirHeight, x, y, w, h, line_pixel,
				               RGN_COLOR_LUT_INDEX_1);
			}
			// LOG_INFO("draw rect time-consuming is %lld\n",(rkipc_get_curren_time_ms() -
			// 	last_ba_result_time));
			// LOG_INFO("triggerRules is %d, ruleID is %d, triggerType is %d\n",
			// 			object->triggerRules,
			// 			object->firstTrigger.ruleID,
			// 			object->firstTrigger.triggerType);
		}
		ret = RK_MPI_RGN_UpdateCanvas(RgnHandle);
		if (ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_RGN_UpdateCanvas failed with %#x!", ret);
			continue;
		}
	}

	return 0;
}

#ifdef DRAW_NN_USE_VENC
int rkipc_osd_draw_nn_init() {
	LOG_DEBUG("start\n");
	int ret = 0;
	RGN_HANDLE RgnHandle = DRAW_NN_OSD_ID;
	RGN_ATTR_S stRgnAttr;
	MPP_CHN_S stMppChn;
	RGN_CHN_ATTR_S stRgnChnAttr;
	BITMAP_S stBitmap;
	int rotation = rk_param_get_int("video.source:rotation", 0);

	// create overlay regions
	memset(&stRgnAttr, 0, sizeof(stRgnAttr));
	stRgnAttr.enType = OVERLAY_RGN;
	stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_8BPP;
	stRgnAttr.unAttr.stOverlay.u32CanvasNum = 1;
	if (rotation == 90 || rotation == 270) {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.0:height", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.0:width", -1);
	} else {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.0:width", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.0:height", -1);
	}
	ret = RK_MPI_RGN_Create(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Create (%d) failed with %#x\n", RgnHandle, ret);
		RK_MPI_RGN_Destroy(RgnHandle);
		return RK_FAILURE;
	}
	LOG_DEBUG("The handle: %d, create success\n", RgnHandle);
	stRgnAttr.unAttr.stOverlay.u32ClutNum = CLUT_TABLE_8BPP_NUM;
	memcpy(stRgnAttr.unAttr.stOverlay.u32Clut, clut_table_8bpp,
	       sizeof(stRgnAttr.unAttr.stOverlay.u32Clut[0]) * CLUT_TABLE_8BPP_NUM);
	ret = RK_MPI_RGN_SetAttr(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_SetAttr (%d) failed with %#x!", RgnHandle, ret);
		return RK_FAILURE;
	}

	// display overlay regions to venc groups
	memset(&stRgnChnAttr, 0, sizeof(stRgnChnAttr));
	stRgnChnAttr.bShow = RK_TRUE;
	stRgnChnAttr.enType = OVERLAY_RGN;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	// 1126 osd alpha not support multi layer, so use layer 0 to draw nn
	stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = 0;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = DRAW_NN_VENC_CHN_ID;
	ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
		return RK_FAILURE;
	}
	LOG_DEBUG("RK_MPI_RGN_AttachToChn to venc0 success\n");
	pthread_create(&get_nn_update_osd_thread_id, NULL, rkipc_get_nn_update_osd, NULL);
	LOG_DEBUG("end\n");

	return ret;
}

int rkipc_osd_draw_nn_deinit() {
	LOG_DEBUG("%s\n", __func__);
	int ret = 0;
	pthread_join(get_nn_update_osd_thread_id, NULL);
	// Detach osd from chn
	MPP_CHN_S stMppChn;
	RGN_HANDLE RgnHandle = DRAW_NN_OSD_ID;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = DRAW_NN_VENC_CHN_ID;
	ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
	if (RK_SUCCESS != ret)
		LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);

	// destory region
	ret = RK_MPI_RGN_Destroy(RgnHandle);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Destroy [%d] failed with %#x\n", RgnHandle, ret);
	}
	LOG_DEBUG("Destory handle:%d success\n", RgnHandle);

	return ret;
}
#endif

int rkipc_osd_draw_nn_change() {
	LOG_INFO("%s\n", __func__);
	int ret = 0;
	int rotation = rk_param_get_int("video.source:rotation", 0);
	MPP_CHN_S stMppChn;
	RGN_ATTR_S stRgnAttr;
	RGN_CHN_ATTR_S stRgnChnAttr;
	RGN_HANDLE RgnHandle = DRAW_NN_OSD_ID;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = DRAW_NN_VENC_CHN_ID;
	ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
	if (RK_SUCCESS != ret)
		LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
	ret = RK_MPI_RGN_GetAttr(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_GetAttr (%d) failed with %#x!", RgnHandle, ret);
		return RK_FAILURE;
	}
	if (rotation == 90 || rotation == 270) {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.0:height", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.0:width", -1);
	} else {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.0:width", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.0:height", -1);
	}
	stRgnAttr.unAttr.stOverlay.u32ClutNum = CLUT_TABLE_8BPP_NUM;
	memcpy(stRgnAttr.unAttr.stOverlay.u32Clut, clut_table_8bpp,
	       sizeof(stRgnAttr.unAttr.stOverlay.u32Clut[0]) * CLUT_TABLE_8BPP_NUM);
	ret = RK_MPI_RGN_SetAttr(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_SetAttr (%d) failed with %#x!", RgnHandle, ret);
		return RK_FAILURE;
	}

	memset(&stRgnChnAttr, 0, sizeof(stRgnChnAttr));
	stRgnChnAttr.bShow = RK_TRUE;
	stRgnChnAttr.enType = OVERLAY_RGN;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = 0;
	stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = 0;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 128;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 128;
	// 1126 osd alpha not support multi layer, so use layer 0 to draw nn
	stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = 0;

	ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", RgnHandle, ret);
		return RK_FAILURE;
	}

	return ret;
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
	// 1126 osd alpha not support multi layer, so use layer 6 and 7 to draw cover
	stCoverChnAttr.unChnAttr.stCoverChn.u32Layer = id + 2; // 4 5 → 6 7
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
	stRgnAttr.unAttr.stOverlay.u32ClutNum = CLUT_TABLE_8BPP_NUM;
	memcpy(stRgnAttr.unAttr.stOverlay.u32Clut, clut_table_8bpp,
	       sizeof(stRgnAttr.unAttr.stOverlay.u32Clut[0]) * CLUT_TABLE_8BPP_NUM);
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
	// 1126 osd alpha not support multi layer, so use layer 0 to draw nn
	stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = id + 1; // 0 1 2 3 6 → 1 2 3 4 5
	if (id == 6)
		stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = 5; // make sure cover layer is higher
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
	if (get_jpeg_cnt) {
		LOG_WARN("the last photo was not completed\n");
		return -1;
	}
	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = 1;
	RK_MPI_VENC_StartRecvFrame(JPEG_VENC_CHN, &stRecvParam);
	get_jpeg_cnt++;

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
	snprintf(entry, 127, "video.source:rotation");
	*value = rk_param_get_int(entry, 0);

	return 0;
}

int rk_video_set_rotation(int value) {
	LOG_INFO("value is %d\n", value);
	int rotation = 0;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.source:rotation");
	rk_param_set_int(entry, value);

	return 0;
}

int rk_video_init() {
	LOG_INFO("begin\n");
	int ret = 0;
	enable_ivs = rk_param_get_int("video.source:enable_ivs", 1);
	enable_jpeg = rk_param_get_int("video.source:enable_jpeg", 1);
	enable_venc_0 = rk_param_get_int("video.source:enable_venc_0", 1);
	enable_venc_1 = rk_param_get_int("video.source:enable_venc_1", 1);
	enable_venc_2 = rk_param_get_int("video.source:enable_venc_2", 1);
	LOG_INFO("enable_jpeg is %d, enable_venc_0 is %d, enable_venc_1 is %d, enable_venc_2 is %d\n",
	         enable_jpeg, enable_venc_0, enable_venc_1, enable_venc_2);

	g_enable_vo = rk_param_get_int("video.source:enable_vo", 1);
	g_vo_dev_id = rk_param_get_int("video.source:vo_dev_id", 3);
	enable_npu = rk_param_get_int("video.source:enable_npu", 0);
	LOG_INFO("g_enable_vo is %d, g_vo_dev_id is %d, enable_npu is %d\n", g_enable_vo, g_vo_dev_id,
	         enable_npu);
	g_video_run_ = 1;
	g_nn_osd_run_ = 1;
	ret |= rkipc_vi_dev_init();
	ret |= rkipc_rtsp_init(RTSP_URL_0, RTSP_URL_1, RTSP_URL_2);
	ret |= rkipc_rtmp_init();

	if (enable_venc_0 || enable_jpeg)
		ret |= rkipc_pipe_0_init();
	if (enable_venc_1 || enable_venc_2)
		ret |= rkipc_pipe_1_init();
	if (g_enable_vo || enable_npu || enable_ivs)
		ret |= rkipc_pipe_2_init();

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
	g_nn_osd_run_ = 0;
	int ret = 0;
	rk_region_clip_set_callback_register(NULL);
	rk_roi_set_callback_register(NULL);
	ret |= rkipc_osd_deinit();
	if (g_enable_vo)
		ret |= rkipc_pipe_2_deinit();
	if (enable_venc_0) {
		ret |= rkipc_pipe_0_deinit();
	}
	if (enable_venc_1 || enable_venc_2) {

		ret |= rkipc_pipe_1_deinit();
	}
	ret |= rkipc_vi_dev_deinit();
	ret |= rkipc_rtmp_deinit();
	ret |= rkipc_rtsp_deinit();

	return ret;
}

extern char *rkipc_iq_file_path_;
int rk_video_restart() {
	int ret;
	ret = rk_storage_deinit();
	ret = rk_video_deinit();
	rk_isp_deinit(0);
	usleep(100 * 1000);
	rk_isp_init(0, rkipc_iq_file_path_);
	ret |= rk_video_init();
	ret |= rk_storage_init();

	return ret;
}
