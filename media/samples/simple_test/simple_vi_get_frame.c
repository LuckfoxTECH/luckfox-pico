
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

static RK_S32 g_s32FrameCnt = -1;
static bool quit = false;

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

RK_U64 TEST_COMM_GetNowUs() {
	struct timespec time = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

static void *GetMediaBuffer0(void *arg) {
	printf("========%s========\n", __func__);
	int loopCount = 0;
	int s32Ret;
	RK_S32 waitTime = 1000;
	int pipeId = 0;
	int channelId = 0;
	channelId = *(int *)arg;
	VIDEO_FRAME_INFO_S stViFrame;
	VI_CHN_STATUS_S stChnStatus;

	while (!quit) {
		s32Ret = RK_MPI_VI_GetChnFrame(pipeId, channelId, &stViFrame, waitTime);
		if (s32Ret == RK_SUCCESS) {
			RK_U64 nowUs = TEST_COMM_GetNowUs();
			void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
			RK_LOGD(
			    "RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%llu",
			    data, loopCount, stViFrame.stVFrame.u32TimeRef,
			    stViFrame.stVFrame.u64PTS / 1000, stViFrame.stVFrame.u64PrivateData);
			// 6.get the channel status
			s32Ret = RK_MPI_VI_QueryChnStatus(pipeId, channelId, &stChnStatus);
			RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d,"
			        "current frame id:%d,input lost:%d,output lost:%d,"
			        "framerate:%d,vbfail:%d delay=%lldus",
			        s32Ret, stChnStatus.stSize.u32Width, stChnStatus.stSize.u32Height,
			        stChnStatus.bEnable, stChnStatus.u32CurFrameID,
			        stChnStatus.u32InputLostFrame, stChnStatus.u32OutputLostFrame,
			        stChnStatus.u32FrameRate, stChnStatus.u32VbFail,
			        nowUs - stViFrame.stVFrame.u64PTS);

			// 7.release the frame
			s32Ret = RK_MPI_VI_ReleaseChnFrame(pipeId, channelId, &stViFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
			}
			loopCount++;
		} else {
			RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
		}

		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
			quit = true;
			break;
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

static RK_CHAR optstr[] = "?::w:h:c:I:o:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -I 0 -w 1920 -h 1080 -o 1\n", name);
	printf("\t-w | --width: VI width, Default:1920\n");
	printf("\t-h | --heght: VI height, Default:1080\n");
	printf("\t-c | --frame_cnt: frame number of output, Default:-1\n");
	printf("\t-I | --camid: camera ctx id, Default 0. "
	       "0:rkisp_mainpath,1:rkisp_selfpath,2:rkisp_bypasspath\n");
	printf("\t-o: output path, Default:0  0 or 1 /data/test_0.yuv\n");
}

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 1920;
	RK_U32 u32Height = 1080;
	int savefile = 0;
	RK_S32 s32chnlId = 0;
	int c;
	VI_SAVE_FILE_INFO_S stDebugFile;

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
		case 'o':
			savefile = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#Resolution: %dx%d\n", u32Width, u32Height);
	printf("#Output Path: %d\n", savefile);
	printf("#CameraIdx: %d\n\n", s32chnlId);
	printf("#Frame Count to save: %d\n", g_s32FrameCnt);

	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("rk mpi sys init fail!");
		goto __FAILED;
	}

	vi_dev_init();
	vi_chn_init(s32chnlId, u32Width, u32Height);

	stDebugFile.bCfg = (RK_BOOL)savefile;
	// memcpy(stDebugFile.aFilePath, "/userdata/", strlen("/data"));
	strcpy(stDebugFile.aFilePath, "/userdata/");

	snprintf(stDebugFile.aFileName, sizeof(stDebugFile.aFileName), "test_%d.yuv",
	         s32chnlId);
	RK_MPI_VI_ChnSaveFile(0, s32chnlId, &stDebugFile);

	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer0, &s32chnlId);

	while (!quit) {
		usleep(50000);
	}
	pthread_join(&main_thread, NULL);
	s32Ret = RK_MPI_VI_DisableChn(0, s32chnlId);
	RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

	s32Ret = RK_MPI_VI_DisableDev(0);
	RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);

__FAILED:
	RK_LOGE("test running exit:%d", s32Ret);
	RK_MPI_SYS_Exit();

	return 0;
}
