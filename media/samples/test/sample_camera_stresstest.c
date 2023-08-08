/*
 * Copyright 2021 Rockchip Electronics Co. LTD
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
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

//#define FILE_TEST

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi;
	SAMPLE_VPSS_CTX_S vpss;
	SAMPLE_VENC_CTX_S venc;
} SAMPLE_MPI_CTX_S;

static int g_loopcount = 1;
static int g_framcount = 200;
static COMPRESS_MODE_E g_compressMode = COMPRESS_AFBC_16x16;
static SAMPLE_MPI_CTX_S *g_ctx;

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	g_loopcount = 0;
	quit = true;
}

static RK_CHAR optstr[] = "?::n:l:c:f:";
static const struct option long_options[] = {
    {"index", required_argument, NULL, 'n'},
    {"loop_count", required_argument, NULL, 'l'},
    {"frame_count", required_argument, NULL, 'c'},
    {"pixel_format", optional_argument, NULL, 'f'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("Usage : %s -n <index> -l <loop count> -c <frame count> -f <pixel format>\n",
	       name);
	printf("index:\n");
	printf("\t 0)isp stresstest(IqFileDir: /etc/iqfiles ~ /usr/share/iqfiles)\n");
	printf("\t 1)venc stresstest(H264 ~ H265)\n");
	printf("\t 2)venc stresstest(1x ~ 0.5x resolution)\n");
	printf("\t 3)vi->vps->venc stresstest\n");
	printf("pixel format:\n");
	printf("\t nv12) no compress\n");
	printf("\t afbc) compress\n");
}

/******************************************************************************
 * function : vi thread
 ******************************************************************************/
static void *vi_get_stream(void *pArgs) {
	SAMPLE_VI_CTX_S *ctx = (SAMPLE_VI_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	char name[256] = {0};
	FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/vi_%d.bin", ctx->dstFilePath, ctx->s32DevId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			printf("chn %d can't open %s file !\n", ctx->s32DevId, ctx->dstFilePath);
			quit = true;
			return RK_NULL;
		}
	}

	while (!quit) {
		s32Ret = SAMPLE_COMM_VI_GetChnFrame(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			if (ctx->stViFrame.stVFrame.u64PrivateData <= 0) {
				// SAMPLE_COMM_VI_ReleaseChnFrame(ctx);
				// continue;
			}

			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VI_ReleaseChnFrame(ctx);
					quit = true;
					break;
				}
			}

			if (fp) {
				fwrite(pData, 1, ctx->stViFrame.stVFrame.u64PrivateData, fp);
				fflush(fp);
			}

			RK_LOGE(
			    "SAMPLE_COMM_VI_GetChnFrame DevId %d ok:data %p size:%llu loop:%d seq:%d "
			    "pts:%lld ms\n",
			    ctx->s32DevId, pData, ctx->stViFrame.stVFrame.u64PrivateData, loopCount,
			    ctx->stViFrame.stVFrame.u32TimeRef,
			    ctx->stViFrame.stVFrame.u64PTS / 1000);

			SAMPLE_COMM_VI_ReleaseChnFrame(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	if (fp)
		fclose(fp);

	return RK_NULL;
}

/******************************************************************************
 * function : vpss thread
 ******************************************************************************/
static RK_U8 data[2560 * 1520 * 2];
static RK_U8 data0[2560 * 1520 * 2];
static RK_U8 data1[2560 * 1520 * 2];
static void *vpss_get_stream(void *pArgs) {
	SAMPLE_VPSS_CTX_S *ctx = (SAMPLE_VPSS_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	char name[256] = {0};
	FILE *fp = RK_NULL;
	FILE *fbc0 = RK_NULL;
	FILE *fbc1 = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;
	VIDEO_FRAME_INFO_S stChnFrameInfos;

	if (ctx->dstFilePath) {
		snprintf(name, sizeof(name), "/%s/vpss_%d.bin", ctx->dstFilePath, ctx->s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			printf("chn %d can't open %s file !\n", ctx->s32ChnId, ctx->dstFilePath);
			quit = true;
			return RK_NULL;
		}
	}
#if 0
	fbc0 = fopen("/data/vpss_1280_760_fbc.bin", "r");
	if (fbc0 == RK_NULL) {
		printf("can't open /data/vpss_1280_760_fbc.bin file !\n");
		quit = true;
		return RK_NULL;
	}
	fbc1 = fopen("/data/vpss_2560_1520_fbc.bin", "r");
	if (fbc1 == RK_NULL) {
		printf("can't open /data/vpss_2560_1520_fbc.bin file !\n");
		quit = true;
		return RK_NULL;
	}

	fread(data0, 1, 1552384, fbc0);
	fread(data1, 1, 6164480, fbc1);
#endif
	while (!quit) {
		s32Ret = SAMPLE_COMM_VPSS_GetChnFrame(ctx, &pData);
		if (s32Ret == RK_SUCCESS) {
			if (ctx->stChnFrameInfos.stVFrame.u64PrivateData <= 0) {
				continue;
			}
#if 0
			// exit when complete
			if (ctx->s32loopCount > 0) {
				if (loopCount >= ctx->s32loopCount) {
					SAMPLE_COMM_VPSS_ReleaseChnFrame(ctx);
					quit = true;
					break;
				}
			}
#endif
			if (fp) {
				fwrite(pData, 1, ctx->stChnFrameInfos.stVFrame.u64PrivateData, fp);
				fflush(fp);
			}
#if 1
			/*SAMPLE_COMM_VENC_SendStream(
			    &g_ctx->venc, pData, ctx->stChnFrameInfos.stVFrame.u32Width,
			    ctx->stChnFrameInfos.stVFrame.u32Height,
			    ctx->stChnFrameInfos.stVFrame.u64PrivateData, g_compressMode);*/

			s32Ret = RK_MPI_VENC_SendFrame(g_ctx->venc.s32ChnId,
			                               &ctx->stChnFrameInfos.stVFrame, -1);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VENC_SendFrame fail %x", s32Ret);
			}
#else
			if (ctx->stChnFrameInfos.stVFrame.u32Width == 1280) {
				SAMPLE_COMM_VENC_SendStream(
				    &g_ctx->venc, data0, ctx->stChnFrameInfos.stVFrame.u32Width,
				    ctx->stChnFrameInfos.stVFrame.u32Height, 1552384, g_compressMode);
			} else if (ctx->stChnFrameInfos.stVFrame.u32Width == 2560) {
				SAMPLE_COMM_VENC_SendStream(
				    &g_ctx->venc, data1, ctx->stChnFrameInfos.stVFrame.u32Width,
				    ctx->stChnFrameInfos.stVFrame.u32Height, 6164480, g_compressMode);
			}
#endif
			RK_LOGD("SAMPLE_COMM_VPSS_GetChnFrame DevId %d ok:data %p size:%llu loop:%d "
			        "seq:%d "
			        "pts:%lld ms\n",
			        ctx->s32GrpId, pData, ctx->stChnFrameInfos.stVFrame.u64PrivateData,
			        loopCount, ctx->stChnFrameInfos.stVFrame.u32TimeRef,
			        ctx->stChnFrameInfos.stVFrame.u64PTS / 1000);

			SAMPLE_COMM_VPSS_ReleaseChnFrame(ctx);
			loopCount++;
		}
		usleep(1000);
	}

	if (fp)
		fclose(fp);

	return RK_NULL;
}

static void *vpss_send_stream(void *pArgs) {
	SAMPLE_VPSS_CTX_S *ctx = (SAMPLE_VPSS_CTX_S *)(pArgs);
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fbc = RK_NULL;
	void *pData = RK_NULL;
	RK_S32 loopCount = 0;

	fbc = fopen("/data/vpss_2560_1520_fbc.bin", "r");
	if (fbc == RK_NULL) {
		printf("can't open /data/vpss_2560_1520_fbc.bin file !\n");
		quit = true;
		return RK_NULL;
	}

	fread(data, 1, 6164480, fbc);

	while (!quit) {
		// exit when complete
		if (ctx->s32loopCount > 0) {
			if (loopCount >= ctx->s32loopCount) {
				quit = true;
				break;
			}
		}

		SAMPLE_COMM_VPSS_SendStream(&g_ctx->vpss, data, 2560, 1520, 6164480,
		                            g_compressMode);

		loopCount++;

		usleep(20000);
	}

	if (fbc)
		fclose(fbc);

	return RK_NULL;
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

				if (fp) {
					fwrite(pData, 1, ctx->stFrame.pstPack->u32Len, fp);
					fflush(fp);
				}
			}

			PrintStreamDetails(ctx->s32ChnId, ctx->stFrame.pstPack->u32Len);

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
 * function    : SAMPLE_COMM_ISP_Stresstest
 ******************************************************************************/
int SAMPLE_CAMERA_ISP_Stresstest(SAMPLE_MPI_CTX_S *ctx, char *pIqFileDir) {
	RK_S32 s32Ret = RK_FAILURE;
	int video_width = 2560;
	int video_height = 1520;
	RK_CHAR *pDeviceName = NULL;
	RK_CHAR *pOutPath = NULL;
	char *iq_file_dir = pIqFileDir;
	RK_S32 s32CamId = 0;
	RK_S32 s32loopCnt = g_framcount;
	MPP_CHN_S stSrcChn, stDestChn;
	FILE *fp = RK_NULL;
	void *pData = RK_NULL;
	RK_BOOL bMultictx = RK_FALSE;
	pthread_t vi_thread_id;
	quit = false;

	printf("#pDeviceName: %s\n", pDeviceName);
	printf("#Output Path: %s\n", pOutPath);
	printf("#IQ Path: %s\n", iq_file_dir);
	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);
		rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;

		SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, iq_file_dir);
		SAMPLE_COMM_ISP_Run(s32CamId);
#endif
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		printf("RK_MPI_SYS_Init failed!\n");
		goto __FAILED;
	}

	// Init VI
	ctx->vi.u32Width = video_width;
	ctx->vi.u32Height = video_height;
	ctx->vi.s32DevId = s32CamId;
	ctx->vi.u32PipeId = ctx->vi.s32DevId;
	ctx->vi.s32ChnId = 2;
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = 3;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi.stChnAttr.u32Depth = 2;
	ctx->vi.stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi.stChnAttr.enCompressMode = g_compressMode;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = -1;
	ctx->vi.dstFilePath = pOutPath;
	ctx->vi.s32loopCount = s32loopCnt;
	if (g_compressMode == COMPRESS_MODE_NONE) {
		ctx->vi.s32ChnId = 0;
	}
	SAMPLE_COMM_VI_CreateChn(&ctx->vi);

	pthread_create(&vi_thread_id, 0, vi_get_stream, (void *)(&ctx->vi));

	printf("%s initial finish\n", __func__);

	while (!quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	pthread_join(vi_thread_id, NULL);

	// Destroy VI[0]
	SAMPLE_COMM_VI_DestroyChn(&ctx->vi);

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		SAMPLE_COMM_ISP_Stop(s32CamId);
#endif
	}

	return 0;
}

/******************************************************************************
 * function    : SAMPLE_COMM_VENC_Stresstest
 ******************************************************************************/
int SAMPLE_CAMERA_VENC_SetConfig(SAMPLE_MPI_CTX_S *ctx, MPP_CHN_S *pSrc,
                                 MPP_CHN_S *pDest) {
	RK_S32 s32Ret = RK_FAILURE;
	quit = false;

	printf("%s entering!\n", __func__);

	// UnBind VPSS[0] and VENC[0]
	SAMPLE_COMM_UnBind(pSrc, pDest);

	// Destroy VENC[0]
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);

	// Init VENC[0]
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc);

	// Bind VPSS[0] and VENC[0]
	SAMPLE_COMM_Bind(pSrc, pDest);

	while (!quit) {
		sleep(1);
	}

	if (ctx->venc.getStreamCbFunc) {
		pthread_join(ctx->venc.getStreamThread, NULL);
	}

	printf("%s exit!\n", __func__);

	return 0;
}

int SAMPLE_CAMERA_VENC_Stresstest(SAMPLE_MPI_CTX_S *ctx, RK_S32 mode) {
	RK_S32 s32Ret = RK_FAILURE;
	int video_width = 2560;
	int video_height = 1520;
	int venc_width = 2560;
	int venc_height = 1520;
	RK_CHAR *pOutPathVenc = NULL;
	RK_CHAR *iq_file_dir = "/etc/iqfiles";
	RK_CHAR *pCodecName = "H264";
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H264CBR;
	RK_S32 s32BitRate = 4 * 1024;
	RK_S32 s32CamId = 0;
	MPP_CHN_S stSrcChn, stDestChn;
	RK_S32 s32loopCnt = g_framcount;
	RK_S32 i;
	RK_BOOL bMultictx = RK_FALSE;
	quit = false;
	pthread_t vpss_thread_id[2];

	if (mode == 1) {
		s32loopCnt = -1;
	}

	printf("#CameraIdx: %d\n", s32CamId);
	printf("#CodecName:%s\n", pCodecName);
	printf("#Output Path: %s\n", pOutPathVenc);

	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);
		rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;

		SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, iq_file_dir);
		SAMPLE_COMM_ISP_Run(s32CamId);
#endif
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}

	MB_POOL pool = MB_INVALID_POOLID;
	MB_POOL_CONFIG_S stMbPoolCfg;
	memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
	stMbPoolCfg.u64MBSize = video_width * video_height * 2;
	stMbPoolCfg.u32MBCnt = 10;
	stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
	pool = RK_MPI_MB_CreatePool(&stMbPoolCfg);
	ctx->vpss.pool = ctx->venc.pool = pool;

	// Init VI[0]
	ctx->vi.u32Width = video_width;
	ctx->vi.u32Height = video_height;
	ctx->vi.s32DevId = s32CamId;
	ctx->vi.u32PipeId = ctx->vi.s32DevId;
	ctx->vi.s32ChnId = 2; // rk3588 mainpath:0 selfpath:1 fbcpath:2
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = 4;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi.stChnAttr.u32Depth = 1;
	ctx->vi.stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi.stChnAttr.enCompressMode = g_compressMode;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = -1;
	if (g_compressMode == COMPRESS_MODE_NONE) {
		ctx->vi.s32ChnId = 0;
	}
	SAMPLE_COMM_VI_CreateChn(&ctx->vi);

	// Init VPSS[0]
	ctx->vpss.s32GrpId = 0;
	ctx->vpss.s32ChnId = 0;
	// RGA_device: VIDEO_PROC_DEV_RGA GPU_device: VIDEO_PROC_DEV_GPU
	ctx->vpss.enVProcDevType = VIDEO_PROC_DEV_RGA;
	// ctx->vpss.enVProcDevType = VIDEO_PROC_DEV_GPU;
	ctx->vpss.s32loopCount = s32loopCnt;
	ctx->vpss.stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss.stGrpVpssAttr.enCompressMode = g_compressMode;

	ctx->vpss.stCropInfo.bEnable = RK_FALSE;
	ctx->vpss.stCropInfo.enCropCoordinate = VPSS_CROP_RATIO_COOR;
	ctx->vpss.stCropInfo.stCropRect.s32X = 0;
	ctx->vpss.stCropInfo.stCropRect.s32Y = 0;
	ctx->vpss.stCropInfo.stCropRect.u32Width = video_width;
	ctx->vpss.stCropInfo.stCropRect.u32Height = video_height;

	ctx->vpss.stChnCropInfo[0].bEnable = RK_TRUE;
	ctx->vpss.stChnCropInfo[0].enCropCoordinate = VPSS_CROP_RATIO_COOR;
	ctx->vpss.stChnCropInfo[0].stCropRect.s32X = 0;
	ctx->vpss.stChnCropInfo[0].stCropRect.s32Y = 0;
	ctx->vpss.stChnCropInfo[0].stCropRect.u32Width = venc_width * 1000 / video_width;
	ctx->vpss.stChnCropInfo[0].stCropRect.u32Height = venc_height * 1000 / video_height;
	ctx->vpss.s32ChnRotation[0] = ROTATION_0;
	ctx->vpss.stRotationEx[0].bEnable = RK_FALSE;
	ctx->vpss.stRotationEx[0].stRotationEx.u32Angle = 60;
	ctx->vpss.stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
	ctx->vpss.stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss.stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss.stVpssChnAttr[0].enCompressMode = g_compressMode;
	ctx->vpss.stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss.stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss.stVpssChnAttr[0].u32Width = venc_width;
	ctx->vpss.stVpssChnAttr[0].u32Height = venc_height;
	ctx->vpss.stVpssChnAttr[0].u32Depth = 2;
	SAMPLE_COMM_VPSS_CreateChn(&ctx->vpss);
#if defined(FILE_TEST)
	pthread_create(&vpss_thread_id[0], 0, vpss_get_stream, (void *)(&ctx->vpss));
// pthread_create(&vpss_thread_id[1], 0, vpss_send_stream, (void *)(&ctx->vpss));
#endif

	// Init VENC[0]
	ctx->venc.s32ChnId = 0;
	ctx->venc.u32Width = venc_width;
	ctx->venc.u32Height = venc_height;
	ctx->venc.u32Fps = 30;
	ctx->venc.u32Gop = 50;
	ctx->venc.u32BitRate = s32BitRate;
	ctx->venc.enCodecType = enCodecType;
	ctx->venc.enRcMode = enRcMode;
	ctx->venc.getStreamCbFunc = venc_get_stream;
	ctx->venc.s32loopCount = s32loopCnt;
	ctx->venc.dstFilePath = pOutPathVenc;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	ctx->venc.stChnAttr.stVencAttr.u32Profile = 100;
	ctx->venc.stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc);

	// Bind VI[0] and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = ctx->vpss.s32GrpId;
	stDestChn.s32ChnId = ctx->vpss.s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

#if !defined(FILE_TEST)
	// Bind VPSS[0] and VENC[0]
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = ctx->vpss.s32GrpId;
	stSrcChn.s32ChnId = ctx->vpss.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);
#endif

	printf("%s initial finish\n", __func__);

	if (mode == 0) {
		while (!quit) {
			sleep(1);
		}

		if (ctx->venc.getStreamCbFunc) {
			pthread_join(ctx->venc.getStreamThread, NULL);
		}

		if (g_loopcount > 0) {
			g_loopcount--;
			printf("sample_camera_stresstest: g_loopcount(%d)\n", g_loopcount);
		}
	}

	while (g_loopcount) {
		if (g_loopcount % 2 == 0) {
			if (mode == 0) {
				printf("Switch to h265cbr coding!\n");
				ctx->venc.enCodecType = RK_CODEC_TYPE_H265;
				ctx->venc.enRcMode = VENC_RC_MODE_H265CBR;
				ctx->venc.stChnAttr.stVencAttr.u32Profile = 0;
				s32Ret = SAMPLE_CAMERA_VENC_SetConfig(ctx, &stSrcChn, &stDestChn);
			} else if (mode == 1) {
				printf("Switch to %dx%d coding!\n", venc_width / 2, venc_height / 2);
				ctx->vpss.s32GrpId = 0;
				ctx->vpss.s32ChnId = 0;
				ctx->vpss.stVpssChnAttr[0].u32Width = venc_width / 2;
				ctx->vpss.stVpssChnAttr[0].u32Height = venc_height / 2;
				SAMPLE_COMM_VPSS_SetChnAttr(&ctx->vpss);
			}
		} else {
			if (mode == 0) {
				printf("Switch to h264cbr coding!\n");
				ctx->venc.enCodecType = RK_CODEC_TYPE_H264;
				ctx->venc.enRcMode = VENC_RC_MODE_H264CBR;
				ctx->venc.stChnAttr.stVencAttr.u32Profile = 100;
				s32Ret = SAMPLE_CAMERA_VENC_SetConfig(ctx, &stSrcChn, &stDestChn);
			} else if (mode == 1) {
				printf("Switch to %dx%d coding!\n", venc_width, venc_height);
				ctx->vpss.s32GrpId = 0;
				ctx->vpss.s32ChnId = 0;
				ctx->vpss.stVpssChnAttr[0].u32Width = venc_width;
				ctx->vpss.stVpssChnAttr[0].u32Height = venc_height;
				SAMPLE_COMM_VPSS_SetChnAttr(&ctx->vpss);
			}
		}

		if (g_loopcount > 0) {
			g_loopcount--;
			printf("sample_camera_stresstest: g_loopcount(%d)\n", g_loopcount);
		} else {
			quit = true;
			break;
		}

		sleep(2);
	}

	printf("%s exit!\n", __func__);

	if (mode == 1) {
		quit = true;
		if (ctx->venc.getStreamCbFunc) {
			pthread_join(ctx->venc.getStreamThread, NULL);
		}
		pthread_join(vpss_thread_id[0], NULL);
		pthread_join(vpss_thread_id[1], NULL);
	}

	RK_MPI_MB_DestroyPool(pool);

#if !defined(FILE_TEST)
	// UnBind VPSS[0] and VENC[0]
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = ctx->vpss.s32GrpId;
	stSrcChn.s32ChnId = ctx->vpss.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	printf("%s Unbind VPSS[0] - VENC[0]!\n", __func__);

	// Bind VI[0] and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = ctx->vpss.s32GrpId;
	stDestChn.s32ChnId = ctx->vpss.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	printf("%s Unbind VI[0] - VPSS[0]!\n", __func__);
#endif
	// Destroy VENC[0]
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);
	// Destroy VPSS[0]
	SAMPLE_COMM_VPSS_DestroyChn(&ctx->vpss);
	// Destroy VI[0]
	SAMPLE_COMM_VI_DestroyChn(&ctx->vi);

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		SAMPLE_COMM_ISP_Stop(s32CamId);
#endif
	}

	return 0;
}

/******************************************************************************
 * function    : SAMPLE_COMM_VI_AVS_VENC_Stresstest
 ******************************************************************************/
int SAMPLE_CAMERA_VI_VPSS_VENC_Stresstest(SAMPLE_MPI_CTX_S *ctx) {
	RK_S32 s32Ret = RK_FAILURE;
	int video_width = 2560;
	int video_height = 1520;
	int venc_width = 2560;
	int venc_height = 1520;
	RK_CHAR *pOutPathVenc = NULL;
	RK_CHAR *iq_file_dir = "/etc/iqfiles";
	RK_CHAR *pCodecName = "H264";
	CODEC_TYPE_E enCodecType = RK_CODEC_TYPE_H264;
	VENC_RC_MODE_E enRcMode = VENC_RC_MODE_H264CBR;
	RK_S32 s32BitRate = 4 * 1024;
	RK_S32 s32CamId = 0;
	MPP_CHN_S stSrcChn, stDestChn;
	RK_S32 s32loopCnt = g_framcount;
	RK_S32 i;
	RK_BOOL bMultictx = RK_FALSE;
	quit = false;

	printf("#CameraIdx: %d\n", s32CamId);
	printf("#CodecName:%s\n", pCodecName);
	printf("#Output Path: %s\n", pOutPathVenc);

	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);
		rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;

		SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, iq_file_dir);
		SAMPLE_COMM_ISP_Run(s32CamId);
#endif
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}

	// Init VI[0]
	ctx->vi.u32Width = video_width;
	ctx->vi.u32Height = video_height;
	ctx->vi.s32DevId = s32CamId;
	ctx->vi.u32PipeId = ctx->vi.s32DevId;
	ctx->vi.s32ChnId = 2; // rk3588 mainpath:0 selfpath:1 fbcpath:2
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = 4;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi.stChnAttr.u32Depth = 1;
	ctx->vi.stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi.stChnAttr.enCompressMode = g_compressMode;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = -1;
	if (g_compressMode == COMPRESS_MODE_NONE) {
		ctx->vi.s32ChnId = 0;
	}
	SAMPLE_COMM_VI_CreateChn(&ctx->vi);

	// Init VPSS[0]
	ctx->vpss.s32GrpId = 0;
	ctx->vpss.s32ChnId = 0;
	// RGA_device: VIDEO_PROC_DEV_RGA GPU_device: VIDEO_PROC_DEV_GPU
	ctx->vpss.enVProcDevType = VIDEO_PROC_DEV_RGA;
	// ctx->vpss.enVProcDevType = VIDEO_PROC_DEV_GPU;
	ctx->vpss.s32loopCount = s32loopCnt;
	ctx->vpss.stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss.stGrpVpssAttr.enCompressMode = g_compressMode;

	ctx->vpss.stCropInfo.bEnable = RK_FALSE;
	ctx->vpss.stCropInfo.enCropCoordinate = VPSS_CROP_RATIO_COOR;
	ctx->vpss.stCropInfo.stCropRect.s32X = 0;
	ctx->vpss.stCropInfo.stCropRect.s32Y = 0;
	ctx->vpss.stCropInfo.stCropRect.u32Width = video_width;
	ctx->vpss.stCropInfo.stCropRect.u32Height = video_height;

	ctx->vpss.stChnCropInfo[0].bEnable = RK_TRUE;
	ctx->vpss.stChnCropInfo[0].enCropCoordinate = VPSS_CROP_RATIO_COOR;
	ctx->vpss.stChnCropInfo[0].stCropRect.s32X = 0;
	ctx->vpss.stChnCropInfo[0].stCropRect.s32Y = 0;
	ctx->vpss.stChnCropInfo[0].stCropRect.u32Width = venc_width * 1000 / video_width;
	ctx->vpss.stChnCropInfo[0].stCropRect.u32Height = venc_height * 1000 / video_height;
	ctx->vpss.s32ChnRotation[0] = ROTATION_0;
	ctx->vpss.stRotationEx[0].bEnable = RK_FALSE;
	ctx->vpss.stRotationEx[0].stRotationEx.u32Angle = 60;
	ctx->vpss.stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
	ctx->vpss.stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss.stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss.stVpssChnAttr[0].enCompressMode = g_compressMode;
	ctx->vpss.stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss.stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss.stVpssChnAttr[0].u32Width = venc_width;
	ctx->vpss.stVpssChnAttr[0].u32Height = venc_height;
	// ctx->vpss.stVpssChnAttr[0].u32Depth = 2;
	SAMPLE_COMM_VPSS_CreateChn(&ctx->vpss);

	// Init VENC[0]
	ctx->venc.s32ChnId = 0;
	ctx->venc.u32Width = venc_width;
	ctx->venc.u32Height = venc_height;
	ctx->venc.u32Fps = 30;
	ctx->venc.u32Gop = 50;
	ctx->venc.u32BitRate = s32BitRate;
	ctx->venc.enCodecType = enCodecType;
	ctx->venc.enRcMode = enRcMode;
	ctx->venc.getStreamCbFunc = venc_get_stream;
	ctx->venc.s32loopCount = s32loopCnt;
	ctx->venc.dstFilePath = pOutPathVenc;
	// H264  66：Baseline  77：Main Profile 100：High Profile
	// H265  0：Main Profile  1：Main 10 Profile
	// MJPEG 0：Baseline
	ctx->venc.stChnAttr.stVencAttr.u32Profile = 100;
	ctx->venc.stChnAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP; // VENC_GOPMODE_SMARTP
	SAMPLE_COMM_VENC_CreateChn(&ctx->venc);

	// Bind VI[0] and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = ctx->vpss.s32GrpId;
	stDestChn.s32ChnId = ctx->vpss.s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	// Bind VPSS[0] and VENC[0]
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = ctx->vpss.s32GrpId;
	stSrcChn.s32ChnId = ctx->vpss.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	SAMPLE_COMM_Bind(&stSrcChn, &stDestChn);

	printf("%s initial finish\n", __func__);

	while (!quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	if (ctx->venc.getStreamCbFunc) {
		pthread_join(ctx->venc.getStreamThread, NULL);
	}

	// UnBind VPSS[0] and VENC[0]
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = ctx->vpss.s32GrpId;
	stSrcChn.s32ChnId = ctx->vpss.s32ChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = ctx->venc.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	printf("%s Unbind VPSS[0] - VENC[0]!\n", __func__);

	// Bind VI[0] and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = ctx->vpss.s32GrpId;
	stDestChn.s32ChnId = ctx->vpss.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);
	printf("%s Unbind VI[0] - VPSS[0]!\n", __func__);

	// Destroy VENC[0]
	SAMPLE_COMM_VENC_DestroyChn(&ctx->venc);
	// Destroy VPSS[0]
	SAMPLE_COMM_VPSS_DestroyChn(&ctx->vpss);
	// Destroy VI[0]
	SAMPLE_COMM_VI_DestroyChn(&ctx->vi);

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		SAMPLE_COMM_ISP_Stop(s32CamId);
#endif
	}

	return 0;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32Index;
	struct sigaction sa;
	RK_CHAR *iq_file_dir = "/etc/iqfiles";
	RK_BOOL bMultictx = RK_FALSE;

	RK_S32 s32CamId = 0;
	SAMPLE_MPI_CTX_S ctx;

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
	}

	signal(SIGINT, sigterm_handler);

	int c;
	while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
		const char *tmp_optarg = optarg;
		switch (c) {
		case 'n':
			s32Index = atoi(optarg);
			break;
		case 'l':
			g_loopcount = atoi(optarg);
			break;
		case 'c':
			g_framcount = atoi(optarg);
			break;
		case 'f':
			if (!strcmp(optarg, "nv12")) {
				g_compressMode = COMPRESS_MODE_NONE;
			} else if (!strcmp(optarg, "afbc")) {
				g_compressMode = COMPRESS_AFBC_16x16;
			}
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	memset(&ctx, 0, sizeof(SAMPLE_MPI_CTX_S));
	g_ctx = &ctx;

	while (g_loopcount) {
		switch (s32Index) {
		case 0:
			if (g_loopcount % 2 == 0) {
				s32Ret = SAMPLE_CAMERA_ISP_Stresstest(&ctx, "/etc/iqfiles");
			} else {
				s32Ret = SAMPLE_CAMERA_ISP_Stresstest(&ctx, "/oem/usr/share/iqfiles");
			}
			break;
		case 1:
			s32Ret = SAMPLE_CAMERA_VENC_Stresstest(&ctx, 0);
			g_loopcount = 0;
			break;
		case 2:
			s32Ret = SAMPLE_CAMERA_VENC_Stresstest(&ctx, 1);
			g_loopcount = 0;
			break;
		case 3:
			s32Ret = SAMPLE_CAMERA_VI_VPSS_VENC_Stresstest(&ctx);
			break;
		default:
			printf("the index %d is invaild!\n", s32Index);
			print_usage(argv[0]);
			return RK_FAILURE;
		}

		if (g_loopcount > 0) {
			g_loopcount--;
			printf("sample_camera_stresstest: g_loopcount(%d)\n", g_loopcount);
		} else {
			break;
		}

		sleep(2);
	}

	printf("%s finish!\n", __func__);

	return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
