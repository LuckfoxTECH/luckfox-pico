/*
 * Copyright 2022 Rockchip Electronics Co. LTD
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
#include <error.h>
#include <fcntl.h>
#include <getopt.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdbool.h>

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

#define MAX_AIQ_CTX 8
static rk_aiq_sys_ctx_t *g_aiq_ctx[MAX_AIQ_CTX];
rk_aiq_working_mode_t g_WDRMode[MAX_AIQ_CTX];
#include <stdatomic.h>
static atomic_int g_sof_cnt = 0;
static atomic_bool g_should_quit = false;

#define GET_STREAM_TIMEOUT 2000
#define SEND_STREAM_TIMEOUT 2000
#define BUFFER_SIZE 255
#define VENC_CHN_MAX 2
#define VENC_COMBO_CHN 1
#define RK_ALIGN(x, a) (((x) + (a)-1) & ~((a)-1))
#define RK_ALIGN_2(x) RK_ALIGN(x, 2)

static RK_BOOL g_quit = RK_FALSE;
static RK_S32 g_exit_result = 0;
static RK_BOOL g_bIfVencThreadQuit[VENC_CHN_MAX] = {RK_FALSE};
static RK_U32 g_u32MainVencWidth = 1920;
static RK_U32 g_u32MainVencHeight = 1080;
static RK_CHAR *g_pOutPath = RK_NULL;
static RK_S32 g_s32loopCnt = -1;
static RK_BOOL g_wrap = RK_FALSE;
static RK_S32 g_s32JpegCaptureNum = 1;

static void program_handle_error(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> error exit!", func, line);
	g_exit_result = -1;
	g_quit = RK_TRUE;
}

static void program_normal_exit(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> normal exit!", func, line);
	g_quit = RK_TRUE;
}

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	program_normal_exit(__func__, __LINE__);
}

static RK_CHAR optstr[] = "?::a::w:h:l:o:r:j:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"loop_count", required_argument, NULL, 'l'},
    {"output_path", required_argument, NULL, 'o'},
    {"wrap", required_argument, NULL, 'r'},
    {"jpeg_capture_num", required_argument, NULL, 'j'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\tNOTE: Only RV1106 and RV1103 support this demo.\n");
	printf("\t%s -w 1920 -h 1080 -a /etc/iqfiles/ -l -1 -o /userdata/\n", name);
#ifdef RKAIQ
	printf("\t-a | --aiq: enable aiq with dirpath provided, eg:-a /etc/iqfiles/, "
	       "set dirpath empty to using path by default, \n"
	       "\t		without this option aiq "
	       "should run in other application\n");
#endif
	printf("\t-w | --width: Vi width, Default 1920\n");
	printf("\t-h | --height: Vi height, Default 1080\n");
	printf("\t-l | --loop_count: loop count, Default -1\n");
	printf("\t-o | --output_path: Venc output file path, Default NULL\n");
	printf("\t-r | --wrap: wrap mode, 0:close 1:open. Default: 0\n");
	printf("\t-j | --jpeg_capture_num: set jpeg capture photos one time. Default: 1\n");
}

static void *venc_get_stream(void *pArgs) {

	RK_S32 s32ChnId = *(RK_S32 *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fp = RK_NULL;
	RK_S32 s32fd = 0;
	RK_S32 loopCount = 0;
	RK_VOID *pData = RK_NULL;
	RK_CHAR name[BUFFER_SIZE] = {0};
	VENC_STREAM_S stFrame;

	if (g_pOutPath) {
		snprintf(name, sizeof(name), "/%s/venc_%d.bin", g_pOutPath, s32ChnId);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			RK_LOGE("chn %d can't open %s file !\n", s32ChnId, g_pOutPath);
			program_handle_error(__func__, __LINE__);
			return RK_NULL;
		}
		s32fd = fileno(fp);
	}
	memset(&stFrame, 0, sizeof(VENC_STREAM_S));
	stFrame.pstPack = (VENC_PACK_S *)(malloc(sizeof(VENC_PACK_S)));
	if (!stFrame.pstPack) {
		RK_LOGE("malloc for stFrame.pstPack failure, chnid:%d", s32ChnId);
		program_handle_error(__func__, __LINE__);
		return RK_NULL;
	}
	while (!g_bIfVencThreadQuit[s32ChnId]) {
		s32Ret = RK_MPI_VENC_GetStream(s32ChnId, &stFrame, -1);
		if (s32Ret == RK_SUCCESS) {
			pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);

			if (g_s32loopCnt > 0 && loopCount >= g_s32loopCnt) {
				RK_MPI_VENC_ReleaseStream(s32ChnId, &stFrame);
				program_normal_exit(__func__, __LINE__);
				break;
			}

			if (fp) {
				fwrite(pData, 1, stFrame.pstPack->u32Len, fp);
				fflush(fp);
			}

			RK_LOGE("venc %d get_stream count: %d", s32ChnId, loopCount);

			RK_MPI_VENC_ReleaseStream(s32ChnId, &stFrame);
			loopCount++;
		}
	}

	if (fp) {
		fsync(s32fd);
		fclose(fp);
		fp = RK_NULL;
	}
	if (stFrame.pstPack) {
		free(stFrame.pstPack);
		stFrame.pstPack = RK_NULL;
	}
	RK_LOGE("venc_get_stream chnid:%d exit", s32ChnId);
	return RK_NULL;
}

static void *combo_jpeg_get_stream(void *pArgs) {

	RK_S32 s32ChnId = *(RK_S32 *)pArgs;
	RK_S32 s32Ret = RK_FAILURE;
	FILE *fp = RK_NULL;
	RK_S32 s32fd = 0;
	RK_S32 loopCount = 0;
	RK_VOID *pData = RK_NULL;
	RK_CHAR name[BUFFER_SIZE] = {0};
	VENC_STREAM_S stFrame;
	RK_U32 u32TakePhotoCount = 0;
	VENC_RECV_PIC_PARAM_S stRecvParam;

	memset(&stFrame, 0, sizeof(VENC_STREAM_S));
	stFrame.pstPack = (VENC_PACK_S *)(malloc(sizeof(VENC_PACK_S)));
	if (!stFrame.pstPack) {
		RK_LOGE("malloc for stFrame.pstPack failure, chnid:%d", s32ChnId);
		program_handle_error(__func__, __LINE__);
		return RK_NULL;
	}

	while (!g_bIfVencThreadQuit[s32ChnId]) {
		s32Ret = RK_MPI_VENC_GetStream(s32ChnId, &stFrame, 2000);
		if (s32Ret == RK_SUCCESS) {
			pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);

			if (g_pOutPath) {
				snprintf(name, sizeof(name), "/%s/jpeg_chn%d_%d.jpeg", g_pOutPath,
				         s32ChnId, u32TakePhotoCount);
				fp = fopen(name, "wb");
				if (fp == RK_NULL) {
					RK_LOGE("chn %d can't open %s file !\n", s32ChnId, g_pOutPath);
					program_handle_error(__func__, __LINE__);
					return RK_NULL;
				}
				s32fd = fileno(fp);
			}

			if (fp) {
				fwrite(pData, 1, stFrame.pstPack->u32Len, fp);
				fflush(fp);
				fsync(s32fd);
				fclose(fp);
				fp = RK_NULL;
			}

			RK_LOGE("-------------------venc %d get_stream count: %d", s32ChnId,
			        loopCount);

			RK_MPI_VENC_ReleaseStream(s32ChnId, &stFrame);
			loopCount++;
			u32TakePhotoCount++;

			if (g_s32loopCnt > 0 && loopCount >= g_s32loopCnt) {
				program_normal_exit(__func__, __LINE__);
				break;
			}

			if (u32TakePhotoCount >= g_s32JpegCaptureNum) {
				RK_LOGE("restart take photo");
				RK_MPI_VENC_StopRecvFrame(s32ChnId);
				memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
				stRecvParam.s32RecvPicNum = g_s32JpegCaptureNum;
				s32Ret = RK_MPI_VENC_StartRecvFrame(s32ChnId, &stRecvParam);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VENC_StartRecvFrame failure:%X, s32Ret");
				}
				u32TakePhotoCount = 0;
			}

		} else {
			RK_LOGE("RK_MPI_VENC_GetStream failure:%X", s32Ret);
		}
	}

	if (stFrame.pstPack) {
		free(stFrame.pstPack);
		stFrame.pstPack = RK_NULL;
	}
	RK_LOGE("venc_get_stream chnid:%d exit", s32ChnId);
	return RK_NULL;
}

static RK_S32 vi_init(RK_S32 s32DevId, RK_S32 s32ChnId, RK_U32 u32Width,
                      RK_U32 u32Height) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32PipeId = s32DevId;
	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	VI_CHN_ATTR_S stChnAttr;

	memset(&stDevAttr, 0, sizeof(VI_DEV_ATTR_S));
	s32Ret = RK_MPI_VI_GetDevAttr(s32DevId, &stDevAttr);
	if (s32Ret == RK_ERR_VI_NOT_CONFIG) {
		/* config dev */
		s32Ret = RK_MPI_VI_SetDevAttr(s32DevId, &stDevAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetDevAttr failed with %#x!\n", s32Ret);
			program_handle_error(__func__, __LINE__);
			goto __FAILED1;
		}
	} else {
		RK_LOGE("RK_MPI_VI_SetDevAttr already!");
	}

	/* get dev enable status */
	s32Ret = RK_MPI_VI_GetDevIsEnable(s32DevId);
	if (s32Ret != RK_SUCCESS) {
		/* enable dev */
		s32Ret = RK_MPI_VI_EnableDev(s32DevId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_EnableDev failed with %#x!\n", s32Ret);
			program_handle_error(__func__, __LINE__);
			goto __FAILED1;
		}
		memset(&stBindPipe, 0, sizeof(VI_DEV_BIND_PIPE_S));
		/* bind dev/pipe */
		stBindPipe.u32Num = 1;
		stBindPipe.PipeId[0] = s32PipeId;
		s32Ret = RK_MPI_VI_SetDevBindPipe(s32DevId, &stBindPipe);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetDevBindPipe failed with %#x!\n", s32Ret);
			program_handle_error(__func__, __LINE__);
			goto __FAILED2;
		}
	} else {
		RK_LOGE("RK_MPI_VI_EnableDev already!");
	}

	/* config channel */
	memset(&stChnAttr, 0, sizeof(VI_CHN_ATTR_S));
	stChnAttr.stSize.u32Width = u32Width;
	stChnAttr.stSize.u32Height = u32Height;
	stChnAttr.stIspOpt.stMaxSize.u32Width = u32Width;
	stChnAttr.stIspOpt.stMaxSize.u32Height = u32Height;
	stChnAttr.stIspOpt.u32BufCount = 3;
	stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	stChnAttr.u32Depth = 0; //0, get fail, 1 - u32BufCount, can get, if bind to other device, must be < u32BufCount
	stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	stChnAttr.stFrameRate.s32DstFrameRate = -1;
	s32Ret = RK_MPI_VI_SetChnAttr(s32PipeId, s32ChnId, &stChnAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_SetChnAttr failed with %#x!\n", s32Ret);
		program_handle_error(__func__, __LINE__);
		goto __FAILED2;
	}

	if (g_wrap) {
		VI_CHN_BUF_WRAP_S stViWrap;
		memset(&stViWrap, 0, sizeof(VI_CHN_BUF_WRAP_S));

		stViWrap.bEnable = RK_TRUE;
		stViWrap.u32BufLine = u32Height / 4;
		stViWrap.u32WrapBufferSize = stViWrap.u32BufLine * u32Width * 3 / 2;
		s32Ret = RK_MPI_VI_SetChnWrapBufAttr(s32PipeId, s32ChnId, &stViWrap);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VI_SetChnWrapBufAttr failure:%X", s32Ret);
			program_handle_error(__func__, __LINE__);
			goto __FAILED2;
		}
	}

	/* enable channel */
	s32Ret = RK_MPI_VI_EnableChn(s32PipeId, s32ChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_EnableChn failed with %#x!\n", s32Ret);
		program_handle_error(__func__, __LINE__);
		goto __FAILED2;
	}

	return RK_SUCCESS;

/* disable dev(will diabled all chn) */
__FAILED2:
	s32Ret = RK_MPI_VI_DisableDev(s32DevId);
	RK_LOGE("RK_MPI_VI_DisableDev with %#x!\n", s32Ret);
__FAILED1:
	return s32Ret;
}

static RK_S32 venc_init(RK_S32 s32ChnId, RK_U32 u32Width, RK_U32 u32Height,
                        RK_CODEC_ID_E enCodecType, RK_BOOL bIfEnableCombo) {
	RK_S32 s32Ret = RK_FAILURE;
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_ATTR_S stAttr;

	memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
	/* set venc_attr*/
	if (enCodecType == RK_VIDEO_ID_AVC) {
		stAttr.stVencAttr.enType = RK_VIDEO_ID_AVC;
	} else if (enCodecType == RK_VIDEO_ID_JPEG) {
		stAttr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
	} else {
		RK_LOGE("the enType is no support in this simple");
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	stAttr.stVencAttr.u32MaxPicWidth = u32Width;
	stAttr.stVencAttr.u32MaxPicHeight = u32Height;
	stAttr.stVencAttr.u32PicWidth = u32Width;
	stAttr.stVencAttr.u32PicHeight = u32Height;
	stAttr.stVencAttr.u32VirWidth = RK_ALIGN_2(u32Width);
	stAttr.stVencAttr.u32VirHeight = RK_ALIGN_2(u32Height);
	stAttr.stVencAttr.u32StreamBufCnt = 3;
	stAttr.stVencAttr.u32BufSize = u32Width * u32Height / 2;

	/* set rc_attr */
	if (enCodecType == RK_VIDEO_ID_AVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
		stAttr.stRcAttr.stH264Cbr.u32Gop = 50;
		stAttr.stRcAttr.stH264Cbr.u32BitRate = 2 * 1024;
	} else if (enCodecType == RK_VIDEO_ID_JPEG) {
		stAttr.stVencAttr.stAttrJpege.bSupportDCF = RK_FALSE;
		stAttr.stVencAttr.stAttrJpege.stMPFCfg.u8LargeThumbNailNum = 0;
		stAttr.stVencAttr.stAttrJpege.enReceiveMode = VENC_PIC_RECEIVE_SINGLE;
	}

	/* set gop_attr */
	if (enCodecType == RK_VIDEO_ID_AVC) {
		stAttr.stGopAttr.enGopMode = VENC_GOPMODE_NORMALP;
	} else if (enCodecType == RK_VIDEO_ID_JPEG) {
		stAttr.stGopAttr.enGopMode = VENC_GOPMODE_INIT;
	}

	s32Ret = RK_MPI_VENC_CreateChn(s32ChnId, &stAttr);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_CreateChn failure:%X chnid:%d", s32Ret, s32ChnId);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	if (g_wrap) {
		VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
		memset(&stVencChnBufWrap, 0, sizeof(VENC_CHN_BUF_WRAP_S));

		stVencChnBufWrap.bEnable = RK_TRUE;
		stVencChnBufWrap.u32BufLine = u32Height / 4;
		s32Ret = RK_MPI_VENC_SetChnBufWrapAttr(s32ChnId, &stVencChnBufWrap);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VENC_SetChnBufWrapAttr failure:%X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return RK_FAILURE;
		}
	}

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	if (enCodecType == RK_VIDEO_ID_JPEG) {
		stRecvParam.s32RecvPicNum = g_s32JpegCaptureNum;
	} else {
		stRecvParam.s32RecvPicNum = -1;
	}
	s32Ret = RK_MPI_VENC_StartRecvFrame(s32ChnId, &stRecvParam);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VENC_StartRecvFrame failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return s32Ret;
	}

	/* set combo */
	if (bIfEnableCombo) {
		VENC_COMBO_ATTR_S stComboAttr;
		memset(&stComboAttr, 0, sizeof(VENC_COMBO_ATTR_S));
		stComboAttr.bEnable = RK_TRUE;
		stComboAttr.s32ChnId = 0;
		s32Ret = RK_MPI_VENC_SetComboAttr(s32ChnId, &stComboAttr);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("RK_MPI_VENC_SetComboAttr failure:%X", s32Ret);
			program_handle_error(__func__, __LINE__);
			return RK_FAILURE;
		}
	}

	return s32Ret;
}

static XCamReturn SIMPLE_COMM_ISP_SofCb(rk_aiq_metas_t *meta) {
	g_sof_cnt++;
	if (g_sof_cnt <= 2)
		printf("=== %u ===\n", meta->frame_id);
	return XCAM_RETURN_NO_ERROR;
}

static XCamReturn SIMPLE_COMM_ISP_ErrCb(rk_aiq_err_msg_t *msg) {
	if (msg->err_code == XCAM_RETURN_BYPASS)
		g_should_quit = true;
	return XCAM_RETURN_NO_ERROR;
}

RK_S32 SIMPLE_COMM_ISP_Init(RK_S32 CamId, rk_aiq_working_mode_t WDRMode, RK_BOOL MultiCam,
                            const char *iq_file_dir) {
	if (CamId >= MAX_AIQ_CTX) {
		printf("%s : CamId is over 3\n", __FUNCTION__);
		return -1;
	}
	// char *iq_file_dir = "iqfiles/";
	setlinebuf(stdout);
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

#ifdef RV1126_RV1109
	rk_aiq_uapi_sysctl_enumStaticMetas(CamId, &aiq_static_info);

	printf("ID: %d, sensor_name is %s, iqfiles is %s\n", CamId,
	       aiq_static_info.sensor_info.sensor_name, iq_file_dir);

	aiq_ctx =
	    rk_aiq_uapi_sysctl_init(aiq_static_info.sensor_info.sensor_name, iq_file_dir,
	                             SIMPLE_COMM_ISP_ErrCb, SIMPLE_COMM_ISP_SofCb);

	if (MultiCam)
		rk_aiq_uapi_sysctl_setMulCamConc(aiq_ctx, true);
#else
	rk_aiq_uapi2_sysctl_enumStaticMetas(CamId, &aiq_static_info);

	printf("ID: %d, sensor_name is %s, iqfiles is %s\n", CamId,
	       aiq_static_info.sensor_info.sensor_name, iq_file_dir);

	aiq_ctx =
	    rk_aiq_uapi2_sysctl_init(aiq_static_info.sensor_info.sensor_name, iq_file_dir,
	                             SIMPLE_COMM_ISP_ErrCb, SIMPLE_COMM_ISP_SofCb);

	if (MultiCam)
		rk_aiq_uapi2_sysctl_setMulCamConc(aiq_ctx, true);
#endif
	g_aiq_ctx[CamId] = aiq_ctx;
	return 0;
}

RK_S32 SIMPLE_COMM_ISP_Run(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init\n", __FUNCTION__);
		return -1;
	}

#ifdef RV1126_RV1109
	if (rk_aiq_uapi_sysctl_prepare(g_aiq_ctx[CamId], 0, 0, g_WDRMode[CamId])) {
		printf("rkaiq engine prepare failed !\n");
		g_aiq_ctx[CamId] = NULL;
		return -1;
	}
	printf("rk_aiq_uapi_sysctl_init/prepare succeed\n");
	if (rk_aiq_uapi_sysctl_start(g_aiq_ctx[CamId])) {
		printf("rk_aiq_uapi_sysctl_start  failed\n");
		return -1;
	}
	printf("rk_aiq_uapi_sysctl_start succeed\n");
#else
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
#endif
	return 0;
}

RK_S32 SIMPLE_COMM_ISP_Stop(RK_S32 CamId) {
	if (CamId >= MAX_AIQ_CTX || !g_aiq_ctx[CamId]) {
		printf("%s : CamId is over 3 or not init g_aiq_ctx[%d] = %p\n", __FUNCTION__,
		       CamId, g_aiq_ctx[CamId]);
		return -1;
	}
#ifdef RV1126_RV1109
	printf("rk_aiq_uapi_sysctl_stop enter\n");
	rk_aiq_uapi_sysctl_stop(g_aiq_ctx[CamId], false);
	printf("rk_aiq_uapi_sysctl_deinit enter\n");
	rk_aiq_uapi_sysctl_deinit(g_aiq_ctx[CamId]);
	printf("rk_aiq_uapi_sysctl_deinit exit\n");
#else
	printf("rk_aiq_uapi2_sysctl_stop enter\n");
	rk_aiq_uapi2_sysctl_stop(g_aiq_ctx[CamId], false);
	printf("rk_aiq_uapi2_sysctl_deinit enter\n");
	rk_aiq_uapi2_sysctl_deinit(g_aiq_ctx[CamId]);
	printf("rk_aiq_uapi2_sysctl_deinit exit\n");
#endif
	g_aiq_ctx[CamId] = NULL;
	return 0;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32CamId = 0;
	RK_S32 s32ViChnId = 0;
	RK_S32 s32ViDevId = 0;
	RK_S32 s32PipeId = 0;
	RK_S32 s32MainVencChnId = 0;
	RK_S32 s32ComboVencChnId = 1;
	MPP_CHN_S stSrcChn, stDestChn;
#ifdef RKAIQ
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
#endif
	pthread_t main_venc_thread_id, combo_venc_thread_id;

	if (argc < 2) {
		print_usage(argv[0]);
		return -1;
	}

	signal(SIGINT, sigterm_handler);
	signal(SIGTERM, sigterm_handler);

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
		case 'w':
			g_u32MainVencWidth = atoi(optarg);
			break;
		case 'h':
			g_u32MainVencHeight = atoi(optarg);
			break;
		case 'l':
			g_s32loopCnt = atoi(optarg);
			break;
		case 'o':
			g_pOutPath = optarg;
			break;
		case 'r':
			if (0 == atoi(optarg)) {
				g_wrap = RK_FALSE;
			} else if (1 == atoi(optarg)) {
				g_wrap = RK_TRUE;
			} else {
				RK_LOGE("input wrap mode is no support(invalid)");
				print_usage(argv[0]);
				return -1;
			}
			break;
		case 'j':
			g_s32JpegCaptureNum = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return -1;
		}
	}

	printf("#CameraIdx: %d\n", s32CamId);
	printf("#Output Path: %s\n", g_pOutPath);
	printf("#IQ Path: %s\n", iq_file_dir);
	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);

		s32Ret = SIMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, iq_file_dir);
		s32Ret |= SIMPLE_COMM_ISP_Run(s32CamId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("ISP init failure:%X", s32Ret);
			g_exit_result = -1;
			goto __FAILED2;
		}
#endif
	}
	RK_LOGE("ISP init success");
	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		g_exit_result = -1;
		goto __FAILED;
	}

	// Init VI
	vi_init(s32ViDevId, s32ViChnId, g_u32MainVencWidth, g_u32MainVencHeight);

	/* Init VENC[0] */
	venc_init(s32MainVencChnId, g_u32MainVencWidth, g_u32MainVencHeight, RK_VIDEO_ID_AVC,
	          RK_FALSE);
	/* Init combo VENC[1] */
	venc_init(s32ComboVencChnId, g_u32MainVencWidth, g_u32MainVencHeight,
	          RK_VIDEO_ID_JPEG, RK_TRUE);

	/* VI[0] bind VENC[0] */
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = s32ViDevId;
	stSrcChn.s32ChnId = s32ViChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = s32MainVencChnId;
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
	}

	pthread_create(&main_venc_thread_id, 0, venc_get_stream, &s32MainVencChnId);
	pthread_create(&combo_venc_thread_id, 0, combo_jpeg_get_stream, &s32ComboVencChnId);

	printf("%s initial finish\n", __func__);

	while (!g_quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	/* Venc[1] deinit */
	g_bIfVencThreadQuit[s32ComboVencChnId] = RK_TRUE;
	pthread_join(combo_venc_thread_id, RK_NULL);
	s32Ret = RK_MPI_VENC_StopRecvFrame(s32ComboVencChnId);
	s32Ret |= RK_MPI_VENC_DestroyChn(s32ComboVencChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Venc 0 destroy failure");
		g_exit_result = -1;
	}
	RK_LOGE("Venc 1 destroy success");

	/* Venc[0] deinit */
	g_bIfVencThreadQuit[s32MainVencChnId] = RK_TRUE;
	pthread_join(main_venc_thread_id, RK_NULL);

	/* vi[0] venc[0] ubind*/
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = s32ViDevId;
	stSrcChn.s32ChnId = s32ViChnId;
	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = s32MainVencChnId;
	s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("VI and VENC bind failure:%X", s32Ret);
		g_exit_result = -1;
	}

	s32Ret = RK_MPI_VENC_StopRecvFrame(s32MainVencChnId);
	s32Ret |= RK_MPI_VENC_DestroyChn(s32MainVencChnId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("Venc 0 destroy failure");
		g_exit_result = -1;
	}

	RK_LOGE("Venc 0 destroy success");

	/*  Destroy Vi deinit */
	s32Ret = RK_MPI_VI_DisableChn(s32PipeId, s32ViChnId);
	s32Ret |= RK_MPI_VI_DisableDev(s32ViDevId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_Close failed with %X!", s32Ret);
		g_exit_result = -1;
	}

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		SIMPLE_COMM_ISP_Stop(s32CamId);
#endif
	}

#ifdef RKAIQ
__FAILED2:
	return g_exit_result;
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
