
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

#define MAXWIDTH 2304
#define MAXHEIGHT 1296

static RK_S32 g_s32FrameCnt = -1;
static bool quit = false;
#define RK_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

typedef struct rkResolution_st {
	SIZE_S size;
	const char *resStr;
} RK_RESOLUTION_ST;

static RK_RESOLUTION_ST test_res[] = {
    {{1920, 1080}, "1920x1080"}, // 1080p
    {{1280, 720}, "1280x1080"},  // 720p
    {{720, 576}, "720 x 576"},   // 576p
    {{640, 480}, "640 x 480"},   // 480p
    {{640, 360}, "640 x 360"},   // 360p
    {{512, 288}, "512 x 288"},   //
    {{480, 320}, "480 x 320"},   // 320p
    {{320, 240}, "320 x 240"},   // 240p
    {{2304, 1296}, "2304x1296"}, // 300w
                                 // {{2560, 1440}, "2560x1440"},	// 400w
                                 // {{2880, 1616}, "2880x1616"},	// 500w
};

RK_U64 TEST_COMM_GetNowUs() {
	struct timespec time = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

static void *GetMediaBuffer0(void *arg) {
	(void)arg;
	printf("========%s========\n", __func__);
	void *pData = RK_NULL;
	int loopCount = 0;
	int s32Ret;
	RK_U32 idx = 0;
	RK_U32 resCnt;
	resCnt = RK_ARRAY_ELEMS(test_res);
	VENC_STREAM_S stFrame;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	VENC_CHN_ATTR_S stAttr;
	VI_CHN_ATTR_S stChnAttr;

	MPP_CHN_S stSrcChn, stDestChn;
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 0;

	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;

	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(0, &stFrame, 500);
		if (s32Ret == RK_SUCCESS) {
			RK_U64 nowUs = TEST_COMM_GetNowUs();

			RK_LOGD("chn:0, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n",
			        loopCount, stFrame.u32Seq, stFrame.pstPack->u32Len,
			        stFrame.pstPack->u64PTS, nowUs - stFrame.pstPack->u64PTS);

			s32Ret = RK_MPI_VENC_ReleaseStream(0, &stFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
			}
			loopCount++;
		}

		// change resolution
		if (loopCount % 100 == 0) {
			printf("===change resolution===\n");
			if (idx >= resCnt)
				idx = 0;

			// 1, unbind
			s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
			}

			// 2, set venc
			stAttr.stVencAttr.u32PicWidth = test_res[idx].size.u32Width;
			stAttr.stVencAttr.u32PicHeight = test_res[idx].size.u32Height;
			stAttr.stVencAttr.u32VirWidth = test_res[idx].size.u32Width;
			stAttr.stVencAttr.u32VirHeight = test_res[idx].size.u32Height;
			stAttr.stVencAttr.u32MaxPicWidth = MAXWIDTH;
			stAttr.stVencAttr.u32MaxPicHeight = MAXHEIGHT;
			RK_MPI_VENC_SetChnAttr(0, &stAttr);

			// 3, set vi
			stChnAttr.stSize.u32Width = test_res[idx].size.u32Width;
			stChnAttr.stSize.u32Height = test_res[idx].size.u32Height;
			stChnAttr.stIspOpt.stMaxSize.u32Width = MAXWIDTH;
			stChnAttr.stIspOpt.stMaxSize.u32Height = MAXHEIGHT;
			stChnAttr.stFrameRate.s32SrcFrameRate = -1;
			stChnAttr.stFrameRate.s32DstFrameRate = -1;
			s32Ret = RK_MPI_VI_SetChnAttr(0, 0, &stChnAttr);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGD("RK_MPI_VI_SetChnAttr %d x %d fail, ret: %x",
				        test_res[idx].size.u32Width, test_res[idx].size.u32Height,
				        s32Ret);
			} else {
				RK_LOGD("====RK_MPI_VI_SetChnAttr %d x %d success====",
				        test_res[idx].size.u32Width, test_res[idx].size.u32Height);
			}

			// 4, bind
			s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("bind %x ch venc failed", s32Ret);
			}
			idx++;
		}
		usleep(10 * 1000);

		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
			quit = true;
			break;
		}

		usleep(10 * 1000);
	}

	free(stFrame.pstPack);
	return NULL;
}

static RK_S32 test_venc_init(int chnId, int width, int height, RK_CODEC_ID_E enType) {
	printf("========%s========\n", __func__);
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_ATTR_S stAttr;
	memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));

	stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
	stAttr.stRcAttr.stH264Cbr.u32BitRate = 10 * 1024;
	stAttr.stRcAttr.stH264Cbr.u32Gop = 60;

	stAttr.stVencAttr.enType = enType;
	stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
	stAttr.stVencAttr.u32MaxPicWidth = MAXWIDTH;
	stAttr.stVencAttr.u32MaxPicHeight = MAXHEIGHT;
	stAttr.stVencAttr.u32PicWidth = width;
	stAttr.stVencAttr.u32PicHeight = height;
	stAttr.stVencAttr.u32VirWidth = width;
	stAttr.stVencAttr.u32VirHeight = height;
	stAttr.stVencAttr.u32StreamBufCnt = 2;
	stAttr.stVencAttr.u32BufSize = MAXWIDTH * MAXHEIGHT * 3 / 2;
	// stAttr.stGopAttr.u32MaxLtrCount = 1;
	// stAttr.stVencAttr.enMirror = MIRROR_NONE;

	RK_MPI_VENC_CreateChn(chnId, &stAttr);

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

// demo板dev默认都是0，根据不同的channel 来选择不同的vi节点
int vi_dev_init() {
	printf("%s\n", __func__);
	int ret = 0;
	int devId = 0;
	int pipeId = devId;

	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	memset(&stDevAttr, 0, sizeof(stDevAttr));
	memset(&stBindPipe, 0, sizeof(stBindPipe));
	// 0. get dev config status
	ret = RK_MPI_VI_GetDevAttr(devId, &stDevAttr);
	if (ret == RK_ERR_VI_NOT_CONFIG) {
		// 0-1.config dev
		ret = RK_MPI_VI_SetDevAttr(devId, &stDevAttr);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VI_SetDevAttr %x\n", ret);
			return -1;
		}
	} else {
		printf("RK_MPI_VI_SetDevAttr already\n");
	}
	// 1.get dev enable status
	ret = RK_MPI_VI_GetDevIsEnable(devId);
	if (ret != RK_SUCCESS) {
		// 1-2.enable dev
		ret = RK_MPI_VI_EnableDev(devId);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VI_EnableDev %x\n", ret);
			return -1;
		}
		// 1-3.bind dev/pipe
		stBindPipe.u32Num = pipeId;
		stBindPipe.PipeId[0] = pipeId;
		ret = RK_MPI_VI_SetDevBindPipe(devId, &stBindPipe);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VI_SetDevBindPipe %x\n", ret);
			return -1;
		}
	} else {
		printf("RK_MPI_VI_EnableDev already\n");
	}

	return 0;
}

int vi_chn_init(int channelId, int width, int height) {
	int ret;
	int buf_cnt = 2;
	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType =
	    VI_V4L2_MEMORY_TYPE_MMAP; // VI_V4L2_MEMORY_TYPE_MMAP;
	vi_chn_attr.stIspOpt.stMaxSize.u32Width = MAXWIDTH;
	vi_chn_attr.stIspOpt.stMaxSize.u32Height = MAXHEIGHT;
	vi_chn_attr.stIspOpt.bNoUseLibV4L2 = RK_TRUE;
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;
	vi_chn_attr.u32Depth = 2;
	vi_chn_attr.stFrameRate.s32SrcFrameRate = -1;
	vi_chn_attr.stFrameRate.s32DstFrameRate = -1;

	ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(0, channelId);
	if (ret) {
		printf("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}

	return ret;
}

static RK_CHAR optstr[] = "?::w:h:c:I:e:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -I 0 -w 1920 -h 1080 -o /tmp/venc.h264\n", name);
	printf("\t-w | --width: VI width, Default:1920\n");
	printf("\t-h | --heght: VI height, Default:1080\n");
	printf("\t-c | --frame_cnt: frame number of output, Default:150\n");
	printf("\t-I | --camid: camera ctx id, Default 0. "
	       "0:rkisp_mainpath,1:rkisp_selfpath,2:rkisp_bypasspath\n");
	printf("\t-e | --encode: encode type, Default:h264, Value:h264, h265, mjpeg\n");
}

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 1920;
	RK_U32 u32Height = 1080;
	RK_CODEC_ID_E enCodecType = RK_VIDEO_ID_AVC;
	RK_CHAR *pCodecName = "H264";
	RK_S32 s32chnlId = 0;
	int c;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
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
			} else if (!strcmp(optarg, "mjpeg")) {
				enCodecType = RK_VIDEO_ID_MJPEG;
				pCodecName = "MJPEG";
			} else {
				printf("ERROR: Invalid encoder type.\n");
				return 0;
			}
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#CodecName:%s\n", pCodecName);
	printf("#Resolution: %dx%d\n", u32Width, u32Height);
	printf("#CameraIdx: %d\n\n", s32chnlId);
	printf("#Frame Count to save: %d\n", g_s32FrameCnt);

	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("rk mpi sys init fail!");
		goto __FAILED;
	}

	vi_dev_init();
	vi_chn_init(s32chnlId, u32Width, u32Height);

	// venc  init
	test_venc_init(0, u32Width, u32Height,
	               enCodecType); // RK_VIDEO_ID_AVC RK_VIDEO_ID_HEVC

	MPP_CHN_S stSrcChn, stDestChn;
	// bind vi to venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = s32chnlId;

	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 0;
	printf("====RK_MPI_SYS_Bind vi0 to venc0====\n");
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("bind 0 ch venc failed");
		goto __FAILED;
	}

	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer0, NULL);

	while (!quit) {
		usleep(50000);
	}
	pthread_join(&main_thread, NULL);

	s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
	}

	s32Ret = RK_MPI_VI_DisableChn(0, s32chnlId);
	RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

	s32Ret = RK_MPI_VENC_StopRecvFrame(0);
	if (s32Ret != RK_SUCCESS) {
		return s32Ret;
	}

	s32Ret = RK_MPI_VENC_DestroyChn(0);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VDEC_DestroyChn fail %x", s32Ret);
	}

	s32Ret = RK_MPI_VI_DisableDev(0);
	RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);

__FAILED:
	RK_LOGE("test running exit:%d", s32Ret);
	RK_MPI_SYS_Exit();

	return 0;
}
