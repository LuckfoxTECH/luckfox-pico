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
#define RGN_NUM_MAX 2

rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session;

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[VI_NUM_MAX];
	SAMPLE_AVS_CTX_S avs;
	SAMPLE_VENC_CTX_S venc;
	SAMPLE_RGN_CTX_S rgn[RGN_NUM_MAX];
	SAMPLE_VPSS_CTX_S vpss;
	SAMPLE_VO_CTX_S vo;
} SAMPLE_MPI_CTX_S;

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_CHAR optstr[] = "?::a::A:n:b:l:o:e:i:M:F:D:m:d:L:v:s:e:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"avs_lut_name", required_argument, NULL, 'A'},
    {"camera_num", required_argument, NULL, 'n'},
    {"bitrate", required_argument, NULL, 'b'},
    {"vi_size", required_argument, NULL, 'v' + 's'},
    {"avs_size", required_argument, NULL, 'a' + 's'},
    {"venc_size", required_argument, NULL, 'e' + 's'},
    {"loop_count", required_argument, NULL, 'l'},
    {"output_path", required_argument, NULL, 'o'},
    {"encode", required_argument, NULL, 'e'},
    {"input_bmp_name", required_argument, NULL, 'i'},
    {"multictx", required_argument, NULL, 'M'},
    {"dstfps", required_argument, NULL, 'F'},
    {"disp_devid", required_argument, NULL, 'D'},
    {"hdr_mode", required_argument, NULL, 'h' + 'm'},
    {"stitch_distance", required_argument, NULL, 'd'},
    {"cam0_ldch_path", required_argument, NULL, 'L'},
    {"cam1_ldch_path", required_argument, NULL, 'L' + 'm'},
    {"ldch", required_argument, NULL, 'l' + 'd'},
    {"ispLaunchMode", required_argument, NULL, 'i' + 'm'},
    {"vi_chnid", required_argument, NULL, 'v' + 'i'},
    {"vi_buffcnt", required_argument, NULL, 'v' + 'c'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s --vi_size 1920x1080 --avs_size 3840x1080 -a /etc/iqfiles/ -e h265cbr -b "
	       "4096 -n 6 -i "
	       "/usr/share/image.bmp -o /data/\n",
	       name);
	printf("\trtsp://xx.xx.xx.xx/live/0, Default OPEN\n");
#ifdef RKAIQ
	printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a "
	       "/etc/iqfiles/, "
	       "set dirpath empty to using path by default, without this option aiq "
	       "should run in other application\n");
	printf("\t-M | --multictx: switch of multictx in isp, set 0 to disable, set "
	       "1 to enable. Default: 0\n");
#endif
	printf("\t-A | --calib_file_path: input file path of xxx.xml "
	       "Default /oem/usr/share/avs_calib/calib_file.xml\n");
	printf("\t-n | --camera_num: camera number, Default 6\n");
	printf("\t-b | --bitrate: encode bitrate, Default 4096\n");
	printf("\t-l | --loop_count: loop count, Default -1\n");
	printf("\t-o | --output_path: encode output file path, Default NULL\n");
	printf("\t-e | --encode: encode type, Default:h264cbr, Value:h264cbr, "
	       "h264vbr, h264avbr "
	       "h265cbr, h265vbr, h265avbr, mjpegcbr, mjpegvbr\n");
	printf("\t-i | --input_bmp_name: input file path of logo.bmp, Default NULL\n");
	printf("\t-F | --set venc output fps, Default 25\n");
	printf("\t-D | --disp_devid, display dev id, default: -1\n");
	printf("\t--vi_size: set vi resolution WidthxHeight, default: 1920x1080\n");
	printf("\t--avs_size: set avs resolution WidthxHeight, default: 3840x1080\n");
	printf("\t--venc_size: set venc resolution WidthxHeight, default: 3840x1080\n");
	printf("\t--hdr_mode: set hdr mode, 0: normal 1: HDR2, 2: HDR3, Default: 0\n");
	printf("\t--stitch_distance: set stitch distance, default: 5.0(m)\n");
	printf("\t--cam0_ldch_path: cam0 ldch mesh path, default: "
	       "/oem/usr/share/iqfiles/cam0_ldch_mesh.bin\n");
	printf("\t--cam1_ldch_path: cam1 ldch mesh path, default: "
	       "/oem/usr/share/iqfiles/cam1_ldch_mesh.bin\n");
	printf("\t--ldch: set ldch, -1: disable, 1: read_file_set_ldch, 2: "
	       "read_buff_set_ldch. Default: 2\n");
	printf("\t--ispLaunchMode : 0: single cam init, 1: camera group init. default: 1\n");
	printf("\t--vi_chnid : set vi channel id, default: 2\n");
	printf("\t--vi_buffcnt : set vi buff cnt, default: 2\n");
}

/******************************************************************************
 * function : venc thread
 ******************************************************************************/
static void *venc_get_stream(void *pArgs) {
	SAMPLE_VENC_CTX_S *ctx = (SAMPLE_VENC_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	char name[256] = {0};
	FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/venc_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			printf("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			quit = true;
			return RK_NULL;
		}
	}

	while (!quit) {
		s32Ret = SAMPLE_COMM_VENC_GetStream(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VENC_ReleaseStream(ctx);
					quit = true;
					break;
				}
			}

			if (fp) {
				fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}

			PrintStreamDetails(ctx->s32ChnId, ctx->stFrame.pstPack->u32Len);
			rtsp_tx_video(g_rtsp_session, pData, ctx->stFrame.pstPack->u32Len,
			              ctx->stFrame.pstPack->u64PTS);
			rtsp_do_event(g_rtsplive);

			RK_LOGD("chn:%d, loopCount:%d wd:%d\n", ctx->s32ChnId, loopCount,
			        ctx->stFrame.pstPack->u32Len);

			SAMPLE_COMM_VENC_ReleaseStream(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	if (fp)
		fclose(fp);

	return RK_NULL;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	SAMPLE_MPI_CTX_S *ctx;
	RK_BOOL bIfIspGroupInit = RK_TRUE;
	RK_U32 u32ViWidth = 1920;
	RK_U32 u32ViHeight = 1080;
	RK_U32 u32AvsWidth = 3840;
	RK_U32 u32AvsHeight = 1080;
	RK_U32 u32VencWidth = 3840;
	RK_U32 u32VencHeight = 1080;
	RK_U32 disp_width = 1080;
	RK_U32 disp_height = 1920;
	RK_CHAR *pAvsCalibFilePath = "/oem/usr/share/avs_calib/calib_file.xml";
	RK_CHAR *pAvsMeshAlphaPath = "/tmp/";
	RK_CHAR *pAvsLutFilePath = NULL;
	RK_CHAR *pInPathBmp = NULL;
	RK_CHAR *pOutPathVenc = NULL;
	RK_CHAR *pCam0LdchMeshPath = "/oem/usr/share/iqfiles/cam0_ldch_mesh.bin";
	RK_CHAR *pCam1LdchMeshPath = "/oem/usr/share/iqfiles/cam1_ldch_mesh.bin";
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H264CBR;
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	RK_CHAR *pCodecName = "H264";
	RK_S32 s32CamId = 0;
	RK_S32 s32DisId = -1;
	RK_S32 s32DisLayerId = 0;
	MPP_CHN_S stSrcChn, stDestChn;
	RK_S32 s32CamNum = 6;
	RK_S32 s32DstFps = 15;
	RK_S32 s32loopCnt = -1;
	RK_S32 s32BitRate = 4 * 1024;
	RK_S32 s32CamGrpId = 0;
	RK_S32 s32ViChnid = 2;
	RK_S32 s32ViBuffCnt = 2;
	GET_LDCH_MODE_E eGetLdchMode = RK_GET_LDCH_BY_BUFF;
	RK_S32 i;
	RK_FLOAT fStitchDistance = 5;
	RK_VOID *pLdchMeshData[VI_NUM_MAX] = {0};

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	signal(SIGINT, sigterm_handler);

#ifdef RKAIQ
	RK_BOOL bMultictx = RK_FALSE;
#endif
	int c;
	char *iq_file_dir = NULL;
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
			// pAvsLutFilePath = optarg;
			pAvsCalibFilePath = optarg;
			break;
		case 'b':
			s32BitRate = atoi(optarg);
			break;
		case 'D':
			s32DisId = atoi(optarg);
			if (s32DisId == 3) { // MIPI
				disp_width = 1080;
				disp_height = 1920;
			} else {
				disp_width = 1920;
				disp_height = 1080;
			}
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
			} else if (!strcmp(optarg, "h264avbr")) {
				enCodecType = RK_CODEC_TYPE_H264;
				enRcMode = VENC_RC_MODE_H264AVBR;
				pCodecName = "H264";
			} else if (!strcmp(optarg, "h265cbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265CBR;
				pCodecName = "H265";
			} else if (!strcmp(optarg, "h265vbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265VBR;
				pCodecName = "H265";
			} else if (!strcmp(optarg, "h265avbr")) {
				enCodecType = RK_CODEC_TYPE_H265;
				enRcMode = VENC_RC_MODE_H265AVBR;
				pCodecName = "H265";
			} else if (!strcmp(optarg, "mjpegcbr")) {
				enCodecType = RK_CODEC_TYPE_MJPEG;
				enRcMode = VENC_RC_MODE_MJPEGCBR;
				pCodecName = "MJPEG";
			} else if (!strcmp(optarg, "mjpegvbr")) {
				enCodecType = RK_CODEC_TYPE_MJPEG;
				enRcMode = VENC_RC_MODE_MJPEGVBR;
				pCodecName = "MJPEG";
			} else {
				printf("ERROR: Invalid encoder type.\n");
				return 0;
			}
			break;
		case 'n':
			s32CamNum = atoi(optarg);
			break;
		case 'i':
			pInPathBmp = optarg;
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
			u32AvsWidth = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32AvsHeight = atoi(tmp_optarg + 1);
			break;
		case 'e' + 's':
			u32VencWidth = atoi(optarg);
			tmp_optarg = strstr(optarg, "x");
			u32VencHeight = atoi(tmp_optarg + 1);
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
		case 'i' + 'm':
			bIfIspGroupInit = atoi(optarg);
			break;
#ifdef RKAIQ
		case 'M':
			if (atoi(optarg)) {
				bMultictx = RK_TRUE;
			}
			break;
#endif
		case 'v' + 'i':
			s32ViChnid = atoi(optarg);
			break;
		case 'v' + 'c':
			s32ViBuffCnt = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#CameraIdx: %d\n", s32CamId);
	printf("#pAvsLutFilePath: %s\n", pAvsLutFilePath);
	printf("#pAvsCalibFilePath: %s\n", pAvsCalibFilePath);
	printf("#pAvsMeshAlphaPath: %s\n", pAvsMeshAlphaPath);
	printf("#CodecName:%s\n", pCodecName);
	printf("#Output Path: %s\n", pOutPathVenc);
	printf("#IQ Path: %s\n", iq_file_dir);
	printf("#fStitchDistance: %f\n", fStitchDistance);
	printf("#pCam0LdchMeshPath: %s\n", pCam0LdchMeshPath);
	printf("#pCam1LdchMeshPath: %s\n", pCam1LdchMeshPath);

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
	ctx->avs.stAvsGrpAttr.enMode = 0; /* 0: blend 1: no blend */
	ctx->avs.stAvsGrpAttr.u32PipeNum = s32CamNum;
	ctx->avs.stAvsGrpAttr.stGainAttr.enMode = AVS_GAIN_MODE_AUTO;
	ctx->avs.stAvsGrpAttr.stOutAttr.enPrjMode = AVS_PROJECTION_EQUIRECTANGULAR;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Width = u32AvsWidth;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Height = u32AvsHeight;
	ctx->avs.stAvsGrpAttr.bSyncPipe = RK_TRUE;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Width = u32ViWidth;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Height = u32ViHeight;
	ctx->avs.stAvsGrpAttr.stOutAttr.fDistance = fStitchDistance;
	ctx->avs.stAvsGrpAttr.stInAttr.enParamSource = AVS_PARAM_SOURCE_CALIB;
	ctx->avs.stAvsGrpAttr.stInAttr.stCalib.pCalibFilePath = pAvsCalibFilePath;
	ctx->avs.stAvsChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].u32Depth = 1;
	ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 2;
	ctx->avs.stAvsChnAttr[0].u32Width = u32AvsWidth;
	ctx->avs.stAvsChnAttr[0].u32Height = u32AvsHeight;
	ctx->avs.stAvsChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;

	s32Ret = SAMPLE_COMM_AVS_CreateGrp(&ctx->avs);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("SAMPLE_COMM_AVS_CreateGrp failure:%#X", s32Ret);
		goto __AVS_FAILED;
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
				return RK_FAILURE;
			} else {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init success");
			}
			s32Ret = SAMPLE_COMM_ISP_CamGroup_SetFrameRate(s32CamGrpId, s32DstFps);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_SetFrameRate failure");
				return RK_FAILURE;
			} else {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_SetFrameRate success");
			}
#endif
		}

#endif
	}

	// init rtsp
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/0");
	if (enCodecType == RK_CODEC_TYPE_H264) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	} else if (enCodecType == RK_CODEC_TYPE_H265) {
		rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
	} else {
		printf("not support other type\n");
		return -1;
	}
	rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(), rtsp_get_ntptime());

	// Init VI[0] ~ VI[5]
	for (i = 0; i < s32CamNum; i++) {
		ctx->vi[i].u32Width = u32ViWidth;
		ctx->vi[i].u32Height = u32ViHeight;
		ctx->vi[i].s32DevId = i;
		ctx->vi[i].u32PipeId = i;
		ctx->vi[i].bIfIspGroupInit = bIfIspGroupInit;
#ifdef RV1126
		ctx->vi[i].bIfIspGroupInit = RK_FALSE;
#endif
		ctx->vi[i].s32ChnId = s32ViChnid; // rk3588 mainpath:0 selfpath:1 fbcpath:2
		ctx->vi[i].stChnAttr.stIspOpt.stMaxSize.u32Width = u32ViWidth;
		ctx->vi[i].stChnAttr.stIspOpt.stMaxSize.u32Height = u32ViHeight;
		ctx->vi[i].stChnAttr.stIspOpt.u32BufCount = s32ViBuffCnt;
		ctx->vi[i].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
		ctx->vi[i].stChnAttr.u32Depth = 1;
		ctx->vi[i].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
		ctx->vi[i].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
		ctx->vi[i].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
		ctx->vi[i].stChnAttr.stFrameRate.s32DstFrameRate = -1;
		SAMPLE_COMM_VI_CreateChn(&ctx->vi[i]);
	}

	for (i = 0; i < s32CamNum && ctx->vi[i].bIfIspGroupInit; i++) {
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

	// Init VENC[0]
	ctx->venc.s32ChnId = 0;
	ctx->venc.u32Width = u32VencWidth;
	ctx->venc.u32Height = u32VencHeight;
	ctx->venc.u32Fps = s32DstFps;
	ctx->venc.u32Gop = s32DstFps * 2;
	ctx->venc.u32BitRate = s32BitRate;
	ctx->venc.enCodecType = enCodecType;
	ctx->venc.enRcMode = enRcMode;
	ctx->venc.getStreamCbFunc = venc_get_stream;
	ctx->venc.s32loopCount = s32loopCnt;
	ctx->venc.dstFilePath = pOutPathVenc;
	ctx->venc.u32BuffSize = u32VencWidth * u32VencHeight / 2;
	ctx->venc.u32StreamBufCnt = 2;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	if (enCodecType == RK_CODEC_TYPE_H264) {
		ctx->venc.stChnAttr.stVencAttr.u32Profile = 66;
	} else {
		ctx->venc.stChnAttr.stVencAttr.u32Profile = 0;
	}
	ctx->venc.stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc);

	/* Init RGN[0] */
	ctx->rgn[0].rgnHandle = 0;
	ctx->rgn[0].stRgnAttr.enType = COVER_RGN;
	ctx->rgn[0].stMppChn.enModId = RK_ID_VI;
	ctx->rgn[0].stMppChn.s32ChnId = VI_MAX_CHN_NUM;
	ctx->rgn[0].stMppChn.s32DevId = ctx->vi[0].s32DevId;
	ctx->rgn[0].stRegion.s32X = 0;        // must be 16 aligned
	ctx->rgn[0].stRegion.s32Y = 0;        // must be 16 aligned
	ctx->rgn[0].stRegion.u32Width = 640;  // must be 16 aligned
	ctx->rgn[0].stRegion.u32Height = 640; // must be 16 aligned
	ctx->rgn[0].u32Color = 0x00f800;
	ctx->rgn[0].u32Layer = 1;
	SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[0]);

	/* Init RGN[1] */
	if (pInPathBmp) {
		s32Ret = SAMPLE_COMM_GetBmpResolution(pInPathBmp, &ctx->rgn[1].stRegion.u32Width,
		                                      &ctx->rgn[1].stRegion.u32Height);
		if (s32Ret == RK_SUCCESS) {
			ctx->rgn[1].rgnHandle = 1;
			ctx->rgn[1].stRgnAttr.enType = OVERLAY_RGN;
			ctx->rgn[1].stMppChn.enModId = RK_ID_VENC;
			ctx->rgn[1].stMppChn.s32ChnId = 0;
			ctx->rgn[1].stMppChn.s32DevId = ctx->venc.s32ChnId;
			ctx->rgn[1].stRegion.s32X =
			    RK_ALIGN_2(ctx->venc.u32Width / 2); // must be 16 aligned
			ctx->rgn[1].stRegion.s32Y =
			    RK_ALIGN_2(ctx->venc.u32Height / 2); // must be 16 aligned
			ctx->rgn[1].u32BmpFormat = RK_FMT_BGRA5551;
			ctx->rgn[1].u32BgAlpha = 128;
			ctx->rgn[1].u32FgAlpha = 128;
			ctx->rgn[1].u32Layer = 2;
			ctx->rgn[1].srcFileBmpName = pInPathBmp;
			SAMPLE_COMM_RGN_CreateChn(&ctx->rgn[1]);

		} else {
			RK_LOGE("SAMPLE_COMM_GetBmpResolution failure");
		}
	}

	if (s32DisId >= 0) {
		// Init VPSS[0]
		ctx->vpss.s32GrpId = 0;
		ctx->vpss.s32ChnId = 0;
		// RGA_device: VIDEO_PROC_DEV_RGA GPU_device: VIDEO_PROC_DEV_GPU
		ctx->vpss.enVProcDevType = VIDEO_PROC_DEV_RGA;
		ctx->vpss.stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
		ctx->vpss.stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE; // no compress
		ctx->vpss.s32ChnRotation[0] = ROTATION_0;
		ctx->vpss.stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_AUTO;
		ctx->vpss.stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
		ctx->vpss.stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
		ctx->vpss.stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
		ctx->vpss.stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
		ctx->vpss.stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
		ctx->vpss.stVpssChnAttr[0].u32Width = u32AvsWidth;
		ctx->vpss.stVpssChnAttr[0].u32Height = u32AvsHeight;
		if (s32DisId == 3) { // MIPI
			ctx->vpss.s32ChnRotation[0] = ROTATION_90;
			ctx->vpss.stVpssChnAttr[0].u32Width = u32AvsHeight;
			ctx->vpss.stVpssChnAttr[0].u32Height = u32AvsWidth;
		}
		SAMPLE_COMM_VPSS_CreateChn(&ctx->vpss);

		// Init VO[0]
		ctx->vo.s32DevId = s32DisId;
		ctx->vo.s32ChnId = 0;
		ctx->vo.s32LayerId = s32DisLayerId;
		ctx->vo.Volayer_mode = VO_LAYER_MODE_GRAPHIC;
		ctx->vo.u32DispBufLen = 3;
		ctx->vo.stVoPubAttr.enIntfType = VO_INTF_MIPI;
		ctx->vo.stVoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;
		ctx->vo.stLayerAttr.stDispRect.s32X = 0;
		ctx->vo.stLayerAttr.stDispRect.s32Y = 0;
		ctx->vo.stLayerAttr.stDispRect.u32Width = disp_width;
		ctx->vo.stLayerAttr.stDispRect.u32Height = disp_height;
		ctx->vo.stLayerAttr.stImageSize.u32Width = disp_width;
		ctx->vo.stLayerAttr.stImageSize.u32Height = disp_height;
		ctx->vo.stLayerAttr.u32DispFrmRt = 30;
		ctx->vo.stLayerAttr.enPixFormat = RK_FMT_RGB888;
		ctx->vo.stChnAttr.stRect.s32X = 0;
		ctx->vo.stChnAttr.stRect.s32Y = 0;
		ctx->vo.stChnAttr.stRect.u32Width = disp_width;
		ctx->vo.stChnAttr.stRect.u32Height = disp_height;
		ctx->vo.stChnAttr.u32Priority = 1;
		if (s32DisId == 3) { // MIPI
			ctx->vo.stVoPubAttr.enIntfType = VO_INTF_MIPI;
			ctx->vo.stVoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;
		} else {
			ctx->vo.stVoPubAttr.enIntfType = VO_INTF_HDMI;
			ctx->vo.stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
		}
		SAMPLE_COMM_VO_CreateChn(&ctx->vo);
	}

	// Bind VI[0]~VI[5] and avs[0]
	for (i = 0; i < s32CamNum; i++) {
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi[i].s32DevId;
		stSrcChn.s32ChnId = ctx->vi[i].s32ChnId;
		stDestChn.enModId = RK_ID_AVS;
		stDestChn.s32DevId = ctx->avs.s32GrpId;
		stDestChn.s32ChnId = i;
		SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	}

	// Bind AVS[0] and VENC[0]
	stSrcChn.enModId = RK_ID_AVS;
	stSrcChn.s32DevId = ctx->avs.s32GrpId;
	stSrcChn.s32ChnId = ctx->avs.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	if (s32DisId >= 0) {
		// Bind AVS[0] and VPSS[0]
		stSrcChn.enModId = RK_ID_AVS;
		stSrcChn.s32DevId = ctx->avs.s32GrpId;
		stSrcChn.s32ChnId = ctx->avs.s32ChnId;
		stDestChn.enModId = RK_ID_VPSS;
		stDestChn.s32DevId = ctx->vpss.s32GrpId;
		stDestChn.s32ChnId = ctx->vpss.s32ChnId;
		SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

		// Bind VPSS[0] and VO[0]
		stSrcChn.enModId = RK_ID_VPSS;
		stSrcChn.s32DevId = ctx->vpss.s32GrpId;
		stSrcChn.s32ChnId = ctx->vpss.s32ChnId;
		stDestChn.enModId = RK_ID_VO;
		stDestChn.s32DevId = ctx->vo.s32LayerId;
		stDestChn.s32ChnId = ctx->vo.s32ChnId;
		SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
	}

	printf("%s initial finish\n", __func__);

	while (!quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	if (ctx->venc.getStreamCbFunc) {
		pthread_join(ctx->venc.getStreamThread, NULL);
	}

	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);

	if (s32DisId >= 0) {
		// UnBind VPSS[0] and VO[0]
		stSrcChn.enModId = RK_ID_VPSS;
		stSrcChn.s32DevId = ctx->vpss.s32GrpId;
		stSrcChn.s32ChnId = ctx->vpss.s32ChnId;
		stDestChn.enModId = RK_ID_VO;
		stDestChn.s32DevId = ctx->vo.s32LayerId;
		stDestChn.s32ChnId = ctx->vo.s32ChnId;
		SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

		// UnBind AVS[0] and VPSS[0]
		stSrcChn.enModId = RK_ID_AVS;
		stSrcChn.s32DevId = ctx->avs.s32GrpId;
		stSrcChn.s32ChnId = ctx->avs.s32ChnId;
		stDestChn.enModId = RK_ID_VPSS;
		stDestChn.s32DevId = ctx->vpss.s32GrpId;
		stDestChn.s32ChnId = ctx->vpss.s32ChnId;
		SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	}

	// UnBind avs[0] and VENC[0]
	stSrcChn.enModId = RK_ID_AVS;
	stSrcChn.s32DevId = ctx->avs.s32GrpId;
	stSrcChn.s32ChnId = ctx->avs.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	// UnBind Bind VI[0]~VI[5] and AVS[0]
	for (i = 0; i < s32CamNum; i++) {
		stSrcChn.enModId = RK_ID_VI;
		stSrcChn.s32DevId = ctx->vi[i].s32DevId;
		stSrcChn.s32ChnId = ctx->vi[i].s32ChnId;
		stDestChn.enModId = RK_ID_AVS;
		stDestChn.s32DevId = ctx->avs.s32GrpId;
		stDestChn.s32ChnId = i;
		SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	}

	if (s32DisId >= 0) {
		// Destroy VO[0]
		SAMPLE_COMM_VO_DestroyChn(&ctx->vo);
		// Destroy VPSS[0]
		SAMPLE_COMM_VPSS_DestroyChn(&ctx->vpss);
	}
	// Destroy RGN[1]
	if (pInPathBmp) {
		SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn[1]);
	}

	// Destroy RGN[0]
	SAMPLE_COMM_RGN_DestroyChn(&ctx->rgn[0]);
	// Destroy VENC[0]
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);
	// Destroy AVS[0]
	SAMPLE_COMM_AVS_StopGrp(&ctx->avs);
	SAMPLE_COMM_AVS_DestroyGrp(&ctx->avs);

__AVS_FAILED:

	for (i = 0; i < s32CamNum && ctx->vi[i].bIfIspGroupInit; i++) {
		s32Ret = RK_MPI_VI_StopPipe(ctx->vi[i].u32PipeId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_StopPipe failure:$#X pipe:%d", s32Ret,
			        ctx->vi[i].u32PipeId);
		}
	}

__VI_INITFAIL:
	// Destroy VI[0]
	for (i = 0; i < s32CamNum; i++) {
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
		} else {
#ifdef RKAIQ_GRP
			s32Ret = SAMPLE_COMM_ISP_CamGroup_Stop(s32CamGrpId);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Stop failure:%#X", s32Ret);
				return s32Ret;
			}
#endif
		}
#endif
	}
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
