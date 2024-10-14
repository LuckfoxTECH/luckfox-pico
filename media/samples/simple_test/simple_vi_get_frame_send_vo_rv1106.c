
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

#include "rk_debug.h"
#include "rk_defines.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_vi.h"
#include "rk_mpi_vo.h"

static bool quit = false;
static	int u32Width = 1024;
static	int u32Height = 600;
static	int s32chnlId = 0;
static	int VoLayer = 0;
static	int VoDev = 0;
static	int VoChn = 0;

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static void *GetMediaBuffer0(void *arg) {
	printf("========%s========\n", __func__);
	int s32Ret;
	RK_S32 waitTime = 1000;
	int pipeId = 0;
	VIDEO_FRAME_INFO_S stViFrame;

	while (!quit) {
		s32Ret = RK_MPI_VI_GetChnFrame(pipeId, s32chnlId, &stViFrame, waitTime);
		if (s32Ret == RK_SUCCESS) {

			RK_MPI_VO_SendFrame(VoLayer, VoChn, &stViFrame, -1);

			// 7.release the frame
			s32Ret = RK_MPI_VI_ReleaseChnFrame(pipeId, s32chnlId, &stViFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
			}
		} else {
			RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
		}
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
	vi_chn_attr.u32Depth = 2; //0, get fail, 1 - u32BufCount, can get, if bind to other device, must be < u32BufCount
	ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(0, channelId);
	if (ret) {
		printf("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}

	return ret;
}

static int vo_init(int VoLayer, int VoDev, int VoChn, int Width, int Height) {
	int ret = RK_SUCCESS;
	VO_PUB_ATTR_S            stVoPubAttr;
	VO_VIDEO_LAYER_ATTR_S    stLayerAttr;
	VO_CHN_ATTR_S            stChnAttr;

	ret = RK_MPI_VO_BindLayer(VoLayer, VoDev, VO_LAYER_MODE_GRAPHIC);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_BindLayer failed, ret = %x", ret);
		return ret;
	}

	memset(&stVoPubAttr, 0, sizeof(VO_PUB_ATTR_S));
	memset(&stLayerAttr, 0, sizeof(VO_VIDEO_LAYER_ATTR_S));
	memset(&stChnAttr, 0, sizeof(VO_CHN_ATTR_S));

	stVoPubAttr.enIntfType = VO_INTF_DEFAULT;
	stVoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;

	ret = RK_MPI_VO_SetPubAttr(VoDev, &stVoPubAttr);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_SetPubAttr failed, ret = %x", ret);
		return ret;
	}

	ret = RK_MPI_VO_Enable(VoDev);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_Enable failed, ret = %x", ret);
		return ret;
	}

	/* Enable Layer */
	stLayerAttr.enPixFormat      = RK_FMT_RGB888;
	stLayerAttr.enCompressMode   = COMPRESS_AFBC_16x16;
	stLayerAttr.stDispRect.s32X  = 0;
	stLayerAttr.stDispRect.s32Y  = 0;
	stLayerAttr.stDispRect.u32Width   = Width;
	stLayerAttr.stDispRect.u32Height  = Height;
	stLayerAttr.stImageSize.u32Width  = Width;
	stLayerAttr.stImageSize.u32Height = Height;
	stLayerAttr.u32DispFrmRt          = 25;

	ret = RK_MPI_VO_SetLayerAttr(VoLayer, &stLayerAttr);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_SetLayerAttr failed, ret = %x", ret);
		return ret;
	}

	RK_MPI_VO_SetLayerSpliceMode(VoLayer, VO_SPLICE_MODE_RGA);

	ret = RK_MPI_VO_EnableLayer(VoLayer);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_EnableLayer failed, ret = %x", ret);
		return ret;
	}

	stChnAttr.stRect.s32X = 0;
	stChnAttr.stRect.s32Y = 0;
	stChnAttr.stRect.u32Width = Width;
	stChnAttr.stRect.u32Height = Height;
	stChnAttr.u32FgAlpha = 255;
	stChnAttr.u32BgAlpha = 0;
	stChnAttr.enMirror = MIRROR_NONE;
	stChnAttr.enRotation = ROTATION_0;
	stChnAttr.u32Priority = 1;

	ret = RK_MPI_VO_SetChnAttr(VoLayer, VoChn, &stChnAttr);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_SetChnAttr failed, ret = %x", ret);
		return ret;
	}

	ret = RK_MPI_VO_EnableChn(VoLayer, VoChn);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_EnableChn failed, ret = %x", ret);
		return ret;
	}

	RK_LOGE("Create vo [dev: %d, layer: %d, chn: %d] success!",
				VoDev, VoLayer, VoChn);
	return ret;
}

static int vo_deinit(int VoLayer, int VoDev, int VoChn) {
	int ret = 0;

	ret = RK_MPI_VO_DisableChn(VoLayer, VoChn);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_DisableChn failed, ret = %x", ret);
		return ret;
	}

	ret = RK_MPI_VO_DisableLayer(VoLayer);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_DisableLayer failed, ret = %x", ret);
		return ret;
	}

	ret = RK_MPI_VO_Disable(VoDev);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_Disable failed, ret = %x", ret);
		return ret;
	}

	ret = RK_MPI_VO_UnBindLayer(VoLayer, VoDev);
	if (ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VO_UnBindLayer failed, ret = %x", ret);
		return ret;
	}

	RK_MPI_VO_CloseFd();

	RK_LOGE("Destroy vo [dev: %d, layer: %d, chn: %d] success!",
			VoDev, VoLayer, VoChn);
	return ret;
}


static RK_CHAR optstr[] = "?::w:h:I:l:d:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s simple_vi_get_frame_send_vo  -w 1024 -h 600 -I 0 -l 0 -d 0\n", name);
	printf("\t-w | --width: VI width, Default:1920\n");
	printf("\t-h | --heght: VI height, Default:1080\n");
	printf("\t-I | --camid: camera ctx id, Default 0. "
	       "0:rkisp_mainpath,1:rkisp_selfpath,2:rkisp_bypasspath\n");
	printf("\t-l | --layer: Vo layer, Default 0. ");
	printf("\t-d | --device: Vo device, Default 0. ");
}

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
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
		case 'l':
			VoLayer = atoi(optarg);
			break;
		case 'd':
			VoDev = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return -1;
		}
	}

	printf("#Resolution: %dx%d\n", u32Width, u32Height);
	printf("#CameraIdx: %d\n\n", s32chnlId);
	printf("#Vo Layer: %d\n\n", VoLayer);
	printf("#Vo Devices: %d\n\n", VoDev);

	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("rk mpi sys init fail!");
		goto __FAILED;
	}

	vi_dev_init();
	vi_chn_init(s32chnlId, u32Width, u32Height);
	vo_init(VoLayer, VoDev, VoChn, u32Width, u32Height);

	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer0, NULL);

	while (!quit) {
		usleep(50000);
	}
	pthread_join(main_thread, NULL);
	s32Ret = RK_MPI_VI_DisableChn(0, s32chnlId);
	RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

	s32Ret = RK_MPI_VI_DisableDev(0);
	RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);

	vo_deinit(VoLayer, VoDev, VoChn);
	ret = 0;
__FAILED:
	RK_LOGE("test running exit:%d", s32Ret);
	RK_MPI_SYS_Exit();

	return ret;
}
