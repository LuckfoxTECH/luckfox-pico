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

#include "sample_comm.h"

typedef struct _rkMpiCtx {
	SAMPLE_VI_CTX_S vi;
	SAMPLE_VPSS_CTX_S vpss;
	SAMPLE_VO_CTX_S vo;
} SAMPLE_MPI_CTX_S;

static bool quit = false;
static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

static RK_CHAR optstr[] = "?::a::w:h:d:D:I:L:M:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"device_name", required_argument, NULL, 'd'},
    {"disp_devid", required_argument, NULL, 'D'},
    {"camid", required_argument, NULL, 'I'},
    {"disp_layerid", required_argument, NULL, 'L'},
    {"hdr_mode", required_argument, NULL, 'h' + 'm'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("Support platform: rv1109/rv1126, rk3588/rk356x\n");
	printf("usage example:\n");
	printf("\t%s -w 1920 -h 1080 -a /etc/iqfiles/ -I 0 -D 0 -L 0\n", name);
#ifdef RKAIQ
	printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a "
	       "/etc/iqfiles/, "
	       "set dirpath empty to using path by default, without this option aiq "
	       "should run in other application\n");
	printf("\t-M | --multictx: switch of multictx in isp, set 0 to disable, set "
	       "1 to enable. Default: 0\n");
#endif
	printf("\t-w | --width: camera with, Default 1920\n");
	printf("\t-h | --height: camera height, Default 1080\n");
	printf("\t-d | --device_name: encode save file path, Default NULL\n");
	printf("\t-D | --disp_devid: display DevId, Default 0\n");
	printf("\t-I | --camid: camera ctx id, Default 0\n");
	printf("\t-L | --disp_layerid: display LayerId, Default 0\n");
	printf("\t--hdr_mode: set HDR mode, 0: normal, 1: HDR2. default: 0\n");
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	SAMPLE_MPI_CTX_S *ctx;
	RK_U32 video_width = 1920;
	RK_U32 video_height = 1080;
	RK_U32 disp_width = 720;
	RK_U32 disp_height = 1280;
	RK_CHAR *pDeviceName = NULL;
	RK_S32 s32CamId = 0;
	RK_S32 s32DisId = 0;
	RK_S32 s32DisLayerId = 0;
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	MPP_CHN_S stSrcChn, stDestChn;

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
		case 'd':
			pDeviceName = optarg;
			break;
		case 'D':
			s32DisId = atoi(optarg);
			if (s32DisId == 3) { // MIPI
				disp_width = 1920;
				disp_height = 1080;
			} else {
				disp_width = 720;
				disp_height = 1280;
			}
			break;
		case 'w':
			video_width = atoi(optarg);
			break;
		case 'h':
			video_height = atoi(optarg);
			break;
		case 'I':
			s32CamId = atoi(optarg);
			break;
		case 'L':
			s32DisLayerId = atoi(optarg);
			break;
#ifdef RKAIQ
		case 'M':
			if (atoi(optarg)) {
				bMultictx = RK_TRUE;
			}
			break;
#endif
		case 'h' + 'm':
			if (0 == atoi(optarg)) {
				hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
			} else if (1 == atoi(optarg)) {
				hdr_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
			}
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#CameraIdx: %d\n", s32CamId);
	printf("#pDeviceName: %s\n", pDeviceName);
	printf("#IQ Path: %s\n", iq_file_dir);
	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);

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
	ctx->vi.s32ChnId = 1;
	ctx->vi.stChnAttr.stIspOpt.u32BufCount = 3;
	ctx->vi.stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	ctx->vi.stChnAttr.u32Depth = 2;
	ctx->vi.stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vi.stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	ctx->vi.stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	ctx->vi.stChnAttr.stFrameRate.s32DstFrameRate = -1;
	if (pDeviceName) {
		strcpy(ctx->vi.stChnAttr.stIspOpt.aEntityName, pDeviceName);
	}
	SAMPLE_COMM_VI_CreateChn(&ctx->vi);

	// Init VPSS[0]
	ctx->vpss.s32GrpId = 0;
	ctx->vpss.s32ChnId = 0;
	// RGA_device: VIDEO_PROC_DEV_RGA GPU_device: VIDEO_PROC_DEV_GPU
	ctx->vpss.enVProcDevType = VIDEO_PROC_DEV_RGA;
	ctx->vpss.stGrpVpssAttr.enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss.stGrpVpssAttr.enCompressMode = COMPRESS_MODE_NONE; // no compress
	ctx->vpss.s32ChnRotation[0] = ROTATION_0;
	ctx->vpss.stVpssChnAttr[0].enChnMode = VPSS_CHN_MODE_USER;
	ctx->vpss.stVpssChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
	ctx->vpss.stVpssChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
	ctx->vpss.stVpssChnAttr[0].enPixelFormat = RK_FMT_YUV420SP;
	ctx->vpss.stVpssChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
	ctx->vpss.stVpssChnAttr[0].stFrameRate.s32DstFrameRate = -1;
	ctx->vpss.stVpssChnAttr[0].u32Width = disp_width;
	ctx->vpss.stVpssChnAttr[0].u32Height = disp_height;
	if (s32DisId == 0) { // MIPI
		ctx->vpss.s32ChnRotation[0] = ROTATION_90;
		ctx->vpss.stVpssChnAttr[0].u32Width = disp_height;
		ctx->vpss.stVpssChnAttr[0].u32Height = disp_width;
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
	if (s32DisId == 3) { // HDMI
		ctx->vo.stVoPubAttr.enIntfType = VO_INTF_HDMI;
		ctx->vo.stVoPubAttr.enIntfSync = VO_OUTPUT_1080P60;
	} else {
		ctx->vo.stVoPubAttr.enIntfType = VO_INTF_MIPI;
		ctx->vo.stVoPubAttr.enIntfSync = VO_OUTPUT_DEFAULT;
	}
	SAMPLE_COMM_VO_CreateChn(&ctx->vo);

	// Bind VI[0] and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
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

	printf("%s initial finish\n", __func__);

	while (!quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	// UnBind VPSS[0] and VO[0]
	stSrcChn.enModId = RK_ID_VPSS;
	stSrcChn.s32DevId = ctx->vpss.s32GrpId;
	stSrcChn.s32ChnId = ctx->vpss.s32ChnId;
	stDestChn.enModId = RK_ID_VO;
	stDestChn.s32DevId = ctx->vo.s32LayerId;
	stDestChn.s32ChnId = ctx->vo.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	// UnBind VI[0] and VPSS[0]
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = ctx->vi.s32DevId;
	stSrcChn.s32ChnId = ctx->vi.s32ChnId;
	stDestChn.enModId = RK_ID_VPSS;
	stDestChn.s32DevId = ctx->vpss.s32GrpId;
	stDestChn.s32ChnId = ctx->vpss.s32ChnId;
	SAMPLE_COMM_UnBind(&stSrcChn, &stDestChn);

	// Destroy VO[0]
	SAMPLE_COMM_VO_DestroyChn(&ctx->vo);
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
