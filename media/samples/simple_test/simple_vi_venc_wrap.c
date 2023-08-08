#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

#define RK_MAX(a, b) ((a) > (b) ? (a) : (b))
#define RK_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

typedef struct _rkTestVencCfg {
	RK_BOOL bOutDebugCfg;
	VENC_CHN_ATTR_S stAttr;
	RK_CHAR dstFilePath[128];
	RK_CHAR dstFileName[128];
	RK_S32 s32ChnId;
	FILE *fp;
	RK_S32 selectFd;
} TEST_VENC_CFG;

static RK_S32 g_s32FrameCnt = -1;
static bool quit = false;
static RK_U32 g_u32MaxWidth;
static RK_U32 g_u32MaxHeight;
static RK_BOOL g_u8SaveCfg;

static VI_CHN_ATTR_S g_vi_chn_attr;
static RK_BOOL g_bWrapEn;
static RK_U32 g_u32WrapLine;
static VI_CHN_BUF_WRAP_S g_stViWrap;
static RK_U32 g_u32DstCodec;
static RK_U32 g_u32DeBreath;
static TEST_VENC_CFG g_venc_cfgs[2];
static VENC_STREAM_S g_venc_stFrame[2];

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

RK_U64 TEST_COMM_GetNowUs() {
	struct timespec time = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

// demo板dev默认都是0，根据不同的channel 来选择不同的vi节点
static int vi_dev_init() {
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

static int vi_chn_init(int channelId, int width, int height, int maxWidth,
                       int maxHeight) {
	int ret;
	int buf_cnt = 3;
	// VI init

	memset(&g_vi_chn_attr, 0, sizeof(g_vi_chn_attr));
	g_vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	g_vi_chn_attr.stIspOpt.enMemoryType =
	    VI_V4L2_MEMORY_TYPE_DMABUF; // VI_V4L2_MEMORY_TYPE_MMAP;
	g_vi_chn_attr.stSize.u32Width = width;
	g_vi_chn_attr.stSize.u32Height = height;
	g_vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;

	g_vi_chn_attr.stIspOpt.stMaxSize.u32Width = RK_MAX(maxWidth, width);
	g_vi_chn_attr.stIspOpt.stMaxSize.u32Height = RK_MAX(maxHeight, height);
	g_vi_chn_attr.u32Depth = 1;

	ret = RK_MPI_VI_SetChnAttr(0, channelId, &g_vi_chn_attr);
	if (ret) {
		printf("ERROR: VI set channel attribute fail, ret=%d\n", ret);
		return ret;
	}

	// set wrap mode attr
	memset(&g_stViWrap, 0, sizeof(VI_CHN_BUF_WRAP_S));
	if (g_bWrapEn) {
		if (g_u32WrapLine < 128 || g_u32WrapLine > height) {
			printf("wrap mode buffer line must between [128, H]");
			return -1;
		}
		g_stViWrap.bEnable = RK_TRUE;
		g_stViWrap.u32BufLine = g_u32WrapLine;
		g_stViWrap.u32WrapBufferSize =
		    g_stViWrap.u32BufLine * width * 3 / 2; // nv12 (w * wrapLine *3 / 2)
		printf("set channel wrap line: %d, wrapBuffSize = %d", g_u32WrapLine,
		       g_stViWrap.u32WrapBufferSize);
		RK_MPI_VI_SetChnWrapBufAttr(0, channelId, &g_stViWrap);
	}

	ret |= RK_MPI_VI_EnableChn(0, channelId);
	if (ret) {
		printf("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}

	return ret;
}

static void init_venc_cfg(RK_U32 u32Ch, RK_CODEC_ID_E enType) {
	RK_BOOL bSvc = RK_FALSE;
	RK_S32 vencChId = 0;
	RK_U32 u32GopSize = 60;
	RK_U32 u32BitRateKb = 10 * 1024;

	g_venc_cfgs[u32Ch].stAttr.stVencAttr.enType = enType;
	g_venc_cfgs[u32Ch].s32ChnId = vencChId + u32Ch;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.enPixelFormat = g_vi_chn_attr.enPixelFormat;

	g_venc_cfgs[u32Ch].stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
	g_venc_cfgs[u32Ch].stAttr.stRcAttr.stH264Cbr.u32BitRate = u32BitRateKb;
	g_venc_cfgs[u32Ch].stAttr.stRcAttr.stH264Cbr.u32Gop = u32GopSize;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.u32MaxPicWidth = g_u32MaxWidth;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.u32MaxPicHeight = g_u32MaxHeight;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.u32PicWidth = g_vi_chn_attr.stSize.u32Width;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.u32PicHeight = g_vi_chn_attr.stSize.u32Height;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.u32VirWidth = g_vi_chn_attr.stSize.u32Width;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.u32VirHeight = g_vi_chn_attr.stSize.u32Height;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.u32StreamBufCnt = 5;
	g_venc_cfgs[u32Ch].stAttr.stVencAttr.u32BufSize =
	    g_u32MaxWidth * g_u32MaxHeight * 3 / 2;
	g_venc_cfgs[u32Ch].stAttr.stGopAttr.u32MaxLtrCount = 1;
}

static RK_S32 create_venc(RK_U32 u32Ch) {
	RK_BOOL bRefBufShare = RK_FALSE;
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;

	memset(&stVencChnBufWrap, 0, sizeof(stVencChnBufWrap));
	stVencChnBufWrap.bEnable = g_stViWrap.bEnable;

	memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
	stVencChnRefBufShare.bEnable = bRefBufShare;

	stRecvParam.s32RecvPicNum = g_s32FrameCnt;
	RK_MPI_VENC_CreateChn(g_venc_cfgs[u32Ch].s32ChnId, &g_venc_cfgs[u32Ch].stAttr);
	RK_MPI_VENC_SetChnBufWrapAttr(g_venc_cfgs[u32Ch].s32ChnId, &stVencChnBufWrap);
	RK_MPI_VENC_SetChnRefBufShareAttr(g_venc_cfgs[u32Ch].s32ChnId, &stVencChnRefBufShare);

	if (g_u32DeBreath > 0) {
		VENC_DEBREATHEFFECT_S stDeBreathEffect;
		memset(&stDeBreathEffect, 0, sizeof(stDeBreathEffect));
		stDeBreathEffect.bEnable = RK_TRUE;
		stDeBreathEffect.s32Strength1 = g_u32DeBreath;
		RK_MPI_VENC_SetDeBreathEffect(u32Ch, &stDeBreathEffect);
	}

	RK_MPI_VENC_StartRecvFrame(g_venc_cfgs[u32Ch].s32ChnId, &stRecvParam);

	return RK_SUCCESS;
}

static void *vi_venc_wrap(void *arg) {
	RK_S32 s32Ret;
	RK_S32 loopCount = 0;
	void *pData = RK_NULL;

	(void)arg;
	VI_CHN_STATUS_S stChnStatus;
	printf("========%s========\n", __func__);

	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(g_venc_cfgs[0].s32ChnId, &g_venc_stFrame[0], -1);
		if (s32Ret == RK_SUCCESS) {
			if (g_venc_cfgs[0].bOutDebugCfg) {
				pData = RK_MPI_MB_Handle2VirAddr(g_venc_stFrame[0].pstPack->pMbBlk);
				fwrite(pData, 1, g_venc_stFrame[0].pstPack->u32Len, g_venc_cfgs[0].fp);
				fflush(g_venc_cfgs[0].fp);
			}
			RK_U64 nowUs = TEST_COMM_GetNowUs();

			printf("chn:0, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n",
			       loopCount, g_venc_stFrame[0].u32Seq, g_venc_stFrame[0].pstPack->u32Len,
			       g_venc_stFrame[0].pstPack->u64PTS,
			       nowUs - g_venc_stFrame[0].pstPack->u64PTS);

			s32Ret =
			    RK_MPI_VENC_ReleaseStream(g_venc_cfgs[0].s32ChnId, &g_venc_stFrame[0]);
			if (s32Ret != RK_SUCCESS) {
				printf("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
			}
			loopCount++;
		} else {
			printf("RK_MPI_VI_GetChnFrame fail %x", s32Ret);
		}

		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
			quit = true;
			break;
		}

		usleep(1 * 1000);
	}
}

static RK_CHAR optstr[] = "?::d:n:w:h:W:H:c:I:o:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -I 0 -w 1920 -h 1080 -W 2560 -H 1440 -o 1\n", name);
	printf("\t-w | --width: VI width, Default:2560\n");
	printf("\t-h | --height: VI height, Default:1440\n");
	printf("\t-W | --maxWidth: sensor output resolution max width, Default:2560\n");
	printf("\t-H | --maxHeight: sensor output resolution max height, Default:1440\n");
	printf("\t-c | --frame_cnt: frame number of output, Default:-1\n");
	printf("\t-I | --camid: camera ctx id, Default 0. "
	       "0:rkisp_mainpath,1:rkisp_selfpath,2:rkisp_bypasspath\n");
	printf("\t-o: output path, Default:0  0 or 1 /data/test_0.yuv\n");
}
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"pixel_format", optional_argument, NULL, 'f'},
    {"device_name", required_argument, NULL, 'd'},
    {"camera_num", required_argument, NULL, 'n'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"maxWidth", required_argument, NULL, 'W'},
    {"maxHeight", required_argument, NULL, 'H'},
    {"wrap", required_argument, NULL, 'r'},
    {"wrap_line", required_argument, NULL, 'L'},
    {"codec", required_argument, NULL, 'C'},
    {"de_breath", required_argument, NULL, 'D'}, //  debreath[0, 35] default(0)
    {"loop_count", required_argument, NULL, 'l'},
    {"output_path", required_argument, NULL, 'o'},
    {"camid", required_argument, NULL, 'I'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 1920;
	RK_U32 u32Height = 1080;
	RK_U32 u32MaxWidth = 2560;
	RK_U32 u32MaxHeight = 1440;
	int savefile = 0;
	RK_S32 s32DevId = 0;
	RK_S32 s32chnlId = 0;
	RK_U32 u32DeBreath = 0;
	int c;
	VI_SAVE_FILE_INFO_S stDebugFile;

	RK_U32 u32DstCodec = RK_VIDEO_ID_HEVC;
	g_bWrapEn = RK_TRUE;

	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		switch (c) {
		case 'w':
			u32Width = atoi(optarg);
			break;
		case 'h':
			u32Height = atoi(optarg);
			break;
		case 'W':
			u32MaxWidth = atoi(optarg);
			break;
		case 'H':
			u32MaxHeight = atoi(optarg);
			break;
		case 'I':
			s32chnlId = atoi(optarg);
			break;
		case 'C': // codec
			u32DstCodec = atoi(optarg);
			break;
		case 'D': // de_breath
			u32DeBreath = atoi(optarg);
			break;
		case 'l':
			g_s32FrameCnt = atoi(optarg);
			break;
		case 'o':
			savefile = atoi(optarg);
			break;
		case 'r': // wrap
			g_bWrapEn = (RK_BOOL)atoi(optarg);
			break;
		case 'L': // wrap line
			g_u32WrapLine = atoi(optarg);
			break;

		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#Resolution: %dx%d\n", u32Width, u32Height);
	printf("#Sensor Max Resolution: %dx%d\n", g_u32MaxWidth, g_u32MaxHeight);
	printf("#Output Path: %d\n", savefile);
	printf("#CameraIdx: %d\n\n", s32chnlId);
	printf("#Frame Count to save: %d\n", g_s32FrameCnt);

	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		printf("rk mpi sys init fail!");
		goto __FAILED;
	}

	g_u32MaxWidth = u32MaxWidth;
	g_u32MaxHeight = u32MaxHeight;

	g_u32DstCodec = u32DstCodec;
	g_u32DeBreath = u32DeBreath;

	vi_dev_init();
	vi_chn_init(s32chnlId, u32Width, u32Height, u32MaxWidth, u32MaxHeight);

	stDebugFile.bCfg = (RK_BOOL)savefile;
	g_u8SaveCfg = stDebugFile.bCfg;
	g_venc_cfgs[0].bOutDebugCfg = g_u8SaveCfg;
	// memcpy(stDebugFile.aFilePath, "/userdata/", strlen("/data"));
	strcpy(stDebugFile.aFilePath, "/userdata/");

	snprintf(stDebugFile.aFileName, sizeof(stDebugFile.aFileName), "venc_wrap_%d.bin",
	         s32chnlId);
	RK_MPI_VI_ChnSaveFile(0, 0, &stDebugFile);

	MPP_CHN_S stSrcChn;
	MPP_CHN_S stDestChn[2];
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = s32DevId;
	stSrcChn.s32ChnId = s32chnlId;

	// venc  init and create
	init_venc_cfg(0, (RK_CODEC_ID_E)u32DstCodec);
	s32Ret = create_venc(0);
	if (s32Ret != RK_SUCCESS) {
		printf("create %d ch venc failed", g_venc_cfgs[0].s32ChnId);
		return s32Ret;
	}
	// bind vi to venc
	stDestChn[0].enModId = RK_ID_VENC;
	stDestChn[0].s32DevId = 0;
	stDestChn[0].s32ChnId = g_venc_cfgs[0].s32ChnId;
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn[0]);
	if (s32Ret != RK_SUCCESS) {
		printf("bind %d ch venc failed", g_venc_cfgs[0].s32ChnId);
		goto __FAILED;
	}

	g_venc_stFrame[0].pstPack = (VENC_PACK_S *)(malloc(sizeof(VENC_PACK_S)));

	pthread_t main_thread;
	pthread_create(&main_thread, NULL, vi_venc_wrap, NULL);

	while (!quit) {
		usleep(50000);
	}
	pthread_join(&main_thread, NULL);

	s32Ret = RK_MPI_VI_DisableChn(0, 0);
	printf("RK_MPI_VI_DisableChn %x", s32Ret);

	s32Ret = RK_MPI_VI_DisableDev(0);
	printf("RK_MPI_VI_DisableDev %x", s32Ret);

__FAILED:
	printf("test running exit:%d", s32Ret);
	RK_MPI_SYS_Exit();

	return 0;
}