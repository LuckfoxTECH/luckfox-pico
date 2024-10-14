/*
 * Copyright 2023 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

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

#define VI_NUM_MAX 8
#define VENC_NUM_MAX 2

rtsp_demo_handle g_rtsplive = RK_NULL;
static rtsp_session_handle g_rtsp_session[VENC_NUM_MAX] = {RK_NULL};

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[VI_NUM_MAX];
	SAMPLE_AVS_CTX_S avs;
	SAMPLE_VENC_CTX_S venc[VENC_NUM_MAX];
	SAMPLE_RGN_CTX_S rgn;
} SAMPLE_MPI_CTX_S;

static bool g_bMainThreadQuit = RK_FALSE;
static bool g_bVencThreadQuit[VENC_NUM_MAX] = {RK_FALSE};
pthread_mutex_t g_rtsp_mutex = {0};
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	g_bMainThreadQuit = true;
}

static RK_CHAR optstr[] = "?::a::A:n:b:l:o:e:F:h:m:d:L:v:s:e:i:c:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"calib_file_path", required_argument, NULL, 'A'},
    {"camera_num", required_argument, NULL, 'n'},
    {"bitrate", required_argument, NULL, 'b'},
    {"vi_size", required_argument, NULL, 'v' + 's'},
    {"avs_chn0_size", required_argument, NULL, 'a' + 's'},
    {"avs_chn1_size", required_argument, NULL, 'a' + 'v'},
    {"loop_count", required_argument, NULL, 'l'},
    {"output_path", required_argument, NULL, 'o'},
    {"encode", required_argument, NULL, 'e'},
    {"dstfps", required_argument, NULL, 'F'},
    {"hdr_mode", required_argument, NULL, 'h' + 'm'},
    {"stitch_distance", required_argument, NULL, 'd'},
    {"cam0_ldch_path", required_argument, NULL, 'L'},
    {"cam1_ldch_path", required_argument, NULL, 'L' + 'm'},
    {"set_ldch", required_argument, NULL, 'l' + 'd'},
    {"ispLaunchMode", required_argument, NULL, 'i'},
    {"input_bmp_path", required_argument, NULL, 'i' + 'b'},
    {"osd_display", required_argument, RK_NULL, 'o' + 'd'},
    {"vi_chnid", required_argument, NULL, 'v' + 'i'},
    {"vi_buffcnt", required_argument, NULL, 'v' + 'c'},
    {"avs_mode_blend", required_argument, NULL, 'a' + 'm' + 'b'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s --vi_size 1920x1080 --avs_chn0_size 3840x1080 --avs_chn1_size 1920x544 "
	       "-a /etc/iqfiles/ -e h265cbr -b "
	       "4096 -n 2\n",
	       name);
	printf("\trtsp://xx.xx.xx.xx/live/0, Default OPEN\n");
#ifdef RKAIQ
	printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a "
	       "/etc/iqfiles/, "
	       "set dirpath empty to using path by default, without this option aiq "
	       "should run in other application\n");
#endif
	printf("\t-A | --calib_file_path: input file path of xxx.xml "
	       "Default /oem/usr/share/avs_calib/calib_file.xml\n");
	printf("\t-n | --camera_num: camera number, Default 2\n");
	printf("\t-b | --bitrate: encode bitrate, Default 4096\n");
	printf("\t-l | --loop_count: loop count, Default -1\n");
	printf("\t-o | --output_path: encode output file path, Default NULL\n");
	printf("\t-e | --encode: encode type, Default:h264cbr, Value:h264cbr, "
	       "h264vbr, h265cbr, h265vbr\n");
	printf("\t-F | --dstfps: set venc output fps, Default: 15\n");
	printf("\t-i | --ispLaunchMode : 0: single cam init, 1: camera group init. default: "
	       "1\n");
	printf("\t--vi_size: set vi resolution WidthxHeight, default: 1920x1080\n");
	printf(
	    "\t--avs_chn0_size: set avs chn0 resolution WidthxHeight, default: 3840x1080\n");
	printf(
	    "\t--avs_chn1_size: set avs chn1 resolution WidthxHeight, default: 1920x544\n");
	printf("\t--hdr_mode: set hdr mode, 0: normal 1: HDR2, 2: HDR3, Default: 0\n");
	printf("\t--stitch_distance: set stitch distance, default: 5.0(m)\n");
	printf("\t--cam0_ldch_path: cam0 ldch mesh path, default: "
	       "/oem/usr/share/iqfiles/cam0_ldch_mesh.bin\n");
	printf("\t--cam1_ldch_path: cam1 ldch mesh path, default: "
	       "/oem/usr/share/iqfiles/cam1_ldch_mesh.bin\n");
	printf("\t--set_ldch: set ldch, -1: disable, 1: read_file_set_ldch, 2: "
	       "read_buff_set_ldch. Default: 2\n");
	printf("\t--input_bmp_path : set bmp path for osd, default: NULL\n");
	printf("\t--osd_display : osd if display, 0: no-display, 1: display. default: 1\n");
	printf("\t--vi_chnid : set vi channel id, default: 1\n");
	printf("\t--vi_buffcnt : set vi buff cnt, default: 2\n");
	printf("\t--avs_mode_blend : set avs blend mode, 0: blend, 1 no-blend-ver, 2 "
	       "no-blend-hor. default: 0\n");
}

/******************************************************************************
 * function : venc thread
 ******************************************************************************/
static void *venc_get_stream(void *pArgs) {
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)(pArgs);
	void *pData = RK_NULL;
	char name[256] = {0};
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 loopCount = 0;
	FILE *fp = RK_NULL;

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/venc_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			printf("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			return RK_NULL;
		}
	}

	while (!g_bVencThreadQuit[ctx->s32ChnId]) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					break;
				}
			}

			if (fp) {
				fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}
			pthread_mutex_lock(&g_rtsp_mutex);
			rtsp_tx_video(g_rtsp_session[ctx->s32ChnId], pData,
			              ctx->stFrame.pstPack->u32Len, ctx->stFrame.pstPack->u64PTS);
			rtsp_do_event(g_rtsplive);
			pthread_mutex_unlock(&g_rtsp_mutex);

			RK_LOGD("chn:%d, loopCount:%d wd:%d\n", ctx->s32ChnId, loopCount,
			        ctx->stFrame.pstPack->u32Len);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	if (fp) {
		fclose(fp);
		fp = RK_NULL;
	}
	return RK_NULL;
}

static void handle_pipe(int sig) {
	printf("sigaction will ignore signal %d\n", sig);
	return;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	SAMPLE_MPI_CTX_S *ctx = RK_NULL;
	RK_BOOL bIfOsdDisplay = RK_TRUE;
	RK_U32 u32ViWidth = 1920;
	RK_U32 u32ViHeight = 1080;
	RK_U32 u32AvsChn0Width = 3840;
	RK_U32 u32AvsChn0Height = 1080;
	RK_U32 u32AvsChn1Width = 1920;
	RK_U32 u32AvsChn1Height = 544;
	RK_CHAR *pAvsCalibFilePath = "/oem/usr/share/avs_calib/calib_file.xml";
	RK_CHAR *pAvsMeshAlphaPath = "/tmp/";
	RK_CHAR *pAvsLutFilePath = NULL;
	RK_CHAR *pOutPathVenc = NULL;
	RK_CHAR *pCam0LdchMeshPath = "/oem/usr/share/iqfiles/cam0_ldch_mesh.bin";
	RK_CHAR *pCam1LdchMeshPath = "/oem/usr/share/iqfiles/cam1_ldch_mesh.bin";
	RK_CHAR *pBmpPath = NULL;
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H264CBR;
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	RK_CHAR *pCodecName = "H264";
	MPP_CHN_S stSrcChn, stDestChn;
	RK_S32 s32CamNum = 2;
	RK_S32 s32DstFps = 15;
	RK_S32 s32loopCnt = -1;
	RK_S32 s32BitRate = 4 * 1024;
	RK_S32 s32CamGrpId = 0;
	RK_S32 s32ViChnid = 1;
	RK_S32 s32ViBuffCnt = 2;
	RK_S32 s32AvsModeBlend = 0;
	GET_LDCH_MODE_E eGetLdchMode = RK_GET_LDCH_BY_BUFF;
	RK_FLOAT fStitchDistance = 5;
	RK_VOID *pLdchMeshData[VI_NUM_MAX] = {NULL};
	RK_BOOL bIfIspGroupInit = RK_TRUE;

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	if (!ctx) {
		RK_LOGE("malloc for SAMPLE_MPI_CTX_S ctx failure");
		return RK_FAILURE;
	}
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	if (RK_SUCCESS != pthread_mutex_init(&g_rtsp_mutex, RK_NULL)) {
		RK_LOGE("pthread_mutex_init failure");
		goto __FAILED2;
	}

	signal(SIGINT, sigterm_handler);
	struct sigaction action;
	action.sa_handler = handle_pipe;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	sigaction(SIGPIPE, &action, NULL);

#ifdef RKAIQ
	RK_BOOL bMultictx = RK_FALSE;
#endif
	int c;
	char *iq_file_dir = "/etc/iqfiles/";
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
				iq_file_dir = NULL;
			}
			break;
		case 'A':
			pAvsCalibFilePath = optarg;
			break;
		case 'b':
			s32BitRate = atoi(optarg);
			break;
		case 'e':
			if (!strcmp(optarg, "h264cbr")) {
				enCodecType = RK_CODEC_TYPE_H264;
				enRcMode = VENC_RC_MODE_H264CBR;
				pCodecName = "H264";
			} else if (!strcmp(optarg, "h264vbr")) {
				enCodecType = RK_CODEC_TYPE_H264;
				enRcMode = VENC_RC_MODE_H264VBR;
				pCodecName = "H264";
			} else if (!strcmp(optarg, "h265cbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265CBR;
				pCodecName = "H265";
			} else if (!strcmp(optarg, "h265vbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265VBR;
				pCodecName = "H265";
			} else {
				RK_LOGE("this demo isn't support this encoder type.");
				return 0;
			}
			break;
		case 'n':
			s32CamNum = atoi(optarg);
			break;
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case 'o':
			pOutPathVenc = optarg;
			break;
		case 'F':
			s32DstFps = atoi(optarg);
			break;
		case 'd':
			fStitchDistance = atof(optarg);
			break;
		case 'L':
			pCam0LdchMeshPath = optarg;
			break;
		case 'L' + 'm':
			pCam1LdchMeshPath = optarg;
			break;
		case 'v' + 's':
			u32ViWidth = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32ViHeight = atoi(tmp_optarg + 1);
			break;
		case 'a' + 's':
			u32AvsChn0Width = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32AvsChn0Height = atoi(tmp_optarg + 1);
			break;
		case 'a' + 'v':
			u32AvsChn1Width = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32AvsChn1Height = atoi(tmp_optarg + 1);
			break;
		case 'h' + 'm':
			if (atoi(optarg) == 0) {
				hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
			} else if (atoi(optarg) == 1) {
				hdr_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
			} else if (atoi(optarg) == 2) {
				hdr_mode = RK_AIQ_WORKING_MODE_ISP_HDR3;
			} else {
				RK_LOGE("input hdr_mode is not support(error)");
				print_usage(argv[0]);
				goto __FAILED2;
			}
			break;
		case 'l' + 'd':
			eGetLdchMode = atoi(optarg);
			break;
		case 'i':
			bIfIspGroupInit = atoi(optarg);
			break;
		case 'i' + 'b':
			pBmpPath = optarg;
			break;
		case 'o' + 'd':
			bIfOsdDisplay = atoi(optarg);
			break;
		case 'v' + 'i':
			s32ViChnid = atoi(optarg);
			break;
		case 'v' + 'c':
			s32ViBuffCnt = atoi(optarg);
			break;
		case 'a' + 'm' + 'b':
			s32AvsModeBlend = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#CameraGrpIdx: %d\n", s32CamGrpId);
	printf("#pAvsLutFilePath: %s\n", pAvsLutFilePath);
	printf("#pAvsCalibFilePath: %s\n", pAvsCalibFilePath);
	printf("#pAvsMeshAlphaPath: %s\n", pAvsMeshAlphaPath);
	printf("#CodecName:%s\n", pCodecName);
	printf("#Output Path: %s\n", pOutPathVenc);
	printf("#IQ Path: %s\n", iq_file_dir);
	printf("#fStitchDistance: %f\n", fStitchDistance);
	printf("#pCam0LdchMeshPath: %s\n", pCam0LdchMeshPath);
	printf("#pCam1LdchMeshPath: %s\n", pCam1LdchMeshPath);
	printf("#AvsModeBlend: %d\n", s32AvsModeBlend);

	if (eGetLdchMode == RK_GET_LDCH_BY_FILE && pCam0LdchMeshPath && pCam1LdchMeshPath) {
		pLdchMeshData[0] = pCam0LdchMeshPath;
		pLdchMeshData[1] = pCam1LdchMeshPath;
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}

	/* Init avs[0] */
	ctx->avs.s32GrpId = 0;
	ctx->avs.s32ChnId = 0;
	ctx->avs.eGetLdchMode = eGetLdchMode;
	ctx->avs.u32SrcWidth = u32ViWidth;
	ctx->avs.u32SrcHeight = u32ViHeight;
	ctx->avs.fDistance = fStitchDistance;
	ctx->avs.pLdchMeshData = (RK_U16 **)pLdchMeshData;
	/* GrpAttr setting */
	ctx->avs.stAvsGrpAttr.enMode = s32AvsModeBlend; /* 0: blend 1: no blend */
	ctx->avs.stAvsGrpAttr.u32PipeNum = s32CamNum;
	ctx->avs.stAvsGrpAttr.stGainAttr.enMode = AVS_GAIN_MODE_AUTO;
	ctx->avs.stAvsGrpAttr.stOutAttr.enPrjMode = AVS_PROJECTION_EQUIRECTANGULAR;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Width = u32AvsChn0Width;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Height = u32AvsChn0Height;
	ctx->avs.stAvsGrpAttr.bSyncPipe = RK_TRUE;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Width = u32ViWidth;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Height = u32ViHeight;
	ctx->avs.stAvsGrpAttr.stOutAttr.fDistance = fStitchDistance;
	ctx->avs.stAvsGrpAttr.stInAttr.enParamSource = AVS_PARAM_SOURCE_CALIB;
	ctx->avs.stAvsGrpAttr.stInAttr.stCalib.pCalibFilePath = pAvsCalibFilePath;
	/* set chn0 */
	ctx->avs.stAvsChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 2;
#ifdef RV1126
	ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 4;
#endif
	ctx->avs.stAvsChnAttr[0].u32Depth = 0;
	ctx->avs.stAvsChnAttr[0].u32Width = u32AvsChn0Width;
	ctx->avs.stAvsChnAttr[0].u32Height = u32AvsChn0Height;
	ctx->avs.stAvsChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	/* set chn1 */
	ctx->avs.stAvsChnAttr[1].enCompressMode = COMPRESS_MODE_NONE;
	ctx->avs.stAvsChnAttr[1].stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsChnAttr[1].stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsChnAttr[1].u32FrameBufCnt = 2;
#ifdef RV1126
	ctx->avs.stAvsChnAttr[1].u32FrameBufCnt = 4;
#endif
	ctx->avs.stAvsChnAttr[1].u32Depth = 0;
	ctx->avs.stAvsChnAttr[1].u32Width = u32AvsChn1Width;
	ctx->avs.stAvsChnAttr[1].u32Height = u32AvsChn1Height;
	ctx->avs.stAvsChnAttr[1].enDynamicRange = DYNAMIC_RANGE_SDR8;
	/* avs create */
	s32Ret = SAMPLE_COMM_AVS_CreateGrp(&ctx->avs);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_AVS_CreateGrp failure:%#X", s32Ret);
	}

	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);
		if (bIfIspGroupInit == RK_FALSE) {
			for (RK_S32 i = 0; i < s32CamNum; i++) {
				s32Ret = SAMPLE_COMM_ISP_Init(i, hdr_mode, bMultictx, iq_file_dir);
				s32Ret |= SAMPLE_COMM_ISP_Run(i);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("ISP init failure camid:%d", i);
					return RK_FAILURE;
				}
			}
		} else {
#ifdef RKAIQ_GRP
			rk_aiq_camgroup_instance_cfg_t camgroup_cfg;

			memset(&camgroup_cfg, 0, sizeof(camgroup_cfg));
			camgroup_cfg.sns_num = s32CamNum;
			camgroup_cfg.config_file_dir = iq_file_dir;
			s32Ret =
			    SAMPLE_COMM_ISP_CamGroup_Init(s32CamGrpId, hdr_mode, bMultictx,
			                                  eGetLdchMode, pLdchMeshData, &camgroup_cfg);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init failure");
				return -1;
			} else {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init success");
			}
			s32Ret = SAMPLE_COMM_ISP_CamGroup_SetFrameRate(s32CamGrpId, s32DstFps);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_SetFrameRate failure");
			} else {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_SetFrameRate success");
			}
#endif
		}

#endif
	}

	// init rtsp
	RK_CHAR str_buff[255] = {0};
	g_rtsplive = create_rtsp_demo(554);
	for (RK_S32 i = 0; i < VENC_NUM_MAX; i++) {
		sprintf(str_buff, "/live/%d", i);
		g_rtsp_session[i] = rtsp_new_session(g_rtsplive, str_buff);
		if (enCodecType == RK_CODEC_TYPE_H264) {
			rtsp_set_video(g_rtsp_session[i], RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
		} else if (enCodecType == RK_CODEC_TYPE_H265) {
			rtsp_set_video(g_rtsp_session[i], RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
		} else {
			printf("not support other type\n");
			return -1;
		}
		rtsp_sync_video_ts(g_rtsp_session[i], rtsp_get_reltime(), rtsp_get_ntptime());
	}

	/* init vi */
	for (RK_S32 i = 0; i < s32CamNum; i++) {
		ctx->vi[i].u32Width = u32ViWidth;
		ctx->vi[i].u32Height = u32ViHeight;
		ctx->vi[i].s32DevId = i;
		ctx->vi[i].u32PipeId = i;
		ctx->vi[i].s32ChnId = s32ViChnid;
		ctx->vi[i].bIfIspGroupInit = bIfIspGroupInit;
#ifdef RV1126
		ctx->vi[i].bIfIspGroupInit = RK_FALSE;
#endif
		ctx->vi[i].stChnAttr.stIspOpt.u32BufCount = s32ViBuffCnt;
		ctx->vi[i].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
		ctx->vi[i].stChnAttr.u32Depth = 0;
		ctx->vi[i].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
		ctx->vi[i].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
		ctx->vi[i].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
		ctx->vi[i].stChnAttr.stFrameRate.s32DstFrameRate = -1;
		SAMPLE_COMM_VI_CreateChn(&ctx->vi[i]);
	}

	for (RK_S32 i = 0; i < s32CamNum && ctx->vi[i].bIfIspGroupInit; i++) {
		s32Ret = RK_MPI_VI_StartPipe(ctx->vi[i].u32PipeId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_StartPipe failure:$#X pipe:%d", s32Ret,
			        ctx->vi[i].u32PipeId);
			goto __VI_INITFAIL;
		}
	}

	/* avs startgrp*/
	s32Ret = SAMPLE_COMM_AVS_StartGrp(&ctx->avs);
	if (s32Ret != RK_SUCCESS) {
		printf("func: %s  line: %d avs create failure!/n", __func__, __LINE__);
		goto __AVS_FAILED;
	}

	/* Init VENC[0] */
	ctx->venc[0].s32ChnId = 0;
	ctx->venc[0].u32Width = u32AvsChn0Width;
	ctx->venc[0].u32Height = u32AvsChn0Height;
	ctx->venc[0].u32Fps = s32DstFps;
	ctx->venc[0].u32Gop = s32DstFps * 2;
	ctx->venc[0].u32BitRate = s32BitRate;
	ctx->venc[0].enCodecType = enCodecType;
	ctx->venc[0].enRcMode = enRcMode;
	ctx->venc[0].getStreamCbFunc = venc_get_stream;
	ctx->venc[0].s32loopCount = s32loopCnt;
	ctx->venc[0].dstFilePath = pOutPathVenc;
	ctx->venc[0].u32BuffSize = ctx->venc[0].u32Width * ctx->venc[0].u32Height / 3;
	ctx->venc[0].u32StreamBufCnt = 3;
#ifdef RV1126
	ctx->venc[0].u32StreamBufCnt = 4;
#endif
	ctx->venc[0].enable_buf_share = RK_TRUE;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	if (enCodecType == RK_CODEC_TYPE_H264) {
		ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 66;
	} else {
		ctx->venc[0].stChnAttr.stVencAttr.u32Profile = 0;
	}
	ctx->venc[0].stChnAttr.stGopAttr.enGopMode =
	    VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc[0]);

	/* Init VENC[1] */
	ctx->venc[1].s32ChnId = 1;
	ctx->venc[1].u32Width = u32AvsChn1Width;
	ctx->venc[1].u32Height = u32AvsChn1Height;
	ctx->venc[1].u32Fps = s32DstFps;
	ctx->venc[1].u32Gop = s32DstFps * 2;
	ctx->venc[1].enCodecType = enCodecType;
	ctx->venc[1].enRcMode = enRcMode;
	ctx->venc[1].getStreamCbFunc = venc_get_stream;
	ctx->venc[1].s32loopCount = s32loopCnt;
	ctx->venc[1].dstFilePath = pOutPathVenc;
	ctx->venc[1].u32BuffSize = ctx->venc[1].u32Width * ctx->venc[1].u32Height / 3;
	ctx->venc[1].u32BitRate = 1024;
	ctx->venc[1].u32StreamBufCnt = 3;
#ifdef RV1126
	ctx->venc[1].u32StreamBufCnt = 4;
#endif
	ctx->venc[1].enable_buf_share = RK_TRUE;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	if (enCodecType == RK_CODEC_TYPE_H264) {
		ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 66;
	} else {
		ctx->venc[1].stChnAttr.stVencAttr.u32Profile = 0;
	}
	ctx->venc[1].stChnAttr.stGopAttr.enGopMode =
	    VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc[1]);

	/* Init RGN */
	RK_U32 u32BmpWidth = 0;
	RK_U32 u32BmpHeight = 0;
	s32Ret = SAMPLE_COMM_GetBmpResolution(pBmpPath, &u32BmpWidth, &u32BmpHeight);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
		u32BmpWidth = 128;
		u32BmpHeight = 128;
	}
	ctx->rgn.rgnHandle = 0;
	ctx->rgn.stRgnAttr.enType = OVERLAY_RGN;
	ctx->rgn.stMppChn.enModId = RK_ID_VENC;
	ctx->rgn.stMppChn.s32ChnId = ctx->venc[0].s32ChnId;
	ctx->rgn.stMppChn.s32DevId = 0;
	ctx->rgn.stRegion.s32X = ctx->venc[0].u32Width / 2; // must be 16 aligned
	ctx->rgn.stRegion.s32Y = 0;                         // must be 16 aligned
	ctx->rgn.stRegion.u32Width = u32BmpWidth;           // must be 16 aligned
	ctx->rgn.stRegion.u32Height = u32BmpHeight;         // must be 16 aligned
	ctx->rgn.u32BmpFormat = RK_FMT_BGRA5551;
	ctx->rgn.u32BgAlpha = 128;
	ctx->rgn.u32FgAlpha = 128;
	ctx->rgn.srcFileBmpName = pBmpPath;
	ctx->rgn.u32Layer = 1;
	if (bIfOsdDisplay) {
		SAMPLE_COMM_RGN_CreateChn(&ctx->rgn);
	}

	// Bind VI[0] and avs[0]
	for (RK_S32 i = 0; i < s32CamNum; i++) {
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi[i].s32DevId;
		stSrcChn.s32ChnId = ctx->vi[i].s32ChnId;
		stDestChn.enModId = RK_ID_AVS;
		stDestChn.s32DevId = ctx->avs.s32GrpId;
		stDestChn.s32ChnId = i;
		SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	}

	/* Bind AVS[0,0] and VENC[0,0] */
	stSrcChn.enModId = RK_ID_AVS;
	stSrcChn.s32DevId = ctx->avs.s32GrpId;
	stSrcChn.s32ChnId = ctx->avs.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[0].s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	/* Bind AVS[0,1] and VENC[0,1] */
	stSrcChn.enModId = RK_ID_AVS;
	stSrcChn.s32DevId = ctx->avs.s32GrpId;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[1].s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	printf("%s initial finish\n", __func__);

	while (!g_bMainThreadQuit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);
	/* Destroy RGN[0] */
	if (bIfOsdDisplay) {
		SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn);
	}
	// Destroy VENC[0]
	g_bVencThreadQuit[0] = RK_TRUE;
	pthread_join(ctx->venc[0].getStreamThread, NULL);
	/* Bind AVS[0,0] and VENC[0,0] */
	stSrcChn.enModId = RK_ID_AVS;
	stSrcChn.s32DevId = ctx->avs.s32GrpId;
	stSrcChn.s32ChnId = ctx->avs.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[0].s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_UnBind failure:%#X", s32Ret);
	}
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[0]);

	// Destroy VENC[1]
	g_bVencThreadQuit[1] = RK_TRUE;
	pthread_join(ctx->venc[1].getStreamThread, NULL);
	/* Bind AVS[0,1] and VENC[0,1] */
	stSrcChn.enModId = RK_ID_AVS;
	stSrcChn.s32DevId = ctx->avs.s32GrpId;
	stSrcChn.s32ChnId = 1;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc[1].s32ChnId;
	s32Ret = SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_UnBind failure:%#X", s32Ret);
	}
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc[1]);

	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);

	// UnBind Bind VI[0]~VI[5] and AVS[0]
	for (RK_S32 i = 0; i < s32CamNum; i++) {
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi[i].s32DevId;
		stSrcChn.s32ChnId = ctx->vi[i].s32ChnId;
		stDestChn.enModId = RK_ID_AVS;
		stDestChn.s32DevId = ctx->avs.s32GrpId;
		stDestChn.s32ChnId = i;
		SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	}

	// Destroy AVS[0]
	SAMPLE_COMM_AVS_StopGrp(&ctx->avs);
	SAMPLE_COMM_AVS_DestroyGrp(&ctx->avs);

__AVS_FAILED:

	for (RK_S32 i = 0; i < s32CamNum && ctx->vi[i].bIfIspGroupInit; i++) {
		s32Ret = RK_MPI_VI_StopPipe(ctx->vi[i].u32PipeId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_StopPipe failure:$#X pipe:%d", s32Ret,
			        ctx->vi[i].u32PipeId);
		}
	}

__VI_INITFAIL:
	// Destroy VI[0]
	for (RK_S32 i = 0; i < s32CamNum; i++) {
		SAMPLE_COMM_VI_DestroyChn(&ctx->vi[i]);
	}

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		if (bIfIspGroupInit == RK_FALSE) {
			for (RK_S32 i = 0; i < s32CamNum; i++) {
				s32Ret = SAMPLE_COMM_ISP_Stop(i);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("SAMPLE_COMM_ISP_Stop failure:%#X", s32Ret);
					return s32Ret;
				}
			}
		} else if (bIfIspGroupInit == RK_TRUE) {
#ifdef RKAIQ_GRP
			s32Ret = SAMPLE_COMM_ISP_CamGroup_Stop(s32CamGrpId);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Stop failure:%#X", s32Ret);
				return s32Ret;
			}
#endif
		} else {
			RK_LOGE("ISP deinit dosen't support this mode %d", bIfIspGroupInit);
			return RK_FAILURE;
		}
#endif
	}
	pthread_mutex_destroy(&g_rtsp_mutex);
__FAILED2:

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
