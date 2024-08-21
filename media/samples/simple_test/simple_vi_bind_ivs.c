#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "rk_comm_ivs.h"
#include "rk_debug.h"
#include "rk_defines.h"
#include "rk_mpi_adec.h"
#include "rk_mpi_aenc.h"
#include "rk_mpi_ai.h"
#include "rk_mpi_ao.h"
#include "rk_mpi_avs.h"
#include "rk_mpi_cal.h"
#include "rk_mpi_ivs.h"
#include "rk_mpi_mb.h"
#include "rk_mpi_rgn.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_tde.h"
#include "rk_mpi_vdec.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_vi.h"
#include "rk_mpi_vo.h"
#include "rk_mpi_vpss.h"

struct ivs_info {
	RK_U32 u32Width;
	RK_U32 u32Height;
	RK_U32 u32Sensitivity;
};

static bool quit = false;

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static void *GetMediaBuffer0(void *arg) {
	struct ivs_info *info = (struct ivs_info *)arg;
	printf("========%s========\n", __func__);
	int loopCount = 0;
	int s32Ret;
	IVS_RESULT_INFO_S stResults;
	// int width = 1920;
	// int height = 1080;
	RK_U32 u32SquarePct[5] = {50, 30, 25, 20, 20};

	while (!quit) {
		memset(&stResults, 0, sizeof(IVS_RESULT_INFO_S));
		s32Ret = RK_MPI_IVS_GetResults(0, &stResults, -1);
		if (s32Ret == RK_SUCCESS) {
			if (stResults.s32ResultNum == 1) {
				for (int i = 0; i < stResults.pstResults->stMdInfo.u32RectNum; i++) {
					printf("%d: [%d, %d, %d, %d]\n", i,
					       stResults.pstResults->stMdInfo.stRect[i].s32X,
					       stResults.pstResults->stMdInfo.stRect[i].s32Y,
					       stResults.pstResults->stMdInfo.stRect[i].u32Width,
					       stResults.pstResults->stMdInfo.stRect[i].u32Height);
					printf("u32Square %u, u32DetAreaSquare %u, u32DetOutputSquare %u\n",
					       stResults.pstResults->stMdInfo.u32Square,
					       stResults.pstResults->stMdInfo.u32DetAreaSquare,
					       stResults.pstResults->stMdInfo.u32DetOutputSquare);
				}
				if (1000 * stResults.pstResults->stMdInfo.u32Square / info->u32Width / info->u32Height > u32SquarePct[info->u32Sensitivity])
					printf("Detect movement!\n");
			}
			RK_MPI_IVS_ReleaseResults(0, &stResults);
		} else {
			RK_LOGE("RK_MPI_IVS_GetResults fail %x", s32Ret);
		}
		loopCount++;
	}

	return NULL;
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
		stBindPipe.u32Num = 1;
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
	    VI_V4L2_MEMORY_TYPE_DMABUF; // VI_V4L2_MEMORY_TYPE_MMAP;
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;
	vi_chn_attr.u32Depth = 0; //0, get fail, 1 - u32BufCount, can get, if bind to other device, must be < u32BufCount
	ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(0, channelId);
	if (ret) {
		printf("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}

	return ret;
}

static RK_S32 create_ivs(int width, int height, RK_U32 u32AreaEn, RK_U32 u32Sensitivity) {
	RK_S32 s32Ret;
	IVS_CHN_ATTR_S attr;
	memset(&attr, 0, sizeof(attr));
	attr.enMode = IVS_MODE_MD_OD;
	attr.u32PicWidth = width;
	attr.u32PicHeight = height;
	attr.enPixelFormat = RK_FMT_YUV420SP;
	attr.s32Gop = 30;
	attr.bSmearEnable = RK_FALSE;
	attr.bWeightpEnable = RK_FALSE;
	attr.bMDEnable = RK_TRUE;
	attr.s32MDInterval = 5;
	attr.bMDNightMode = RK_FALSE;
	attr.bODEnable = RK_TRUE;
	attr.s32ODInterval = 1;
	attr.s32ODPercent = 7;
	if (u32AreaEn) {
		attr.stDetAttr.stDetArea.u32AreaNum = 1;
		attr.stDetAttr.stDetArea.areas[0].u32PointNum = 4;
		attr.stDetAttr.stDetArea.areas[0].points[0].s32X = width / 2;
		attr.stDetAttr.stDetArea.areas[0].points[0].s32Y = 0;
		attr.stDetAttr.stDetArea.areas[0].points[1].s32X = width;
		attr.stDetAttr.stDetArea.areas[0].points[1].s32Y = height / 2;
		attr.stDetAttr.stDetArea.areas[0].points[2].s32X = width / 2;
		attr.stDetAttr.stDetArea.areas[0].points[2].s32Y = height;
		attr.stDetAttr.stDetArea.areas[0].points[3].s32X = 0;
		attr.stDetAttr.stDetArea.areas[0].points[3].s32Y = height / 2;
	}

	s32Ret = RK_MPI_IVS_CreateChn(0, &attr);
	if (s32Ret) {
		RK_LOGE("ivs create failed:%x", s32Ret);
		goto __FAILED;
	}

#ifdef RV1106_RV1103
	IVS_MD_ATTR_S stMdAttr;
	memset(&stMdAttr, 0, sizeof(stMdAttr));
	s32Ret = RK_MPI_IVS_GetMdAttr(0, &stMdAttr);
	if (s32Ret) {
		RK_LOGE("ivs get mdattr failed:%x", s32Ret);
		goto __FAILED;
	}
	switch (u32Sensitivity) {
	case 0:
		stMdAttr.s32ThreshSad = 96;
		stMdAttr.s32ThreshMove = 3;
		stMdAttr.s32SwitchSad = 2;
		break;
	case 1:
		stMdAttr.s32ThreshSad = 72;
		stMdAttr.s32ThreshMove = 2;
		stMdAttr.s32SwitchSad = 2;
		break;
	case 2:
		stMdAttr.s32ThreshSad = 64;
		stMdAttr.s32ThreshMove = 2;
		stMdAttr.s32SwitchSad = 2;
	case 3:
		stMdAttr.s32ThreshSad = 48;
		stMdAttr.s32ThreshMove = 1;
		stMdAttr.s32SwitchSad = 2;
		break;
	case 4:
		stMdAttr.s32ThreshSad = 32;
		stMdAttr.s32ThreshMove = 1;
		stMdAttr.s32SwitchSad = 0;
		break;
	default:
		stMdAttr.s32ThreshSad = 64;
		stMdAttr.s32ThreshMove = 2;
		stMdAttr.s32SwitchSad = 2;
		break;
	}
	stMdAttr.bFlycatkinFlt = RK_TRUE;
	stMdAttr.s32ThresDustMove = 3;
	stMdAttr.s32ThresDustBlk = 3;
	stMdAttr.s32ThresDustChng = 50;
	s32Ret = RK_MPI_IVS_SetMdAttr(0, &stMdAttr);
	if (s32Ret) {
		RK_LOGE("ivs set mdattr failed:%x", s32Ret);
		goto __FAILED;
	}
#endif

	return 0;
__FAILED:
	return -1;
}

static RK_CHAR optstr[] = "?::w:h:I:A:S:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -I 0 -w 1920 -h 1080\n", name);
	printf("\t-w | --width: VI width, Default:1920\n");
	printf("\t-h | --heght: VI height, Default:1080\n");
	printf("\t-I | --camid: camera ctx id, Default 0. "
	       "0:rkisp_mainpath,1:rkisp_selfpath,2:rkisp_bypasspath\n");
	printf("\t-A | --Area: Area detect, Default:0\n");
	printf("\t-S | --Sensitivity: MD sensitivity, range [0, 4], Default: 2\n");
}

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 1920;
	RK_U32 u32Height = 1080;
	MPP_CHN_S stSrcChn, stIvsChn;
	RK_S32 s32chnlId = 0;
	RK_U32 u32AreaEn = 0;
	RK_U32 u32Sensitivity = 2;
	int c;
	int ret = -1;

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
		case 'A':
			u32AreaEn = atoi(optarg);
			break;
		case 'S':
			u32Sensitivity = atoi(optarg);
			if (u32Sensitivity > 4)
				u32Sensitivity = 4;
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return -1;
		}
	}

	printf("#Resolution: %dx%d\n", u32Width, u32Height);
	printf("#CameraIdx: %d\n\n", s32chnlId);

	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("rk mpi sys init fail!");
		goto __FAILED;
	}

	vi_dev_init();
	vi_chn_init(s32chnlId, u32Width, u32Height);
	create_ivs(u32Width, u32Height, u32AreaEn, u32Sensitivity);

	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = s32chnlId;

	stIvsChn.enModId = RK_ID_IVS;
	stIvsChn.s32DevId = 0;
	stIvsChn.s32ChnId = 0;
	printf("====RK_MPI_SYS_Bind vi0 to venc0====\n");
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stIvsChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("bind 0 ch venc failed");
		goto __FAILED;
	}

	struct ivs_info ivs_info = {u32Width, u32Height, u32Sensitivity};
	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer0, &ivs_info);

	while (!quit) {
		usleep(5000);
	}
	pthread_join(main_thread, NULL);

	s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stIvsChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
	}

	s32Ret = RK_MPI_IVS_DestroyChn(0);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_IVS_DestroyChn fail %x", s32Ret);
	}

	s32Ret = RK_MPI_VI_DisableChn(0, s32chnlId);
	RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

	s32Ret = RK_MPI_VI_DisableDev(0);
	RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
	ret = 0;
__FAILED:
	RK_LOGE("test running exit:%d", s32Ret);
	RK_MPI_SYS_Exit();

	return ret;
}
