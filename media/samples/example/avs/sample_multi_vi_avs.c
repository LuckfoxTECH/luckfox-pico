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

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi[VI_NUM_MAX];
	SAMPLE_AVS_CTX_S avs;
} SAMPLE_MPI_CTX_S;

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_CHAR optstr[] = "?::a::A:n:l:o:M:f:L:d:m:v:s:i:c:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"calib_file_path", required_argument, NULL, 'A'},
    {"camera_num", required_argument, NULL, 'n'},
    {"vi_size", required_argument, NULL, 'v' + 's'},
    {"avs_size", required_argument, NULL, 'a' + 's'},
    {"loop_count", required_argument, NULL, 'l'},
    {"output_path", required_argument, NULL, 'o'},
    {"multictx", required_argument, NULL, 'M'},
    {"fps", required_argument, NULL, 'f'},
    {"cam0_ldch_path", required_argument, NULL, 'L'},
    {"cam1_ldch_path", required_argument, NULL, 'L' + 'm'},
    {"stitch_distance", required_argument, NULL, 'd'},
    {"ldch", required_argument, NULL, 'l' + 'd'},
    {"ispLaunchMode", required_argument, NULL, 'i'},
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
	printf("\t%s --vi_size 1920x1080 --avs_size 3840x1080 -a /etc/iqfiles/ -o /data/\n",
	       name);
	printf("\trtsp://xx.xx.xx.xx/live/0. Default OPEN\n");
#ifdef RKAIQ
	printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a "
	       "/etc/iqfiles/, "
	       "set dirpath empty to using path by default, without this option aiq "
	       "should run in other application\n");
	printf("\t-M | --multictx: switch of multictx in isp, set 0 to disable, set "
	       "1 to enable. Default: 0\n");
#endif
	printf("\t-A | --calib_file_path: input file path of xxx.xml. "
	       "Default /oem/usr/share/avs_calib/calib_file.xml\n");
	printf("\t-n | --camera_num: camera number. Default 2\n");
	printf("\t-l | --loop_count: loop count. Default -1\n");
	printf("\t-o | --output_path: avs output file path. Default NULL\n");
	printf("\t-f | --fps: vi framerate. Default: 15\n");
	printf("\t-i | --ispLaunchMode : 0: single cam init, 1: camera group init. default: "
	       "1\n");
	printf("\t--ldch: set ldch, -1: disable, 1: read_file_set_ldch, 2: "
	       "read_buff_set_ldch. Default: 2\n");
	printf("\t--vi_size: set vi resolution WidthxHeight, default: 1920x1080\n");
	printf("\t--avs_size: set avs resolution WidthxHeight, default: 3840x1080\n");
	printf("\t--stitch_distance: set stitch distance, default: 5.0(m)\n");
	printf("\t--cam0_ldch_path: cam0 ldch mesh path, default: "
	       "/oem/usr/share/iqfiles/cam0_ldch_mesh.bin\n");
	printf("\t--cam1_ldch_path: cam1 ldch mesh path, default: "
	       "/oem/usr/share/iqfiles/cam1_ldch_mesh.bin\n");
	printf("\t--vi_chnid : set vi channel id, default: 2\n");
	printf("\t--vi_buffcnt : set vi buff cnt, default: 2\n");
}

/******************************************************************************
 * function : avs thread
 ******************************************************************************/
static void *avs_get_stream(void *pArgs) {
	SAMPLE_AVS_CTX_S *ctx = (SAMPLE_AVS_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	char name[256] = {0};
	FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;
	PIC_BUF_ATTR_S pstBufAttr;
	MB_PIC_CAL_S pstPicCal;

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/avs_%d.bin", ctx->dstFilePath, ctx->s32GrpId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			printf("chn %d can't open %s file !\n", ctx->s32GrpId, ctx->dstFilePath);
			quit = true;
			return RK_NULL;
		}
	}

	while (!quit) {
		s32Ret = SAMPLE_COMM_AVS_GetChnFrame(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_AVS_ReleaseChnFrame(ctx);
					quit = true;
					break;
				}
			}

			if (fp) {
				/* cal frame size */
				pstBufAttr.u32Width = ctx->stVideoFrame.stVFrame.u32Width;
				pstBufAttr.u32Height = ctx->stVideoFrame.stVFrame.u32Height;
				pstBufAttr.enCompMode = ctx->stVideoFrame.stVFrame.enCompressMode;
				pstBufAttr.enPixelFormat = ctx->stVideoFrame.stVFrame.enPixelFormat;
				memset(&pstPicCal, 0, sizeof(MB_PIC_CAL_S));
				s32Ret = RK_MPI_CAL_TDE_GetPicBufferSize(&pstBufAttr, &pstPicCal);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_CAL_TDE_GetPicBufferSize failure s32Ret:%#X", s32Ret);
				} else {
					fwrite(pData, 1, pstPicCal.u32MBSize, fp);
					fflush(fp);
				}
			}

			RK_LOGE("SAMPLE_COMM_AVS_GetChnFrame DevId %d ok:data %p size:%d "
			        "loop:%d pts:%ld ms\n",
			        ctx->s32GrpId, pData, pstPicCal.u32MBSize, loopCount,
			        ctx->stVideoFrame.stVFrame.u64PTS / 1000);
			SAMPLE_COMM_AVS_ReleaseChnFrame(ctx);
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
	RK_U32 u32ViWidth = 1920;
	RK_U32 u32ViHeight = 1080;
	RK_U32 u32AvsWidth = 3840;
	RK_U32 u32AvsHeight = 1080;
	RK_U32 u32ViFps = 15;
	RK_CHAR *pAvsCalibFilePath = "/oem/usr/share/avs_calib/calib_file.xml";
	RK_CHAR *pAvsMeshAlphaPath = "/tmp/";
	RK_CHAR *pAvsLutFilePath = NULL;
	RK_CHAR *pOutPath = NULL;
	RK_CHAR *pCam0LdchMeshPath = "/oem/usr/share/iqfiles/cam0_ldch_mesh.bin";
	RK_CHAR *pCam1LdchMeshPath = "/oem/usr/share/iqfiles/cam1_ldch_mesh.bin";
	RK_S32 s32CamId = 0;
	RK_S32 s32CamGrpId = 0;
	RK_S32 s32CamNum = 2;
	RK_S32 s32loopCnt = -1;
	RK_S32 i;
	RK_S32 s32ViChnid = 2;
	RK_S32 s32ViBuffCnt = 2;
	GET_LDCH_MODE_E eGetLdchMode = RK_GET_LDCH_BY_BUFF;
	MPP_CHN_S stSrcChn, stDestChn;
	pthread_t avs_thread_id;
	RK_FLOAT fStitchDistance = 5;
	RK_BOOL bIfIspGroupInit = RK_TRUE;
	RK_U16 *pLdchMeshData[VI_NUM_MAX] = {NULL};

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
	char *iq_file_dir = "/oem/usr/share/iqfiles/";
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
		case 'n':
			s32CamNum = atoi(optarg);
			break;
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case 'o':
			pOutPath = optarg;
			break;
#ifdef RKAIQ
		case 'M':
			if (atoi(optarg)) {
				bMultictx = RK_TRUE;
			}
			break;
#endif
		case 'f':
			u32ViFps = atoi(optarg);
			break;
		case 'd':
			fStitchDistance = atof(optarg);
			break;
		case 'l' + 'd':
			eGetLdchMode = atoi(optarg);
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
		case 'i':
			bIfIspGroupInit = atoi(optarg);
			break;
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
	printf("#Output Path: %s\n", pOutPath);
	printf("#IQ Path: %s\n", iq_file_dir);
	printf("#fStitchDistance: %f\n", fStitchDistance);
	printf("#pCam0LdchMeshPath: %s\n", pCam0LdchMeshPath);
	printf("#pCam1LdchMeshPath: %s\n", pCam1LdchMeshPath);

	if (eGetLdchMode == RK_GET_LDCH_BY_FILE && pCam0LdchMeshPath && pCam1LdchMeshPath) {
		pLdchMeshData[0] = (short unsigned int *)pCam0LdchMeshPath;
		pLdchMeshData[1] = (short unsigned int *)pCam1LdchMeshPath;
	}

	/* SYS Init */
	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}

	/* Avs grp create */
	ctx->avs.s32GrpId = 0;
	ctx->avs.s32ChnId = 0;
	ctx->avs.eGetLdchMode = eGetLdchMode;
	ctx->avs.u32SrcWidth = u32ViWidth;
	ctx->avs.u32SrcHeight = u32ViHeight;
	ctx->avs.fDistance = fStitchDistance;
	ctx->avs.pLdchMeshData = (RK_U16 **)pLdchMeshData;
	ctx->avs.stAvsGrpAttr.enMode = 0; // 0: blend 1: no blend
	ctx->avs.stAvsGrpAttr.u32PipeNum = s32CamNum;
	ctx->avs.stAvsGrpAttr.stGainAttr.enMode = AVS_GAIN_MODE_AUTO;
	ctx->avs.stAvsGrpAttr.stOutAttr.enPrjMode = AVS_PROJECTION_EQUIRECTANGULAR;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Width = u32AvsWidth;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Height = u32AvsHeight;
	ctx->avs.stAvsGrpAttr.bSyncPipe = RK_TRUE;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stInAttr.enParamSource = AVS_PARAM_SOURCE_CALIB;
	ctx->avs.stAvsGrpAttr.stInAttr.stCalib.pCalibFilePath = pAvsCalibFilePath;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Width = u32ViWidth;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Height = u32ViHeight;
	ctx->avs.stAvsGrpAttr.stOutAttr.fDistance = fStitchDistance;
	// ctx->avs.stAvsGrpAttr.stInAttr.stCalib.pMeshAlphaPath = pAvsMeshAlphaPath;
	ctx->avs.stAvsChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].u32Depth = 1;
	ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 2;
	ctx->avs.stAvsChnAttr[0].u32Width = u32AvsWidth;
	ctx->avs.stAvsChnAttr[0].u32Height = u32AvsHeight;
	ctx->avs.stAvsChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	SAMPLE_COMM_AVS_CreateGrp(&ctx->avs);

	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);
		rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;

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
			                                  eGetLdchMode, (void **)pLdchMeshData, &camgroup_cfg);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init failure");
				return RK_FAILURE;
			} else {
				RK_LOGE("SAMPLE_COMM_ISP_CamGroup_Init success");
			}
			s32Ret = SAMPLE_COMM_ISP_CamGroup_SetFrameRate(s32CamGrpId, u32ViFps);
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

	// Init VI
	for (i = 0; i < s32CamNum; i++) {
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

	for (i = 0; i < s32CamNum && ctx->vi[i].bIfIspGroupInit; i++) {
		s32Ret = RK_MPI_VI_StartPipe(ctx->vi[i].u32PipeId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_StartPipe failure:$#X pipe:%d", s32Ret,
			        ctx->vi[i].u32PipeId);
			goto __VI_INITFAIL;
		}
	}

	// Init avs[0]
	ctx->avs.dstFilePath = pOutPath;
	ctx->avs.s32loopCount = s32loopCnt;
	s32Ret = SAMPLE_COMM_AVS_StartGrp(&ctx->avs);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("avs create failure");
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

	pthread_create(&avs_thread_id, 0, avs_get_stream, (void *)(&ctx->avs));

	printf("%s initial finish\n", __func__);

	while (!quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	pthread_join(avs_thread_id, NULL);

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

	// Destroy AVS[0]
	SAMPLE_COMM_AVS_StopGrp(&ctx->avs);
	SAMPLE_COMM_AVS_DestroyGrp(&ctx->avs);
	// Destroy VI[0]

	for (i = 0; i < s32CamNum && ctx->vi[i].bIfIspGroupInit; i++) {
		s32Ret = RK_MPI_VI_StopPipe(ctx->vi[i].u32PipeId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_StopPipe failure:$#X pipe:%d", s32Ret,
			        ctx->vi[i].u32PipeId);
		}
	}

__VI_INITFAIL:
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
