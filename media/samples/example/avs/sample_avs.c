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
#include <unistd.h>

#include "sample_comm.h"

#define CAM_NUM_MAX 8

typedef struct _rkMpiCtx {
	SAMPLE_AVS_CTX_S avs;
} SAMPLE_MPI_CTX_S;

static bool quit = false;
static bool send_frame_thread_quit = false;
static bool get_frame_thread_quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_CHAR optstr[] = "?::A:n:w:h:W:H:i:l:o:";
static const struct option long_options[] = {
    {"calib_file_path", required_argument, NULL, 'A'},
    {"camera_num", required_argument, NULL, 'n'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"avs_width", required_argument, NULL, 'W'},
    {"avs_height", required_argument, NULL, 'H'},
    {"loop_count", required_argument, NULL, 'l'},
    {"input_yuv_path", required_argument, NULL, 'i'},
    {"output_path", required_argument, NULL, 'o'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -w 1920 -h 1080 -n 2\n", name);
	printf("\t-A | --calib_file_path: input file path of calib_file_xxx.pto "
	       "Default: /oem/usr/share/avs_calib/calib_file.xml\n");
	printf("\t-n | --camera_num: camera number, Default 2\n");
	printf("\t-w | --width: input yuv width, Default 1920\n");
	printf("\t-h | --height: input yuv height, Default 1080\n");
	printf("\t-W | --avs_width: avs out width, Default 3840\n");
	printf("\t-H | --avs_height: avs out height, Default 1080\n");
	printf("\t-l | --loop_count: loop count, Default -1\n");
	printf("\t-i | --input_yuv_path: avs input file folder, Default: NULL, need "
	       "0~x.yuv\n");
	printf("\t-o | --output_path: avs output file path, Default NULL\n");
}

/******************************************************************************
 * function : avs send thread
 ******************************************************************************/
static void *avs_send_stream(void *pArgs) {
	SAMPLE_AVS_CTX_S *ctx = (SAMPLE_AVS_CTX_S *)(pArgs);
	char name[256] = {0};
	FILE *fp[CAM_NUM_MAX] = {RK_NULL};
	RK_S32 loopCount = 0;
	RK_S32 s32Ret = RK_FAILURE;
	VIDEO_FRAME_INFO_S stVideoFrames[CAM_NUM_MAX] = {0};
	MB_POOL mbPipePool[CAM_NUM_MAX] = {0};
	MB_BLK mbApplyBlk = RK_NULL;
	PIC_BUF_ATTR_S pstBufAttr = {0};
	MB_PIC_CAL_S pstPicCal = {0};
	MB_POOL_CONFIG_S stMbPoolCfg = {0};

	pstBufAttr.u32Width = ctx->u32SrcWidth;
	pstBufAttr.u32Height = ctx->u32SrcHeight;
	pstBufAttr.enCompMode = COMPRESS_MODE_NONE;
	pstBufAttr.enPixelFormat = RK_FMT_YUV420SP;
	s32Ret = RK_MPI_CAL_TDE_GetPicBufferSize(&pstBufAttr, &pstPicCal);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_CAL_TDE_GetPicBufferSize failure:%#X", s32Ret);
		return RK_NULL;
	}

	/* mb pool create */
	for (RK_S32 i = 0; i < ctx->stAvsGrpAttr.u32PipeNum; i++) {
		memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
		stMbPoolCfg.u64MBSize = pstPicCal.u32MBSize;
		stMbPoolCfg.u32MBCnt = 3;
		stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
		stMbPoolCfg.bPreAlloc = RK_TRUE;
		mbPipePool[i] = RK_MPI_MB_CreatePool(&stMbPoolCfg);
	}

	if (ctx->pInputYUVPath) {
		for (RK_S32 i = 0; i < ctx->stAvsGrpAttr.u32PipeNum; i++) {
			snprintf(name, sizeof(name), "%s/%d.yuv", ctx->pInputYUVPath, i);
			fp[i] = fopen(name, "rb");
			if (!fp[i]) {
				RK_LOGE("fopen %s failed, error: %s", name, strerror(errno));
				ctx->pInputYUVPath = RK_NULL;
			}
		}
	}

	while (!send_frame_thread_quit) {

		for (RK_S32 i = 0; i < ctx->stAvsGrpAttr.u32PipeNum; i++) {

			mbApplyBlk = RK_MPI_MB_GetMB(mbPipePool[i], pstPicCal.u32MBSize, RK_TRUE);
			if (!mbApplyBlk) {
				i--;
				continue;
			}

			stVideoFrames[i].stVFrame.pMbBlk = mbApplyBlk;
			stVideoFrames[i].stVFrame.u32Width = pstBufAttr.u32Width;
			stVideoFrames[i].stVFrame.u32Height = pstBufAttr.u32Height;
			stVideoFrames[i].stVFrame.u32VirWidth = pstPicCal.u32VirWidth;
			stVideoFrames[i].stVFrame.u32VirHeight = pstPicCal.u32VirHeight;
			stVideoFrames[i].stVFrame.enPixelFormat = COMPRESS_MODE_NONE;
			stVideoFrames[i].stVFrame.enCompressMode = RK_FMT_YUV420SP;

			if (ctx->pInputYUVPath) {
				fread(RK_MPI_MB_Handle2VirAddr(stVideoFrames[i].stVFrame.pMbBlk), 1,
				      pstPicCal.u32MBSize, fp[i]);
			} else {
				s32Ret = SAMPLE_COMM_FillImage(
				    (RK_U8 *)RK_MPI_MB_Handle2VirAddr(stVideoFrames[i].stVFrame.pMbBlk),
				    pstBufAttr.u32Width, pstBufAttr.u32Height,
				    RK_MPI_CAL_COMM_GetHorStride(stVideoFrames[i].stVFrame.u32Width,
				                                 stVideoFrames[i].stVFrame.enPixelFormat),
				    stVideoFrames[i].stVFrame.u32VirHeight,
				    stVideoFrames[i].stVFrame.enPixelFormat, loopCount + i);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("TEST_COMM_FillImage failure:%#X", s32Ret);
					quit = true; /* main thread exit */
				}
			}
			RK_MPI_SYS_MmzFlushCache(stVideoFrames[i].stVFrame.pMbBlk, RK_FALSE);
			s32Ret = RK_MPI_AVS_SendPipeFrame(ctx->s32GrpId, i, &stVideoFrames[i], 1000);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_AVS_SendPipeFrame %#X", s32Ret);
				quit = true; /* main thread exit */
			}
			RK_MPI_MB_ReleaseMB(stVideoFrames[i].stVFrame.pMbBlk);
			stVideoFrames[i].stVFrame.pMbBlk = RK_NULL;
		}

		loopCount++;
		usleep(33 * 1000);
	}
	for (RK_S32 i = 0; i < ctx->stAvsGrpAttr.u32PipeNum; i++) {
		if (fp[i]) {
			fclose(fp[i]);
			fp[i] = RK_NULL;
		}
		if (MB_INVALID_POOLID != mbPipePool[i]) {
			RK_MPI_MB_DestroyPool(mbPipePool[i]);
		}
	}
	RK_LOGE("avs_send_stream thread eixt !!!");
	return RK_NULL;
}

/******************************************************************************
 * function : avs get thread
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

	while (!get_frame_thread_quit) {
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

	if (fp) {
		fclose(fp);
		fp = NULL;
	}

	RK_LOGE("avs_get_stream thread exit !!!");
	return RK_NULL;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	SAMPLE_MPI_CTX_S *ctx;
	RK_S32 s32Ret = RK_FAILURE;
	RK_CHAR *pAvsCalibFilePath = "/oem/usr/share/avs_calib/calib_file.xml";
	RK_CHAR *pAvsMeshAlphaPath = "/tmp/";
	RK_CHAR *pAvsLutFilePath = RK_NULL;
	RK_CHAR *pOutPath = RK_NULL;
	RK_CHAR *pInputYUVPath = RK_NULL;
	RK_U32 u32SrcWidth = 1920;
	RK_U32 u32SrcHeight = 1080;
	RK_U32 u32AvsWidth = 3840;
	RK_U32 u32AvsHeight = 1080;
	RK_U32 u32CamNum = 2;
	RK_S32 s32loopCnt = -1;
	RK_FLOAT fStitchDistance = 5.0;
	RK_U16 *pLdchMeshData[CAM_NUM_MAX] = {RK_NULL};
	pthread_t avs_send_thread_id = 0, avs_get_thread_id = 0;

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	ctx = (SAMPLE_MPI_CTX_S *)(malloc(sizeof(SAMPLE_MPI_CTX_S)));
	memset(ctx, 0, sizeof(SAMPLE_MPI_CTX_S));

	signal(SIGINT, sigterm_handler);

	int c;
	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		switch (c) {
		case 'A':
			pAvsCalibFilePath = optarg;
			break;
		case 'n':
			u32CamNum = atoi(optarg);
			break;
		case 'w':
			u32SrcWidth = atoi(optarg);
			break;
		case 'h':
			u32SrcHeight = atoi(optarg);
			break;
		case 'W':
			u32AvsWidth = atoi(optarg);
			break;
		case 'H':
			u32AvsHeight = atoi(optarg);
			break;
		case 'l':
			s32loopCnt = atoi(optarg);
			break;
		case 'i':
			pInputYUVPath = optarg;
			break;
		case 'o':
			pOutPath = optarg;
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#input_yuv_path: %s\n", pInputYUVPath);
	printf("#pAvsLutFilePath: %s\n", pAvsLutFilePath);
	printf("#pAvsCalibFilePath: %s\n", pAvsCalibFilePath);
	printf("#pAvsMeshAlphaPath: %s\n", pAvsMeshAlphaPath);
	printf("#pInputYUVPath: %s\n", pInputYUVPath);
	printf("#Output Path: %s\n", pOutPath);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}

	/* Avs grp create */
	ctx->avs.dstFilePath = pOutPath;
	ctx->avs.s32loopCount = s32loopCnt;
	ctx->avs.pInputYUVPath = pInputYUVPath;
	ctx->avs.s32GrpId = 0;
	ctx->avs.s32ChnId = 0;
	ctx->avs.eGetLdchMode = RK_GET_LDCH_BY_BUFF;
	ctx->avs.u32SrcWidth = u32SrcWidth;
	ctx->avs.u32SrcHeight = u32SrcHeight;
	ctx->avs.fDistance = fStitchDistance;
	ctx->avs.pLdchMeshData = (RK_U16 **)pLdchMeshData;
	ctx->avs.stAvsGrpAttr.enMode = 0; // 0: blend 1: no blend
	ctx->avs.stAvsGrpAttr.u32PipeNum = u32CamNum;
	ctx->avs.stAvsGrpAttr.stGainAttr.enMode = AVS_GAIN_MODE_AUTO;
	ctx->avs.stAvsGrpAttr.stOutAttr.enPrjMode = AVS_PROJECTION_EQUIRECTANGULAR;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Width = u32AvsWidth;
	ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Height = u32AvsHeight;
	ctx->avs.stAvsGrpAttr.bSyncPipe = RK_FALSE;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsGrpAttr.stInAttr.enParamSource = AVS_PARAM_SOURCE_CALIB;
	ctx->avs.stAvsGrpAttr.stInAttr.stCalib.pCalibFilePath = pAvsCalibFilePath;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Width = u32SrcWidth;
	ctx->avs.stAvsGrpAttr.stInAttr.stSize.u32Height = u32SrcHeight;
	ctx->avs.stAvsGrpAttr.stOutAttr.fDistance = fStitchDistance;
	// ctx->avs.stAvsGrpAttr.stInAttr.stCalib.pMeshAlphaPath = pAvsMeshAlphaPath;
	ctx->avs.stAvsChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->avs.stAvsChnAttr[0].u32Depth = 1;
	ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 3;
	ctx->avs.stAvsChnAttr[0].u32Width = u32AvsWidth;
	ctx->avs.stAvsChnAttr[0].u32Height = u32AvsHeight;
	ctx->avs.stAvsChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	s32Ret = SAMPLE_COMM_AVS_CreateGrp(&ctx->avs);
	s32Ret |= SAMPLE_COMM_AVS_StartGrp(&ctx->avs);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("avs create failure");
	}

	pthread_create(&avs_send_thread_id, 0, avs_send_stream, (void *)(&ctx->avs));
	pthread_create(&avs_get_thread_id, 0, avs_get_stream, (void *)(&ctx->avs));

	printf("%s initial finish\n", __func__);

	while (!quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	get_frame_thread_quit = true;
	pthread_join(avs_get_thread_id, NULL);

	send_frame_thread_quit = true;
	pthread_join(avs_send_thread_id, NULL);

	// Destroy AVS[0]
	SAMPLE_COMM_AVS_StopGrp(&ctx->avs);
	SAMPLE_COMM_AVS_DestroyGrp(&ctx->avs);

__FAILED:
	RK_MPI_SYS_Exit();

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
