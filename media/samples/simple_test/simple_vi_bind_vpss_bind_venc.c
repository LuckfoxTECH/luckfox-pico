#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

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

static FILE *venc0_file;
static bool quit = false;
static RK_S32 g_s32FrameCnt = -1;
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
	(void)arg;
	printf("========%s========\n", __func__);
	void *pData = RK_NULL;
	int loopCount = 0;
	int s32Ret;

	VENC_STREAM_S stFrame;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(0, &stFrame, -1);
		if (s32Ret == RK_SUCCESS) {
			if (venc0_file) {
				pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
				fwrite(pData, 1, stFrame.pstPack->u32Len, venc0_file);
				fflush(venc0_file);
			}
			RK_U64 nowUs = TEST_COMM_GetNowUs();

			RK_LOGD("chn:0, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n",
			        loopCount, stFrame.u32Seq, stFrame.pstPack->u32Len,
			        stFrame.pstPack->u64PTS, nowUs - stFrame.pstPack->u64PTS);

			s32Ret = RK_MPI_VENC_ReleaseStream(0, &stFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
			}
			loopCount++;
		} else {
			RK_LOGE("RK_MPI_VENC_GetChnFrame fail %x", s32Ret);
		}

		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt))
			quit = true;
		usleep(10 * 1000);
	}

	if (venc0_file)
		fclose(venc0_file);

	free(stFrame.pstPack);
	return NULL;
}

static RK_S32 test_venc_init(int chnId, int width, int height, RK_CODEC_ID_E enType) {
	printf("================================%s==================================\n",
	       __func__);
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_ATTR_S stAttr;
	memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));

	stAttr.stVencAttr.enType = enType;
	stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
	stAttr.stRcAttr.stH264Cbr.u32BitRate = 10 * 1024;
	stAttr.stRcAttr.stH264Cbr.u32Gop = 60;
	stAttr.stVencAttr.u32PicWidth = width;
	stAttr.stVencAttr.u32PicHeight = height;
	stAttr.stVencAttr.u32VirWidth = width;
	stAttr.stVencAttr.u32VirHeight = height;
	stAttr.stVencAttr.u32StreamBufCnt = 2;
	stAttr.stVencAttr.u32BufSize = width * height * 3 / 2;

	RK_MPI_VENC_CreateChn(chnId, &stAttr);

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

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
	printf("================================%s==================================\n",
	       __func__);
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

int test_vpss_init(int VpssChn, int width, int height) {
	printf("================================%s==================================\n",
	       __func__);
	int s32Ret;
	VPSS_CHN_ATTR_S stVpssChnAttr;
	VPSS_GRP_ATTR_S stGrpVpssAttr;

	int s32Grp = 0;

	stGrpVpssAttr.u32MaxW = 4096;
	stGrpVpssAttr.u32MaxH = 4096;
	stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
	stGrpVpssAttr.stFrameRate.s32SrcFrameRate = -1;
	stGrpVpssAttr.stFrameRate.s32DstFrameRate = -1;
	stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE;

	stVpssChnAttr.enChnMode = VPSS_CHN_MODE_USER;
	stVpssChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
	stVpssChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	stVpssChnAttr.stFrameRate.s32SrcFrameRate = -1;
	stVpssChnAttr.stFrameRate.s32DstFrameRate = -1;
	stVpssChnAttr.u32Width = width;
	stVpssChnAttr.u32Height = height;
	stVpssChnAttr.enCompressMode = COMPRESS_MODE_NONE;

	s32Ret = RK_MPI_VPSS_CreateGrp(s32Grp, &stGrpVpssAttr);
	if (s32Ret != RK_SUCCESS) {
		return s32Ret;
	}

	s32Ret = RK_MPI_VPSS_SetChnAttr(s32Grp, VpssChn, &stVpssChnAttr);
	if (s32Ret != RK_SUCCESS) {
		return s32Ret;
	}
	s32Ret = RK_MPI_VPSS_EnableChn(s32Grp, VpssChn);
	if (s32Ret != RK_SUCCESS) {
		return s32Ret;
	}

	s32Ret = RK_MPI_VPSS_StartGrp(s32Grp);
	if (s32Ret != RK_SUCCESS) {
		return s32Ret;
	}
	return s32Ret;
}

static RK_CHAR optstr[] = "?::W:H:w:h:c:I:e:o:";

static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -I 0 -W 1920 -H 1080 -w 1280 -h 720 -e h264 -o /tmp/venc.h264\n", name);
	printf("\t-W | --width: VI width, Default:1920\n");
	printf("\t-H | --heght: VI height, Default:1080\n");
	printf("\t-w | --disp_width: VPSS width, Default:1280\n");
	printf("\t-h | --disp_height: VPSS height, Default:720\n");
	printf("\t-c | --frame_cnt: frame number of output, Default:-1\n");
	printf("\t-I | --camid: camera ctx id, Default 0. "
	       "0:rkisp_mainpath,1:rkisp_selfpath,2:rkisp_bypasspath\n");
	printf("\t-e | --encode: encode type, Default:h264, Value:h264, h265, mjpeg\n");
	printf("\t-o: output path, Default:NULL\n");
}

int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	MPP_CHN_S stSrcChn, stvpssChn, stvencChn;

	RK_U32 u32Width = 1920;
	RK_U32 u32Height = 1080;
	RK_U32 disp_width = 1280;
	RK_U32 disp_height = 720;

	RK_CHAR *pOutPath = NULL;
	RK_CODEC_ID_E enCodecType = RK_VIDEO_ID_AVC;
	RK_CHAR *pCodecName = "H264";
	RK_S32 s32chnlId = 0;
	int c;
	int ret = -1;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'W':
			u32Width = atoi(optarg);
			break;
		case 'H':
			u32Height = atoi(optarg);
			break;
		case 'w':
			disp_width = atoi(optarg);
			break;
		case 'h':
			disp_height = atoi(optarg);
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
				return -1;
			}
			break;
		case 'o':
			pOutPath = optarg;
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return -1;
		}
	}

	printf("#CodecName:%s\n", pCodecName);
	printf("#INPUT Resolution: %dx%d\n", u32Width, u32Height);
	printf("#VPSS Resolution: %dx%d\n", disp_width, disp_height);
	printf("#Output Path: %s\n", pOutPath);
	printf("#CameraIdx: %d\n\n", s32chnlId);
	printf("#Frame Count to save: %d\n", g_s32FrameCnt);

	if (pOutPath) {
		venc0_file = fopen(pOutPath, "w");
		if (!venc0_file) {
			printf("ERROR: open file: %s fail, exit\n", pOutPath);
			return 0;
		}
	}
	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("rk mpi sys init fail!");
		goto __FAILED;
	}

	vi_dev_init();
	vi_chn_init(s32chnlId, u32Width, u32Height);
	test_vpss_init(0, disp_width, disp_height);
	// venc  init
	test_venc_init(0, disp_width, disp_height, enCodecType);

	// bind vi to venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = s32chnlId;

	stvpssChn.enModId = RK_ID_VPSS;
	stvpssChn.s32DevId = 0;
	stvpssChn.s32ChnId = 0;
	printf("====RK_MPI_SYS_Bind vi0 to vpss0====\n");
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stvpssChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("bind 0 ch venc failed");
		goto __FAILED;
	}

	stvencChn.enModId = RK_ID_VENC;
	stvencChn.s32DevId = 0;
	stvencChn.s32ChnId = 0;
	printf("====RK_MPI_SYS_Bind vpss0 to venc0====\n");
	s32Ret = RK_MPI_SYS_Bind(&stvpssChn, &stvencChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("bind 1 ch venc failed");
		goto __FAILED;
	}

	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer0, NULL);

	while (!quit) {
		usleep(50000);
	}
	pthread_join(main_thread, NULL);
	ret = 0;
__FAILED:

	s32Ret = RK_MPI_SYS_UnBind(&stvpssChn, &stvencChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
	}

	s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stvpssChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
	}

	s32Ret = RK_MPI_VI_DisableChn(0, s32chnlId);
	RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);

	RK_MPI_VPSS_StopGrp(0);
	RK_MPI_VPSS_DestroyGrp(0);

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

	RK_LOGE("test running exit:%d", s32Ret);
	RK_MPI_SYS_Exit();

	return ret;
}
