/* thunder boot service
/1.aiq init & run
/2.rockit vi & venc init, bind
*/
#include <stdio.h>
#include <stddef.h>
#include <sys/poll.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <fcntl.h>

#include "rk_type.h"
#include "rk_debug.h"
#include "rk_defines.h"
#include "rk_mpi_venc.h"
#include "rk_mpi_vi.h"
#include "rk_mpi_sys.h"
#include <rk_aiq_user_api2_camgroup.h>
#include <rk_aiq_user_api2_imgproc.h>
#include <rk_aiq_user_api2_sysctl.h>
#include <mediactl/mediactl.h>

#include "v4l2_link.h"
#include "rk_gpio.h"
#include "rk_meta.h"
#include "sensor_init_info.h"
#include "sensor_iq_info.h"
#include "rk_meta_app_param.h"

#define RKAIQ
//#define ENABLE_GET_STREAM

#define SMART_DOOR_IR_FLOOD_MDEV	"/dev/media3"
#define SMART_DOOR_IR_PROJECTOR_MDEV	"/dev/media4"

#define MAP_SIZE (4096UL * 50) //MAP_SIZE = 4 * 50 K
#define MAP_MASK (MAP_SIZE - 1) //MAP_MASK = 0XFFF

#define MAP_SIZE_NIGHT (4096UL) //MAP_SIZE = 4K
#define MAP_MASK_NIGHT (MAP_SIZE_NIGHT - 1) //MAP_MASK = 0XFFF

void *rkaiq_dl = NULL;
XCamReturn (*dlsym_rk_aiq_uapi2_sysctl_enumStaticMetas)(int, rk_aiq_static_info_t*);
XCamReturn (*dlsym_rk_aiq_uapi2_sysctl_preInit_scene)(const char*, const char*, const char*);
XCamReturn (*dlsym_rk_aiq_uapi2_sysctl_preInit_iq_addr)(const char*, void *, size_t);
XCamReturn (*dlsym_rk_aiq_uapi2_sysctl_preInit_tb_info)(const char*, const rk_aiq_tb_info_t*);
rk_aiq_sys_ctx_t* (*dlsym_rk_aiq_uapi2_sysctl_init)(const char*,
		const char*,
		rk_aiq_error_cb ,
		rk_aiq_metas_cb );

XCamReturn (*dlsym_rk_aiq_uapi2_sysctl_prepare)(const rk_aiq_sys_ctx_t*,
		uint32_t  , uint32_t  ,
		rk_aiq_working_mode_t );

XCamReturn (*dlsym_rk_aiq_uapi2_sysctl_start)(const rk_aiq_sys_ctx_t *);
XCamReturn (*dlsym_rk_aiq_uapi2_sysctl_stop)(const rk_aiq_sys_ctx_t*, bool);
XCamReturn (*dlsym_rk_aiq_uapi2_sysctl_deinit)(const rk_aiq_sys_ctx_t*);

static RK_S32 g_s32FrameCnt = -1;
static VI_CHN_BUF_WRAP_S g_stViWrap;
static bool g_bWrap = false;
static RK_U32 g_u32WrapLine = 0;
static RK_U32 g_prd_type = RK_AIQ_PRD_TYPE_TB_DOORLOCK;
static char *g_sEntityName = NULL;
static bool quit = false;
static struct app_param_info *g_pAppParam = NULL;
static struct sensor_init_cfg *g_pSensorInitParam = NULL;


static void sigterm_handler(int sig)
{
	quit = true;
}

static int dlsym_rkaiq (void)
{
	rkaiq_dl = dlopen("/usr/lib/librkaiq.so", RTLD_LAZY);
	if (!rkaiq_dl) {
		printf("\ndlopen /usr/lib/librkaiq.so error\n");
		return -1;
	}
	dlsym_rk_aiq_uapi2_sysctl_enumStaticMetas = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_enumStaticMetas");
	dlsym_rk_aiq_uapi2_sysctl_preInit_scene = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_preInit_scene");
	dlsym_rk_aiq_uapi2_sysctl_preInit_iq_addr = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_preInit_iq_addr");
	dlsym_rk_aiq_uapi2_sysctl_preInit_tb_info = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_preInit_tb_info");
	dlsym_rk_aiq_uapi2_sysctl_init = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_init");
	dlsym_rk_aiq_uapi2_sysctl_prepare = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_prepare");
	dlsym_rk_aiq_uapi2_sysctl_start = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_start");
	dlsym_rk_aiq_uapi2_sysctl_stop = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_stop");
	dlsym_rk_aiq_uapi2_sysctl_deinit = dlsym(rkaiq_dl, "rk_aiq_uapi2_sysctl_deinit");
	return 0;
}

/*
 *  * get cmdline from /proc/cmdline
 *  */
static int read_cmdline_to_buf(void *buf, int len)
{
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

long get_cmd_val(const char *string, int len)
{
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

RK_U64 TEST_COMM_GetNowUs()
{
	struct timespec time = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

#ifdef ENABLE_GET_STREAM

// fast client to get media buffer.
static void  *GetMediaBuffer0(void *arg)
{
	(void)arg;
	void *pData = RK_NULL;
	int loopCount = 0;
	int s32Ret;
	VENC_STREAM_S stFrame;

	char *pOutPath = "/tmp/venc-test.bin";
	FILE *venc0_file = fopen(pOutPath, "w");
	if (!venc0_file) {
		return 0;
	}

	FILE *fp = fopen("/tmp/pts.txt", "wb");

	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(0, &stFrame, -1);
		if (s32Ret == RK_SUCCESS) {
			if (venc0_file) {
				pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
				fwrite(pData, 1, stFrame.pstPack->u32Len, venc0_file);
				fflush(venc0_file);
			}
			RK_U64 nowUs = TEST_COMM_GetNowUs();

			printf("chn:0, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n", loopCount,
			       stFrame.u32Seq, stFrame.pstPack->u32Len, stFrame.pstPack->u64PTS, nowUs - stFrame.pstPack->u64PTS);
			if (fp) {
				char str[128];
				snprintf(str, sizeof(str), "seq:%u, pts:%llums\n", stFrame.u32Seq, stFrame.pstPack->u64PTS / 1000);
				fputs(str, fp);
				fsync(fileno(fp));
			}

			s32Ret = RK_MPI_VENC_ReleaseStream(0, &stFrame);
			loopCount++;
		} else {
			printf("RK_MPI_VENC_GetChnFrame fail %x\n", s32Ret);
		}

		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
			quit = true;
			break;
		}

		usleep(10 * 1000);
	}

	if (venc0_file)
		fclose(venc0_file);

	if (fp)
		fclose(fp);

	free(stFrame.pstPack);
	return NULL;
}
#endif

static RK_S32 test_venc_init(int chnId, int width, int height, int max_width, int max_height,
			     RK_CODEC_ID_E enType)
{
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_BUF_WRAP_S stVencChnBufWrap;
	VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
	VENC_CHN_ATTR_S stAttr;
	RK_U32 fps = 0;
	RK_U32 gop = 0;

	fps = (RK_U32)get_cmd_val("rk_cam_fps", 10);
	RK_ASSERT(fps > 0);
	gop = fps * 2;

	memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
	stVencChnBufWrap.bEnable = g_bWrap;
	stVencChnBufWrap.u32BufLine = g_u32WrapLine;

	memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
	stVencChnRefBufShare.bEnable = true;

#if 0
	if (enType == RK_VIDEO_ID_AVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264CBR;
		stAttr.stRcAttr.stH264Cbr.u32BitRate = g_pAppParam->venc_bitrate;
		stAttr.stRcAttr.stH264Cbr.u32Gop = gop;
	} else if (enType == RK_VIDEO_ID_HEVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265CBR;
		stAttr.stRcAttr.stH265Cbr.u32BitRate = g_pAppParam->venc_bitrate;
		stAttr.stRcAttr.stH265Cbr.u32Gop = gop;
	}
#endif
	if (enType == RK_VIDEO_ID_AVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H264VBR;
		stAttr.stRcAttr.stH264Vbr.u32BitRate = g_pAppParam->venc_bitrate;
		stAttr.stRcAttr.stH264Vbr.u32MaxBitRate = g_pAppParam->venc_bitrate;
		stAttr.stRcAttr.stH264Vbr.u32MinBitRate = 200;
		stAttr.stRcAttr.stH264Vbr.u32Gop = gop;
		stAttr.stRcAttr.stH264Vbr.u32SrcFrameRateNum = fps;
		stAttr.stRcAttr.stH264Vbr.u32SrcFrameRateDen = 1;
		stAttr.stRcAttr.stH264Vbr.fr32DstFrameRateNum = fps;
		stAttr.stRcAttr.stH264Vbr.fr32DstFrameRateDen = 1;
	} else if (enType == RK_VIDEO_ID_HEVC) {
		stAttr.stRcAttr.enRcMode = VENC_RC_MODE_H265VBR;
		stAttr.stRcAttr.stH265Vbr.u32BitRate = g_pAppParam->venc_bitrate;
		stAttr.stRcAttr.stH265Vbr.u32MaxBitRate = g_pAppParam->venc_bitrate;
		stAttr.stRcAttr.stH265Vbr.u32MinBitRate = 200;
		stAttr.stRcAttr.stH265Vbr.u32Gop = gop;
		stAttr.stRcAttr.stH265Vbr.u32SrcFrameRateNum = fps;
		stAttr.stRcAttr.stH265Vbr.u32SrcFrameRateDen = 1;
		stAttr.stRcAttr.stH265Vbr.fr32DstFrameRateNum = fps;
		stAttr.stRcAttr.stH265Vbr.fr32DstFrameRateDen = 1;
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
	stAttr.stVencAttr.u32MaxPicWidth = max_width;
	stAttr.stVencAttr.u32MaxPicHeight = max_height;
	stAttr.stVencAttr.u32StreamBufCnt = 4;
	/* stAttr.stVencAttr.u32BufSize = max_width * max_height / 2; */
	stAttr.stVencAttr.u32BufSize = max_width * max_height;
	stAttr.stVencAttr.enMirror = MIRROR_NONE;

	RK_MPI_VENC_CreateChn(chnId, &stAttr);
	if (g_bWrap)
		RK_MPI_VENC_SetChnBufWrapAttr(chnId, &stVencChnBufWrap);
	RK_MPI_VENC_SetChnRefBufShareAttr(chnId, &stVencChnRefBufShare);

	VENC_RC_PARAM_S stRcParam;
	memset(&stRcParam, 0, sizeof(stRcParam));
	if (enType == RK_VIDEO_ID_AVC) {
		stRcParam.s32FirstFrameStartQp = 28;
		stRcParam.stParamH264.u32MinQp = 10;
		stRcParam.stParamH264.u32MaxQp = 51;
		stRcParam.stParamH264.u32MinIQp = 10;
		stRcParam.stParamH264.u32MaxIQp = 51;
		stRcParam.stParamH264.u32FrmMinQp = 25;
		stRcParam.stParamH264.u32FrmMinIQp = 24;
		stRcParam.stParamH264.u32FrmMaxQp = 41;
		stRcParam.stParamH264.u32FrmMaxIQp = 35;
	} else if (enType == RK_VIDEO_ID_HEVC) {
		stRcParam.s32FirstFrameStartQp = 28;
		stRcParam.stParamH265.u32MinQp = 10;
		stRcParam.stParamH265.u32MaxQp = 51;
		stRcParam.stParamH265.u32MinIQp = 10;
		stRcParam.stParamH265.u32MaxIQp = 51;
		stRcParam.stParamH265.u32FrmMinQp = 25;
		stRcParam.stParamH265.u32FrmMinIQp = 24;
		stRcParam.stParamH265.u32FrmMaxQp = 41;
		stRcParam.stParamH265.u32FrmMaxIQp = 35;
	}
	RK_MPI_VENC_SetRcParam(chnId, &stRcParam);

	RK_MPI_VENC_EnableSvc(chnId, RK_TRUE);

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

int vi_dev_init()
{
	int ret = 0;
	int devId = 0;
	int pipeId = devId;

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
		stBindPipe.u32Num = 1;
		stBindPipe.PipeId[0] = pipeId;
		stBindPipe.bUserStartPipe[0] = RK_TRUE;
		ret = RK_MPI_VI_SetDevBindPipe(devId, &stBindPipe);
		if (ret != RK_SUCCESS) {
			return -1;
		}
	}

	return 0;
}

int vi_chn_init(int channelId, int width, int height, int max_width, int max_height)
{
	int ret;
	int buf_cnt = 3;

	// VI init
	VI_CHN_ATTR_S vi_chn_attr;

	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	vi_chn_attr.stIspOpt.stMaxSize.u32Width  = max_width;
	vi_chn_attr.stIspOpt.stMaxSize.u32Height = max_height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE;
	vi_chn_attr.u32Depth = 2;

	if (g_sEntityName != NULL)
		memcpy(vi_chn_attr.stIspOpt.aEntityName, g_sEntityName, strlen(g_sEntityName));
	ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);

	g_stViWrap.bEnable           = g_bWrap;
	g_stViWrap.u32BufLine        = g_u32WrapLine;
	g_stViWrap.u32WrapBufferSize = g_stViWrap.u32BufLine * max_width * 3 / 2;
	if (g_bWrap)
		RK_MPI_VI_SetChnWrapBufAttr(0, channelId, &g_stViWrap);

	ret |= RK_MPI_VI_EnableChnExt(0, channelId);
	if (ret) {
		printf("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}

	return ret;
}

static int g_err_cnt = 0;
static bool g_should_quit = false;

static XCamReturn SAMPLE_COMM_ISP_ErrCb(rk_aiq_err_msg_t *msg)
{
	g_err_cnt++;
	if (g_err_cnt <= 2)
		printf("=== %u ===\n", msg->err_code);
	if (msg->err_code == XCAM_RETURN_BYPASS)
		g_should_quit = true;
}

#if 1
void klog(const char *log)
{
	FILE *fp = fopen("/dev/kmsg", "w");
	if (NULL != fp) {
		fprintf(fp, "[app]: %s\n", log);
		fclose(fp);
	}
}
#else
void klog(const char *log)
{
	return;
}
#endif

/* link isp before all video stream on */
int v4l2_isp_luma_link_init(const char *mdev_path_isp_flood, const char *mdev_path_isp_pro)
{
	v4l2_media_link_rawrd(mdev_path_isp_flood, true);

	v4l2_media_link_rawrd(mdev_path_isp_pro, true);

	v4l2_media_copy_isp_entity(mdev_path_isp_flood, mdev_path_isp_pro);
}

int main(int argc, char *argv[])
{
	klog(__FILE__);

	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 0;
	RK_U32 u32Height = 0;
	RK_U32 u32MaxWidth = 0;
	RK_U32 u32MaxHeight = 0;
	RK_CHAR *pCodecName = "H264";

	RK_CODEC_ID_E enCodecType = RK_VIDEO_ID_Max;//RK_VIDEO_ID_HEVC;
	RK_S32 s32chnlId = 0;
	int aiq_index = -1;

	v4l2_isp_luma_link_init(SMART_DOOR_IR_FLOOD_MDEV, SMART_DOOR_IR_PROJECTOR_MDEV);

	int mem_fd = -1;
	off_t appParamOffs = 0, metaAddr = 0;
	void *metaVirmem = NULL, *appVirAddr = NULL, *SensorInitVirAddr;
	RK_U32 metaSize = (RK_U32)get_cmd_val("meta_part_size", 16);
	metaAddr = (off_t)get_cmd_val("meta_load_addr", 16);
	if ((mem_fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) {
		printf("cannot open /dev/mem.\n");
		return -1;
	}

	metaVirmem = mmap(NULL, metaSize, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, metaAddr);
	if (metaVirmem != MAP_FAILED) {
		//get sensir init cfg addr, include ircut info
		SensorInitVirAddr = metaVirmem + SENSOR_INIT_OFFSET;
		g_pSensorInitParam = (struct sensor_init_cfg *)(SensorInitVirAddr);
		//get app param addr
		appVirAddr = metaVirmem + APP_PARAM_OFFSET;
		g_pAppParam = (struct app_param_info *)(appVirAddr);
		u32Width = g_pAppParam->venc_w;
		u32Height = g_pAppParam->venc_h;
		u32MaxWidth = g_pSensorInitParam->cam_w;
		u32MaxHeight = g_pSensorInitParam->cam_h;

		switch (g_pAppParam->venc_type) {
		case 1:
			enCodecType = RK_VIDEO_ID_AVC; // h.264
			break;
		case 2:
		default:
			enCodecType = RK_VIDEO_ID_HEVC; // h.265
			break;
		}
		printf("\n read from meta: w:%d, h:%d, bitrate:%d, venc type:%d\n",
		       g_pAppParam->venc_w, g_pAppParam->venc_h,
		       g_pAppParam->venc_bitrate, g_pAppParam->venc_type);
	} else {
		printf("mmap fail.\n");
		goto __FAILED;
	}

	//ircut pull down to low power mode
	int gpio_ircut_a = g_pSensorInitParam->ircut_a.gpio_index;
	int gpio_ircut_b = g_pSensorInitParam->ircut_b.gpio_index;
	if (gpio_ircut_a >= 0 && gpio_ircut_b >= 0) {
		rk_gpio_export(gpio_ircut_a);
		rk_gpio_set_direction(gpio_ircut_a, RK_FALSE);
		rk_gpio_set_value(gpio_ircut_a, 0);
		rk_gpio_export(gpio_ircut_b);
		rk_gpio_set_direction(gpio_ircut_b, RK_FALSE);
		rk_gpio_set_value(gpio_ircut_b, 0);
	}

	if (g_bWrap)
		g_u32WrapLine = u32Height / 4; // 360   // 1080
	g_s32FrameCnt = 20;
	g_sEntityName = "/dev/video22";

	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		goto __FAILED;
	}
	klog("sys_init");
	test_venc_init(0, u32Width, u32Height, u32MaxWidth, u32MaxHeight,
		       enCodecType);//RK_VIDEO_ID_AVC RK_VIDEO_ID_HEVC
	klog("venc_init");

	vi_dev_init();
	klog("vi_dev");
	vi_chn_init(s32chnlId, u32Width, u32Height, u32MaxWidth, u32MaxHeight);
	klog("vi_chn");

	MPP_CHN_S stSrcChn, stDestChn;
	// bind vi to venc
	stSrcChn.enModId    = RK_ID_VI;
	stSrcChn.s32DevId   = 0;
	stSrcChn.s32ChnId   = 0;

	stDestChn.enModId   = RK_ID_VENC;
	stDestChn.s32DevId  = 0;
	stDestChn.s32ChnId  = 0;
	s32Ret = RK_MPI_SYS_Bind(&stSrcChn, &stDestChn);
	if (s32Ret != RK_SUCCESS) {
		goto __FAILED;
	}
	klog("bind");

#if 1
	// venc init, if is fast boot, must first init venc.
	if (fork() > 0) {

	} else {
		while (1)
			usleep(1000 * 1000); //when client online
		printf("sub service exit main\n");
		goto __FAILED;
	}

	RK_MPI_VI_StartPipe(0);
	klog("vi resume");
#endif

#ifdef RKAIQ
	size_t file_size;
	int ret = 0;
	void *vir_iqaddr;
	RK_S64 s64AiqInitStart = TEST_COMM_GetNowUs();

	if (dlsym_rkaiq() != 0) {
		goto __FAILED;
	}

	// addr_iq = (off_t)get_cmd_val("rk_iqbin_addr", 16);
	file_size = (int)get_cmd_val("rk_iqbin_size", 16);
	vir_iqaddr = metaVirmem + SENSOR_IQ_BIN_OFFSET + offsetof(struct sensor_iq_info, data);
	klog("mmap iq mem");

	int cam_hdr = (int)get_cmd_val("rk_cam_hdr", 0);
	rk_aiq_working_mode_t hdr_mode = (cam_hdr == 5) ? RK_AIQ_WORKING_MODE_ISP_HDR2 :
					 RK_AIQ_WORKING_MODE_NORMAL;

	rk_aiq_sys_ctx_t *aiq_ctx;
	const char *sensor_name = "m00_b_gc2093 4-0037";

	klog("aiq enum");

	if (g_pAppParam->color_mode) {
		printf("=====night mode=====\n");
		ret = dlsym_rk_aiq_uapi2_sysctl_preInit_scene(sensor_name, "normal", "night");
		if (ret < 0) {
			printf("%s: failed to set night scene\n", sensor_name);
			goto __FAILED;
		}
	} else {
		printf("=====day mode=======\n");
		ret = dlsym_rk_aiq_uapi2_sysctl_preInit_scene(sensor_name, "normal", "day");
		if (ret < 0) {
			printf("%s: failed to set day scene\n", sensor_name);
			goto __FAILED;
		}
	}
	klog("preint scene");

	ret = dlsym_rk_aiq_uapi2_sysctl_preInit_iq_addr(sensor_name, vir_iqaddr,
						  (size_t)file_size);
	if (ret < 0) {
		printf("%s: failed to load binary iqfiles\n", sensor_name);
	}
	klog("preinit iq addr");

	rk_aiq_tb_info_t tb_info;

	memset(&tb_info, 0x00, sizeof(tb_info));

	tb_info.magic      = sizeof(rk_aiq_tb_info_t) - 2;
	tb_info.is_pre_aiq = true;
	tb_info.prd_type   = g_prd_type;

	dlsym_rk_aiq_uapi2_sysctl_preInit_tb_info(sensor_name, &tb_info);
	klog("preinit tb info");

	klog(sensor_name);
	aiq_ctx = dlsym_rk_aiq_uapi2_sysctl_init(sensor_name,
					   "/etc/iqfiles/", SAMPLE_COMM_ISP_ErrCb, NULL);
	klog("sysctl init");
	if (!aiq_ctx) {
		printf("%s: failed to init aiq\n", sensor_name);
		goto __FAILED;
	}

	if (dlsym_rk_aiq_uapi2_sysctl_prepare(aiq_ctx, 0, 0, hdr_mode)) {
		printf("rkaiq engine prepare failed !\n");
		goto __FAILED;
	}
	klog("sysctl prepare");

	if (dlsym_rk_aiq_uapi2_sysctl_start(aiq_ctx)) {
		printf("rk_aiq_uapi2_sysctl_start  failed\n");
		goto __FAILED;
	}
	klog("sysctl start");
	RK_S64 s64AiqInitEnd = TEST_COMM_GetNowUs();
	printf("Aiq:%lld us\n", s64AiqInitEnd - s64AiqInitStart);
#endif
	klog("aiq start");

#ifdef ENABLE_GET_STREAM
	GetMediaBuffer0(NULL);
#endif

#ifdef RKAIQ
	int loop_cnt = 0x400;
	while (1) {
		if (g_should_quit) {
			klog("should quit");
			dlsym_rk_aiq_uapi2_sysctl_stop(aiq_ctx, false);
			klog("aiq stop");
			dlsym_rk_aiq_uapi2_sysctl_deinit(aiq_ctx);
			klog("aiq deinit");
			break;
		}
		usleep(1 * 1000); //when client online
		if (loop_cnt-- == 0) {
			klog("aiq timeout exit");
			break;
		}
	}
#endif

__FAILED:
	if (rkaiq_dl != NULL) {
		dlclose(rkaiq_dl);
	}

	if (metaVirmem != MAP_FAILED) munmap(metaVirmem, metaSize);
	if (mem_fd != -1) close(mem_fd);

	printf("main service exit main\n");
	return 0;
}
