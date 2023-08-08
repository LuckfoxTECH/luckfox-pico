// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "video.h"
#include "audio.h"
#include "rockiva.h"

#define HAS_VO 0
#if HAS_VO
#include <rk_mpi_vo.h>
#endif

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
#define DRAW_NN_VENC_CHN_ID 1
#define VPSS_ROTATE 6
#define VPSS_BGR 0
#define DRAW_NN_OSD_ID 7
#define RED_COLOR 0x0000FF
#define BLUE_COLOR 0xFF0000
#define SAVE_ENC_FRM_CNT_MAX 30

#define RK3588_VO_DEV_HDMI 0
#define RK3588_VO_DEV_MIPI 3
#define RK3588_VOP_LAYER_CLUSTER0 0

#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"
#define RTSP_URL_2 "/live/2"
#define RTMP_URL_0 "rtmp://127.0.0.1:1935/live/mainstream"
#define RTMP_URL_1 "rtmp://127.0.0.1:1935/live/substream"
#define RTMP_URL_2 "rtmp://127.0.0.1:1935/live/thirdstream"

pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
rtsp_demo_handle g_rtsplive = NULL;
rtsp_session_handle g_rtsp_session_0, g_rtsp_session_1, g_rtsp_session_2;
static int get_jpeg_cnt = 0;
static int enable_pp, enable_jpeg, enable_venc_0, enable_venc_1, enable_rtsp, enable_rtmp;
static int g_enable_vo, g_vo_dev_id, g_vi_chn_id, enable_npu, enable_wrap, enable_osd;
static int g_video_run_ = 1;
static int g_nn_osd_run_ = 1;
static int pipe_id_ = 0;
static int dev_id_ = 0;
static int cycle_snapshot_flag = 0;
static const char *tmp_output_data_type = "H.264";
static const char *tmp_rc_mode;
static const char *tmp_h264_profile;
static const char *tmp_smart;
static const char *tmp_gop_mode;
static const char *tmp_rc_quality;
static FILE *venc0_file;
static pthread_t vi_thread_1, venc_thread_0, venc_thread_1, venc_thread_2, jpeg_venc_thread_id,
    vpss_thread_rgb, cycle_snapshot_thread_id, get_nn_update_osd_thread_id, get_vi_2_thread,
    get_ivs_result_thread;

static MPP_CHN_S vi_chn, vpss_bgr_chn, vpss_rotate_chn, vo_chn, vpss_out_chn[4], venc_chn, ivs_chn;
static VO_DEV VoLayer = RK3588_VOP_LAYER_CLUSTER0;
typedef enum rkCOLOR_INDEX_E {
	RGN_COLOR_LUT_INDEX_0 = 0,
	RGN_COLOR_LUT_INDEX_1 = 1,
} COLOR_INDEX_E;

#if HAS_VO
static void *get_vi_send_vo(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "get_vi_send_vo", 0, 0, 0);
	VIDEO_FRAME_INFO_S stViFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_1, &stViFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
			LOG_ERROR("RK_MPI_VI_GetChnFrame ok:data %p loop:%d pts:%" PRId64 " ms\n", data,
			          loopCount, stViFrame.stVFrame.u64PTS / 1000);
			// 6.get the channel status
			// ret = RK_MPI_VI_QueryChnStatus(pipe_id_, VIDEO_PIPE_1, &stChnStatus);
			// LOG_ERROR("RK_MPI_VI_QueryChnStatus ret %x, "
			//           "w:%d,h:%d,enable:%d,lost:%d,framerate:%d,vbfail:%d\n",
			//           ret, stChnStatus.stSize.u32Width, stChnStatus.stSize.u32Height,
			//           stChnStatus.bEnable, stChnStatus.u32LostFrame, stChnStatus.u32FrameRate,
			//           stChnStatus.u32VbFail);

			// send vo
			ret = RK_MPI_VO_SendFrame(VoLayer, 0, &stViFrame, 1000);
			if (ret)
				LOG_ERROR("RK_MPI_VO_SendFrame timeout %x\n", ret);

			// 7.release the frame
			ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_1, &stViFrame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
			}
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", ret);
		}
		usleep(10 * 1000);
	}

	return 0;
}
#endif

static void *rkipc_get_venc_0(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcVenc0", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	// FILE *fp = fopen("/data/venc.h265", "wb");
	FILE *fp = fopen("/tmp/pts.txt", "wb");
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_0, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			if (venc0_file && loopCount <= SAVE_ENC_FRM_CNT_MAX) {
				fwrite(data, 1, stFrame.pstPack->u32Len, venc0_file);
				fflush(venc0_file);
			}
			if (fp && (loopCount <= SAVE_ENC_FRM_CNT_MAX)) {
				RK_U64 nowUs = rkipc_get_curren_time_ms();
				char str[128] = {0};
				int len;
				LOG_INFO("chn:0, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n", loopCount,
				         stFrame.u32Seq, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
				         nowUs - stFrame.pstPack->u64PTS);
				len = snprintf(str, sizeof(str), "seq:%u, pts:%llums\n", stFrame.u32Seq,
				               stFrame.pstPack->u64PTS / 1000);
				fwrite(str, 1, len, fp);
				fflush(fp);
			}
			// fwrite(data, 1, stFrame.pstPack->u32Len, fp);
			// fflush(fp);
			// LOG_DEBUG("Count:%d, Len:%d, PTS is %" PRId64 ", enH264EType is %d\n", loopCount,
			//           stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			//           stFrame.pstPack->DataType.enH264EType);

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
	if (venc0_file)
		fclose(venc0_file);
	if (fp)
		fclose(fp);
	if (stFrame.pstPack)
		free(stFrame.pstPack);
	// if (fp)
	// fclose(fp);

	return 0;
}

static int rga_nv12_border(rga_buffer_t buf, int x, int y, int width, int height, int line_pixel,
                           int color) {
	im_rect rect_up = {x, y, width, line_pixel};
	im_rect rect_buttom = {x, y + height - line_pixel, width, line_pixel};
	im_rect rect_left = {x, y, line_pixel, height};
	im_rect rect_right = {x + width - line_pixel, y, line_pixel, height};
	IM_STATUS STATUS = imfill(buf, rect_up, color);
	STATUS |= imfill(buf, rect_buttom, color);
	STATUS |= imfill(buf, rect_left, color);
	STATUS |= imfill(buf, rect_right, color);
	return STATUS == IM_STATUS_SUCCESS ? 0 : 1;
}

static void *rkipc_get_vi_draw_send_venc(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcVi2Venc", 0, 0, 0);
	VIDEO_FRAME_INFO_S stViFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	int line_pixel = 2;
	long long last_ba_result_time;
	RockIvaBaResult ba_result;
	im_handle_param_t param;
	RockIvaBaObjectInfo *object;
	rga_buffer_handle_t handle;
	rga_buffer_t src;

	memset(&ba_result, 0, sizeof(ba_result));
	memset(&param, 0, sizeof(im_handle_param_t));
	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_1, &stViFrame, 1000);
		if (ret == RK_SUCCESS) {
			uint64_t phy_data = RK_MPI_MB_Handle2PhysAddr(stViFrame.stVFrame.pMbBlk);
			// LOG_DEBUG("phy_data %p, loop:%d pts:%" PRId64 " ms\n", phy_data, loopCount,
			//           stViFrame.stVFrame.u64PTS / 1000);

			ret = rkipc_rknn_object_get(&ba_result);
			if ((!ret && ba_result.objNum) ||
			    ((ret == -1) && (rkipc_get_curren_time_ms() - last_ba_result_time < 300))) {
				// LOG_DEBUG("ret is %d, ba_result.objNum is %d\n", ret, ba_result.objNum);
				handle = importbuffer_physicaladdr(phy_data, &param);
				src = wrapbuffer_handle_t(handle, stViFrame.stVFrame.u32Width,
				                          stViFrame.stVFrame.u32Height, stViFrame.stVFrame.u32Width,
				                          stViFrame.stVFrame.u32Height, RK_FORMAT_YCbCr_420_SP);
				if (!ret)
					last_ba_result_time = rkipc_get_curren_time_ms();
				for (int i = 0; i < ba_result.objNum; i++) {
					int x, y, w, h;
					object = &ba_result.triggerObjects[i];
					LOG_DEBUG("topLeft:[%d,%d], bottomRight:[%d,%d],"
					          "objId is %d, frameId is %d, score is %d, type is %d\n",
					          object->objInfo.rect.topLeft.x, object->objInfo.rect.topLeft.y,
					          object->objInfo.rect.bottomRight.x,
					          object->objInfo.rect.bottomRight.y, object->objInfo.objId,
					          object->objInfo.frameId, object->objInfo.score, object->objInfo.type);
					x = stViFrame.stVFrame.u32Width * object->objInfo.rect.topLeft.x / 10000;
					y = stViFrame.stVFrame.u32Height * object->objInfo.rect.topLeft.y / 10000;
					w = stViFrame.stVFrame.u32Width *
					    (object->objInfo.rect.bottomRight.x - object->objInfo.rect.topLeft.x) /
					    10000;
					h = stViFrame.stVFrame.u32Height *
					    (object->objInfo.rect.bottomRight.y - object->objInfo.rect.topLeft.y) /
					    10000;
					x = x / 2 * 2;
					y = y / 2 * 2;
					w = w / 2 * 2;
					h = h / 2 * 2;
					while (x + w + line_pixel >= stViFrame.stVFrame.u32Width) {
						w -= 8;
					}
					while (y + h + line_pixel >= stViFrame.stVFrame.u32Height) {
						h -= 8;
					}
					LOG_DEBUG("i is %d, x,y,w,h is %d,%d,%d,%d\n", i, x, y, w, h);
					rga_nv12_border(src, x, y, w, h, line_pixel, 0x000000ff);
					// LOG_INFO("draw rect time-consuming is %lld\n",(rkipc_get_curren_time_ms() -
					// last_ba_result_time));
					// LOG_INFO("triggerRules is %d, ruleID is %d, triggerType is %d\n",
					//          object->triggerRules,
					//          object->firstTrigger.ruleID,
					//          object->firstTrigger.triggerType);
				}
				releasebuffer_handle(handle);
			}

			// send venc
			ret = RK_MPI_VENC_SendFrame(VIDEO_PIPE_1, &stViFrame, 1000);
			if (ret)
				LOG_ERROR("RK_MPI_VENC_SendFrame timeout %x\n", ret);
			// 7.release the frame
			ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_1, &stViFrame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
			}

			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", ret);
		}
	}

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
			// LOG_DEBUG("Count:%d, Len:%d, PTS is %" PRId64 ", enH264EType is %d\n", loopCount,
			//           stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			//           stFrame.pstPack->DataType.enH264EType);
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

static void *rkipc_get_jpeg(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcGetJpeg", 0, 0, 0);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
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
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(JPEG_VENC_CHN, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			LOG_DEBUG("Count:%d, Len:%d, PTS is %" PRId64 ", enH264EType is %d\n", loopCount,
			          stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS,
			          stFrame.pstPack->DataType.enH264EType);
			// save jpeg file
			time_t t = time(NULL);
			struct tm tm = *localtime(&t);
			snprintf(file_name, 128, "%s/%d%02d%02d%02d%02d%02d.jpeg", record_path,
			         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
			         tm.tm_sec);
			LOG_INFO("file_name is %s, u32Len is %d\n", file_name, stFrame.pstPack->u32Len);
			FILE *fp = fopen(file_name, "wb");
			if (fp == NULL) {
				LOG_ERROR("fp is NULL\n");
			} else {
				fwrite(data, 1, stFrame.pstPack->u32Len, fp);
			}
			// 7.release the frame
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
		// usleep(33 * 1000);
	}
	if (stFrame.pstPack)
		free(stFrame.pstPack);

	return 0;
}

static void *rkipc_cycle_snapshot(void *arg) {
	LOG_INFO("start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcCycleSnapshot", 0, 0, 0);

	while (g_video_run_ && cycle_snapshot_flag) {
		usleep(rk_param_get_int("video.jpeg:snapshot_interval_ms", 1000) * 1000);
		rk_take_photo();
	}
	LOG_INFO("exit %s thread, arg:%p\n", __func__, arg);

	return 0;
}

static void *rkipc_get_vi_2(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	int s32Ret;
	int32_t loopCount = 0;
	VIDEO_FRAME_INFO_S stViFrame;

	while (g_video_run_) {
		s32Ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_2, &stViFrame, 1000);
		if (s32Ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
			int32_t fd = RK_MPI_MB_Handle2Fd(stViFrame.stVFrame.pMbBlk);

			rkipc_rockiva_write_nv12_frame_by_fd(stViFrame.stVFrame.u32Width,
			                                     stViFrame.stVFrame.u32Height, loopCount, fd);
			s32Ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_2, &stViFrame);
			if (s32Ret != RK_SUCCESS)
				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", s32Ret);
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %x\n", s32Ret);
		}
	}
	return NULL;
}

static void *rkipc_get_vpss_bgr(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "RkipcGetVpssBgr", 0, 0, 0);
	VIDEO_FRAME_INFO_S frame;
	VI_CHN_STATUS_S stChnStatus;
	int32_t loopCount = 0;
	int ret = 0;

	while (g_video_run_) {
		ret = RK_MPI_VPSS_GetChnFrame(VPSS_BGR, 0, &frame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(frame.stVFrame.pMbBlk);
			// LOG_INFO("data:%p, u32Width:%d, u32Height:%d, PTS is %" PRId64 "\n", data,
			//          frame.stVFrame.u32Width, frame.stVFrame.u32Height, frame.stVFrame.u64PTS);
			// rkipc_rockiva_write_rgb888_frame(frame.stVFrame.u32Width, frame.stVFrame.u32Height,
			//                                  data);
			int32_t fd = RK_MPI_MB_Handle2Fd(frame.stVFrame.pMbBlk);
#if 0
			FILE *fp = fopen("/data/test.bgr", "wb");
			fwrite(data, 1, frame.stVFrame.u32Width * frame.stVFrame.u32Height * 3, fp);
			fflush(fp);
			fclose(fp);
			exit(1);
#endif
			// long long last_nn_time = rkipc_get_curren_time_ms();
			rkipc_rockiva_write_rgb888_frame_by_fd(frame.stVFrame.u32Width,
			                                       frame.stVFrame.u32Height, loopCount, fd);
			// LOG_DEBUG("nn time-consuming is %lld\n",(rkipc_get_curren_time_ms() - last_nn_time));

			ret = RK_MPI_VPSS_ReleaseChnFrame(VPSS_BGR, 0, &frame);
			if (ret != RK_SUCCESS)
				LOG_ERROR("RK_MPI_VPSS_ReleaseChnFrame fail %x\n", ret);
			loopCount++;
		} else {
			LOG_ERROR("RK_MPI_VPSS_GetChnFrame timeout %x\n", ret);
		}
	}

	return 0;
}

static void *rkipc_ivs_get_results(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	int ret, i;
	IVS_RESULT_INFO_S stResults;
	int resultscount = 0;
	int count = 0;
	int md = rk_param_get_int("video.3:md", 0);
	int od = rk_param_get_int("video.3:od", 0);
	int width = rk_param_get_int("video.3:width", 640);

	while (g_video_run_) {
		ret = RK_MPI_IVS_GetResults(0, &stResults, 1000);
		if (ret >= 0) {
			resultscount++;
			// LOG_DEBUG("get chn %d results %d\n", 0, resultscount);
			// LOG_DEBUG("get stResults.s32ResultNum %d\n", stResults.s32ResultNum);
			if (md == 1) {
				if (resultscount % 10 == 0 && stResults.s32ResultNum == 1) {
					int x = width / 8 / 8;
					int y = stResults.pstResults->stMdInfo.u32Size / 64;
					if (stResults.pstResults->stMdInfo.pData) {
						// for (int n = 0; n < x * 8; n++)
						// 	printf("-");
						// printf("\n");
						count = 0;
						for (int j = 0; j < y; j++) {
							for (int i = 0; i < x; i++) {
								for (int k = 0; k < 8; k++) {
									if (stResults.pstResults->stMdInfo.pData[j * 64 + i] & (1 << k))
										count++;
									// 	printf("1");
									// else
									// 	printf("0");
								}
							}
							// printf("\n");
						}
						// for (int n = 0; n < x * 8; n++)
						// 	printf("-");
						// printf("\n");
					}
					if (count > (x * y * 8 / 5)) {
						LOG_INFO("Detect movement\n");
					}
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
	sleep(1); // rtmp can't too fast, or substream can't open
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

	pthread_create(&venc_thread_0, NULL, rkipc_get_venc_0, NULL);
	return 0;
}

int rkipc_pipe_0_deinit() {

	pthread_join(venc_thread_0, NULL);
	return 0;
}

int rkipc_pipe_1_init() {
	int ret;
	int video_width = rk_param_get_int("video.1:width", 1920);
	int video_height = rk_param_get_int("video.1:height", 1080);
	int buf_cnt = rk_param_get_int("video.1:input_buffer_count", 2);
	int rotation = rk_param_get_int("video.source:rotation", 0);
	int frame_min_i_qp = rk_param_get_int("video.1:frame_min_i_qp", 26);
	int frame_min_qp = rk_param_get_int("video.1:frame_min_qp", 28);
	int scalinglist = rk_param_get_int("video.1:scalinglist", 0);

	// VI
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stIspOpt.stMaxSize.u32Width = rk_param_get_int("video.1:max_width", 704);
	vi_chn_attr.stIspOpt.stMaxSize.u32Height = rk_param_get_int("video.1:max_height", 576);
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.u32Depth = 0;
	if (g_enable_vo)
		vi_chn_attr.u32Depth += 1;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_1, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_1);
	if (ret) {
		LOG_ERROR("ERROR: RK_MPI_VI_EnableChn error! ret=%#x\n", ret);
		return ret;
	}

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
	tmp_smart = rk_param_get_string("video.1:smart", NULL);
	tmp_gop_mode = rk_param_get_string("video.1:gop_mode", NULL);
	if (!strcmp(tmp_gop_mode, "normalP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	} else if (!strcmp(tmp_gop_mode, "smartP")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		venc_chn_attr.stGopAttr.s32VirIdrLen = rk_param_get_int("video.1:smartp_viridrlen", 25);
		venc_chn_attr.stGopAttr.u32MaxLtrCount = 1; // long-term reference frame ltr is fixed to 1
	} else if (!strcmp(tmp_gop_mode, "tsvc4")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_TSVC4;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.1:gop", -1);

	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32MaxPicWidth = rk_param_get_int("video.1:max_width", 704);
	venc_chn_attr.stVencAttr.u32MaxPicHeight = rk_param_get_int("video.1:max_height", 576);
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

	if (!strcmp(tmp_smart, "open"))
		RK_MPI_VENC_EnableSvc(VIDEO_PIPE_1, 1);

	if (rk_param_get_int("video.1:enable_motion_deblur", 0)) {
		ret = RK_MPI_VENC_EnableMotionDeblur(VIDEO_PIPE_1, true);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_EnableMotionDeblur error! ret=%#x\n", ret);
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

	// pthread_create(&vi_thread_1, NULL, rkipc_get_vi_draw_send_venc, NULL);
	// bind
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = VIDEO_PIPE_1;
	venc_chn.enModId = RK_ID_VENC;
	venc_chn.s32DevId = 0;
	venc_chn.s32ChnId = VIDEO_PIPE_1;
	ret = RK_MPI_SYS_Bind(&vi_chn, &venc_chn);
	if (ret)
		LOG_ERROR("Bind VI and VENC error! ret=%#x\n", ret);
	else
		LOG_DEBUG("Bind VI and VENC success\n");

	if (!g_enable_vo)
		return 0;
#if HAS_VO
	// VO
	VO_PUB_ATTR_S VoPubAttr;
	VO_VIDEO_LAYER_ATTR_S stLayerAttr;
	VO_CSC_S VideoCSC;
	VO_CHN_ATTR_S VoChnAttr;
	RK_U32 u32DispBufLen;
	memset(&VoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
	memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));
	memset(&VideoCSC, 0, sizeof(VO_CSC_S));
	memset(&VoChnAttr, 0, sizeof(VoChnAttr));

	if (g_vo_dev_id == 0) {
		VoPubAttr.enIntfType = VO_INTF_HDMI;
		VoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
	} else {
		VoPubAttr.enIntfType = VO_INTF_MIPI;
		VoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;
	}
	ret = RK_MPI_VO_SetPubAttr(g_vo_dev_id, &VoPubAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetPubAttr %x\n", ret);
		return ret;
	}
	LOG_DEBUG("RK_MPI_VO_SetPubAttr success\n");

	ret = RK_MPI_VO_Enable(g_vo_dev_id);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_Enable err is %x\n", ret);
		return ret;
	}
	LOG_DEBUG("RK_MPI_VO_Enable success\n");

	ret = RK_MPI_VO_GetLayerDispBufLen(VoLayer, &u32DispBufLen);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("Get display buf len failed with error code %d!\n", ret);
		return ret;
	}
	LOG_DEBUG("Get VoLayer %d disp buf len is %d.\n", VoLayer, u32DispBufLen);
	u32DispBufLen = 3;
	ret = RK_MPI_VO_SetLayerDispBufLen(VoLayer, u32DispBufLen);
	if (ret != RK_SUCCESS) {
		return ret;
	}
	LOG_DEBUG("Agin Get VoLayer %d disp buf len is %d.\n", VoLayer, u32DispBufLen);

	/* get vo attribute*/
	ret = RK_MPI_VO_GetPubAttr(g_vo_dev_id, &VoPubAttr);
	if (ret) {
		LOG_ERROR("RK_MPI_VO_GetPubAttr fail!\n");
		return ret;
	}
	LOG_DEBUG("RK_MPI_VO_GetPubAttr success\n");
	if ((VoPubAttr.stSyncInfo.u16Hact == 0) || (VoPubAttr.stSyncInfo.u16Vact == 0)) {
		if (g_vo_dev_id == RK3588_VO_DEV_HDMI) {
			VoPubAttr.stSyncInfo.u16Hact = 1920;
			VoPubAttr.stSyncInfo.u16Vact = 1080;
		} else {
			VoPubAttr.stSyncInfo.u16Hact = 1080;
			VoPubAttr.stSyncInfo.u16Vact = 1920;
		}
	}

	stLayerAttr.stDispRect.s32X = 0;
	stLayerAttr.stDispRect.s32Y = 0;
	stLayerAttr.stDispRect.u32Width = VoPubAttr.stSyncInfo.u16Hact;
	stLayerAttr.stDispRect.u32Height = VoPubAttr.stSyncInfo.u16Vact;
	stLayerAttr.stImageSize.u32Width = VoPubAttr.stSyncInfo.u16Hact;
	stLayerAttr.stImageSize.u32Height = VoPubAttr.stSyncInfo.u16Vact;
	LOG_DEBUG("stLayerAttr W=%d, H=%d\n", stLayerAttr.stDispRect.u32Width,
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
	LOG_DEBUG("RK_MPI_VO_BindLayer success\n");

	ret = RK_MPI_VO_SetLayerAttr(VoLayer, &stLayerAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetLayerAttr VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_DEBUG("RK_MPI_VO_SetLayerAttr success\n");

	ret = RK_MPI_VO_EnableLayer(VoLayer);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_EnableLayer VoLayer = %d error\n", VoLayer);
		return ret;
	}
	LOG_DEBUG("RK_MPI_VO_EnableLayer success\n");

	ret = RK_MPI_VO_SetLayerCSC(VoLayer, &VideoCSC);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VO_SetLayerCSC error\n");
		return ret;
	}
	LOG_DEBUG("RK_MPI_VO_SetLayerCSC success\n");

	ret = RK_MPI_VO_EnableChn(RK3588_VOP_LAYER_CLUSTER0, u32VoChn);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("create RK3588_VOP_LAYER_CLUSTER0 layer %d ch vo failed!\n", u32VoChn);
		return ret;
	}
	LOG_DEBUG("RK_MPI_VO_EnableChn success\n");

	VoChnAttr.bDeflicker = RK_FALSE;
	VoChnAttr.u32Priority = 1;
	VoChnAttr.stRect.s32X = 0;
	VoChnAttr.stRect.s32Y = 0;
	VoChnAttr.stRect.u32Width = stLayerAttr.stDispRect.u32Width;
	VoChnAttr.stRect.u32Height = stLayerAttr.stDispRect.u32Height;
	ret = RK_MPI_VO_SetChnAttr(VoLayer, 0, &VoChnAttr);

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, get_vi_send_vo, NULL);
#endif

	return 0;
}

int rkipc_pipe_1_deinit() {
	int ret;
	// unbind
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = VIDEO_PIPE_1;
	venc_chn.enModId = RK_ID_VENC;
	venc_chn.s32DevId = 0;
	venc_chn.s32ChnId = VIDEO_PIPE_1;
	ret = RK_MPI_SYS_UnBind(&vi_chn, &venc_chn);
	if (ret)
		LOG_ERROR("Unbind VI and VENC error! ret=%#x\n", ret);
	else
		LOG_DEBUG("Unbind VI and VENC success\n");
	// VENC
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_1);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_1);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_DEBUG("RK_MPI_VENC_DestroyChn success\n");
	// VI
	ret = RK_MPI_VI_DisableChn(pipe_id_, VIDEO_PIPE_1);
	if (ret)
		LOG_ERROR("ERROR: Destroy VI error! ret=%#x\n", ret);

	return 0;
}

int rkipc_pipe_2_init() {
	int ret;
	int video_width = rk_param_get_int("video.2:width", -1);
	int video_height = rk_param_get_int("video.2:height", -1);
	int buf_cnt = 2;

	// VI
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stIspOpt.stMaxSize.u32Width = rk_param_get_int("video.2:max_width", 960);
	vi_chn_attr.stIspOpt.stMaxSize.u32Height = rk_param_get_int("video.2:max_height", 540);
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
	vi_chn_attr.stFrameRate.s32SrcFrameRate = rk_param_get_int("isp.0.adjustment:fps", 30);
	vi_chn_attr.stFrameRate.s32DstFrameRate = rk_param_get_int("video.source:npu_fps", 10);
	vi_chn_attr.u32Depth = 0;
	if (enable_npu)
		vi_chn_attr.u32Depth += 1;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_2, &vi_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%#x\n", ret);
		return ret;
	}

	ret = RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_2);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%#x\n", ret);
		return ret;
	}
	pthread_create(&get_vi_2_thread, NULL, rkipc_get_vi_2, NULL);
}

int rkipc_pipe_2_deinit() {
	int ret;
	pthread_join(get_vi_2_thread, NULL);
	ret = RK_MPI_VI_DisableChn(pipe_id_, VIDEO_PIPE_2);
	if (ret)
		LOG_ERROR("ERROR: Destroy VI error! ret=%#x\n", ret);

	return 0;
}

int rkipc_vpss_bgr_init() {
	LOG_DEBUG("start\n");
	int ret;
	VPSS_CHN VpssChn[1] = {VPSS_CHN0};
	VPSS_GRP VpssGrp = VPSS_BGR;
	VPSS_GRP_ATTR_S stVpssGrpAttr;
	VPSS_CHN_ATTR_S stVpssChnAttr[VPSS_MAX_CHN_NUM];
	memset(&stVpssGrpAttr, 0, sizeof(stVpssGrpAttr));
	memset(&stVpssChnAttr[0], 0, sizeof(stVpssChnAttr[0]));
	stVpssGrpAttr.u32MaxW = 4096;
	stVpssGrpAttr.u32MaxH = 4096;
	stVpssGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	stVpssGrpAttr.stFrameRate.s32DstFrameRate = -1;
	stVpssGrpAttr.enPixelFormat = RK_FMT_BGR888; // ROCKIVA need BGR888
	ret = RK_MPI_VPSS_CreateGrp(VpssGrp, &stVpssGrpAttr);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_CreateGrp error! ret is %#x\n", ret);
		return ret;
	}

	stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
	stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	stVpssChnAttr[0].enPixelFormat = RK_FMT_BGR888;
	stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = rk_param_get_int("isp.0.adjustment:fps", 30);
	stVpssChnAttr[0].stFrameRate.s32DstFrameRate = rk_param_get_int("video.source:npu_fps", 10);
	stVpssChnAttr[0].u32Width = 512;
	stVpssChnAttr[0].u32Height = 288;
	stVpssChnAttr[0].u32Depth = 1;
	stVpssChnAttr[0].u32FrameBufCnt = 1;
	ret = RK_MPI_VPSS_SetChnAttr(VpssGrp, VpssChn[0], &stVpssChnAttr[0]);
	if (ret != RK_SUCCESS)
		LOG_ERROR("RK_MPI_VPSS_SetChnAttr error! ret is %#x\n", ret);
	ret = RK_MPI_VPSS_EnableChn(VpssGrp, VpssChn[0]);
	if (ret != RK_SUCCESS)
		LOG_ERROR("RK_MPI_VPSS_EnableChn error! ret is %#x\n", ret);

	ret = RK_MPI_VPSS_SetVProcDev(VpssGrp, VIDEO_PROC_DEV_RGA);
	ret = RK_MPI_VPSS_StartGrp(VpssGrp);
	if (ret != RK_SUCCESS) {
		LOG_ERROR("RK_MPI_VPSS_StartGrp error! ret is %#x\n", ret);
		return ret;
	}
	// bind
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = VIDEO_PIPE_1;
	vpss_bgr_chn.enModId = RK_ID_VPSS;
	vpss_bgr_chn.s32DevId = VPSS_BGR;
	vpss_bgr_chn.s32ChnId = 0;
	ret = RK_MPI_SYS_Bind(&vi_chn, &vpss_bgr_chn);
	if (ret)
		LOG_ERROR("Bind VI and VPSS error! ret=%#x\n", ret);
	else
		LOG_DEBUG("Bind VI and VPSS success\n");
	pthread_create(&vpss_thread_rgb, NULL, rkipc_get_vpss_bgr, NULL);
	LOG_DEBUG("end\n");

	return ret;
}

int rkipc_vpss_bgr_deinit() {
	LOG_DEBUG("start\n");
	int ret = 0;
	VPSS_CHN VpssChn[1] = {VPSS_CHN0};
	VPSS_GRP VpssGrp = 0;
	pthread_join(vpss_thread_rgb, NULL);
	// unbind
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = VIDEO_PIPE_1;
	vpss_bgr_chn.enModId = RK_ID_VPSS;
	vpss_bgr_chn.s32DevId = VPSS_BGR;
	vpss_bgr_chn.s32ChnId = 0;
	ret = RK_MPI_SYS_UnBind(&vi_chn, &vpss_bgr_chn);
	if (ret)
		LOG_ERROR("Unbind VI and VPSS error! ret=%#x\n", ret);
	else
		LOG_DEBUG("Unbind VI and VPSS success\n");
	// deinit
	ret |= RK_MPI_VPSS_StopGrp(VpssGrp);
	ret |= RK_MPI_VPSS_DisableChn(VpssGrp, VpssChn[0]);
	ret |= RK_MPI_VPSS_DestroyGrp(VpssGrp);
	LOG_DEBUG("end\n");

	return ret;
}

int rkipc_pipe_jpeg_init() {
	// jpeg resolution same to video.0
	int ret;
	int video_width = rk_param_get_int("video.jpeg:width", 1920);
	int video_height = rk_param_get_int("video.jpeg:height", 1080);
	int rotation = rk_param_get_int("video.source:rotation", 0);
	// VENC[3] init
	VENC_CHN_ATTR_S jpeg_chn_attr;
	memset(&jpeg_chn_attr, 0, sizeof(jpeg_chn_attr));
	jpeg_chn_attr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
	jpeg_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	jpeg_chn_attr.stVencAttr.u32MaxPicWidth = rk_param_get_int("video.0:max_width", 2560);
	jpeg_chn_attr.stVencAttr.u32MaxPicHeight = rk_param_get_int("video.0:max_height", 1440);
	jpeg_chn_attr.stVencAttr.u32PicWidth = video_width;
	jpeg_chn_attr.stVencAttr.u32PicHeight = video_height;
	jpeg_chn_attr.stVencAttr.u32VirWidth = video_width;
	jpeg_chn_attr.stVencAttr.u32VirHeight = video_height;
	jpeg_chn_attr.stVencAttr.u32StreamBufCnt = 2;
	jpeg_chn_attr.stVencAttr.u32BufSize = rk_param_get_int("video.jpeg:jpeg_buffer_size", 204800);
	// jpeg_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(JPEG_VENC_CHN, &jpeg_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%#x\n", ret);
		return -1;
	}
	VENC_JPEG_PARAM_S stJpegParam;
	memset(&stJpegParam, 0, sizeof(stJpegParam));
	stJpegParam.u32Qfactor = rk_param_get_int("video.jpeg:jpeg_qfactor", 70);
	RK_MPI_VENC_SetJpegParam(JPEG_VENC_CHN, &stJpegParam);
	if (rotation == 0) {
		RK_MPI_VENC_SetChnRotation(JPEG_VENC_CHN, ROTATION_0);
	} else if (rotation == 90) {
		RK_MPI_VENC_SetChnRotation(JPEG_VENC_CHN, ROTATION_90);
	} else if (rotation == 180) {
		RK_MPI_VENC_SetChnRotation(JPEG_VENC_CHN, ROTATION_180);
	} else if (rotation == 270) {
		RK_MPI_VENC_SetChnRotation(JPEG_VENC_CHN, ROTATION_270);
	}

	VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
	memset(&stVencChnBufWrap, 0, sizeof(stVencChnBufWrap));
	if (enable_wrap) {
		stVencChnBufWrap.bEnable = enable_wrap;
		stVencChnBufWrap.u32BufLine =
		    rk_param_get_int("video.source:buffer_line", video_height / 4);
		RK_MPI_VENC_SetChnBufWrapAttr(JPEG_VENC_CHN, &stVencChnBufWrap);
	}

	VENC_COMBO_ATTR_S stComboAttr;
	memset(&stComboAttr, 0, sizeof(VENC_COMBO_ATTR_S));
	stComboAttr.bEnable = RK_TRUE;
	stComboAttr.s32ChnId = VIDEO_PIPE_0;
	RK_MPI_VENC_SetComboAttr(JPEG_VENC_CHN, &stComboAttr);

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = 1;
	RK_MPI_VENC_StartRecvFrame(JPEG_VENC_CHN,
	                           &stRecvParam); // must, for no streams callback running failed

	pthread_create(&jpeg_venc_thread_id, NULL, rkipc_get_jpeg, NULL);
	if (rk_param_get_int("video.jpeg:enable_cycle_snapshot", 0)) {
		cycle_snapshot_flag = 1;
		pthread_create(&cycle_snapshot_thread_id, NULL, rkipc_cycle_snapshot, NULL);
	}

	return ret;
}

int rkipc_pipe_jpeg_deinit() {
	int ret = 0;
	ret = RK_MPI_VENC_StopRecvFrame(JPEG_VENC_CHN);
	ret |= RK_MPI_VENC_DestroyChn(JPEG_VENC_CHN);
	if (ret)
		LOG_ERROR("ERROR: Destroy VENC error! ret=%#x\n", ret);
	else
		LOG_INFO("RK_MPI_VENC_DestroyChn success\n");

	return ret;
}

int rkipc_pipe_3_init() {
	int ret;
	int video_width = rk_param_get_int("video.3:width", -1);
	int video_height = rk_param_get_int("video.3:height", -1);
	int buf_cnt = 2;
	int smear = rk_param_get_int("video.3:smear", 1);
	int weightp = rk_param_get_int("video.3:weightp", 1);
	int md = rk_param_get_int("video.3:md", 0);
	int od = rk_param_get_int("video.3:od", 0);
	if (!smear && !weightp && !md && !od) {
		LOG_INFO("no pp function enabled! end\n");
		return -1;
	}

	// VI
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stIspOpt.stMaxSize.u32Width = video_width;
	vi_chn_attr.stIspOpt.stMaxSize.u32Height = video_height;
	vi_chn_attr.stSize.u32Width = video_width;
	vi_chn_attr.stSize.u32Height = video_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.u32Depth = 0;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_3, &vi_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: RK_MPI_VI_SetChnAttr error! ret=%#x\n", ret);
		return ret;
	}

	ret = RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_3);
	if (ret) {
		LOG_ERROR("ERROR: RK_MPI_VI_EnableChn error! ret=%#x\n", ret);
		return ret;
	}

	// IVS
	IVS_CHN_ATTR_S attr;
	memset(&attr, 0, sizeof(attr));
	attr.enMode = IVS_MODE_MD_OD;
	attr.u32PicWidth = video_width;
	attr.u32PicHeight = video_height;
	attr.enPixelFormat = RK_FMT_YUV420SP;
	attr.s32Gop = 30;
	attr.bSmearEnable = smear;
	attr.bWeightpEnable = weightp;
	attr.bMDEnable = md;
	attr.s32MDInterval = 1;
	attr.bMDNightMode = RK_TRUE;
	attr.u32MDSensibility = rk_param_get_int("video.3:md_sensibility", 3);
	attr.bODEnable = od;
	attr.s32ODInterval = 1;
	attr.s32ODPercent = 7;
	RK_MPI_IVS_CreateChn(0, &attr);

	if (md == 1 || od == 1)
		pthread_create(&get_ivs_result_thread, NULL, rkipc_ivs_get_results, NULL);

	// bind
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = VIDEO_PIPE_3;
	ivs_chn.enModId = RK_ID_IVS;
	ivs_chn.s32DevId = 0;
	ivs_chn.s32ChnId = 0;
	ret = RK_MPI_SYS_Bind(&vi_chn, &ivs_chn);
	if (ret)
		LOG_ERROR("Bind VI and IVS error! ret=%#x\n", ret);
	else
		LOG_DEBUG("Bind VI and IVS success\n");

	return 0;
}

int rkipc_pipe_3_deinit() {
	int ret;
	int md = rk_param_get_int("video.3:md", 0);
	int od = rk_param_get_int("video.3:od", 0);
	if (md == 1 || od == 1)
		pthread_join(get_ivs_result_thread, NULL);

	// unbind
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = VIDEO_PIPE_3;
	ivs_chn.enModId = RK_ID_IVS;
	ivs_chn.s32DevId = 0;
	ivs_chn.s32ChnId = 0;
	ret = RK_MPI_SYS_UnBind(&vi_chn, &ivs_chn);
	if (ret)
		LOG_ERROR("Unbind VI and IVS error! ret=%#x\n", ret);
	else
		LOG_DEBUG("Unbind VI and IVS success\n");
	// IVS
	ret = RK_MPI_IVS_DestroyChn(0);
	if (ret)
		LOG_ERROR("ERROR: Destroy IVS error! ret=%#x\n", ret);
	else
		LOG_DEBUG("RK_MPI_IVS_DestroyChn success\n");
	// VI
	ret = RK_MPI_VI_DisableChn(pipe_id_, VIDEO_PIPE_3);
	if (ret)
		LOG_ERROR("ERROR: Destroy VI error! ret=%#x\n", ret);

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
	while (g_nn_osd_run_) {
		usleep(40 * 1000);
		rotation = rk_param_get_int("video.source:rotation", 0);
		if (rotation == 90 || rotation == 270) {
			video_width = rk_param_get_int("video.1:height", -1);
			video_height = rk_param_get_int("video.1:width", -1);
		} else {
			video_width = rk_param_get_int("video.1:width", -1);
			video_height = rk_param_get_int("video.1:height", -1);
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
	int rotation = rk_param_get_int("video.source:rotation", 0);

	// create overlay regions
	memset(&stRgnAttr, 0, sizeof(stRgnAttr));
	stRgnAttr.enType = OVERLAY_RGN;
	stRgnAttr.unAttr.stOverlay.enPixelFmt = RK_FMT_2BPP;
	stRgnAttr.unAttr.stOverlay.u32CanvasNum = 1;
	if (rotation == 90 || rotation == 270) {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.1:max_height", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.1:max_width", -1);
	} else {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.1:max_width", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.1:max_height", -1);
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
	stMppChn.s32ChnId = DRAW_NN_VENC_CHN_ID;
	ret = RK_MPI_RGN_AttachToChn(RgnHandle, &stMppChn, &stRgnChnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_AttachToChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
		return RK_FAILURE;
	}
	LOG_DEBUG("RK_MPI_RGN_AttachToChn to venc1 success\n");

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
		LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);

	// destory region
	ret = RK_MPI_RGN_Destroy(RgnHandle);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_Destroy [%d] failed with %#x\n", RgnHandle, ret);
	}
	LOG_DEBUG("Destory handle:%d success\n", RgnHandle);

	return ret;
}

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
		LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
	ret = RK_MPI_RGN_GetAttr(RgnHandle, &stRgnAttr);
	if (RK_SUCCESS != ret) {
		LOG_ERROR("RK_MPI_RGN_GetAttr (%d) failed with %#x!", RgnHandle, ret);
		return RK_FAILURE;
	}
	if (rotation == 90 || rotation == 270) {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.1:height", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.1:width", -1);
	} else {
		stRgnAttr.unAttr.stOverlay.stSize.u32Width = rk_param_get_int("video.1:width", -1);
		stRgnAttr.unAttr.stOverlay.stSize.u32Height = rk_param_get_int("video.1:height", -1);
	}
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
	stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = DRAW_NN_OSD_ID;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[RGN_COLOR_LUT_INDEX_0] = BLUE_COLOR;
	stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[RGN_COLOR_LUT_INDEX_1] = RED_COLOR;
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
	VENC_CHN_ATTR_S venc_chn_attr;
	char entry[128] = {'\0'};
	const char *rc_mode;
	int ret;
	rc_mode = rk_param_get_string("video.0:rc_mode", NULL);
	if (stream_id == 0) {
		vi_chn.enModId = RK_ID_VI;
		vi_chn.s32DevId = 0;
		vi_chn.s32ChnId = stream_id;
		venc_chn.enModId = RK_ID_VENC;
		venc_chn.s32DevId = 0;
		venc_chn.s32ChnId = stream_id;
		ret = RK_MPI_SYS_UnBind(&vi_chn, &venc_chn);
		if (ret)
			LOG_ERROR("Unbind VI and VENC error! ret=%#x\n", ret);
		else
			LOG_DEBUG("Unbind VI and VENC success\n");

		RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
		if (!strcmp(value, "H.264")) {
			venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_AVC;
			if (!strcmp(rc_mode, "CBR")) {
				venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
				venc_chn_attr.stRcAttr.stH264Cbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			} else {
				venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
				venc_chn_attr.stRcAttr.stH264Vbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			}
			system("make_meta --update --rk_venc_type 1 --meta_path /dev/block/by-name/meta");
		} else if (!strcmp(value, "H.265")) {
			venc_chn_attr.stVencAttr.enType = RK_VIDEO_ID_HEVC;
			if (!strcmp(rc_mode, "CBR")) {
				venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
				venc_chn_attr.stRcAttr.stH265Cbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			} else {
				venc_chn_attr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
				venc_chn_attr.stRcAttr.stH265Vbr.u32Gop = rk_param_get_int("video.0:gop", -1);
			}
			system("make_meta --update --rk_venc_type 2 --meta_path /dev/block/by-name/meta");
		}
		RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);

		ret = RK_MPI_SYS_Bind(&vi_chn, &venc_chn);
		if (ret)
			LOG_ERROR("bind VI and VENC error! ret=%#x\n", ret);
	}
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

	sscanf(value, "%d*%d", &width, &height);
	LOG_INFO("value is %s, width is %d, height is %d\n", value, width, height);

	// unbind
	vi_chn.enModId = RK_ID_VI;
	vi_chn.s32DevId = 0;
	vi_chn.s32ChnId = stream_id;
	venc_chn.enModId = RK_ID_VENC;
	venc_chn.s32DevId = 0;
	venc_chn.s32ChnId = stream_id;
	ret = RK_MPI_SYS_UnBind(&vi_chn, &venc_chn);
	if (ret)
		LOG_ERROR("Unbind VI and VENC error! ret=%#x\n", ret);
	else
		LOG_DEBUG("Unbind VI and VENC success\n");

	snprintf(entry, 127, "video.%d:width", stream_id);
	rk_param_set_int(entry, width);
	snprintf(entry, 127, "video.%d:height", stream_id);
	rk_param_set_int(entry, height);

	VENC_CHN_ATTR_S venc_chn_attr;
	RK_MPI_VENC_GetChnAttr(stream_id, &venc_chn_attr);
	venc_chn_attr.stVencAttr.u32PicWidth = width;
	venc_chn_attr.stVencAttr.u32PicHeight = height;
	venc_chn_attr.stVencAttr.u32VirWidth = width;
	venc_chn_attr.stVencAttr.u32VirHeight = height;
	ret = RK_MPI_VENC_SetChnAttr(stream_id, &venc_chn_attr);
	if (ret)
		LOG_ERROR("RK_MPI_VENC_SetChnAttr error! ret=%#x\n", ret);

	if (enable_jpeg && (stream_id == 0)) {
		RK_MPI_VENC_GetChnAttr(JPEG_VENC_CHN, &venc_chn_attr);
		venc_chn_attr.stVencAttr.u32PicWidth = width;
		venc_chn_attr.stVencAttr.u32PicHeight = height;
		venc_chn_attr.stVencAttr.u32VirWidth = width;
		venc_chn_attr.stVencAttr.u32VirHeight = height;
		ret = RK_MPI_VENC_SetChnAttr(JPEG_VENC_CHN, &venc_chn_attr);
		if (ret)
			LOG_ERROR("JPEG RK_MPI_VENC_SetChnAttr error! ret=%#x\n", ret);
	}
	VI_CHN_ATTR_S vi_chn_attr;
	RK_MPI_VI_GetChnAttr(pipe_id_, stream_id, &vi_chn_attr);
	vi_chn_attr.stIspOpt.stMaxSize.u32Width = rk_param_get_int("video.0:max_width", 2304);
	vi_chn_attr.stIspOpt.stMaxSize.u32Height = rk_param_get_int("video.0:max_height", 1296);
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, stream_id, &vi_chn_attr);
	if (ret)
		LOG_ERROR("RK_MPI_VI_SetChnAttr error! ret=%#x\n", ret);
	if (stream_id == DRAW_NN_VENC_CHN_ID && enable_npu)
		rkipc_osd_draw_nn_change();
	rk_osd_privacy_mask_restart();
	ret = RK_MPI_SYS_Bind(&vi_chn, &venc_chn);
	if (ret)
		LOG_ERROR("bind VI and VENC error! ret=%#x\n", ret);

	if (stream_id == 0 && enable_npu) {
		snprintf(entry, 127,
		         "make_meta --update --rk_venc_w %d --rk_venc_h %d --meta_path "
		         "/dev/block/by-name/meta",
		         width, height);
		system(entry);
	}

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
	if (stream_id == 0) {
		snprintf(entry, 127,
		         "make_meta --update --rk_cam_fps %d --meta_path /dev/block/by-name/meta", num);
		system(entry);
	}

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
	snprintf(entry, 127, "video.source:rotation");
	*value = rk_param_get_int(entry, 0);

	return 0;
}

int rk_video_set_rotation(int value) {
	int ret = 0;
	int rotation = 0;
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
	if (!enable_wrap) {
		ret = RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_0, rotation);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_SetChnRotation VIDEO_PIPE_0 error! ret=%#x\n", ret);
		ret = RK_MPI_VENC_SetChnRotation(JPEG_VENC_CHN, rotation);
		if (ret)
			LOG_ERROR("RK_MPI_VENC_SetChnRotation JPEG_VENC_CHN error! ret=%#x\n", ret);
	} else {
		LOG_WARN("enable wrap, venc-0 and jpeg can't rotate\n");
	}

	ret = RK_MPI_VENC_SetChnRotation(VIDEO_PIPE_1, rotation);
	if (ret)
		LOG_ERROR("RK_MPI_VENC_SetChnRotation VIDEO_PIPE_1 error! ret=%#x\n", ret);
	rk_roi_set_all(); // update roi info
	// update osd info, cover currently attaches to VI
	rk_osd_privacy_mask_restart();
	if (enable_npu)
		rkipc_osd_draw_nn_change();

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
	snprintf(entry, 127, "video.3:md");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_md_switch(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.3:md");
	rk_param_set_int(entry, value);
	rk_video_restart();

	return 0;
}

int rk_video_get_md_sensebility(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.3:md_sensibility");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_md_sensebility(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.3:md_sensibility");
	rk_param_set_int(entry, value);
	rk_video_restart();
}

int rk_video_get_od_switch(int *value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.3:od");
	*value = rk_param_get_int(entry, -1);

	return 0;
}

int rk_video_set_od_switch(int value) {
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.3:md");
	rk_param_set_int(entry, value);
	rk_video_restart();
}

int rkipc_osd_cover_create(int id, osd_data_s *osd_data) {
	LOG_DEBUG("id is %d\n", id);
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

int rkipc_osd_cover_destroy(int id) {
	LOG_DEBUG("%s\n", __func__);
	int ret = 0;
	// Detach osd from chn
	MPP_CHN_S stMppChn;
	RGN_HANDLE RgnHandle = id;

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
	LOG_DEBUG("Destory handle:%d success\n", RgnHandle);

	return ret;
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
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc0 failed with %#x\n", RgnHandle, ret);
	}
	if (enable_venc_1) {
		stMppChn.s32ChnId = 1;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to venc1 failed with %#x\n", RgnHandle, ret);
	}
	if (enable_jpeg) {
		stMppChn.s32ChnId = JPEG_VENC_CHN;
		ret = RK_MPI_RGN_DetachFromChn(RgnHandle, &stMppChn);
		if (RK_SUCCESS != ret)
			LOG_ERROR("RK_MPI_RGN_DetachFrmChn (%d) to jpeg failed with %#x\n", RgnHandle, ret);
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
	if (value && !cycle_snapshot_flag) {
		cycle_snapshot_flag = 1;
		pthread_create(&cycle_snapshot_thread_id, NULL, rkipc_cycle_snapshot, NULL);
	} else if (!value && cycle_snapshot_flag) {
		get_jpeg_cnt = 0;
		cycle_snapshot_flag = 0;
		pthread_join(cycle_snapshot_thread_id, NULL);
	}

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
#if 0
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
#else
	LOG_INFO("1103 combo, jpeg resolution must be consistent with the main stream resolution\n");
#endif

	return 0;
}

int rk_take_photo() {
	LOG_DEBUG("start\n");
	if (get_jpeg_cnt) {
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

int rkipc_read_venctype_from_meta() {
	struct app_param_info *g_pAppParam = NULL;
	off_t metaAddr = 0;
	void *metaVirmem = NULL, *appVirAddr = NULL;
	int mem_fd = -1;
	int metaSize = get_cmd_val("meta_part_size", 16);
	metaAddr = (off_t)get_cmd_val("meta_load_addr", 16);
	char entry[128] = {'\0'};
	snprintf(entry, 127, "video.0:output_data_type");
	if ((mem_fd = open("/dev/mem", O_RDONLY)) < 0) {
		LOG_ERROR("cannot open /dev/mem.\n");
		return -1;
	}

	metaVirmem = mmap(NULL, metaSize, PROT_READ, MAP_SHARED, mem_fd, metaAddr);
	if (metaVirmem != MAP_FAILED) {
		RK_U32 app_param_offset = (RK_U32)get_cmd_val(RK_APP_PARAM_OFFSET, 16);
		appVirAddr = metaVirmem + app_param_offset;
		g_pAppParam = (struct app_param_info *)(appVirAddr);
		switch (g_pAppParam->venc_type) {
		case 1:
			rk_param_set_string(entry, "H.264");
			break;
		case 2:
		default:
			rk_param_set_string(entry, "H.265");
			break;
		}
	} else {
		LOG_ERROR("mmap fail.\n");
		return -1;
	}
}

int rk_video_init() {
	LOG_DEBUG("begin\n");
	int ret = 0;
	char *pOutPath = "/tmp/venc-test.bin";
	if (pOutPath) {
		venc0_file = fopen(pOutPath, "w");
		if (!venc0_file) {
			LOG_ERROR("open venc0 file failed! cant record first 30 frame!\n");
		}
	}
	enable_pp = rk_param_get_int("video.source:enable_pp", 1);
	enable_jpeg = rk_param_get_int("video.source:enable_jpeg", 1);
	enable_venc_0 = rk_param_get_int("video.source:enable_venc_0", 1);
	enable_venc_1 = rk_param_get_int("video.source:enable_venc_1", 1);
	enable_rtsp = rk_param_get_int("video.source:enable_rtsp", 1);
	enable_rtmp = rk_param_get_int("video.source:enable_rtmp", 1);
	LOG_INFO("enable_jpeg is %d, enable_venc_0 is %d, enable_venc_1 is %d, enable_rtsp is %d, "
	         "enable_rtmp is %d\n",
	         enable_jpeg, enable_venc_0, enable_venc_1, enable_rtsp, enable_rtmp);

	g_vi_chn_id = rk_param_get_int("video.source:vi_chn_id", 0);
	g_enable_vo = rk_param_get_int("video.source:enable_vo", 1);
	g_vo_dev_id = rk_param_get_int("video.source:vo_dev_id", 3);
	enable_npu = rk_param_get_int("video.source:enable_npu", 0);
	enable_wrap = rk_param_get_int("video.source:enable_wrap", 0);
	enable_osd = rk_param_get_int("osd.common:enable_osd", 0);
	LOG_DEBUG("g_vi_chn_id is %d, g_enable_vo is %d, g_vo_dev_id is %d, enable_npu is %d, "
	          "enable_wrap is %d, enable_osd is %d\n",
	          g_vi_chn_id, g_enable_vo, g_vo_dev_id, enable_npu, enable_wrap, enable_osd);
	g_video_run_ = 1;
	g_nn_osd_run_ = 1;
	// ret |= rkipc_vi_dev_init(); //跨进程已在server初始化
	if (enable_venc_0)
		ret |= rkipc_pipe_0_init();
	if (enable_venc_1)
		ret |= rkipc_pipe_1_init();
	if (enable_jpeg)
		ret |= rkipc_pipe_jpeg_init();
	// if (g_enable_vo)
	// 	ret |= rkipc_pipe_vpss_vo_init();
	if (enable_pp)
		ret |= rkipc_pipe_3_init();
	rk_roi_set_callback_register(rk_roi_set);
	ret |= rk_roi_set_all();
	// rk_region_clip_set_callback_register(rk_region_clip_set);
	// rk_region_clip_set_all();
	if (enable_npu) {
		// ret |= rkipc_vpss_bgr_init();
		ret |= rkipc_pipe_2_init();
		ret |= rkipc_osd_draw_nn_init();
	}
	// The osd dma buffer must be placed in the last application,
	// otherwise, when the font size is switched, holes may be caused
	if (enable_osd)
		ret |= rkipc_osd_init();
	if (enable_rtsp)
		ret |= rkipc_rtsp_init();
	sleep(3); // need wait S50nginx
	if (enable_rtmp)
		ret |= rkipc_rtmp_init();
	LOG_DEBUG("over\n");

	return ret;
}

int rk_video_deinit() {
	LOG_DEBUG("%s\n", __func__);
	g_video_run_ = 0;
	g_nn_osd_run_ = 0;
	int ret = 0;
	if (enable_npu) {
		ret |= rkipc_osd_draw_nn_deinit();
		// ret |= rkipc_vpss_bgr_deinit();
		ret |= rkipc_pipe_2_deinit();
	}
	// rk_region_clip_set_callback_register(NULL);
	rk_roi_set_callback_register(NULL);
	if (enable_osd)
		ret |= rkipc_osd_deinit();
	// if (g_enable_vo)
	// 	ret |= rkipc_pipe_vi_vo_deinit();
	if (enable_venc_0) {
		pthread_join(venc_thread_0, NULL);
		ret |= rkipc_pipe_0_deinit();
	}
	if (enable_venc_1) {
		pthread_join(venc_thread_1, NULL);
		ret |= rkipc_pipe_1_deinit();
	}
	if (enable_jpeg) {
		if (rk_param_get_int("video.jpeg:enable_cycle_snapshot", 0)) {
			cycle_snapshot_flag = 0;
			pthread_join(cycle_snapshot_thread_id, NULL);
		}
		pthread_join(jpeg_venc_thread_id, NULL);
		ret |= rkipc_pipe_jpeg_deinit();
	}
	if (enable_pp) {
		ret |= rkipc_pipe_3_deinit();
	}
	// ret |= rkipc_vi_dev_deinit();
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
	if (rk_param_get_int("video.source:enable_aiq", 1)) {
		ret |= rk_isp_fastboot_deinit(0);
		ret |= rk_isp_fastboot_init(0);
	}
	ret |= rk_video_init();
	if (rk_param_get_int("audio.0:enable", 0))
		rkipc_audio_rtsp_init();
	ret |= rk_storage_init();

	return ret;
}
