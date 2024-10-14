#include <stdio.h>
#include <sys/poll.h>
#include <errno.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/poll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stddef.h>
#include <fcntl.h>

#include "rk_type.h"
#include "rk_debug.h"
#include "rk_defines.h"
#include "rtsp_demo.h"
#include "rk_pwm.h"

#include "rk_mpi_mb.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_vi.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_rgn.h"
#include "rk_mpi_avs.h"
#include "rk_comm_avs.h"
#include "rk_gpio.h"
#include "rk_meta.h"
#include "sensor_init_info.h"
#include "rk_meta_app_param.h"
#include "sensor_iq_info.h"
#include <rk_aiq_user_api2_camgroup.h>
#include <rk_aiq_user_api2_imgproc.h>
#include <rk_aiq_user_api2_sysctl.h>

#define CAM_NUM 2
#define VENC_CHN_NUM 1
#define MMAP_START_FROM_ZERO 0
#define SERVER_DEBUG_PRT     0
#define SENSOR 0
#define SECONDARY_SENSOR 1
#define ENABLE_RTSP 1
#define EPTZ_DEV_INDEX 1
#define OSD_FILE_PATH "/oem/usr/share/osd_image.data"

#define ISP_CLK_CORE_RATE "clk_core_isp3p2 420000000"
#define ISP_ACLK_RATE     "aclk_isp3p2 350000000"

// for argb8888
#define TEST_ARGB32_PIX_SIZE 4
#define TEST_ARGB32_RED 0xFF0000FF
#define TEST_ARGB32_GREEN 0x00FF00FF
#define TEST_ARGB32_BLUE 0x0000FFFF
#define TEST_ARGB32_TRANS 0x00000000
#define TEST_ARGB32_BLACK 0x000000FF

#define MMAP_SIZE (4096UL)          //MMAP_SIZE = 4K
#define MMAP_MASK (MMAP_SIZE - 1)   //MMAP_MASK = 0XFFF

#define SAVE_ENC_FRM_CNT_MAX 30
#define RUN_TOTAL_CNT_MAX 1000000

#if SERVER_DEBUG_PRT
#define DBG(fmt, args...)      printf("Server DBG: "fmt"", ##args)
#else
#define DBG(fmt, args...)
#endif
#define INFO(fmt, args...)      printf("Server INFO: "fmt"", ##args)
#define ERR(fmt, args...)       printf("Server ERR: "fmt"", ##args)

#define ALIGN_2(data) (data / 2 * 2)

static int32_t g_s32FrameCnt = -1;
static bool quit = false;
static int g_err_cnt = 0;
static bool g_should_quit = false;
static int g_bypass_cnt = 0;

#if (EPTZ_DEV_INDEX == 0)
static int g_eptz_dev = 0;
#else
static int g_eptz_dev = 1;
#endif

pthread_mutex_t g_rtsp_mutex = PTHREAD_MUTEX_INITIALIZER;
rtsp_demo_handle g_rtsplive = NULL;
static rtsp_session_handle g_rtsp_session_0;

static rk_aiq_sys_ctx_t *g_aiq_ctx[2];

static void sigterm_handler(int sig) { quit = true; }
void handle_pipe(int sig) { printf("%s sig = %d\n", __func__, sig); }

typedef struct _rkEptz {
	int32_t u32MultipleOfZoom;
	int32_t s32EptzViIndex;
	uint32_t u32ZoomStep;
	uint32_t u32ZoomSwitch;
	uint32_t u32ZoomLimit;
} VI_EPTZ_S;

typedef struct _rkMpiAVSCtx {
	uint32_t u32SrcWidth;
	uint32_t u32SrcHeight;
	AVS_CHN s32ChnId;
	AVS_MOD_PARAM_S stAvsModParam;
	AVS_GRP_ATTR_S stAvsGrpAttr;
	AVS_PIPE_ATTR_S stAvsPipeAttr[2];
	AVS_CHN_ATTR_S stAvsChnAttr[AVS_MAX_CHN_NUM];
} AVS_CTX_S;

typedef struct _rkMpiVICtx {
	uint32_t u32Width;
	uint32_t u32Height;
	VI_DEV s32DevId;
	VI_CHN s32ChnId;
	VI_EPTZ_S stEptz;
	VI_CROP_INFO_S stCropInfo;
	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	VI_CHN_ATTR_S stChnAttr;
} VI_CTX_S;

typedef struct _rkMpiVENCCtx {
	uint32_t u32Width;
	uint32_t u32Height;
	uint32_t u32Gop;
	uint32_t u32BitRate;
	uint32_t u32StreamBufCnt;
	uint32_t enCodecType;
	PIXEL_FORMAT_E enPixelFormat;
	VENC_CHN s32ChnId;
	VENC_CHN s32ChnNum;
	VENC_CHN_ATTR_S stChnAttr;
	VENC_RC_PARAM_S stRcParam;
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
} VENC_CTX_S;

typedef struct _rkMpiRGNCtx {
	uint32_t u32BgAlpha;
	uint32_t u32FgAlpha;
	uint32_t u32Color;
	uint32_t u32Layer;
	BITMAP_S stBitmap;
	MPP_CHN_S stMppChn;
	RGN_HANDLE rgnHandle;
	RGN_ATTR_S stRgnAttr;
	RGN_CHN_ATTR_S stRgnChnAttr;
} RGN_CTX_S;

typedef struct _FunctionCtx {
	uint32_t avs_splice_en;
	uint32_t avs_overlay_en;
	uint32_t cover_en;
	uint32_t osd_en;
	uint32_t eptz_en;
	uint32_t single_boot;
	uint32_t switch_resolution;
} Func_En_S;

typedef struct _rkMpiCtx {
	Func_En_S func;
	VI_CTX_S vi[2];
	AVS_CTX_S avs;
	VENC_CTX_S venc;
	RGN_CTX_S rgn[2];
} MPI_CTX_S;

typedef struct meta_info
{
	struct app_param_info *app_params;
	struct sensor_init_cfg *sensor_init[2];
} META_INFO;

static void set_argb8888_buffer(uint32_t *buf, uint32_t size, uint32_t color) {
	for (uint32_t i = 0; buf && (i < size); i++)
		*buf++ = color;
}

static XCamReturn SAMPLE_COMM_ISP_ErrCb(rk_aiq_err_msg_t* msg) {
	g_err_cnt++;
	if (g_err_cnt <= 2)
		printf("=== %u ===\n", msg->err_code);
	if (msg->err_code == XCAM_RETURN_BYPASS){
		g_bypass_cnt++;
		if(g_bypass_cnt == 2){
		g_should_quit = true;
			printf("zlj bypass cnt=%d\n", g_bypass_cnt);
		}
	}
}

static void rtsp_init()
{
	g_rtsplive = create_rtsp_demo(554);
	g_rtsp_session_0 = rtsp_new_session(g_rtsplive, "/live/0");
	rtsp_set_video(g_rtsp_session_0, RTSP_CODEC_ID_VIDEO_H264, NULL, 0);
	rtsp_sync_video_ts(g_rtsp_session_0, rtsp_get_reltime(), rtsp_get_ntptime());
}

static int cover_init(MPI_CTX_S *ctx)
{
	int32_t ret = 0;

	ret = RK_MPI_RGN_Create(ctx->rgn[0].rgnHandle, &ctx->rgn[0].stRgnAttr);
	if (0 != ret) {
		RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", ctx->rgn[0].rgnHandle, ret);
		RK_MPI_RGN_Destroy(ctx->rgn[0].rgnHandle);
		return ret;
	}
	ret = RK_MPI_RGN_AttachToChn(ctx->rgn[0].rgnHandle, &ctx->rgn[0].stMppChn, &ctx->rgn[0].stRgnChnAttr);
	if (0 != ret) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[0].rgnHandle, ret);
		return ret;
	}

	ret = RK_MPI_RGN_GetDisplayAttr(ctx->rgn[0].rgnHandle, &ctx->rgn[0].stMppChn, &ctx->rgn[0].stRgnChnAttr);
	if (0 != ret) {
		RK_LOGE("RK_MPI_RGN_GetDisplayAttr (%d)) failed with %#x!", ctx->rgn[0].rgnHandle,
		        ret);
		return ret;
	}

	ctx->rgn[0].stRgnChnAttr.bShow = RK_TRUE;
	ctx->rgn[0].stRgnChnAttr.enType = COVER_RGN;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X = 0;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 480;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 240;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 240;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.u32Color = 0xffff00;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.enCoordinate = RGN_ABS_COOR;
	ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.u32Layer = 1;
	RK_LOGE("cover region %d <%d, %d, %d, %d>, color<0x%x>",
	        ctx->rgn[0].rgnHandle, ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X,
	        ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y,
	        ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width,
	        ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height,
	        ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.u32Color);

	ret = RK_MPI_RGN_SetDisplayAttr(ctx->rgn[0].rgnHandle, &ctx->rgn[0].stMppChn, &ctx->rgn[0].stRgnChnAttr);
	if (0 != ret) {
		RK_LOGE("RK_MPI_RGN_SetDisplayAttr (%d)) failed with %#x!", ctx->rgn[0].rgnHandle,
		        ret);
		return ret;
	}

	return 0;
}

static int osd_init(MPI_CTX_S *ctx)
{
	int32_t ret = 0;

	ret = RK_MPI_RGN_Create(ctx->rgn[1].rgnHandle, &ctx->rgn[1].stRgnAttr);
	if (0 != ret) {
		RK_LOGE("RK_MPI_RGN_Create (%d) failed with %#x!", ctx->rgn[1].rgnHandle, ret);
		RK_MPI_RGN_Destroy(ctx->rgn[1].rgnHandle);
		return ret;
	}

	ret = RK_MPI_RGN_AttachToChn(ctx->rgn[1].rgnHandle, &ctx->rgn[1].stMppChn, &ctx->rgn[1].stRgnChnAttr);
	if (0 != ret) {
		RK_LOGE("RK_MPI_RGN_AttachToChn (%d) failed with %#x!", ctx->rgn[1].rgnHandle, ret);
		return ret;
	}
	RK_LOGI("Display region to chn success!");

	ret = RK_MPI_RGN_SetBitMap(ctx->rgn[1].rgnHandle, &ctx->rgn[1].stBitmap);
	if (ret != 0) {
		RK_LOGE("RK_MPI_RGN_SetBitMap failed with %#x!", ret);
		return ret;
	}

}

static void save_code_stream_file(int chn, VENC_STREAM_S stFrame)
{
	char OutPath[16];
	void *pData = RK_NULL;
	FILE *file = NULL;

	sprintf(OutPath, "/tmp/venc%d.bin", chn);
	file = fopen(OutPath, "a+");

	if (file) {
		pData = (void *)RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
		fwrite(pData, 1, stFrame.pstPack->u32Len, file);
		fflush(file);
	}

	if (file)
		fclose(file);
}

static void *GetVencStream(void *arg) {
	void *pData = RK_NULL;
	int ret;
	int chn = 0, errCnt = 0, loopCount = 0;
	VENC_STREAM_S stFrame;

	MPI_CTX_S *ctx = (MPI_CTX_S *)arg;

	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (!quit) {
		ret = RK_MPI_VENC_GetStream(chn, &stFrame, 1000);
		if (ret == 0) {
			if (ctx->func.single_boot) {
				ret = RK_MPI_VI_DisableChn(1, 0);
				if (ret != 0) {
					RK_LOGE("RK_MPI_VI_DisableChn failure:%#X  Pipe:0 ChnId:\n", ret);
					return (int *)ret;
				}
				ctx->func.single_boot = 0;
			}

			if (ctx->func.eptz_en) {
				/* eptz handle */
				int32_t ret = 0;
				uint32_t u32SrcWidth = ctx->vi[g_eptz_dev].stChnAttr.stSize.u32Width * 100;
				uint32_t u32SrcHeight = ctx->vi[g_eptz_dev].stChnAttr.stSize.u32Height * 100;
				ret = RK_MPI_VI_GetEptz(g_eptz_dev, 0, &ctx->vi[g_eptz_dev].stCropInfo);
				if (ret != 0) {
					RK_LOGE("RK_MPI_VI_GetEptz failure:%#X  Pipe:%d ChnId:0\n", ret, g_eptz_dev);
					return (int *)ret;
				}

				ctx->vi[g_eptz_dev].stCropInfo.stCropRect.u32Width = ALIGN_2(u32SrcWidth / ctx->vi[g_eptz_dev].stEptz.u32MultipleOfZoom);
				ctx->vi[g_eptz_dev].stCropInfo.stCropRect.u32Height = ALIGN_2(u32SrcHeight / ctx->vi[g_eptz_dev].stEptz.u32MultipleOfZoom);
				ctx->vi[g_eptz_dev].stCropInfo.stCropRect.s32X = ALIGN_2((ctx->vi[g_eptz_dev].stChnAttr.stSize.u32Width - ctx->vi[g_eptz_dev].stCropInfo.stCropRect.u32Width) / 2);
				ctx->vi[g_eptz_dev].stCropInfo.stCropRect.s32Y = ALIGN_2((ctx->vi[g_eptz_dev].stChnAttr.stSize.u32Height - ctx->vi[g_eptz_dev].stCropInfo.stCropRect.u32Height) / 2);
				ret = RK_MPI_VI_SetEptz(g_eptz_dev, 0, ctx->vi[g_eptz_dev].stCropInfo);
				if (ret != 0) {
					RK_LOGE("RK_MPI_VI_SetEptz failure:%#X  pipe:%d chnid:0", ret, g_eptz_dev);
					return (int *)ret;
				}
				ctx->vi[g_eptz_dev].stEptz.u32MultipleOfZoom += ctx->vi[g_eptz_dev].stEptz.u32ZoomStep;
				if (ctx->vi[g_eptz_dev].stEptz.u32MultipleOfZoom > ctx->vi[g_eptz_dev].stEptz.u32ZoomLimit) {
					ctx->vi[g_eptz_dev].stEptz.u32MultipleOfZoom = 100;
				}

				if (ctx->vi[g_eptz_dev].stEptz.u32MultipleOfZoom > ctx->vi[g_eptz_dev].stEptz.u32ZoomSwitch) {
					ctx->vi[g_eptz_dev].stEptz.s32EptzViIndex = 1;
				} else {
					ctx->vi[g_eptz_dev].stEptz.s32EptzViIndex = 0;
				}
			}

			if (loopCount <= SAVE_ENC_FRM_CNT_MAX) {
				printf("chn:%d, loopCount:%d enc->seq:%d, pkt_size=%lu, pts=%llu\n", chn, loopCount,
				       stFrame.u32Seq, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS);
				save_code_stream_file(chn, stFrame);
			}
#if (ENABLE_RTSP)
			// tx video to rtspls
			if (loopCount > SAVE_ENC_FRM_CNT_MAX) {
				if (g_rtsplive && g_rtsp_session_0 && chn == 0) {
					pData = (void *)RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
					rtsp_tx_video(g_rtsp_session_0, pData, stFrame.pstPack->u32Len,
						      stFrame.pstPack->u64PTS);
					rtsp_do_event(g_rtsplive);
				}
			}
#endif
			ret = RK_MPI_VENC_ReleaseStream(chn, &stFrame);
			if (ret != 0) {
				ERR("venc release chn %d failed\n", chn);
				goto __FAILED;
			}
			errCnt = 0;
			loopCount++;
		} else {
			if (errCnt < 10) {
				ERR("RK_MPI_VENC_GetChnFrame fail %x\n", ret);
			}
			errCnt++;
		}
	}

__FAILED:
	free(stFrame.pstPack);
	return NULL;
}

void klog(const char *log) {
    FILE *fp = fopen("/dev/kmsg", "w");
    if (NULL != fp) {
        fprintf(fp, "[app]: %s", log);
        fclose(fp);
    }
}

//TODO
RK_U64 TEST_COMM_GetNowUs() {
    struct timespec time = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

/*
 *  * get cmdline from /proc/cmdline
 *  */
static int read_cmdline_to_buf(void *buf, int len)
{
	int fd;
	int ret;
	if(buf == NULL || len < 0){
		printf("%s: illegal para\n", __func__);
		return -1;
	}
	memset(buf, 0, len);
	fd = open("/proc/cmdline", O_RDONLY);
	if(fd < 0){
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
		INFO("get %s value: 0x%0lx\n", string, value);
	}
	return value;
}

static void* mmap_memory_to_viraddr(off_t phy_addr, size_t size)
{
	void *vir_addr, *vir_addr_align_4k;
	int mem_fd, vir_addr_offset;

	if((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0)
	{
		perror ("Open dev/mem Error:");
		return NULL;
	}

	vir_addr_align_4k = mmap(MMAP_START_FROM_ZERO, size, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phy_addr);
	vir_addr_offset = phy_addr & MMAP_MASK;
	vir_addr = vir_addr_align_4k + vir_addr_offset;

	close(mem_fd);
	return vir_addr;
}

__attribute__((unused)) static int dump_virmem_hex(char *buf_dump_addr, const char *msg)
{
	printf("\ndump [%s]\n", msg);
	char *pdata = (char *)(buf_dump_addr);
	for (char j=0; j<64; j++ ) {
		printf("%02x ", *(pdata+j) & 0xff);
		if ((j+1)%16 == 0) {
			printf("\n");
		}
	}
	printf("\n");
	return 0;
}

__attribute__((unused)) static void dump_meta_sensor_app_params(struct meta_info *handle) {
	int camId = 0;
	for(; camId < CAM_NUM; camId++) {
		printf("\n");
		printf("sensor[%d] init info:\n", camId);
		printf("cam_w: %d, cam_h: %d\n",
			handle->sensor_init[camId]->cam_w,
			handle->sensor_init[camId]->cam_h);

		printf("app[%d] params info:\n", camId);
		printf("venc_w: %d, venc_h: %d, venc_type: %d, venc_bitrate: %d, fps: %d, cam_mirror_flip: %d, night_mode: %d, color_mode: %d\n",
			handle->app_params->venc_w,
			handle->app_params->venc_h,
			handle->app_params->venc_type,
			handle->app_params->venc_bitrate,
			handle->app_params->cam_fps,
			handle->app_params->cam_mirror_flip,
			handle->app_params->night_mode,
			handle->app_params->color_mode);
	}
	printf("\n");
}

static void* get_meta_params(struct meta_info *handle) {
	int app_param_offset, meta_size;
	off_t metaAddr;
	void *metaVirmem = NULL, *appVirAddr = NULL, *SensorInitVirAddr;
	struct app_param_info *AppParam = NULL;
	struct sensor_init_cfg *SensorInitParam = NULL;

	meta_size = (int)get_cmd_val("meta_part_size", 16);
	metaAddr = (off_t)get_cmd_val("meta_load_addr", 16);

	metaVirmem = mmap_memory_to_viraddr(metaAddr, (size_t)meta_size);
	if (metaVirmem != MAP_FAILED) {
		SensorInitVirAddr = metaVirmem + SENSOR_INIT_OFFSET;
		handle->sensor_init[SENSOR] = (struct sensor_init_cfg*)(SensorInitVirAddr);

		SensorInitVirAddr = metaVirmem + SECONDARY_SENSOR_INIT_OFFSET;
		handle->sensor_init[SECONDARY_SENSOR] = (struct sensor_init_cfg*)(SensorInitVirAddr);

		app_param_offset = (int)get_cmd_val(RK_APP_PARAM_OFFSET, 16);
		appVirAddr = metaVirmem + app_param_offset;
		handle->app_params = (struct app_param_info *)(appVirAddr);

		return metaVirmem;
	} else {
		ERR("meta addr mmap fail.\n");
		return metaVirmem;
	}
}

static int vi_init(MPI_CTX_S *ctx)
{
	int ret, devId;
	for (devId = 0; devId < CAM_NUM; devId++) {
		memset(&ctx->vi[devId].stDevAttr, 0, sizeof(ctx->vi[devId].stDevAttr));
		memset(&ctx->vi[devId].stBindPipe, 0, sizeof(ctx->vi[devId].stBindPipe));

		ret = RK_MPI_VI_GetDevAttr(devId, &ctx->vi[devId].stDevAttr);
		if (ret == RK_ERR_VI_NOT_CONFIG) {
			ret = RK_MPI_VI_SetDevAttr(devId, &ctx->vi[devId].stDevAttr);
			if (ret != 0)
				ERR("VI dev %d set attr failed ret: %d\n", devId, ret);
		} else {
			ERR("VI dev %d has been configed\n", devId);
		}

		ret = RK_MPI_VI_GetDevIsEnable(devId);
		if (ret != 0) {
			ret = RK_MPI_VI_EnableDev(devId);
			if (ret != 0)
				ERR("VI dev %d enable failed ret: %d\n", devId, ret);
			ctx->vi[devId].stBindPipe.u32Num = devId;
			ctx->vi[devId].stBindPipe.PipeId[0] = devId;
			ret = RK_MPI_VI_SetDevBindPipe(devId, &ctx->vi[devId].stBindPipe);
			if (ret != 0)
				ERR("VI dev %d set bind pipe failed ret: %d\n", devId, ret);
		} else {
			ERR("VI dev %d has been enabled\n", devId);
		}

		ret = RK_MPI_VI_SetChnAttr(devId, 0, &ctx->vi[devId].stChnAttr);
		if (ret) {
			ERR("VI dev %d set chn 0  attr error! ret=%d\n", devId, ret);
		}
		ret = RK_MPI_VI_EnableChn(devId, 0);
		if (ret) {
			ERR("create VI dev %d chn 0 error! ret=%d\n", devId, ret);
		}
	}
	return 0;
}

static int avs_init(MPI_CTX_S *ctx)
{
	int AvsGrp = 0, AvsChn = 0, ret, AvsPipe;

	ret = RK_MPI_AVS_SetModParam(&ctx->avs.stAvsModParam);
	if (0 != ret) {
		printf("RK_MPI_AVS_SetModParam failed, ret is %#x\n", ret);
		return ret;
	}

	ret = RK_MPI_AVS_CreateGrp(AvsGrp, &ctx->avs.stAvsGrpAttr);
	if (0 != ret) {
		printf("RK_MPI_AVS_CreateGrp failed, ret is %#x\n", ret);
		return ret;
	}

	if (ctx->avs.stAvsGrpAttr.enMode == AVS_MODE_NOBLEND_OVL) {
		for (AvsPipe = 0; AvsPipe < 2; AvsPipe++) {//todo pipe_num
			ret = RK_MPI_AVS_SetPipeAttr(AvsGrp, AvsPipe, &ctx->avs.stAvsPipeAttr[AvsPipe]);
			if (ret != 0) {
				RK_LOGE("avs [%d, %d] RK_MPI_AVS_SetPipeAttr failed: %#x!",
					AvsGrp, AvsPipe, ret);
				return ret;
			}
			RK_LOGV("avs [%d, %d] RK_MPI_AVS_SetPipeAttr already.", AvsGrp, AvsPipe);
		}
	}

	ret = RK_MPI_AVS_SetChnAttr(AvsGrp, AvsChn, &ctx->avs.stAvsChnAttr[0]);
	if (0 != ret) {
		printf("RK_MPI_AVS_SetChnAttr failed, ret is %#x\n", ret);
		return ret;
	}
	ret = RK_MPI_AVS_EnableChn(AvsGrp, AvsChn);
	if (0 != ret) {
		printf("RK_MPI_AVS_EnableChn failed, ret is %#x\n", ret);
		return ret;
	}

	ret = RK_MPI_AVS_StartGrp(AvsGrp);
	if (0 != ret) {
		printf("RK_MPI_AVS_StartGrp failed, ret is %#x\n", ret);
		return ret;
	}
}

static int venc_init(MPI_CTX_S *ctx)
{
	int32_t ret = 0;

	for (ctx->venc.s32ChnId = 0; ctx->venc.s32ChnId < VENC_CHN_NUM; ctx->venc.s32ChnId++) {
		ret = RK_MPI_VENC_CreateChn(ctx->venc.s32ChnId, &ctx->venc.stChnAttr);
		if (ret != 0) {
			RK_LOGE("venc [%d] RK_MPI_VENC_CreateChn failed: %#x!",
			ctx->venc.s32ChnId, ret);
			return ret;
		}

		ret = RK_MPI_VENC_SetChnRefBufShareAttr(ctx->venc.s32ChnId, &ctx->venc.stVencChnRefBufShare);
		if (ret != 0) {
			RK_LOGE("venc [%d] RK_MPI_VENC_SetChnRefBufShareAttr failed: %#x!",
			ctx->venc.s32ChnId, ret);
			return ret;
		}

		ret = RK_MPI_VENC_SetRcParam(ctx->venc.s32ChnId, &ctx->venc.stRcParam);
		if (ret != 0) {
			RK_LOGE("venc [%d] RK_MPI_VENC_SetRcParam failed: %#x!",
			ctx->venc.s32ChnId, ret);
			return ret;
		}

		ret = RK_MPI_VENC_EnableSvc(ctx->venc.s32ChnId, RK_TRUE);
		if (ret != 0) {
			RK_LOGE("venc [%d] RK_MPI_VENC_EnableSvc failed: %#x!",
			ctx->venc.s32ChnId, ret);
			return ret;
		}

		ret = RK_MPI_VENC_StartRecvFrame(ctx->venc.s32ChnId, &ctx->venc.stRecvParam);
		if (ret != 0) {
			RK_LOGE("venc [%d] RK_MPI_VENC_StartRecvFrame failed: %#x!",
			ctx->venc.s32ChnId, ret);
			return ret;
		}
	}

	return ret;
}
static int32_t vi_bind_venc()
{
	int32_t ret;
	MPP_CHN_S stSrcChn, stDestChn;

	memset(&stSrcChn, 0, sizeof(stSrcChn));
	memset(&stDestChn, 0, sizeof(stDestChn));
	stSrcChn.enModId    = RK_ID_VI;
	stSrcChn.s32DevId   = 0;
	stSrcChn.s32ChnId   = 0;

	stDestChn.enModId   = RK_ID_VENC;
	stDestChn.s32DevId  = 0;
	stDestChn.s32ChnId  = 0;

	ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (ret != 0)
		ERR("VI dev 0 chn 0 bind VENC dev 0 chn 0 failed, ret: %d\n", ret);

	return ret;
}

static int32_t vi_bind_avs()
{
	int PipeId = 0;
	int32_t ret;
	MPP_CHN_S stSrcChn, stDestChn;

	for (; PipeId < CAM_NUM; PipeId++) {
		memset(&stSrcChn, 0, sizeof(stSrcChn));
		memset(&stDestChn, 0, sizeof(stDestChn));
		stSrcChn.enModId    = RK_ID_VI;
		stSrcChn.s32DevId   = PipeId;
		stSrcChn.s32ChnId   = 0;

		stDestChn.enModId   = RK_ID_AVS;
		stDestChn.s32DevId  = 0;
		stDestChn.s32ChnId  = PipeId;

		ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
		if (ret != 0)
			ERR("VI dev %d chn 0 bind AVS dev 0 chn %d failed, ret: %d\n", PipeId, PipeId, ret);
	}

	return ret;
}

static int32_t avs_bind_venc()
{
	int pipeId = 0;
	int32_t ret;
	MPP_CHN_S stSrcChn, stDestChn;

	for (; pipeId < 1; pipeId++) {
		memset(&stSrcChn, 0, sizeof(stSrcChn));
		memset(&stDestChn, 0, sizeof(stDestChn));
		stSrcChn.enModId    = RK_ID_AVS;
		stSrcChn.s32DevId   = 0;
		stSrcChn.s32ChnId   = 0;

		stDestChn.enModId   = RK_ID_VENC;
		stDestChn.s32DevId  = 0;
		stDestChn.s32ChnId  = 0;

		ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
		if (ret != 0)
			ERR("AVS dev 0 chn 0 bind VENC dev 0 chn 0 failed, ret: %d\n", ret);
	}

	return ret;
}

static int32_t vi_unbind_venc()
{
	int32_t ret;
	MPP_CHN_S stSrcChn, stDestChn;

	memset(&stSrcChn, 0, sizeof(stSrcChn));
	memset(&stDestChn, 0, sizeof(stDestChn));
	stSrcChn.enModId    = RK_ID_VI;
	stSrcChn.s32DevId   = 0;
	stSrcChn.s32ChnId   = 0;

	stDestChn.enModId   = RK_ID_VENC;
	stDestChn.s32DevId  = 0;
	stDestChn.s32ChnId  = 0;

	ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
	if (ret != 0)
		ERR("VI dev 0 chn 0 unbind VENC dev 0 chn 0 failed, ret: %d\n", ret);

	return ret;
}

static int32_t vi_unbind_avs()
{
	int PipeId = 0;
	int32_t ret;
	MPP_CHN_S stSrcChn, stDestChn;

	for (; PipeId < CAM_NUM; PipeId++) {
		memset(&stSrcChn, 0, sizeof(stSrcChn));
		memset(&stDestChn, 0, sizeof(stDestChn));
		stSrcChn.enModId    = RK_ID_VI;
		stSrcChn.s32DevId   = PipeId;
		stSrcChn.s32ChnId   = 0;

		stDestChn.enModId   = RK_ID_AVS;
		stDestChn.s32DevId  = 0;
		stDestChn.s32ChnId  = PipeId;

		ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
		if (ret != 0)
			ERR("VI dev %d chn 0 unbind AVS dev 0 chn %d failed, ret: %d\n", PipeId, PipeId, ret);
	}

	return ret;
}

static int32_t avs_unbind_venc()
{
	int pipeId = 0;
	int32_t ret;
	MPP_CHN_S stSrcChn, stDestChn;

	for (; pipeId < 1; pipeId++) {
		memset(&stSrcChn, 0, sizeof(stSrcChn));
		memset(&stDestChn, 0, sizeof(stDestChn));
		stSrcChn.enModId    = RK_ID_AVS;
		stSrcChn.s32DevId   = 0;
		stSrcChn.s32ChnId   = 0;

		stDestChn.enModId   = RK_ID_VENC;
		stDestChn.s32DevId  = 0;
		stDestChn.s32ChnId  = 0;

		ret = RK_MPI_SYS_UnBind(&stSrcChn, &stDestChn);
		if (ret != 0)
			ERR("AVS dev 0 chn 0 unbind VENC dev 0 chn 0 failed, ret: %d\n", ret);
	}

	return ret;
}

static int32_t cover_osd_deinit(bool osd_en, bool cover_en, MPI_CTX_S *ctx)
{
	int32_t ret = 0, rgnHandle_num = 1;
	RGN_HANDLE rgnHandle;

	if (osd_en && cover_en) {
		rgnHandle_num = 2;
	} else if (!(osd_en || cover_en)) {
		rgnHandle_num = 0;
		RK_LOGE("Osd and Cover has not been inited\n");
		return 0;
	} else {
		rgnHandle_num = 1;
	}

	/* cover: rgn[0], osd: rgn[1] */
	for (rgnHandle = 0; rgnHandle < rgnHandle_num; rgnHandle++) {
		if (osd_en && !cover_en)
			rgnHandle = 1;
		ret = RK_MPI_RGN_DetachFromChn(rgnHandle, &ctx->rgn[rgnHandle].stMppChn);
		if (0 != ret) {
			RK_LOGE("RK_MPI_RGN_DetachFrmChn (%d) failed with %#x!", rgnHandle, ret);
			return ret;
		}

		ret = RK_MPI_RGN_Destroy(rgnHandle);
		if (0 != ret) {
			RK_LOGE("RK_MPI_RGN_Destroy [%d] failed with %#x", rgnHandle, ret);
			return ret;
		}
	}
}

static int32_t avs_deinit()
{
	int32_t AvsGrp = 0, ret;

	ret = RK_MPI_AVS_StopGrp(AvsGrp);
	if (ret != 0) {
		RK_LOGE("avs [%d, %d] RK_MPI_AVS_StopGrp failed: %#x!!",
			AvsGrp, 0, ret);
		return ret;
	}

	RK_LOGV("avs [%d, %d] RK_MPI_AVS_StopGrp already.", AvsGrp, 0);
	ret = RK_MPI_AVS_DestroyGrp(AvsGrp);
	if (ret != 0) {
		RK_LOGE("avs [%d, %d] RK_MPI_AVS_DestroyGrp failed: %#x!!",
			AvsGrp, 0, ret);
		return ret;
	}
	RK_LOGV("avs [%d, %d] RK_MPI_AVS_DestroyGrp already.", AvsGrp, 0);

	return 0;
}

static int32_t venc_deinit()
{
	int ChnId = 0, ret = 0;

	ret = RK_MPI_VENC_StopRecvFrame(ChnId);
	if (ret != 0) {
		RK_LOGE("venc %d RK_MPI_VENC_StopRecvFrame failed: %#x!!",
			ChnId, ret);
		return ret;
	}
	ret = RK_MPI_VENC_DestroyChn(ChnId);
	if (ret != 0) {
		RK_LOGE("venc %d RK_MPI_VENC_DestroyChn failed: %#x!!",
			ChnId, ret);
		return ret;
	}
}

static int32_t vi_deinit()
{
	int ret, DevId, ChnId = 0;
	for (DevId = 0; DevId < CAM_NUM; DevId++) {
		ret = RK_MPI_VI_DisableChn(DevId, ChnId);
		if (ret != 0) {
			RK_LOGE("vi dev %d chn %d RK_MPI_VI_DisableChn failed: %#x!!",
				DevId, ChnId, ret);
			return ret;
		}
		ret = RK_MPI_VI_DisableDev(DevId);
		if (ret != 0) {
			RK_LOGE("vi dev %d chn %d RK_MPI_VI_DisableDev failed: %#x!!",
				DevId, ChnId, ret);
			return ret;
		}
	}
}

static int aiq_deinit()
{
	int CamId;
	for (CamId = 0; CamId < CAM_NUM; CamId++) {
		rk_aiq_uapi2_sysctl_stop(g_aiq_ctx[CamId], false);
		rk_aiq_uapi2_sysctl_deinit(g_aiq_ctx[CamId]);
	}
}

static int aiq_init_start(struct meta_info *handle, void *metaVirmem, int is_second_init, MPI_CTX_S *ctx)
{
	int cam_hdr, camId, file_size, file_size_secondary, ret;
	void *vir_iqaddr, *vir_iqaddr_scondary, *appVirAddr;
	rk_aiq_working_mode_t hdr_mode;
	rk_aiq_static_info_t aiq_static_info;
	char *sensor_name;

	file_size = (int)get_cmd_val("rk_iqbin_size", 16);
	file_size_secondary = (int)get_cmd_val("rk_iqbin_secondary_size", 16);

	vir_iqaddr = metaVirmem + SENSOR_IQ_BIN_OFFSET + offsetof(struct sensor_iq_info, data);
	vir_iqaddr_scondary = metaVirmem + SENSOR_IQ_BIN_OFFSET + SENSOR_IQ_BIN_MAX_SIZE;

#if SERVER_DEBUG_PRT
	dump_virmem_hex(vir_iqaddr, "main_aiq");
	dump_virmem_hex(vir_iqaddr_scondary, "secondary_aiq");
#endif

	cam_hdr = (int)get_cmd_val("rk_cam_hdr", 0);
	hdr_mode = (cam_hdr == 5) ? RK_AIQ_WORKING_MODE_ISP_HDR2 : RK_AIQ_WORKING_MODE_NORMAL;

	for (camId = 0; camId < CAM_NUM; camId++) {
		rk_aiq_uapi2_sysctl_enumStaticMetasByPhyId(camId, &aiq_static_info);
		sensor_name = aiq_static_info.sensor_info.sensor_name;
		printf("sensor name: %s\n", sensor_name);

		if (handle->app_params[0].color_mode) {
			ret = rk_aiq_uapi2_sysctl_preInit_scene(sensor_name, "normal", "night");
			printf("------night mode------\n");
		} else {
			ret = rk_aiq_uapi2_sysctl_preInit_scene(sensor_name, "normal", "day");
			printf("------day mode------\n");
		}
		if (ret < 0)
			ERR("%s: failed to set night scene\n", sensor_name);
		klog("preinit scene\n");

		if (camId == 0)
			ret = rk_aiq_uapi2_sysctl_preInit_iq_addr(sensor_name, vir_iqaddr, file_size);
		else
			ret = rk_aiq_uapi2_sysctl_preInit_iq_addr(sensor_name, vir_iqaddr_scondary, file_size_secondary);
		if (ret < 0)
			ERR("%s: failed to load binary iqfiles\n", sensor_name);
		klog("preinit iq addr\n");

		rk_aiq_uapi2_sysctl_preInit_devBufCnt(sensor_name, "rkraw_rx", 2);

		rk_aiq_tb_info_t tb_info;

		if (is_second_init) {
			memset(&tb_info, 0, sizeof(tb_info));
			tb_info.magic = sizeof(rk_aiq_tb_info_t) - 2;
			tb_info.is_pre_aiq = false;
			tb_info.prd_type = RK_AIQ_PRD_TYPE_TB_BATIPC;
			rk_aiq_uapi2_sysctl_preInit_tb_info(sensor_name, &tb_info);
			klog("preinit tb info\n");

			g_aiq_ctx[camId] = rk_aiq_uapi2_sysctl_init(sensor_name,
									  "/etc/iqfiles/", NULL, NULL);
		} else {
			memset(&tb_info, 0, sizeof(tb_info));
			tb_info.magic = sizeof(rk_aiq_tb_info_t) - 2;
			tb_info.is_pre_aiq = true;
			tb_info.prd_type = RK_AIQ_PRD_TYPE_TB_BATIPC;
			rk_aiq_uapi2_sysctl_preInit_tb_info(sensor_name, &tb_info);
			klog("preinit tb info\n");

			g_aiq_ctx[camId] = rk_aiq_uapi2_sysctl_init(sensor_name,
									  "/etc/iqfiles/", SAMPLE_COMM_ISP_ErrCb, NULL);
		}

		klog("aiq init\n");
		if (g_aiq_ctx[camId] == NULL)
			ERR("%s: failed to init aiq\n", sensor_name);

		if (rk_aiq_uapi2_sysctl_prepare(g_aiq_ctx[camId], 0, 0, hdr_mode))
			ERR("rkaiq engine prepare failed !\n");
		klog("aiq prepare\n");

		if (rk_aiq_uapi2_sysctl_start(g_aiq_ctx[camId]))
			ERR("rk_aiq_uapi2_sysctl_start  failed\n");
		klog("aiq start\n");
	}
	return 0;
}

int32_t load_file_osdmem(const RK_CHAR *filename, uint8_t *pu8Virt, uint32_t u32Width,
                        uint32_t u32Height, uint32_t pixel_size, uint32_t shift_value) {
	uint32_t mem_len = u32Width;
	uint32_t read_len = mem_len * pixel_size >> shift_value;
	uint32_t read_height;
	FILE *file = NULL;

	file = fopen(filename, "rb");
	if (file == NULL) {
		RK_LOGE("open filename: %s file failed!", filename);
		return -1;
	}
	for (read_height = 0; read_height < u32Height; read_height++) {
		fread((pu8Virt + (u32Width * read_height * pixel_size >> shift_value)), 1,
		      read_len, file);
	}
	fclose(file);
	return 0;
}

static int param_init(MPI_CTX_S *ctx, struct meta_info *handle)
{
	uint32_t fps = 0, gop = 0, enType = RK_VIDEO_ID_HEVC;
	uint32_t i, enWorkMode, ret;

	ctx->func.eptz_en = 1;
	ctx->func.cover_en = 1;
	ctx->func.avs_overlay_en = 0;
	ctx->func.avs_splice_en = 1;
	ctx->func.single_boot = 0;
	ctx->func.switch_resolution = 1;
	ctx->func.osd_en = 0;

	if (ctx->func.avs_overlay_en)
		enWorkMode = AVS_MODE_NOBLEND_OVL;
	else if (ctx->func.avs_splice_en)
		enWorkMode = AVS_MODE_NOBLEND_VER;

	ret = access(OSD_FILE_PATH, 0);
	if (ret == 0) {
		ctx->func.osd_en = 1;
	} else {
		ctx->func.osd_en = 0;
	}

	/* VI parameters init */
	for (i = 0; i < CAM_NUM; i++) {
		ctx->vi[i].stChnAttr.stSize.u32Width = handle->sensor_init[i]->cam_w;
		ctx->vi[i].stChnAttr.stSize.u32Height = handle->sensor_init[i]->cam_h;
		ctx->vi[i].stChnAttr.stIspOpt.u32BufCount = 3;
		ctx->vi[i].stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
		ctx->vi[i].stChnAttr.stIspOpt.stMaxSize.u32Width = handle->sensor_init[i]->cam_w;
		ctx->vi[i].stChnAttr.stIspOpt.stMaxSize.u32Height = handle->sensor_init[i]->cam_h;
		ctx->vi[i].stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
		ctx->vi[i].stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
		ctx->vi[i].stChnAttr.u32Depth = 0;
		ctx->vi[i].stChnAttr.stFrameRate.s32SrcFrameRate = -1;
		ctx->vi[i].stChnAttr.stFrameRate.s32DstFrameRate = -1;
	}
	if (ctx->func.eptz_en) {
		ctx->vi[g_eptz_dev].stEptz.u32MultipleOfZoom = 100;
		ctx->vi[g_eptz_dev].stEptz.s32EptzViIndex = 1;
		ctx->vi[g_eptz_dev].stEptz.u32ZoomStep = 1;
		ctx->vi[g_eptz_dev].stEptz.u32ZoomSwitch = 270;
		ctx->vi[g_eptz_dev].stEptz.u32ZoomLimit = 600;
	}
	/* VI parameters init END */

	/* COVER parameters init */
	/* Init RGN[0] */
	if (ctx->func.cover_en) {
		ctx->rgn[0].rgnHandle = 0;
		ctx->rgn[0].stRgnAttr.enType = COVER_RGN;
		ctx->rgn[0].stMppChn.enModId = RK_ID_VI;
		ctx->rgn[0].stMppChn.s32ChnId = 0;
		ctx->rgn[0].stMppChn.s32DevId = 0;

		ctx->rgn[0].stRgnChnAttr.bShow = RK_TRUE;
		ctx->rgn[0].stRgnChnAttr.enType = COVER_RGN;
		ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X = 0;
		ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y = 480;
		ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width = 640;
		ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height = 240;
		ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.u32Color = 0x00ff00;
		ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.u32Layer = 1;
		ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.enCoordinate = RGN_ABS_COOR;
		RK_LOGE("cover region %d <%d, %d, %d, %d>, color<0x%x>",
		        ctx->rgn[0].rgnHandle, ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32X,
		        ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.s32Y,
		        ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Width,
		        ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.stRect.u32Height,
		        ctx->rgn[0].stRgnChnAttr.unChnAttr.stCoverChn.u32Color);
	}
	/* COVER parameters init END */

	/* OSD OVERLAY parameters init */
	if (ctx->func.osd_en) {
		RK_CHAR *filename = OSD_FILE_PATH;
		int u32Width = 128;
		int u32Height = 128;
		int s32X = 0;
		int s32Y = 0;

		ctx->rgn[1].rgnHandle = 1;
		ctx->rgn[1].stMppChn.enModId = RK_ID_VENC;
		ctx->rgn[1].stMppChn.s32DevId = 0;
		ctx->rgn[1].stMppChn.s32ChnId = 0;

		/****************************************
		step 1: create overlay regions
		****************************************/
		ctx->rgn[1].stRgnAttr.enType = OVERLAY_RGN;
		ctx->rgn[1].stRgnAttr.unAttr.stOverlay.enPixelFmt = (PIXEL_FORMAT_E)RK_FMT_ARGB8888;
		ctx->rgn[1].stRgnAttr.unAttr.stOverlay.stSize.u32Width = u32Width;
		ctx->rgn[1].stRgnAttr.unAttr.stOverlay.stSize.u32Height = u32Height;
		ctx->rgn[1].stRgnAttr.unAttr.stOverlay.u32ClutNum = 0;

		/*********************************************
		step 2: display overlay regions to groups
		*********************************************/
		memset(&ctx->rgn[1].stRgnChnAttr, 0, sizeof(ctx->rgn[1].stRgnChnAttr));
		ctx->rgn[1].stRgnChnAttr.bShow = RK_TRUE;
		ctx->rgn[1].stRgnChnAttr.enType = OVERLAY_RGN;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32X = s32X;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stPoint.s32Y = s32Y;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.u32BgAlpha = 0;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.u32FgAlpha = 0;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.u32Layer = 0;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bEnable = RK_FALSE;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bForceIntra = RK_TRUE;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.bAbsQp = RK_FALSE;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stQpInfo.s32Qp = RK_FALSE;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[0] = 0x00;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.u32ColorLUT[1] = 0xFFFFFF;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.bInvColEn = RK_FALSE;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Width = 16;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.stInvColArea.u32Height = 16;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.enChgMod = LESSTHAN_LUM_THRESH;
		ctx->rgn[1].stRgnChnAttr.unChnAttr.stOverlayChn.stInvertColor.u32LumThresh = 100;

		/*********************************************
		step 3: show bitmap
		*********************************************/
		RK_S64 s64ShowBmpStart = TEST_COMM_GetNowUs();
		ctx->rgn[1].stBitmap.enPixelFormat = (PIXEL_FORMAT_E)RK_FMT_ARGB8888;
		ctx->rgn[1].stBitmap.u32Width = u32Width;
		ctx->rgn[1].stBitmap.u32Height = u32Height;

		RK_U16 ColorBlockSize = ctx->rgn[1].stBitmap.u32Height * ctx->rgn[1].stBitmap.u32Width;
		ctx->rgn[1].stBitmap.pData = malloc(ColorBlockSize * TEST_ARGB32_PIX_SIZE);
		uint8_t *ColorData = (uint8_t *)ctx->rgn[1].stBitmap.pData;
		if (filename) {
			uint32_t ret = load_file_osdmem(filename, ctx->rgn[1].stBitmap.pData, u32Width, u32Height,
			                          TEST_ARGB32_PIX_SIZE, 0);
			if (0 != ret) {
				set_argb8888_buffer((uint32_t *)ColorData, ColorBlockSize / 4, TEST_ARGB32_RED);
				set_argb8888_buffer((uint32_t *)(ColorData + ColorBlockSize),
				                    ColorBlockSize / 4, TEST_ARGB32_GREEN);
				set_argb8888_buffer((uint32_t *)(ColorData + 2 * ColorBlockSize),
				                    ColorBlockSize / 4, TEST_ARGB32_BLUE);
				set_argb8888_buffer((uint32_t *)(ColorData + 3 * ColorBlockSize),
				                    ColorBlockSize / 4, TEST_ARGB32_BLACK);
			}
		}
	}
	/* OSD OVERLAY parameters init END */

	/* AVS parameters init */
	//assume assist sensor is small resolution use it as default
	switch (enWorkMode) {
		case AVS_MODE_NOBLEND_VER:
			/* Splicing mode */
			ctx->avs.stAvsModParam.u32WorkingSetSize = 0;
			ctx->avs.stAvsModParam.enMBSource = MB_SOURCE_PRIVATE;
			ctx->avs.stAvsGrpAttr.enMode = AVS_MODE_NOBLEND_VER;
			ctx->avs.stAvsGrpAttr.bSyncPipe = 0;

			ctx->avs.stAvsGrpAttr.u32PipeNum = 2;
			ctx->avs.stAvsGrpAttr.stGainAttr.enMode = AVS_GAIN_MODE_AUTO;
			ctx->avs.stAvsGrpAttr.stFrameRate.s32SrcFrameRate = -1;

			ctx->avs.stAvsChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
			ctx->avs.stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
			ctx->avs.stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
			ctx->avs.stAvsChnAttr[0].u32Depth = 0;
			ctx->avs.stAvsChnAttr[0].u32Width = handle->sensor_init[1]->cam_w;
			ctx->avs.stAvsChnAttr[0].u32Height = handle->sensor_init[1]->cam_h * 2;

			ctx->avs.stAvsChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
			ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 2;
			break;
		case AVS_MODE_NOBLEND_OVL:
			/* Overlay mode */
			ctx->avs.stAvsModParam.u32WorkingSetSize = 0;
			ctx->avs.stAvsModParam.enMBSource = MB_SOURCE_PRIVATE;
			ctx->avs.stAvsGrpAttr.enMode = AVS_MODE_NOBLEND_OVL;

			ctx->avs.stAvsGrpAttr.u32PipeNum = 2;
			ctx->avs.stAvsGrpAttr.stGainAttr.enMode = AVS_GAIN_MODE_AUTO;

			ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Width = handle->sensor_init[1]->cam_w;
			ctx->avs.stAvsGrpAttr.stOutAttr.stSize.u32Height = handle->sensor_init[1]->cam_h;

			for (int32_t i = 0; i < 2; i++) {//todo pipe num
				ctx->avs.stAvsPipeAttr[i].u32Priority             = i;
				ctx->avs.stAvsPipeAttr[i].stSrcRect.s32X          = 0;
				ctx->avs.stAvsPipeAttr[i].stSrcRect.s32Y          = 0;
				ctx->avs.stAvsPipeAttr[i].stSrcRect.u32Width      = handle->sensor_init[i]->cam_w;
				ctx->avs.stAvsPipeAttr[i].stSrcRect.u32Height     = handle->sensor_init[i]->cam_h;
				if (0 == i) {
					ctx->avs.stAvsPipeAttr[i].stDstRect.u32Width      = handle->sensor_init[1]->cam_w;
					ctx->avs.stAvsPipeAttr[i].stDstRect.u32Height     = handle->sensor_init[1]->cam_h;
					ctx->avs.stAvsPipeAttr[i].stDstRect.s32X          = 0;
					ctx->avs.stAvsPipeAttr[i].stDstRect.s32Y          = 0;
				} else {
					ctx->avs.stAvsPipeAttr[i].stDstRect.u32Width      = handle->sensor_init[1]->cam_w / 4;
					ctx->avs.stAvsPipeAttr[i].stDstRect.u32Height     = handle->sensor_init[1]->cam_h / 4;
					ctx->avs.stAvsPipeAttr[i].stDstRect.s32X          = 0;
					ctx->avs.stAvsPipeAttr[i].stDstRect.s32Y          = 0;
				}
			}

			ctx->avs.stAvsChnAttr[0].enCompressMode = COMPRESS_MODE_NONE;
			ctx->avs.stAvsChnAttr[0].stFrameRate.s32SrcFrameRate = -1;
			ctx->avs.stAvsChnAttr[0].stFrameRate.s32DstFrameRate = -1;
			ctx->avs.stAvsChnAttr[0].u32Depth = 0;
			ctx->avs.stAvsChnAttr[0].u32Width = handle->sensor_init[1]->cam_w;
			ctx->avs.stAvsChnAttr[0].u32Height = handle->sensor_init[1]->cam_h;
			ctx->avs.stAvsChnAttr[0].enDynamicRange = DYNAMIC_RANGE_SDR8;
			ctx->avs.stAvsChnAttr[0].u32FrameBufCnt = 2;
			break;
		default:
			ERR("unsupport avs mode\n");
	}
	/* AVS parameters init END */

	/* VENC parameters init */
	fps = (uint32_t)get_cmd_val("rk_cam_fps", 10);
	RK_ASSERT(fps > 0);
	gop = fps * 2;

	memset(&ctx->venc.stChnAttr,0,sizeof(ctx->venc.stChnAttr));
	memset(&ctx->venc.stVencChnRefBufShare, 0, sizeof(ctx->venc.stVencChnRefBufShare));

	ctx->venc.s32ChnNum = 1;
	if (handle->app_params->venc_type == 1)
		ctx->venc.enCodecType = RK_VIDEO_ID_AVC;
	else
		ctx->venc.enCodecType = RK_VIDEO_ID_HEVC;

	if (ctx->venc.enCodecType == RK_VIDEO_ID_AVC) {
		ctx->venc.stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
		ctx->venc.stChnAttr.stRcAttr.stH264Vbr.u32BitRate = handle->app_params->venc_bitrate;
		ctx->venc.stChnAttr.stRcAttr.stH264Vbr.u32MaxBitRate = handle->app_params->venc_bitrate;
		ctx->venc.stChnAttr.stRcAttr.stH264Vbr.u32MinBitRate = 200;
		ctx->venc.stChnAttr.stRcAttr.stH264Vbr.u32Gop = gop;
		ctx->venc.stChnAttr.stRcAttr.stH264Vbr.u32SrcFrameRateNum = fps;
		ctx->venc.stChnAttr.stRcAttr.stH264Vbr.u32SrcFrameRateDen = 1;
		ctx->venc.stChnAttr.stRcAttr.stH264Vbr.fr32DstFrameRateNum = fps;
		ctx->venc.stChnAttr.stRcAttr.stH264Vbr.fr32DstFrameRateDen = 1;
	} else if (ctx->venc.enCodecType == RK_VIDEO_ID_HEVC) {
		ctx->venc.stChnAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
		ctx->venc.stChnAttr.stRcAttr.stH265Vbr.u32BitRate = handle->app_params->venc_bitrate;
		ctx->venc.stChnAttr.stRcAttr.stH265Vbr.u32MaxBitRate = handle->app_params->venc_bitrate;
		ctx->venc.stChnAttr.stRcAttr.stH265Vbr.u32MinBitRate = 200;
		ctx->venc.stChnAttr.stRcAttr.stH265Vbr.u32Gop = gop;
		ctx->venc.stChnAttr.stRcAttr.stH265Vbr.u32SrcFrameRateNum = fps;
		ctx->venc.stChnAttr.stRcAttr.stH265Vbr.u32SrcFrameRateDen = 1;
		ctx->venc.stChnAttr.stRcAttr.stH265Vbr.fr32DstFrameRateNum = fps;
		ctx->venc.stChnAttr.stRcAttr.stH265Vbr.fr32DstFrameRateDen = 1;
	}

	ctx->venc.stChnAttr.stVencAttr.enType = ctx->venc.enCodecType;
	ctx->venc.stChnAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	if (ctx->venc.enCodecType == RK_VIDEO_ID_AVC)
		ctx->venc.stChnAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
	else if (ctx->venc.enCodecType == RK_VIDEO_ID_HEVC)
		ctx->venc.stChnAttr.stVencAttr.u32Profile = H265E_PROFILE_MAIN;
	if (ctx->avs.stAvsGrpAttr.enMode == AVS_MODE_NOBLEND_VER) {
		//assume assist sensor is small resolution use it as default
		ctx->venc.stChnAttr.stVencAttr.u32PicWidth = handle->sensor_init[1]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32VirWidth = handle->sensor_init[1]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32PicHeight = handle->sensor_init[1]->cam_h * 2;
		ctx->venc.stChnAttr.stVencAttr.u32VirHeight = handle->sensor_init[1]->cam_h * 2;
		ctx->venc.stChnAttr.stVencAttr.u32MaxPicWidth = handle->sensor_init[1]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32MaxPicHeight = handle->sensor_init[1]->cam_h * 2;
		ctx->venc.stChnAttr.stVencAttr.u32BufSize = ctx->venc.stChnAttr.stVencAttr.u32PicWidth * ctx->venc.stChnAttr.stVencAttr.u32PicHeight / 2;
	} else if (ctx->avs.stAvsGrpAttr.enMode == AVS_MODE_NOBLEND_OVL) {
		//use assist sensor resolution as default
		ctx->venc.stChnAttr.stVencAttr.u32PicWidth = handle->sensor_init[1]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32PicHeight = handle->sensor_init[1]->cam_h;
		ctx->venc.stChnAttr.stVencAttr.u32VirWidth = handle->sensor_init[1]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32VirHeight = handle->sensor_init[1]->cam_h;
		ctx->venc.stChnAttr.stVencAttr.u32MaxPicWidth = handle->sensor_init[1]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32MaxPicHeight = handle->sensor_init[1]->cam_h;
		ctx->venc.stChnAttr.stVencAttr.u32BufSize = handle->sensor_init[1]->cam_w * handle->sensor_init[1]->cam_h / 2;
	}

	if (ctx->func.single_boot) {
		ctx->venc.stChnAttr.stVencAttr.u32PicWidth = handle->sensor_init[0]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32PicHeight = handle->sensor_init[0]->cam_h;
		ctx->venc.stChnAttr.stVencAttr.u32VirWidth = handle->sensor_init[0]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32VirHeight = handle->sensor_init[0]->cam_h;
		ctx->venc.stChnAttr.stVencAttr.u32MaxPicWidth = handle->sensor_init[0]->cam_w;
		ctx->venc.stChnAttr.stVencAttr.u32MaxPicHeight = handle->sensor_init[0]->cam_h;
		ctx->venc.stChnAttr.stVencAttr.u32BufSize = handle->sensor_init[0]->cam_w * handle->sensor_init[0]->cam_h / 2;
	}

	ctx->venc.stChnAttr.stVencAttr.u32StreamBufCnt = 4;
	ctx->venc.stChnAttr.stVencAttr.enMirror = MIRROR_NONE;

	ctx->venc.stVencChnRefBufShare.bEnable = true;

	memset(&ctx->venc.stRcParam, 0, sizeof(ctx->venc.stRcParam));
	if (ctx->venc.enCodecType == RK_VIDEO_ID_AVC) {
		ctx->venc.stRcParam.s32FirstFrameStartQp = 28;
		ctx->venc.stRcParam.stParamH264.u32MinQp = 10;
		ctx->venc.stRcParam.stParamH264.u32MaxQp = 51;
		ctx->venc.stRcParam.stParamH264.u32MinIQp = 10;
		ctx->venc.stRcParam.stParamH264.u32MaxIQp = 51;
		ctx->venc.stRcParam.stParamH264.u32FrmMinQp = 25;
		ctx->venc.stRcParam.stParamH264.u32FrmMinIQp = 24;
		ctx->venc.stRcParam.stParamH264.u32FrmMaxQp = 41;
		ctx->venc.stRcParam.stParamH264.u32FrmMaxIQp = 35;
	} else if (ctx->venc.enCodecType == RK_VIDEO_ID_HEVC) {
		ctx->venc.stRcParam.s32FirstFrameStartQp = 28;
		ctx->venc.stRcParam.stParamH265.u32MinQp = 10;
		ctx->venc.stRcParam.stParamH265.u32MaxQp = 51;
		ctx->venc.stRcParam.stParamH265.u32MinIQp = 10;
		ctx->venc.stRcParam.stParamH265.u32MaxIQp = 51;
		ctx->venc.stRcParam.stParamH265.u32FrmMinQp = 25;
		ctx->venc.stRcParam.stParamH265.u32FrmMinIQp = 24;
		ctx->venc.stRcParam.stParamH265.u32FrmMaxQp = 41;
		ctx->venc.stRcParam.stParamH265.u32FrmMaxIQp = 35;
	}

	memset(&ctx->venc.stRecvParam, 0, sizeof(ctx->venc.stRecvParam));
	ctx->venc.stRecvParam.s32RecvPicNum = -1;
	/* VENC parameters init END */

	return 0;
}

static void aiq_quit()
{
	int camId = 0;

	for (; camId < CAM_NUM; camId++) {
		if (g_aiq_ctx[camId]) {
			klog("should quit\n");
			rk_aiq_uapi2_sysctl_stop(g_aiq_ctx[camId], false);
			klog("aiq stop\n");
			rk_aiq_uapi2_sysctl_deinit(g_aiq_ctx[camId]);
			klog("aiq deinit\n");
		}
	}
}

int echo(char *file_path, char *buf, int lenth)
{

		FILE *fp = NULL;
		fp = fopen(file_path, "w");
		if (fp == NULL) {
			perror("fopen error\n");
			return -1;
		}

		if (lenth > fwrite(buf, 1, lenth, fp))
		{
			ERR("fwrite error\n");
			fclose(fp);
			return -1;
		}
		INFO("echo \"%s\" > %s successfully\n", buf, file_path);
		fclose(fp);
		return 0;
}

int main(int argc, char *argv[])
{
	klog("main\n");

	MPI_CTX_S ctx;
	void *metaVirmem = NULL;
	int meta_size, ret;
	struct meta_info handle;
	struct sigaction action;

	action.sa_handler = handle_pipe;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;

	handle.sensor_init[0] = (struct sensor_init_cfg *)malloc(sizeof(struct sensor_init_cfg));
	handle.sensor_init[1] = (struct sensor_init_cfg *)malloc(sizeof(struct sensor_init_cfg));
	handle.app_params = (struct app_param_info *)malloc(sizeof(struct app_param_info));

	g_s32FrameCnt = RUN_TOTAL_CNT_MAX;

	sigaction(SIGPIPE, &action, NULL);
	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != 0) {
		goto __FAILED;
	}
	klog("mpi sys init\n");

	meta_size = (uint32_t)get_cmd_val("meta_part_size", 16);
	metaVirmem = get_meta_params(&handle);
	if (metaVirmem == MAP_FAILED)
		goto __FAILED;
#if SERVER_DEBUG_PRT
	dump_meta_sensor_app_params(&handle);
#endif
	memset(&ctx, 0, sizeof(MPI_CTX_S));
	/*
	 * single cam boot
	 * vi dev0 chn0 -> venc chn 0
	 * 
	 * dual cam boot
	 * vi dev0 chn0 -> avs pipe 0->avs (avs input)
	 * vi dev1 chn0 -> avs pipe 1->avs (avs input)
	 * avs chn 0 -> venc (avs output)
	 * cover -> vi dev0 chn0
	 * osd -> venc chn0
	 */
	param_init(&ctx, &handle);
	venc_init(&ctx);

	vi_init(&ctx);
	if (ctx.func.single_boot)
		vi_bind_venc();
	else {
		avs_init(&ctx);
		vi_bind_avs();
		avs_bind_venc();
	}
	if (ctx.func.cover_en)
		cover_init(&ctx);
	if (ctx.func.osd_en)
		osd_init(&ctx);

	ret = aiq_init_start(&handle, metaVirmem, false, &ctx);
	if (ret)
		goto __FAILED;

	if (ctx.func.eptz_en) {
		INFO("eptz is enabled, fix isp aclk to 350MHz, core_clk to 420MHz\n");
		echo("/proc/clk/rate", ISP_CLK_CORE_RATE, sizeof(ISP_CLK_CORE_RATE));
		echo("/proc/clk/rate", ISP_ACLK_RATE, sizeof(ISP_ACLK_RATE));
	}

#if (ENABLE_RTSP)
	rtsp_init();
#endif

	pthread_t main_thread0;
	pthread_create(&main_thread0, NULL, GetVencStream, &ctx);

	int loop_cnt = 0x400;
	while (1) {
		if (g_should_quit) {
			aiq_quit();
			break;
		}
		usleep(1 * 1000); // when client online
		if (loop_cnt-- == 0) {
			klog("aiq timeout exit\n");
			break;
		}
	}
	ret = aiq_init_start(&handle, metaVirmem, true, &ctx);

	pthread_join(main_thread0, RK_NULL);

#if (ENABLE_RTSP)
	if (g_rtsplive)
		rtsp_del_demo(g_rtsplive);
#endif

__FAILED:
	vi_unbind_avs();
	avs_unbind_venc();
	avs_deinit();
	cover_osd_deinit(ctx.func.osd_en, ctx.func.cover_en, &ctx);
	venc_deinit();
	vi_deinit();
	aiq_deinit();
	if (metaVirmem != MAP_FAILED)
		munmap(metaVirmem, meta_size);
	printf("app exit\n");
	return 0;
}
