// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "video.h"

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
#define VPSS_BGR 0

#define RV1126_VO_DEV_MIPI 0
#define RV1126_VOP_LAYER_CLUSTER0 0

#define RTSP_URL_0 "/live/0"
#define RTSP_URL_1 "/live/1"

int rkbar_capture_one = 0;
int rkbar_retry_time = 0;
static int g_video_run_ = 1;
static int pipe_id_ = 0;
static int dev_id_ = 0;
static int g_rtmp_start = 0;
static int enable_npu;
static const char *tmp_output_data_type = "H.264";
static const char *g_rc_mode;
static const char *g_h264_profile;
static const char *g_smart;
static pthread_t venc_thread_id, vi_thread_1, vpss_thread_rgb;

MPP_CHN_S pipe_0_vi_chn, pipe_0_venc_chn;
MPP_CHN_S pipe_1_vi_chn, pipe_1_venc_chn, vpss_rotate_chn, pipe_1_vo_chn, vpss_bgr_chn;

static VO_DEV VoLayer = RV1126_VOP_LAYER_CLUSTER0;

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

static void *rkipc_get_vi_1(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "rkipc_get_vi_1", 0, 0, 0);
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

static void *rkipc_get_vpss_bgr(void *arg) {
	LOG_DEBUG("#Start %s thread, arg:%p\n", __func__, arg);
	prctl(PR_SET_NAME, "rkipc_get_vpss_bgr", 0, 0, 0);
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
			uint8_t *phy_addr = (uint8_t *)RK_MPI_MB_Handle2PhysAddr(frame.stVFrame.pMbBlk);
			// long long last_nn_time = rkipc_get_curren_time_ms();
			rkipc_rockiva_write_nv12_frame_by_phy_addr(
			    frame.stVFrame.u32Width, frame.stVFrame.u32Height, loopCount, phy_addr);
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

static void *rkipc_get_venc_0(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	VENC_STREAM_S stFrame;
	VI_CHN_STATUS_S stChnStatus;
	int loopCount = 0;
	int ret = 0;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));
	FILE *fp = fopen("/tmp/test.h265", "wb");

	while (g_video_run_) {
		// 5.get the frame
		ret = RK_MPI_VENC_GetStream(VIDEO_PIPE_0, &stFrame, 1000);
		if (ret == RK_SUCCESS) {
			void *data = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
			if (loopCount < 50) {
				fwrite(data, 1, stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}
			rkipc_rtsp_write_video_frame(0, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
			if (rk_param_get_int("tuya:enable", 0)) {
				if ((stFrame.pstPack->DataType.enH264EType == H264E_NALU_IDRSLICE) ||
				    (stFrame.pstPack->DataType.enH264EType == H264E_NALU_ISLICE) ||
				    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_IDRSLICE) ||
				    (stFrame.pstPack->DataType.enH265EType == H265E_NALU_ISLICE)) {
					rk_tuya_push_video(data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS, 1);
				} else {
					rk_tuya_push_video(data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS, 0);
				}
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
	if (fp)
		fclose(fp);

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
			rkipc_rtsp_write_video_frame(1, data, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
			// 7.release the frame
			ret = RK_MPI_VENC_ReleaseStream(VIDEO_PIPE_1, &stFrame);
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

int rkipc_pipe_0_vi_init() {
	int video_width = rk_param_get_int("video.0:width", 1920);
	int video_height = rk_param_get_int("video.0:height", 1080);
	const char *video_device_name = rk_param_get_string("video.0:src_node", "rkisp_mainpath");
	int buf_cnt = 1;
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

	VI_CHN_BUF_WRAP_S stViWrap;
	memset(&stViWrap, 0, sizeof(VI_CHN_BUF_WRAP_S));
	if (rk_param_get_int("video.0:enable_wrap", 0)) {
		int buffer_line = rk_param_get_int("video.0:buffer_line", video_height / 4);
		if (buffer_line < 64 || buffer_line > video_height) {
			LOG_ERROR("wrap mode buffer line must between [64, H]\n");
			return -1;
		}
		stViWrap.bEnable = 1;
		stViWrap.u32BufLine = buffer_line;
		stViWrap.u32WrapBufferSize = stViWrap.u32BufLine * video_width * 3 / 2; // NV12
		LOG_INFO("set vi channel wrap line: %d, wrapBuffSize = %d\n", stViWrap.u32BufLine,
		         stViWrap.u32WrapBufferSize);
		RK_MPI_VI_SetChnWrapBufAttr(pipe_id_, VIDEO_PIPE_0, &stViWrap);
	}

	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_0);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%d\n", ret);
		return -1;
	}
	// pthread_t thread_id;
	// pthread_create(&thread_id, NULL, rkipc_get_vi_0, NULL);

	return 0;
}

int rkipc_pipe_0_venc_init() {
	int ret;
	int video_width = rk_param_get_int("video.0:width", 1920);
	int video_height = rk_param_get_int("video.0:height", 1080);
	// VENC init
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	tmp_output_data_type = rk_param_get_string("video.0:output_data_type", NULL);
	g_rc_mode = rk_param_get_string("video.0:rc_mode", NULL);
	g_h264_profile = rk_param_get_string("video.0:h264_profile", NULL);
	if ((tmp_output_data_type == NULL) || (g_rc_mode == NULL)) {
		LOG_ERROR("tmp_output_data_type or g_rc_mode is NULL\n");
		return -1;
	}
	LOG_INFO("tmp_output_data_type is %s, g_rc_mode is %s, g_h264_profile is %s\n",
	         tmp_output_data_type, g_rc_mode, g_h264_profile);
	if (!strcmp(tmp_output_data_type, "H.264")) {
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
	} else if (!strcmp(tmp_output_data_type, "H.265")) {
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
		LOG_ERROR("tmp_output_data_type is %s, not support\n", tmp_output_data_type);
		return -1;
	}
	g_smart = rk_param_get_string("video.0:smart", NULL);
	if (!strcmp(g_smart, "open")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		venc_chn_attr.stGopAttr.s32VirIdrLen = venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum /
		                                       venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen;
	} else {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.0:gop", -1);

	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	venc_chn_attr.stVencAttr.u32StreamBufCnt = rk_param_get_int("video.0:buffer_count", 4);
	venc_chn_attr.stVencAttr.u32BufSize = rk_param_get_int("video.0:buffer_size", 1843200);
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_0, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%d\n", ret);
		return -1;
	}

	VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
	memset(&stVencChnBufWrap, 0, sizeof(stVencChnBufWrap));
	if (rk_param_get_int("video.0:enable_wrap", 0)) {
		stVencChnBufWrap.bEnable = 1;
		stVencChnBufWrap.u32BufLine = rk_param_get_int("video.0:buffer_line", video_height / 4);
		RK_MPI_VENC_SetChnBufWrapAttr(VIDEO_PIPE_0, &stVencChnBufWrap);
	}

	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
	memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
	stVencChnRefBufShare.bEnable = rk_param_get_int("video.0:enable_refer_buffer_share", 1);
	RK_MPI_VENC_SetChnRefBufShareAttr(VIDEO_PIPE_0, &stVencChnRefBufShare);

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_0, &stRecvParam);

	return 0;
}

int rkipc_pipe_0_bind() {
	int ret;
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
	pthread_create(&venc_thread_id, NULL, rkipc_get_venc_0, NULL);

	return 0;
}

int rkipc_pipe_0_init() {
	rkipc_pipe_0_vi_init();
	rkipc_pipe_0_venc_init();
	rkipc_pipe_0_bind();

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
	int video_width = rk_param_get_int("video.1:width", 640);
	int video_height = rk_param_get_int("video.1:height", 480);
	const char *video_device_name = rk_param_get_string("video.1:src_node", "rkisp_selfpath");
	int buf_cnt = 1;
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
	vi_chn_attr.u32Depth = 1;
	if (enable_npu)
		vi_chn_attr.u32Depth += 1;
	ret = RK_MPI_VI_SetChnAttr(pipe_id_, VIDEO_PIPE_1, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(pipe_id_, VIDEO_PIPE_1);
	if (ret) {
		LOG_ERROR("ERROR: create VI error! ret=%d\n", ret);
		return -1;
	}

	// VENC init
	VENC_CHN_ATTR_S venc_chn_attr;
	memset(&venc_chn_attr, 0, sizeof(venc_chn_attr));
	tmp_output_data_type = rk_param_get_string("video.1:output_data_type", NULL);
	g_rc_mode = rk_param_get_string("video.1:rc_mode", NULL);
	g_h264_profile = rk_param_get_string("video.1:h264_profile", NULL);
	if ((tmp_output_data_type == NULL) || (g_rc_mode == NULL)) {
		LOG_ERROR("tmp_output_data_type or g_rc_mode is NULL\n");
		return -1;
	}
	LOG_INFO("tmp_output_data_type is %s, g_rc_mode is %s, g_h264_profile is %s\n",
	         tmp_output_data_type, g_rc_mode, g_h264_profile);
	if (!strcmp(tmp_output_data_type, "H.264")) {
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
		if (!strcmp(g_rc_mode, "CBR")) {
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
	g_smart = rk_param_get_string("video.1:smart", NULL);
	if (!strcmp(g_smart, "open")) {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_SMARTP;
		venc_chn_attr.stGopAttr.s32VirIdrLen = venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateNum /
		                                       venc_chn_attr.stRcAttr.stH265Vbr.u32SrcFrameRateDen;
	} else {
		venc_chn_attr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	}
	// venc_chn_attr.stGopAttr.u32GopSize = rk_param_get_int("video.1:gop", -1);

	venc_chn_attr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	venc_chn_attr.stVencAttr.u32PicWidth = video_width;
	venc_chn_attr.stVencAttr.u32PicHeight = video_height;
	venc_chn_attr.stVencAttr.u32VirWidth = video_width;
	venc_chn_attr.stVencAttr.u32VirHeight = video_height;
	venc_chn_attr.stVencAttr.u32StreamBufCnt = rk_param_get_int("video.1:buffer_count", 4);
	venc_chn_attr.stVencAttr.u32BufSize = rk_param_get_int("video.1:buffer_size", 1843200);
	// venc_chn_attr.stVencAttr.u32Depth = 1;
	ret = RK_MPI_VENC_CreateChn(VIDEO_PIPE_1, &venc_chn_attr);
	if (ret) {
		LOG_ERROR("ERROR: create VENC error! ret=%d\n", ret);
		return -1;
	}

	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
	memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
	stVencChnRefBufShare.bEnable = rk_param_get_int("video.1:enable_refer_buffer_share", 1);
	RK_MPI_VENC_SetChnRefBufShareAttr(VIDEO_PIPE_1, &stVencChnRefBufShare);

	VENC_RECV_PIC_PARAM_S stRecvParam;
	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(VIDEO_PIPE_1, &stRecvParam);

	pthread_create(&venc_thread_id, NULL, rkipc_get_venc_1, NULL);
	if (enable_npu) {
		// vi-draw-venc
		pthread_create(&vi_thread_1, NULL, rkipc_get_vi_1, NULL);
	} else {
		// bind
		pipe_1_venc_chn.enModId = RK_ID_VENC;
		pipe_1_venc_chn.s32DevId = pipe_id_;
		pipe_1_venc_chn.s32ChnId = VIDEO_PIPE_1;
		pipe_1_vi_chn.enModId = RK_ID_VI;
		pipe_1_vi_chn.s32DevId = pipe_id_;
		pipe_1_vi_chn.s32ChnId = VIDEO_PIPE_1;
		ret = RK_MPI_SYS_Bind(&pipe_1_vi_chn, &pipe_1_venc_chn);
		if (ret) {
			LOG_ERROR("ERROR: Bind VI and VENC error! ret=%d\n", ret);
			return -1;
		}
	}

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
	ret = RK_MPI_VENC_StopRecvFrame(VIDEO_PIPE_1);
	ret |= RK_MPI_VENC_DestroyChn(VIDEO_PIPE_1);
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
	stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = rk_param_get_int("isp.0.adjustment:fps", 15);
	stVpssChnAttr[0].stFrameRate.s32DstFrameRate = rk_param_get_int("video.1:npu_fps", 3);
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
	pipe_1_vi_chn.enModId = RK_ID_VI;
	pipe_1_vi_chn.s32DevId = 0;
	pipe_1_vi_chn.s32ChnId = VIDEO_PIPE_1;
	vpss_bgr_chn.enModId = RK_ID_VPSS;
	vpss_bgr_chn.s32DevId = VPSS_BGR;
	vpss_bgr_chn.s32ChnId = 0;
	ret = RK_MPI_SYS_Bind(&pipe_1_vi_chn, &vpss_bgr_chn);
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
	pipe_1_vi_chn.enModId = RK_ID_VI;
	pipe_1_vi_chn.s32DevId = 0;
	pipe_1_vi_chn.s32ChnId = VIDEO_PIPE_1;
	vpss_bgr_chn.enModId = RK_ID_VPSS;
	vpss_bgr_chn.s32DevId = VPSS_BGR;
	vpss_bgr_chn.s32ChnId = 0;
	ret = RK_MPI_SYS_UnBind(&pipe_1_vi_chn, &vpss_bgr_chn);
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

static void *wait_key_event(void *arg) {

	while (g_video_run_) {
		// RECOVERY KEY at ADCIN0: iio 0, channel 0; range (0 - 1023)
		int val = rk_adc_get_value(0, 0);
		if (val < 100) {
			LOG_INFO("start capture\n");
			rkbar_capture_one = 1;
			rkbar_retry_time = 30;
		}
		usleep(100 * 1000);
	}

	LOG_DEBUG("wait key event out\n");
	return NULL;
}

static void *get_vi_send_rkbar(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	VIDEO_FRAME_INFO_S vi_frame;
	VI_CHN_STATUS_S stChnStatus;
	int ret = 0;
	char file_name[256];

	while (g_video_run_) {
		if (rkbar_capture_one == 0) {
			usleep(300 * 1000);
			continue;
		}
		rkbar_capture_one = 0;
		// rkbar
		image_t *img = NULL;
		img = (image_t *)malloc(sizeof(image_t));
		img->width = rk_param_get_int("video.1:width", -1);
		img->height = rk_param_get_int("video.1:height", -1);
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
		while (rkbar_retry_time) {
			LOG_DEBUG("rkbar_retry_time is %d\n", rkbar_retry_time);
			rkbar_retry_time--;
			ret = RK_MPI_VI_GetChnFrame(pipe_id_, VIDEO_PIPE_1, &vi_frame, 1000);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VI_GetChnFrame timeout %#x\n", ret);
				continue;
			}
			void *buffer = RK_MPI_MB_Handle2VirAddr(vi_frame.stVFrame.pMbBlk);
			LOG_DEBUG("RK_MPI_VI_GetChnFrame ok:data %p pts:%" PRId64 " ms\n", buffer,
			          vi_frame.stVFrame.u64PTS / 1000);
			img->data = (uint8_t *)buffer;
			ret = rkbar_scan(rkbar_hand, img);
			if (ret <= 0) {
				LOG_DEBUG("scan fail\n");
				ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_1, &vi_frame);
				if (ret != RK_SUCCESS) {
					LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
				}
				continue;
			}
			ret = RK_MPI_VI_ReleaseChnFrame(pipe_id_, VIDEO_PIPE_1, &vi_frame);
			if (ret != RK_SUCCESS) {
				LOG_ERROR("RK_MPI_VI_ReleaseChnFrame fail %x\n", ret);
			}
			LOG_INFO("scan success\n");
			rkbar_retry_time = 0;
			const char *test = rkbar_getresult(rkbar_hand);
			char *data = (char *)malloc(strlen(test));
			memcpy(data, test, strlen(test));
			LOG_INFO("rkbar the decoding result is \" %s \" \n", data);
			system("aplay /etc/qr_recognized.wav &");

			tuya_ipc_direct_connect(data, 0);
			// connect wifi
			char ssid[64], psk[64], cmd[128], wifi_ssid[64], wifi_psk[64];
			sscanf(data, "%*[^:]:%[^,],%*[^:]:%[^,]", psk, ssid);
			memset(wifi_ssid, 0, 64);
			memset(wifi_psk, 0, 64);
			memcpy(wifi_ssid, &ssid[1], strlen(ssid) - 2);
			memcpy(wifi_psk, &psk[1], strlen(psk) - 2);
			LOG_INFO("wifi_ssid is %s, wifi_psk is %s\n", wifi_ssid, wifi_psk);
			enable_network(wifi_ssid, wifi_psk);

			if (data)
				free(data);
		}
		// rkbar_retry_time = 0, or connect success
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
	enable_npu = rk_param_get_int("video.1:enable_npu", 0);
	// ret |= rkipc_vi_dev_init();
	// ret |= rkipc_pipe_0_init();
	ret |= rkipc_pipe_1_init();
	ret |= rkipc_rtsp_init(RTSP_URL_0, RTSP_URL_1, NULL);
// 	if (enable_npu)
// 		ret |= rkipc_vpss_bgr_init();
#if 1
	// pthread_t key_id;
	pthread_t get_vi_id;
	// pthread_create(&key_id, NULL, wait_key_event, NULL);
	pthread_create(&get_vi_id, NULL, get_vi_send_rkbar, NULL);
#endif

	return ret;
}

int rk_video_deinit() {
	LOG_INFO("%s\n", __func__);
	g_video_run_ = 0;
	pthread_join(venc_thread_id, NULL);
	int ret = 0;
	if (enable_npu)
		ret |= rkipc_vpss_bgr_deinit();
	ret |= rkipc_rtsp_deinit();
	ret |= rkipc_pipe_0_deinit();
	ret |= rkipc_pipe_1_deinit();
	ret |= rkipc_vi_dev_deinit();

	return ret;
}
