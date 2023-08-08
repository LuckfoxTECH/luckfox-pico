// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "video.h"
#include "audio.h"
#include "rkAVS_stitchFor1106App.h"
#include "rockiva.h"

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
#define VIDEO_PIPE_3 3
#define JPEG_VENC_CHN 4
#define VPSS_ROTATE 6
#define VPSS_BGR 0
#define DRAW_NN_OSD_ID 7
#define RED_COLOR 0x0000FF
#define BLUE_COLOR 0xFF0000

#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"
#define RTSP_URL_2 "/live/2"
#define RTMP_URL_0 "rtmp://127.0.0.1:1935/live/mainstream"
#define RTMP_URL_1 "rtmp://127.0.0.1:1935/live/substream"
#define RTMP_URL_2 "rtmp://127.0.0.1:1935/live/thirdstream"

pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
rtsp_demo_handle g_rtsplive = NULL;
rtsp_session_handle g_rtsp_session_0, g_rtsp_session_1, g_rtsp_session_2;
static int g_sensor_num = 2;
static int send_jpeg_cnt = 0;
static int get_jpeg_cnt = 0;
static int enable_ivs, enable_jpeg, enable_venc_0, enable_venc_1, enable_rtsp, enable_rtmp;
static int g_enable_vo, g_vo_dev_id, g_vi_chn_id, enable_npu, enable_wrap, enable_osd;
static int g_video_run_ = 1;
static int g_osd_run_ = 1;
static int pipe_id_ = 0;
static int dev_id_ = 0;
static int cycle_snapshot_flag = 0;
static int g_avs_width = 0;
static int g_avs_height = 0;
static const char *tmp_output_data_type = "H.264";
static const char *tmp_rc_mode;
static const char *tmp_h264_profile;
static const char *tmp_smart;
static const char *tmp_gop_mode;
static const char *tmp_rc_quality;
static pthread_t vi_thread_1, venc_thread_0, venc_thread_1, venc_thread_2, jpeg_venc_thread_id,
    vpss_thread_rgb, cycle_snapshot_thread_id, get_nn_update_osd_thread_id,
    get_vi_send_jpeg_thread_id, get_vi_2_send_thread, get_ivs_result_thread;

static MB_POOL mb_pool, tmp_mb_pool;
static MB_BLK dst_buf_0, dst_buf_1, tmp_top_mb_0, tmp_top_mb_1, tmp_bottom_mb_0, tmp_bottom_mb_1;
static MB_BLK tmp_top_mb_run, tmp_bottom_mb_run, dst_mb_run;
static pthread_t get_vi_thread_id, rga_pre_process_id, cpu_algo_thread_id;
static int rga_ready_cnt = 0;
static int rga_pre_process_ok = 0;
static uint64_t tmp_pts;

static MPP_CHN_S vi_chn[2], vpss_chn[2], venc_chn[2];
static AVS_IMAGE_DATA_EX stLeftImageEx, stRightImageEx;
static AVS_IMAGE_DATA_EX stStitchImageEx;
static AVS_INPUT_PARAMS avsSetParams;
static AVS_STITCH_ROI_PARAMS_S *sptStitchRoiParams;
static AVS_ENGINE stEngine;
static uint16_t *meshYUV[2];
static uint16_t *alphaYuv;
static uint8_t *stitchImage;

typedef enum rkCOLOR_INDEX_E {
	RGN_COLOR_LUT_INDEX_0 = 0,
	RGN_COLOR_LUT_INDEX_1 = 1,
} COLOR_INDEX_E;

static void *rkipc_get_venc_0(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcVenc0", 0, 0, 0);
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
			// LOG_DEBUG("Count:%d, Len:%d, PTS is %" PRId64", enH264EType is %d\n", loopCount,
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
				if (enable_rtmp)
					rk_rtmp_write_video_frame(0, data, stFrame.pstPack->u32Len,
					                          stFrame.pstPack->u64PTS, 1);
			} else {
				rk_storage_write_video_frame(0, data, stFrame.pstPack->u32Len,
				                             stFrame.pstPack->u64PTS, 0);
				if (enable_rtmp)
					rk_rtmp_write_video_frame(0, data, stFrame.pstPack->u32Len,
					                          stFrame.pstPack->u64PTS, 0);
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
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcVenc1", 0, 0, 0);
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
				if (enable_rtmp)
					rk_rtmp_write_video_frame(1, data, stFrame.pstPack->u32Len,
					                          stFrame.pstPack->u64PTS, 1);
			} else {
				rk_storage_write_video_frame(1, data, stFrame.pstPack->u32Len,
				                             stFrame.pstPack->u64PTS, 0);
				if (enable_rtmp)
					rk_rtmp_write_video_frame(1, data, stFrame.pstPack->u32Len,
					                          stFrame.pstPack->u64PTS, 0);
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

int rkipc_rtsp_init() {
	LOG_DEBUG("start\n");
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session_0 = rtsp_new_session(g_rtsplive, RTSP_URL_0);
	g_rtsp_session_1 = rtsp_new_session(g_rtsplive, RTSP_URL_1);
	tmp_output_data_type = rk_param_get_string("video.0:output_data_type", "H.264");
	if (!strcmp(tmp_output_data_type, "H.264"))
		rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	else if (!strcmp(tmp_output_data_type, "H.265"))
		rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	else
		LOG_DEBUG("0 tmp_output_data_type is %s, not support\n", tmp_output_data_type);

	tmp_output_data_type = rk_param_get_string("video.1:output_data_type", "H.264");
	if (!strcmp(tmp_output_data_type, "H.264"))
		rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	else if (!strcmp(tmp_output_data_type, "H.265"))
		rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	else
		LOG_DEBUG("1 tmp_output_data_type is %s, not support\n", tmp_output_data_type);

	rtsp_sync_video_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session_1, rtsp_get_reltime(), rtsp_get_ntptime());
	LOG_DEBUG("end\n");

	return 0;
}

int rkipc_rtsp_deinit() {
	LOG_DEBUG("%s\n", __func__);
	if (g_rtsp_session_0) {
		rtsp_del_session(g_rtsp_session_0);
		g_rtsp_session_0 = NULL;
	}
	if (g_rtsp_session_1) {
		rtsp_del_session(g_rtsp_session_1);
		g_rtsp_session_1 = NULL;
	}
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
	g_rtsplive = NULL;
	return 0;
}

int rkipc_rtmp_init() {
	int ret = 0;
	ret |= rk_rtmp_init(0, RTMP_URL_0);
	ret |= rk_rtmp_init(1, RTMP_URL_1);
	// ret |= rk_rtmp_init(2, RTMP_URL_2);

	return ret;
}

int rkipc_rtmp_deinit() {
	int ret = 0;
	ret |= rk_rtmp_deinit(0);
	ret |= rk_rtmp_deinit(1);
	// ret |= rk_rtmp_deinit(2);

	return ret;
}

int rkipc_vi_dev_init() {
	LOG_DEBUG("%s\n", __func__);
	int ret = 0;
	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	memset(&stDevAttr, 0, sizeof(stDevAttr));
	memset(&stBindPipe, 0, sizeof(stBindPipe));
	for (int i = 0; i < g_sensor_num; i++) {
		ret = RK_MPI_VI_GetDevAttr(i, &stDevAttr);
		if (ret == RK_ERR_VI_NOT_CONFIG) {
			ret = RK_MPI_VI_SetDevAttr(i, &stDevAttr);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("%d: RK_MPI_VI_SetDevAttr %#x\n", i, ret);
				return -1;
			}
		} else {
			LOG_ERROR("%d: RK_MPI_VI_SetDevAttr already\n", i);
		}
		ret = RK_MPI_VI_GetDevIsEnable(i);
		if (ret != RK_SUCCESS) {
			ret = RK_MPI_VI_EnableDev(i);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("%d: RK_MPI_VI_EnableDev %#x\n", i, ret);
				return -1;
			}
			stBindPipe.u32Num = 1;
			stBindPipe.PipeId[i] = i;
			ret = RK_MPI_VI_SetDevBindPipe(i, &stBindPipe);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("%d: RK_MPI_VI_SetDevBindPipe %#x\n", i, ret);
				return -1;
			}
		} else {
			LOG_ERROR("%d: RK_MPI_VI_EnableDev already\n", i);
		}
	}

	return 0;
}

int rkipc_vi_dev_deinit() {
	int ret = 0;
	for (int i = 0; i < g_sensor_num; i++) {
		RK_MPI_VI_DisableDev(i);
		if (ret) {
			LOG_ERROR("RK_MPI_VI_DisableDev error! ret=%#x\n", ret);
			return -1;
		}
		LOG_INFO("RK_MPI_VI_DisableDev success\n");
	}

	return 0;
}

int rkipc_multi_vi_init() {
	int video_width = rk_param_get_int("avs:source_width", 1920);
	int video_height = rk_param_get_int("avs:source_height", 1080);
	int buf_cnt = 2; // 比较极限，可能会帧率不足
	int ret = 0;
	VI_CHN_ATTR_S vi_chn_attr;

	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
	vi_chn_attr.u32Depth = 2;
	for (int i = 0; i < g_sensor_num; i++) {
		ret = RK_MPI_VI_SetChnAttr(i, 0, &vi_chn_attr);
		ret |= RK_MPI_VI_EnableChnExt(i, 0);
		if (ret) {
			LOG_ERROR("%d: ERROR: RK_MPI_VI_EnableChnExt error! ret=%#x\n", i, ret);
			return -1;
		}
		LOG_INFO("%d: RK_MPI_VI_EnableChnExt success\n", i);
	}
	// group mode muset buf all ready, or stream on will fail
	for (int i = 0; i < g_sensor_num; i++) {
		ret = RK_MPI_VI_StartPipe(i);
		if (ret) {
			LOG_ERROR("%d: RK_MPI_VI_StartPipe error! ret=%#x\n", i, ret);
			return -1;
		}
		LOG_INFO("%d: RK_MPI_VI_StartPipe success\n", i);
	}

	return 0;
}

int rkipc_multi_vi_deinit() {
	int ret = 0;
	for (int i = 0; i < g_sensor_num; i++) {
		ret = RK_MPI_VI_StopPipe(i);
		if (ret) {
			LOG_ERROR("%d: RK_MPI_VI_StopPipe error, ret=%#x\n", i, ret);
			return ret;
		}
		ret = RK_MPI_VI_DisableChnExt(i, RKISP_MAINPATH);
		if (ret) {
			LOG_ERROR("%d: RK_MPI_VI_DisableChn error, ret=%#x\n", i, ret);
			return ret;
		}
		LOG_INFO("%d: RK_MPI_VI_DisableChn success\n", i);
	}

	return ret;
}

bool readBinParams(char *image_path, char *output_params, uint64_t length) {
	FILE *fp = NULL;
	fp = fopen(image_path, "rb");
	if (fp == NULL) {
		printf("%s fopen fail\n", image_path);
		return false;
	}
	fread(output_params, length, 1, fp);
	close(fp);
	printf("%s readBinParams success\n", image_path);

	return true;
}

int cpu_blend_init() {
	char avslVersion[128]; /* 存放AVS的版本号 */
	char file_path[128];
	rkAVS_getVersion(avslVersion);
	printf("cpu_blend_init, RKALGO_AVS: %s\n", avslVersion);

	int32_t imageWidth = 1080;
	int32_t imageHeight = 1920;

	/*******************************************读取配置参数************************************************/

	avsSetParams.s32LogLevel = 0;
	snprintf(file_path, 127, "%s/rkAvsStitchParams.bin",
	         rk_param_get_string("avs:middle_lut_path", "/oem/usr/share/middle_lut/5m/"));
	readBinParams(file_path, (char *)(&avsSetParams.stStitchParams), sizeof(AVS_STITCH_PARAMS_S));
	sptStitchRoiParams = avsSetParams.stStitchParams.stStitchRoiParams;
	AVS_IMAGE_SIZE_S stStitchImageSize = avsSetParams.stStitchParams.stStitchImageSize;
	AVS_IMAGE_SIZE_S stAlphaSize = avsSetParams.stStitchParams.stAlphaSize;
	int32_t bandNum = avsSetParams.stStitchParams.s32BandNum;

	// 设置分割图像参数
	stLeftImageEx.imageSize.s32ImageWidth = sptStitchRoiParams[0].s32SrcRemapWidth;
	stLeftImageEx.imageSize.s32ImageHeight = sptStitchRoiParams[0].s32SrcRemapHeight;
	stLeftImageEx.pitch.p = sptStitchRoiParams[0].s32SrcRemapWidth;
	// stLeftImageEx.dat.p = srcInputImage[0];

	stRightImageEx.imageSize.s32ImageWidth = sptStitchRoiParams[1].s32SrcRemapWidth;
	stRightImageEx.imageSize.s32ImageHeight = sptStitchRoiParams[1].s32SrcRemapHeight;
	stRightImageEx.pitch.p = sptStitchRoiParams[1].s32SrcRemapWidth;
	// stRightImageEx.dat.p = srcInputImage[1];

	/****************************************读取查找表和Alpha表********************************************/
	//读取查找表
	for (int32_t cam = 0; cam < 2; cam++) {
		int32_t meshHeight = sptStitchRoiParams[cam].s32SrcRemapHeight;
		int32_t meshWidth =
		    ((sptStitchRoiParams[cam].s32SrcRemapWidth + (1 << MESH_SCALE_BIT) - 1) >>
		     MESH_SCALE_BIT) +
		    1;
		int32_t mapSize = (meshHeight * meshWidth * 1.5) * sizeof(uint16_t);
		LOG_INFO("mapSize is %d\n", mapSize);
		meshYUV[cam] = (uint16_t *)malloc(mapSize);
		if (cam == 0) {
			snprintf(file_path, 127, "%s/MeshLdchCpu_0.bin",
			         rk_param_get_string("avs:middle_lut_path", "/oem/usr/share/middle_lut/5m/"));
			readBinParams(file_path, (char *)meshYUV[0], mapSize);
		} else {
			snprintf(file_path, 127, "%s/MeshLdchCpu_1.bin",
			         rk_param_get_string("avs:middle_lut_path", "/oem/usr/share/middle_lut/5m/"));
			readBinParams(file_path, (char *)meshYUV[1], mapSize);
		}
	}

	//读取Alpha表
	uint64_t alphaYSize = 0;
	for (int32_t i = 0; i < bandNum; i++) {
		int32_t curralphaYSize =
		    (stAlphaSize.s32ImageWidth >> i) * (stAlphaSize.s32ImageHeight >> i) * sizeof(int16_t);
		alphaYSize += curralphaYSize;
	}
	uint64_t alphaYuvSize = alphaYSize + (alphaYSize >> 1);
	LOG_INFO("alphaYuvSize is %d\n", alphaYuvSize);
	alphaYuv = (uint16_t *)malloc(alphaYuvSize);
	snprintf(file_path, 127, "%s/AlphaYuv.bin",
	         rk_param_get_string("avs:middle_lut_path", "/oem/usr/share/middle_lut/5m/"));
	readBinParams(file_path, (char *)alphaYuv, alphaYuvSize);

	/********************************************配置输入输出参数*********************************************/
	LOG_INFO("tmpBuffer Size is %d\n", 1080 * 128 * 8 * sizeof(int16_t));
	uint16_t *tmpBuffer = (uint16_t *)malloc(1080 * 128 * 8 * sizeof(int16_t));
	avsSetParams.pu16LutForCpu[0] = meshYUV[0];
	avsSetParams.pu16LutForCpu[1] = meshYUV[1];
	avsSetParams.pu16Alpha = alphaYuv;
	avsSetParams.pu16TmpBuffer = tmpBuffer;

	stStitchImageEx.imageSize.s32ImageWidth = imageWidth;
	stStitchImageEx.imageSize.s32ImageHeight = imageHeight * 2;
	stStitchImageEx.pitch.p = imageWidth;

	stEngine.s32AsynchSelect = rk_param_get_int("avs:avs_mode", 0);
	rkAVS_initParams(&stEngine);

	return 0;
}

int cpu_blend_deinit() {
	rkAVS_destroy(&stEngine);
	free(meshYUV[0]);
	free(meshYUV[1]);
	free(alphaYuv);
	free(stitchImage);

	return 0;
}

static void *sample_get_vi_to_vpss_thread(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "sample_get_vi_to_vpss_thread", 0, 0, 0);
	int ret;
	int i = 0;
	int32_t loopCount = 0;
	VIDEO_FRAME_INFO_S vi_0_frame, vi_1_frame;
	MB_BLK dst_mb_ready;
	long long before_time, cycle_begin_time;

	VIDEO_FRAME_INFO_S dst_frame;

	memset(&dst_frame, 0, sizeof(VIDEO_FRAME_INFO_S));
	dst_frame.stVFrame.enPixelFormat = RK_FMT_YUV420SP;
	dst_frame.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
	dst_frame.stVFrame.u32Width = g_avs_width;
	dst_frame.stVFrame.u32Height = g_avs_height;
	dst_frame.stVFrame.u32VirWidth = g_avs_width;
	dst_frame.stVFrame.u32VirHeight = g_avs_height;

	// rga处理
	im_handle_param_t param;
	memset(&param, 0, sizeof(param));
	rga_buffer_handle_t rga_handle_src, rga_handle_dst;
	rga_buffer_t rga_buffer_src, rga_buffer_dst;
	rga_buffer_t rga_buffer_pat = {0};
	im_rect src_rect, dst_rect;
	im_rect pat_rect = {0};

	char file_name[256];

	while (g_video_run_) {
		// cycle_begin_time = rkipc_get_curren_time_ms();
		ret = RK_MPI_VI_GetChnFrame(0, 0, &vi_0_frame, 1000);
		if (ret) {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", ret);
			continue;
		}
		ret = RK_MPI_VI_GetChnFrame(1, 0, &vi_1_frame, 1000);
		if (ret) {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", ret);
			continue;
		}
		// if (capture_one) {
		// 	time_t t = time(NULL);
		// 	struct tm tm = *localtime(&t);
		// 	snprintf(file_name, 128,
		// 	         "/userdata/1920x1080_camera_0_%d%02d%02d%02d%02d%02d.yuv",
		// 	         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
		// 	         tm.tm_sec);
		// 	LOG_ERROR("file_name is %s\n", file_name);
		// 	FILE *fp = fopen(file_name, "wb");
		// 	fwrite(RK_MPI_MB_Handle2VirAddr(vi_0_frame.stVFrame.pMbBlk), 1,
		// 	       1920 * 1080 * 3 / 2, fp);
		// 	fflush(fp);
		// 	fclose(fp);

		// 	snprintf(file_name, 128,
		// 	         "/userdata/1920x1080_camera_1_%d%02d%02d%02d%02d%02d.yuv",
		// 	         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
		// 	         tm.tm_sec);
		// 	LOG_ERROR("file_name is %s\n", file_name);
		// 	fp = fopen(file_name, "wb");
		// 	fwrite(RK_MPI_MB_Handle2VirAddr(vi_1_frame.stVFrame.pMbBlk), 1,
		// 	       1920 * 1080 * 3 / 2, fp);
		// 	fflush(fp);
		// 	fclose(fp);
		// 	capture_one = 0;
		// }

		if ((loopCount % 2) == 0) {
			dst_mb_ready = dst_buf_0;
		} else {
			dst_mb_ready = dst_buf_1;
		}

		// before_time = rkipc_get_curren_time_ms();
		// 顶部1/2的旋转
		rga_handle_src = importbuffer_fd(RK_MPI_MB_Handle2Fd(vi_0_frame.stVFrame.pMbBlk), &param);
		rga_handle_dst = importbuffer_fd(RK_MPI_MB_Handle2Fd(dst_mb_ready), &param);
		rga_buffer_src =
		    wrapbuffer_handle(rga_handle_src, 1920, 1080, RK_FORMAT_YCbCr_420_SP, 1920, 1080);
		rga_buffer_dst = wrapbuffer_handle(rga_handle_dst, g_avs_width, g_avs_height,
		                                   RK_FORMAT_YCbCr_420_SP, g_avs_width, g_avs_height);

		src_rect.x = 0;
		src_rect.y = 0;
		src_rect.width = 1920;
		src_rect.height = 1080;

		dst_rect.x = 0;
		dst_rect.y = 0;
		dst_rect.width = 1080;
		dst_rect.height = 1920;

		ret = improcess(rga_buffer_src, rga_buffer_dst, rga_buffer_pat, src_rect, dst_rect,
		                pat_rect, IM_HAL_TRANSFORM_ROT_90 | IM_SYNC);
		if (ret != IM_STATUS_SUCCESS)
			LOG_ERROR("top improcess ret is %d, reason: %s\n", ret, imStrError(ret));
		releasebuffer_handle(rga_handle_src);
		releasebuffer_handle(rga_handle_dst);

		// 底部1/2的旋转
		rga_handle_src = importbuffer_fd(RK_MPI_MB_Handle2Fd(vi_1_frame.stVFrame.pMbBlk), &param);
		rga_handle_dst = importbuffer_fd(RK_MPI_MB_Handle2Fd(dst_mb_ready), &param);
		rga_buffer_src =
		    wrapbuffer_handle(rga_handle_src, 1920, 1080, RK_FORMAT_YCbCr_420_SP, 1920, 1080);
		rga_buffer_dst = wrapbuffer_handle(rga_handle_dst, g_avs_width, g_avs_height,
		                                   RK_FORMAT_YCbCr_420_SP, g_avs_width, g_avs_height);

		src_rect.x = 0;
		src_rect.y = 0;
		src_rect.width = 1920;
		src_rect.height = 1080;

		dst_rect.x = 0;
		dst_rect.y = 1920;
		dst_rect.width = 1080;
		dst_rect.height = 1920;

		ret = improcess(rga_buffer_src, rga_buffer_dst, rga_buffer_pat, src_rect, dst_rect,
		                pat_rect, IM_HAL_TRANSFORM_ROT_90 | IM_SYNC);
		if (ret != IM_STATUS_SUCCESS)
			LOG_ERROR("bottom improcess ret is %d, reason: %s\n", ret, imStrError(ret));
		releasebuffer_handle(rga_handle_src);
		releasebuffer_handle(rga_handle_dst);

		// LOG_ERROR("两次rga耗时 %lld\n", rkipc_get_curren_time_ms() - before_time);

		ret = RK_MPI_VI_ReleaseChnFrame(0, 0, &vi_0_frame);
		if (ret)
			LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
		ret = RK_MPI_VI_ReleaseChnFrame(1, 0, &vi_1_frame);
		if (ret)
			LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);

		dst_frame.stVFrame.pMbBlk = dst_mb_ready;
		dst_frame.stVFrame.u64PTS = vi_0_frame.stVFrame.u64PTS;
		ret = RK_MPI_VPSS_SendFrame(0, 0, &dst_frame, 1000);
		if (ret)
			LOG_ERROR("RK_MPI_VPSS_SendFrame timeout %x\n", ret);

		loopCount++;
	}

	return NULL;
}

static void *sample_get_vi_thread(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "sample_get_vi_thread", 0, 0, 0);
	int ret;
	int i = 0;
	int32_t loopCount = 0;
	VIDEO_FRAME_INFO_S vi_0_frame, vi_1_frame;
	MB_BLK dst_mb_ready, tmp_top_mb, tmp_bottom_mb;
	long long before_time, cycle_begin_time;

	// rga处理
	im_handle_param_t param;
	memset(&param, 0, sizeof(param));
	rga_buffer_handle_t rga_handle_src, rga_handle_dst;
	rga_buffer_t rga_buffer_src, rga_buffer_dst;
	rga_buffer_t rga_buffer_pat = {0};
	im_rect src_rect, dst_rect;
	im_rect pat_rect = {0};

	while (g_video_run_) {
		// cycle_begin_time = rkipc_get_curren_time_ms();
		// before_time = rkipc_get_curren_time_ms();
		ret = RK_MPI_VI_GetChnFrame(0, 0, &vi_0_frame, 1000);
		if (ret) {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", ret);
			continue;
		}
		ret = RK_MPI_VI_GetChnFrame(1, 0, &vi_1_frame, 1000);
		if (ret) {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", ret);
			continue;
		}

		// LOG_ERROR("RK_MPI_VI_GetChnFrame cost_time is %lld\n",rkipc_get_curren_time_ms() -
		// before_time);

		if ((loopCount % 2) == 0) {
			tmp_top_mb = tmp_top_mb_0;
			tmp_bottom_mb = tmp_bottom_mb_0;
			dst_mb_ready = dst_buf_0;
		} else {
			tmp_top_mb = tmp_top_mb_1;
			tmp_bottom_mb = tmp_bottom_mb_1;
			dst_mb_ready = dst_buf_1;
		}

		// before_time = rkipc_get_curren_time_ms();
		// 顶部1/4的缩放+旋转
		rga_handle_src = importbuffer_fd(RK_MPI_MB_Handle2Fd(vi_0_frame.stVFrame.pMbBlk), &param);
		rga_handle_dst = importbuffer_fd(RK_MPI_MB_Handle2Fd(dst_mb_ready), &param);
		rga_buffer_src =
		    wrapbuffer_handle(rga_handle_src, 1920, 1080, RK_FORMAT_YCbCr_420_SP, 1920, 1080);
		rga_buffer_dst = wrapbuffer_handle(
		    rga_handle_dst, g_avs_width,
		    avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight, RK_FORMAT_YCbCr_420_SP,
		    g_avs_width, avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight);

		src_rect.x = UPALIGNTO2(sptStitchRoiParams[0].s32SrcCopyStartY);
		src_rect.y = UPALIGNTO2(g_avs_width - sptStitchRoiParams[0].s32SrcCopyStartX -
		                        sptStitchRoiParams[0].s32SrcCopyWidth);
		src_rect.width = UPALIGNTO2(sptStitchRoiParams[0].s32SrcCopyHeight);
		src_rect.height = UPALIGNTO2(sptStitchRoiParams[0].s32SrcCopyWidth);

		dst_rect.x = UPALIGNTO2(sptStitchRoiParams[0].s32DstCopyStartX);
		dst_rect.y = UPALIGNTO2(sptStitchRoiParams[0].s32DstCopyStartY);
		dst_rect.width = UPALIGNTO2(sptStitchRoiParams[0].s32DstCopyWidth);
		dst_rect.height = UPALIGNTO2(sptStitchRoiParams[0].s32DstCopyHeight);

		ret = improcess(rga_buffer_src, rga_buffer_dst, rga_buffer_pat, src_rect, dst_rect,
		                pat_rect, IM_HAL_TRANSFORM_ROT_90 | IM_SYNC);
		if (ret != IM_STATUS_SUCCESS)
			LOG_ERROR("improcess ret is %d, reason: %s\n", ret, imStrError(ret));
		releasebuffer_handle(rga_handle_src);
		releasebuffer_handle(rga_handle_dst);

		// 底部1/4的缩放+旋转
		rga_handle_src = importbuffer_fd(RK_MPI_MB_Handle2Fd(vi_1_frame.stVFrame.pMbBlk), &param);
		rga_handle_dst = importbuffer_fd(RK_MPI_MB_Handle2Fd(dst_mb_ready), &param);
		rga_buffer_src =
		    wrapbuffer_handle(rga_handle_src, 1920, 1080, RK_FORMAT_YCbCr_420_SP, 1920, 1080);
		rga_buffer_dst = wrapbuffer_handle(
		    rga_handle_dst, g_avs_width,
		    avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight, RK_FORMAT_YCbCr_420_SP,
		    g_avs_width, avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight);

		src_rect.x = UPALIGNTO2(sptStitchRoiParams[1].s32SrcCopyStartY);
		src_rect.y = UPALIGNTO2(g_avs_width - sptStitchRoiParams[1].s32SrcCopyStartX -
		                        sptStitchRoiParams[1].s32SrcCopyWidth);
		src_rect.width = UPALIGNTO2(sptStitchRoiParams[1].s32SrcCopyHeight);
		src_rect.height = UPALIGNTO2(sptStitchRoiParams[1].s32SrcCopyWidth);

		dst_rect.x = UPALIGNTO2(sptStitchRoiParams[1].s32DstCopyStartX);
		dst_rect.y = UPALIGNTO2(sptStitchRoiParams[1].s32DstCopyStartY);
		dst_rect.width = UPALIGNTO2(sptStitchRoiParams[1].s32DstCopyWidth);
		dst_rect.height = UPALIGNTO2(sptStitchRoiParams[1].s32DstCopyHeight);

		ret = improcess(rga_buffer_src, rga_buffer_dst, rga_buffer_pat, src_rect, dst_rect,
		                pat_rect, IM_HAL_TRANSFORM_ROT_90 | IM_SYNC);
		if (ret != IM_STATUS_SUCCESS)
			LOG_ERROR("improcess ret is %d, reason: %s\n", ret, imStrError(ret));
		releasebuffer_handle(rga_handle_src);
		releasebuffer_handle(rga_handle_dst);

		// 中间1/2的拼接，先旋转到tmp buffer，再传入cpu算法
		rga_handle_src = importbuffer_fd(RK_MPI_MB_Handle2Fd(vi_0_frame.stVFrame.pMbBlk), &param);
		rga_handle_dst = importbuffer_fd(RK_MPI_MB_Handle2Fd(tmp_top_mb), &param);
		rga_buffer_src = wrapbuffer_handle(rga_handle_src, 1920, g_avs_width,
		                                   RK_FORMAT_YCbCr_420_SP, 1920, g_avs_width);
		rga_buffer_dst = wrapbuffer_handle(
		    rga_handle_dst, g_avs_width, sptStitchRoiParams[0].s32SrcRemapHeight,
		    RK_FORMAT_YCbCr_420_SP, g_avs_width, sptStitchRoiParams[0].s32SrcRemapHeight);
		src_rect.x = sptStitchRoiParams[0].s32SrcRemapStartY;
		src_rect.y = 0;
		src_rect.width = sptStitchRoiParams[0].s32SrcRemapHeight;
		src_rect.height = sptStitchRoiParams[0].s32SrcRemapWidth;
		dst_rect.x = 0;
		dst_rect.y = 0;
		dst_rect.width = sptStitchRoiParams[0].s32SrcRemapWidth;
		dst_rect.height = sptStitchRoiParams[0].s32SrcRemapHeight;
		// printf("顶部 src xywh is %d,%d,%d,%d dst xywh is %d,%d,%d,%d\n",
		// 	src_rect.x,src_rect.y,src_rect.width,src_rect.height,
		// 	dst_rect.x,dst_rect.y,dst_rect.width,dst_rect.height);
		ret = improcess(rga_buffer_src, rga_buffer_dst, rga_buffer_pat, src_rect, dst_rect,
		                pat_rect, IM_HAL_TRANSFORM_ROT_90 | IM_SYNC);
		if (ret != IM_STATUS_SUCCESS)
			LOG_ERROR("improcess ret is %d, reason: %s\n", ret, imStrError(ret));
		releasebuffer_handle(rga_handle_src);
		releasebuffer_handle(rga_handle_dst);

		rga_handle_src = importbuffer_fd(RK_MPI_MB_Handle2Fd(vi_1_frame.stVFrame.pMbBlk), &param);
		rga_handle_dst = importbuffer_fd(RK_MPI_MB_Handle2Fd(tmp_bottom_mb), &param);
		rga_buffer_src =
		    wrapbuffer_handle(rga_handle_src, 1920, 1080, RK_FORMAT_YCbCr_420_SP, 1920, 1080);
		rga_buffer_dst = wrapbuffer_handle(
		    rga_handle_dst, 1080, sptStitchRoiParams[1].s32SrcRemapHeight, RK_FORMAT_YCbCr_420_SP,
		    1080, sptStitchRoiParams[1].s32SrcRemapHeight);
		src_rect.x = sptStitchRoiParams[1].s32SrcRemapStartY;
		src_rect.y = 0;
		src_rect.width = sptStitchRoiParams[1].s32SrcRemapHeight;
		src_rect.height = sptStitchRoiParams[1].s32SrcRemapWidth;
		dst_rect.x = 0;
		dst_rect.y = 0;
		dst_rect.width = sptStitchRoiParams[1].s32SrcRemapWidth;
		dst_rect.height = sptStitchRoiParams[1].s32SrcRemapHeight;
		// printf("底部 src xywh is %d,%d,%d,%d dst xywh is %d,%d,%d,%d\n",
		// 	src_rect.x,src_rect.y,src_rect.width,src_rect.height,
		// 	dst_rect.x,dst_rect.y,dst_rect.width,dst_rect.height);
		ret = improcess(rga_buffer_src, rga_buffer_dst, rga_buffer_pat, src_rect, dst_rect,
		                pat_rect, IM_HAL_TRANSFORM_ROT_90 | IM_SYNC);
		if (ret != IM_STATUS_SUCCESS)
			LOG_ERROR("improcess ret is %d, reason: %s\n", ret, imStrError(ret));
		releasebuffer_handle(rga_handle_src);
		releasebuffer_handle(rga_handle_dst);
		// 顶部1/4 + 底部1/4的缩放+旋转 + 中间1/4旋转 + 1/4旋转
		// LOG_ERROR("四次rga耗时 %lld\n", rkipc_get_curren_time_ms() - before_time);

#if 0
		FILE *fp;
		if (loopCount == 30) {
			fp = fopen("/data/top.yuv", "wb");
			fwrite(RK_MPI_MB_Handle2VirAddr(tmp_top_mb), 1,
				sptStitchRoiParams[0].s32SrcRemapHeight * sptStitchRoiParams[0].s32SrcRemapWidth * 3 / 2, fp);
			RK_MPI_SYS_MmzFlushCache(tmp_top_mb, RK_FALSE);
			fflush(fp);
			fclose(fp);

			fp = fopen("/data/bottom.yuv", "wb");
			fwrite(RK_MPI_MB_Handle2VirAddr(tmp_bottom_mb), 1,
				sptStitchRoiParams[1].s32SrcRemapHeight * sptStitchRoiParams[1].s32SrcRemapWidth * 3 / 2, fp);
			RK_MPI_SYS_MmzFlushCache(tmp_bottom_mb, RK_FALSE);
			fflush(fp);
			fclose(fp);

			fp = fopen("/data/vi_0_1920_1080.yuv", "wb");
			fwrite(RK_MPI_MB_Handle2VirAddr(vi_0_frame.stVFrame.pMbBlk), 1,
				1080 * 1920 * 3 / 2, fp);
			RK_MPI_SYS_MmzFlushCache(vi_0_frame.stVFrame.pMbBlk, RK_FALSE);
			fflush(fp);
			fclose(fp);

			fp = fopen("/data/vi_1_1920_1080.yuv", "wb");
			fwrite(RK_MPI_MB_Handle2VirAddr(vi_1_frame.stVFrame.pMbBlk), 1,
				1080 * 1920 * 3 / 2, fp);
			RK_MPI_SYS_MmzFlushCache(vi_1_frame.stVFrame.pMbBlk, RK_FALSE);
			fflush(fp);
			fclose(fp);

			fp = fopen("/data/dst_no_avs.yuv", "wb");
			fwrite(RK_MPI_MB_Handle2VirAddr(dst_mb_ready), 1,
				1080 * avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight * 3 / 2, fp);
			RK_MPI_SYS_MmzFlushCache(dst_mb_ready, RK_FALSE);
			fflush(fp);
			fclose(fp);
		}
#endif

		ret = RK_MPI_VI_ReleaseChnFrame(0, 0, &vi_0_frame);
		if (ret)
			LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
		ret = RK_MPI_VI_ReleaseChnFrame(1, 0, &vi_1_frame);
		if (ret)
			LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);

		// 判断如果已有两个，就等cpu处理释放完，再去get下一组VI来处理
		// before_time = rkipc_get_curren_time_ms();
		rga_ready_cnt += 1;
		while (rga_ready_cnt == 2) {
			// LOG_ERROR("rga_ready_cnt is %d\n", rga_ready_cnt);
			usleep(5 * 1000);
		}
		// LOG_ERROR("等待cpu处理完成耗时 %lld\n", rkipc_get_curren_time_ms() - before_time);

		dst_mb_run = dst_mb_ready;
		tmp_top_mb_run = tmp_top_mb;
		tmp_bottom_mb_run = tmp_bottom_mb;
		// before_time = rkipc_get_curren_time_ms();
		stLeftImageEx.dat.p = RK_MPI_MB_Handle2VirAddr(tmp_top_mb_run);
		stRightImageEx.dat.p = RK_MPI_MB_Handle2VirAddr(tmp_bottom_mb_run);
		stStitchImageEx.dat.p = RK_MPI_MB_Handle2VirAddr(dst_mb_run);
		RK_MPI_SYS_MmzFlushCache(tmp_top_mb_run, RK_TRUE);
		RK_MPI_SYS_MmzFlushCache(tmp_bottom_mb_run, RK_TRUE);
		// LOG_ERROR("转虚拟地址+刷cache耗时 cost_time is %lld\n", rkipc_get_curren_time_ms()
		// - before_time);
		tmp_pts = vi_0_frame.stVFrame.u64PTS;
		rga_pre_process_ok = 1;

		loopCount++;
		// LOG_ERROR("sample_get_vi_thread cost_time is %lld\n",
		//		rkipc_get_curren_time_ms() - cycle_begin_time);
	}

	return NULL;
}

static void *sample_cpu_algo_thread(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "sample_cpu_algo_thread", 0, 0, 0);
	int ret;
	int32_t loopCount = 0;
	VIDEO_FRAME_INFO_S dst_frame;

	memset(&dst_frame, 0, sizeof(VIDEO_FRAME_INFO_S));
	dst_frame.stVFrame.enPixelFormat = RK_FMT_YUV420SP;
	dst_frame.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
	dst_frame.stVFrame.u32Width = 1080;
	dst_frame.stVFrame.u32Height = avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight;
	dst_frame.stVFrame.u32VirWidth = 1080;
	dst_frame.stVFrame.u32VirHeight = avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight;
	long long before_time, cycle_begin_time;

	while (g_video_run_) {
		// cycle_begin_time = rkipc_get_curren_time_ms();
		// before_time = rkipc_get_curren_time_ms();
		while (rga_pre_process_ok != 1) {
			// LOG_DEBUG("rga_ready_cnt is %d, rga_pre_process_ok is %d\n", rga_ready_cnt,
			//         rga_pre_process_ok);
			usleep(5 * 1000);
		}
		dst_frame.stVFrame.pMbBlk = dst_mb_run;
		dst_frame.stVFrame.u64PTS = tmp_pts;
		// LOG_ERROR("rga_pre_process_ok cost_time is %lld\n", rkipc_get_curren_time_ms() -
		// before_time);

		// before_time = rkipc_get_curren_time_ms();
		// 输入的cache在另一个线程刷了
		rkAVS_stitchImages(&stEngine, &stLeftImageEx, &stRightImageEx, &stStitchImageEx,
		                   &avsSetParams);
		RK_MPI_SYS_MmzFlushCache(dst_frame.stVFrame.pMbBlk, RK_FALSE);
		// LOG_ERROR("rkAVS_stitchImages cost_time is %lld\n",
		// 		rkipc_get_curren_time_ms() - before_time);
#if 0
		if (loopCount == 30) {
			FILE *fp = fopen("/data/avs/dst_have_avs.yuv", "wb");
			fwrite(RK_MPI_MB_Handle2VirAddr(dst_frame.stVFrame.pMbBlk), 1,
				1080 * avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight * 3 / 2, fp);
			RK_MPI_SYS_MmzFlushCache(dst_frame.stVFrame.pMbBlk, RK_FALSE);
			fflush(fp);
			fclose(fp);
		}
#endif
		ret = RK_MPI_VPSS_SendFrame(0, 0, &dst_frame, 1000);
		if (ret)
			LOG_ERROR("RK_MPI_VPSS_SendFrame timeout %x\n", ret);

		loopCount++;
		rga_ready_cnt -= 1;
		if (rga_ready_cnt == 0)
			rga_pre_process_ok = 0;

		// LOG_ERROR("avs cycle cost_time is %lld\n",
		//		rkipc_get_curren_time_ms() - cycle_begin_time);
	}

	return NULL;
}

int rkipc_avs_init() {
	// create MB
	MB_POOL_CONFIG_S stPoolConfig;
	memset(&stPoolConfig, 0, sizeof(MB_POOL_CONFIG_S));
	stPoolConfig.u32MBCnt = 2;
	// stPoolConfig.u64MBSize = 1080 * avsSetParams.stStitchParams.stStitchImageSize.s32ImageHeight
	// * 3 / 2;
	stPoolConfig.u64MBSize = g_avs_width * g_avs_height * 3 / 2;
	stPoolConfig.enRemapMode = MB_REMAP_MODE_CACHED;
	stPoolConfig.enAllocType = MB_ALLOC_TYPE_DMA;
	stPoolConfig.enDmaType = MB_DMA_TYPE_CMA;
	stPoolConfig.bPreAlloc = RK_TRUE;
	mb_pool = RK_MPI_MB_CreatePool(&stPoolConfig);
	dst_buf_0 = RK_MPI_MB_GetMB(mb_pool, 0, RK_TRUE);
	dst_buf_1 = RK_MPI_MB_GetMB(mb_pool, 0, RK_TRUE);

	stPoolConfig.u32MBCnt = 4;
	stPoolConfig.u64MBSize = 1080 * 960 * 3 / 2;
	tmp_mb_pool = RK_MPI_MB_CreatePool(&stPoolConfig);
	tmp_top_mb_0 = RK_MPI_MB_GetMB(tmp_mb_pool, 0, RK_TRUE);
	tmp_top_mb_1 = RK_MPI_MB_GetMB(tmp_mb_pool, 0, RK_TRUE);
	tmp_bottom_mb_0 = RK_MPI_MB_GetMB(tmp_mb_pool, 0, RK_TRUE);
	tmp_bottom_mb_1 = RK_MPI_MB_GetMB(tmp_mb_pool, 0, RK_TRUE);

	if (rk_param_get_int("avs:blend_mode", 0) == 0) {
		cpu_blend_init();
		pthread_create(&get_vi_thread_id, NULL, sample_get_vi_thread, NULL);
		pthread_create(&cpu_algo_thread_id, NULL, sample_cpu_algo_thread, NULL);
	} else {
		pthread_create(&get_vi_thread_id, NULL, sample_get_vi_to_vpss_thread, NULL);
	}

	return 0;
}

int rkipc_avs_deinit() {
	if (rk_param_get_int("avs:blend_mode", 0) == 0) {
		pthread_join(cpu_algo_thread_id, NULL);
		pthread_join(get_vi_thread_id, NULL);
		cpu_blend_deinit();
	} else {
		pthread_join(get_vi_thread_id, NULL);
	}

	RK_MPI_MB_ReleaseMB(dst_buf_0);
	RK_MPI_MB_ReleaseMB(dst_buf_1);
	RK_MPI_MB_DestroyPool(mb_pool);
	RK_MPI_MB_ReleaseMB(tmp_top_mb_0);
	RK_MPI_MB_ReleaseMB(tmp_top_mb_1);
	RK_MPI_MB_ReleaseMB(tmp_bottom_mb_0);
	RK_MPI_MB_ReleaseMB(tmp_bottom_mb_1);
	RK_MPI_MB_DestroyPool(tmp_mb_pool);

	return 0;
}

int rkipc_vpss_0_init() {
	int ret;
	VPSS_CHN VpssChn[2] = {VPSS_CHN0, VPSS_CHN1};
	VPSS_GRP VpssGrp = 0;
	VPSS_GRP_ATTR_S stVpssGrpAttr;
	VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
	memset(&stVpssGrpAttr, 0, sizeof(stVpssGrpAttr));
	memset(&stVpssChnAttr[0], 0, sizeof(stVpssChnAttr[0]));
	stVpssGrpAttr.u32MaxW = 4096;
	stVpssGrpAttr.u32MaxH = 4096;
	stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
	stVpssGrpAttr.enPixelFormat = RK_FMT_YUV420SP;
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
	stVpssChnAttr[0].u32Width = rk_param_get_int("video.0:width", 1080);
	stVpssChnAttr[0].u32Height = rk_param_get_int("video.0:height", 3840);
	stVpssChnAttr[0].u32FrameBufCnt = 1; // 可能帧率不足
	// stVpssChnAttr[0].u32Depth = 1;
	ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[0], &stVpssChnAttr[0]);
	if (ret != RK_SUCCESS)
		LOG_ERROR("RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
	ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[0]);
	if (ret != RK_SUCCESS)
		LOG_ERROR("RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);

	if (enable_venc_1) {
		stVpssChnAttr[1].enChnMode = VPSS_CHN_MODE_USER;
		stVpssChnAttr[1].enCompressMode = COMPRESS_MODE_NONE;
		stVpssChnAttr[1].enDynamicRange = DYNAMIC_RANGE_SDR8;
		stVpssChnAttr[1].enPixelFormat = RK_FMT_YUV420SP;
		stVpssChnAttr[1].stFrameRate.s32SrcFrameRate = -1;
		stVpssChnAttr[1].stFrameRate.s32DstFrameRate = -1;
		stVpssChnAttr[1].u32Width = rk_param_get_int("video.1:width", 1920);
		stVpssChnAttr[1].u32Height = rk_param_get_int("video.1:height", 544);
		stVpssChnAttr[1].u32FrameBufCnt = 1; // 可能帧率不足
		// stVpssChnAttr[1].u32Depth = 1;
		ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[1], &stVpssChnAttr[1]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
		ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[1]);
		if (ret != RK_SUCCESS)
			LOG_ERROR("RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);
	}

	ret = RK_MPI_VPSS_SetVProcDev(VpssGrp, VIDEO_PROC_DEV_RGA);
	ret = RK_MPI_VPSS_StartGrp(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_StartGrp error! ret is %#x\n", ret);
		return ret;
	}
	LOG_INFO("end\n");

	return 0;
}

int rkipc_vpss_0_deinit() {
	int ret = 0;
	VPSS_CHN VpssChn[1] = {VPSS_CHN0};
	VPSS_GRP VpssGrp = 0;
	ret |= RK_MPI_VPSS_StopGrp(VpssGrp);
	ret |= RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[0]);
	ret |= RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[1]);
	ret |= RK_MPI_VPSS_DestroyGrp(VpssGrp);

	return ret;
}

int rkipc_bind_init() {
	LOG_INFO("start\n");
	int ret = 0;

	for (int i = 0; i < g_sensor_num; i++) {
		vi_chn[i].enModId = RK_ID_VI;
		vi_chn[i].s32DevId = i;
		vi_chn[i].s32ChnId = RKISP_MAINPATH;
		vpss_chn[i].enModId = RK_ID_VPSS;
		vpss_chn[i].s32DevId = 0;
		vpss_chn[i].s32ChnId = i;
		venc_chn[i].enModId = RK_ID_VENC;
		venc_chn[i].s32DevId = 0;
		venc_chn[i].s32ChnId = i;
	}

	// avs_out_chn.enModId = RK_ID_AVS;
	// avs_out_chn.s32DevId = 0;
	// avs_out_chn.s32ChnId = 0;

	// bind avs, vpss and venc
	if (enable_venc_0) {
		ret = RK_MPI_SYS_Bind(&vpss_chn[0], &venc_chn[0]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_chn[0] [%d, %d] -> venc_chn[0] [%d, %d]\n", ret,
			          vpss_chn[0].s32DevId, vpss_chn[0].s32ChnId, venc_chn[0].s32DevId,
			          venc_chn[0].s32ChnId);
			return ret;
		}
	}
	if (enable_venc_1) {
		ret = RK_MPI_SYS_Bind(&vpss_chn[1], &venc_chn[1]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_chn[1] [%d, %d] -> venc_chn[1] [%d, %d]\n", ret,
			          vpss_chn[1].s32DevId, vpss_chn[1].s32ChnId, venc_chn[1].s32DevId,
			          venc_chn[1].s32ChnId);
			return ret;
		}
	}

	return ret;
}

int rkipc_bind_deinit() {
	LOG_INFO("start\n");
	int ret = 0;

	// unbind
	if (enable_venc_1) {
		ret = RK_MPI_SYS_UnBind(&vpss_chn[1], &venc_chn[1]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("unbind error %#x: vpss_chn[1] [%d, %d] -> venc_chn[1] [%d, %d]", ret,
			          vpss_chn[1].s32DevId, vpss_chn[1].s32ChnId, venc_chn[1].s32DevId,
			          venc_chn[1].s32ChnId);
		}
	}

	if (enable_venc_0) {
		ret = RK_MPI_SYS_UnBind(&vpss_chn[0], &venc_chn[0]);
		if (ret != RK_SUCCESS) {
			LOG_ERROR("bind error %#x: vpss_out_chn[i] [%d, %d] -> venc_chn[i] [%d, %d]", ret,
			          vpss_chn[0].s32DevId, vpss_chn[0].s32ChnId, venc_chn[0].s32DevId,
			          venc_chn[0].s32ChnId);
		}
	}

	return ret;
}

int rkipc_venc_0_init() {
	int ret;
	int video_width = rk_param_get_int("video.0:width", -1);
	int video_height = rk_param_get_int("video.0:height", -1);
	int rotation = rk_param_get_int("avs:rotation", 0);
	int buf_cnt = 2;
	int frame_min_i_qp = rk_param_get_int("video.0:frame_min_i_qp", 26);
	int frame_min_qp = rk_param_get_int("video.0:frame_min_qp", 28);
	int frame_max_i_qp = rk_param_get_int("video.0:frame_max_i_qp", 51);
	int frame_max_qp = rk_param_get_int("video.0:frame_max_qp", 51);
	int scalinglist = rk_param_get_int("video.0:scalinglist", 0);

	// VENC
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	tmp_output_data_type = rk_param_get_string("video.0:output_data_type", NULL);
	tmp_rc_mode = rk_param_get_string("video.0:rc_mode", NULL);
	tmp_h264_profile = rk_param_get_string("video.0:h264_profile", NULL);
	if ((tmp_output_data_type == NULL) || (tmp_rc_mode == NULL)) {
		LOG_ERROR("tmp_output_data_type or tmp_rc_mode is NULL\n");
		return -1;
	}
	LOG_DEBUG("tmp_output_data_type is %s, tmp_rc_mode is %s, tmp_h264_profile is %s\n",
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
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = rk_param_get_int("video.0:mid_rate", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate =
			    rk_param_get_int("video.0:max_rate", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32MinBitRate =
			    rk_param_get_int("video.0:min_rate", -1);
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
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = rk_param_get_int("video.0:mid_rate", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate =
			    rk_param_get_int("video.0:max_rate", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32MinBitRate =
			    rk_param_get_int("video.0:min_rate", -1);
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
		venc_chn_attr.stGopAttr.u32MaxLtrCount = 1; // long-term reference frame ltr is fixed to 1
	} else if (!strcmp(tmp_gop_mode, "TSVC4")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_TSVC4;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.0:gop", -1);
	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32MaxPicWidth = video_width;
	venc_chn_attr.stVencAttr.u32MaxPicHeight = video_height;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	venc_chn_attr.stVencAttr.u32StreamBufCnt = rk_param_get_int("video.0:buffer_count", 4);
	venc_chn_attr.stVencAttr.u32BufSize = rk_param_get_int("video.0:buffer_size", 1843200);
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_0, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
		return -1;
	}

	tmp_smart = rk_param_get_string("video.0:smart", "close");
	if (!strcmp(tmp_smart, "open"))
		RK_MPI_VENC_EnableSvc(VIDEO_PIPE_0, 1);

	if (rk_param_get_int("video.0:enable_motion_deblur", 0)) {
		ret = RK_MPI_VENC_EnableMotionDeblur(VIDEO_PIPE_0, true);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_EnableMotionDeblur error! ret=%#x\n", ret);
	}
	if (rk_param_get_int("video.0:enable_motion_static_switch", 0)) {
		ret = RK_MPI_VENC_EnableMotionStaticSwitch(VIDEO_PIPE_0, true);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_EnableMotionStaticSwitch error! ret=%#x\n", ret);
	}

	VENC_DEBREATHEFFECT_S debfrath_effect;
	memset(&debfrath_effect, 0, sizeof(VENC_DEBREATHEFFECT_S));
	if (rk_param_get_int("video.0:enable_debreath_effect", 0)) {
		debfrath_effect.bEnable = true;
		debfrath_effect.s32Strength1 = rk_param_get_int("video.0:debreath_effect_strength", 16);
		ret = RK_MPI_VENC_SetDeBreathEffect(VIDEO_PIPE_0, &debfrath_effect);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_SetDeBreathEffect error! ret=%#x\n", ret);
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
		venc_rc_param.stParamH264.u32FrmMinIQp = frame_min_i_qp;
		venc_rc_param.stParamH264.u32FrmMinQp = frame_min_qp;
		venc_rc_param.stParamH264.u32FrmMaxIQp = frame_max_i_qp;
		venc_rc_param.stParamH264.u32FrmMaxQp = frame_max_qp;
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
		venc_rc_param.stParamH265.u32FrmMinIQp = frame_min_i_qp;
		venc_rc_param.stParamH265.u32FrmMinQp = frame_min_qp;
		venc_rc_param.stParamH265.u32FrmMaxIQp = frame_max_i_qp;
		venc_rc_param.stParamH265.u32FrmMaxQp = frame_max_qp;
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetRcParam(VIDEO_PIPE_0, &venc_rc_param);

	if (!strcmp(tmp_output_data_type, "H.264")) {
		VENC_H264_TRANS_S pstH264Trans;
		RK_MPI_VENC_GetH264Trans(VIDEO_PIPE_0, &pstH264Trans);
		pstH264Trans.bScalingListValid = scalinglist;
		RK_MPI_VENC_SetH264Trans(VIDEO_PIPE_0, &pstH264Trans);
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		VENC_H265_TRANS_S pstH265Trans;
		RK_MPI_VENC_GetH265Trans(VIDEO_PIPE_0, &pstH265Trans);
		pstH265Trans.bScalingListEnabled = scalinglist;
		RK_MPI_VENC_SetH265Trans(VIDEO_PIPE_0, &pstH265Trans);
	}
	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
	memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
	stVencChnRefBufShare.bEnable = rk_param_get_int("video.0:enable_refer_buffer_share", 0);
	RK_MPI_VENC_SetChnRefBufShareAttr(VIDEO_PIPE_0, &stVencChnRefBufShare);
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
	pthread_create(&venc_thread_0, NULL, rkipc_get_venc_0, NULL);

	return 0;
}

int rkipc_venc_0_deinit() {
	int ret;
	// VENC
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_0);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_0);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_DEBUG("RK_MPI_VENC_DestroyChn success\n");

	return 0;
}

int rkipc_venc_1_init() {
	int ret;
	int video_width = rk_param_get_int("video.1:width", 1920);
	int video_height = rk_param_get_int("video.1:height", 1080);
	int buf_cnt = rk_param_get_int("video.1:input_buffer_count", 2);
	int rotation = rk_param_get_int("avs:rotation", 0);
	int frame_min_i_qp = rk_param_get_int("video.1:frame_min_i_qp", 26);
	int frame_min_qp = rk_param_get_int("video.1:frame_min_qp", 28);
	int frame_max_i_qp = rk_param_get_int("video.1:frame_max_i_qp", 51);
	int frame_max_qp = rk_param_get_int("video.1:frame_max_qp", 51);
	int scalinglist = rk_param_get_int("video.1:scalinglist", 0);

	// VENC
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	tmp_output_data_type = rk_param_get_string("video.1:output_data_type", NULL);
	tmp_rc_mode = rk_param_get_string("video.1:rc_mode", NULL);
	tmp_h264_profile = rk_param_get_string("video.1:h264_profile", NULL);
	if ((tmp_output_data_type == NULL) || (tmp_rc_mode == NULL)) {
		LOG_ERROR("tmp_output_data_type or tmp_rc_mode is NULL\n");
		return -1;
	}
	LOG_DEBUG("tmp_output_data_type is %s, tmp_rc_mode is %s, tmp_h264_profile is %s\n",
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
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = rk_param_get_int("video.1:mid_rate", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate =
			    rk_param_get_int("video.1:max_rate", -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32MinBitRate =
			    rk_param_get_int("video.1:min_rate", -1);
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
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = rk_param_get_int("video.1:mid_rate", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate =
			    rk_param_get_int("video.1:max_rate", -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32MinBitRate =
			    rk_param_get_int("video.1:min_rate", -1);
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
		venc_chn_attr.stGopAttr.u32MaxLtrCount = 1; // long-term reference frame ltr is fixed to 1
	} else if (!strcmp(tmp_gop_mode, "TSVC4")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_TSVC4;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.1:gop", -1);

	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32MaxPicWidth = video_width;
	venc_chn_attr.stVencAttr.u32MaxPicHeight = video_height;
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

	tmp_smart = rk_param_get_string("video.1:smart", "close");
	if (!strcmp(tmp_smart, "open"))
		RK_MPI_VENC_EnableSvc(VIDEO_PIPE_1, 1);

	if (rk_param_get_int("video.1:enable_motion_deblur", 0)) {
		ret = RK_MPI_VENC_EnableMotionDeblur(VIDEO_PIPE_1, true);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_EnableMotionDeblur error! ret=%#x\n", ret);
	}
	if (rk_param_get_int("video.1:enable_motion_static_switch", 0)) {
		ret = RK_MPI_VENC_EnableMotionStaticSwitch(VIDEO_PIPE_1, true);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_EnableMotionStaticSwitch error! ret=%#x\n", ret);
	}

	VENC_DEBREATHEFFECT_S debfrath_effect;
	memset(&debfrath_effect, 0, sizeof(VENC_DEBREATHEFFECT_S));
	if (rk_param_get_int("video.1:enable_debreath_effect", 0)) {
		debfrath_effect.bEnable = true;
		debfrath_effect.s32Strength1 = rk_param_get_int("video.1:debreath_effect_strength", 16);
		ret = RK_MPI_VENC_SetDeBreathEffect(VIDEO_PIPE_1, &debfrath_effect);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_SetDeBreathEffect error! ret=%#x\n", ret);
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
		venc_rc_param.stParamH264.u32FrmMinIQp = frame_min_i_qp;
		venc_rc_param.stParamH264.u32FrmMinQp = frame_min_qp;
		venc_rc_param.stParamH264.u32FrmMaxIQp = frame_max_i_qp;
		venc_rc_param.stParamH264.u32FrmMaxQp = frame_max_qp;
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
		venc_rc_param.stParamH265.u32FrmMinIQp = frame_min_i_qp;
		venc_rc_param.stParamH265.u32FrmMinQp = frame_min_qp;
		venc_rc_param.stParamH265.u32FrmMaxIQp = frame_max_i_qp;
		venc_rc_param.stParamH265.u32FrmMaxQp = frame_max_qp;
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetRcParam(VIDEO_PIPE_1, &venc_rc_param);

	if (!strcmp(tmp_output_data_type, "H.264")) {
		VENC_H264_TRANS_S pstH264Trans;
		RK_MPI_VENC_GetH264Trans(VIDEO_PIPE_1, &pstH264Trans);
		pstH264Trans.bScalingListValid = scalinglist;
		RK_MPI_VENC_SetH264Trans(VIDEO_PIPE_1, &pstH264Trans);
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		VENC_H265_TRANS_S pstH265Trans;
		RK_MPI_VENC_GetH265Trans(VIDEO_PIPE_1, &pstH265Trans);
		pstH265Trans.bScalingListEnabled = scalinglist;
		RK_MPI_VENC_SetH265Trans(VIDEO_PIPE_1, &pstH265Trans);
	}
	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
	memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
	stVencChnRefBufShare.bEnable = rk_param_get_int("video.1:enable_refer_buffer_share", 0);
	RK_MPI_VENC_SetChnRefBufShareAttr(VIDEO_PIPE_1, &stVencChnRefBufShare);
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
	pthread_create(&venc_thread_1, NULL, rkipc_get_venc_1, NULL);

	return 0;
}

int rkipc_venc_1_deinit() {
	int ret;
	// VENC
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_1);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_1);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_DEBUG("RK_MPI_VENC_DestroyChn success\n");

	return 0;
}

static RK_U8 rgn_color_lut_0_left_value[4] = {0x03, 0xf, 0x3f, 0xff};
static RK_U8 rgn_color_lut_0_right_value[4] = {0xc0, 0xf0, 0xfc, 0xff};
static RK_U8 rgn_color_lut_1_left_value[4] = {0x02, 0xa, 0x2a, 0xaa};
static RK_U8 rgn_color_lut_1_right_value[4] = {0x80, 0xa0, 0xa8, 0xaa};
RK_S32 draw_rect_2bpp(RK_U8 *buffer, RK_U32 width, RK_U32 height, int rgn_x, int rgn_y, int rgn_w,
                      int rgn_h, int line_pixel, COLOR_INDEX_E color_index) {
	int i;
	RK_U8 *ptr = buffer;
	RK_U8 value = 0;
	if (color_index == RGN_COLOR_LUT_INDEX_0)
		value = 0xff;
	if (color_index == RGN_COLOR_LUT_INDEX_1)
		value = 0xaa;

	if (line_pixel > 4) {
		printf("line_pixel > 4, not support\n", line_pixel);
		return -1;
	}

	// printf("YUV %dx%d, rgn (%d,%d,%d,%d), line pixel %d\n", width, height, rgn_x, rgn_y, rgn_w,
	// rgn_h, line_pixel); draw top line
	ptr += (width * rgn_y + rgn_x) >> 2;
	for (i = 0; i < line_pixel; i++) {
		memset(ptr, value, (rgn_w + 3) >> 2);
		ptr += width >> 2;
	}
	// draw letft/right line
	for (i = 0; i < (rgn_h - line_pixel * 2); i++) {
		if (color_index == RGN_COLOR_LUT_INDEX_1) {
			*ptr = rgn_color_lut_1_left_value[line_pixel - 1];
			*(ptr + ((rgn_w + 3) >> 2)) = rgn_color_lut_1_right_value[line_pixel - 1];
		} else {
			*ptr = rgn_color_lut_0_left_value[line_pixel - 1];
			*(ptr + ((rgn_w + 3) >> 2)) = rgn_color_lut_0_right_value[line_pixel - 1];
		}
		ptr += width >> 2;
	}
	// draw bottom line
	for (i = 0; i < line_pixel; i++) {
		memset(ptr, value, (rgn_w + 3) >> 2);
		ptr += width >> 2;
	}
	return 0;
}

static void *rkipc_get_nn_update_osd(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcNpuOsd", 0, 0, 0);

	int ret = 0;
	int line_pixel = 2;
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
	while (g_osd_run_) {
		usleep(40 * 1000);
		rotation = rk_param_get_int("avs:rotation", 0);
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
			LOG_ERROR("RK_MPI_RGN_GetCanvasInfo failed with %#x!", ret);
			continue;
		}
		if ((stCanvasInfo.stSize.u32Width != UPALIGNTO16(video_width)) ||
		    (stCanvasInfo.stSize.u32Height != UPALIGNTO16(video_height))) {
			LOG_WARN("canvas is %d*%d, not equal %d*%d, maybe in the process of switching,"
			         "skip this time\n",
			         stCanvasInfo.stSize.u32Width, stCanvasInfo.stSize.u32Height,
			         UPALIGNTO16(video_width), UPALIGNTO16(video_height));
			continue;
		}
		memset((void *)stCanvasInfo.u64VirAddr, 0,
		       stCanvasInfo.u32VirWidth * stCanvasInfo.u32VirHeight >> 2);
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
			if (x < 0 || y < 0 || w < 0 || h < 0) {
				continue;
			}
			// LOG_DEBUG("i is %d, x,y,w,h is %d,%d,%d,%d\n", i, x, y, w, h);
			if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_PERSON) {
				draw_rect_2bpp((RK_U8 *)stCanvasInfo.u64VirAddr, stCanvasInfo.u32VirWidth,
				               stCanvasInfo.u32VirHeight, x, y, w, h, line_pixel,
				               RGN_COLOR_LUT_INDEX_0);
			} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_FACE) {
				draw_rect_2bpp((RK_U8 *)stCanvasInfo.u64VirAddr, stCanvasInfo.u32VirWidth,
				               stCanvasInfo.u32VirHeight, x, y, w, h, line_pixel,
				               RGN_COLOR_LUT_INDEX_0);
			} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_VEHICLE) {
				draw_rect_2bpp((RK_U8 *)stCanvasInfo.u64VirAddr, stCanvasInfo.u32VirWidth,
				               stCanvasInfo.u32VirHeight, x, y, w, h, line_pixel,
				               RGN_COLOR_LUT_INDEX_1);
			} else if (object->objInfo.type == ROCKIVA_OBJECT_TYPE_NON_VEHICLE) {
				draw_rect_2bpp((RK_U8 *)stCanvasInfo.u64VirAddr, stCanvasInfo.u32VirWidth,
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
			LOG_ERROR("RK_MPI_RGN_UpdateCanvas failed with %#x!", ret);
			continue;
		}
	}

	return 0;
}

int rkipc_osd_draw_nn_init() {
	LOG_DEBUG("start\n");
	int ret = 0;
	RGN_HANDLE RgnHandle = DRAW_NN_OSD_ID;
	RGN_ATTR_S stRgnAttr;
	MPP_CHN_S stMppChn;
	RGN_CHN_ATTR_S stRgnChnAttr;
	BITMAP_S stBitmap;
	int rotation = rk_param_get_int("avs:rotation", 0);

	// create overlay regions
	memset(&stRgnAttr, 0, sizeof(stRgnAttr));
	stRgnAttr.enType = OVERLAY_RGN;
	stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_2BPP;
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
	// after malloc max size, it needs to be set to the actual size
	if (rotation == 90 || rotation == 270) {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.0:height", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.0:width", -1);
	} else {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.0:width", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.0:height", -1);
	}
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
	stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = DRAW_NN_OSD_ID;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[RGN_COLOR_LUT_INDEX_0] = BLUE_COLOR;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[RGN_COLOR_LUT_INDEX_1] = RED_COLOR;
	stMppChn.enModId = RK_ID_VENC;
	stMppChn.s32DevId = 0;
	stMppChn.s32ChnId = 0;
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
	stMppChn.s32ChnId = 0;
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
		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.stH264Cbr.u32BitRate = value;
		} else {
			venc_chn_attr.stRcAttr.stH264Vbr.u32MinBitRate = value / 3;
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = value / 3 * 2;
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate = value;
		}
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
		if (!strcmp(tmp_rc_mode, "CBR")) {
			venc_chn_attr.stRcAttr.stH265Cbr.u32BitRate = value;
		} else {
			venc_chn_attr.stRcAttr.stH265Vbr.u32MinBitRate = value / 3;
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = value / 3 * 2;
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate = value;
		}
	} else {
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);
	snprintf(entry, 127, "video.%d:max_rate", stream_id);
	rk_param_set_int(entry, value);
	snprintf(entry, 127, "video.%d:mid_rate", stream_id);
	rk_param_set_int(entry, value / 3 * 2);
	snprintf(entry, 127, "video.%d:min_rate", stream_id);
	rk_param_set_int(entry, value / 3);

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
	char entry_mid_rate[128] = {'\0'};
	char entry_max_rate[128] = {'\0'};
	char entry_min_rate[128] = {'\0'};
	char entry_dst_frame_rate_den[128] = {'\0'};
	char entry_dst_frame_rate_num[128] = {'\0'};
	char entry_src_frame_rate_den[128] = {'\0'};
	char entry_src_frame_rate_num[128] = {'\0'};
	char entry_rc_mode[128] = {'\0'};
	snprintf(entry_output_data_type, 127, "video.%d:output_data_type", stream_id);
	snprintf(entry_gop, 127, "video.%d:gop", stream_id);
	snprintf(entry_mid_rate, 127, "video.%d:mid_rate", stream_id);
	snprintf(entry_max_rate, 127, "video.%d:max_rate", stream_id);
	snprintf(entry_min_rate, 127, "video.%d:min_rate", stream_id);
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
			venc_chn_attr.stRcAttr.stH264Vbr.u32BitRate = rk_param_get_int(entry_mid_rate, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32MaxBitRate = rk_param_get_int(entry_max_rate, -1);
			venc_chn_attr.stRcAttr.stH264Vbr.u32MinBitRate = rk_param_get_int(entry_min_rate, -1);
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
			venc_chn_attr.stRcAttr.stH265Vbr.u32BitRate = rk_param_get_int(entry_mid_rate, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32MaxBitRate = rk_param_get_int(entry_max_rate, -1);
			venc_chn_attr.stRcAttr.stH265Vbr.u32MinBitRate = rk_param_get_int(entry_min_rate, -1);
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
	*value = rk_param_get_string(entry, "normalP");

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
	int width, height, ret;

	// sscanf(value, "%d*%d", &width, &height);
	// LOG_INFO("value is %s, width is %d, height is %d\n", value, width, height);
	// if (stream_id == 0) {
	// 	g_osd_run_ = 0;
	// 	rkipc_osd_draw_nn_deinit();
	// }

	// // unbind
	// vi_chn.enModId = RK_ID_VI;
	// vi_chn.s32DevId = 0;
	// vi_chn.s32ChnId = stream_id;
	// venc_chn.enModId = RK_ID_VENC;
	// venc_chn.s32DevId = 0;
	// venc_chn.s32ChnId = stream_id;
	// ret = RK_MPI_SYS_UnBind(&vi_chn, &venc_chn);
	// if (ret)
	// 	LOG_ERROR("Unbind VI and VENC error! ret=%#x\n", ret);
	// else
	// 	LOG_DEBUG("Unbind VI and VENC success\n");

	// snprintf(entry, 127, "video.%d:width", stream_id);
	// rk_param_set_int(entry, width);
	// snprintf(entry, 127, "video.%d:height", stream_id);
	// rk_param_set_int(entry, height);

	// VENC_CHN_ATTR_S venc_chn_attr;
	// RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
	// venc_chn_attr.stVencAttr.u32PicWidth = width;
	// venc_chn_attr.stVencAttr.u32PicHeight = height;
	// venc_chn_attr.stVencAttr.u32VirWidth = width;
	// venc_chn_attr.stVencAttr.u32VirHeight = height;
	// ret = RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);
	// if (ret)
	// 	LOG_ERROR("RK_MPI_VENC_SetChnAttr error! ret=%#x\n", ret);
	// if (enable_osd) {
	// 	ret |= rkipc_osd_deinit();
	//  temporarily avoid the memory hole problem, and later change it to osd not to rebuild
	//	usleep(1000 * 1000);
	// 	ret |= rkipc_osd_init();
	// }
	// if (stream_id == 0) {
	// 	g_osd_run_ = 1;
	// 	rkipc_osd_draw_nn_init();
	// }
	// VI_CHN_ATTR_S vi_chn_attr;
	// RK_MPI_VI_GetChnAttr(0, stream_id, &vi_chn_attr);
	// vi_chn_attr.stSize.u32Width = width;
	// vi_chn_attr.stSize.u32Height = height;
	// ret = RK_MPI_VI_SetChnAttr(pipe_id_, stream_id, &vi_chn_attr);
	// if (ret)
	// 	LOG_ERROR("RK_MPI_VI_SetChnAttr error! ret=%#x\n", ret);

	// rk_roi_set_all(); // update roi info, and osd cover attach vi, no update required
	// ret = RK_MPI_SYS_Bind(&vi_chn, &venc_chn);
	// if (ret)
	// 	LOG_ERROR("Unbind VI and VENC error! ret=%#x\n", ret);

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

int rk_video_get_rotation(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "avs:rotation");
	*value = rk_param_get_int(entry, 0);

	return 0;
}

int rk_video_set_rotation(int value) {
	int ret = 0;
	int rotation = 0;
	char entry[128] = {'\0'};
	snprintf(entry, 127, "avs:rotation");
	g_osd_run_ = 0;
	rkipc_osd_draw_nn_deinit();

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
	ret = RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_0, rotation);
	if (ret)
		LOG_ERROR("RK_MPI_VENC_SetChnRotation VIDEO_PIPE_0 error! ret=%#x\n", ret);
	ret = RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_1, rotation);
	if (ret)
		LOG_ERROR("RK_MPI_VENC_SetChnRotation VIDEO_PIPE_1 error! ret=%#x\n", ret);
	ret = RK_MPI_VENC_SetChnRotation(JPEG_VENC_CHN, rotation);
	if (ret)
		LOG_ERROR("RK_MPI_VENC_SetChnRotation JPEG_VENC_CHN error! ret=%#x\n", ret);

	rk_roi_set_all(); // update roi info
	// update osd info, cover currently attaches to VI
	if (enable_osd) {
		ret |= rkipc_osd_deinit();
		// temporarily avoid the memory hole problem, and later change it to osd not to rebuild
		usleep(1000 * 1000);
		ret |= rkipc_osd_init();
	}

	g_osd_run_ = 1;
	rkipc_osd_draw_nn_init();
	return 0;
}

int rk_video_get_smartp_viridrlen(int stream_id, int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.%d:smartp_viridrlen", stream_id);
	*value = rk_param_get_int(entry, 25);

	return 0;
}

int rk_video_set_smartp_viridrlen(int stream_id, int value) {
	char entry[128] = {'\0'};

	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
	venc_chn_attr.stGopAttr.s32VirIdrLen = value;
	RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);

	snprintf(entry, 127, "video.%d:smartp_viridrlen", stream_id);
	rk_param_set_int(entry, value);

	return 0;
}

int rk_video_get_md_switch(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "ivs:md");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_md_switch(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "ivs:md");
	rk_param_set_int(entry, value);
	rk_video_restart();

	return 0;
}

int rk_video_get_md_sensebility(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "ivs:md_sensibility");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_md_sensebility(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "ivs:md_sensibility");
	rk_param_set_int(entry, value);
	rk_video_restart();
}

int rk_video_get_od_switch(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "ivs:od");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_od_switch(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "ivs:od");
	rk_param_set_int(entry, value);
	rk_video_restart();
}

int rkipc_osd_cover_create(int id, osd_data_s *osd_data) {
	// TODO
	return 0;
}

int rkipc_osd_cover_destroy(int id) {
	// TODO
	return 0;
}

int rkipc_osd_bmp_create(int id, osd_data_s *osd_data) {
	LOG_DEBUG("id is %d\n", id);
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
	stRgnAttr.unAttr.stOverlay.u32CanvasNum = 2;
	stRgnAttr.unAttr.stOverlay.stSize.u32Width = osd_data->width;
	stRgnAttr.unAttr.stOverlay.stSize.u32Height = osd_data->height;
	ret = RK_MPI_RGN_Create(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Create (%d) failed with %#x\n", RgnHandle, ret);
		RK_MPI_RGN_Destroy(RgnHandle);
		return RK_FAILURE;
	}
	LOG_DEBUG("The handle: %d, create success\n", RgnHandle);

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

	// stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bEnable = true;
	// stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bForceIntra = false;
	// stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = false;
	// stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp = -3;
	if (enable_venc_0) {
		stMppChn.s32ChnId = 0;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
			return RK_FAILURE;
		}
		LOG_DEBUG("RK_MPI_RGN_AttachToChn to venc0 success\n");
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
		LOG_DEBUG("RK_MPI_RGN_AttachToChn to venc1 success\n");
	}
	if (enable_jpeg) {
		stMppChn.s32ChnId = JPEG_VENC_CHN;
		ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
		if (RK_SUCCESS != ret) {
			LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to jpeg failed with %#x\n", RgnHandle, ret);
			return RK_FAILURE;
		}
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

int rkipc_osd_bmp_destroy(int id) {
	LOG_DEBUG("%s\n", __func__);
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
			LOG_DEBUG("RK_MPI_RGN_DetachFrmChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
	}
	if (enable_venc_1) {
		stMppChn.s32ChnId = 1;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_DEBUG("RK_MPI_RGN_DetachFrmChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
	}
	if (enable_jpeg) {
		stMppChn.s32ChnId = JPEG_VENC_CHN;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_DEBUG("RK_MPI_RGN_DetachFrmChn (%d) to jpeg failed with %#x\n", RgnHandle, ret);
	}

	// destory region
	ret = RK_MPI_RGN_Destroy(RgnHandle);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Destroy [%d] failed with %#x\n", RgnHandle, ret);
	}
	LOG_DEBUG("Destory handle:%d success\n", RgnHandle);

	return ret;
}

int rkipc_osd_bmp_change(int id, osd_data_s *osd_data) {
	// LOG_DEBUG("id is %d\n", id);
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
int rk_video_get_enable_cycle_snapshot(int *value) { return 0; }

int rk_video_set_enable_cycle_snapshot(int value) { return 0; }

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

	VENC_JPEG_PARAM_S stJpegParam;
	memset(&stJpegParam, 0, sizeof(stJpegParam));
	stJpegParam.u32Qfactor = value;
	RK_MPI_VENC_SetJpegParam(JPEG_VENC_CHN, &stJpegParam);

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

	VENC_CHN_ATTR_S venc_chn_attr;
	RK_MPI_VENC_GetChnAttr(JPEG_VENC_CHN, &venc_chn_attr);
	venc_chn_attr.stVencAttr.u32PicWidth = width;
	venc_chn_attr.stVencAttr.u32PicHeight = height;
	venc_chn_attr.stVencAttr.u32VirWidth = width;
	venc_chn_attr.stVencAttr.u32VirHeight = height;
	ret = RK_MPI_VENC_SetChnAttr(JPEG_VENC_CHN, &venc_chn_attr);
	if (ret)
		LOG_ERROR("JPEG RK_MPI_VENC_SetChnAttr error! ret=%#x\n", ret);

	return 0;
}

int rk_take_photo() {
	LOG_DEBUG("start\n");
	if (send_jpeg_cnt || get_jpeg_cnt) {
		LOG_WARN("the last photo was not completed\n");
		return -1;
	}
	if (rkipc_storage_dev_mount_status_get() != DISK_MOUNTED) {
		LOG_WARN("dev not mount\n");
		return -1;
	}
	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = 1;
	RK_MPI_VENC_StartRecvFrame(JPEG_VENC_CHN, &stRecvParam);
	send_jpeg_cnt++;
	get_jpeg_cnt++;

	return 0;
}

int rk_roi_set(roi_data_s *roi_data) {
	// LOG_DEBUG("id is %d\n", id);
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

	if (!strcmp(roi_data->stream_type, "mainStream") && rk_param_get_int("avs:enable_venc_0", 0)) {
		venc_chn = 0;
	} else if (!strcmp(roi_data->stream_type, "subStream") &&
	           rk_param_get_int("avs:enable_venc_1", 0)) {
		venc_chn = 1;
	} else if (!strcmp(roi_data->stream_type, "thirdStream") &&
	           rk_param_get_int("avs:enable_venc_2", 0)) {
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

// int rk_region_clip_set(int venc_chn, region_clip_data_s *region_clip_data) {
// 	int ret = 0;
// 	VENC_CHN_PARAM_S stParam;

// 	RK_MPI_VENC_GetChnParam(venc_chn, &stParam);
// 	if (RK_SUCCESS != ret) {
// 		LOG_ERROR("RK_MPI_VENC_GetChnParam to venc failed with %#x\n", ret);
// 		return RK_FAILURE;
// 	}
// 	LOG_DEBUG("RK_MPI_VENC_GetChnParam to venc success\n");
// 	LOG_DEBUG("venc_chn is %d\n", venc_chn);
// 	if (region_clip_data->enabled)
// 		stParam.stCropCfg.enCropType = VENC_CROP_ONLY;
// 	else
// 		stParam.stCropCfg.enCropType = VENC_CROP_NONE;
// 	stParam.stCropCfg.stCropRect.s32X = region_clip_data->position_x;
// 	stParam.stCropCfg.stCropRect.s32Y = region_clip_data->position_y;
// 	stParam.stCropCfg.stCropRect.u32Width = region_clip_data->width;
// 	stParam.stCropCfg.stCropRect.u32Height = region_clip_data->height;
// 	LOG_DEBUG("xywh is %d,%d,%d,%d\n", stParam.stCropCfg.stCropRect.s32X,
// stParam.stCropCfg.stCropRect.s32Y, 				stParam.stCropCfg.stCropRect.u32Width,
// stParam.stCropCfg.stCropRect.u32Height); 	ret = RK_MPI_VENC_SetChnParam(venc_chn, &stParam);
// if
// (RK_SUCCESS != ret) { 		LOG_ERROR("RK_MPI_VENC_SetChnParam to venc failed with %#x\n", ret);
// return RK_FAILURE;
// 	}
// 	LOG_DEBUG("RK_MPI_VENC_SetChnParam to venc success\n");

// 	return ret;
// }

int rk_video_init() {
	LOG_DEBUG("begin\n");
	int ret = 0;
	enable_ivs = rk_param_get_int("avs:enable_ivs", 1);
	enable_jpeg = rk_param_get_int("avs:enable_jpeg", 1);
	enable_venc_0 = rk_param_get_int("avs:enable_venc_0", 1);
	enable_venc_1 = rk_param_get_int("avs:enable_venc_1", 1);
	enable_rtsp = rk_param_get_int("avs:enable_rtsp", 1);
	enable_rtmp = rk_param_get_int("avs:enable_rtmp", 1);
	LOG_INFO("enable_jpeg is %d, enable_venc_0 is %d, enable_venc_1 is %d, enable_rtsp is %d, "
	         "enable_rtmp is %d\n",
	         enable_jpeg, enable_venc_0, enable_venc_1, enable_rtsp, enable_rtmp);

	g_vi_chn_id = rk_param_get_int("avs:vi_chn_id", 0);
	g_enable_vo = rk_param_get_int("avs:enable_vo", 1);
	g_vo_dev_id = rk_param_get_int("avs:vo_dev_id", 3);
	enable_npu = rk_param_get_int("avs:enable_npu", 0);
	enable_wrap = rk_param_get_int("avs:enable_wrap", 0);
	enable_osd = rk_param_get_int("osd.common:enable_osd", 0);
	LOG_DEBUG("g_vi_chn_id is %d, g_enable_vo is %d, g_vo_dev_id is %d, enable_npu is %d, "
	          "enable_wrap is %d, enable_osd is %d\n",
	          g_vi_chn_id, g_enable_vo, g_vo_dev_id, enable_npu, enable_wrap, enable_osd);
	g_video_run_ = 1;
	g_osd_run_ = 1;
	g_avs_width = rk_param_get_int("avs:avs_width", 0);
	g_avs_height = rk_param_get_int("avs:avs_height", 0);
	ret |= rkipc_vi_dev_init();
	ret |= rkipc_multi_vi_init();
	ret |= rkipc_avs_init();
	ret |= rkipc_vpss_0_init();
	if (enable_rtsp)
		ret |= rkipc_rtsp_init();
	if (enable_rtmp)
		ret |= rkipc_rtmp_init();
	if (enable_venc_0)
		ret |= rkipc_venc_0_init();
	if (enable_venc_1)
		ret |= rkipc_venc_1_init();
	rkipc_bind_init();
	// The osd dma buffer must be placed in the last application,
	// otherwise, when the font size is switched, holes may be caused
	if (enable_osd)
		ret |= rkipc_osd_init();
	LOG_DEBUG("over\n");

	return ret;
}

int rk_video_deinit() {
	LOG_DEBUG("%s\n", __func__);
	g_video_run_ = 0;
	g_osd_run_ = 0;
	int ret = 0;
	if (enable_osd)
		ret |= rkipc_osd_deinit();
	rkipc_bind_deinit();
	if (enable_venc_0) {
		pthread_join(venc_thread_0, NULL);
		ret |= rkipc_venc_0_deinit();
	}
	if (enable_venc_1) {
		pthread_join(venc_thread_1, NULL);
		ret |= rkipc_venc_1_deinit();
	}
	ret |= rkipc_multi_vi_deinit();
	ret |= rkipc_vi_dev_deinit();
	if (enable_rtmp)
		ret |= rkipc_rtmp_deinit();
	if (enable_rtsp)
		ret |= rkipc_rtsp_deinit();

	return ret;
}

extern char *rkipc_iq_file_path_;
int rk_video_restart() {
	int ret;
	ret = rk_storage_deinit();
	ret |= rk_video_deinit();
	if (rk_param_get_int("avs:enable_aiq", 1))
		ret |= rk_isp_deinit(0);
	if (rk_param_get_int("avs:enable_aiq", 1)) {
		ret |= rk_isp_init(0, rkipc_iq_file_path_);
		if (rk_param_get_int("isp:init_form_ini", 1))
			ret |= rk_isp_set_from_ini(0);
	}
	ret |= rk_video_init();
	if (rk_param_get_int("audio.0:enable", 0))
		rkipc_audio_rtsp_init();
	ret |= rk_storage_init();

	return ret;
}
