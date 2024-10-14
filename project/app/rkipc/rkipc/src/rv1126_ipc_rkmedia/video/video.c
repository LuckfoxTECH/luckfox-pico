// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "video.h"
#include "common.h"
// #include "event.h"
#include "isp.h"
#include "osd.h"
#include "rkmuxer.h"
#include "roi.h"
#include "rtmp.h"
#include "rtsp_demo.h"

#include "rkmedia_api.h"
#include "rkmedia_common.h"
#include "rkmedia_venc.h"
#include "rkmedia_vi.h"
#include "rkmedia_vo.h"
#include "storage.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "video.c"

#define VIDEO_PIPE_0 0
#define VIDEO_PIPE_1 1
// #define VIDEO_PIPE_2 2
#define JPEG_CHN_VENC 2

#define RV1126_VO_DEV_MIPI 0
#define RV1126_VOP_LAYER_CLUSTER0 0
#define RV1126_ROI_REGION 2

#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"
#define RTSP_URL_2 "/live/2"

#define RTMP_URL_0 "rtmp://127.0.0.1:1935/live/mainstream"
#define RTMP_URL_1 "rtmp://127.0.0.1:1935/live/substream"
#define RTMP_URL_2 "rtmp://127.0.0.1:1935/live/thirdstream"

#define TEST_ARGB32_YELLOW 0xFFFFFF00
#define TEST_ARGB32_RED 0xFFFF0033
#define TEST_ARGB32_BLUE 0xFF003399
#define TEST_ARGB32_TRANS 0x00000000

static pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
static int g_video_run_ = 1;
static int pipe_id_ = 0;
static int dev_id_ = 0;
static int g_rtmp_start = 0;
static int g_roi_region_cnt = 0;
static VENC_ROI_ATTR_S *g_pst_roi_attr = NULL;
static rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session_0;
static rtsp_session_handle g_rtsp_session_1;
static rtsp_session_handle g_rtsp_session_2;
static const char *g_output_data_type;
static const char *g_rc_mode;
static const char *g_h264_profile;
static const char *g_smart;
static const char *g_gop_mode;
static pthread_t venc_thread_id_0;
static pthread_t venc_thread_id_1;
static pthread_t venc_thread_id_2;
static pthread_t venc_thread_JPEG;
static void *g_rtmp_handle_0;
static void *g_rtmp_handle_1;
static void *g_rtmp_handle_2;

MPP_CHN_S pipe_0_vi_chn;
MPP_CHN_S pipe_0_venc_chn;

MPP_CHN_S pipe_1_vi_chn;
MPP_CHN_S pipe_1_venc_chn;

MPP_CHN_S pipe_2_vi_chn;
MPP_CHN_S pipe_2_venc_chn;

MPP_CHN_S pipe_3_vi_chn;
MPP_CHN_S pipe_3_venc_chn;
MPP_CHN_S pipe_3_vo_chn;
MPP_CHN_S pipe_3_rga_chn;

static void *rkipc_get_vi_0(void *arg) {
	LOG_INFO("#Start %s thread, arg:%p\n", __func__, arg);
	int loopCount = 0;
	int ret = 0;

	while (g_video_run_) {
		// 5.get the frame
		// ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_0, &vi_frame, -1);
		MEDIA_BUFFER mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VI, VIDEO_PIPE_0, -1);
		if (!mb) {
			LOG_ERROR("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
			continue;
		} else {
			// LOG_DEBUG("RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld
			// ms\n", data,
			//           loopCount, vi_frame.s32Seq, vi_frame.s64PTS / 1000);
			// // 6.get the channel status
			// ret = RK_MPI_VI_QueryChnStatus(pipe_id_, VIDEO_PIPE_0, &stChnStatus);
			// LOG_DEBUG("RK_MPI_VI_QueryChnStatus ret %x, "
			//           "w:%d,h:%d,enable:%d,lost:%d,framerate:%d,vbfail:%d\n",
			//           ret, stChnStatus.stSize.u32Width,
			//           stChnStatus.stSize.u32Height, stChnStatus.bEnable,
			//           stChnStatus.u32LostFrame, stChnStatus.u32FrameRate,
			//           stChnStatus.u32VbFail);

			// event_process(RK_MPI_MB_Handle2Fd(vi_frame.pMbBlk), data,
			//               rk_param_get_int("video.0:width", 0),
			//               rk_param_get_int("video.0:height", 0), vi_frame.u32Len,
			//               vi_frame.s64PTS);

			ret = RK_MPI_SYS_SendMediaBuffer(RK_ID_VENC, 0, mb);
			// ret = RK_MPI_VENC_SendFrame(0, &venc_nv12_frame, -1);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VENC_SendFrame fail %x\n", ret);
			}
			// 7.release the frame
			ret = RK_MPI_MB_ReleaseBuffer(mb);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
			}
			loopCount++;
		}
	}

	return 0;
}

static void *rkipc_get_venc_0(void *arg) {
	LOG_INFO("#Start %s thread, arg:%p\n", __func__, arg);
	int loopCount = 0;
	int ret = 0;
	MEDIA_BUFFER mb = NULL;

	while (g_video_run_) {
		// 5.get the frame
		// ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_0, &stFrame, -1);
		mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VENC, 0, 2500);
		if (!mb) {
			LOG_ERROR("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
			continue;
		} else {
			if (g_rtsplive && g_rtsp_session_0) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session_0, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
				              RK_MPI_MB_GetTimestamp(mb));
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			}
			if (g_rtmp_start) {
				if (RK_MPI_MB_GetFlag(mb) == VENC_NALU_IDRSLICE ||
				    RK_MPI_MB_GetFlag(mb) == VENC_NALU_ISLICE) { // g_rtmp_handle_0 &&
					rk_storage_write_video_frame(0, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                             RK_MPI_MB_GetTimestamp(mb), 1);
					rk_rtmp_write_video_frame(0, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                          RK_MPI_MB_GetTimestamp(mb), 1);
				} else if (RK_MPI_MB_GetFlag(mb) == VENC_NALU_PSLICE) {
					rk_storage_write_video_frame(0, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                             RK_MPI_MB_GetTimestamp(mb), 0);
					rk_rtmp_write_video_frame(0, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                          RK_MPI_MB_GetTimestamp(mb), 0);
				}
			}
			// 7.release the frame
			ret = RK_MPI_MB_ReleaseBuffer(mb);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("ReleaseStream fail %x\n", ret);
			}
			loopCount++;
		}
	}

	return 0;
}

static void *rkipc_get_venc_1(void *arg) {
	LOG_INFO("#Start %s thread, arg:%p\n", __func__, arg);
	int loopCount = 0;
	int ret = 0;
	MEDIA_BUFFER mb = NULL;
	while (g_video_run_) {
		mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VENC, 1, -1);
		if (!mb) {
			LOG_ERROR("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
			continue;
		} else {
			if (g_rtsplive && g_rtsp_session_1) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session_1, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
				              RK_MPI_MB_GetTimestamp(mb));
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			}
			if (g_rtmp_start) {
				if (RK_MPI_MB_GetFlag(mb) == VENC_NALU_IDRSLICE ||
				    RK_MPI_MB_GetFlag(mb) == VENC_NALU_ISLICE) { // g_rtmp_handle_0 &&
					rk_storage_write_video_frame(1, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                             RK_MPI_MB_GetTimestamp(mb), 1);
					rk_rtmp_write_video_frame(1, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                          RK_MPI_MB_GetTimestamp(mb), 1);
				} else if (RK_MPI_MB_GetFlag(mb) == VENC_NALU_PSLICE) {
					rk_storage_write_video_frame(1, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                             RK_MPI_MB_GetTimestamp(mb), 0);
					rk_rtmp_write_video_frame(1, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                          RK_MPI_MB_GetTimestamp(mb), 0);
				}
			}
			ret = RK_MPI_MB_ReleaseBuffer(mb);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
			}
			loopCount++;
		}
	}
	return 0;
}

static void *rkipc_get_venc_2(void *arg) {
	LOG_INFO("#Start %s thread, arg:%p\n", __func__, arg);
	int loopCount = 0;
	int ret = 0;
	MEDIA_BUFFER mb = NULL;
	while (g_video_run_) {
		mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VENC, 2, -1);
		if (!mb) {
			LOG_ERROR("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
			continue;
		} else {
			if (g_rtsplive && g_rtsp_session_2) {
				pthread_mutex_lock(&g_rtsp_mutex);
				rtsp_tx_video(g_rtsp_session_2, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
				              RK_MPI_MB_GetTimestamp(mb));
				rtsp_do_event(g_rtsplive);
				pthread_mutex_unlock(&g_rtsp_mutex);
			}
			if (g_rtmp_start) {
				if (RK_MPI_MB_GetFlag(mb) == VENC_NALU_IDRSLICE ||
				    RK_MPI_MB_GetFlag(mb) == VENC_NALU_ISLICE) { // g_rtmp_handle_0 &&
					rk_storage_write_video_frame(2, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                             RK_MPI_MB_GetTimestamp(mb), 1);
					rk_rtmp_write_video_frame(2, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                          RK_MPI_MB_GetTimestamp(mb), 1);
				} else if (RK_MPI_MB_GetFlag(mb) == VENC_NALU_PSLICE) {
					rk_storage_write_video_frame(2, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                             RK_MPI_MB_GetTimestamp(mb), 0);
					rk_rtmp_write_video_frame(2, RK_MPI_MB_GetPtr(mb), RK_MPI_MB_GetSize(mb),
					                          RK_MPI_MB_GetTimestamp(mb), 0);
				}
			}
			ret = RK_MPI_MB_ReleaseBuffer(mb);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VENC_ReleaseStream fail %x\n", ret);
			}
			loopCount++;
		}
	}
	return 0;
}

int rkipc_rtsp_init(int URL_option) {
	LOG_INFO("rtsp_init start\n");
	if (!g_rtsplive)
		g_rtsplive = create_rtsp_demo(554);
	if (URL_option == 0) {
		g_output_data_type = rk_param_get_string("video.0:output_data_type", NULL);
		g_rtsp_session_0 = rtsp_new_session(g_rtsplive, RTSP_URL_0);
		if (!strcmp(g_output_data_type, "H.264")) {
			rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		} else if (!strcmp(g_output_data_type, "H.265")) {
			rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		} else {
			LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
			return -1;
		}
		rtsp_sync_video_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());
	}
	if (URL_option == 1) {
		g_output_data_type = rk_param_get_string("video.1:output_data_type", NULL);
		g_rtsp_session_1 = rtsp_new_session(g_rtsplive, RTSP_URL_1);
		if (!strcmp(g_output_data_type, "H.264")) {
			rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		} else if (!strcmp(g_output_data_type, "H.265")) {
			rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		} else {
			LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
			return -1;
		}
		rtsp_sync_video_ts(g_rtsp_session_1, rtsp_get_reltime(), rtsp_get_ntptime());
	}
	if (URL_option == 2) {
		g_output_data_type = rk_param_get_string("video.2:output_data_type", NULL);
		g_rtsp_session_2 = rtsp_new_session(g_rtsplive, RTSP_URL_2);
		if (!strcmp(g_output_data_type, "H.264")) {
			rtsp_set_video(g_rtsp_session_2, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		} else if (!strcmp(g_output_data_type, "H.265")) {
			rtsp_set_video(g_rtsp_session_2, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		} else {
			LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
			return -1;
		}
		rtsp_sync_video_ts(g_rtsp_session_2, rtsp_get_reltime(), rtsp_get_ntptime());
	}

	LOG_INFO("end\n");

	return 0;
}

int rkipc_rtsp_deinit() {
	LOG_INFO("%s\n", __func__);
	if (g_rtsplive) {
		rtsp_del_demo(g_rtsplive);
		g_rtsplive = NULL;
	}

	LOG_INFO("%s over\n", __func__);

	return 0;
}

static VideoParam g_video_param;
static AudioParam g_audio_param;

int rkipc_rtmp_init(int id, const char *rtmp_url) {
	int ret = rk_rtmp_init(id, rtmp_url);
	g_rtmp_start = 1;

	return ret;
}

int rkipc_rtmp_deinit(int id) {
	LOG_INFO("begin\n");
	rkmuxer_deinit(id + 3);
	g_rtmp_start = -1;
	LOG_INFO("end\n");

	return 0;
}

static void *rkipc_VENC2_get_jpeg(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	MEDIA_BUFFER mb = NULL;
	int loopCount = 0;
	int ret = 0;
	char file_name[128] = {0};
	const char *file_path = rk_param_get_string("storage:file_path", "/userdata");

	while (g_video_run_) {
		// 5.get the frame
		mb = RK_MPI_SYS_GetMediaBuffer(RK_ID_VENC, JPEG_CHN_VENC, -1);
		if (!mb) {
			LOG_ERROR("RK_MPI_SYS_GetMediaBuffer get null buffer!\n");
			continue;
		} else {
			LOG_INFO("Count:%d, Len:%d, PTS is %lld\n", loopCount, RK_MPI_MB_GetSize(mb),
			         RK_MPI_MB_GetTimestamp(mb));
			// save jpeg file
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			snprintf(file_name, 128, "%s/%d%02d%02d%02d%02d%02d.jpeg", file_path, tm.tm_year + 1900,
			         tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
			LOG_INFO("file_name is %s\n", file_name);
			FILE *fp = fopen(file_name, "wb");
			fwrite(RK_MPI_MB_GetPtr(mb), 1, RK_MPI_MB_GetSize(mb), fp);
			fflush(fp);
			fclose(fp);
			// 7.release the frame
			ret = RK_MPI_MB_ReleaseBuffer(mb);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("ReleaseStream fail %x\n", ret);
			}
			loopCount++;
		}
	}
	return 0;
}

int rkipc_pipe_0_to_jpeg_init() { // VI 0 2688*1520 ->VENC 3 JPEG ->save jpeg
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	const char *video_device_name = rk_param_get_string("video.0:src_node", "rkispp_scale0");
	int buf_cnt = 2;
	int ret = 0;
	RK_U32 u32Rotation = 0;

	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_JPEG;
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
	ret = RK_MPI_VENC_CreateChn(JPEG_CHN_VENC, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC[2] error! ret=%d\n", ret);
		return 0;
	}

	pipe_0_venc_chn.enModId = RK_ID_VENC;
	pipe_0_venc_chn.s32DevId = pipe_id_;
	pipe_0_venc_chn.s32ChnId = JPEG_CHN_VENC;

	pipe_0_vi_chn.enModId = RK_ID_VI;
	pipe_0_vi_chn.s32DevId = pipe_id_;
	pipe_0_vi_chn.s32ChnId = VIDEO_PIPE_0;
	ret = RK_MPI_SYS_Bind(&pipe_0_vi_chn, &pipe_0_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: Bind VI[0] and VENC[2] error! ret=%d\n", ret);
		return -1;
	}
	VENC_JPEG_PARAM_S stJpegParam;
	memset(&stJpegParam, 0, sizeof(VENC_JPEG_PARAM_S));
	stJpegParam.u32Qfactor = 95;
	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = 1;
	RK_MPI_VENC_StartRecvFrame(JPEG_CHN_VENC, &stRecvParam);
	pthread_create(&venc_thread_JPEG, NULL, rkipc_VENC2_get_jpeg, NULL);
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
	RK_MPI_VENC_StartRecvFrame(JPEG_CHN_VENC, &stRecvParam);

	return 0;
}

int rkipc_pipe_0_to_jpeg_deinit() {
	int ret = 0;
	// unbind first
	pipe_0_venc_chn.s32ChnId = JPEG_CHN_VENC;
	ret = RK_MPI_SYS_UnBind(&pipe_0_vi_chn, &pipe_0_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: UnBind VI and VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("UnBind VI and VENC success\n");
	// destroy venc before vi
	// ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(2);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VENC success\n");
	pthread_join(venc_thread_JPEG, NULL);
	return 0;
}

int rkipc_pipe_0_to_RTSP_init() { // VI 0 2688*1520 -> VENC 0 H264 ->RTSP 0 &
	                              // RTMP
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	const char *video_device_name = rk_param_get_string("video.0:src_node", "rkispp_scale0");
	g_output_data_type = rk_param_get_string("video.0:output_data_type", NULL);
	g_rc_mode = rk_param_get_string("video.0:rc_mode", NULL);
	g_h264_profile = rk_param_get_string("video.0:h264_profile", NULL);
	if ((g_output_data_type == NULL) || (g_rc_mode == NULL)) {
		LOG_ERROR("g_output_data_type or g_rc_mode is NULL\n");
		return -1;
	}
	int buf_cnt = 2;
	int ret = 0;
	rkipc_rtsp_init(0);
	rkipc_rtmp_init(0, RTMP_URL_0);

	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	LOG_INFO("video_device_name = %s\n", video_device_name);
	vi_chn_attr.pcVideoNode = video_device_name;
	vi_chn_attr.u32BufCnt = buf_cnt;
	vi_chn_attr.u32Width = video_width;
	vi_chn_attr.u32Height = video_height;
	vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12; // IMAGE_TYPE_YUV420P;RK_FMT_YUV420SP;
	vi_chn_attr.enBufType = VI_CHN_BUF_TYPE_MMAP;
	vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_0, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_0);
	if (ret) {
		LOG_ERROR("Create VI[0] failed! ret=%d\n", ret);
		return -1;
	}
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, rkipc_get_vi_0, NULL); // get vi 0
	// VENC init
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	LOG_INFO("g_output_data_type is %s, g_rc_mode is %s, g_h264_profile is %s\n",
	         g_output_data_type, g_rc_mode, g_h264_profile);
	if (!strcmp(g_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264; // RK_VIDEO_ID_AVC;

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
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate =
			    rk_param_get_int("video.0:max_rate", -1) * 1000;
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
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate =
			    rk_param_get_int("video.0:max_rate", -1) * 1000;
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
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H265; // RK_VIDEO_ID_HEVC;
		if (!strcmp(g_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate =
			    rk_param_get_int("video.0:max_rate", -1) * 1000;
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
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate =
			    rk_param_get_int("video.0:max_rate", -1) * 1000;
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
	g_gop_mode = rk_param_get_string("video.0:gop_mode", NULL);
	memset(&(venc_chn_attr.stGopAttr), 0, sizeof(venc_chn_attr.stGopAttr));
	if (!strcmp(g_gop_mode, "normalP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	} else if (!strcmp(g_gop_mode, "smartP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		// venc_chn_attr.stGopAttr.s32IPQpDelta = 3;
		// venc_chn_attr.stGopAttr.s32ViQpDelta = 3;
		venc_chn_attr.stGopAttr.u32BgInterval = 300;
		venc_chn_attr.stGopAttr.u32GopSize = 25;
	}
	venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	// venc_chn_attr.stVencAttr.u32Profile = 77;
	ret = RK_MPI_VENC_CreateChn(0, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC[0] error! ret=%d\n", ret);
		return 0;
	}
	pipe_0_venc_chn.enModId = RK_ID_VENC;
	pipe_0_venc_chn.s32DevId = pipe_id_;
	pipe_0_venc_chn.s32ChnId = VIDEO_PIPE_0;

	pipe_0_vi_chn.enModId = RK_ID_VI;
	pipe_0_vi_chn.s32DevId = pipe_id_;
	pipe_0_vi_chn.s32ChnId = VIDEO_PIPE_0;
	ret = RK_MPI_SYS_Bind(&pipe_0_vi_chn, &pipe_0_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: Bind VI[0] and VENC[0] error! ret=%d\n", ret);
		return -1;
	}

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	// RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_0, &stRecvParam);
	pthread_create(&venc_thread_id_0, NULL, rkipc_get_venc_0, NULL);

	return 0;
}

int rkipc_pipe_0_to_RTSP_deinit() {
	int ret = 0;
	// unbind first
	pipe_0_venc_chn.s32ChnId = VIDEO_PIPE_0;
	ret = RK_MPI_SYS_UnBind(&pipe_0_vi_chn, &pipe_0_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: UnBind VI and VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("UnBind VI and VENC success\n");
	// destroy venc before vi
	// ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(0);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VENC[0] error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VENC[0] success\n");
	// destroy vi
	ret = RK_MPI_VI_DisableChn(pipe_id_, VIDEO_PIPE_0);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VI error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VI success\n");
	rkipc_rtmp_deinit(0);
	rkipc_rtsp_deinit();
	pthread_join(venc_thread_id_0, NULL);
	return 0;
}

int rkipc_pipe_1_init() { // VI 1 640*480 -> VENC 1 H264 ->RTSP 1 & RTMP
	int video_width = rk_param_get_int("video.1:width", -1);
	int video_height = rk_param_get_int("video.1:height", -1);
	const char *video_device_name = rk_param_get_string("video.1:src_node", "rkispp_scale1");
	g_output_data_type = rk_param_get_string("video.1:output_data_type", NULL);
	g_rc_mode = rk_param_get_string("video.1:rc_mode", NULL);
	g_h264_profile = rk_param_get_string("video.1:h264_profile", NULL);
	if ((g_output_data_type == NULL) || (g_rc_mode == NULL)) {
		LOG_ERROR("g_output_data_type or g_rc_mode is NULL\n");
		return -1;
	}
	int buf_cnt = 2;
	int ret = 0;
	rkipc_rtsp_init(1);
	rkipc_rtmp_init(1, RTMP_URL_1);

	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	LOG_INFO("video_device_name = %s\n", video_device_name);
	vi_chn_attr.pcVideoNode = video_device_name;
	vi_chn_attr.u32BufCnt = buf_cnt;
	vi_chn_attr.u32Width = video_width;
	vi_chn_attr.u32Height = video_height;
	vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12; // IMAGE_TYPE_YUV420P;RK_FMT_YUV420SP;
	vi_chn_attr.enBufType = VI_CHN_BUF_TYPE_MMAP;
	vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_1, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_1);
	if (ret) {
		LOG_ERROR("Create VI[1] failed! ret=%d\n", ret);
		return -1;
	}
	// VENC init
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	LOG_INFO("g_output_data_type is %s, g_rc_mode is %s, g_h264_profile is %s\n",
	         g_output_data_type, g_rc_mode, g_h264_profile);
	if (!strcmp(g_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264; // RK_VIDEO_ID_AVC;

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
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int("video.1:gop", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate =
			    rk_param_get_int("video.1:max_rate", -1) * 1000;
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
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate =
			    rk_param_get_int("video.1:max_rate", -1) * 1000;
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.1:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.1:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.1:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.1:src_frame_rate_num", -1);
		}
	} else if (!strcmp(g_output_data_type, "H.265")) {
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H265; // RK_VIDEO_ID_HEVC;
		if (!strcmp(g_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int("video.1:gop", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate =
			    rk_param_get_int("video.1:max_rate", -1) * 1000;
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
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate =
			    rk_param_get_int("video.1:max_rate", -1) * 1000;
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
		LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
		return -1;
	}
	g_smart = rk_param_get_string("video.1:smart", NULL);
	g_gop_mode = rk_param_get_string("video.1:gop_mode", NULL);
	memset(&(venc_chn_attr.stGopAttr), 0, sizeof(venc_chn_attr.stGopAttr));
	if (!strcmp(g_gop_mode, "normalP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	} else if (!strcmp(g_gop_mode, "smartP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		// venc_chn_attr.stGopAttr.s32IPQpDelta = 3;
		// venc_chn_attr.stGopAttr.s32ViQpDelta = 3;
		venc_chn_attr.stGopAttr.u32BgInterval = 300;
		venc_chn_attr.stGopAttr.u32GopSize = 25;
	}
	venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	// venc_chn_attr.stVencAttr.u32Profile = 77;
	ret = RK_MPI_VENC_CreateChn(1, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC[1] error! ret=%d\n", ret);
		return 0;
	}
	pipe_1_venc_chn.enModId = RK_ID_VENC;
	pipe_1_venc_chn.s32DevId = pipe_id_;
	pipe_1_venc_chn.s32ChnId = VIDEO_PIPE_1;

	pipe_1_vi_chn.enModId = RK_ID_VI;
	pipe_1_vi_chn.s32DevId = pipe_id_;
	pipe_1_vi_chn.s32ChnId = VIDEO_PIPE_1;
	ret = RK_MPI_SYS_Bind(&pipe_1_vi_chn, &pipe_1_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: Bind VI[1] and VENC[1] error! ret=%d\n", ret);
		return -1;
	}

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	// RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_0, &stRecvParam);
	pthread_create(&venc_thread_id_1, NULL, rkipc_get_venc_1, NULL);

	return 0;
}

int rkipc_pipe_1_deinit() {
	int ret = 0;
	// unbind first
	ret = RK_MPI_SYS_UnBind(&pipe_1_vi_chn, &pipe_1_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: UnBind VI and VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("UnBind VI and VENC success\n");
	// destroy venc before vi
	// ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(1);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VENC success\n");
	// destroy vi
	ret = RK_MPI_VI_DisableChn(pipe_id_, VIDEO_PIPE_1);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VI error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VI success\n");
	rkipc_rtmp_deinit(1);
	rkipc_rtsp_deinit();
	pthread_join(venc_thread_id_1, NULL);
	return 0;
}

#if 0
int rkipc_pipe_2_init() { // VI 2 1920*1080 -> VENC 2 H264 ->RTSP 2  & RTMP
	int video_width = rk_param_get_int("video.2:width", -1);
	int video_height = rk_param_get_int("video.2:height", -1);
	const char *video_device_name = rk_param_get_string("video.2:src_node", "rkispp_scale2");
	g_output_data_type = rk_param_get_string("video.2:output_data_type", NULL);
	g_rc_mode = rk_param_get_string("video.2:rc_mode", NULL);
	g_h264_profile = rk_param_get_string("video.2:h264_profile", NULL);
	if ((g_output_data_type == NULL) || (g_rc_mode == NULL)) {
		LOG_ERROR("g_output_data_type or g_rc_mode is NULL\n");
		return -1;
	}
	int buf_cnt = 2;
	int ret = 0;
	rkipc_rtsp_init(2);
	rkipc_rtmp_init(2, RTMP_URL_2);

	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	LOG_INFO("video_device_name = %s\n", video_device_name);
	vi_chn_attr.pcVideoNode = video_device_name;
	vi_chn_attr.u32BufCnt = buf_cnt;
	vi_chn_attr.u32Width = video_width;
	vi_chn_attr.u32Height = video_height;
	vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12; // IMAGE_TYPE_YUV420P;RK_FMT_YUV420SP;
	vi_chn_attr.enBufType = VI_CHN_BUF_TYPE_MMAP;
	vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_2, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_2);
	if (ret) {
		LOG_ERROR("Create VI[2] failed! ret=%d\n", ret);
		return -1;
	}
	// VENC init
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	LOG_INFO("g_output_data_type is %s, g_rc_mode is %s, g_h264_profile is %s\n",
	         g_output_data_type, g_rc_mode, g_h264_profile);
	if (!strcmp(g_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264; // RK_VIDEO_ID_AVC;

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
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int("video.2:gop", -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = rk_param_get_int("video.2:max_rate", -1) * 1000;
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
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate =
			    rk_param_get_int("video.2:max_rate", -1) * 1000;
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen =
			    rk_param_get_int("video.2:dst_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum =
			    rk_param_get_int("video.2:dst_frame_rate_num", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen =
			    rk_param_get_int("video.2:src_frame_rate_den", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum =
			    rk_param_get_int("video.2:src_frame_rate_num", -1);
		}
	} else if (!strcmp(g_output_data_type, "H.265")) {
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H265; // RK_VIDEO_ID_HEVC;
		if (!strcmp(g_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int("video.2:gop", -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = rk_param_get_int("video.2:max_rate", -1) * 1000;
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
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate =
			    rk_param_get_int("video.2:max_rate", -1) * 1000;
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
		LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
		return -1;
	}
	g_smart = rk_param_get_string("video.2:smart", NULL);
	g_gop_mode = rk_param_get_string("video.2:gop_mode", NULL);
	if (!strcmp(g_gop_mode, "normalP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	} else if (!strcmp(g_gop_mode, "smartP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
	}
	venc_chn_attr.stVencAttr.imageType = IMAGE_TYPE_NV12;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	// venc_chn_attr.stVencAttr.u32Profile = 77;
	ret = RK_MPI_VENC_CreateChn(2, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC[2] error! ret=%d\n", ret);
		return 0;
	}
	pipe_2_venc_chn.enModId = RK_ID_VENC;
	pipe_2_venc_chn.s32DevId = pipe_id_;
	pipe_2_venc_chn.s32ChnId = VIDEO_PIPE_2;

	pipe_2_vi_chn.enModId = RK_ID_VI;
	pipe_2_vi_chn.s32DevId = pipe_id_;
	pipe_2_vi_chn.s32ChnId = VIDEO_PIPE_2;
	ret = RK_MPI_SYS_Bind(&pipe_2_vi_chn, &pipe_2_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: Bind VI[2] and VENC[2] error! ret=%d\n", ret);
		return -1;
	}

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	// RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_0, &stRecvParam);
	pthread_create(&venc_thread_id_2, NULL, rkipc_get_venc_2, NULL);

	return 0;
}

int rkipc_pipe_2_deinit() {
	int ret = 0;
	// unbind first
	ret = RK_MPI_SYS_UnBind(&pipe_2_vi_chn, &pipe_2_venc_chn);
	if (ret) {
		LOG_ERROR("ERROR: UnBind VI and VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("UnBind VI and VENC success\n");
	// destroy venc before vi
	// ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(2);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VENC error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VENC success\n");
	// destroy vi
	ret = RK_MPI_VI_DisableChn(pipe_id_, VIDEO_PIPE_2);
	if (ret) {
		LOG_ERROR("ERROR: Destroy VI error! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("Destroy VI success\n");
	rkipc_rtmp_deinit(2);
	rkipc_rtsp_deinit();
	pthread_join(venc_thread_id_2, NULL);
	return 0;
}
#endif

int rkipc_pipe_3_init() { // VI 3 1280*720 ->RGA 720*1280 ->VO
	int video_width = 1280;
	int video_height = 720;
	int disp_width = 720;
	int disp_height = 1280;
	const char *video_device_name = "rkispp_scale2";
	int buf_cnt = 2;
	int ret = 0;
	RK_S32 s32CamId = 0;

	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	// VI init
	// 2.config channel
	LOG_INFO("video_device_name = %s\n", video_device_name);
	// strncpy(vi_chn_attr.pcVideoNode, video_device_name,
	// strlen(video_device_name));
	vi_chn_attr.pcVideoNode = video_device_name;
	vi_chn_attr.u32BufCnt = buf_cnt;
	// vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.u32Width = video_width;
	vi_chn_attr.u32Height = video_height;
	vi_chn_attr.enPixFmt = IMAGE_TYPE_NV12; // IMAGE_TYPE_YUV420P;RK_FMT_YUV420SP;
	vi_chn_attr.enBufType = VI_CHN_BUF_TYPE_MMAP;
	vi_chn_attr.enWorkMode = VI_WORK_MODE_NORMAL;
	ret = RK_MPI_VI_SetChnAttr(s32CamId, 3, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(s32CamId, 3);
	if (ret) {
		printf("Create VI[3] failed! ret=%d\n", ret);
		return -1;
	}

	// pthread_t thread_id;
	// pthread_create(&thread_id, NULL, test_get_vi, NULL);

	RGA_ATTR_S stRgaAttr;
	memset(&stRgaAttr, 0, sizeof(stRgaAttr));
	stRgaAttr.bEnBufPool = RK_TRUE;
	stRgaAttr.u16BufPoolCnt = 3;
	stRgaAttr.u16Rotaion = 90;
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
		printf("Create rga[0] falied! ret=%d\n", ret);
		return -1;
	}

	VO_CHN_ATTR_S VoChnAttr = {0};
	VoChnAttr.pcDevNode = "/dev/dri/card0";
	VoChnAttr.emPlaneType = VO_PLANE_PRIMARY;
	VoChnAttr.enImgType = IMAGE_TYPE_RGB888;
	VoChnAttr.u16Zpos = 0;
	VoChnAttr.stDispRect.s32X = 0;
	VoChnAttr.stDispRect.s32Y = 0;
	VoChnAttr.stDispRect.u32Width = disp_width;
	VoChnAttr.stDispRect.u32Height = disp_height;
	ret = RK_MPI_VO_CreateChn(0, &VoChnAttr);
	if (ret) {
		LOG_ERROR("Create VO[0] failed! ret=%d\n", ret);
		return -1;
	}

	pipe_3_vi_chn.enModId = RK_ID_VI;
	pipe_3_vi_chn.s32DevId = 0;
	pipe_3_vi_chn.s32ChnId = 3;
	pipe_3_rga_chn.enModId = RK_ID_RGA;
	pipe_3_rga_chn.s32DevId = 0;
	pipe_3_rga_chn.s32ChnId = 0;
	ret = RK_MPI_SYS_Bind(&pipe_3_vi_chn, &pipe_3_rga_chn);
	if (ret) {
		LOG_ERROR("Bind vi[3] to rga[0] failed! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("vi and rga bind success!\n");

	pipe_3_rga_chn.enModId = RK_ID_RGA;
	pipe_3_rga_chn.s32DevId = 0;
	pipe_3_rga_chn.s32ChnId = 0;
	pipe_3_vo_chn.enModId = RK_ID_VO;
	pipe_3_vo_chn.s32DevId = RV1126_VOP_LAYER_CLUSTER0;
	pipe_3_vo_chn.s32ChnId = 0;
	ret = RK_MPI_SYS_Bind(&pipe_3_rga_chn, &pipe_3_vo_chn);
	if (ret) {
		LOG_ERROR("Bind rga[0] to vo[0] failed! ret=%d\n", ret);
		return -1;
	}
	LOG_INFO("vi and vo bind success!\n");

	return 0;
}

int rkipc_pipe_3_deinit() {
	int ret;
	ret = RK_MPI_SYS_UnBind(&pipe_3_vi_chn, &pipe_3_rga_chn);
	if (ret) {
		LOG_ERROR("UnBind vi to rga failed! ret=%d\n", ret);
		return -1;
	}
	ret = RK_MPI_SYS_UnBind(&pipe_3_rga_chn, &pipe_3_vo_chn);
	if (ret) {
		LOG_ERROR("UnBind rga to vo failed! ret=%d\n", ret);
		return -1;
	}
	RK_MPI_VO_DestroyChn(0);
	RK_MPI_RGA_DestroyChn(0);
	RK_MPI_VI_DisableChn(0, 3);

	return 0;
}

int rk_video_get_gop(int stream_id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:gop", stream_id);
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_gop(int stream_id, int value) {
	LOG_INFO("-------stream_id:%d,value:%d\n", stream_id, value);
	int ret;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:gop_mode", stream_id);
	g_gop_mode = rk_param_get_string(entry, NULL);

	if (!strcmp(g_gop_mode, "normalP")) {
		ret = RK_MPI_VENC_SetGop(stream_id, value);
	} else if (!strcmp(g_gop_mode, "smartP")) {
		VENC_GOP_ATTR_S stGopModeAttr;
		stGopModeAttr.enGopMode = VENC_GOPMODE_SMARTP;
		stGopModeAttr.u32BgInterval = value;
		stGopModeAttr.u32GopSize = 25;
		ret = RK_MPI_VENC_SetGopMode(stream_id, &stGopModeAttr);
	} else {
		ret = -1;
	}

	if (ret) {
		LOG_ERROR("Set gop error! g_gop_mode is %s, ret is %d\n", g_gop_mode, ret);
		return -1;
	}
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

int rk_video_set_max_rate(int stream_id,
                          int value) { // change u32BitRate to u32MaxBitRate in H265/H264 VBR
	VENC_CHN_ATTR_S venc_chn_attr;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	g_output_data_type = rk_param_get_string(entry, NULL);
	snprintf(entry, 127, "video.%d:rc_mode", stream_id);
	g_rc_mode = rk_param_get_string(entry, "CBR");
	snprintf(entry, 127, "video.%d:max_rate", stream_id);
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	RK_MPI_VENC_GetVencChnAttr(stream_id, &venc_chn_attr);

	if (!strcmp(g_output_data_type, "H.264")) {
		if (!strcmp(g_rc_mode, "CBR"))
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = value * 1000;
		else
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate = value * 1000;
	} else if (!strcmp(g_output_data_type, "H.265")) {
		if (!strcmp(g_rc_mode, "CBR"))
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = value * 1000;
		else
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate = value * 1000;
	} else {
		LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetVencChnAttr(stream_id, &venc_chn_attr);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_video_get_RC_mode(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:rc_mode", stream_id);
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_video_set_RC_mode(int stream_id,
                         const char *value) { // change u32BitRate to u32MaxBitRate in H265/H264 VBR
	char entry[128] = {'\0'};
	char gop[128] = {'\0'};
	char max_rate[128] = {'\0'};
	char dst_frame_rate_den[128] = {'\0'};
	char dst_frame_rate_num[128] = {'\0'};
	char src_frame_rate_den[128] = {'\0'};
	char src_frame_rate_num[128] = {'\0'};
	snprintf(gop, 127, "video.%d:gop", stream_id);
	snprintf(max_rate, 127, "video.%d:max_rate", stream_id);
	snprintf(dst_frame_rate_den, 127, "video.%d:dst_frame_rate_den", stream_id);
	snprintf(dst_frame_rate_num, 127, "video.%d:dst_frame_rate_num", stream_id);
	snprintf(src_frame_rate_den, 127, "video.%d:src_frame_rate_den", stream_id);
	snprintf(src_frame_rate_num, 127, "video.%d:src_frame_rate_num", stream_id);
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);

	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	RK_MPI_VENC_GetVencChnAttr(stream_id, &venc_chn_attr);
	g_output_data_type = rk_param_get_string(entry, NULL);
	if (!strcmp(g_output_data_type, "H.264")) {
		if (!strcmp(value, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int(gop, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = rk_param_get_int(max_rate, -1) * 1000;
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen =
			    rk_param_get_int(dst_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum =
			    rk_param_get_int(dst_frame_rate_num, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen =
			    rk_param_get_int(src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum =
			    rk_param_get_int(src_frame_rate_num, -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
			venc_chn_attr.stRcAttr.stH264Vbr.u32Gop = rk_param_get_int(gop, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate = rk_param_get_int(max_rate, -1) * 1000;
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen =
			    rk_param_get_int(dst_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum =
			    rk_param_get_int(dst_frame_rate_num, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen =
			    rk_param_get_int(src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum =
			    rk_param_get_int(src_frame_rate_num, -1);
		}
	} else if (!strcmp(g_output_data_type, "H.265")) {
		if (!strcmp(value, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int(gop, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = rk_param_get_int(max_rate, -1) * 1000;
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen =
			    rk_param_get_int(dst_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum =
			    rk_param_get_int(dst_frame_rate_num, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateDen =
			    rk_param_get_int(src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateNum =
			    rk_param_get_int(src_frame_rate_num, -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
			venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = rk_param_get_int(gop, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate = rk_param_get_int(max_rate, -1) * 1000;
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen =
			    rk_param_get_int(dst_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum =
			    rk_param_get_int(dst_frame_rate_num, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen =
			    rk_param_get_int(src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum =
			    rk_param_get_int(src_frame_rate_num, -1);
		}
	} else {
		LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetVencChnAttr(stream_id, &venc_chn_attr);

	snprintf(entry, 127, "video.%d:rc_mode", stream_id);
	rk_param_set_string(entry, value);
	g_rc_mode = rk_param_get_string(entry, NULL); // update global variables
	LOG_INFO("g_rc_mode is %s\n", g_rc_mode);

	return 0;
}

int rk_video_get_output_data_type(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	*value = rk_param_get_string(entry, NULL);

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
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_video_set_rc_quality(int stream_id, const char *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:rc_quality", stream_id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_video_get_smart(int stream_id, const char **value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:smart", stream_id);
	*value = rk_param_get_string(entry, NULL);

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
	*value = rk_param_get_string(entry, NULL);

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
	*value = rk_param_get_string(entry, NULL);

	return 0;
}

int rk_video_set_h264_profile(int stream_id, const char *value) { // changed
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	g_output_data_type = rk_param_get_string(entry, NULL);
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	RK_MPI_VENC_GetVencChnAttr(stream_id, &venc_chn_attr);
	if (!strcmp(g_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264; // RK_VIDEO_ID_AVC;

		if (!strcmp(value, "high"))
			venc_chn_attr.stVencAttr.u32Profile = 100;
		else if (!strcmp(value, "main"))
			venc_chn_attr.stVencAttr.u32Profile = 77;
		else if (!strcmp(value, "baseline"))
			venc_chn_attr.stVencAttr.u32Profile = 66;
		else
			LOG_ERROR("value is %s\n", value);
		RK_MPI_VENC_SetVencChnAttr(stream_id, &venc_chn_attr);
	} else {
		LOG_INFO("g_output_data_type not H.264\n");
	}
	snprintf(entry, 127, "video.%d:h264_profile", stream_id);
	rk_param_set_string(entry, value);

	return 0;
}

int rk_video_get_resolution(int stream_id, char **value) {
	char entry0[128] = {'\0'};
	char entry1[128] = {'\0'};
	snprintf(entry0, 127, "video.%d:width", stream_id);
	snprintf(entry1, 127, "video.%d:height", stream_id);
	int width = rk_param_get_int(entry0, -1);
	int height = rk_param_get_int(entry1, -1);
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
	char entry0[128] = {'\0'};
	char entry1[128] = {'\0'};
	snprintf(entry0, 127, "video.%d:dst_frame_rate_den", stream_id);
	snprintf(entry1, 127, "video.%d:dst_frame_rate_num", stream_id);
	int den = rk_param_get_int(entry0, -1);
	int num = rk_param_get_int(entry1, -1);
	if (den == 1)
		sprintf(*value, "%d", num);
	else
		sprintf(*value, "%d/%d", num, den);

	return 0;
}

int rk_video_set_frame_rate(
    int stream_id,
    const char *value) { // change u32BitRate to u32MaxBitRate in H265/H264 VBR
	char entry[128] = {'\0'};
	char gop[128] = {'\0'};
	char max_rate[128] = {'\0'};
	char src_frame_rate_den[128] = {'\0'};
	char src_frame_rate_num[128] = {'\0'};
	snprintf(gop, 127, "video.%d:gop", stream_id);
	snprintf(max_rate, 127, "video.%d:max_rate", stream_id);
	snprintf(src_frame_rate_den, 127, "video.%d:src_frame_rate_den", stream_id);
	snprintf(src_frame_rate_num, 127, "video.%d:src_frame_rate_num", stream_id);
	snprintf(entry, 127, "video.%d:output_data_type", stream_id);
	g_output_data_type = rk_param_get_string(entry, NULL);
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
	RK_MPI_VENC_GetVencChnAttr(stream_id, &venc_chn_attr);
	if (!strcmp(g_output_data_type, "H.264")) {
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H264; // RK_VIDEO_ID_AVC;
		if (!strcmp(g_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
			venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int(gop, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = rk_param_get_int(max_rate, -1) * 1000;
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateDen = den;
			venc_chn_attr.stRcAttr.stH264Cbr.fr32DstFrameRateNum = num;
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateDen =
			    rk_param_get_int(src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Cbr.u32SrcFrameRateNum =
			    rk_param_get_int(src_frame_rate_num, -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
			venc_chn_attr.stRcAttr.stH264Vbr.u32Gop = rk_param_get_int(gop, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate = rk_param_get_int(max_rate, -1) * 1000;
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateDen = den;
			venc_chn_attr.stRcAttr.stH264Vbr.fr32DstFrameRateNum = num;
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateDen =
			    rk_param_get_int(src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32SrcFrameRateNum =
			    rk_param_get_int(src_frame_rate_num, -1);
		}
	} else if (!strcmp(g_output_data_type, "H.265")) {
		venc_chn_attr.stVencAttr.enType = RK_CODEC_TYPE_H265; // RK_VIDEO_ID_HEVC;
		if (!strcmp(g_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
			venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int(gop, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = rk_param_get_int(max_rate, -1) * 1000;
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateDen = den;
			venc_chn_attr.stRcAttr.stH265Cbr.fr32DstFrameRateNum = num;
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateDen =
			    rk_param_get_int(src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Cbr.u32SrcFrameRateNum =
			    rk_param_get_int(src_frame_rate_num, -1);
		} else {
			venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
			venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = rk_param_get_int(gop, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate = rk_param_get_int(max_rate, -1) * 1000;
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateDen = den;
			venc_chn_attr.stRcAttr.stH265Vbr.fr32DstFrameRateNum = num;
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen =
			    rk_param_get_int(src_frame_rate_den, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum =
			    rk_param_get_int(src_frame_rate_num, -1);
		}
	} else {
		LOG_ERROR("g_output_data_type is %s, not support\n", g_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetVencChnAttr(stream_id, &venc_chn_attr);

	snprintf(entry, 127, "video.%d:dst_frame_rate_den", stream_id);
	rk_param_set_int(entry, den);
	snprintf(entry, 127, "video.%d:dst_frame_rate_num", stream_id);
	rk_param_set_int(entry, num);

	return 0;
}

int rk_video_get_frame_rate_in(int stream_id, char **value) {
	char entry0[128] = {'\0'};
	char entry1[128] = {'\0'};
	snprintf(entry0, 127, "video.%d:src_frame_rate_den", stream_id);
	snprintf(entry1, 127, "video.%d:src_frame_rate_num", stream_id);
	int den = rk_param_get_int(entry0, -1);
	int num = rk_param_get_int(entry1, -1);
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

int rkipc_osd_cover_create(int id, osd_data_s *osd_data) {
	LOG_INFO("id is %d\n", id);
	int ret = 0;

	OSD_REGION_INFO_S pstRgnInfo;
	memset(&pstRgnInfo, 0, sizeof(pstRgnInfo));
	pstRgnInfo.u32PosX = osd_data->origin_x;
	pstRgnInfo.u32PosY = osd_data->origin_y;
	pstRgnInfo.u32Width = osd_data->width;
	pstRgnInfo.u32Height = osd_data->height;
	pstRgnInfo.enRegionId = id;

	if (osd_data->width && osd_data->height) {
		pstRgnInfo.u8Enable = osd_data->enable;
	} else {
		pstRgnInfo.u8Enable = 0;
	}

	COVER_INFO_S pstCoverInfo;
	pstCoverInfo.enPixelFormat = PIXEL_FORMAT_ARGB_8888; // RK_FMT_ARGB8888;  //for now ,not sure
	pstCoverInfo.u32Color = 0xffffff;

	LOG_DEBUG("PosX:%d, PosY:%d, Width:%d, Height:%d\n", pstRgnInfo.u32PosX, pstRgnInfo.u32PosY,
	          pstRgnInfo.u32Width, pstRgnInfo.u32Height);
	ret = RK_MPI_VENC_RGN_SetCover(VIDEO_PIPE_0, &pstRgnInfo, &pstCoverInfo);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("VIDEO_PIPE_0: RK_MPI_VENC_RGN_SetCover failed \n");
		return RK_FAILURE;
	}

	pstRgnInfo.u32PosX = UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.1:width", 1) /
	                                 rk_param_get_int("video.0:width", 1));
	pstRgnInfo.u32PosY = UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.1:height", 1) /
	                                 rk_param_get_int("video.0:height", 1));
	pstRgnInfo.u32Width = UPALIGNTO16(osd_data->width * rk_param_get_int("video.1:width", 1) /
	                                  rk_param_get_int("video.0:width", 1));
	pstRgnInfo.u32Height = UPALIGNTO16(osd_data->height * rk_param_get_int("video.1:height", 1) /
	                                   rk_param_get_int("video.0:height", 1));
	LOG_DEBUG("PosX:%d, PosY:%d, Width:%d, Height:%d\n", pstRgnInfo.u32PosX, pstRgnInfo.u32PosY,
	          pstRgnInfo.u32Width, pstRgnInfo.u32Height);
	ret = RK_MPI_VENC_RGN_SetCover(VIDEO_PIPE_1, &pstRgnInfo, &pstCoverInfo);
	// ret = RK_MPI_RGN_AttachToChn(coverHandle, &stCoverChn, &stCoverChnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("VIDEO_PIPE_1: RK_MPI_VENC_RGN_SetCover failed \n");
		return RK_FAILURE;
	}
	LOG_INFO("cover region to chn success\n");

	return ret;
}

int rkipc_osd_cover_destroy(int id) {
	LOG_INFO("id is %d\n", id);
	int ret = 0;

	OSD_REGION_INFO_S pstRgnInfo;
	memset(&pstRgnInfo, 0, sizeof(pstRgnInfo));
	pstRgnInfo.u8Enable = 0;
	pstRgnInfo.enRegionId = id;

	ret = RK_MPI_VENC_RGN_SetCover(VIDEO_PIPE_0, &pstRgnInfo, NULL);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("VIDEO_PIPE_0: RK_MPI_VENC_RGN_SetCover failed \n");
		return RK_FAILURE;
	}

	ret = RK_MPI_VENC_RGN_SetCover(VIDEO_PIPE_1, &pstRgnInfo, NULL);
	// ret = RK_MPI_RGN_AttachToChn(coverHandle, &stCoverChn, &stCoverChnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("VIDEO_PIPE_1: RK_MPI_VENC_RGN_SetCover failed \n");
		return RK_FAILURE;
	}
	LOG_INFO("destroy cover region to chn success\n");

	return ret;
}

int rkipc_osd_bmp_create(int id, osd_data_s *osd_data) {
	LOG_INFO("id is %d\n", id);
	int ret = 0;
	BITMAP_S stBitmap;

	OSD_REGION_INFO_S pstRgnInfo;
	memset(&pstRgnInfo, 0, sizeof(pstRgnInfo));
	pstRgnInfo.u32PosX = osd_data->origin_x;
	pstRgnInfo.u32PosY = osd_data->origin_y;
	pstRgnInfo.u8Enable = osd_data->enable;
	pstRgnInfo.u32Width = osd_data->width;
	pstRgnInfo.u32Height = osd_data->height;
	pstRgnInfo.enRegionId = id;

	// set bitmap
	stBitmap.enPixelFormat = PIXEL_FORMAT_ARGB_8888; // RK_FMT_ARGB8888;
	stBitmap.u32Width = osd_data->width;
	stBitmap.u32Height = osd_data->height;
	stBitmap.pData = (RK_VOID *)osd_data->buffer;
	LOG_DEBUG("PosX:%d, PosY:%d, Width:%d, Height:%d\n", pstRgnInfo.u32PosX, pstRgnInfo.u32PosY,
	          pstRgnInfo.u32Width, pstRgnInfo.u32Height);
	ret = RK_MPI_VENC_RGN_SetBitMap(VIDEO_PIPE_0, &pstRgnInfo, &stBitmap);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("VIDEO_PIPE_0: RK_MPI_VENC_RGN_SetBitMap failed with %d\n", ret);
		return RK_FAILURE;
	}

	pstRgnInfo.u32PosX = UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.1:width", 1) /
	                                 rk_param_get_int("video.0:width", 1));
	pstRgnInfo.u32PosY = UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.1:height", 1) /
	                                 rk_param_get_int("video.0:height", 1));
	LOG_DEBUG("PosX:%d, PosY:%d, Width:%d, Height:%d\n", pstRgnInfo.u32PosX, pstRgnInfo.u32PosY,
	          pstRgnInfo.u32Width, pstRgnInfo.u32Height);
	ret = RK_MPI_VENC_RGN_SetBitMap(VIDEO_PIPE_1, &pstRgnInfo, &stBitmap);
	// ret = RK_MPI_RGN_SetBitMap(RgnHandle, &stBitmap);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("VIDEO_PIPE_1: RK_MPI_VENC_RGN_SetBitMap failed with %d\n", ret);
		return RK_FAILURE;
	}

	return ret;
}

int rkipc_osd_bmp_destroy(int id) {
	LOG_INFO("id is %d\n", id);
	int ret = 0;

	OSD_REGION_INFO_S pstRgnInfo;
	memset(&pstRgnInfo, 0, sizeof(pstRgnInfo));
	pstRgnInfo.u8Enable = 0;
	pstRgnInfo.enRegionId = id;

	ret = RK_MPI_VENC_RGN_SetBitMap(VIDEO_PIPE_0, &pstRgnInfo, NULL);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("VIDEO_PIPE_0: RK_MPI_VENC_RGN_SetBitMap failed with %d\n", ret);
		return RK_FAILURE;
	}

	ret = RK_MPI_VENC_RGN_SetBitMap(VIDEO_PIPE_1, &pstRgnInfo, NULL);
	// ret = RK_MPI_RGN_SetBitMap(RgnHandle, &stBitmap);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("VIDEO_PIPE_1: RK_MPI_VENC_RGN_SetBitMap failed with %d\n", ret);
		return RK_FAILURE;
	}
	LOG_INFO("destroy bmp success\n");

	return ret;
}

int rkipc_osd_bmp_change(int id, osd_data_s *osd_data) {
	// LOG_INFO("id is %d\n", id);
	int ret = 0;
	BITMAP_S stBitmap;

	OSD_REGION_INFO_S pstRgnInfo;
	memset(&pstRgnInfo, 0, sizeof(pstRgnInfo));
	pstRgnInfo.u32PosX = osd_data->origin_x;
	pstRgnInfo.u32PosY = osd_data->origin_y;
	pstRgnInfo.u8Enable = osd_data->enable;
	pstRgnInfo.u32Width = osd_data->width;
	pstRgnInfo.u32Height = osd_data->height;
	pstRgnInfo.enRegionId = id;

	// set bitmap
	stBitmap.enPixelFormat = PIXEL_FORMAT_ARGB_8888; // RK_FMT_ARGB8888;
	stBitmap.u32Width = osd_data->width;
	stBitmap.u32Height = osd_data->height;
	stBitmap.pData = (RK_VOID *)osd_data->buffer;
	ret = RK_MPI_VENC_RGN_SetBitMap(VIDEO_PIPE_0, &pstRgnInfo, &stBitmap);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("VIDEO_PIPE_0: RK_MPI_VENC_RGN_SetBitMap failed with %d\n", ret);
		return RK_FAILURE;
	}

	pstRgnInfo.u32PosX = UPALIGNTO16(osd_data->origin_x * rk_param_get_int("video.1:width", 1) /
	                                 rk_param_get_int("video.0:width", 1));
	pstRgnInfo.u32PosY = UPALIGNTO16(osd_data->origin_y * rk_param_get_int("video.1:height", 1) /
	                                 rk_param_get_int("video.0:height", 1));
	ret = RK_MPI_VENC_RGN_SetBitMap(VIDEO_PIPE_1, &pstRgnInfo, &stBitmap);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("VIDEO_PIPE_1: RK_MPI_VENC_RGN_SetBitMap failed with %d\n", ret);
		return RK_FAILURE;
	}

	return ret;
}

int rkipc_osd_init() {
	RK_MPI_VENC_RGN_Init(VIDEO_PIPE_0, NULL);
	RK_MPI_VENC_RGN_Init(VIDEO_PIPE_1, NULL);
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

int rk_roi_set(roi_data_s *roi_data) {
	// LOG_DEBUG("id is %d\n", id);
	int ret = 0;
	int venc_chn = 0;
	int region_index = roi_data->id - 1;

	// 同一个码流的多个区域数据需要保存下来为一组，以便后续同时SetRoiAttr
	if (g_roi_region_cnt == 0)
		g_pst_roi_attr = (VENC_ROI_ATTR_S *)malloc(RV1126_ROI_REGION * sizeof(VENC_ROI_ATTR_S));

	g_roi_region_cnt++;
	if (!strcmp(roi_data->stream_type, "mainStream")) {
		venc_chn = 0;
	} else if (!strcmp(roi_data->stream_type, "subStream")) {
		venc_chn = 1;
	} else {
		ret = RK_FAILURE;
		LOG_DEBUG("%s is not exit\n", roi_data->stream_type);
		goto exit;
	}

	LOG_DEBUG("region[%d]: venc_chn is %d, region_index is %d\n", g_roi_region_cnt, venc_chn,
	          region_index);
	g_pst_roi_attr[region_index].u32Index = roi_data->id;
	g_pst_roi_attr[region_index].bEnable = roi_data->enabled;
	g_pst_roi_attr[region_index].bAbsQp = RK_FALSE;
	g_pst_roi_attr[region_index].bIntra = RK_FALSE;
	g_pst_roi_attr[region_index].stRect.s32X = roi_data->position_x;
	g_pst_roi_attr[region_index].stRect.s32Y = roi_data->position_y;
	g_pst_roi_attr[region_index].stRect.u32Width = roi_data->width;
	g_pst_roi_attr[region_index].stRect.u32Height = roi_data->height;
	switch (roi_data->quality_level) {
	case 6:
		g_pst_roi_attr[region_index].s32Qp = -16;
		break;
	case 5:
		g_pst_roi_attr[region_index].s32Qp = -14;
		break;
	case 4:
		g_pst_roi_attr[region_index].s32Qp = -12;
		break;
	case 3:
		g_pst_roi_attr[region_index].s32Qp = -10;
		break;
	case 2:
		g_pst_roi_attr[region_index].s32Qp = -8;
		break;
	case 1:
	default:
		g_pst_roi_attr[region_index].s32Qp = -6;
	}

	if (roi_data->id == RV1126_ROI_REGION) {
		ret = RK_MPI_VENC_SetRoiAttr(venc_chn, g_pst_roi_attr, RV1126_ROI_REGION);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_VENC_SetRoiAttr to venc %d failed with %d\n", venc_chn, ret);
		} else {
			LOG_DEBUG("RK_MPI_VENC_SetRoiAttr to venc %d success\n", venc_chn);
		}
	}

exit:
	if (g_roi_region_cnt == MAX_ROI_NUM) {
		g_roi_region_cnt = 0;
		if (g_pst_roi_attr) {
			free(g_pst_roi_attr);
			g_pst_roi_attr = NULL;
		}
	}

	return ret;
}

int rk_video_init() {
	LOG_INFO("%s\n", __func__);
	int ret = 0;
	g_video_run_ = 1;
	ret = RK_MPI_SYS_Init();
	ret |= rkipc_pipe_0_to_RTSP_init(); //主码流
	ret |= rkipc_pipe_0_to_jpeg_init();
	ret |= rkipc_pipe_1_init();
	// ret |= rkipc_pipe_2_init();
	ret |= rkipc_pipe_3_init();
	ret |= rkipc_osd_init();
	rk_roi_set_callback_register(rk_roi_set);
	ret |= rk_roi_set_all();

	return ret;
}

int rk_video_deinit() {
	LOG_INFO("%s\n", __func__);
	g_video_run_ = 0;
	int ret = 0;
	rk_roi_set_callback_register(NULL);
	ret = rkipc_osd_deinit();
	ret |= rkipc_pipe_3_deinit();
	// ret |= rkipc_pipe_2_deinit();
	ret |= rkipc_pipe_1_deinit();
	ret |= rkipc_pipe_0_to_jpeg_deinit();
	ret |= rkipc_pipe_0_to_RTSP_deinit(); //主码流

	return ret;
}

int rk_video_restart() {
	int ret;
	ret = rk_video_deinit();
	rk_isp_deinit(0);
	// rk_isp_deinit(1);
	sleep(1);
	rk_isp_init(0, NULL);
	ret |= rk_video_init();
	// rk_isp_init(1, NULL);

	return ret;
}
