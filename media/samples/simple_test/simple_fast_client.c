#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "rk_debug.h"
#include "rk_defines.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_vi.h"
#include "rk_type.h"
#include "sample_comm.h"

#include "rk_gpio.h"
#include "rk_pwm.h"
#include "rtsp_demo.h"
#include <rk_smart_ir_api.h>
#define ENABLE_RKAIQ 1
#define ENABLE_RTSP 1
#define ENABLE_SMALL_STREAM 1
#define ENABLE_CHANGE_RESOLUTION 0

#define MAP_SIZE (4096UL * 50)  // MAP_SIZE = 4 * 50 K
#define MAP_MASK (MAP_SIZE - 1) // MAP_MASK = 0XFFF

#define MAP_SIZE_NIGHT (4096UL)             // MAP_SIZE = 4K
#define MAP_MASK_NIGHT (MAP_SIZE_NIGHT - 1) // MAP_MASK = 0XFFF

#define SAVE_ENC_FRM_CNT_MAX 30
#define RUN_TOTAL_CNT_MAX 1000000

static FILE *venc0_file;
static RK_S32 g_s32FrameCnt = -1;
// static RK_U32 g_u32WrapLine = 0;
static char *g_sEntityName = NULL;
static bool quit = false;
static int venc_w[3] = {1920, 1280, 640};
static int venc_h[3] = {1080, 720, 480};
static int index_w_h = 1;
static int index_num = 3;

// SmartIr Params
#define ircut_on_gpio GPIO(RK_GPIO1, RK_PD1)
#define ircut_off_gpio GPIO(RK_GPIO1, RK_PD3)
#define irled_enable_gpio GPIO(RK_GPIO1, RK_PD0)
#define irled_pwm_channel 11
static float d2n_envL_th = 0.04f;
static float n2d_envL_th = 0.20f;
static float rggain_base = 1.0f;
static float bggain_base = 1.0f;
static float awbgain_rad = 0.10f;
static float awbgain_dis = 0.22f;
int rk_led_value = 100;

rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session;

typedef struct rk_smart_ir_ctx_s {
	const rk_aiq_sys_ctx_t *aiq_ctx;
	int cur_working_mode;
	rk_smart_ir_params_t ir_configs;
	RK_SMART_IR_STATUS_t state;
	uint16_t switch_cnts;
	bool awbgain_update;
} rk_smart_ir_ctx_t;

typedef struct sample_smartIr_s {
	pthread_t tid;
	bool tquit;
	bool started;
	const rk_aiq_sys_ctx_t *aiq_ctx;
	rk_smart_ir_ctx_t *ir_ctx;
	rk_smart_ir_params_t ir_configs;
	rk_smart_ir_result_t ir_res;
	rk_aiq_isp_stats_t *isp_status;
} sample_smartIr_t;
static sample_smartIr_t g_sample_smartIr_ctx;

static void sigterm_handler(int sig) { quit = true; }

static RK_CHAR optstr[] = "?::d:n:s:B:b:R:r:";
static const struct option long_options[] = {
    {"d2n", required_argument, NULL, 'd'},
    {"n2d", required_argument, NULL, 'n'},
    {"switch_cnt", required_argument, NULL, 's'},
    {"bg_max", required_argument, NULL, 'B'},
    {"bg_min", required_argument, NULL, 'b'},
    {"rg_max", required_argument, NULL, 'R'},
    {"rg_min", optional_argument, NULL, 'r'},
    {"help", optional_argument, NULL, '?'},
    {NULL, 0, NULL, 0},
};

RK_U64 TEST_COMM_GetNowUs() {
	struct timespec time = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

int rk_isp_enable_ircut(bool on) {

	if (on) {
		rk_gpio_set_value(ircut_on_gpio, 1);
		usleep(100 * 1000);
		rk_gpio_set_value(ircut_on_gpio, 0);

	} else {
		rk_gpio_set_value(ircut_off_gpio, 1);
		usleep(100 * 1000);
		rk_gpio_set_value(ircut_off_gpio, 0);
	}
}

static void load_ir_configs(float d2n, float n2d, float rbase, float bbase, float rad,
                            float dis, int switch_cnt) {
	sample_smartIr_t *smartIr_ctx = &g_sample_smartIr_ctx;

	smartIr_ctx->ir_configs.d2n_envL_th = d2n;
	smartIr_ctx->ir_configs.n2d_envL_th = n2d;
	smartIr_ctx->ir_configs.rggain_base = rbase;
	smartIr_ctx->ir_configs.bggain_base = bbase;
	smartIr_ctx->ir_configs.awbgain_rad = rad;
	smartIr_ctx->ir_configs.awbgain_dis = dis;
	smartIr_ctx->ir_configs.switch_cnts_th = switch_cnt;
	rk_smart_ir_config(smartIr_ctx->ir_ctx, &smartIr_ctx->ir_configs);
}

static void *switch_ir_thread(void *args) {
	sample_smartIr_t *smartIr_ctx = &g_sample_smartIr_ctx;
	int init_stat = smartIr_ctx->ir_ctx->state;
	int ret, switch_flag, sleep_count = 15;
	int irled_pwm_period = 5000, irled_pwm_duty = 0;
	FILE *fp;

	ret |= rk_gpio_export_direction(ircut_on_gpio, GPIO_DIRECTION_OUTPUT);
	ret |= rk_gpio_export_direction(ircut_off_gpio, GPIO_DIRECTION_OUTPUT);
	ret |= rk_gpio_export_direction(irled_enable_gpio, GPIO_DIRECTION_OUTPUT);

	if (init_stat == RK_SMART_IR_STATUS_NIGHT) {
		switch_flag = 0;
		rk_gpio_set_value(irled_enable_gpio, 1);
	} else {
		switch_flag = 1;
		rk_gpio_set_value(irled_enable_gpio, 0);
	}

	while (--sleep_count >= 0) {
		if ((access("/dev/block/by-name/meta", F_OK)) == 0) {
			printf("load meta partition finished\n");
			break;
		}
		usleep(1000 * 1000);
	}

	irled_pwm_duty = MIN(rk_led_value, 100) / 100 * irled_pwm_period;
	ret = rk_pwm_init(irled_pwm_channel, irled_pwm_period, irled_pwm_duty,
	                  PWM_POLARITY_NORMAL);
	if (ret) {
		printf("rk_pwm_init error ret [%d]\n", ret);
	}

	while (!smartIr_ctx->tquit && (quit == false)) {
		rk_aiq_uapi2_sysctl_get3AStatsBlk(smartIr_ctx->aiq_ctx, &smartIr_ctx->isp_status,
		                                  -1);
		rk_smart_ir_runOnce(smartIr_ctx->ir_ctx, smartIr_ctx->isp_status,
		                    &smartIr_ctx->ir_res);
		rk_aiq_uapi2_sysctl_release3AStatsRef(smartIr_ctx->aiq_ctx,
		                                      smartIr_ctx->isp_status);
		if (smartIr_ctx->ir_res.status == RK_SMART_IR_STATUS_DAY) {
			if (switch_flag != 0) {
				switch_flag = 0;
				rk_gpio_set_value(irled_enable_gpio, 0);
				if (rk_pwm_set_enable(irled_pwm_channel, false))
					printf("pwm%d disable failed %d\n", irled_pwm_channel);
				rk_isp_enable_ircut(true);
				rk_aiq_uapi2_sysctl_switch_scene(smartIr_ctx->aiq_ctx, "normal", "day");

				printf("SAMPLE_SMART_IR: switch to DAY\n");
				system("make_meta --update --meta_path /dev/block/by-name/meta "
				       "--rk_color_mode 0");
			}
		} else if (smartIr_ctx->ir_res.status == RK_SMART_IR_STATUS_NIGHT) {
			if (switch_flag != 1) {
				switch_flag = 1;
				rk_aiq_uapi2_sysctl_switch_scene(smartIr_ctx->aiq_ctx, "normal", "night");
				rk_isp_enable_ircut(false);
				rk_gpio_set_value(irled_enable_gpio, 1);
				if (rk_pwm_set_enable(irled_pwm_channel, true))
					printf("pwm%d enable failed %d\n", irled_pwm_channel);

				printf("SAMPLE_SMART_IR: switch to Night\n");
				system("make_meta --update --meta_path /dev/block/by-name/meta "
				       "--rk_color_mode 1");
			}
		}
	}

	return NULL;
}

void sample_smartIr_stop() {
	sample_smartIr_t *smartIr_ctx = &g_sample_smartIr_ctx;

	printf("%s-%d\n", __func__, __LINE__);
	rk_pwm_deinit(irled_pwm_channel);
	if (smartIr_ctx->started) {
		smartIr_ctx->tquit = true;
		pthread_join(smartIr_ctx->tid, NULL);
	}
	smartIr_ctx->started = false;

	if (smartIr_ctx->ir_ctx) {
		rk_smart_ir_deInit(smartIr_ctx->ir_ctx);
		smartIr_ctx->ir_ctx = NULL;
	}
}

static int errCnt = 0;
static void *GetMediaBuffer0(void *arg) {
	(void)arg;
	void *pData = RK_NULL;
	int loopCount = 0;
	int s32Ret;
	VENC_STREAM_S stFrame;

	FILE *fp = fopen("/tmp/pts.txt", "wb");
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(0, &stFrame, 1000);
		if (s32Ret == RK_SUCCESS) {
			if (venc0_file && loopCount <= SAVE_ENC_FRM_CNT_MAX) {
				pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
				fwrite(pData, 1, stFrame.pstPack->u32Len, venc0_file);
				fflush(venc0_file);
			}
			RK_U64 nowUs = TEST_COMM_GetNowUs();
			if (fp && (loopCount <= SAVE_ENC_FRM_CNT_MAX)) {
				char str[128] = {0};
				int len;
				printf("chn:0, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n",
				       loopCount, stFrame.u32Seq, stFrame.pstPack->u32Len,
				       stFrame.pstPack->u64PTS, nowUs - stFrame.pstPack->u64PTS);
				len = snprintf(str, sizeof(str), "seq:%u, pts:%llums\n", stFrame.u32Seq,
				               stFrame.pstPack->u64PTS / 1000);
				fwrite(str, 1, len, fp);
				fflush(fp);
			}
			loopCount++;
#if (ENABLE_RTSP)
			// tx video to rtspls
			if (loopCount > SAVE_ENC_FRM_CNT_MAX) {
				if (g_rtsplive && g_rtsp_session) {
					pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
					rtsp_tx_video(g_rtsp_session, pData, stFrame.pstPack->u32Len,
					              stFrame.pstPack->u64PTS);
					rtsp_do_event(g_rtsplive);
				}
			}
#endif
			errCnt = 0;
			s32Ret = RK_MPI_VENC_ReleaseStream(0, &stFrame);

#if (ENABLE_CHANGE_RESOLUTION)
			if ((loopCount >= SAVE_ENC_FRM_CNT_MAX) &&
			    ((loopCount % SAVE_ENC_FRM_CNT_MAX)) == 0) {
				MPP_CHN_S stSrcChn, stDestChn;
				VENC_CHN_ATTR_S stAttr;
				VI_CHN_ATTR_S stChnAttr;
				// unbind venc from vi
				stSrcChn.enModId = RK_ID_VI;
				stSrcChn.s32DevId = 0;
				stSrcChn.s32ChnId = 0;

				stDestChn.enModId = RK_ID_VENC;
				stDestChn.s32DevId = 0;
				stDestChn.s32ChnId = 0;
				s32Ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_SYS_UnBind fail %x", s32Ret);
					goto __FAILED;
				}
				s32Ret = RK_MPI_VENC_GetChnAttr(0, &stAttr);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VENC_GetChnAttr fail %x", s32Ret);
					goto __FAILED;
				}
				stAttr.stVencAttr.u32PicWidth = venc_w[index_w_h % index_num];
				stAttr.stVencAttr.u32PicHeight = venc_h[index_w_h % index_num];
				stAttr.stVencAttr.u32VirWidth = venc_w[index_w_h % index_num];
				stAttr.stVencAttr.u32VirHeight = venc_h[index_w_h % index_num];
				s32Ret = RK_MPI_VENC_SetChnAttr(0, &stAttr);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VENC_SetChnAttr fail %x", s32Ret);
					goto __FAILED;
				}
				usleep(80 * 1000);
				stChnAttr.stIspOpt.stMaxSize.u32Width = venc_w[0];
				stChnAttr.stIspOpt.stMaxSize.u32Height = venc_h[0];
				stChnAttr.stSize.u32Width = venc_w[index_w_h % index_num];
				stChnAttr.stSize.u32Height = venc_h[index_w_h % index_num];
				s32Ret = RK_MPI_VI_SetChnAttr(0, 0, &stChnAttr);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VI_SetChnAttr fail %x", s32Ret);
					goto __FAILED;
				}

				s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_SYS_Bind fail %x", s32Ret);
					goto __FAILED;
				}
				index_w_h++;
			}
#endif
#if (ENABLE_RTSP)
			if (loopCount == SAVE_ENC_FRM_CNT_MAX) {
				g_rtsplive = create_rtsp_demo(554);
				if (g_rtsplive == NULL) {
					printf("rtsp create fail");
					goto __FAILED;
				}
				g_rtsp_session = rtsp_new_session(g_rtsplive, "/live/0");
				if (g_rtsp_session == NULL) {
					printf("rtsp create session fail");
					goto __FAILED;
				}
				VENC_CHN_ATTR_S stAttr;
				s32Ret = RK_MPI_VENC_GetChnAttr(0, &stAttr);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("RK_MPI_VENC_GetChnAttr fail %x", s32Ret);
					goto __FAILED;
				}
				if (stAttr.stVencAttr.enType == RK_VIDEO_ID_AVC)
					rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
				else if (stAttr.stVencAttr.enType == RK_VIDEO_ID_HEVC)
					rtsp_set_video(g_rtsp_session, RTSP_CODEC_ID_VIDEO_H265, NULL, 0);
				rtsp_sync_video_ts(g_rtsp_session, rtsp_get_reltime(),
				                   rtsp_get_ntptime());
			}
#endif

		} else {
			if (errCnt < 10) {
				printf("RK_MPI_VENC_GetChnFrame fail %x\n", s32Ret);
			}
			errCnt++;
		}

#if 0
        if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
            quit = true;
            break;
        }
#endif
	}

__FAILED:
	if (venc0_file)
		fclose(venc0_file);

	if (fp)
		fclose(fp);

	free(stFrame.pstPack);
	return NULL;
}

static void *GetMediaBuffer(void *arg) {
	void *pData = RK_NULL;
	int loopCount = 0;
	int s32Ret;
	VENC_STREAM_S stFrame;
	int chn = (int)arg;
	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));
	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(chn, &stFrame, 1000);
		if (s32Ret == RK_SUCCESS) {
			if (loopCount < SAVE_ENC_FRM_CNT_MAX)
				printf("chn:%d, loopCount:%d enc->seq:%d,pts=%lld\n", chn, loopCount,
				       stFrame.u32Seq, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
			loopCount++;
			s32Ret = RK_MPI_VENC_ReleaseStream(chn, &stFrame);
		}
		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
			quit = true;
			break;
		}
	}
	free(stFrame.pstPack);
	return NULL;
}

static void *GetViBuffer(void *arg) {
	void *pData = RK_NULL;
	int loopCount = 0;
	int s32Ret;
	VIDEO_FRAME_INFO_S stFrame;
	int chn = (int)arg;
	int piple = ((int)arg >> 16);
	void *pVirAddr;

	while (!quit) {
		s32Ret = RK_MPI_VI_GetChnFrame(piple, chn, &stFrame, 1000);
		if (s32Ret == RK_SUCCESS) {
			if (loopCount < SAVE_ENC_FRM_CNT_MAX)
				printf("piple: %d, chn:%d, loopCount:%d vi->seq:%d pts=%lld\n", piple,
				       chn, loopCount, stFrame.stVFrame.u32TimeRef,
				       stFrame.stVFrame.u64PTS);
			loopCount++;
			pVirAddr = RK_MPI_MB_Handle2VirAddr(stFrame.stVFrame.pMbBlk);
			s32Ret = RK_MPI_VI_ReleaseChnFrame(piple, chn, &stFrame);
		}
		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
			quit = true;
			break;
		}
	}
	return NULL;
}

static RK_S32 test_jpeg_init(int chnId, int width, int height, RK_CODEC_ID_E enType) {
	VENC_CHN_ATTR_S stJpegChnAttr;
	VENC_JPEG_PARAM_S stJpegParam;
	VENC_RECV_PIC_PARAM_S stRecvParam;
	// memset(&stJpegChnAttr,0,sizeof(VENC_JPEG_PARAM_S));

	stJpegChnAttr.stVencAttr.enType = enType;
	stJpegChnAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	stJpegChnAttr.stVencAttr.u32PicWidth = width;
	stJpegChnAttr.stVencAttr.u32PicHeight = height;
	stJpegChnAttr.stVencAttr.u32VirWidth = width;
	stJpegChnAttr.stVencAttr.u32VirHeight = height;
	stJpegChnAttr.stVencAttr.u32StreamBufCnt = 1;
	stJpegChnAttr.stVencAttr.u32BufSize = width * height;

	stJpegChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_MJPEGCBR;
	stJpegChnAttr.stRcAttr.stMjpegCbr.u32BitRate = 64;
	stJpegChnAttr.stRcAttr.stMjpegCbr.fr32DstFrameRateDen = 1;
	stJpegChnAttr.stRcAttr.stMjpegCbr.fr32DstFrameRateNum = 1;
	stJpegChnAttr.stRcAttr.stMjpegCbr.u32SrcFrameRateDen = 1;
	stJpegChnAttr.stRcAttr.stMjpegCbr.u32SrcFrameRateNum = 1;
	// stJpegChnAttr.stVencAttr.u32Depth = 1;
	RK_MPI_VENC_CreateChn(chnId, &stJpegChnAttr);
	stJpegParam.u32Qfactor = 75;
	RK_MPI_VENC_SetJpegParam(chnId, &stJpegParam);

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

static RK_S32 test_venc_init(int chnId, int width, int height, RK_CODEC_ID_E enType) {
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
	VENC_CHN_ATTR_S stAttr;

	memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
	stVencChnBufWrap.bEnable = false;
	stVencChnBufWrap.u32BufLine = 1080;

	memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
	stVencChnRefBufShare.bEnable = true;

	if (enType == RK_VIDEO_ID_AVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
		stAttr.stRcAttr.stH264Cbr.u32BitRate = 2 * 1024;
		stAttr.stRcAttr.stH264Cbr.u32Gop = 60;
	} else if (enType == RK_VIDEO_ID_HEVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
		stAttr.stRcAttr.stH265Cbr.u32BitRate = 2 * 1024;
		stAttr.stRcAttr.stH265Cbr.u32Gop = 60;
	}

	stAttr.stVencAttr.enType = enType;
	stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	if (enType == RK_VIDEO_ID_AVC)
		stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
	else if (enType == RK_VIDEO_ID_HEVC)
		stAttr.stVencAttr.u32Profile = H265E_PROFILE_MAIN;
	stAttr.stVencAttr.u32PicWidth = width;
	stAttr.stVencAttr.u32PicHeight = height;
	stAttr.stVencAttr.u32VirWidth = width;
	stAttr.stVencAttr.u32VirHeight = height;
	stAttr.stVencAttr.u32StreamBufCnt = 5;
	stAttr.stVencAttr.u32BufSize = width * height / 2;
	stAttr.stVencAttr.enMirror = MIRROR_NONE;

	RK_MPI_VENC_CreateChn(chnId, &stAttr);
	RK_MPI_VENC_SetChnBufWrapAttr(chnId, &stVencChnBufWrap);
	RK_MPI_VENC_SetChnRefBufShareAttr(chnId, &stVencChnRefBufShare);

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

int vi_dev_init(int devId, int pipeId) {
	int ret = 0;
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
			return -1;
		}
	}

	// 1.get dev enable status
	ret = RK_MPI_VI_GetDevIsEnable(devId);
	if (ret != RK_SUCCESS) {
		// 1-2.enable dev
		ret = RK_MPI_VI_EnableDev(devId);
		if (ret != RK_SUCCESS) {
			return -1;
		}
		// 1-3.bind dev/pipe
		stBindPipe.u32Num = pipeId;
		stBindPipe.PipeId[0] = pipeId;
		ret = RK_MPI_VI_SetDevBindPipe(devId, &stBindPipe);
		if (ret != RK_SUCCESS) {
			return -1;
		}
	}

	return 0;
}

int vi_chn_init(int channelId, int width, int height) {
	int ret;
	int buf_cnt = 1;

	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
	vi_chn_attr.u32Depth = 2;

	if (g_sEntityName != NULL)
		memcpy(vi_chn_attr.stIspOpt.aEntityName, g_sEntityName, strlen(g_sEntityName));
	ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);

	RK_S64 s64ViEnSta = TEST_COMM_GetNowUs();
	ret |= RK_MPI_VI_EnableChn(0, channelId);
	RK_S64 s64ViEnEnd = TEST_COMM_GetNowUs();
	printf("  vi en: %lld us\n", s64ViEnEnd - s64ViEnSta);

	if (ret) {
		printf("ERROR: create VI  %d, error! ret=%d\n", channelId, ret);
		return ret;
	}

	return ret;
}

static int read_cmdline_to_buf(void *buf, int len) {
	int fd;
	int ret;
	if (buf == NULL || len < 0) {
		printf("%s: illegal para\n", __func__);
		return -1;
	}
	memset(buf, 0, len);
	fd = open("/proc/cmdline", O_RDONLY);
	if (fd < 0) {
		perror("open:");
		return -1;
	}
	ret = read(fd, buf, len);
	close(fd);
	return ret;
}

long get_cmd_val(const char *string, int len) {
	char *addr;
	long value = 0;
	char key_equal[16];
	static char cmdline[1024];
	static char cmd_init = 0;

	if (cmd_init == 0) {
		cmd_init = 1;
		memset(cmdline, 0, sizeof(cmdline));
		read_cmdline_to_buf(cmdline, sizeof(cmdline));
	}

	snprintf(key_equal, sizeof(key_equal), "%s=", string);
	addr = strstr(cmdline, string);
	if (addr) {
		value = strtol(addr + strlen(string) + 1, NULL, len);
		printf("get %s value: 0x%0lx\n", string, value);
	}
	return value;
}

#if 1
void klog(const char *log) {
	FILE *fp = fopen("/dev/kmsg", "w");
	if (NULL != fp) {
		fprintf(fp, "[app-cli]: %s\n", log);
		fclose(fp);
	}
}
#else
void klog(const char *log) { return; }
#endif

static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -d 0.04 -n 0.2 -r 1.0 -r 1.0 -R 0.1 -D 0.3 -s 50\n", name);
	printf("\t-s | --switch_cnt: switch_cnts_th, Default 50\n");
	printf("\t-d | --d2n:   d2n_envL_th, Default 0.04f\n");
	printf("\t-n | --n2d:   n2d_envL_th, Default 0.2f\n");
	printf("\t-r | --rbase: rggain_base, Default 1.0f\n");
	printf("\t-r | --bbase: bggain_base, Default 1.0f\n");
	printf("\t-R | --rad:   awbgain_rad, Default 0.1f\n");
	printf("\t-D | --dis:   awbgain_dis, Default 0.3f\n");
}

int main(int argc, char *argv[]) {
	klog("main");
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 1920;
	RK_U32 u32Height = 1080;
	RK_CHAR *pOutPath = NULL;
	RK_CODEC_ID_E enCodecType = RK_VIDEO_ID_AVC;
	RK_S32 s32chnlId = 0;

	g_s32FrameCnt = RUN_TOTAL_CNT_MAX;
	pOutPath = "/tmp/venc-test.bin";

	sample_smartIr_t *smartIr_ctx = &g_sample_smartIr_ctx;
	float d2n = d2n_envL_th;
	float n2d = n2d_envL_th;
	float rbase = rggain_base;
	float bbase = bggain_base;
	float rad = awbgain_rad;
	float dis = awbgain_dis;
	int switch_cnt = 60;
	int c;
	if (argc > 1) {
		while ((c = getopt_long(argc, argv, optstr, long_options, NULL)) != -1) {
			const char *tmp_optarg = optarg;
			switch (c) {
			case 's':
				switch_cnt = atoi(optarg);
				break;
			case 'd':
				d2n = atof(optarg);
				break;
			case 'n':
				n2d = atof(optarg);
				break;
			case 'r':
				rbase = atof(optarg);
				break;
			case 'b':
				bbase = atof(optarg);
				break;
			case 'R':
				rad = atof(optarg);
				break;
			case 'D':
				dis = atof(optarg);
				break;
			case '?':
			default:
				print_usage(argv[0]);
				return 0;
			}
		}
	}
	printf(
	    "d2n_envL_th:%f, n2d_envL_th:%f, rggain_base:%f, bggain_base:%f, awbgain_rad:%f,"
	    "awbgain_dis:%f, switch_cnts_th:%d\n",
	    d2n, n2d, rbase, bbase, rad, dis, switch_cnt);
	if (d2n < 0 || n2d < 0 || rbase < 0 || bbase < 0 || rad < 0 || dis < 0 ||
	    switch_cnt < 0) {
		printf("invalid input param,please check!\n");
		return -1;
	}

	signal(SIGINT, sigterm_handler);
	RK_S64 s64VencInitStart = TEST_COMM_GetNowUs();
	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		printf("rockit init fail");
		goto __FAILED;
	}

	klog("SYS_Init");
	if (pOutPath) {
		venc0_file = fopen(pOutPath, "w");
		if (!venc0_file) {
			return 0;
		}
	}

#if (ENABLE_SMALL_STREAM)
	// venc init, if is fast boot, must first init venc.
	test_venc_init(1, 1280, 720, enCodecType); // RK_VIDEO_ID_AVC RK_VIDEO_ID_HEVC
	klog("venc chn1");
	test_jpeg_init(2, 640, 360, RK_VIDEO_ID_MJPEG); // RK_VIDEO_ID_AVC RK_VIDEO_ID_HEVC
	klog("venc chn2");
	// vi_dev_init(0, 0);
	vi_chn_init(1, 1280, 720);
	klog("vi ch1");
	vi_chn_init(2, 640, 360);
	klog("vi chn2");

	MPP_CHN_S stSrcChn, stDestChn;
	// bind vi to venc
	stSrcChn.enModId = RK_ID_VI;
	stSrcChn.s32DevId = 0;
	stSrcChn.s32ChnId = 1;

	stDestChn.enModId = RK_ID_VENC;
	stDestChn.s32DevId = 0;
	stDestChn.s32ChnId = 1;
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		goto __FAILED;
	}
	klog("bind chn 1");

	stSrcChn.s32ChnId = 2;
	stDestChn.s32ChnId = 2;
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		goto __FAILED;
	}
	klog("bind chn 2");

	pthread_t main_thread1;
	pthread_create(&main_thread1, NULL, GetMediaBuffer, 1);
	pthread_t main_thread2;
	pthread_create(&main_thread2, NULL, GetViBuffer, 2);
	pthread_t main_thread3;
	pthread_create(&main_thread3, NULL, GetMediaBuffer, 2);
#endif

#if (ENABLE_RKAIQ)
	int rk_color_mode, file_size, fd, ret = 0;
	void *mem, *vir_addr, *iq_mem, *vir_iqaddr;
	off_t rk_color_mode_addr, addr_iq;

	RK_S64 s64AiqInitStart = TEST_COMM_GetNowUs();

	rk_led_value = (int)get_cmd_val("rk_led_value", 0);
	int rk_night_mode = (int)get_cmd_val("rk_night_mode", 0);
	int cam_hdr = (int)get_cmd_val("rk_cam_hdr", 0);
	rk_aiq_working_mode_t hdr_mode =
	    (cam_hdr == 5) ? RK_AIQ_WORKING_MODE_ISP_HDR2 : RK_AIQ_WORKING_MODE_NORMAL;

	rk_color_mode_addr = (off_t)get_cmd_val("rk_color_mode", 16);
	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		perror("open error");
		return -1;
	}

	mem = mmap(0, MAP_SIZE_NIGHT, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
	           rk_color_mode_addr & ~MAP_MASK_NIGHT);
	vir_addr = mem + (rk_color_mode_addr & MAP_MASK_NIGHT);
	rk_color_mode = *((unsigned long *)vir_addr);

	addr_iq = (off_t)get_cmd_val("rk_iqbin_addr", 16);
	file_size = (int)get_cmd_val("rk_iqbin_size", 16);
	iq_mem =
	    mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr_iq & ~MAP_MASK);
	vir_iqaddr = iq_mem + (addr_iq & MAP_MASK);

	rk_aiq_sys_ctx_t *aiq_ctx;
	rk_aiq_static_info_t aiq_static_info;
	rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(s32chnlId, &aiq_static_info);

	if ((rk_night_mode == 2 && rk_color_mode) || rk_night_mode == 4) {
		printf("=====night mode=====\n");
		ret = rk_aiq_uapi2_sysctl_preInit_scene(aiq_static_info.sensor_info.sensor_name,
		                                        "normal", "night");
		if (ret < 0) {
			printf("%s: failed to set night scene\n",
			       aiq_static_info.sensor_info.sensor_name);
			return -1;
		}
	} else {
		printf("=====day mode=======\n");
		ret = rk_aiq_uapi2_sysctl_preInit_scene(aiq_static_info.sensor_info.sensor_name,
		                                        "normal", "day");
		if (ret < 0) {
			printf("%s: failed to set day scene\n",
			       aiq_static_info.sensor_info.sensor_name);
			return -1;
		}
	}

	ret = rk_aiq_uapi2_sysctl_preInit_iq_addr(aiq_static_info.sensor_info.sensor_name,
	                                          vir_iqaddr, file_size);
	if (ret < 0) {
		printf("%s: failed to load binary iqfiles\n",
		       aiq_static_info.sensor_info.sensor_name);
	}

	aiq_ctx = rk_aiq_uapi2_sysctl_init(aiq_static_info.sensor_info.sensor_name,
	                                   "/etc/iqfiles/", NULL, NULL);
	if (aiq_ctx == NULL) {
		printf("%s: failed to init aiq\n", aiq_static_info.sensor_info.sensor_name);
	}

	if (hdr_mode == RK_AIQ_WORKING_MODE_ISP_HDR2)
		klog("aiq in hdr mode");
	if (rk_aiq_uapi2_sysctl_prepare(aiq_ctx, 0, 0, hdr_mode)) {
		printf("rkaiq engine prepare failed !\n");
		return -1;
	}
	if (rk_aiq_uapi2_sysctl_start(aiq_ctx)) {
		printf("rk_aiq_uapi2_sysctl_start  failed\n");
		return -1;
	}
	klog("aiq start");
	if (rk_night_mode == 2) {
		smartIr_ctx->aiq_ctx = aiq_ctx;
		smartIr_ctx->ir_ctx = rk_smart_ir_init(aiq_ctx);
		smartIr_ctx->ir_ctx->state = rk_color_mode;
		load_ir_configs(d2n, n2d, rbase, bbase, rad, dis, switch_cnt);
		smartIr_ctx->tquit = false;
		pthread_create(&smartIr_ctx->tid, NULL, switch_ir_thread, NULL);
		smartIr_ctx->started = true;
	}

	RK_S64 s64AiqInitEnd = TEST_COMM_GetNowUs();
	printf("Aiq:%lld us\n", s64AiqInitEnd - s64AiqInitStart);
#endif

	GetMediaBuffer0(NULL);

#if (ENABLE_SMALL_STREAM)
	pthread_join(main_thread1, RK_NULL);
	pthread_join(main_thread2, RK_NULL);
	pthread_join(main_thread3, RK_NULL);
	s32Ret |= RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	s32Ret |= RK_MPI_VI_DisableChn(0, 1);
	s32Ret |= RK_MPI_VI_DisableChn(0, 2);

	s32Ret |= RK_MPI_VENC_StopRecvFrame(1);
	s32Ret |= RK_MPI_VENC_StopRecvFrame(2);
	if (s32Ret != RK_SUCCESS) {
		return s32Ret;
	}

	s32Ret = RK_MPI_VENC_DestroyChn(1);
	s32Ret = RK_MPI_VENC_DestroyChn(2);
	s32Ret = RK_MPI_VI_DisableDev(0);
#endif

#if (ENABLE_RTSP)
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
#endif

__FAILED:
#if (ENABLE_RKAIQ)
	if (fd > 0)
		close(fd);
	if (mem != MAP_FAILED)
		munmap(mem, MAP_SIZE_NIGHT);
	if (iq_mem != MAP_FAILED)
		munmap(iq_mem, file_size);
	if (rk_night_mode == 2) {
		rk_gpio_unexport(irled_enable_gpio);
		rk_gpio_unexport(ircut_on_gpio);
		rk_gpio_unexport(ircut_off_gpio);
		pthread_join(smartIr_ctx->tid, RK_NULL);
		sample_smartIr_stop();
	}
	rk_aiq_uapi2_sysctl_stop(aiq_ctx, false);
	rk_aiq_uapi2_sysctl_deinit(aiq_ctx);
#endif

	RK_MPI_SYS_Exit();
	return 0;
}
