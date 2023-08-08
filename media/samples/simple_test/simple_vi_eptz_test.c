/*
 * Copyright (c) 2022 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "rtsp_demo.h"
#include "sample_comm.h"

#define STEP_COUNT 10
#define PORT_NUMBER 554

static RK_S32 g_s32FrameCnt = -1;
static RK_U32 g_u32Bitrate = 10 * 1024;
static bool quit = false;
static RK_U32 vi_eptz_w[8] = {1920, 1280, 960, 720, 640, 512, 480, 320};
static RK_U32 vi_eptz_h[8] = {1080, 720, 540, 576, 480, 288, 320, 240};
//#define RK_ARRAY_ELEMS(a)      (sizeof(a) / sizeof((a)[0]))

rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session;

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_S32 vi_eptz(RK_U32 loopCount, RK_U32 srcWidth, RK_U32 srcHeight,
                      RK_U32 dstWidth, RK_U32 dstHeight) {
	RK_U32 x = 0;
	RK_U32 y = 0;
	RK_U32 w = 0;
	RK_U32 h = 0;
	RK_U32 i = 0;
	RK_U32 width = 0;
	RK_U32 height = 0;
	RK_U32 xStride = 0;
	RK_U32 yStride = 0;
	RK_BOOL bDoCrop = RK_FALSE;
	RK_BOOL bEptz = RK_FALSE;
	VI_CROP_INFO_S stCropInfo;
	memset(&stCropInfo, 0, sizeof(VI_CROP_INFO_S));
	stCropInfo.enCropCoordinate = VI_CROP_ABS_COOR;
	stCropInfo.bEnable = RK_TRUE;
	if (srcWidth >= dstWidth) {
		xStride = (srcWidth - dstWidth) / STEP_COUNT / 2;
		yStride = (srcHeight - dstHeight) / STEP_COUNT / 2;
		width = srcWidth;
		height = srcHeight;
	} else {
		xStride = (dstWidth - srcWidth) / STEP_COUNT / 2;
		yStride = (dstHeight - srcHeight) / STEP_COUNT / 2;
		width = dstWidth;
		height = dstHeight;
	}
	for (i = 0; i < STEP_COUNT * 2; ++i) {
		if ((0 <= i) && (i <= STEP_COUNT)) {
			if (loopCount % (STEP_COUNT * 2) == i) {
				x = xStride * i;
				y = yStride * i;
				w = width - x * 2;
				h = height - y * 2;
				bEptz = RK_TRUE;
				bDoCrop = RK_TRUE;
			}
		} else if ((STEP_COUNT < i) && (i < STEP_COUNT * 2)) {
			if (loopCount % (STEP_COUNT * 2) == i) {
				x = xStride * (STEP_COUNT * 2 - i);
				y = yStride * (STEP_COUNT * 2 - i);
				w = width - x * 2;
				h = height - y * 2;
				bEptz = RK_TRUE;
				bDoCrop = RK_TRUE;
			}
		} else {
			RK_LOGE("loopCount:%d", loopCount);
		}
	}
	if (bDoCrop) {
		stCropInfo.stCropRect.s32X = x;
		stCropInfo.stCropRect.s32Y = y;
		stCropInfo.stCropRect.u32Width = w;
		stCropInfo.stCropRect.u32Height = h;
		if (bEptz) {
			RK_MPI_VI_SetEptz(0, 0, stCropInfo);
		}
		bEptz = RK_FALSE;
		bDoCrop = RK_FALSE;
	}
	return 0;
}

static void *GetMediaBuffer(void *arg) {
	(void)arg;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;
	RK_S32 s32Ret = RK_SUCCESS;
	RK_BOOL bAttachPool = RK_TRUE;
	VI_CROP_INFO_S stCropInfo;
	memset(&stCropInfo, 0, sizeof(VI_CROP_INFO_S));
	stCropInfo.bEnable = RK_TRUE;
	stCropInfo.enCropCoordinate = VI_CROP_ABS_COOR;

	VENC_STREAM_S stFrame;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(0, &stFrame, 200000);
		if (s32Ret == RK_SUCCESS) {
			if (g_rtsplive && g_rtsp_session) {
				pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
				rtsp_tx_video(g_rtsp_session, pData, stFrame.pstPack->u32Len,
				              stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
			}

			s32Ret = RK_MPI_VENC_ReleaseStream(0, &stFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
				return RK_FAILURE;
			}
			loopCount++;
			vi_eptz(loopCount, vi_eptz_w[0], vi_eptz_h[0], vi_eptz_w[1], vi_eptz_h[1]);
		} else {
			RK_LOGE("RK_MPI_VENC_GetStream timeout", s32Ret);
			return RK_FAILURE;
		}
		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
			quit = true;
			break;
		}
	}
	// restore crops setting as beginning resolution, avoiding getting video frame fail
	// when run again.
	RK_MPI_VI_GetEptz(0, 0, &stCropInfo);
	if (stCropInfo.stCropRect.u32Width != vi_eptz_w[0] ||
	    stCropInfo.stCropRect.u32Height != vi_eptz_h[0]) {
		stCropInfo.stCropRect.u32Width = vi_eptz_w[0];
		stCropInfo.stCropRect.u32Height = vi_eptz_h[0];
		RK_MPI_VI_SetEptz(0, 0, stCropInfo);
	}

FAILED:
	if (bAttachPool) {
		RK_MPI_VI_DetachMbPool(0, 0);
		RK_MPI_MB_DestroyPool(bAttachPool);
		bAttachPool = RK_FALSE;
	}
	free(stFrame.pstPack);
	return NULL;
}

static RK_S32 test_venc_init(RK_U32 chnId, RK_U32 width, RK_U32 height,
                             RK_CODEC_ID_E enType) {
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_ATTR_S stAttr;
	memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));

	stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
	stAttr.stRcAttr.stH264Cbr.u32BitRate = g_u32Bitrate;
	stAttr.stRcAttr.stH264Cbr.u32Gop = 1;

	stAttr.stVencAttr.enType = enType;
	stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
	stAttr.stVencAttr.u32PicWidth = width;
	stAttr.stVencAttr.u32PicHeight = height;
	stAttr.stVencAttr.u32VirWidth = width;
	stAttr.stVencAttr.u32VirHeight = height;
	stAttr.stVencAttr.u32StreamBufCnt = 2;
	stAttr.stVencAttr.u32BufSize = width * height * 3 / 2;
	stAttr.stVencAttr.enMirror = MIRROR_NONE;

	RK_MPI_VENC_CreateChn(chnId, &stAttr);

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

static RK_S32 vi_dev_init() {
	RK_LOGI("%s\n", __func__);
	RK_S32 s32Ret = RK_SUCCESS;
	RK_S32 devId = 0;
	RK_S32 pipeId = 0;

	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	memset(&stDevAttr, 0, sizeof(stDevAttr));
	memset(&stBindPipe, 0, sizeof(stBindPipe));
	// 0. get dev config status
	s32Ret = RK_MPI_VI_GetDevAttr(devId, &stDevAttr);
	if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
		// 0-1.config dev
		s32Ret = RK_MPI_VI_SetDevAttr(devId, &stDevAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetDevAttr %x\n", s32Ret);
			return RK_FAILURE;
		}
	} else {
		RK_LOGE("RK_MPI_VI_SetDevAttr already\n");
	}
	// 1.get dev enable status
	s32Ret = RK_MPI_VI_GetDevIsEnable(devId);
	if (s32Ret != RK_SUCCESS) {
		// 1-2.enable dev
		s32Ret = RK_MPI_VI_EnableDev(devId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_EnableDev %x\n", s32Ret);
			return RK_FAILURE;
		}
		// 1-3.bind dev/pipe
		stBindPipe.u32Num = pipeId;
		stBindPipe.PipeId[0] = pipeId;
		s32Ret = RK_MPI_VI_SetDevBindPipe(devId, &stBindPipe);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetDevBindPipe %x\n", s32Ret);
			return RK_FAILURE;
		}
	} else {
		RK_LOGE("RK_MPI_VI_EnableDev already\n");
		return RK_FAILURE;
	}

	return 0;
}

static RK_S32 vi_chn_init(RK_U32 channelId, RK_U32 width, RK_U32 height) {
	RK_S32 s32Ret = RK_SUCCESS;
	RK_S32 buf_cnt = 2;
	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType =
	    VI_V4L2_MEMORY_TYPE_DMABUF; // VI_V4L2_MEMORY_TYPE_MMAP;
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;
	vi_chn_attr.u32Depth = 2;
	s32Ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);
	s32Ret |= RK_MPI_VI_EnableChn(0, channelId);
	if (s32Ret) {
		RK_LOGE("ERROR: create VI error! ret=%d\n", s32Ret);
		return s32Ret;
	}

	return s32Ret;
}

static RK_CHAR optstr[] = "?::a::w:h:c:I:e:";
static void print_usage(const RK_CHAR *name) {
	printf("Usage example:\n");
	printf("\t%s -I 0 -w 1920 -h 1080 (rtsp://ip/live/0)\n", name);
	printf("\t-w | --width: VI width, Default:1920\n");
	printf("\t-h | --heght: VI height, Default:1080\n");
	printf("\t-a | --aiq: iq file path, Default:/etc/iqfiles\n");
	printf("\t-c | --frame_cnt: frame number of output, Default:150\n");
	printf("\t-I | --camid: camera ctx id, Default 0. "
	       "0:rkisp_mainpath,1:rkisp_selfpath,2:rkisp_bypasspath\n");
	printf("\t-e | --encode: encode type, Default:h264, Value:h264, h265\n");
}

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_SUCCESS;
	RK_U32 u32Width = 1920;
	RK_U32 u32Height = 1080;
	RK_CHAR *pOutPath = NULL;
	RK_CODEC_ID_E enCodecType = RK_VIDEO_ID_AVC;
	RK_CHAR *pCodecName = "H264";
	RK_S32 s32chnlId = 0;
	char *iq_dir = "/etc/iqfiles";
	RK_S32 c;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'a':
			if (optarg)
				iq_dir = optarg;
			break;
		case 'w':
			u32Width = atoi(optarg);
			break;
		case 'h':
			u32Height = atoi(optarg);
			break;
		case 'I':
			s32chnlId = atoi(optarg);
			break;
		case 'c':
			g_s32FrameCnt = atoi(optarg);
			break;
		case 'e':
			if (!strcmp(optarg, "h264")) {
				enCodecType = RK_VIDEO_ID_AVC;
				pCodecName = "H264";
			} else if (!strcmp(optarg, "h265")) {
				enCodecType = RK_VIDEO_ID_HEVC;
				pCodecName = "H265";
			} else {
				RK_LOGE("ERROR: Invalid encoder type.\n");
				return 0;
			}
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	RK_LOGW("#CodecName:%s\n", pCodecName);
	RK_LOGW("#Resolution: %dx%d\n", u32Width, u32Height);
	RK_LOGW("#CameraIdx: %d\n\n", s32chnlId);
	RK_LOGW("#Frame Count to save: %d\n", g_s32FrameCnt);

	signal(SIGINT, sigterm_handler);

	if (iq_dir) {
#ifdef RKAIQ
		RK_LOGW("ISP IQ file path: %s\n\n", iq_dir);
		SAMPLE_COMM_ISP_Init(0, RK_AIQ_WORKING_MODE_NORMAL, 0, iq_dir);
		SAMPLE_COMM_ISP_Run(0);
		// SAMPLE_COMM_ISP_SetFrameRate(0, fps);
#endif
	}

	// init rtsp
	g_rtsplive = create_rtsp_demo(PORT_NUMBER);
	if (g_rtsplive == NULL) {
		RK_LOGE("rtsp create session fail\n");
		goto __FAILED;
	}
	g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/0");
	if (g_rtsp_session == NULL) {
		RK_LOGE("rtsp create session fail\n");
		goto __FAILED;
	}
	if (enCodecType == RK_VIDEO_ID_AVC) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	} else if (enCodecType == RK_VIDEO_ID_HEVC) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	} else {
		RK_LOGE("not support other type\n");
		return -1;
	}

	rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(), rtsp_get_ntptime());

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("rk mpi sys init fail!");
		goto __FAILED;
	}

	vi_dev_init();
	vi_chn_init(0, u32Width, u32Height);
	// venc  init
	test_venc_init(0, u32Width, u32Height,
	               enCodecType); // RK_VIDEO_ID_AVC RK_VIDEO_ID_HEVC

	MPP_CHN_S stSrcChn, stDestChn;
	// bind vi to venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 0;

	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("bind 0 ch venc failed");
		goto __FAILED;
	}
	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer, NULL);

	while (!quit) {
		usleep(500000);
	}
	pthread_join(&main_thread, NULL);

	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);

	s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
		goto __FAILED;
	}

	s32Ret = RK_MPI_VI_DisableChn(0, 0);
	RK_LOGW("RK_MPI_VI_DisableChn %x", s32Ret);

	s32Ret = RK_MPI_VENC_StopRecvFrame(0);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_StopRecvFrame 0 fail %x", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_VENC_DestroyChn(0);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
		return RK_FAILURE;
	}

	s32Ret = RK_MPI_VI_DisableDev(0);
	RK_LOGW("RK_MPI_VI_DisableDev %x", s32Ret);

__FAILED:
	RK_LOGE("test running exit:%d", s32Ret);
	RK_MPI_SYS_Exit();

#ifdef RKAIQ
	SAMPLE_COMM_ISP_Stop(0);
#endif

	return 0;
}
