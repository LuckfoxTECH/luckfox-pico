#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include "loadbmp.h"
#include "rk_mpi_rgn.h"
#include "sample_comm_isp.h"
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
#define RGN_NUM_MAX 4
#define VENC_NUM_MAX 4
pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
static rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session[VENC_NUM_MAX];

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[4];
	SAMPLE_VENC_CTX_S venc[4];
	SAMPLE_RGN_CTX_S rgn[RGN_NUM_MAX];
} SAMPLE_MPI_CTX_S;

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

void handle_pipe(int sig) { printf("%s sig = %d\n", __func__, sig); }
static RK_CHAR optstr[] = "?::a:f:W:H:s:i:n:b:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"fps", required_argument, NULL, 'f'},
    {"main_width", required_argument, NULL, 'W'},
    {"main_height", required_argument, NULL, 'H'},
    {"sensorid", required_argument, NULL, 's'},
    {"inputBmpPath", required_argument, RK_NULL, 'i'},
    {"buf_share", required_argument, NULL, 'b'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -s 0 -W 1920 -H 1080 -f 15 -b 1 -s 1 -W 1920 -H 1080 "
	       "-f 15 -b 1 -a /oem/usr/share/iqfiles\n",
	       name);
#ifdef RKAIQ
	printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a "
	       "/oem/usr/share/iqfiles, Default: /oem/usr/share/iqfiles");
#endif
	printf("\trtsp://xx.xx.xx.xx/live/0, sensor 0 main-stream, Default OPEN\n");
	printf("\trtsp://xx.xx.xx.xx/live/1, sensor 1 main-stream, Default OPEN\n");
	printf("\t-s | --sensor id\n");
	printf("\t-i | --inputBmpPath : input bmp file path. default: RK_NULL\n");
	printf("\t-f | --fps: frame per second, Default 15\n");
	printf("\t-W | --main_width: main-stream with, Default 1920\n");
	printf("\t-H | --main_height: main-stream height, Default 1080\n");
	printf("\t-b | --buf_share: enable buf share, Default 1\n");
}

static void *venc_get_stream(void *pArgs) {
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;
	RK_CHAR name[255] = {0};
	sprintf(name, "venc_%d_get_stream", ctx->s32ChnId);
	prctl(PR_SET_NAME, name);

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

			pthread_mutex_lock(&g_rtsp_mutex);
			rtsp_tx_video(g_rtsp_session[ctx->s32ChnId], pData,
			              ctx->stFrame.pstPack->u32Len, ctx->stFrame.pstPack->u64PTS);
			rtsp_do_event(g_rtsplive);
			pthread_mutex_unlock(&g_rtsp_mutex);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	return RK_NULL;
}

RK_S32 rgn_init(RK_CHAR *bmp_file_path, SAMPLE_MPI_CTX_S *ctx) {

	RK_S32 s32Ret = RK_FAILURE;

	RK_U32 u32Width = 0;
	RK_U32 u32Height = 0;

	/* Init RGN[0] camera 0 : vi main stream*/
	ctx->rgn[0].rgnHandle = 0;
	ctx->rgn[0].stRgnAttr.enType = COVER_RGN;
	ctx->rgn[0].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[0].stMppChn.s32ChnId = 0;
	ctx->rgn[0].stMppChn.s32DevId = 0;
	ctx->rgn[0].stRegion.s32X = 0;        // must be 16 aligned
	ctx->rgn[0].stRegion.s32Y = 0;        // must be 16 aligned
	ctx->rgn[0].stRegion.u32Width = 256;  // must be 16 aligned
	ctx->rgn[0].stRegion.u32Height = 256; // must be 16 aligned
	ctx->rgn[0].u32Color = 0x00f800;
	ctx->rgn[0].u32Layer = 1;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[0]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[0].rgnHandle);
		return s32Ret;
	}

	/* Init RGN[1] camera1 : vi main stream*/
	ctx->rgn[1].rgnHandle = 1;
	ctx->rgn[1].stRgnAttr.enType = COVER_RGN;
	ctx->rgn[1].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[1].stMppChn.s32ChnId = 0;
	ctx->rgn[1].stMppChn.s32DevId = 1;
	ctx->rgn[1].stRegion.s32X = 0;        // must be 16 aligned
	ctx->rgn[1].stRegion.s32Y = 0;        // must be 16 aligned
	ctx->rgn[1].stRegion.u32Width = 256;  // must be 16 aligned
	ctx->rgn[1].stRegion.u32Height = 256; // must be 16 aligned
	ctx->rgn[1].u32Color = 0x00f800;
	ctx->rgn[1].u32Layer = 1;
	s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[1]);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
		        ctx->rgn[1].rgnHandle);
		return s32Ret;
	}

	/* Init RGN[2] camera 0 :venc main stream*/

	s32Ret = SAMPLE_COMM_GetBmpResolution(bmp_file_path, &u32Width, &u32Height);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
		u32Width = 128;
		u32Height = 128;
	}
	ctx->rgn[2].rgnHandle = 2;
	ctx->rgn[2].stRgnAttr.unAttr.stOverlay.u32CanvasNum = 1;
	ctx->rgn[2].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[2].stMppChn.enModId = RK_ID_VENC;
	ctx->rgn[2].stMppChn.s32ChnId = ctx->venc[0].s32ChnId;
	ctx->rgn[2].stMppChn.s32DevId = 0;
	ctx->rgn[2].stRegion.s32X =
	    RK_ALIGN_16(ctx->venc[0].u32Width / 2); // must be 16 aligned
	ctx->rgn[2].stRegion.s32Y =
	    RK_ALIGN_16(ctx->venc[0].u32Height / 2); // must be 16 aligned
	ctx->rgn[2].stRegion.u32Width = u32Width;    // must be 8 aligned
	ctx->rgn[2].stRegion.u32Height = u32Height;  // must be 8 aligned
	ctx->rgn[2].u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn[2].u32BgAlpha = 128;
	ctx->rgn[2].u32FgAlpha = 128;
	ctx->rgn[2].u32Layer = 2;
	ctx->rgn[2].srcFileBmpName = bmp_file_path;
	if (bmp_file_path) {
		s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[2]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
			        ctx->rgn[2].rgnHandle);
			return s32Ret;
		}
	} else {
		RK_LOGE("the bmp file is NULL, overlay rgn skips");
	}

	/* Init RGN[3] camera 1 :venc main stream*/
	ctx->rgn[3].rgnHandle = 3;
	ctx->rgn[3].stRgnAttr.unAttr.stOverlay.u32CanvasNum = 1;
	ctx->rgn[3].stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn[3].stMppChn.enModId = RK_ID_VENC;
	ctx->rgn[3].stMppChn.s32ChnId = ctx->venc[1].s32ChnId;
	ctx->rgn[3].stMppChn.s32DevId = 0;
	ctx->rgn[3].stRegion.s32X =
	    RK_ALIGN_16(ctx->venc[1].u32Width / 2); // must be 16 aligned
	ctx->rgn[3].stRegion.s32Y =
	    RK_ALIGN_16(ctx->venc[1].u32Height / 2); // must be 16 aligned
	ctx->rgn[3].stRegion.u32Width = u32Width;    // must be 8 aligned
	ctx->rgn[3].stRegion.u32Height = u32Height;  // must be 8 aligned
	ctx->rgn[3].u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn[3].u32BgAlpha = 128;
	ctx->rgn[3].u32FgAlpha = 128;
	ctx->rgn[3].u32Layer = 3;
	ctx->rgn[3].srcFileBmpName = bmp_file_path;
	if (bmp_file_path) {
		s32Ret = SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[3]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_RGN_CreateChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
			        ctx->rgn[3].rgnHandle);
			return s32Ret;
		}
	} else {
		RK_LOGE("the bmp file is NULL, overlay rgn skips");
	}

	return s32Ret;
}

static RK_S32 rgn_deinit(SAMPLE_MPI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_SUCCESS;
	for (RK_S32 i = 0; i < RGN_NUM_MAX; i++) {
		if (!ctx->rgn[i].srcFileBmpName && ctx->rgn[i].stRgnAttr.enType == OVERLAY_RGN) {
			continue;
		}
		s32Ret = SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn[i]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("SAMPLE_COMM_RGN_DestroyChn Failure s32Ret:%#X rgn handle:%d", s32Ret,
			        ctx->rgn[i].rgnHandle);
		}
	}
	return s32Ret;
}

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	SAMPLE_MPI_CTX_S *ctx;
	int cam_0_fps = 15;
	int cam_0_video_0_width = 1920;
	int cam_0_video_0_height = 1080;

	int cam_1_fps = 15;
	int cam_1_video_0_width = 1920;
	int cam_1_video_0_height = 1080;

	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H265;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H265CBR;
	RK_S32 enable_buf_share = 1;
	RK_CHAR *pCodecName = "H265";
	RK_S32 s32CamId = -1;
	MPP_CHN_S vi_chn[2], venc_chn[2];
	RK_S32 s32CamNum = 2;
	RK_S32 s32loopCnt = -1;
	RK_S32 s32BitRate = 4 * 1024;
	RK_CHAR *inputBmpPath = RK_NULL;
	RK_S32 i;
	char *iq_file_dir = "/oem/usr/share/iqfiles";

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	signal(SIGINT, sigterm_handler);

	ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	if (!ctx) {
		RK_LOGE("ctx is null, malloc failure");
		return RK_FAILURE;
	}
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

#ifdef RKAIQ
	RK_BOOL bMultictx = RK_TRUE;
#endif
	int c;
	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		const char *tmp_optarg = optarg;
		switch (c) {
		case 'a':
			if (!optarg && NULL != argv[optind] && '-' != argv[optind][0]) {
				tmp_optarg = argv[optind++];
			}
			if (tmp_optarg) {
				iq_file_dir = (char *)tmp_optarg;
			} else {
				iq_file_dir = "/oem/usr/share/iqfiles";
			}
			break;
		case 's':
			s32CamId = atoi(optarg);
			break;
		case 'i':
			inputBmpPath = optarg;
			break;
		case 'f':
			if (s32CamId == 0)
				cam_0_fps = atoi(optarg);
			if (s32CamId == 1)
				cam_1_fps = atoi(optarg);
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
		case 'b':
			enable_buf_share = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}
	printf("sensor 0: main:%d*%d\n", cam_0_video_0_width, cam_0_video_0_height);
	printf("sensor 1: main:%d*%d\n", cam_1_video_0_width, cam_1_video_0_height);
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
		rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
		SAMPLE_COMM_PreInit_devBufCnt(0, 1);
		SAMPLE_COMM_ISP_Init(0, hdr_mode, bMultictx, iq_file_dir);
		SAMPLE_COMM_ISP_Run(0);
		SAMPLE_COMM_ISP_SetFrameRate(0, cam_0_fps);
		SAMPLE_COMM_PreInit_devBufCnt(1, 1);
		SAMPLE_COMM_ISP_Init(1, hdr_mode, bMultictx, iq_file_dir);
		SAMPLE_COMM_ISP_Run(1);
		SAMPLE_COMM_ISP_SetFrameRate(1, cam_1_fps);
#endif
	}
	// init rtsp
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session[0] = rtsp_new_session(g_rtsplive, "/live/0");
	g_rtsp_session[1] = rtsp_new_session(g_rtsplive, "/live/1");
	rtsp_set_video(g_rtsp_session[0], RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	rtsp_set_video(g_rtsp_session[1], RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	rtsp_sync_video_ts(g_rtsp_session[0], rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session[1], rtsp_get_reltime(), rtsp_get_ntptime());

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}

	// Init VI[0]
	ctx->vi[0].u32Width = cam_0_video_0_width;
	ctx->vi[0].u32Height = cam_0_video_0_height;
	ctx->vi[0].s32DevId = 0;
	ctx->vi[0].u32PipeId = 0;
	ctx->vi[0].s32ChnId = 0;
	ctx->vi[0].stChnAttr.stIspOpt.u32BufCount = 1;
	ctx->vi[0].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi[0].stChnAttr.u32Depth = 0;
	ctx->vi[0].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi[0].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi[0].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi[0].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	SAMPLE_COMM_VI_CreateChn(&ctx->vi[0]);

	// Init VI[1]
	ctx->vi[1].u32Width = cam_1_video_0_width;
	ctx->vi[1].u32Height = cam_1_video_0_height;
	ctx->vi[1].s32DevId = 1;
	ctx->vi[1].u32PipeId = 1;
	ctx->vi[1].s32ChnId = 0;
	ctx->vi[1].stChnAttr.stIspOpt.u32BufCount = 1;
	ctx->vi[1].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi[1].stChnAttr.u32Depth = 0;
	ctx->vi[1].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi[1].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi[1].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi[1].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	SAMPLE_COMM_VI_CreateChn(&ctx->vi[1]);

	// Init venc[0]
	ctx->venc[0].s32ChnId = 0;
	ctx->venc[0].u32Width = cam_0_video_0_width;
	ctx->venc[0].u32Height = cam_0_video_0_height;
	ctx->venc[0].stChnAttr.stVencAttr.u32BufSize =
	    cam_0_video_0_width * cam_0_video_0_height / 4;
	ctx->venc[0].u32Fps = cam_0_fps;
	ctx->venc[0].u32Gop = 50;
	ctx->venc[0].u32BitRate = s32BitRate;
	ctx->venc[0].enCodecType = enCodecType;
	ctx->venc[0].enRcMode = enRcMode;
	ctx->venc[0].getStreamCbFunc = venc_get_stream;
	ctx->venc[0].s32loopCount = s32loopCnt;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 0;
	ctx->venc[0].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	ctx->venc[0].enable_buf_share = enable_buf_share;
	ctx->venc[0].bSvcIfEnable = 0;
	ctx->venc[0].bMotionDeblurIfEnable = 0;
	ctx->venc[0].bComboIfEnable = 0;
	ctx->venc[0].u32BuffSize = ctx->venc[0].stChnAttr.stVencAttr.u32BufSize;
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc[0]);

	// Init venc[1]
	ctx->venc[1].s32ChnId = 1;
	ctx->venc[1].u32Width = cam_1_video_0_width;
	ctx->venc[1].u32Height = cam_1_video_0_height;
	ctx->venc[1].stChnAttr.stVencAttr.u32BufSize =
	    cam_1_video_0_width * cam_1_video_0_height / 4;
	ctx->venc[1].u32Fps = cam_1_fps;
	ctx->venc[1].u32Gop = 50;
	ctx->venc[1].u32BitRate = s32BitRate;
	ctx->venc[1].enCodecType = enCodecType;
	ctx->venc[1].enRcMode = enRcMode;
	ctx->venc[1].getStreamCbFunc = venc_get_stream;
	ctx->venc[1].s32loopCount = s32loopCnt;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 0;
	ctx->venc[1].stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	ctx->venc[1].enable_buf_share = enable_buf_share;
	ctx->venc[1].bSvcIfEnable = 0;
	ctx->venc[1].bMotionDeblurIfEnable = 0;
	ctx->venc[1].bComboIfEnable = 0;
	ctx->venc[1].u32BuffSize = ctx->venc[1].stChnAttr.stVencAttr.u32BufSize;
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc[1]);

	// bind vi & venc
	for (i = 0; i < s32CamNum; i++) {
		vi_chn[i].enModId = RK_ID_VI;
		vi_chn[i].s32DevId = ctx->vi[i].s32DevId;
		vi_chn[i].s32ChnId = ctx->vi[i].s32ChnId;
		venc_chn[i].enModId = RK_ID_VENC;
		venc_chn[i].s32DevId = 0;
		venc_chn[i].s32ChnId = ctx->venc[i].s32ChnId;
		SAMPLE_COMM_Bind(&vi_chn[i], &venc_chn[i]);
	}
	// if u32CanvasNum(osd buffer)=1,rgn must be created after bind
	rgn_init(inputBmpPath, ctx);
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
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);

	SAMPLE_COMM_UnBind(&vi_chn[0], &venc_chn[0]);
	SAMPLE_COMM_UnBind(&vi_chn[1], &venc_chn[1]);

	rgn_deinit(ctx);

	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[0]);
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[1]);
	SAMPLE_COMM_VI_DestroyChn(&ctx->vi[0]);
	SAMPLE_COMM_VI_DestroyChn(&ctx->vi[1]);

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		for (int i = 0; i < s32CamNum; i++) {
			s32Ret = SAMPLE_COMM_ISP_Stop(i);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_Stop failure:%#X", s32Ret);
				return s32Ret;
			}
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
