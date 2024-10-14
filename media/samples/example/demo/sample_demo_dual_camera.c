
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "rockiva/rockiva_ba_api.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <time.h>
#include <unistd.h>

#include "rtsp_demo.h"
#include "sample_comm.h"
#include <stdatomic.h>

pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
static rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session_0, g_rtsp_session_1, g_rtsp_session_2,
    g_rtsp_session_3;
static int rociva_run_flag = 0;
static RockIvaHandle rkba_handle;
static RockIvaBaTaskParams initParams;
static RockIvaInitParam globalParams;
typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[4]; // camera 0: 0,1; camera 1: 2,3
	SAMPLE_VENC_CTX_S venc[4];
} SAMPLE_MPI_CTX_S;

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_CHAR optstr[] = "?::r:f:W:H:w:h:s:n:b:";
static const struct option long_options[] = {
    {"hdr", required_argument, NULL, 'r'},
    {"fps", required_argument, NULL, 'f'},
    {"main_width", required_argument, NULL, 'W'},
    {"main_height", required_argument, NULL, 'H'},
    {"sub_width", required_argument, NULL, 'w'},
    {"sub_height", required_argument, NULL, 'h'},
    {"sensorid", required_argument, NULL, 's'},
    {"enable_npu", required_argument, NULL, 'n'},
    {"buf_share", required_argument, NULL, 'b'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : venc thread
 ******************************************************************************/
static void *venc0_get_stream(void *pArgs) {
	printf("#Start %s , arg:%p\n", __func__, pArgs);
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	// char name[256] = {0};
	// FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;

	while (!quit) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			RK_LOGD("chn:%d, loopCount:%d wd:%d\n", ctx->s32ChnId, loopCount,
			        ctx->stFrame.pstPack->u32Len);
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					quit = true;
					break;
				}
			}

			PrintStreamDetails(ctx->s32ChnId, ctx->stFrame.pstPack->u32Len);
			pthread_mutex_lock(&g_rtsp_mutex);
			rtsp_tx_video(g_rtsp_session_0, pData, ctx->stFrame.pstPack->u32Len,
			              ctx->stFrame.pstPack->u64PTS);
			rtsp_do_event(g_rtsplive);
			pthread_mutex_unlock(&g_rtsp_mutex);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	return RK_NULL;
}

static void *venc1_get_stream(void *pArgs) {
	printf("#Start %s , arg:%p\n", __func__, pArgs);
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	// char name[256] = {0};
	// FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;

	while (!quit) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			RK_LOGD("chn:%d, loopCount:%d wd:%d\n", ctx->s32ChnId, loopCount,
			        ctx->stFrame.pstPack->u32Len);
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					quit = true;
					break;
				}
			}

			PrintStreamDetails(ctx->s32ChnId, ctx->stFrame.pstPack->u32Len);
			pthread_mutex_lock(&g_rtsp_mutex);
			rtsp_tx_video(g_rtsp_session_1, pData, ctx->stFrame.pstPack->u32Len,
			              ctx->stFrame.pstPack->u64PTS);
			rtsp_do_event(g_rtsplive);
			pthread_mutex_unlock(&g_rtsp_mutex);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	return RK_NULL;
}

static void *venc2_get_stream(void *pArgs) {
	printf("#Start %s , arg:%p\n", __func__, pArgs);
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	// char name[256] = {0};
	// FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;

	while (!quit) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			RK_LOGD("chn:%d, loopCount:%d wd:%d\n", ctx->s32ChnId, loopCount,
			        ctx->stFrame.pstPack->u32Len);
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					quit = true;
					break;
				}
			}

			PrintStreamDetails(ctx->s32ChnId, ctx->stFrame.pstPack->u32Len);
			pthread_mutex_lock(&g_rtsp_mutex);
			rtsp_tx_video(g_rtsp_session_2, pData, ctx->stFrame.pstPack->u32Len,
			              ctx->stFrame.pstPack->u64PTS);
			rtsp_do_event(g_rtsplive);
			pthread_mutex_unlock(&g_rtsp_mutex);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	return RK_NULL;
}

static void *venc3_get_stream(void *pArgs) {
	printf("#Start %s , arg:%p\n", __func__, pArgs);
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	// char name[256] = {0};
	// FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;

	while (!quit) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			RK_LOGD("chn:%d, loopCount:%d wd:%d\n", ctx->s32ChnId, loopCount,
			        ctx->stFrame.pstPack->u32Len);
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					quit = true;
					break;
				}
			}

			PrintStreamDetails(ctx->s32ChnId, ctx->stFrame.pstPack->u32Len);
			pthread_mutex_lock(&g_rtsp_mutex);
			rtsp_tx_video(g_rtsp_session_3, pData, ctx->stFrame.pstPack->u32Len,
			              ctx->stFrame.pstPack->u64PTS);
			rtsp_do_event(g_rtsplive);
			pthread_mutex_unlock(&g_rtsp_mutex);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	return RK_NULL;
}

void rkba_callback(const RockIvaBaResult *result, const RockIvaExecuteStatus status,
                   void *userData) {
	if (result->objNum == 0)
		return;
	printf("status is %d, frame %d, result->objNum is %d\n", status, result->frameId,
	       result->objNum);
	for (int i = 0; i < result->objNum; i++) {
		printf("topLeft:[%d,%d], bottomRight:[%d,%d],"
		       "objId is %d, frameId is %d, score is %d, type is %d\n",
		       result->triggerObjects[i].objInfo.rect.topLeft.x,
		       result->triggerObjects[i].objInfo.rect.topLeft.y,
		       result->triggerObjects[i].objInfo.rect.bottomRight.x,
		       result->triggerObjects[i].objInfo.rect.bottomRight.y,
		       result->triggerObjects[i].objInfo.objId,
		       result->triggerObjects[i].objInfo.frameId,
		       result->triggerObjects[i].objInfo.score,
		       result->triggerObjects[i].objInfo.type);
	}
}

int rockiva_init() {
	RockIvaRetCode ret;
	// const char *model_type;

	memset(&initParams, 0, sizeof(initParams));
	memset(&globalParams, 0, sizeof(globalParams));

	snprintf(globalParams.modelPath, ROCKIVA_PATH_LENGTH, "/usr/lib/");
	globalParams.coreMask = 0x04;
	globalParams.logLevel = ROCKIVA_LOG_ERROR;
	globalParams.detModel |= ROCKIVA_OBJECT_TYPE_FACE;
	globalParams.detModel |= ROCKIVA_OBJECT_TYPE_PERSON;
	globalParams.detModel |= ROCKIVA_OBJECT_TYPE_NON_VEHICLE;
	globalParams.detModel |= ROCKIVA_OBJECT_TYPE_VEHICLE;
	globalParams.imageInfo.width = 720;
	globalParams.imageInfo.height = 576;
	globalParams.imageInfo.format = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;

	ROCKIVA_Init(&rkba_handle, ROCKIVA_MODE_VIDEO, &globalParams, NULL);
	printf("ROCKIVA_Init over\n");

	// 构建一个区域入侵规则
	int web_width = 704;
	int web_height = 480;
	int ri_x = 0;
	int ri_y = 0;
	int ri_w = 704;
	int ri_h = 480;

	initParams.baRules.areaInBreakRule[0].ruleEnable = 1;
	initParams.baRules.areaInBreakRule[0].sense = 50;
	initParams.baRules.areaInBreakRule[0].alertTime = 1000; // ms
	initParams.baRules.areaInBreakRule[0].minObjSize[2].height = 5;
	initParams.baRules.areaInBreakRule[0].minObjSize[2].width = 5;
	initParams.baRules.areaInBreakRule[0].event = ROCKIVA_BA_TRIP_EVENT_STAY;
	initParams.baRules.areaInBreakRule[0].ruleID = 0;
	initParams.baRules.areaInBreakRule[0].objType =
	    ROCKIVA_OBJECT_TYPE_BITMASK(ROCKIVA_OBJECT_TYPE_PERSON);
	initParams.baRules.areaInBreakRule[0].area.pointNum = 4;
	initParams.baRules.areaInBreakRule[0].area.points[0].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x);
	initParams.baRules.areaInBreakRule[0].area.points[0].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y);
	initParams.baRules.areaInBreakRule[0].area.points[1].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x + ri_w);
	initParams.baRules.areaInBreakRule[0].area.points[1].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y);
	initParams.baRules.areaInBreakRule[0].area.points[2].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x + ri_w);
	initParams.baRules.areaInBreakRule[0].area.points[2].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y + ri_h);
	initParams.baRules.areaInBreakRule[0].area.points[3].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x);
	initParams.baRules.areaInBreakRule[0].area.points[3].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y + ri_h);
	printf("(%d,%d), (%d,%d), (%d,%d), (%d,%d)\n",
	       initParams.baRules.areaInBreakRule[0].area.points[0].x,
	       initParams.baRules.areaInBreakRule[0].area.points[0].y,
	       initParams.baRules.areaInBreakRule[0].area.points[1].x,
	       initParams.baRules.areaInBreakRule[0].area.points[1].y,
	       initParams.baRules.areaInBreakRule[0].area.points[2].x,
	       initParams.baRules.areaInBreakRule[0].area.points[2].y,
	       initParams.baRules.areaInBreakRule[0].area.points[3].x,
	       initParams.baRules.areaInBreakRule[0].area.points[3].y);
	initParams.aiConfig.detectResultMode = 0;
	ret = ROCKIVA_BA_Init(rkba_handle, &initParams, rkba_callback);
	if (ret != ROCKIVA_RET_SUCCESS) {
		printf("ROCKIVA_BA_Init error %d\n", ret);
		return -1;
	}
	printf("ROCKIVA_BA_Init success\n");
	rociva_run_flag = 1;

	return ret;
}

int rockiva_deinit() {
	rociva_run_flag = 0;
	ROCKIVA_BA_Release(rkba_handle);
	ROCKIVA_Release(rkba_handle);

	return 0;
}

int rkipc_rockiva_write_nv12_frame_by_fd(uint16_t width, uint16_t height,
                                         uint32_t frame_id, int32_t fd) {
	int ret;
	if (!rociva_run_flag)
		return 0;
	RockIvaImage *image = (RockIvaImage *)malloc(sizeof(RockIvaImage));
	memset(image, 0, sizeof(RockIvaImage));
	image->info.transformMode = ROCKIVA_IMAGE_TRANSFORM_NONE;
	image->info.width = width;
	image->info.height = height;
	image->info.format = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
	image->frameId = frame_id;
	image->dataAddr = NULL;
	image->dataPhyAddr = NULL;
	image->dataFd = fd;
	ret = ROCKIVA_PushFrame(rkba_handle, image, NULL);
	free(image);

	return ret;
}

pthread_t get_vi_to_npu_thread;
static void *rkipc_get_vi_to_npu(void *arg) {
	printf("#Start %s thread, arg:%p\n", __func__, arg);
	int s32Ret;
	int32_t loopCount = 0;
	VIDEO_FRAME_INFO_S stViFrame;

	while (!quit) {
		s32Ret = RK_MPI_VI_GetChnFrame(0, 1, &stViFrame, 1000);
		if (s32Ret == RK_SUCCESS) {
			RK_LOGD("loopCount is %d, w is %d, h is %d, seq is %d, pts is %lld\n",
			        loopCount, stViFrame.stVFrame.u32Width, stViFrame.stVFrame.u32Height,
			        stViFrame.stVFrame.u32TimeRef, stViFrame.stVFrame.u64PTS / 1000);
			// void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
			int32_t fd = RK_MPI_MB_Handle2Fd(stViFrame.stVFrame.pMbBlk);

			rkipc_rockiva_write_nv12_frame_by_fd(
			    stViFrame.stVFrame.u32Width, stViFrame.stVFrame.u32Height, loopCount, fd);
			s32Ret = RK_MPI_VI_ReleaseChnFrame(0, 1, &stViFrame);
			if (s32Ret != RK_SUCCESS)
				printf("RK_MPI_VI_ReleaseChnFrame fail %x\n", s32Ret);
			loopCount++;
		} else {
			printf("RK_MPI_VI_GetChnFrame timeout %x\n", s32Ret);
		}
	}
	return NULL;
}

static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -s 0 -W 1920 -H 1080 -w 720 -h 576 -f 30 -r 0 -s 1 -W 1920 -H 1080 -w "
	       "720 -h 576 -f 30 -r 0 -n 1 -b 1\n",
	       name);
	printf("\trtsp://xx.xx.xx.xx/live/0, sensor 0 main-stream, Default OPEN\n");
	printf("\trtsp://xx.xx.xx.xx/live/1, sensor 0 sub-stream, Default OPEN\n");
	printf("\trtsp://xx.xx.xx.xx/live/2, sensor 1 main-stream, Default OPEN\n");
	printf("\trtsp://xx.xx.xx.xx/live/3, sensor 1 sub-stream, Default OPEN\n");
	printf("\t-s | --sensor id\n");
	printf("\t-f | --fps: frame per second, Default 30\n");
	printf("\t-r | --hdr: high dynamic range, Default 0\n");
	printf("\t-W | --main_width: main-stream with, Default 1920\n");
	printf("\t-H | --main_height: main-stream height, Default 1080\n");
	printf("\t-w | --sub_width: sub-stream with, Default 720\n");
	printf("\t-h | --sub_height: sub-stream height, Default 576\n");
	printf("\t-n | --enable_npu: enable npu, Default 1\n");
	printf("\t-b | --buf_share: enable buf share, Default 1\n");
}
/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
void handle_pipe(int sig) { printf("%s sig = %d\n", __func__, sig); }

int main(int argc, char *argv[]) {
	// RK_S32 s32Ret = RK_FAILURE;
	SAMPLE_MPI_CTX_S *ctx;
	int cam_0_fps = 30;
	int cam_0_enable_hdr = 0;
	int cam_0_video_0_width = 1920;
	int cam_0_video_0_height = 1080;
	int cam_0_video_1_width = 720;
	int cam_0_video_1_height = 576;
	// int cam_0_video_2_width = 896;
	// int cam_0_video_2_height = 512;

	int cam_1_fps = 30;
	int cam_1_enable_hdr = 0;
	int cam_1_video_0_width = 1920;
	int cam_1_video_0_height = 1080;
	int cam_1_video_1_width = 720;
	int cam_1_video_1_height = 576;
	int enable_npu = 1;
	int enable_buf_share = 1;
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H265;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H265CBR;
	RK_CHAR *pCodecName = "H265";
	RK_S32 s32CamId = -1;
	MPP_CHN_S vi_chn[6], venc_chn[4];
	RK_CHAR *pOutPathVenc = "/userdata";
	RK_S32 s32CamNum = 4;
	RK_S32 s32loopCnt = -1;
	RK_S32 s32BitRate = 4 * 1024;
	RK_S32 i;
	char *iq_file_dir = "/oem/usr/share/iqfiles";

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	struct sigaction action;
	action.sa_handler = handle_pipe;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGPIPE, &action, NULL);

	ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	signal(SIGINT, sigterm_handler);

#ifdef RKAIQ
	RK_BOOL bMultictx = RK_TRUE;
#endif
	int c;
	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		switch (c) {
		case 's':
			s32CamId = atoi(optarg);
			break;
		case 'f':
			if (s32CamId == 0)
				cam_0_fps = atoi(optarg);
			if (s32CamId == 1)
				cam_1_fps = atoi(optarg);
			break;
		case 'r':
			if (s32CamId == 0)
				cam_0_enable_hdr = atoi(optarg);
			if (s32CamId == 1)
				cam_1_enable_hdr = atoi(optarg);
			break;
		case 'W':
			if (s32CamId == 0)
				cam_0_video_0_width = atoi(optarg);
			if (s32CamId == 1)
				cam_1_video_0_width = atoi(optarg);
			break;
		case 'H':
			if (s32CamId == 0)
				cam_0_video_0_height = atoi(optarg);
			if (s32CamId == 1)
				cam_1_video_0_height = atoi(optarg);
			break;
		case 'w':
			if (s32CamId == 0)
				cam_0_video_1_width = atoi(optarg);
			if (s32CamId == 1)
				cam_1_video_1_width = atoi(optarg);
			break;
		case 'h':
			if (s32CamId == 0)
				cam_0_video_1_height = atoi(optarg);
			if (s32CamId == 1)
				cam_1_video_1_height = atoi(optarg);
			break;
		case 'n':
			enable_npu = atoi(optarg);
			break;
		case 'b':
			enable_buf_share = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}
	printf("sensor 0: main:%d*%d, sub:%d*%d\n", cam_0_video_0_width, cam_0_video_0_height,
	       cam_0_video_1_width, cam_0_video_1_height);
	printf("sensor 1: main:%d*%d, sub:%d*%d\n", cam_1_video_0_width, cam_1_video_0_height,
	       cam_1_video_1_width, cam_1_video_1_height);
	if (cam_0_video_0_width <= 0 || cam_1_video_0_width <= 0 ||
	    cam_0_video_0_height <= 0 || cam_1_video_0_height <= 0) {
		printf("invalid main stream width/height,please check!\n");
		return -1;
	}
	printf("#CodecName:%s\n", pCodecName);
	printf("#IQ Path: %s\n", iq_file_dir);
	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);
		rk_aiq_working_mode_t hdr_mode_0 = RK_AIQ_WORKING_MODE_NORMAL;
		rk_aiq_working_mode_t hdr_mode_1 = RK_AIQ_WORKING_MODE_NORMAL;
		if (cam_0_enable_hdr)
			hdr_mode_0 = RK_AIQ_WORKING_MODE_ISP_HDR2;
		if (cam_1_enable_hdr)
			hdr_mode_1 = RK_AIQ_WORKING_MODE_ISP_HDR2;

		SAMPLE_COMM_ISP_Init(0, hdr_mode_0, bMultictx, iq_file_dir);
		SAMPLE_COMM_ISP_Run(0);
		SAMPLE_COMM_ISP_SetFrameRate(0, cam_0_fps);
		SAMPLE_COMM_ISP_Init(1, hdr_mode_1, bMultictx, iq_file_dir);
		SAMPLE_COMM_ISP_Run(1);
		SAMPLE_COMM_ISP_SetFrameRate(1, cam_1_fps);
#endif
	}

	// init rtsp
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session_0 = rtsp_new_session(g_rtsplive, "/live/0");
	g_rtsp_session_1 = rtsp_new_session(g_rtsplive, "/live/1");
	g_rtsp_session_2 = rtsp_new_session(g_rtsplive, "/live/2");
	g_rtsp_session_3 = rtsp_new_session(g_rtsplive, "/live/3");
	rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	rtsp_set_video(g_rtsp_session_1, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	rtsp_set_video(g_rtsp_session_2, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	rtsp_set_video(g_rtsp_session_3, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	rtsp_sync_video_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session_1, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session_2, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session_3, rtsp_get_reltime(), rtsp_get_ntptime());

	if (enable_npu)
		rockiva_init();

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}

	// Init VI and VENC 0~4
	for (i = 0; i < s32CamNum; i++) {
		if (i == 0) {
			ctx->vi[i].u32Width = cam_0_video_0_width;
			ctx->vi[i].u32Height = cam_0_video_0_height;
		}
		if (i == 1) {
			ctx->vi[i].u32Width = cam_0_video_1_width;
			ctx->vi[i].u32Height = cam_0_video_1_height;
		}
		if (i == 2) {
			ctx->vi[i].u32Width = cam_1_video_0_width;
			ctx->vi[i].u32Height = cam_1_video_0_height;
		}
		if (i == 3) {
			ctx->vi[i].u32Width = cam_1_video_1_width;
			ctx->vi[i].u32Height = cam_1_video_1_height;
		}
		if (i < 2) {
			ctx->vi[i].s32DevId = 0;
			ctx->vi[i].u32PipeId = 0;
			ctx->vi[i].s32ChnId = i;
		} else {
			ctx->vi[i].s32DevId = 1;
			ctx->vi[i].u32PipeId = 1;
			ctx->vi[i].s32ChnId = i - 2;
		}
		ctx->vi[i].stChnAttr.stIspOpt.u32BufCount = 2;
		ctx->vi[i].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
		ctx->vi[i].stChnAttr.u32Depth = 0;
		ctx->vi[i].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
		ctx->vi[i].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
		ctx->vi[i].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
		ctx->vi[i].stChnAttr.stFrameRate.s32DstFrameRate = -1;
		if ((i == 1) && enable_npu) { // NPU only 10 fps,  need anothor buffer
			ctx->vi[i].stChnAttr.stIspOpt.u32BufCount = 3;
			ctx->vi[i].stChnAttr.u32Depth = 1;
			pthread_create(&get_vi_to_npu_thread, NULL, rkipc_get_vi_to_npu, NULL);
		}
		SAMPLE_COMM_VI_CreateChn(&ctx->vi[i]);

		vi_chn[i].enModId = RK_ID_VI;
		vi_chn[i].s32DevId = ctx->vi[i].s32DevId;
		vi_chn[i].s32ChnId = ctx->vi[i].s32ChnId;

		ctx->venc[i].s32ChnId = i;
		if (i == 0) {
			ctx->venc[i].u32Width = cam_0_video_0_width;
			ctx->venc[i].u32Height = cam_0_video_0_height;
			ctx->venc[i].stChnAttr.stVencAttr.u32BufSize =
			    cam_0_video_0_width * cam_0_video_0_height / 4;
			ctx->venc[i].u32Fps = cam_0_fps;
		}
		if (i == 1) {
			ctx->venc[i].u32Width = cam_0_video_1_width;
			ctx->venc[i].u32Height = cam_0_video_1_height;
			ctx->venc[i].stChnAttr.stVencAttr.u32BufSize =
			    cam_0_video_1_width * cam_0_video_1_height / 4;
			ctx->venc[i].u32Fps = cam_0_fps;
		}
		if (i == 2) {
			ctx->venc[i].u32Width = cam_1_video_0_width;
			ctx->venc[i].u32Height = cam_1_video_0_height;
			ctx->venc[i].stChnAttr.stVencAttr.u32BufSize =
			    cam_1_video_0_width * cam_1_video_0_height / 4;
			ctx->venc[i].u32Fps = cam_1_fps;
		}
		if (i == 3) {
			ctx->venc[i].u32Width = cam_1_video_1_width;
			ctx->venc[i].u32Height = cam_1_video_1_height;
			ctx->venc[i].stChnAttr.stVencAttr.u32BufSize =
			    cam_1_video_1_width * cam_1_video_1_height / 4;
			ctx->venc[i].u32Fps = cam_1_fps;
		}
		ctx->venc[i].u32Gop = 50;
		ctx->venc[i].u32BitRate = s32BitRate;
		ctx->venc[i].enCodecType = enCodecType;
		ctx->venc[i].enRcMode = enRcMode;
		if (i == 0)
			ctx->venc[i].getStreamCbFunc = venc0_get_stream;
		if (i == 1)
			ctx->venc[i].getStreamCbFunc = venc1_get_stream;
		if (i == 2)
			ctx->venc[i].getStreamCbFunc = venc2_get_stream;
		if (i == 3)
			ctx->venc[i].getStreamCbFunc = venc3_get_stream;
		ctx->venc[i].s32loopCount = s32loopCnt;
		ctx->venc[i].dstFilePath = pOutPathVenc;
		// H264  66：Baseline  77：Main Profile 100：High Profile
		// H265  0：Main Profile  1：Main 10 Profile
		// MJPEG 0：Baseline
		ctx->venc[i].stChnAttr.stVencAttr.u32Profile = 0;
		ctx->venc[i].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
		ctx->venc[i].enable_buf_share = enable_buf_share;
		SAMPLE_COMM_VENC_CreateChn(&ctx->venc[i]);
		printf("venc[%d]:u32BufSize:%d\n", i,
		       ctx->venc[i].stChnAttr.stVencAttr.u32BufSize);

		venc_chn[i].enModId = RK_ID_VENC;
		venc_chn[i].s32DevId = 0;
		venc_chn[i].s32ChnId = ctx->venc[i].s32ChnId;
	}
	SAMPLE_COMM_Bind(&vi_chn[0], &venc_chn[0]);
	SAMPLE_COMM_Bind(&vi_chn[1], &venc_chn[1]);
	SAMPLE_COMM_Bind(&vi_chn[2], &venc_chn[2]);
	SAMPLE_COMM_Bind(&vi_chn[3], &venc_chn[3]);

	printf("%s initial finish\n", __func__);

	while (!quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);
	for (i = 0; i < s32CamNum; i++) {
		if (ctx->venc[i].getStreamCbFunc) {
			pthread_join(ctx->venc[i].getStreamThread, NULL);
		}
	}
	if (enable_npu)
		rockiva_deinit();
	pthread_join(get_vi_to_npu_thread, NULL);

	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);

	SAMPLE_COMM_UnBind(&vi_chn[0], &venc_chn[0]);
	SAMPLE_COMM_UnBind(&vi_chn[1], &venc_chn[1]);
	SAMPLE_COMM_UnBind(&vi_chn[2], &venc_chn[2]);
	SAMPLE_COMM_UnBind(&vi_chn[3], &venc_chn[3]);

	// Destroy VI[0]
	for (i = 0; i < s32CamNum; i++) {
		SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[i]);
		SAMPLE_COMM_VI_DestroyChn(&ctx->vi[i]);
	}

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		for (int i = 0; i < s32CamNum; i++) {
			SAMPLE_COMM_ISP_Stop(i);
		}
#endif
	}

	if (ctx) {
		free(ctx);
		ctx = RK_NULL;
	}

	return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
