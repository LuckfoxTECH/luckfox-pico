#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

static bool quit = false;

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static void *GetMediaBuffer0(void *arg) {
	(void)arg;
	printf("========%s========\n", __func__);
	int loopCount = 0;
	int s32Ret;
	IVS_RESULT_INFO_S stResults;
	int width = 1920;
	// int height = 1080;

	while (!quit) {
		memset(&stResults, 0, sizeof(IVS_RESULT_INFO_S));
		s32Ret = RK_MPI_IVS_GetResults(0, &stResults, -1);
		if (s32Ret == RK_SUCCESS) {
			if (loopCount % 10 == 0 && stResults.s32ResultNum == 1) {
				int x = width / 8 / 8;
				int y = stResults.pstResults->stMdInfo.u32Size / 64;
				if (stResults.pstResults->stMdInfo.pData) {
					for (int n = 0; n < x * 8; n++)
						printf("-");
					printf("\n");
					for (int j = 0; j < y; j++) {
						for (int i = 0; i < x; i++) {
							for (int k = 0; k < 8; k++) {
								if (stResults.pstResults->stMdInfo.pData[j * 64 + i] &
								    (1 << k))
									printf("1");
								else
									printf("0");
							}
						}
						printf("\n");
					}
					for (int n = 0; n < x * 8; n++)
						printf("-");
					printf("\n");
				}
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
	    VI_V4L2_MEMORY_TYPE_DMABUF; // VI_V4L2_MEMORY_TYPE_MMAP;
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;
	vi_chn_attr.u32Depth = 2;
	ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(0, channelId);
	if (ret) {
		printf("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}

	return ret;
}

static RK_S32 create_ivs(int width, int height) {
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
	attr.s32MDInterval = 1;
	attr.bMDNightMode = RK_FALSE;
	attr.bODEnable = RK_TRUE;
	attr.s32ODInterval = 1;
	attr.s32ODPercent = 7;

	return RK_MPI_IVS_CreateChn(0, &attr);
}

int main() {
	int width = 1920;
	int height = 1080;
	MPP_CHN_S stSrcChn, stIvsChn;

	signal(SIGINT, sigterm_handler);
	RK_MPI_SYS_Init();
	vi_dev_init();
	vi_chn_init(0, width, height);
	create_ivs(width, height);

	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 0;

	stIvsChn.enModId = RK_ID_IVS;
	stIvsChn.s32DevId = 0;
	stIvsChn.s32ChnId = 0;
	RK_MPI_SYS_Bind(&stSrcChn, &stIvsChn);

	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer0, NULL);

	while (!quit) {
		usleep(5000);
	}
	pthread_join(&main_thread, NULL);
	RK_MPI_SYS_UnBind(&stSrcChn, &stIvsChn);
	RK_MPI_IVS_DestroyChn(0);
	RK_MPI_VI_DisableChn(0, 0);
	RK_MPI_VI_DisableDev(0);
	RK_MPI_SYS_Exit();
	return 0;
}
