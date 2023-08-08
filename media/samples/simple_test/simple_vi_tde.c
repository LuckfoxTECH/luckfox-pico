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
#include <stdio.h>
#include <sys/poll.h>
#include <sys/prctl.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

#define GET_STREAM_TIMEOUT 2000
#define BUFFER_SIZE 255

static RK_BOOL g_quit = RK_FALSE;
static RK_S32 g_exit_result = RK_SUCCESS;
static RK_BOOL g_bIfViThreadQuit = RK_FALSE;
static RK_U32 g_u32ViWidth = 1920;
static RK_U32 g_u32ViHeight = 1080;
static RK_U32 g_u32TdeDstWidth = 704;
static RK_U32 g_u32TdeDstHeight = 576;
static RK_CHAR *g_pOutPath = RK_NULL;
static RK_S32 g_s32loopCnt = -1;

static void program_handle_error(const char *func, RK_U32 line) {
	RK_LOGE("func: <%s> line: <%d> error exit!", func, line);
	g_exit_result = RK_FAILURE;
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

static RK_CHAR optstr[] = "?::a::w:h:o:l:";
static const struct option long_options[] = {
    {"aiq", optional_argument, NULL, 'a'},
    {"width", required_argument, NULL, 'w'},
    {"height", required_argument, NULL, 'h'},
    {"output_path", required_argument, NULL, 'o'},
    {"loop_count", required_argument, NULL, 'l'},
    {"help", optional_argument, NULL, '?'},
};

/******************************************************************************
 * function : show usage
 ******************************************************************************/
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
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
	printf("\t-o | --output_path: Tde output file path, Default NULL\n");
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
	stChnAttr.stIspOpt.u32BufCount = 2;
	stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	stChnAttr.u32Depth = 1;
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

/******************************************************************************
 * function : vi thread
 ******************************************************************************/
static void *vi_get_stream(void *pArgs) {
	prctl(PR_SET_NAME, "vi_tde_thread");
	RK_S32 s32Ret = RK_FAILURE;
	char name[256] = {0};
	FILE *fp = RK_NULL;
	RK_S32 s32fd = 0;
	RK_S32 loopCount = 0;
	TDE_HANDLE hHandle;
	TDE_SURFACE_S stSrc, stDst;
	TDE_RECT_S stSrcRect, stDstRect;
	VIDEO_FRAME_INFO_S stViFrame, DstFrame;
	PIC_BUF_ATTR_S stTdeOutPicBufAttr;
	MB_PIC_CAL_S stMbPicCalResult;

	/* alloc tde output blk*/
	memset(&stTdeOutPicBufAttr, 0, sizeof(PIC_BUF_ATTR_S));
	memset(&stMbPicCalResult, 0, sizeof(MB_PIC_CAL_S));

	memset(&stViFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
	memset(&DstFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
	DstFrame.stVFrame.enPixelFormat = RK_FMT_YUV420SP;
	DstFrame.stVFrame.enCompressMode = COMPRESS_MODE_NONE;

	stTdeOutPicBufAttr.u32Width = g_u32TdeDstWidth;
	stTdeOutPicBufAttr.u32Height = g_u32TdeDstHeight;
	stTdeOutPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
	stTdeOutPicBufAttr.enPixelFormat = RK_FMT_YUV420SP;
	s32Ret = RK_MPI_CAL_TDE_GetPicBufferSize(&stTdeOutPicBufAttr, &stMbPicCalResult);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_CAL_TDE_GetPicBufferSize failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return RK_NULL;
	}
	s32Ret = RK_MPI_SYS_MmzAlloc(&DstFrame.stVFrame.pMbBlk, RK_NULL, RK_NULL,
	                             stMbPicCalResult.u32MBSize);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_SYS_MmzAlloc failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
		return RK_NULL;
	}

	/* set tde src/dst rect*/
	stSrc.u32Width = g_u32ViWidth;
	stSrc.u32Height = g_u32ViHeight;
	stSrc.enColorFmt = RK_FMT_YUV420SP;
	stSrc.enComprocessMode = COMPRESS_MODE_NONE;
	stSrcRect.s32Xpos = 0;
	stSrcRect.s32Ypos = 0;
	stSrcRect.u32Width = g_u32TdeDstWidth;
	stSrcRect.u32Height = g_u32TdeDstHeight;

	stDst.u32Width = g_u32TdeDstWidth;
	stDst.u32Height = g_u32TdeDstHeight;
	stDst.enColorFmt = RK_FMT_YUV420SP;
	stDst.enComprocessMode = COMPRESS_MODE_NONE;
	stDstRect.s32Xpos = 0;
	stDstRect.s32Ypos = 0;
	stDstRect.u32Width = g_u32TdeDstWidth;
	stDstRect.u32Height = g_u32TdeDstHeight;

	if (g_pOutPath) {
		snprintf(name, sizeof(name), "/%s/tde_out.bin", g_pOutPath);
		fp = fopen(name, "wb");
		if (fp == RK_NULL) {
			RK_LOGE("open %s failure", name);
			program_handle_error(__func__, __LINE__);
			goto __TDE_INIT_FAILED;
		}
		s32fd = fileno(fp);
	}

	/* tde open */
	s32Ret = RK_TDE_Open();
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_TDE_Open failure:%X", s32Ret);
		program_handle_error(__func__, __LINE__);
		goto __TDE_INIT_FAILED;
	}
	while (!g_bIfViThreadQuit) {
		s32Ret = RK_MPI_VI_GetChnFrame(0, 0, &stViFrame, GET_STREAM_TIMEOUT);
		if (s32Ret == RK_SUCCESS) {

			if (g_s32loopCnt > 0 && loopCount >= g_s32loopCnt) {
				RK_LOGE("tde handle loopCnt is up to %d", g_s32loopCnt);
				RK_MPI_VI_ReleaseChnFrame(0, 0, &stViFrame);
				program_normal_exit(__func__, __LINE__);
				goto __TDE_HANDLE_FAILED;
			}

			hHandle = RK_TDE_BeginJob();
			if (RK_ERR_TDE_INVALID_HANDLE == hHandle) {
				RK_LOGE("RK_TDE_BeginJob Failure");
				RK_MPI_VI_ReleaseChnFrame(0, 0, &stViFrame);
				program_handle_error(__func__, __LINE__);
				goto __TDE_HANDLE_FAILED;
			}

			stSrc.pMbBlk = stViFrame.stVFrame.pMbBlk;
			stDst.pMbBlk = DstFrame.stVFrame.pMbBlk;
			DstFrame.stVFrame.u32TimeRef = stViFrame.stVFrame.u32TimeRef;
			DstFrame.stVFrame.u64PTS = stViFrame.stVFrame.u64PTS;

			s32Ret = RK_TDE_QuickCopy(hHandle, &stSrc, &stSrcRect, &stDst, &stDstRect);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_TDE_QuickCopy Failure %#X ", s32Ret);
				RK_MPI_VI_ReleaseChnFrame(0, 0, &stViFrame);
				RK_TDE_CancelJob(hHandle);
				program_handle_error(__func__, __LINE__);
				goto __TDE_HANDLE_FAILED;
			}

			s32Ret = RK_TDE_EndJob(hHandle, RK_FALSE, RK_TRUE, -1);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_TDE_EndJob Failure %#X ", s32Ret);
				RK_MPI_VI_ReleaseChnFrame(0, 0, &stViFrame);
				RK_TDE_CancelJob(hHandle);
				program_handle_error(__func__, __LINE__);
				goto __TDE_HANDLE_FAILED;
			}

			s32Ret = RK_TDE_WaitForDone(hHandle);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_TDE_WaitForDone Failure s32Ret: %#X", s32Ret);
				RK_MPI_VI_ReleaseChnFrame(0, 0, &stViFrame);
				program_handle_error(__func__, __LINE__);
				goto __TDE_HANDLE_FAILED;
			}
			/* save tde out file */
			if (fp) {
				fwrite(RK_MPI_MB_Handle2VirAddr(DstFrame.stVFrame.pMbBlk), 1,
				       stMbPicCalResult.u32MBSize, fp);
				fflush(fp);
			}
			loopCount++;
			RK_LOGE("------------Tde hanlde one task success, count:%d", loopCount);

			s32Ret = RK_MPI_VI_ReleaseChnFrame(0, 0, &stViFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_ReleaseChnFrame failure:%X pipe:%d chnid:%d", s32Ret,
				        0, 0);
				program_handle_error(__func__, __LINE__);
				goto __TDE_HANDLE_FAILED;
			}

		} else {
			RK_LOGE("RK_MPI_VI_GetChnFrame Failure:%X pipe:%d chnid:%d", s32Ret, 0, 0);
		}
	}
	RK_LOGE("vi get stream exit");

__TDE_HANDLE_FAILED:
	RK_TDE_Close();

__TDE_INIT_FAILED:
	if (DstFrame.stVFrame.pMbBlk) {
		RK_MPI_SYS_Free(DstFrame.stVFrame.pMbBlk);
		DstFrame.stVFrame.pMbBlk = RK_NULL;
	}
	if (fp) {
		fsync(s32fd);
		fclose(fp);
		fp = RK_NULL;
	}
	return RK_NULL;
}

/******************************************************************************
 * function    : main()
 * Description : main
 ******************************************************************************/
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_S32 s32CamId = 0;
	RK_S32 s32ChnId = 0;
	RK_S32 s32DevId = 0;
	RK_S32 s32PipeId = 0;
	rk_aiq_working_mode_t hdr_mode = RK_AIQ_WORKING_MODE_NORMAL;
	pthread_t vi_thread_id;

	if (argc < 2) {
		print_usage(argv[0]);
		return 0;
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
			g_u32ViWidth = atoi(optarg);
			break;
		case 'h':
			g_u32ViHeight = atoi(optarg);
			break;
		case 'l':
			g_s32loopCnt = atoi(optarg);
			break;
		case 'o':
			g_pOutPath = optarg;
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#CameraIdx: %d\n", s32CamId);
	printf("#Output Path: %s\n", g_pOutPath);
	printf("#IQ Path: %s\n", iq_file_dir);
	if (iq_file_dir) {
#ifdef RKAIQ
		printf("#Rkaiq XML DirPath: %s\n", iq_file_dir);
		printf("#bMultictx: %d\n\n", bMultictx);

		s32Ret = SAMPLE_COMM_ISP_Init(s32CamId, hdr_mode, bMultictx, iq_file_dir);
		s32Ret |= SAMPLE_COMM_ISP_Run(s32CamId);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("ISP init Failure:%X", s32Ret);
			g_exit_result = RK_FALSE;
			goto __CTX_ALLOC_FAILED;
		}
#endif
	}

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		g_exit_result = RK_FALSE;
		goto __FAILED;
	}

	/* Init VI */
	vi_init(s32DevId, s32ChnId, g_u32ViWidth, g_u32ViHeight);

	pthread_create(&vi_thread_id, 0, vi_get_stream, RK_NULL);

	printf("%s initial finish\n", __func__);

	while (!g_quit) {
		sleep(1);
	}

	printf("%s exit!\n", __func__);

	/*  Destroy Vi deinit */
	g_bIfViThreadQuit = RK_TRUE;
	pthread_join(vi_thread_id, NULL);

	s32Ret = RK_MPI_VI_DisableChn(s32PipeId, s32ChnId);
	s32Ret |= RK_MPI_VI_DisableDev(s32DevId);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_MPI_VI_Close failed with %#x!\n", s32Ret);
		g_exit_result = RK_FALSE;
	}

__FAILED:
	RK_MPI_SYS_Exit();
	if (iq_file_dir) {
#ifdef RKAIQ
		SAMPLE_COMM_ISP_Stop(s32CamId);
#endif
	}

__CTX_ALLOC_FAILED:
	return g_exit_result;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
