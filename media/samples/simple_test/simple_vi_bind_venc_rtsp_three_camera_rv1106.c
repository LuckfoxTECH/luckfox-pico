#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>
#include <stdatomic.h>

#include "rtsp_demo.h"
#ifdef RV1126_RV1109
#include <rk_aiq_user_api_camgroup.h>
#include <rk_aiq_user_api_imgproc.h>
#include <rk_aiq_user_api_sysctl.h>
#else
#include <rk_aiq_user_api2_camgroup.h>
#include <rk_aiq_user_api2_imgproc.h>
#include <rk_aiq_user_api2_sysctl.h>
#endif

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

#define MAX_AIQ_CTX 3
#define IS_MULTI_CAM 1
static rk_aiq_sys_ctx_t *g_aiq_ctx[MAX_AIQ_CTX];
rk_aiq_working_mode_t g_WDRMode[MAX_AIQ_CTX];

static int g_width_mipi0 = 1920, g_height_mipi0 = 1080, g_width_mipi1 = 1920, g_height_mipi1 = 1080, g_width_dvp = 1920, g_height_dvp = 1080;
static RK_S32 g_s32FrameCnt = -1;
static RK_U32 g_u32Bitrate = 10 * 1024;
static bool quit = false;

rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session;
static rtsp_session_handle g_rtsp_session1;
static rtsp_session_handle g_rtsp_session2;

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
	int chn = (int)arg;
	void *pData = RK_NULL;
	int loopCount = 0;
	int s32Ret;

	VENC_STREAM_S stFrame;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(chn, &stFrame, 200000);
		if (s32Ret == RK_SUCCESS) {
			if (g_rtsplive && g_rtsp_session && chn == 0) {
				pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
				rtsp_tx_video(g_rtsp_session, pData, stFrame.pstPack->u32Len,
				              stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
			}
			if (g_rtsplive && g_rtsp_session1 && chn == 1) {
				pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
				rtsp_tx_video(g_rtsp_session1, pData, stFrame.pstPack->u32Len,
				              stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
			}
			if (g_rtsplive && g_rtsp_session2 && chn == 2) {
				pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
				rtsp_tx_video(g_rtsp_session2, pData, stFrame.pstPack->u32Len,
				              stFrame.pstPack->u64PTS);
				rtsp_do_event(g_rtsplive);
			}

			RK_U64 nowUs = TEST_COMM_GetNowUs();
			RK_LOGD("chn:%d, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n",
			        chn, loopCount, stFrame.u32Seq, stFrame.pstPack->u32Len,
			        stFrame.pstPack->u64PTS, nowUs - stFrame.pstPack->u64PTS);

			s32Ret = RK_MPI_VENC_ReleaseStream(chn, &stFrame);
			if (s32Ret != RK_SUCCESS) {
				printf("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
			}
			loopCount++;
		}

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
	int s32Ret;
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_ATTR_S stAttr;
	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
	// VENC_CHN_PARAM_S stChnParam;

	memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
	memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));

	if (enType == RK_VIDEO_ID_AVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
		stAttr.stRcAttr.stH264Cbr.u32BitRate = g_u32Bitrate;
		stAttr.stRcAttr.stH264Cbr.u32Gop = 60;
	} else if (enType == RK_VIDEO_ID_HEVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
		stAttr.stRcAttr.stH265Cbr.u32BitRate = g_u32Bitrate;
		stAttr.stRcAttr.stH265Cbr.u32Gop = 60;
	} else if (enType == RK_VIDEO_ID_MJPEG) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
		stAttr.stRcAttr.stMjpegCbr.u32BitRate = g_u32Bitrate;
	}

	stAttr.stVencAttr.enType = enType;
	stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	if (enType == RK_VIDEO_ID_AVC)
		stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
	stAttr.stVencAttr.u32PicWidth = width;
	stAttr.stVencAttr.u32PicHeight = height;
	stAttr.stVencAttr.u32VirWidth = width;
	stAttr.stVencAttr.u32VirHeight = height;
	stAttr.stVencAttr.u32StreamBufCnt = 2;
	stAttr.stVencAttr.u32BufSize = width * height * 3 / 2;
	stAttr.stVencAttr.enMirror = MIRROR_NONE;

	RK_MPI_VENC_CreateChn(chnId, &stAttr);

	stVencChnRefBufShare.bEnable = 1;
	s32Ret = RK_MPI_VENC_SetChnRefBufShareAttr(chnId, &stVencChnRefBufShare);
	if (s32Ret != RK_SUCCESS) {
		printf("RK_MPI_VENC_SetChnRefBufShareAttr failed,ret=%#x\n", s32Ret);
	}

	/* set chn2 to 20 fps */
	// s32Ret = RK_MPI_VENC_GetChnParam(chnId, &stChnParam);
	// if (s32Ret != RK_SUCCESS) {
	// 	printf("RK_MPI_VENC_GetChnParam failed,ret=%#x\n", s32Ret);
	// }
	// if (chnId == 2) {
	// 	stChnParam.stFrameRate.bEnable = 1;
	// 	stChnParam.stFrameRate.s32SrcFrmRateNum = 25;
	// 	stChnParam.stFrameRate.s32SrcFrmRateDen = 1;
	// 	stChnParam.stFrameRate.s32DstFrmRateNum = 20;
	// 	stChnParam.stFrameRate.s32DstFrmRateDen = 1;
	// 	s32Ret = RK_MPI_VENC_SetChnParam(chnId, &stChnParam);
	// }

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

int vi_dev_init(int devId) {
	int ret = 0;
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
			printf("RK_MPI_VI_EnableDev failed devId: %d ret: %x\n", devId, ret);
			return -1;
		}
		// 1-3.bind dev/pipe
		stBindPipe.u32Num = 1;
		stBindPipe.PipeId[0] = pipeId;
		ret = RK_MPI_VI_SetDevBindPipe(devId, &stBindPipe);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VI_SetDevBindPipe failed devId: %d ret %x\n", devId, ret);
			return -1;
		}
	} else {
		printf("RK_MPI_VI_EnableDev already\n");
	}

	return 0;
}

int vi_chn_init(int devId,int channelId, int width, int height) {
	int ret;
	int buf_cnt = 2;//when the three camera is 15fps, buf_cnt can be set to 1
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
	ret = RK_MPI_VI_SetChnAttr(devId, channelId, &vi_chn_attr);
	if (ret) {
		printf("ERROR: RK_MPI_VI_SetChnAttr error! ret=0x%x\n", ret);
		return ret;
	}
	ret = RK_MPI_VI_EnableChn(devId, channelId);
	if (ret) {
		printf("ERROR: RK_MPI_VI_EnableChn error! ret=0x%x\n", ret);
		return ret;
	}

	return ret;
}

RK_S32 SIMPLE_COMM_ISP_Init(RK_S32 CamId, rk_aiq_working_mode_t WDRMode, RK_BOOL MultiCam,
                            const char *iq_file_dir) {
	if (CamId >= MAX_AIQ_CTX) {
		printf("%s : CamId is over 3\n", __FUNCTION__);
		return -1;
	}

	// char *iq_file_dir = "iqfiles/";
	if (iq_file_dir == NULL) {
		printf("SIMPLE_COMM_ISP_Init : not start.\n");
		g_aiq_ctx[CamId] = NULL;
		return 0;
	}

	// must set HDR_MODE, before init
	g_WDRMode[CamId] = WDRMode;
	char hdr_str[16];
	snprintf(hdr_str, sizeof(hdr_str), "%d", (int)WDRMode);
	setenv("HDR_MODE", hdr_str, 1);

	rk_aiq_sys_ctx_t *aiq_ctx;
	rk_aiq_static_info_t aiq_static_info;
	rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(CamId, &aiq_static_info);

	printf("ID: %d, sensor_name is %s, iqfiles is %s\n", CamId,
	       aiq_static_info.sensor_info.sensor_name, iq_file_dir);

	rk_aiq_uapi2_sysctl_preInit_devBufCnt(aiq_static_info.sensor_info.sensor_name, "rkraw_rx", 2);

	aiq_ctx =
	    rk_aiq_uapi2_sysctl_init(aiq_static_info.sensor_info.sensor_name, iq_file_dir,
	                             NULL, NULL);

	if (MultiCam)
		rk_aiq_uapi2_sysctl_setMulCamConc(aiq_ctx, true);

	g_aiq_ctx[CamId] = aiq_ctx;
	return 0;
}

RK_S32 SIMPLE_COMM_ISP_Run(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}

	if (rk_aiq_uapi2_sysctl_prepare(g_aiq_ctx[CamId], 0, 0, g_WDRMode[CamId])) {
		printf("rkaiq engine prepare failed !\n");
		g_aiq_ctx[CamId] = NULL;
		return -1;
	}
	printf("rk_aiq_uapi2_sysctl_init/prepare succeed\n");
	if (rk_aiq_uapi2_sysctl_start(g_aiq_ctx[CamId])) {
		printf("rk_aiq_uapi2_sysctl_start  failed\n");
		return -1;
	}
	printf("rk_aiq_uapi2_sysctl_start succeed\n");

	return 0;
}

RK_S32 SIMPLE_COMM_ISP_Stop(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init g_aiq_ctx[%d] = %p\n", __FUNCTION__,
		       CamId, g_aiq_ctx[CamId]);
		return -1;
	}

	printf("rk_aiq_uapi2_sysctl_stop enter\n");
	rk_aiq_uapi2_sysctl_stop(g_aiq_ctx[CamId], false);
	printf("rk_aiq_uapi2_sysctl_deinit enter\n");
	rk_aiq_uapi2_sysctl_deinit(g_aiq_ctx[CamId]);
	printf("rk_aiq_uapi2_sysctl_deinit exit\n");

	g_aiq_ctx[CamId] = NULL;
	return 0;
}

static RK_CHAR optstr[] = "?:a:c:e:b:h:0:1:2:";

static const struct option long_options[] = {
	{"aiq", optional_argument, NULL, 'a'},
	{"frame_cnt", required_argument, NULL, 'c'},
	{"encode", required_argument, NULL, 'e'},
	{"bitrate", required_argument, NULL, 'b'},
	{"width_mipi0", required_argument, NULL, 'w' + '0'},
	{"height_mipi0", required_argument, NULL, 'h' + '0'},
	{"width_mipi1", required_argument, NULL, 'w' + '1'},
	{"height_mipi1", required_argument, NULL, 'h' + '1'},
	{"width_dvp", required_argument, NULL, 'w' + '2'},
	{"height_dvp", optional_argument, NULL, 'h' + '2'},
	{"help", optional_argument, NULL, 'h'},
	{NULL, 0, NULL, 0},
};

static void print_usage(const RK_CHAR *name) {
	printf("Usage example:\n");
	printf("\t%s --width_mipi0=1920 --height_mipi0=1080 --width_mipi1=1920 --height_mipi1=1080 --width_dvp=1920 --height_dvp=1080 -a /etc/iqfiles\n", name);
	printf("\t--width_mipi0: VI dev0 chn0 width, Default: 1920\n");
	printf("\t--height_mipi0: VI dev0 chn0 height, Default: 1080\n");
	printf("\t--width_mipi1: VI dev1 chn0 width, Default: 1920\n");
	printf("\t--height_mipi1: VI dev1 chn0 height, Default: 1080\n");
	printf("\t--width_dvp: VI dev2 chn0 width, Default: 1920\n");
	printf("\t--height_dvp: VI dev2 chn0 height, Default: 1080\n");
	printf("\t-a | --aiq: iq file path, Default: /etc/iqfiles\n");
	printf("\t-c | --frame_cnt: frame number of output, Default:-1\n");
	printf("\t-e | --encode: encode type, Default:h264, Value:h264 = 0, h265 = 1\n");
	printf("\t-b | --bitrate: set bitrate (Kbps), Default: 10Mbps\n");
}

void vi_bind_venc(int src_dev_id, int src_chn_id, int dst_chn_id) {
	int s32Ret;
	MPP_CHN_S stSrcChn, stDestChn;
	// bind vi to venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = src_dev_id;
	stSrcChn.s32ChnId = src_chn_id;

	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = dst_chn_id;

	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		printf("[%s] bind vi dev%d chn%d venc dev0 chn%d failed ret: 0x%x\n", __func__, src_dev_id, src_chn_id, dst_chn_id, s32Ret);
	}
}

void vi_unbind_venc(int src_dev_id, int src_chn_id, int dst_chn_id) {
	int s32Ret;
	MPP_CHN_S stSrcChn, stDestChn;
	// bind vi to venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = src_dev_id;
	stSrcChn.s32ChnId = src_chn_id;

	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = dst_chn_id;

	s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		printf("[%s] unbind vi dev%d chn%d venc dev0 chn%d failed ret: 0x%x\n", __func__, src_dev_id, src_chn_id, dst_chn_id, s32Ret);
	}
}

void venc_deinit(int chn) {
	int s32Ret;

	RK_MPI_VENC_StopRecvFrame(chn);
	s32Ret = RK_MPI_VENC_DestroyChn(chn);
	if (s32Ret != RK_SUCCESS) {
		printf("RK_MPI_VDEC_DestroyChn fail 0x%x\n", s32Ret);
	}
}

void vi_deinit(int dev, int chn){
	int s32Ret;

	s32Ret = RK_MPI_VI_DisableChn(dev, chn);
	if (s32Ret != RK_SUCCESS) {
		printf("[RK_MPI_VI_DisableChn] vi disable dev %d chn %d failed ret 0x%x\n", dev, chn, s32Ret);
	}

	s32Ret |= RK_MPI_VI_DisableDev(dev);
	if (s32Ret != RK_SUCCESS) {
		printf("[RK_MPI_VI_DisableDev] vi disable dev %d failed ret 0x%x\n", dev, s32Ret);
	}

}

int main(int argc, char *argv[]) {
	RK_CODEC_ID_E enCodecType = RK_VIDEO_ID_AVC;
	RK_S32 encode = 0;
	char *iq_dir = "/etc/iqfiles";
	int c;
	int ret = -1;

	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		switch (c) {
		case 'a':
			if (optarg)
				iq_dir = optarg;
			break;
		case 'w' + '0':
			g_width_mipi0 = atoi(optarg);
			break;
		case 'h' + '0':
			g_height_mipi0 = atoi(optarg);
			break;
		case 'w' + '1':
			g_width_mipi1 = atoi(optarg);
			break;
		case 'h' + '1':
			g_height_mipi1 = atoi(optarg);
			break;
		case 'w' + '2':
			g_width_dvp = atoi(optarg);
			break;
		case 'h' + '2':
			g_height_dvp = atoi(optarg);
			break;
		case 'c':
			g_s32FrameCnt = atoi(optarg);
			break;
		case 'e':
			encode = atoi(optarg);
			break;
		case 'b':
			g_u32Bitrate = atoi(optarg);
			break;
		case 'h':
			print_usage(argv[0]);
			break;
		default:
			print_usage(argv[0]);
			return -1;
		}
	}

	printf("#width_mipi0: %d height_mipi0: %d, width_mipi1: %d height_mipi1: %d, width_dvp: %d height_dvp: %d\n",
		g_width_mipi0, g_height_mipi0, g_width_mipi1, g_height_mipi1, g_width_dvp, g_height_dvp);

	signal(SIGINT, sigterm_handler);

	if (iq_dir) {
		SIMPLE_COMM_ISP_Init(0, RK_AIQ_WORKING_MODE_NORMAL, IS_MULTI_CAM, iq_dir);
		SIMPLE_COMM_ISP_Run(0);
		SIMPLE_COMM_ISP_Init(1, RK_AIQ_WORKING_MODE_NORMAL, IS_MULTI_CAM, iq_dir);
		SIMPLE_COMM_ISP_Run(1);
		SIMPLE_COMM_ISP_Init(2, RK_AIQ_WORKING_MODE_NORMAL, IS_MULTI_CAM, iq_dir);
		SIMPLE_COMM_ISP_Run(2);
		printf("ISP IQ file path: %s\n\n", iq_dir);
	}

	if (encode == 0) {
		enCodecType = RK_VIDEO_ID_AVC;
	} else if (encode == 1) {
		enCodecType = RK_VIDEO_ID_HEVC;
	} else {
		printf("not support other type\n");
		return -1;
	}

	// init rtsp
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/0");
	g_rtsp_session1 = rtsp_new_session(g_rtsplive, "/live/1");
	g_rtsp_session2 = rtsp_new_session(g_rtsplive, "/live/2");
	if (enCodecType == RK_VIDEO_ID_AVC) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		rtsp_set_video(g_rtsp_session1, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		rtsp_set_video(g_rtsp_session2, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	} else if (enCodecType == RK_VIDEO_ID_HEVC) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		rtsp_set_video(g_rtsp_session1, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		rtsp_set_video(g_rtsp_session2, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	} else {
		printf("not support other type\n");
		return -1;
	}
	rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session1, rtsp_get_reltime(), rtsp_get_ntptime());
	rtsp_sync_video_ts(g_rtsp_session2, rtsp_get_reltime(), rtsp_get_ntptime());

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		printf("rk mpi sys init fail!");
		goto __FAILED;
	}

	vi_dev_init(0);
	vi_chn_init(0, 0, g_width_mipi0, g_height_mipi0);
	vi_chn_init(0, 1, 640, 360);
	vi_dev_init(1);
	vi_chn_init(1, 0, g_width_mipi1, g_height_mipi1);
	vi_chn_init(1, 1, 640, 360);
	vi_dev_init(2);
	vi_chn_init(2, 0, g_width_dvp, g_height_dvp);
	vi_chn_init(2, 1, 640, 360);

	// venc  init
	test_venc_init(0, g_width_mipi0, g_height_mipi0, enCodecType);
	test_venc_init(1, g_width_mipi1, g_height_mipi1, enCodecType);
	test_venc_init(2, g_width_dvp, g_height_dvp, enCodecType);
	test_venc_init(3, 640, 360, enCodecType);
	test_venc_init(4, 640, 360, enCodecType);
	test_venc_init(5, 640, 360, enCodecType);

	// main_stream
	vi_bind_venc(0, 0, 0);
	vi_bind_venc(1, 0, 1);
	vi_bind_venc(2, 0, 2);
	// small_stream
	vi_bind_venc(0, 1, 3);
	vi_bind_venc(1, 1, 4);
	vi_bind_venc(2, 1, 5);

	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer0, (int *)0);
	pthread_t main_thread1;
	pthread_create(&main_thread1, NULL, GetMediaBuffer0, (int *)1);
	pthread_t main_thread2;
	pthread_create(&main_thread2, NULL, GetMediaBuffer0, (int *)2);

	pthread_t main_thread3;
	pthread_create(&main_thread3, NULL, GetMediaBuffer0, (int *)3);
	pthread_t main_thread4;
	pthread_create(&main_thread4, NULL, GetMediaBuffer0, (int *)4);
	pthread_t main_thread5;
	pthread_create(&main_thread5, NULL, GetMediaBuffer0, (int *)5);

	while (!quit) {
		usleep(500000);
	}

	pthread_join(main_thread, NULL);
	pthread_join(main_thread1, NULL);
	pthread_join(main_thread2, NULL);
	pthread_join(main_thread3, NULL);
	pthread_join(main_thread4, NULL);
	pthread_join(main_thread5, NULL);

	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);

	// main_stream
	vi_unbind_venc(0, 0, 0);
	vi_unbind_venc(1, 0, 1);
	vi_unbind_venc(2, 0, 2);
	// small_stream
	vi_unbind_venc(0, 1, 3);
	vi_unbind_venc(1, 1, 4);
	vi_unbind_venc(2, 1, 5);

	venc_deinit(0);
	venc_deinit(1);
	venc_deinit(2);
	venc_deinit(3);
	venc_deinit(4);
	venc_deinit(5);

	vi_deinit(0, 0);
	vi_deinit(1, 0);
	vi_deinit(2, 0);
	vi_deinit(0, 1);
	vi_deinit(1, 1);
	vi_deinit(2, 1);
	ret = 0;
__FAILED:
	RK_MPI_SYS_Exit();

	SIMPLE_COMM_ISP_Stop(0);
	SIMPLE_COMM_ISP_Stop(1);
	SIMPLE_COMM_ISP_Stop(2);

	return ret;
}
