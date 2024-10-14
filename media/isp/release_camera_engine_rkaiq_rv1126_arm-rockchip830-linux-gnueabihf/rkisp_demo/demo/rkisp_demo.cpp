/*
 * V4L2 video capture example
 * AUTHOT : Jacob Chen
 * DATA : 2018-02-25
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h> /* getopt_long() */
#include <fcntl.h> /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <dlfcn.h>
#include <signal.h>
#include <linux/videodev2.h>

#include "drmDsp.h"
#include "rk_aiq_user_api_sysctl.h"
#include "rk_aiq_user_api_imgproc.h"
#include "rk_aiq_user_api_debug.h"
#include <termios.h>
#include "display.h"
#include "rga.h"
#include "ae_algo_demo/rk_aiq_algo_ae_demo_itf.h"
#include "ae_algo_demo/third_party_ae_algo.h"
#include "rkisp_demo.h"
#include "sample/sample_smartIr.h"
#include <cstdio>
#include <fcntl.h>
#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define FMT_NUM_PLANES 1

#define BUFFER_COUNT 8
#define CAPTURE_RAW_PATH "/tmp"
#define CAPTURE_CNT_FILENAME ".capture_cnt"
// #define ENABLE_UAPI_TEST
#define IQFILE_PATH_MAX_LEN 256
//#define OFFLINE_FRAME_TEST
//#define CUSTOM_AE_DEMO_TEST
//#define TEST_MEMS_SENSOR_INTF
//#define SMART_IR_SAMPLE_TEST
#ifdef ISPFEC_API
#include "IspFec/rk_ispfec_api.h"
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <libdrm/drm_mode.h>
#include <drm_fourcc.h>

struct drm_buf {
  int fb_id;
  uint32_t handle;
  uint32_t size;
  uint32_t pitch;
  char *map;
  int dmabuf_fd;
};

static rk_ispfec_ctx_t* g_ispfec_ctx = NULL;
static rk_ispfec_cfg_t g_ispfec_cfg;
struct drm_buf g_drm_buf_pic_out;
struct drm_buf g_drm_buf_xint;
struct drm_buf g_drm_buf_xfra;
struct drm_buf g_drm_buf_yint;
struct drm_buf g_drm_buf_yfra;
#endif

struct buffer {
    void *start;
    size_t length;
    int export_fd;
    int sequence;
};

enum TEST_CTL_TYPE {
    TEST_CTL_TYPE_DEFAULT,
    TEST_CTL_TYPE_REPEAT_INIT_PREPARE_START_STOP_DEINIT,
    TEST_CTL_TYPE_REPEAT_START_STOP,
    TEST_CTL_TYPE_REPEAT_PREPARE_START_STOP,
    TEST_CTL_TYPE_REPEAT_GETSET_CALIB,
    TEST_CTL_TYPE_REPEAT_HDR_NORMAL_SWITCH,
};

rk_aiq_ae_register_t ae_reg;

static struct termios oldt;
static int silent;
static demo_context_t *g_main_ctx = NULL, *g_second_ctx = NULL;
#ifdef ISPFEC_API
int alloc_drm_buffer(int fd, int width, int height,
		int bpp, struct drm_buf *buf)
{
	struct drm_mode_create_dumb alloc_arg;
	struct drm_mode_map_dumb mmap_arg;
	struct drm_mode_destroy_dumb destory_arg;
	void *map;
	int ret;

	memset(&alloc_arg, 0, sizeof(alloc_arg));
	alloc_arg.bpp = bpp;
	alloc_arg.width = width;
	alloc_arg.height = height;

	ret = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &alloc_arg);
	if (ret) {
		printf("failed to create dumb buffer\n");
		return ret;
	}

	memset(&mmap_arg, 0, sizeof(mmap_arg));
	mmap_arg.handle = alloc_arg.handle;
	ret = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mmap_arg);
	if (ret) {
		printf("failed to create map dumb\n");
		ret = -EINVAL;
		goto destory_dumb;
	}
	map = mmap(0, alloc_arg.size,
		PROT_READ | PROT_WRITE, MAP_SHARED,
		fd, mmap_arg.offset);
	if (map == MAP_FAILED) {
		printf("failed to mmap buffer\n");
		ret = -EINVAL;
		goto destory_dumb;
	}
	ret = drmPrimeHandleToFD(fd, alloc_arg.handle, 0,
			&buf->dmabuf_fd);
	if (ret) {
		printf("failed to get dmabuf fd\n");
		munmap(map, alloc_arg.size);
		ret = -EINVAL;
		goto destory_dumb;
	}
	buf->size = alloc_arg.size;
	buf->map = (char*)map;

destory_dumb:
	memset(&destory_arg, 0, sizeof(destory_arg));
	destory_arg.handle = alloc_arg.handle;
	drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destory_arg);
	return ret;
}

int free_drm_buffer(int fd, struct drm_buf *buf)
{
	if (buf) {
		close(buf->dmabuf_fd);
		return munmap(buf->map, buf->size);
	}
	return -EINVAL;
}

int init_ispfec_bufs(rk_ispfec_cfg_t* cfg)
{
    int ret = 0;
	int drm_fd = drmOpen("rockchip", NULL);
	if (drm_fd < 0) {
		printf("failed to open rockchip drm\n");
        return -1;
	}

    int mesh_size = rk_ispfec_api_calFecMeshsize(cfg->width, cfg->height);

	printf("\nmesh_size:%d\n", mesh_size);
	ret = alloc_drm_buffer(drm_fd, mesh_size * 2, 1, 8, &g_drm_buf_xint);
	if (ret)
		goto close_drm_fd;
	printf("xint fd:%d size:%d\n", g_drm_buf_xint.dmabuf_fd, g_drm_buf_xint.size);

	ret = alloc_drm_buffer(drm_fd, mesh_size, 1, 8, &g_drm_buf_xfra);
	if (ret)
		goto free_drm_buf_xint;
	printf("xfra fd:%d size:%d\n", g_drm_buf_xfra.dmabuf_fd, g_drm_buf_xfra.size);

	ret = alloc_drm_buffer(drm_fd, mesh_size * 2, 1, 8, &g_drm_buf_yint);
	if (ret)
		goto free_drm_buf_xfra;
	printf("yint fd:%d size:%d\n", g_drm_buf_yint.dmabuf_fd, g_drm_buf_yint.size);

	ret = alloc_drm_buffer(drm_fd, mesh_size, 1, 8, &g_drm_buf_yfra);
	if (ret)
		goto free_drm_buf_yint;
	printf("yfra fd:%d size:%d\n", g_drm_buf_yfra.dmabuf_fd, g_drm_buf_yfra.size);

	ret = alloc_drm_buffer(drm_fd, cfg->width, cfg->height * 3 / 2, 8, &g_drm_buf_pic_out);
	if (ret)
		goto free_drm_buf_yfra;
	printf("out pic fd:%d size:%d\n", g_drm_buf_pic_out.dmabuf_fd, g_drm_buf_pic_out.size);

    cfg->mesh_xint.dmaFd = g_drm_buf_xint.dmabuf_fd;
    cfg->mesh_xint.size = g_drm_buf_xint.size;
    cfg->mesh_xint.vir_addr = g_drm_buf_xint.map;

    cfg->mesh_xfra.dmaFd = g_drm_buf_xfra.dmabuf_fd;
    cfg->mesh_xfra.size = g_drm_buf_xfra.size;
    cfg->mesh_xfra.vir_addr = g_drm_buf_xfra.map;

    cfg->mesh_yint.dmaFd = g_drm_buf_yint.dmabuf_fd;
    cfg->mesh_yint.size = g_drm_buf_yint.size;
    cfg->mesh_yint.vir_addr = g_drm_buf_yint.map;

    cfg->mesh_yfra.dmaFd = g_drm_buf_yfra.dmabuf_fd;
    cfg->mesh_yfra.size = g_drm_buf_yfra.size;
    cfg->mesh_yfra.vir_addr = g_drm_buf_yfra.map;

    goto close_drm_fd;

free_drm_buf_pic_out:
	free_drm_buffer(drm_fd, &g_drm_buf_pic_out);
free_drm_buf_yfra:
	free_drm_buffer(drm_fd, &g_drm_buf_yfra);
free_drm_buf_yint:
	free_drm_buffer(drm_fd, &g_drm_buf_yfra);
free_drm_buf_xfra:
	free_drm_buffer(drm_fd, &g_drm_buf_xfra);
free_drm_buf_xint:
	free_drm_buffer(drm_fd, &g_drm_buf_xint);
close_drm_fd:
	close(drm_fd);

    return ret;
}

void deinit_ispfec_bufs()
{
	free_drm_buffer(-1, &g_drm_buf_pic_out);
	free_drm_buffer(-1, &g_drm_buf_yfra);
	free_drm_buffer(-1, &g_drm_buf_yfra);
	free_drm_buffer(-1, &g_drm_buf_xfra);
	free_drm_buffer(-1, &g_drm_buf_xint);
}
#endif

//restore terminal settings
void restore_terminal_settings(void)
{
    // Apply saved settings
    tcsetattr(0, TCSANOW, &oldt);
}

//make terminal read 1 char at a time
void disable_terminal_return(void)
{
    struct termios newt;

    //save terminal settings
    tcgetattr(0, &oldt);
    //init new settings
    newt = oldt;
    //change settings
    newt.c_lflag &= ~(ICANON | ECHO);
    //apply settings
    tcsetattr(0, TCSANOW, &newt);

    //make sure settings will be restored when program ends
    atexit(restore_terminal_settings);
}

char* get_dev_name(demo_context_t* ctx)
{
    if (ctx->dev_using == 1)
        return ctx->dev_name;
    else if(ctx->dev_using == 2)
        return ctx->dev_name2;
    else {
        ERR("!!!dev_using is not supported!!!");
        return NULL;
    }
}

char* get_sensor_name(demo_context_t* ctx)
{
    return ctx->sns_name;
}

void test_update_iqfile(const rk_aiq_sys_ctx_t* ctx)
{
    char iqfile[IQFILE_PATH_MAX_LEN] = {0};
    printf("\nspecial an new iqfile:\n");
    printf("\t1) /oem/etc/iqfiles/os04a10_CMK-OT1607-FV1_M12-40IRC-4MP-F16.xml\n");
    printf("\t2) /oem/etc/iqfiles/os04a10_CMK-OT1607-FV1_M12-40IRC-4MP-F16.bin\n");
    printf("\ninput 1/2 use default xml or input full path end of ENTER\n");
    fgets(iqfile, IQFILE_PATH_MAX_LEN, stdin);
    if (iqfile[0] == '1') {
        snprintf(iqfile, IQFILE_PATH_MAX_LEN, "%s",
                 "/oem/etc/iqfiles/os04a10_CMK-OT1607-FV1_M12-40IRC-4MP-F16.xml");
    } else if (iqfile[0] == '2') {
        snprintf(iqfile, IQFILE_PATH_MAX_LEN, "%s",
                 "/oem/etc/iqfiles/os04a10_CMK-OT1607-FV1_M12-40IRC-4MP-F16.bin");
    } else if (!strstr(iqfile, "xml")) {
        printf("[AIQ]input is not an valide xml:%s\n", iqfile);
        return;
    }

    printf("[AIQ] appling new iq file:%s\n", iqfile);

    rk_aiq_uapi_sysctl_updateIq(ctx, iqfile);
}

static int set_ae_onoff(const rk_aiq_sys_ctx_t* ctx, bool onoff);
void test_imgproc(const demo_context_t* demo_ctx) {

    if (demo_ctx == NULL) {
        return;
    }

    const rk_aiq_sys_ctx_t* ctx = NULL;
#if 1
    if (demo_ctx->camGroup) {
        ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }
#else
    const rk_aiq_sys_ctx_t* ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
#endif

    if (ctx == NULL)
        return;

    int key = getchar();
    int f = -1;
    printf("press key=[%c]\n", key);

    opMode_t mode;
    paRange_t range;
    expPwrLineFreq_t freq;
    rk_aiq_wb_scene_t scene;
    rk_aiq_wb_gain_t gain;
    rk_aiq_wb_cct_t ct;
    antiFlickerMode_t flicker;

    switch (key)
    {
    case '0':
        rk_aiq_uapi_setExpMode(ctx, OP_MANUAL);
        printf("set exp manual\n");
        break;
    case '.':
        rk_aiq_uapi_setExpMode(ctx, OP_AUTO);
        printf("set exp auto\n");
        break;
    case '1':
        rk_aiq_uapi_getExpMode(ctx, &mode);
        printf("exp mode=%d\n", mode);
        break;
    case '2':
        range.min = 5.0f;
        range.max = 8.0f;
        rk_aiq_uapi_setExpGainRange(ctx, &range);
        printf("set gain range\n");
        break;
    case '3':
        rk_aiq_uapi_getExpGainRange(ctx, &range);
        printf("get gain range[%f,%f]\n", range.min, range.max);
        break;
    case '4':
        range.min = 10.0f;
        range.max = 30.0f;
        rk_aiq_uapi_setExpTimeRange(ctx, &range);
        printf("set time range\n");
        break;
    case '5':
        rk_aiq_uapi_getExpTimeRange(ctx, &range);
        printf("get time range[%f,%f]\n", range.min, range.max);
        break;
    case '6':
        rk_aiq_uapi_setExpPwrLineFreqMode(ctx, EXP_PWR_LINE_FREQ_50HZ);
        printf("setExpPwrLineFreqMode 50hz\n");
        break;
    case ',':
        rk_aiq_uapi_setExpPwrLineFreqMode(ctx, EXP_PWR_LINE_FREQ_60HZ);
        printf("setExpPwrLineFreqMode 60hz\n");
        break;
    case '7':
        rk_aiq_uapi_getExpPwrLineFreqMode(ctx, &freq);
        printf("getExpPwrLineFreqMode=%d\n", freq);
        break;
    case '8':
        rk_aiq_uapi_setWBMode(ctx, OP_MANUAL);
        printf("setWBMode manual\n");
        break;
    case '/':
        rk_aiq_uapi_setWBMode(ctx, OP_AUTO);
        printf("setWBMode auto\n");
        break;
    case '9':
        rk_aiq_uapi_getWBMode(ctx, &mode);
        printf("getWBMode=%d\n", mode);
        break;
    case 'a':
        rk_aiq_uapi_lockAWB(ctx);
        printf("lockAWB\n");
        break;
    case 'b':
        rk_aiq_uapi_unlockAWB(ctx);
        printf("unlockAWB\n");
        break;
    case 'c':
        rk_aiq_uapi_setMWBScene(ctx, RK_AIQ_WBCT_TWILIGHT);
        printf("setMWBScene\n");
        break;
    case 'd':
        rk_aiq_uapi_getMWBScene(ctx, &scene);
        printf("getMWBScene=%d\n", scene);
        break;
    case 'e':
        gain.rgain = 0.5f;
        gain.grgain = 0.5f;
        gain.gbgain = 0.5f;
        gain.bgain = 0.5f;
        rk_aiq_uapi_setMWBGain(ctx, &gain);
        printf("setMWBGain\n");
        break;
    case 'f':
        rk_aiq_uapi_getMWBGain(ctx, &gain);
        printf("getMWBGain=[%f %f %f %f]\n", gain.rgain, gain.grgain, gain.gbgain, gain.bgain);
        break;
    case 'g': {
        rk_aiq_cpsl_info_t info;
        rk_aiq_uapi_sysctl_getCpsLtInfo(ctx, &info);
        printf("mode:%d, on:%d, gray:%s \n", info.mode, info.on, (info.gray ? "true" : "false"));
        printf("led:%f, ir:%f, s:%f, s_intv:%d, lsrc:%d \n", info.strength_led,
               info.strength_ir, info.sensitivity, info.sw_interval, info.lght_src);
    }
    break;
    case 'h': {
        rk_aiq_cpsl_cfg_t cfg;
        cfg.mode = RK_AIQ_OP_MODE_MANUAL;
        cfg.lght_src = RK_AIQ_CPSLS_LED;
        //cfg.gray_on = true;
        cfg.u.m.on = true;
        rk_aiq_uapi_sysctl_setCpsLtCfg(ctx, &cfg);
    }
    break;
    case 'i':
        rk_aiq_uapi_setAntiFlickerMode(ctx, ANTIFLICKER_NORMAL_MODE);
        printf("setAntiFlickerMode normal\n");
        break;
    case 'j':
        rk_aiq_uapi_setAntiFlickerMode(ctx, ANTIFLICKER_AUTO_MODE);
        printf("setAntiFlickerMode auto\n");
        break;
    case 'k':
        rk_aiq_uapi_getAntiFlickerMode(ctx, &flicker);
        printf("getAntiFlickerMode=%d\n", flicker);
        break;
    case 'l':
        rk_aiq_uapi_setLut3dstrth(ctx, 0.8);
        printf("setLut3dstrth\n");
        break;
    case 'm':
        rk_aiq_lut3d_attrib_t lut3d;
        rk_aiq_lut3d_querry_info_t lut3d_querry_info;
        rk_aiq_user_api_a3dlut_Query3dlutInfo(ctx, &lut3d_querry_info);
        rk_aiq_user_api_a3dlut_GetAttrib(ctx, &lut3d);
        if (lut3d_querry_info.lut3d_en == 0) {
            printf("bypass 3dlut\n");
        } else {
            if (lut3d.mode == RK_AIQ_LUT3D_MODE_AUTO) {
                printf("get Lut3dstrth = %f\n", lut3d.stAuto.alpha);
                printf("get Lut3d r[128] = %d, g[128] = %d, b[128] = %d\n",
                        lut3d_querry_info.look_up_table_r[128],
                        lut3d_querry_info.look_up_table_g[128],
                        lut3d_querry_info.look_up_table_b[128]);
            } else {
                if (lut3d.mode == RK_AIQ_LUT3D_MODE_MANUAL) {
                    printf("MANUAL 3DLUT\n");
                } else {
                    printf("get err Lut3d attr\n");
                }
            }
        }
        break;
    case 'n':
        rk_aiq_uapi_setCrSuppsn(ctx, 50);
        printf("setCrSuppsn\n");
        break;
    case 'o':
        rk_aiq_ccm_attrib_t ccm;
        rk_aiq_user_api_accm_GetAttrib(ctx, &ccm);
        ccm.mode = RK_AIQ_CCM_MODE_MANUAL;
        ccm.stManual.offs[0] = 1024;
        rk_aiq_user_api_accm_SetAttrib(ctx, ccm);
        printf("set mccm offs\n");
        break;
    case 'p':
        printf("set bayernr para  manual\n");
        rk_aiq_bayernr_manual_IQPara_t bayerNrPara;

        // rk_aiq_user_api_anr_GetManualBayernrIQPara(ctx, &bayerNrPara);
        bayerNrPara.enable = 1;
        bayerNrPara.filtPara = 0.3;
        bayerNrPara.luLevelVal[0] = 32.0;
        bayerNrPara.luLevelVal[1] = 32.0;
        bayerNrPara.luLevelVal[2] = 48.0;
        bayerNrPara.luLevelVal[3] = 80.0;
        bayerNrPara.luLevelVal[4] = 112.0;
        bayerNrPara.luLevelVal[5] = 144.0;
        bayerNrPara.luLevelVal[6] = 208.1;
        bayerNrPara.luLevelVal[7] = 255.0;
        bayerNrPara.luRatio[0] = 1.5;
        bayerNrPara.luRatio[1] = 1.5;
        bayerNrPara.luRatio[2] = 1.4;
        bayerNrPara.luRatio[3] = 1.39;
        bayerNrPara.luRatio[4] = 1.22;
        bayerNrPara.luRatio[5] = 1.03;
        bayerNrPara.luRatio[6] = 1.02;
        bayerNrPara.luRatio[7] = 1.0;

        bayerNrPara.fixW[0] = 0.7;
        bayerNrPara.fixW[1] = 0.6;
        bayerNrPara.fixW[2] = 0.6;
        bayerNrPara.fixW[3] = 0.4;
        bayerNrPara.lamda = 307.0;
        bayerNrPara.gauss_en = 1;
        bayerNrPara.RGainOff = 1.0;
        bayerNrPara.RGainFilp = 2.1;
        bayerNrPara.BGainOff = 0.4;
        bayerNrPara.BGainFilp = 3.2;
        bayerNrPara.edgeSoftness = 1.6;
        bayerNrPara.gaussWeight0 = 0.21;
        bayerNrPara.gaussWeight1 = 0.56;
        bayerNrPara.bilEdgeFilter = 1.0;
        bayerNrPara.bilFilterStreng = 1.0;
        bayerNrPara.bilEdgeSoftRatio = 1.0;
        bayerNrPara.bilEdgeSoft = 2.9;
        bayerNrPara.bilRegWgt = 0.8;

        rk_aiq_user_api_anr_SetManualBayernrIQPara(ctx, &bayerNrPara);
        printf("set bayernr para  manual ok\n");

         printf("get bayernr manual para \n");

        rk_aiq_bayernr_manual_IQPara_t bayerNrParaGet;

        rk_aiq_user_api_anr_GetManualBayernrIQPara(ctx, &bayerNrParaGet);

        sleep(5);
        f = memcmp(&bayerNrPara, &bayerNrParaGet,sizeof(bayerNrParaGet));
        printf("get bayernr manual para ok memcmp = %d!\n", f);
        //printf all the para
        for (int i = 0; i < 8; i++) {
            printf("bayernr luLevelVal[%d] = %f, luRatio[%d] = %f \n",
                    i,
                    bayerNrParaGet.luLevelVal[i],
                    i,
                    bayerNrParaGet.luRatio[i]);
        }
        for (int i = 0; i < 4; i++){
            printf("bayernr fixw[%d] = %f \n", bayerNrParaGet.fixW[i]);
        }
        printf("bayernr enable = %d \n", bayerNrParaGet.enable);
        printf("bayernr gauss_en = %d \n", bayerNrParaGet.gauss_en);
        printf("bayernr RGainOff = %f \n", bayerNrParaGet.RGainOff);
        printf("bayernr RGainFilp = %f \n", bayerNrParaGet.RGainFilp);
        printf("bayernr BGainOff = %f \n", bayerNrParaGet.BGainOff);
        printf("bayernr BGainFilp = %f \n", bayerNrParaGet.BGainFilp);
        printf("bayernr edgeSoftness = %f \n", bayerNrParaGet.edgeSoftness);
        printf("bayernr gaussWeight0 = %f \n", bayerNrParaGet.gaussWeight0);
        printf("bayernr gaussWeight1 = %f \n", bayerNrParaGet.gaussWeight1);
        printf("bayernr bilEdgeFilter = %f \n", bayerNrParaGet.bilEdgeFilter);
        printf("bayernr bilFilterStreng = %f \n", bayerNrParaGet.bilFilterStreng);
        printf("bayernr bilEdgeSoftRatio = %f \n", bayerNrParaGet.bilEdgeSoftRatio);
        printf("bayernr bilEdgeSoft = %f \n", bayerNrParaGet.bilEdgeSoft);
        printf("bayernr bilRegWgt = %f \n", bayerNrParaGet.bilRegWgt);
        printf("get bayernr manual para ok !\n");

        break;
    case 'q':

        rk_aiq_mfnr_manual_IQPara_t mfnrPara;

        mfnrPara.enable = 1;
        mfnrPara.back_ref_num = 1;
        mfnrPara.motion_detect_en = 1;

        for (int i = 0; i < 4;i++)
            mfnrPara.mfnr_iso.weight_limit_y[i] = 64.0;
        for (int i = 0; i < 3;i++)
            mfnrPara.mfnr_iso.weight_limit_uv[i] = 64.0;
        mfnrPara.mfnr_iso.ratio_frq[0] = 1.5;
        mfnrPara.mfnr_iso.ratio_frq[1] = 3.5;
        mfnrPara.mfnr_iso.ratio_frq[2] = 1.5;
        mfnrPara.mfnr_iso.ratio_frq[3] = 3.5;
        for (int i = 0; i < 3;i++)
            mfnrPara.mfnr_iso.luma_w_in_chroma[i] = 0.6;

        mfnrPara.mfnr_iso.noise_curve[0] =  8.4303162971622996e+013;
        mfnrPara.mfnr_iso.noise_curve[1] =  7.9640341802261105e+009;
        mfnrPara.mfnr_iso.noise_curve[2] =  2.6912484620871701e+005;
        mfnrPara.mfnr_iso.noise_curve[3] =  3.3049135399760801e+002;
        mfnrPara.mfnr_iso.noise_curve[4] =  1.8211244079810101e+001;
        mfnrPara.mfnr_iso.noise_curve_x00 = 3.6470000000000000e+003;

        mfnrPara.mfnr_iso.y_lo_noiseprofile[0] = 1.0;
        mfnrPara.mfnr_iso.y_lo_noiseprofile[1] = 0.86;
        mfnrPara.mfnr_iso.y_lo_noiseprofile[2] = 0.623;

        mfnrPara.mfnr_iso.y_lo_bfscale[0] = 0.51;
        mfnrPara.mfnr_iso.y_lo_bfscale[1] = 0.45;
        mfnrPara.mfnr_iso.y_lo_bfscale[2] = 0.32;
        mfnrPara.mfnr_iso.y_lo_bfscale[3] = 0.27;

        mfnrPara.mfnr_iso.y_hi_bfscale[0] = 0.41;
        mfnrPara.mfnr_iso.y_hi_bfscale[1] = 0.42;
        mfnrPara.mfnr_iso.y_hi_bfscale[2] = 0.53;
        mfnrPara.mfnr_iso.y_hi_bfscale[3] = 0.35;

        mfnrPara.mfnr_iso.uv_lo_bfscale[0] = 0.52;
        mfnrPara.mfnr_iso.uv_lo_bfscale[1] = 0.5;
        mfnrPara.mfnr_iso.uv_lo_bfscale[2] = 0.4;

        mfnrPara.mfnr_iso.uv_hi_bfscale[0] = 0.3;
        mfnrPara.mfnr_iso.uv_hi_bfscale[1] = 0.3;
        mfnrPara.mfnr_iso.uv_hi_bfscale[2] = 0.2;

        mfnrPara.motion.enable = 1;
        mfnrPara.motion.gainUpdFlag = 1;
        mfnrPara.motion.medEn = 0;
        mfnrPara.motion.sigmaHScale = 1;
        mfnrPara.motion.sigmaLScale = 1;
        mfnrPara.motion.lightClp    = 32;
        mfnrPara.motion.uvWeight    = 0.3;
        mfnrPara.motion.mfnrSigmaScale  = 4.0;
        mfnrPara.motion.yuvnrGainScale0 = 1.4;
        mfnrPara.motion.yuvnrGainScale1 = 1.0;
        mfnrPara.motion.yuvnrGainScale2 = 1.3;
        mfnrPara.motion.frame_limit_y   = 64.0;
        mfnrPara.motion.frame_limit_uv  = 35.0;
        mfnrPara.motion.yuvnrGainScale3 = 1.2;
        mfnrPara.motion.yuvnrGainGlb    = 1.0;
        mfnrPara.motion.yuvnrGainTH     = 1.3;
        mfnrPara.motion.mfnrSigmaScale  = 2.0;
        mfnrPara.motion.mfnrDnStr       = 1.0;
        mfnrPara.motion.mdSigmaScale    = 0.6;
        rk_aiq_user_api_anr_SetManualMfnrIQPara(ctx, &mfnrPara);
        printf("set mfnr manual para \n");
        sleep(5);

        //get
        printf("get manual mfnr all the paras  \n");
        rk_aiq_mfnr_manual_IQPara_t mfnrParaGet;
        rk_aiq_user_api_anr_GetManualMfnrIQPara(ctx, &mfnrParaGet);

        for (int i =0; i < 4; i++) {
            printf("manual mfnr ratio_frq[%d] = %f \n",
                     i, mfnrParaGet.mfnr_iso.ratio_frq[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("manual mfnr y_lo_bfscale[%d] = %f \n",
                     i, mfnrParaGet.mfnr_iso.y_lo_bfscale[i]);
        }

        for (int i =0; i < 3; i++) {
            printf("manual mfnr y_lo_noiseprofile[%d] = %f \n",
                     i, mfnrParaGet.mfnr_iso.y_lo_noiseprofile[i]);
        }
        for (int i =0; i < 3; i++) {
            printf("manual mfnr uv_lo_bfscale[%d] = %f \n",
                     i, mfnrParaGet.mfnr_iso.uv_lo_bfscale[i]);
        }
        for (int i =0; i < 3; i++) {
            printf("manual mfnr uv_hi_bfscale[%d] = %f \n",
                     i, mfnrParaGet.mfnr_iso.uv_hi_bfscale[i]);
        }
        for (int i =0; i < 3; i++) {
            printf("manual mfnr luma_w_in_chroma[%d] = %f \n",
                     i, mfnrParaGet.mfnr_iso.luma_w_in_chroma[i]);
        }
        for (int i =0; i < 5; i++) {
            printf("manual mfnr noise_curve[%d] = %f \n",
                     i, mfnrParaGet.mfnr_iso.noise_curve[i]);
        }
        printf("mfnr motion.enable= %d \n" ,mfnrParaGet.motion.enable);
        printf("mfnr motion.gainUpdFlag= %f \n" ,mfnrParaGet.motion.gainUpdFlag);
        printf("mfnr motion.medEn= %f \n" ,mfnrParaGet.motion.medEn);
        printf("mfnr motion.sigmaHScale= %f \n" ,mfnrParaGet.motion.sigmaHScale);
        printf("mfnr motion.sigmaLScale= %f \n" ,mfnrParaGet.motion.sigmaLScale);
        printf("mfnr motion.lightClp= %f \n" ,mfnrParaGet.motion.lightClp);
        printf("mfnr motion.uvWeight= %f \n" ,mfnrParaGet.motion.uvWeight);
        printf("mfnr motion.mfnrSigmaScale= %f \n" ,mfnrParaGet.motion.mfnrSigmaScale);
        printf("mfnr motion.yuvnrGainScale0= %f \n" ,mfnrParaGet.motion.yuvnrGainScale0);
        printf("mfnr motion.yuvnrGainScale1= %f \n" ,mfnrParaGet.motion.yuvnrGainScale1);
        printf("mfnr motion.yuvnrGainScale2= %f \n" ,mfnrParaGet.motion.yuvnrGainScale2);
        printf("mfnr motion.frame_limit_y= %f \n" ,mfnrParaGet.motion.frame_limit_y);
        printf("mfnr motion.frame_limit_uv= %f \n" ,mfnrParaGet.motion.frame_limit_uv);
        printf("mfnr motion.yuvnrGainScale3= %f \n" ,mfnrParaGet.motion.yuvnrGainScale3);
        printf("mfnr motion.yuvnrGainGlb= %f \n" ,mfnrParaGet.motion.yuvnrGainGlb);
        printf("mfnr motion.yuvnrGainTH= %f \n" ,mfnrParaGet.motion.yuvnrGainTH);
        printf("mfnr motion.mfnrSigmaScale= %f \n" ,mfnrParaGet.motion.mfnrSigmaScale);
        printf("mfnr motion.mfnrDnStr= %f \n" ,mfnrParaGet.motion.mfnrDnStr);
        printf("mfnr motion.mdSigmaScale= %f \n" ,mfnrParaGet.motion.mdSigmaScale);
        break;

    case '@':

        printf("set manual uvnr paras \n");
        rk_aiq_uvnr_manual_IQPara_t uvnrpara;

        uvnrpara.enbale =0;
        uvnrpara.step0_uvgrad_ratio = 100;
        uvnrpara.step1_median_ratio = 0.5;
        uvnrpara.step2_median_ratio = 0.5;
        uvnrpara.step1_bf_sigmaR = 20;
        uvnrpara.step2_bf_sigmaR = 16;
        uvnrpara.step3_bf_sigmaR = 8;
        rk_aiq_user_api_anr_SetManualUvnrIQPara(ctx, &uvnrpara);

        printf("set manual uvnr paras ok \n");
        sleep(3);

        //get all the paras
        rk_aiq_uvnr_manual_IQPara_t uvnrparaGet;

        rk_aiq_user_api_anr_GetManualUvnrIQPara(ctx, &uvnrparaGet);
        printf("uvnr enalbe = %d\n", uvnrparaGet.enbale);
        printf("uvnr paras %f %f %f %f %f %f \n",
                uvnrpara.step0_uvgrad_ratio,
                uvnrpara.step1_median_ratio,
                uvnrpara.step2_median_ratio,
                uvnrpara.step1_bf_sigmaR,
                uvnrpara.step2_bf_sigmaR,
                uvnrpara.step3_bf_sigmaR);
        break;
    case '#' :

        printf("set sharp manual paras \n");
        rk_aiq_sharp_manual_IQPara_t sharppara;
        sharppara.enable          = 1;
        sharppara.sharp_iso.hratio = 1.9;
        sharppara.sharp_iso.lratio = 0.6;
        sharppara.sharp_iso.mf_sharp_ratio  = 5.0;
        sharppara.sharp_iso.hf_sharp_ratio  = 6.0;
        sharppara.sharp_iso.pbf_gain        = 0.6;
        sharppara.sharp_iso.pbf_ratio       = 0.5;
        sharppara.sharp_iso.pbf_add         = 0.0;
        sharppara.sharp_iso.mbf_gain        = 0.7;
        sharppara.sharp_iso.mbf_add         = 2;
        sharppara.sharp_iso.hbf_add         = 1;
        sharppara.sharp_iso.hbf_ratio       = 0.7;
        sharppara.sharp_iso.hbf_gain        = 0.7;
        sharppara.sharp_iso.local_sharp_strength = 16;
        sharppara.sharp_iso.pbf_coeff_percent   = 0.6;
        sharppara.sharp_iso.rf_m_coeff_percent  = 0.6;
        sharppara.sharp_iso.rf_h_coeff_percent  = 0.6;
        sharppara.sharp_iso.hbf_coeff_percent   = 0.6;

        rk_aiq_user_api_asharp_SetManualIQPara(ctx, &sharppara);

        //get all tht sharp paras
        printf("get sharp manual paras \n");
        sleep(3);

        rk_aiq_sharp_manual_IQPara_t sharpparaGet;

        rk_aiq_user_api_asharp_GetManualIQPara(ctx, &sharpparaGet);
        printf("sharp sharp enable = %d ,edgeFilter = %d \n", sharpparaGet.enable);
        printf("sharp manual paras = \n %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n",
                sharpparaGet.sharp_iso.hratio,
                sharpparaGet.sharp_iso.lratio,
                sharpparaGet.sharp_iso.mf_sharp_ratio,
                sharpparaGet.sharp_iso.hf_sharp_ratio,
                sharpparaGet.sharp_iso.pbf_gain,
                sharpparaGet.sharp_iso.pbf_ratio,
                sharpparaGet.sharp_iso.pbf_add,
                sharpparaGet.sharp_iso.mbf_gain,
                sharpparaGet.sharp_iso.mbf_add,
                sharpparaGet.sharp_iso.hbf_add,
                sharpparaGet.sharp_iso.hbf_ratio,
                sharpparaGet.sharp_iso.hbf_gain ,
                sharpparaGet.sharp_iso.local_sharp_strength,
                sharpparaGet.sharp_iso.pbf_coeff_percent,
                sharpparaGet.sharp_iso.rf_m_coeff_percent,
                sharpparaGet.sharp_iso.rf_h_coeff_percent,
                sharpparaGet.sharp_iso.hbf_coeff_percent);

        printf("get sharp manual paras ok \n");

        printf("set sharp manual paras \n");

        // rk_aiq_edgeFilter_manual_IQPara_t edgeFilterPara;


        break;
    case 'r':
        rk_aiq_ynr_manual_IQPara_t ynrPara;
        ynrPara.enable  = 1;
        ynrPara.stYnrPara.sigma_curve[0] =  5.2418645928747404e+012;
        ynrPara.stYnrPara.sigma_curve[1] =  4.8376129534209651e+008;
        ynrPara.stYnrPara.sigma_curve[2] =  1.5800986577370335e+004;
        ynrPara.stYnrPara.sigma_curve[3] =  2.0569720083710763e+001;
        ynrPara.stYnrPara.sigma_curve[4] =  2.4685997272630630e+001;

        ynrPara.stYnrPara.ynr_lci[0] = 0.9362;
        ynrPara.stYnrPara.ynr_lci[1] = 0.6543;
        ynrPara.stYnrPara.ynr_lci[2] = 0.4559;
        ynrPara.stYnrPara.ynr_lci[3] = 0.4039;

        ynrPara.stYnrPara.ynr_lhci[0] = 0.8559;
        ynrPara.stYnrPara.ynr_lhci[1] = 0.9798;
        ynrPara.stYnrPara.ynr_lhci[2] = 0.6914;
        ynrPara.stYnrPara.ynr_lhci[3] = 0.5389;

        ynrPara.stYnrPara.ynr_hlci[0] = 0.8855;
        ynrPara.stYnrPara.ynr_hlci[1] = 0.9817;
        ynrPara.stYnrPara.ynr_hlci[2] = 0.7013;
        ynrPara.stYnrPara.ynr_hlci[3] = 0.5513;

        ynrPara.stYnrPara.ynr_hhci[0] = 0.7069;
        ynrPara.stYnrPara.ynr_hhci[1] = 1.4633;
        ynrPara.stYnrPara.ynr_hhci[2] = 1.1028;
        ynrPara.stYnrPara.ynr_hhci[3] = 0.7415;

        ynrPara.stYnrPara.lo_lumaPoint[0] = 0.0000;
        ynrPara.stYnrPara.lo_lumaPoint[1] = 32.0000;
        ynrPara.stYnrPara.lo_lumaPoint[2] = 64.0000;
        ynrPara.stYnrPara.lo_lumaPoint[3] = 128.0000;
        ynrPara.stYnrPara.lo_lumaPoint[4] = 192.0000;
        ynrPara.stYnrPara.lo_lumaPoint[5] = 256.0000;

        ynrPara.stYnrPara.lo_lumaRatio[0] = 1.0000;
        ynrPara.stYnrPara.lo_lumaRatio[1] = 1.0000;
        ynrPara.stYnrPara.lo_lumaRatio[2] = 1.5000;
        ynrPara.stYnrPara.lo_lumaRatio[3] = 1.5000;
        ynrPara.stYnrPara.lo_lumaRatio[4] = 1.0000;
        ynrPara.stYnrPara.lo_lumaRatio[5] = 1.0000;

        ynrPara.stYnrPara.lo_directionStrength = 0.2;
        ynrPara.stYnrPara.lo_bfScale[0] = 1.0000;
        ynrPara.stYnrPara.lo_bfScale[1] = 1.0000;
        ynrPara.stYnrPara.lo_bfScale[2] = 3.0000;
        ynrPara.stYnrPara.lo_bfScale[3] = 1.0000;

        ynrPara.stYnrPara.imerge_ratio = 0.1500;
        ynrPara.stYnrPara.imerge_bound = 1.2500;
        ynrPara.stYnrPara.denoise_weight[0] = 0.3000;
        ynrPara.stYnrPara.denoise_weight[1] = 0.8500;
        ynrPara.stYnrPara.denoise_weight[2] = 0.3000;
        ynrPara.stYnrPara.denoise_weight[3] = 0.3000;

        ynrPara.stYnrPara.hi_lumaPoint[0] = 0.0000;
        ynrPara.stYnrPara.hi_lumaPoint[1] = 32.0000;
        ynrPara.stYnrPara.hi_lumaPoint[2] = 64.0000;
        ynrPara.stYnrPara.hi_lumaPoint[3] = 128.0000;
        ynrPara.stYnrPara.hi_lumaPoint[4] = 192.0000;
        ynrPara.stYnrPara.hi_lumaPoint[5] = 256.0000;

        ynrPara.stYnrPara.hi_lumaRatio[0] = 2.0000;
        ynrPara.stYnrPara.hi_lumaRatio[1] = 2.5000;
        ynrPara.stYnrPara.hi_lumaRatio[2] = 1.5000;
        ynrPara.stYnrPara.hi_lumaRatio[3] = 1.5000;
        ynrPara.stYnrPara.hi_lumaRatio[4] = 1.0000;
        ynrPara.stYnrPara.hi_lumaRatio[5] = 1.0000;

        ynrPara.stYnrPara.hi_bfScale[0] = 1.2000;
        ynrPara.stYnrPara.hi_bfScale[1] = 1.1000;
        ynrPara.stYnrPara.hi_bfScale[2] = 0.8600;
        ynrPara.stYnrPara.hi_bfScale[3] = 0.8600;

        ynrPara.stYnrPara.hwith_d[0] = 6.0000;
        ynrPara.stYnrPara.hwith_d[1] = 6.0000;
        ynrPara.stYnrPara.hwith_d[2] = 4.0000;
        ynrPara.stYnrPara.hwith_d[3] = 2.0000;

        ynrPara.stYnrPara.hi_denoiseStrength = 1.2;
        ynrPara.stYnrPara.hi_detailMinAdjDnW = 0.2;
        rk_aiq_user_api_anr_SetManualYnrIQPara(ctx, &ynrPara);
        printf("set manual ynr paras \n");

        sleep(5);
        //get all the ynr paras
        rk_aiq_ynr_manual_IQPara_t ynrparaGet;

        printf("get ynr manual paras\n");
        rk_aiq_user_api_anr_GetManualYnrIQPara(ctx, &ynrparaGet);

        printf("ynr enable = %d \n", ynrparaGet.enable);
        for (int i =0; i < 5; i++) {
            printf("ynr sigma_curve[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.sigma_curve[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("ynr ynr_lci[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.ynr_lci[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("ynr ynr_lhci[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.ynr_lhci[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("ynr ynr_hlci[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.ynr_hlci[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("ynr ynr_hhci[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.ynr_hhci[i]);
        }
        for (int i =0; i < 6; i++) {
            printf("ynr lo_lumaPoint[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.lo_lumaPoint[i]);
        }
        for (int i =0; i < 6; i++) {
            printf("ynr lo_lumaRatio[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.lo_lumaRatio[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("ynr lo_bfScale[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.lo_bfScale[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("ynr denoise_weight[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.denoise_weight[i]);
        }
        for (int i =0; i < 6; i++) {
            printf("ynr hi_lumaPoint[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.hi_lumaPoint[i]);
        }
        for (int i =0; i < 6; i++) {
            printf("ynr hi_lumaRatio[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.hi_lumaRatio[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("ynr hi_bfScale[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.hi_bfScale[i]);
        }
        for (int i =0; i < 4; i++) {
            printf("ynr hwith_d[%d] = %f \n",
                     i, ynrparaGet.stYnrPara.hwith_d[i]);
        }
        printf("get ynr manual paras ok \n");
        break;
    case 's': {
        unsigned int anr_strength = 100;
        rk_aiq_uapi_setANRStrth(ctx, anr_strength);
        printf("setANRStrth: %d\n", anr_strength);
        usleep(500);
        rk_aiq_uapi_getANRStrth(ctx, &anr_strength);
        printf("getANRStrth: %d\n", anr_strength);
        break;
    }
    case 't': {
        unsigned int nr_strength = 0;
        bool nr_state = true;
        rk_aiq_uapi_setMSpaNRStrth(ctx, true, nr_strength);
        rk_aiq_uapi_setMTNRStrth(ctx, true, nr_strength);
        printf("setMSpaNRStrth and setMTNRStrth: %d\n", nr_strength);
        usleep(500);
        rk_aiq_uapi_getMTNRStrth(ctx, &nr_state, &nr_strength);
        printf("getMTNRStrth: %d\n", nr_strength);
        rk_aiq_uapi_getMSpaNRStrth(ctx, &nr_state, &nr_strength);
        printf("getMSpaNRStrth: %d\n", nr_strength);
        break;
    }
    case 'u':
        rk_aiq_uapi_setDhzMode(ctx, OP_MANUAL);
        printf("setDhzMode\n");
        break;
    case 'v':
        rk_aiq_uapi_getDhzMode(ctx, &mode);
        printf("getDhzMode=%d\n", mode);
        break;
    case 'w':
    {
        bool stat = false;
        unsigned int level4 = 0;
        rk_aiq_uapi_getMHDRStrth(ctx, &stat, &level4);
        printf("getMHDRStrth: status:%d, level=%d\n", stat, level4);
    }
    break;
    case 'x':
        rk_aiq_uapi_setMHDRStrth(ctx, true, 8);
        printf("setMHDRStrth true\n");
        break;
    case 'y':
        bool mod_en;
        rk_aiq_uapi_sysctl_getModuleCtl(ctx, RK_MODULE_TNR, &mod_en);
        printf("getModuleCtl=%d\n", mod_en);
        break;
    case 'z':
        rk_aiq_uapi_setFocusMode(ctx, OP_AUTO);
        printf("setFocusMode OP_AUTO\n");
        break;
    case 'A':
        rk_aiq_uapi_setFocusMode(ctx, OP_SEMI_AUTO);
        printf("setFocusMode OP_SEMI_AUTO\n");
        break;
    case 'B':
        rk_aiq_uapi_setFocusMode(ctx, OP_MANUAL);
        printf("setFocusMode OP_MANUAL\n");
        break;
    case 'C':
        rk_aiq_uapi_manualTrigerFocus(ctx);
        printf("manualTrigerFocus\n");
        break;
    case 'D': {
        int code;
        rk_aiq_uapi_getOpZoomPosition(ctx, &code);
        printf("getOpZoomPosition code %d\n", code);

        code += 20;
        if (code > 1520)
            code = 0;
        rk_aiq_uapi_setOpZoomPosition(ctx, code);
        rk_aiq_uapi_endOpZoomChange(ctx);
        printf("setOpZoomPosition %d\n", code);
    }
    break;
    case 'E': {
        int code;
        rk_aiq_uapi_getOpZoomPosition(ctx, &code);
        printf("getOpZoomPosition code %d\n", code);

        code -= 20;
        if (code < 0)
            code = 1520;
        rk_aiq_uapi_setOpZoomPosition(ctx, code);
        rk_aiq_uapi_endOpZoomChange(ctx);
        printf("setOpZoomPosition %d\n", code);
    }
    break;
    case 'F': {
        rk_aiq_af_focusrange range;
        short code;

        rk_aiq_uapi_getFocusRange(ctx, &range);
        printf("focus.min_pos %d, focus.max_pos %d\n", range.min_pos, range.max_pos);

        rk_aiq_uapi_getFixedModeCode(ctx, &code);

        code++;
        if (code > range.max_pos)
            code = range.max_pos;
        rk_aiq_uapi_setFixedModeCode(ctx, code);
        printf("setFixedModeCode %d\n", code);
    }
    break;
    case 'G': {
        rk_aiq_af_focusrange range;
        short code;

        rk_aiq_uapi_getFocusRange(ctx, &range);
        printf("focus.min_pos %d, focus.max_pos %d\n", range.min_pos, range.max_pos);

        rk_aiq_uapi_getFixedModeCode(ctx, &code);

        code--;
        if (code < range.min_pos)
            code = range.min_pos;
        rk_aiq_uapi_setFixedModeCode(ctx, code);
        printf("setFixedModeCode %d\n", code);
    }
    break;
    case 'H': {
        rk_aiq_af_attrib_t attr;
        uint16_t gamma_y[RKAIQ_RAWAF_GAMMA_NUM] =
        {0, 45, 108, 179, 245, 344, 409, 459, 500, 567, 622, 676, 759, 833, 896, 962, 1023};

        rk_aiq_user_api_af_GetAttrib(ctx, &attr);
        attr.manual_meascfg.contrast_af_en = 1;
        attr.manual_meascfg.rawaf_sel = 0; // normal = 0; hdr = 1

        attr.manual_meascfg.window_num = 2;
        attr.manual_meascfg.wina_h_offs = 2;
        attr.manual_meascfg.wina_v_offs = 2;
        attr.manual_meascfg.wina_h_size = 2580;
        attr.manual_meascfg.wina_v_size = 1935;

        attr.manual_meascfg.winb_h_offs = 500;
        attr.manual_meascfg.winb_v_offs = 600;
        attr.manual_meascfg.winb_h_size = 300;
        attr.manual_meascfg.winb_v_size = 300;

        attr.manual_meascfg.gamma_flt_en = 1;
        attr.manual_meascfg.gamma_y[RKAIQ_RAWAF_GAMMA_NUM];
        memcpy(attr.manual_meascfg.gamma_y, gamma_y, RKAIQ_RAWAF_GAMMA_NUM * sizeof(uint16_t));

        attr.manual_meascfg.gaus_flt_en = 1;
        attr.manual_meascfg.gaus_h0 = 0x20;
        attr.manual_meascfg.gaus_h1 = 0x10;
        attr.manual_meascfg.gaus_h2 = 0x08;

        attr.manual_meascfg.afm_thres = 4;

        attr.manual_meascfg.lum_var_shift[0] = 0;
        attr.manual_meascfg.afm_var_shift[0] = 0;
        attr.manual_meascfg.lum_var_shift[1] = 4;
        attr.manual_meascfg.afm_var_shift[1] = 4;

        attr.manual_meascfg.sp_meas.enable = true;
        attr.manual_meascfg.sp_meas.ldg_xl = 10;
        attr.manual_meascfg.sp_meas.ldg_yl = 28;
        attr.manual_meascfg.sp_meas.ldg_kl = (255 - 28) * 256 / 45;
        attr.manual_meascfg.sp_meas.ldg_xh = 118;
        attr.manual_meascfg.sp_meas.ldg_yh = 8;
        attr.manual_meascfg.sp_meas.ldg_kh = (255 - 8) * 256 / 15;
        attr.manual_meascfg.sp_meas.highlight_th = 245;
        attr.manual_meascfg.sp_meas.highlight2_th = 200;
        rk_aiq_user_api_af_SetAttrib(ctx, &attr);
    }
    break;
    case 'X':
        rk_aiq_uapi_startZoomCalib(ctx);
        break;
    case 'Y':
        rk_aiq_uapi_endOpZoomChange(ctx);
        break;
    case 'Z': {
        rk_aiq_af_result_t result;

        rk_aiq_uapi_getSearchResult(ctx, &result);
        printf("result.stat %d, result.final_pos %d\n", result.stat, result.final_pos);
    }
    break;
    case 'I':
        rk_aiq_nr_IQPara_t stNRIQPara;
        rk_aiq_nr_IQPara_t stGetNRIQPara;
        stNRIQPara.module_bits = (1 << ANR_MODULE_BAYERNR) | (1 << ANR_MODULE_MFNR) | (1 << ANR_MODULE_UVNR) | (1 << ANR_MODULE_YNR);
        stGetNRIQPara.module_bits = (1 << ANR_MODULE_BAYERNR) | (1 << ANR_MODULE_MFNR) | (1 << ANR_MODULE_UVNR) | (1 << ANR_MODULE_YNR);
        rk_aiq_user_api_anr_GetIQPara(ctx, &stNRIQPara);

        for(int m = 0; m < 3; m++) {
            for(int k = 0; k < 2; k++) {
                for(int i = 0; i < CALIBDB_NR_SHARP_MAX_ISO_LEVEL; i++ ) {
                    //bayernr
                    stNRIQPara.stBayernrPara.mode_cell[m].setting[k].filtPara[i] = 0.1;
                    stNRIQPara.stBayernrPara.mode_cell[m].setting[k].lamda = 500;
                    stNRIQPara.stBayernrPara.mode_cell[m].setting[k].fixW[0][i] = 0.1;
                    stNRIQPara.stBayernrPara.mode_cell[m].setting[k].fixW[1][i] = 0.1;
                    stNRIQPara.stBayernrPara.mode_cell[m].setting[k].fixW[2][i] = 0.1;
                    stNRIQPara.stBayernrPara.mode_cell[m].setting[k].fixW[3][i] = 0.1;

                    //mfnr
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_y[0] = 2;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_y[1] = 2;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_y[2] = 2;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_y[3] = 2;

                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_uv[0] = 2;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_uv[1] = 2;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_uv[2] = 2;

                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_lo_bfscale[0] = 0.4;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_lo_bfscale[1] = 0.6;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_lo_bfscale[2] = 0.8;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_lo_bfscale[3] = 1.0;

                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_hi_bfscale[0] = 0.4;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_hi_bfscale[1] = 0.6;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_hi_bfscale[2] = 0.8;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_hi_bfscale[3] = 1.0;

                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_lo_bfscale[0] = 0.1;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_lo_bfscale[1] = 0.2;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_lo_bfscale[2] = 0.3;

                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_hi_bfscale[0] = 0.1;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_hi_bfscale[1] = 0.2;
                    stNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_hi_bfscale[2] = 0.3;

                    //ynr
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].lo_bfScale[0] = 0.4;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].lo_bfScale[1] = 0.6;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].lo_bfScale[2] = 0.8;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].lo_bfScale[3] = 1.0;

                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_bfScale[0] = 0.4;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_bfScale[1] = 0.6;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_bfScale[2] = 0.8;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_bfScale[3] = 1.0;

                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseStrength = 1.0;

                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseWeight[0] = 1.0;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseWeight[1] = 1.0;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseWeight[2] = 1.0;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseWeight[3] = 1.0;

                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].denoise_weight[0] = 1.0;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].denoise_weight[1] = 1.0;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].denoise_weight[2] = 1.0;
                    stNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].denoise_weight[3] = 1.0;

                    //uvnr
                    stNRIQPara.stUvnrPara.mode_cell[m].setting[k].step0_uvgrad_ratio[i] = 100;
                    stNRIQPara.stUvnrPara.mode_cell[m].setting[k].step1_median_ratio[i] = 0.5;
                    stNRIQPara.stUvnrPara.mode_cell[m].setting[k].step2_median_ratio[i] = 0.5;
                    stNRIQPara.stUvnrPara.mode_cell[m].setting[k].step1_bf_sigmaR[i] = 20;
                    stNRIQPara.stUvnrPara.mode_cell[m].setting[k].step2_bf_sigmaR[i] = 16;
                    stNRIQPara.stUvnrPara.mode_cell[m].setting[k].step3_bf_sigmaR[i] = 8;

                }
            }
        }

        rk_aiq_user_api_anr_SetIQPara(ctx, &stNRIQPara);

        usleep(500);
        //printf all the para
        rk_aiq_user_api_anr_GetIQPara(ctx, &stGetNRIQPara);

        for(int m = 0; m < 1; m++) {
            for(int k = 0; k < 1; k++) {
                for(int i = 0; i < CALIBDB_NR_SHARP_MAX_ISO_LEVEL; i++ ) {
                    printf("\n\n!!!!!!!!!!set:%d cell:%d !!!!!!!!!!\n", k, i);
                    printf("oyyf222 bayernr: fiter:%f lamda:%f fixw:%f %f %f %f\n",
                           stGetNRIQPara.stBayernrPara.mode_cell[m].setting[k].filtPara[i],
                           stGetNRIQPara.stBayernrPara.mode_cell[m].setting[k].lamda,
                           stGetNRIQPara.stBayernrPara.mode_cell[m].setting[k].fixW[0][i],
                           stGetNRIQPara.stBayernrPara.mode_cell[m].setting[k].fixW[1][i],
                           stGetNRIQPara.stBayernrPara.mode_cell[m].setting[k].fixW[2][i],
                           stGetNRIQPara.stBayernrPara.mode_cell[m].setting[k].fixW[3][i]);

                    printf("oyyf222 mfnr: limiy:%f %f %f %f uv: %f %f %f, y_lo:%f %f %f %f y_hi:%f %f %f %f uv_lo:%f %f %f uv_hi:%f %f %f\n",
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_y[0],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_y[1],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_y[2],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_y[3],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_uv[0],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_uv[1],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].weight_limit_uv[2],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_lo_bfscale[0],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_lo_bfscale[1],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_lo_bfscale[2],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_lo_bfscale[3],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_hi_bfscale[0],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_hi_bfscale[1],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_hi_bfscale[2],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].y_hi_bfscale[3],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_lo_bfscale[0],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_lo_bfscale[1],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_lo_bfscale[2],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_hi_bfscale[0],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_hi_bfscale[1],
                           stGetNRIQPara.stMfnrPara.mode_cell[m].setting[k].mfnr_iso[i].uv_hi_bfscale[2]);

                    printf("oyyf222 ynr: lo_bf:%f %f %f %f  lo_do:%f %f %f %f  hi_bf:%f %f %f %f stre:%f hi_do:%f %f %f %f\n",
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].lo_bfScale[0],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].lo_bfScale[1],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].lo_bfScale[2],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].lo_bfScale[3],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].denoise_weight[0],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].denoise_weight[1],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].denoise_weight[2],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].denoise_weight[3],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_bfScale[0],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_bfScale[1],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_bfScale[2],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_bfScale[3],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseStrength,
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseWeight[0],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseWeight[1],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseWeight[2],
                           stGetNRIQPara.stYnrPara.mode_cell[m].setting[k].ynr_iso[i].hi_denoiseWeight[3]
                          );

                    printf("oyyf222 uvnr: uv:%f  med:%f %f sigmaR:%f %f %f\n",
                           stGetNRIQPara.stUvnrPara.mode_cell[m].setting[k].step0_uvgrad_ratio[i],
                           stGetNRIQPara.stUvnrPara.mode_cell[m].setting[k].step1_median_ratio[i],
                           stGetNRIQPara.stUvnrPara.mode_cell[m].setting[k].step2_median_ratio[i],
                           stGetNRIQPara.stUvnrPara.mode_cell[m].setting[k].step1_bf_sigmaR[i],
                           stGetNRIQPara.stUvnrPara.mode_cell[m].setting[k].step2_bf_sigmaR[i],
                           stGetNRIQPara.stUvnrPara.mode_cell[m].setting[k].step3_bf_sigmaR[i]);

                    printf("!!!!!!!!!!set:%d cell:%d  end !!!!!!!!!!\n\n", k, i);
                }
            }
        }
        break;
    case 'J':
        rk_aiq_sharp_IQpara_t stSharpIQpara;
        rk_aiq_sharp_IQpara_t stGetSharpIQpara;
        rk_aiq_sharp_attrib_t stGetAttrib;
        rk_aiq_sharp_attrib_t stSetAttrib;
        float sharp_strength;
        stSharpIQpara.module_bits= (1<<ASHARP_MODULE_SHARP) | (1<< ASHARP_MODULE_EDGEFILTER) ;
        rk_aiq_user_api_asharp_GetIQPara(ctx, &stSharpIQpara);

        for(int m = 0; m < 3; m++) {
            for(int k = 0; k < 2; k++) {
                for(int i = 0; i < CALIBDB_NR_SHARP_MAX_ISO_LEVEL; i++ ) {
                    stSharpIQpara.stSharpPara.mode_cell[m].setting[k].sharp_iso[i].hratio = 1.9;
                    stSharpIQpara.stSharpPara.mode_cell[m].setting[k].sharp_iso[i].lratio = 0.4;
                    stSharpIQpara.stSharpPara.mode_cell[m].setting[k].sharp_iso[i].mf_sharp_ratio = 5.0;
                    stSharpIQpara.stSharpPara.mode_cell[m].setting[k].sharp_iso[i].hf_sharp_ratio = 6.0;

                    stSharpIQpara.stEdgeFltPara.mode_cell[m].setting[k].edgeFilter_iso[i].edge_thed = 33.0;
                    stSharpIQpara.stEdgeFltPara.mode_cell[m].setting[k].edgeFilter_iso[i].local_alpha = 0.5;
                }
            }
        }

        rk_aiq_user_api_asharp_SetIQPara(ctx, &stSharpIQpara);

        usleep(500);
        rk_aiq_user_api_asharp_GetIQPara(ctx, &stGetSharpIQpara);

        for(int m = 0; m < 1; m++) {
            for(int k = 0; k < 1; k++) {
                for(int i = 0; i < CALIBDB_NR_SHARP_MAX_ISO_LEVEL; i++ ) {
                    printf("\n\n!!!!!!!!!!set:%d cell:%d !!!!!!!!!!\n", k, i);
                    printf("oyyf222 sharp:%f %f ratio:%f %f\n",
                           stGetSharpIQpara.stSharpPara.mode_cell[m].setting[k].sharp_iso[i].lratio,
                           stGetSharpIQpara.stSharpPara.mode_cell[m].setting[k].sharp_iso[i].hratio,
                           stGetSharpIQpara.stSharpPara.mode_cell[m].setting[k].sharp_iso[i].mf_sharp_ratio,
                           stGetSharpIQpara.stSharpPara.mode_cell[m].setting[k].sharp_iso[i].hf_sharp_ratio);

                    printf("oyyf222 edgefilter:%f %f\n",
                           stGetSharpIQpara.stEdgeFltPara.mode_cell[m].setting[k].edgeFilter_iso[i].edge_thed,
                           stGetSharpIQpara.stEdgeFltPara.mode_cell[m].setting[k].edgeFilter_iso[i].local_alpha);

                    printf("!!!!!!!!!!set:%d cell:%d  end !!!!!!!!!!\n", k, i);
                }
            }
        }

        rk_aiq_user_api_asharp_GetAttrib(ctx, &stGetAttrib);
        memcpy(&stSetAttrib, &stGetAttrib, sizeof(rk_aiq_sharp_attrib_t));
        stSetAttrib.stAuto.edgeFltEn = 0;
        stSetAttrib.stAuto.sharpEn = 0;
        stSetAttrib.stManual.edgeFltEn = 0;
        stSetAttrib.stManual.sharpEn = 0;
        rk_aiq_user_api_asharp_SetAttrib(ctx, &stSetAttrib);

        sharp_strength = 0.356;
        rk_aiq_user_api_asharp_SetStrength(ctx, sharp_strength);
        sleep(5);
        rk_aiq_user_api_asharp_GetStrength(ctx, &sharp_strength);
        printf("\r\n>>>>>>>>>>>get sharp strength>>>>>> %f \r\n", sharp_strength);
       break;
    case 'K':
        printf("test mirro, flip\n");
        bool mirror, flip;
        rk_aiq_uapi_getMirrorFlip(ctx, &mirror, &flip);
        printf("oringinal mir %d, flip %d \n", mirror, flip);
        mirror = true;
        flip = true;
        printf("set mir %d, flip %d \n", mirror, flip);
        rk_aiq_uapi_setMirroFlip(ctx, true, true, 3);
        rk_aiq_uapi_getMirrorFlip(ctx, &mirror, &flip);
        printf("after set mir %d, flip %d \n", mirror, flip);
        break;
    case 'L':
        printf("test fec correct level200\n");
        rk_aiq_uapi_setFecCorrectLevel(ctx, 200);
        break;
    case 'M':
       {
           for (int m = 0; m < 256; m++) {
               printf("test fec correct level %d\n", m);
               rk_aiq_uapi_setFecCorrectLevel(ctx, m);
               sleep(1);
           }
       }
       break;
    case 'N':
        {
            rk_aiq_blc_attrib_t getBlcAttrib;
            rk_aiq_blc_attrib_t setBlcAttrib;
            rk_aiq_user_api_ablc_GetAttrib(ctx, &getBlcAttrib);
            memcpy(&setBlcAttrib, &getBlcAttrib, sizeof(rk_aiq_blc_attrib_t));
            setBlcAttrib.stAuto.stParams.enable = 0;
            setBlcAttrib.stAuto.stSelect.enable = 0;
            setBlcAttrib.stManual.stSelect.enable = 0;
            rk_aiq_user_api_ablc_SetAttrib(ctx, &setBlcAttrib);
        }
        break;
    case 'O': {
        CamCalibDbContext_t* calib = rk_aiq_uapi_sysctl_getCurCalib(ctx);
        XCamReturn ret = rk_aiq_uapi_sysctl_upateCalib(ctx, calib);
        if (ret == XCAM_RETURN_NO_ERROR) {
            printf("update calib success.\n");
        } else {
            printf("update calib failed.\n");
        }
    }
    break;
    case 'P': {
        rk_aiq_working_mode_t new_mode;
        if (demo_ctx->hdrmode == RK_AIQ_WORKING_MODE_NORMAL) {
            new_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
        } else {
            new_mode = RK_AIQ_WORKING_MODE_NORMAL;
        }
        printf("switch work mode from %d to %d\n", demo_ctx->hdrmode, new_mode);
        *((rk_aiq_working_mode_t*)(&demo_ctx->hdrmode)) = new_mode;
        rk_aiq_uapi_sysctl_swWorkingModeDyn(ctx, new_mode);
    }
    break;
    case 'Q':
        printf("test enable ldch\n");
        rk_aiq_uapi_setLdchEn(ctx, true);
        break;
    case 'R':
        printf("test disalbe ldch\n");
        rk_aiq_uapi_setLdchEn(ctx, false);
        break;
    case 'S':
        printf("test ldch correct level200\n");
        rk_aiq_uapi_setLdchCorrectLevel(ctx, 200);
        break;
    case 'T':
    {
        for (int m = 0; m < 256; m++) {
            printf("test ldch correct level %d\n", m);
            rk_aiq_uapi_setLdchCorrectLevel(ctx, m);
            sleep(1);
        }
    }
    break;
    case 'U':
    {
        char output_dir[64] = {0};
        printf("test to capture raw sync\n");
        rk_aiq_uapi_debug_captureRawSync(ctx, CAPTURE_RAW_SYNC, 5, "/tmp", output_dir);
        printf("Raw's storage directory is (%s)\n", output_dir);
    }
    break;
    case 'V': {
        test_update_iqfile(ctx);
        break;
    }
    case '[':
        set_ae_onoff(ctx, true);
        printf("set ae on\n");
        break;
    case ']':
        set_ae_onoff(ctx, false);
        printf("set ae off\n");
        break;
    case '~':
        rk_aiq_ccm_attrib_t ccm2;
        rk_aiq_ccm_querry_info_t ccm_querry_info;
        rk_aiq_user_api_accm_QueryCcmInfo(ctx, &ccm_querry_info);
        rk_aiq_user_api_accm_GetAttrib(ctx, &ccm2);
        if (ccm_querry_info.ccm_en == 0) {
            printf("bypass ccm\n");
        } else {
            if(ccm2.mode == RK_AIQ_CCM_MODE_AUTO) {
                printf("getCrSuppsn=%d\n", (int)ccm_querry_info.color_inhibition_level);
            } else {
                if (ccm2.mode == RK_AIQ_CCM_MODE_MANUAL) {
                    printf("mccm=[");
                    for (int i = 0; i < 9; i++) {
                        printf("%f ", ccm2.stManual.matrix[i]);
                    }
                    printf("] \n");
                    printf("offs = [%f, %f, %f]\n ",
                        ccm2.stManual.offs[0],
                        ccm2.stManual.offs[1],
                        ccm2.stManual.offs[2]);
                } else {
                    printf("get err ccm attr\n");
                }
            }
        }
        break;
    case '$':
        rk_aiq_lsc_attrib_t lsc;
        rk_aiq_lsc_querry_info_t lsc_querry_info;
        rk_aiq_user_api_alsc_QueryLscInfo(ctx, &lsc_querry_info);
        rk_aiq_user_api_alsc_GetAttrib(ctx, &lsc);
        if (lsc_querry_info.lsc_en == 0) {
            printf("bypass lsc\n");
        } else {
            if(lsc.mode == RK_AIQ_LSC_MODE_AUTO) {
                printf("auto lsc\n");
                printf("lsc r[3] = %d, gr[3] = %d, gb[3] = %d, b[3] = %d \n",
                    lsc_querry_info.r_data_tbl[3],
                    lsc_querry_info.gr_data_tbl[3],
                    lsc_querry_info.gb_data_tbl[3],
                    lsc_querry_info.b_data_tbl[3]);
            } else {
                if (lsc.mode == RK_AIQ_LSC_MODE_MANUAL) {
                    printf("manual lsc\n ");
                    printf("mlsc r[3] = %d, gr[3] = %d, gb[3] = %d, b[3] = %d \n",
                        lsc.stManual.r_data_tbl[3],
                        lsc.stManual.gr_data_tbl[3],
                        lsc.stManual.gb_data_tbl[3],
                        lsc.stManual.b_data_tbl[3]);
                } else {
                    printf("get err lsc attr\n");
                }
            }
        }
        break;
    case '+':
        rk_ainr_param para;

        rk_aiq_uapi_sysctl_getAinrParams(ctx, &para);
        printf("rkisp_demo test for ainr params get: en=%d tuning_visual_flag: %d\n", para.enable, para.tuning_visual_flag);
        printf("gain_tab_len:%d, gain_max:%d cur_gain:%.2f \n", para.gain_tab_len, para.gain_max, para.gain);
        printf("raw mean:%.2f, yuv mean:%.2f luma_point: %.2f %.2f %.2f %.2f %.2f %.2f \n",
                para.RawMeanluma, para.YuvMeanluma, para.luma_curve_tab[0], para.luma_curve_tab[1],
                para.luma_curve_tab[2], para.luma_curve_tab[3], para.luma_curve_tab[4], para.luma_curve_tab[5]);
        for (int i = 0; i < para.gain_tab_len; i++) {
            printf("gain[%d]:%.2f, sigma[%d]:%.2f, shade[%d]:%.2f, sharp[%d]:%.2f \n",
                   i, para.gain_tab[i], i, para.sigma_tab[i], i, para.shade_tab[i],
                   i, para.sharp_tab[i]);
            printf("min_luma[%d]:%.2f, sat_scale[%d]:%.2f, dark_contrast[%d]:%.2f, ai_ratio[%d]:%.2f \n",
                   i, para.min_luma_tab[i], i, para.sat_scale_tab[i], i, para.dark_contrast_tab[i],
                   i, para.ai_ratio_tab[i]);
            printf("mot_thresh_tab[%d]:%.2f, static_thresh_tab[%d]:%.2f, mot_nr_stren_tab[%d]:%.2f \n",
                   i, para.mot_thresh_tab[i], i, para.static_thresh_tab[i], i, para.mot_nr_stren_tab[i]);
            printf("current simg curve iso_%.2f: sigma_curve: %.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", para.gain_tab[i] * 50,
                    para.sigma_curve_tab[0][i],para.sigma_curve_tab[1][i],para.sigma_curve_tab[2][i],
                    para.sigma_curve_tab[3][i],para.sigma_curve_tab[4][i],para.sigma_curve_tab[5][i]);
        }
        break;
    default:
        break;
    }
}

static void errno_exit(demo_context_t *ctx, const char *s)
{
    ERR("%s: %s error %d, %s\n", get_sensor_name(ctx), s, errno, strerror(errno));
    //exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
    int r;
    do {
        r = ioctl(fh, request, arg);
    } while (-1 == r && EINTR == errno);
    return r;
}

static bool get_value_from_file(const char* path, int* value, int* frameId)
{
    const char *delim = " ";
    char buffer[16] = {0};
    int fp;

    fp = open(path, O_RDONLY | O_SYNC);
    if (fp) {
        if (read(fp, buffer, sizeof(buffer)) > 0) {
            char *p = nullptr;

            p = strtok(buffer, delim);
            if (p != nullptr) {
                *value = atoi(p);
                p = strtok(nullptr, delim);
                if (p != nullptr)
                    *frameId = atoi(p);
            }
        }
        close(fp);
        return true;
    }

    return false;
}

static int write_yuv_to_file(const void *p,
                             int size, int sequence, demo_context_t *ctx)
{
    char file_name[64] = {0};

    snprintf(file_name, sizeof(file_name),
             "%s/frame%d.yuv",
             ctx->yuv_dir_path,
             sequence);
    ctx->fp = fopen(file_name, "wb+");
    if (ctx->fp == NULL) {
        ERR("fopen yuv file %s failed!\n", file_name);
        return -1;
    }

    fwrite(p, size, 1, ctx->fp);
    fflush(ctx->fp);

    if (ctx->fp) {
        fclose(ctx->fp);
        ctx->fp = NULL;
    }

    for (int i = 0; i < ctx->capture_yuv_num; i++)
        printf("<");

    printf("\n");
    // printf("write frame%d yuv\n", sequence);

    return 0;
}

static int creat_yuv_dir(const char* path, demo_context_t *ctx)
{
    time_t now;
    struct tm* timenow;

    if (!path)
        return -1;

    time(&now);
    timenow = localtime(&now);
    snprintf(ctx->yuv_dir_path, sizeof(ctx->yuv_dir_path),
             "%s/yuv_%04d-%02d-%02d_%02d-%02d-%02d",
             path,
             timenow->tm_year + 1900,
             timenow->tm_mon + 1,
             timenow->tm_mday,
             timenow->tm_hour,
             timenow->tm_min,
             timenow->tm_sec);

    // printf("mkdir %s for capturing yuv!\n", yuv_dir_path);

    if(mkdir(ctx->yuv_dir_path, 0755) < 0) {
        printf("mkdir %s error!!!\n", ctx->yuv_dir_path);
        return -1;
    }

    ctx->_is_yuv_dir_exist = true;

    return 0;
}

static void process_image(const void *p, int sequence, int size, demo_context_t *ctx)
{
    FILE* fp = NULL;
    if (ctx->dev_using == 1) {
        fp = ctx->fp;
    } else if (ctx->dev_using == 2) {
        fp = ctx->fp2;
    }
    if(sequence == 1000) {
        printf("==================== %s:%d dev:%d write frame:%d ==========================\n", __func__, __LINE__, ctx->dev_using, sequence);
    }
    if (fp && sequence >= ctx->skipCnt && ctx->outputCnt-- > 0) {
        printf(">\n");
        fwrite(p, size, 1, fp);
        fflush(fp);
    } else if (ctx->writeFileSync) {
        int ret = 0;
        if (!ctx->is_capture_yuv) {
            char file_name[32] = {0};
            int rawFrameId = 0;

            snprintf(file_name, sizeof(file_name), "%s/%s",
                     CAPTURE_RAW_PATH, CAPTURE_CNT_FILENAME);
            get_value_from_file(file_name, &ctx->capture_yuv_num, &rawFrameId);

            /*
             * printf("%s: rawFrameId: %d, sequence: %d\n", __FUNCTION__,
             *        rawFrameId, sequence);
             */

            sequence += 1;
            if (ctx->capture_yuv_num > 0 && \
                    ((sequence >= rawFrameId && rawFrameId > 0) || sequence < 2))
                ctx->is_capture_yuv = true;
        }

        if (ctx->is_capture_yuv) {
            if (!ctx->_is_yuv_dir_exist) {
                creat_yuv_dir(CAPTURE_RAW_PATH, ctx);
            }

            if (ctx->_is_yuv_dir_exist) {
                write_yuv_to_file(p, size, sequence, ctx);
                rk_aiq_uapi_debug_captureRawNotify(ctx->aiq_ctx);
            }

            if (ctx->capture_yuv_num-- == 0) {
                ctx->is_capture_yuv = false;
                ctx->_is_yuv_dir_exist = false;
            }
        }
    }
}

static int file_counter = 1;

// 保存一帧图像到文件
int save_frame_to_file(int fd, const char* base_filename, size_t width, size_t height) {
    // 构建文件名，只包含递增的后缀
    char filename[256];  // 适当选择足够大的缓冲区
    snprintf(filename, sizeof(filename), "%s_%d.nv12", base_filename, file_counter++);

    // 以写入二进制模式打开文件
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // 计算图像大小
    size_t image_size = width * height * 3 / 2;

    // 将图像数据映射到内存中
    void* buffer = mmap(0, image_size, PROT_READ, MAP_SHARED, fd, 0);
    if (buffer == MAP_FAILED) {
        perror("Error mapping image data");
        fclose(file);
        return -1;
    }

    // 将内存中的图像数据写入文件
    size_t bytes_written = fwrite(buffer, 1, image_size, file);
    fflush(file);
    if (bytes_written != image_size) {
        perror("Error writing image data to file");
        munmap(buffer, image_size);
        fclose(file);
        return -1;
    }

    // 取消内存映射并关闭文件
    munmap(buffer, image_size);
    fclose(file);
    usleep(10000);
    return 0;
}

static int read_frame(demo_context_t *ctx)
{
    struct v4l2_buffer buf;
    int i, bytesused;

    CLEAR(buf);

    buf.type = ctx->buf_type;
    buf.memory = V4L2_MEMORY_MMAP;

    struct v4l2_plane planes[FMT_NUM_PLANES];
    memset(planes, 0, sizeof(struct v4l2_plane)*FMT_NUM_PLANES);
    if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == ctx->buf_type) {
        buf.m.planes = planes;
        buf.length = FMT_NUM_PLANES;
    }

    if (-1 == xioctl(ctx->fd, VIDIOC_DQBUF, &buf))
        errno_exit(ctx, "VIDIOC_DQBUF");

    i = buf.index;

    if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == ctx->buf_type)
        bytesused = buf.m.planes[0].bytesused;
    else
        bytesused = buf.bytesused;

#if ISPDEMO_ENABLE_DRM
#ifdef ISPFEC_API
    int buf_fd = -1;
    void* buf_addr = NULL;

    buf_fd = ctx->buffers[i].export_fd;
    buf_addr = ctx->buffers[i].start;

    int dstFd = g_drm_buf_pic_out.dmabuf_fd;
    buf_fd = dstFd;
    buf_addr = g_drm_buf_pic_out.map;
    printf("zj add ctx->buffers[i].export_fd address: %d\n", ctx->buffers[i].export_fd);

// 在您的程序中调用保存函数
// int ret = save_frame_to_file(ctx->buffers[i].export_fd, "/data/output_file", 3840, 2160);
// if (ret != 0) {
//     printf("Failed to save frame to file\n");
//     // 处理错误
// }
    rk_ispfec_api_process(g_ispfec_ctx, ctx->buffers[i].export_fd, dstFd);

#endif
    if (ctx->vop) {
        int dispWidth, dispHeight;

        if (ctx->width > 1920)
            dispWidth = 1920;
        else
            dispWidth = ctx->width;

        if (ctx->height > 1088)
            dispHeight = 1088;
        else
            dispHeight = ctx->height;

#if ISPDEMO_ENABLE_RGA
        if (strlen(ctx->dev_name) && strlen(ctx->dev_name2)) {
            if (ctx->dev_using == 1) {
#ifdef ISPFEC_API
                display_win1(buf_addr, buf_fd,  RK_FORMAT_YCbCr_420_SP, dispWidth, dispHeight, 0);
#else
                display_win1(ctx->buffers[i].start, ctx->buffers[i].export_fd,  RK_FORMAT_YCbCr_420_SP, dispWidth, dispHeight, 0);
#endif
            } else {
#ifdef ISPFEC_API
                display_win2(buf_addr, buf_fd,  RK_FORMAT_YCbCr_420_SP, dispWidth, dispHeight, 0);
#else
                display_win2(ctx->buffers[i].start, ctx->buffers[i].export_fd,  RK_FORMAT_YCbCr_420_SP, dispWidth, dispHeight, 0);
#endif
            }
        } else {
#else
        {
#endif
            drmDspFrame(ctx->width, ctx->height, dispWidth, dispHeight, ctx->buffers[i].start,
                        DRM_FORMAT_NV12);
        }
    }
#endif

#ifdef ISPFEC_API
    process_image(buf_addr,  buf.sequence, bytesused, ctx);
#else
    process_image(ctx->buffers[i].start,  buf.sequence, bytesused, ctx);
#endif

    if (-1 == xioctl(ctx->fd, VIDIOC_QBUF, &buf))
        errno_exit(ctx, "VIDIOC_QBUF");

    return 1;
}

static int read_frame_pp_oneframe(demo_context_t *ctx)
{
    struct v4l2_buffer buf;
    struct v4l2_buffer buf_pp;
    int i, ii, bytesused;
    static int first_time = 1;

    CLEAR(buf);
    // dq one buf from isp mp
    DBG("------ dq 1 from isp mp --------------\n");
    buf.type = ctx->buf_type;
    buf.memory = V4L2_MEMORY_MMAP;

    struct v4l2_plane planes[FMT_NUM_PLANES];
    if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == ctx->buf_type) {
        buf.m.planes = planes;
        buf.length = FMT_NUM_PLANES;
    }

    if (-1 == xioctl(ctx->fd_isp_mp, VIDIOC_DQBUF, &buf))
        errno_exit(ctx, "VIDIOC_DQBUF");

    i = buf.index;

    if (first_time ) {
        DBG("------ dq 2 from isp mp --------------\n");
        if (-1 == xioctl(ctx->fd_isp_mp, VIDIOC_DQBUF, &buf))
            errno_exit(ctx, "VIDIOC_DQBUF");

        ii = buf.index;
    }

    if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == ctx->buf_type)
        bytesused = buf.m.planes[0].bytesused;
    else
        bytesused = buf.bytesused;

    // queue to ispp input
    DBG("------ queue 1 index %d to ispp input --------------\n", i);
    CLEAR(buf_pp);
    buf_pp.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    buf_pp.memory = V4L2_MEMORY_DMABUF;
    buf_pp.index = i;

    struct v4l2_plane planes_pp[FMT_NUM_PLANES];
    memset(planes_pp, 0, sizeof(planes_pp));
    buf_pp.m.planes = planes_pp;
    buf_pp.length = FMT_NUM_PLANES;
    buf_pp.m.planes[0].m.fd = ctx->buffers_mp[i].export_fd;

    if (-1 == xioctl(ctx->fd_pp_input, VIDIOC_QBUF, &buf_pp))
        errno_exit(ctx, "VIDIOC_QBUF");

    if (first_time ) {
        DBG("------ queue 2 index %d to ispp input --------------\n", ii);
        buf_pp.index = ii;
        buf_pp.m.planes[0].m.fd = ctx->buffers_mp[ii].export_fd;
        if (-1 == xioctl(ctx->fd_pp_input, VIDIOC_QBUF, &buf_pp))
            errno_exit(ctx, "VIDIOC_QBUF");
    }
    // read frame from ispp sharp/scale
    DBG("------ readframe from output --------------\n");
    read_frame(ctx);
    // dq from pp input
    DBG("------ dq 1 from ispp input--------------\n");
    if (-1 == xioctl(ctx->fd_pp_input, VIDIOC_DQBUF, &buf_pp))
        errno_exit(ctx, "VIDIOC_DQBUF");
    // queue back to mp
    DBG("------ queue 1 index %d back to isp mp--------------\n", buf_pp.index);
    buf.index = buf_pp.index;
    if (-1 == xioctl(ctx->fd_isp_mp, VIDIOC_QBUF, &buf))
        errno_exit(ctx, "VIDIOC_QBUF");

    first_time = 0;
    return 1;
}

#define XCAM_STATIC_FPS_CALCULATION(objname, count) \
    do{                                             \
        static uint32_t num_frame = 0;              \
        static struct timeval last_sys_time;        \
        static struct timeval first_sys_time;       \
        static bool b_last_sys_time_init = false;   \
        if (!b_last_sys_time_init) {                \
            gettimeofday (&last_sys_time, NULL);    \
            gettimeofday (&first_sys_time, NULL);   \
            b_last_sys_time_init = true;            \
        } else {                                    \
            if ((num_frame%count)==0) {             \
                double total, current;              \
                struct timeval cur_sys_time;        \
                gettimeofday (&cur_sys_time, NULL); \
                total = (cur_sys_time.tv_sec - first_sys_time.tv_sec)*1.0f +     \
                    (cur_sys_time.tv_usec - first_sys_time.tv_usec)/1000000.0f;  \
                current = (cur_sys_time.tv_sec - last_sys_time.tv_sec)*1.0f +    \
                    (cur_sys_time.tv_usec - last_sys_time.tv_usec)/1000000.0f;   \
                printf("%s Current fps: %.2f, Total avg fps: %.2f\n",            \
                    #objname, ((float)(count))/current, (float)num_frame/total); \
                last_sys_time = cur_sys_time;       \
            }                                       \
        }                                           \
        ++num_frame;                                \
    }while(0)

static void mainloop(demo_context_t *ctx)
{
    while ((ctx->frame_count == -1) || (ctx->frame_count-- > 0)) {
        if (ctx->pponeframe)
            read_frame_pp_oneframe(ctx);
        else
        {
            read_frame(ctx);
            XCAM_STATIC_FPS_CALCULATION(rkisp_demo, 30);
        }
    }
}

static void stop_capturing(demo_context_t *ctx)
{
    enum v4l2_buf_type type;

    type = ctx->buf_type;
    if (-1 == xioctl(ctx->fd, VIDIOC_STREAMOFF, &type))
        errno_exit(ctx, "VIDIOC_STREAMOFF");
}

static void stop_capturing_pp_oneframe(demo_context_t *ctx)
{
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    if (-1 == xioctl(ctx->fd_pp_input, VIDIOC_STREAMOFF, &type))
        errno_exit(ctx, "VIDIOC_STREAMOFF ppinput");
    type = ctx->buf_type;
    if (-1 == xioctl(ctx->fd_isp_mp, VIDIOC_STREAMOFF, &type))
        errno_exit(ctx, "VIDIOC_STREAMOFF ispmp");
}

static void start_capturing(demo_context_t *ctx)
{
    unsigned int i;
    enum v4l2_buf_type type;

    for (i = 0; i < ctx->n_buffers; ++i) {
        struct v4l2_buffer buf;

        CLEAR(buf);
        buf.type = ctx->buf_type;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == ctx->buf_type) {
            struct v4l2_plane planes[FMT_NUM_PLANES];

            buf.m.planes = planes;
            buf.length = FMT_NUM_PLANES;
        }
        if (-1 == xioctl(ctx->fd, VIDIOC_QBUF, &buf))
            errno_exit(ctx, "VIDIOC_QBUF");
    }
    type = ctx->buf_type;
    DBG("%s:-------- stream on output -------------\n", get_sensor_name(ctx));
    if (-1 == xioctl(ctx->fd, VIDIOC_STREAMON, &type))
        errno_exit(ctx, "VIDIOC_STREAMON");
}

static void start_capturing_pp_oneframe(demo_context_t *ctx)
{
    unsigned int i;
    enum v4l2_buf_type type;

    type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    DBG("%s:-------- stream on pp input -------------\n", get_sensor_name(ctx));
    if (-1 == xioctl(ctx->fd_pp_input, VIDIOC_STREAMON, &type))
        errno_exit(ctx, "VIDIOC_STREAMON pp input");

    type = ctx->buf_type;
    for (i = 0; i < ctx->n_buffers; ++i) {
        struct v4l2_buffer buf;

        CLEAR(buf);
        buf.type = ctx->buf_type;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == ctx->buf_type) {
            struct v4l2_plane planes[FMT_NUM_PLANES];

            buf.m.planes = planes;
            buf.length = FMT_NUM_PLANES;
        }
        if (-1 == xioctl(ctx->fd_isp_mp, VIDIOC_QBUF, &buf))
            errno_exit(ctx, "VIDIOC_QBUF");
    }
    DBG("%s:-------- stream on isp mp -------------\n", get_sensor_name(ctx));
    if (-1 == xioctl(ctx->fd_isp_mp, VIDIOC_STREAMON, &type))
        errno_exit(ctx, "VIDIOC_STREAMON ispmp");
}


static void uninit_device(demo_context_t *ctx)
{
    unsigned int i;
    if (ctx->n_buffers == 0)
        return;

    for (i = 0; i < ctx->n_buffers; ++i) {
        if (-1 == munmap(ctx->buffers[i].start, ctx->buffers[i].length))
            errno_exit(ctx, "munmap");

        close(ctx->buffers[i].export_fd);
    }

    free(ctx->buffers);
    ctx->n_buffers = 0;
}

static void uninit_device_pp_oneframe(demo_context_t *ctx)
{
    unsigned int i;

    for (i = 0; i < ctx->n_buffers; ++i) {
        if (-1 == munmap(ctx->buffers_mp[i].start, ctx->buffers_mp[i].length))
            errno_exit(ctx, "munmap");

        close(ctx->buffers_mp[i].export_fd);
    }

    free(ctx->buffers_mp);
}

static void init_mmap(int pp_onframe, demo_context_t *ctx)
{
    struct v4l2_requestbuffers req;
    int fd_tmp = -1;

    CLEAR(req);

    if (pp_onframe)
        fd_tmp = ctx->fd_isp_mp ;
    else
        fd_tmp = ctx->fd;

    req.count = BUFFER_COUNT;
    req.type = ctx->buf_type;
    req.memory = V4L2_MEMORY_MMAP;

    struct buffer *tmp_buffers = NULL;

    if (-1 == xioctl(fd_tmp, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            ERR("%s: %s does not support "
                "memory mapping\n", get_sensor_name(ctx), get_dev_name(ctx));
            //exit(EXIT_FAILURE);
        } else {
            errno_exit(ctx, "VIDIOC_REQBUFS");
        }
    }

    if (req.count < 2) {
        ERR("%s: Insufficient buffer memory on %s\n", get_sensor_name(ctx),
            get_dev_name(ctx));
        //exit(EXIT_FAILURE);
    }

    tmp_buffers = (struct buffer*)calloc(req.count, sizeof(struct buffer));

    if (!tmp_buffers) {
        ERR("%s: Out of memory\n", get_sensor_name(ctx));
        //exit(EXIT_FAILURE);
    }

    if (pp_onframe)
        ctx->buffers_mp = tmp_buffers;
    else
        ctx->buffers = tmp_buffers;

    for (ctx->n_buffers = 0; ctx->n_buffers < req.count; ++ctx->n_buffers) {
        struct v4l2_buffer buf;
        struct v4l2_plane planes[FMT_NUM_PLANES];
        CLEAR(buf);
        CLEAR(planes);

        buf.type = ctx->buf_type;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = ctx->n_buffers;

        if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == ctx->buf_type) {
            buf.m.planes = planes;
            buf.length = FMT_NUM_PLANES;
        }

        if (-1 == xioctl(fd_tmp, VIDIOC_QUERYBUF, &buf))
            errno_exit(ctx, "VIDIOC_QUERYBUF");

        if (V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE == ctx->buf_type) {
            tmp_buffers[ctx->n_buffers].length = buf.m.planes[0].length;
            tmp_buffers[ctx->n_buffers].start =
                mmap(NULL /* start anywhere */,
                     buf.m.planes[0].length,
                     PROT_READ | PROT_WRITE /* required */,
                     MAP_SHARED /* recommended */,
                     fd_tmp, buf.m.planes[0].m.mem_offset);
        } else {
            tmp_buffers[ctx->n_buffers].length = buf.length;
            tmp_buffers[ctx->n_buffers].start =
                mmap(NULL /* start anywhere */,
                     buf.length,
                     PROT_READ | PROT_WRITE /* required */,
                     MAP_SHARED /* recommended */,
                     fd_tmp, buf.m.offset);
        }

        if (MAP_FAILED == tmp_buffers[ctx->n_buffers].start)
            errno_exit(ctx, "mmap");

        // export buf dma fd
        struct v4l2_exportbuffer expbuf;
        xcam_mem_clear (expbuf);
        expbuf.type = ctx->buf_type;
        expbuf.index = ctx->n_buffers;
        expbuf.flags = O_CLOEXEC;
        if (xioctl(fd_tmp, VIDIOC_EXPBUF, &expbuf) < 0) {
            errno_exit(ctx, "get dma buf failed\n");
        } else {
            DBG("%s: get dma buf(%d)-fd: %d\n", get_sensor_name(ctx), ctx->n_buffers, expbuf.fd);
        }
        tmp_buffers[ctx->n_buffers].export_fd = expbuf.fd;
    }
}

static void init_input_dmabuf_oneframe(demo_context_t *ctx) {
    struct v4l2_requestbuffers req;

    CLEAR(req);

    printf("%s:-------- request pp input buffer -------------\n", get_sensor_name(ctx));
    req.count = BUFFER_COUNT;
    req.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
    req.memory = V4L2_MEMORY_DMABUF;

    if (-1 == xioctl(ctx->fd_pp_input, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            ERR("does not support "
                "DMABUF\n");
            exit(EXIT_FAILURE);
        } else {
            errno_exit(ctx, "VIDIOC_REQBUFS");
        }
    }

    if (req.count < 2) {
        ERR("Insufficient buffer memory on %s\n",
            get_dev_name(ctx));
        exit(EXIT_FAILURE);
    }
    printf("%s:-------- request isp mp buffer -------------\n", get_sensor_name(ctx));
    init_mmap(true, ctx);
}

static void init_device(demo_context_t *ctx)
{
    struct v4l2_capability cap;
    struct v4l2_format fmt;

    if (-1 == xioctl(ctx->fd, VIDIOC_QUERYCAP, &cap)) {
        if (EINVAL == errno) {
            ERR("%s: %s is no V4L2 device\n", get_sensor_name(ctx),
                get_dev_name(ctx));
            //exit(EXIT_FAILURE);
        } else {
            errno_exit(ctx, "VIDIOC_QUERYCAP");
        }
    }

    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) &&
            !(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)) {
        ERR("%s: %s is not a video capture device, capabilities: %x\n",
            get_sensor_name(ctx), get_dev_name(ctx), cap.capabilities);
        //exit(EXIT_FAILURE);
    }

    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
        ERR("%s: %s does not support streaming i/o\n", get_sensor_name(ctx),
            get_dev_name(ctx));
        //exit(EXIT_FAILURE);
    }

    if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) {
        ctx->buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        CLEAR(fmt);
        fmt.type = ctx->buf_type;
        fmt.fmt.pix.width = ctx->width;
        fmt.fmt.pix.height = ctx->height;
        fmt.fmt.pix.pixelformat = ctx->format;
        fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
        if (ctx->limit_range)
            fmt.fmt.pix.quantization = V4L2_QUANTIZATION_LIM_RANGE;
        else
            fmt.fmt.pix.quantization = V4L2_QUANTIZATION_FULL_RANGE;
    } else if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE) {
        ctx->buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        CLEAR(fmt);
        fmt.type = ctx->buf_type;
        fmt.fmt.pix_mp.width = ctx->width;
        fmt.fmt.pix_mp.height = ctx->height;
        fmt.fmt.pix_mp.pixelformat = ctx->format;
        fmt.fmt.pix_mp.field = V4L2_FIELD_INTERLACED;
        if (ctx->limit_range)
            fmt.fmt.pix_mp.quantization = V4L2_QUANTIZATION_LIM_RANGE;
        else
            fmt.fmt.pix_mp.quantization = V4L2_QUANTIZATION_FULL_RANGE;
    }

    if (-1 == xioctl(ctx->fd, VIDIOC_S_FMT, &fmt))
        errno_exit(ctx, "VIDIOC_S_FMT");

    init_mmap(false, ctx);
}

static void init_device_pp_oneframe(demo_context_t *ctx)
{
    // TODO, set format and link, now do with setup_link.sh
    init_input_dmabuf_oneframe(ctx);
}

static void close_device(demo_context_t *ctx)
{
    if (-1 == close(ctx->fd))
        errno_exit(ctx, "close");

    ctx->fd = -1;
}

static void open_device(demo_context_t *ctx)
{
    printf("-------- open output dev_name:%s -------------\n", get_dev_name(ctx));
    ctx->fd = open(get_dev_name(ctx), O_RDWR /* required */ /*| O_NONBLOCK*/, 0);

    if (-1 == ctx->fd) {
        ERR("Cannot open '%s': %d, %s\n",
            get_dev_name(ctx), errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

static void close_device_pp_oneframe(demo_context_t *ctx)
{
    if (-1 == close(ctx->fd_pp_input))
        errno_exit(ctx, "close");

    ctx->fd_pp_input = -1;

    if (-1 == close(ctx->fd_isp_mp))
        errno_exit(ctx, "close");

    ctx->fd_isp_mp = -1;
}

static void open_device_pp_oneframe(demo_context_t *ctx)
{
    printf("-------- open pp input(video13) -------------\n");
    ctx->fd_pp_input = open("/dev/video13", O_RDWR /* required */ /*| O_NONBLOCK*/, 0);

    if (-1 == ctx->fd_pp_input) {
        ERR("Cannot open '%s': %d, %s\n",
            get_dev_name(ctx), errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("-------- open isp mp(video0) -------------\n");
    ctx->fd_isp_mp = open("/dev/video0", O_RDWR /* required */ /*| O_NONBLOCK*/, 0);

    if (-1 == ctx->fd_isp_mp ) {
        ERR("Cannot open '%s': %d, %s\n",
            get_dev_name(ctx), errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

static void uninit_device_pp_onframe(demo_context_t *ctx)
{
    unsigned int i;

    for (i = 0; i < ctx->n_buffers; ++i) {
        if (-1 == munmap(ctx->buffers_mp[i].start, ctx->buffers_mp[i].length))
            errno_exit(ctx, "munmap");
        close(ctx->buffers_mp[i].export_fd);
    }

    free(ctx->buffers_mp);
}

static void parse_args(int argc, char **argv, demo_context_t *ctx)
{
    int c;
    int digit_optind = 0;
    optind = 0;
    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"width",    required_argument, 0, 'w' },
            {"height",   required_argument, 0, 'h' },
            {"format",   required_argument, 0, 'f' },
            {"device",   required_argument, 0, 'd' },
            {"device2",   required_argument, 0, 'i' },
            {"stream-to",   required_argument, 0, 'o' },
            {"stream-tj",   required_argument, 0, 'j' },
            {"stream-count",   required_argument, 0, 'n' },
            {"stream-skip",   required_argument, 0, 'k' },
            {"count",    required_argument, 0, 'c' },
            {"help",     no_argument,       0, 'p' },
            {"silent",   no_argument,       0, 's' },
            {"vop",   no_argument,       0, 'v' },
            {"rkaiq",   no_argument,       0, 'r' },
            {"pponeframe",   no_argument,       0, 'm' },
            {"hdr",   required_argument,       0, 'a' },
            {"sync-to-raw", no_argument, 0, 'e' },
            {"limit", no_argument, 0, 'l' },
            {"ctl", required_argument, 0, 't' },
            {"iqpath", required_argument, 0, '1' },
            //{"sensor",   required_argument,       0, 'b' },
            {"camgroup",   no_argument,       0, '3' },
            {0,          0,                 0,  0  }
        };

        //c = getopt_long(argc, argv, "w:h:f:i:d:o:c:ps",
        c = getopt_long(argc, argv, "w:h:f:i:d:o:j:c:n:k:a:t:1:mpsevrl",
                        long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 'c':
            ctx->frame_count = atoi(optarg);
            break;
        case 'w':
            ctx->width = atoi(optarg);
            break;
        case 'h':
            ctx->height = atoi(optarg);
            break;
        case 'f':
            ctx->format = v4l2_fourcc(optarg[0], optarg[1], optarg[2], optarg[3]);
            break;
        case 'd':
            strcpy(ctx->dev_name, optarg);
            break;
        case 'i':
            strcpy(ctx->dev_name2, optarg);
            break;
        case 'o':
            strcpy(ctx->out_file, optarg);
            ctx->writeFile = 1;
            break;
        case 'j':
            strcpy(ctx->out_file2, optarg);
            ctx->writeFile = 1;
            break;
        case 'n':
            ctx->outputCnt = atoi(optarg);
            break;
        case 'k':
            ctx->skipCnt = atoi(optarg);
            break;
        case 's':
            silent = 1;
            break;
        case 'v':
            ctx->vop = 1;
            break;
        case 'r':
            ctx->rkaiq = 1;
            break;
        case 'm':
            ctx->pponeframe = 1;
            break;
        case 'a':
            ctx->hdrmode = atoi(optarg);
            break;
        case 'e':
            ctx->writeFileSync = 1;
            break;
        case 'l':
            ctx->limit_range = 1;
        case '1':
            strcpy(ctx->iqpath, optarg);
            break;
        case 't':
            ctx->ctl_type = atoi(optarg);
            break;
        case '3':
            ctx->camGroup = true;
            break;
        case '?':
        case 'p':
            ERR("Usage: %s to capture rkisp1 frames\n"
                "         --width,  default 640,             optional, width of image\n"
                "         --height, default 480,             optional, height of image\n"
                "         --format, default NV12,            optional, fourcc of format\n"
                "         --count,  default 1000,            optional, how many frames to capture\n"
                "         --device,                          required, path of video device1\n"
                "         --device2,                         required, path of video device2\n"
                "         --stream-to,                       optional, output file path, if <file> is '-', then the data is written to stdout\n"
                "         --stream-count, default 3           optional, how many frames to write files\n"
                "         --stream-skip, default 30           optional, how many frames to skip befor writing file\n"
                "         --vop,                             optional, drm display\n"
                "         --rkaiq,                           optional, auto image quality\n"
                "         --silent,                          optional, subpress debug log\n"
                "         --pponeframe,                      optional, pp oneframe readback mode\n"
                "         --hdr <val>,                       optional, hdr mode, val 2 means hdrx2, 3 means hdrx3 \n"
                "         --sync-to-raw,                     optional, write yuv files in sync with raw\n"
                "         --limit,                           optional, yuv limit range\n",
                "         --ctl <val>,                       optional, sysctl procedure test \n",
                "         --iqpath <val>,                    optional, absolute path of iq file dir \n",
                "         --sensor,  default os04a10,        optional, sensor names\n",
                "         --camgroup,                        optional, run in group mode\n",
                argv[0]);
            exit(-1);

        default:
            ERR("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (strlen(ctx->dev_name) == 0) {
        ERR("%s: arguments --output and --device are required\n", get_sensor_name(ctx));
        //exit(-1);
    }

}

static void deinit(demo_context_t *ctx)
{
    if (!ctx->camgroup_ctx)
        stop_capturing(ctx);

    if (ctx->pponeframe)
        stop_capturing_pp_oneframe(ctx);
    if (ctx->aiq_ctx) {
        printf("%s:-------- stop aiq -------------\n", get_sensor_name(ctx));
        rk_aiq_uapi_sysctl_stop(ctx->aiq_ctx, false);
    } else if (ctx->camgroup_ctx) {
        if (ctx->dev_using == 1) {
            printf("%s:-------- stop aiq camgroup -------------\n", get_sensor_name(ctx));
            rk_aiq_uapi_camgroup_stop(ctx->camgroup_ctx);
        }
    }

    stop_capturing(ctx);
    if (ctx->aiq_ctx) {
        printf("%s:-------- deinit aiq -------------\n", get_sensor_name(ctx));
#ifdef CUSTOM_AE_DEMO_TEST
        //rk_aiq_AELibunRegCallBack(ctx->aiq_ctx, 0);
        rk_aiq_uapi_customAE_unRegister(ctx->aiq_ctx);
#endif
        rk_aiq_uapi_sysctl_deinit(ctx->aiq_ctx);
        printf("%s:-------- deinit aiq end -------------\n", get_sensor_name(ctx));
    } else if (ctx->camgroup_ctx) {
        if (ctx->dev_using == 1) {
            printf("%s:-------- deinit aiq camgroup -------------\n", get_sensor_name(ctx));
            rk_aiq_uapi_camgroup_destroy(ctx->camgroup_ctx);
            ctx->camgroup_ctx = NULL;
            printf("%s:-------- deinit aiq camgroup end -------------\n", get_sensor_name(ctx));
        }
    }
#ifdef ISPFEC_API
    rk_ispfec_api_deinit(g_ispfec_ctx);
    g_ispfec_ctx = NULL;
#endif
    uninit_device(ctx);
    if (ctx->pponeframe)
        uninit_device_pp_oneframe(ctx);
    close_device(ctx);
    if (ctx->pponeframe)
        close_device_pp_oneframe(ctx);

    if (ctx->fp)
        fclose(ctx->fp);
}
static void signal_handle(int signo)
{
    printf("force exit signo %d !!!\n", signo);

    if (g_main_ctx) {
        g_main_ctx->frame_count = 0;
        stop_capturing(g_main_ctx);
        if (g_main_ctx->camGroup && g_second_ctx)
            stop_capturing(g_second_ctx);
        deinit(g_main_ctx);
        g_main_ctx = NULL;
    }
    if (g_second_ctx) {
        g_second_ctx->frame_count = 0;
        deinit(g_second_ctx);
        g_second_ctx = NULL;
    }
    exit(0);
}

static void* test_thread(void* args) {
    pthread_detach (pthread_self());
    disable_terminal_return();
    printf("begin test imgproc\n");
    while(1) {
        test_imgproc((demo_context_t*) args);
#ifdef SMART_IR_SAMPLE_TEST
        sample_print_smartIr_info(args);
        sample_smartIr_module(args);
#endif
    }
    printf("end test imgproc\n");
    restore_terminal_settings();
    return 0;
}

static void* test_offline_thread(void* args) {
    pthread_detach (pthread_self());
    const demo_context_t* demo_ctx = (demo_context_t*) args;
    rkraw_frame_info_t frame_info;
    for(int i = 0; i < 10; i++) {
        rk_aiq_uapi_sysctl_enqueueRkRawFile(demo_ctx->aiq_ctx, "/tmp/frame_2688x1520");
        usleep(500000);
        rk_aiq_uapi_sysctl_getRkRawFrameInfo(demo_ctx->aiq_ctx, &frame_info);
    }
    return 0;
}

static int set_ae_onoff(const rk_aiq_sys_ctx_t* ctx, bool onoff)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttr_t expSwAttr;

    ret = rk_aiq_user_api_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.enable = onoff;
    ret = rk_aiq_user_api_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}

static int query_ae_state(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpQueryInfo_t queryInfo;

    ret = rk_aiq_user_api_ae_queryExpResInfo(ctx, &queryInfo);
    printf("ae IsConverged: %d\n", queryInfo.IsConverged);

    return 0;
}

static void set_af_manual_meascfg(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_attrib_t attr;
    uint16_t gamma_y[RKAIQ_RAWAF_GAMMA_NUM] =
    {0, 45, 108, 179, 245, 344, 409, 459, 500, 567, 622, 676, 759, 833, 896, 962, 1023};

    rk_aiq_user_api_af_GetAttrib(ctx, &attr);
    attr.AfMode = RKAIQ_AF_MODE_FIXED;

    attr.manual_meascfg.contrast_af_en = 1;
    attr.manual_meascfg.rawaf_sel = 0; // normal = 0; hdr = 1

    attr.manual_meascfg.window_num = 2;
    attr.manual_meascfg.wina_h_offs = 2;
    attr.manual_meascfg.wina_v_offs = 2;
    attr.manual_meascfg.wina_h_size = 2580;
    attr.manual_meascfg.wina_v_size = 1935;

    attr.manual_meascfg.winb_h_offs = 1146;
    attr.manual_meascfg.winb_v_offs = 972;
    attr.manual_meascfg.winb_h_size = 300;
    attr.manual_meascfg.winb_v_size = 300;

    attr.manual_meascfg.gamma_flt_en = 1;
    attr.manual_meascfg.gamma_y[RKAIQ_RAWAF_GAMMA_NUM];
    memcpy(attr.manual_meascfg.gamma_y, gamma_y, RKAIQ_RAWAF_GAMMA_NUM * sizeof(uint16_t));

    attr.manual_meascfg.gaus_flt_en = 1;
    attr.manual_meascfg.gaus_h0 = 0x20;
    attr.manual_meascfg.gaus_h1 = 0x10;
    attr.manual_meascfg.gaus_h2 = 0x08;

    attr.manual_meascfg.afm_thres = 4;

    attr.manual_meascfg.lum_var_shift[0] = 0;
    attr.manual_meascfg.afm_var_shift[0] = 0;
    attr.manual_meascfg.lum_var_shift[1] = 4;
    attr.manual_meascfg.afm_var_shift[1] = 4;

    attr.manual_meascfg.sp_meas.enable = true;
    attr.manual_meascfg.sp_meas.ldg_xl = 10;
    attr.manual_meascfg.sp_meas.ldg_yl = 28;
    attr.manual_meascfg.sp_meas.ldg_kl = (255 - 28) * 256 / 45;
    attr.manual_meascfg.sp_meas.ldg_xh = 118;
    attr.manual_meascfg.sp_meas.ldg_yh = 8;
    attr.manual_meascfg.sp_meas.ldg_kh = (255 - 8) * 256 / 15;
    attr.manual_meascfg.sp_meas.highlight_th = 245;
    attr.manual_meascfg.sp_meas.highlight2_th = 200;
    rk_aiq_user_api_af_SetAttrib(ctx, &attr);
}

static void print_af_stats(rk_aiq_isp_stats_t *stats_ref)
{
    unsigned long sof_time;

    if (stats_ref->frame_id % 30 != 0)
        return;

    sof_time = stats_ref->af_stats.sof_tim / 1000000LL;
    printf("sof_tim %ld, sharpness roia: 0x%llx-0x%08x roib: 0x%x-0x%08x\n",
           sof_time,
           stats_ref->af_stats.roia_sharpness,
           stats_ref->af_stats.roia_luminance,
           stats_ref->af_stats.roib_sharpness,
           stats_ref->af_stats.roib_luminance);

    printf("global_sharpness\n");
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printf("0x%08x, ", stats_ref->af_stats.global_sharpness[15 * i + j]);
        }
        printf("\n");
    }
    printf("lowpass_fv4_4\n");
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printf("0x%08x, ", stats_ref->af_stats.lowpass_fv4_4[15 * i + j]);
        }
        printf("\n");
    }
    printf("lowpass_fv8_8\n");
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printf("0x%08x, ", stats_ref->af_stats.lowpass_fv8_8[15 * i + j]);
        }
        printf("\n");
    }
    printf("lowpass_highlht\n");
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printf("0x%08x, ", stats_ref->af_stats.lowpass_highlht[15 * i + j]);
        }
        printf("\n");
    }
    printf("lowpass_highlht2\n");
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            printf("0x%08x, ", stats_ref->af_stats.lowpass_highlht2[15 * i + j]);
        }
        printf("\n");
    }
}
static void* stats_thread(void* args) {
    demo_context_t* ctx =  (demo_context_t*)args;
    XCamReturn ret;
    pthread_detach (pthread_self());
    printf("begin stats thread\n");

    set_af_manual_meascfg(ctx->aiq_ctx);
    while(1) {
        rk_aiq_isp_stats_t *stats_ref = NULL;
        ret = rk_aiq_uapi_sysctl_get3AStatsBlk(ctx->aiq_ctx, &stats_ref, -1);
        if (ret == XCAM_RETURN_NO_ERROR && stats_ref != NULL) {
            printf("get one stats frame id %d \n", stats_ref->frame_id);
            query_ae_state(ctx->aiq_ctx);
            print_af_stats(stats_ref);
            rk_aiq_uapi_sysctl_release3AStatsRef(ctx->aiq_ctx, stats_ref);
        } else {
            if (ret == XCAM_RETURN_NO_ERROR) {
                printf("aiq has stopped !\n");
                break;
            } else if (ret == XCAM_RETURN_ERROR_TIMEOUT) {
                printf("aiq timeout!\n");
                continue;
            } else if (ret == XCAM_RETURN_ERROR_FAILED) {
                printf("aiq failed!\n");
                break;
            }
        }
    }
    printf("end stats thread\n");
    return 0;
}

void release_buffer(void *addr) {
    printf("release buffer called: addr=%p\n", addr);
}

static bool ldch_read_lut_from_file(const char* fileName, void** addr, uint32_t* size)
{
    FILE* ofp;
    ofp = fopen(fileName, "rb");
    if (ofp != NULL) {
        unsigned short hpic, vpic, hsize, vsize, hstep, vstep = 0;

        fread(&hpic, sizeof(unsigned short), 1, ofp);
        fread(&vpic, sizeof(unsigned short), 1, ofp);
        fread(&hsize, sizeof(unsigned short), 1, ofp);
        fread(&vsize, sizeof(unsigned short), 1, ofp);
        fread(&hstep, sizeof(unsigned short), 1, ofp);
        fread(&vstep, sizeof(unsigned short), 1, ofp);
        printf("lut info: [%d-%d-%d-%d-%d-%d]\n", hpic, vpic, hsize, vsize, hstep, vstep);
        fseek(ofp, 0, SEEK_SET);

        *size = hsize * vsize * sizeof(unsigned short) + 12;
        *addr = malloc(*size);
        if (*addr) {
            unsigned int num = fread(*addr, 1, *size, ofp);
            printf("check lut file, size: %u, num: %d\n", *size, num);
        }
        fclose(ofp);
    } else {
        printf("lut calib file %s not exist\n", fileName);
        return false;
    }

    return true;
}

static void rkisp_routine(demo_context_t *ctx)
{
    char sns_entity_name[64];
    rk_aiq_working_mode_t work_mode = RK_AIQ_WORKING_MODE_NORMAL;

    if (ctx->hdrmode == 2)
        work_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
    else if (ctx->hdrmode == 3)
        work_mode = RK_AIQ_WORKING_MODE_ISP_HDR3;

    printf("work_mode %d\n", work_mode);

    strcpy(sns_entity_name, rk_aiq_uapi_sysctl_getBindedSnsEntNmByVd(get_dev_name(ctx)));
    printf("sns_entity_name:%s\n", sns_entity_name);
    sscanf(&sns_entity_name[6], "%s", ctx->sns_name);
    printf("sns_name:%s\n", ctx->sns_name);
    rk_aiq_static_info_t s_info;
    rk_aiq_uapi_sysctl_getStaticMetas(sns_entity_name, &s_info);
    // check if hdr mode is supported
    if (work_mode != 0) {
        bool b_work_mode_supported = false;
        rk_aiq_sensor_info_t* sns_info = &s_info.sensor_info;
        for (int i = 0; i < SUPPORT_FMT_MAX; i++)
            // TODO, should decide the resolution firstly,
            // then check if the mode is supported on this
            // resolution
            if ((sns_info->support_fmt[i].hdr_mode == 5/*HDR_X2*/ &&
                    work_mode == RK_AIQ_WORKING_MODE_ISP_HDR2) ||
                    (sns_info->support_fmt[i].hdr_mode == 6/*HDR_X3*/ &&
                     work_mode == RK_AIQ_WORKING_MODE_ISP_HDR3)) {
                b_work_mode_supported = true;
                break;
            }

        if (!b_work_mode_supported) {
            printf("\nWARNING !!!"
                   "work mode %d is not supported, changed to normal !!!\n\n",
                   work_mode);
            work_mode = RK_AIQ_WORKING_MODE_NORMAL;
        }
    }

    printf("%s:-------- open output dev -------------\n", get_sensor_name(ctx));
    open_device(ctx);
    if (ctx->pponeframe)
        open_device_pp_oneframe(ctx);

    if (ctx->writeFile) {
        if (ctx->dev_using == 1) {
            printf(">>>>>>>>>>> openfile :%s \n", ctx->out_file);
            ctx->fp = fopen(ctx->out_file, "w+");
            if (ctx->fp == NULL) {
                ERR("%s: fopen output file %s failed!\n", get_sensor_name(ctx), ctx->out_file);
            }
        } else if (ctx->dev_using == 2) {
            strcpy(ctx->out_file2, "/data/2.yuv");
            printf(">>>>>>>>>>> openfile2 :%s \n", ctx->out_file2);
            ctx->fp2 = fopen(ctx->out_file2, "w+");
            if (ctx->fp2 == NULL) {
                ERR("%s: fopen output file %s failed!\n", get_sensor_name(ctx), ctx->out_file2);
            }
        }
    }

    if (ctx->rkaiq) {
        if (strlen(ctx->iqpath)) {
            if (!ctx->camGroup) {
                ctx->aiq_ctx = rk_aiq_uapi_sysctl_init(sns_entity_name, ctx->iqpath, NULL, NULL);
            } else {
                if (ctx->dev_using == 1) {
                    char sns_entity_name2[64] = {'\0'};
                    bool has_dev2 = false;
                    if (strlen(ctx->dev_name2)) {
                        strcpy(sns_entity_name2, rk_aiq_uapi_sysctl_getBindedSnsEntNmByVd(ctx->dev_name2));
                        printf("sns_entity_name2:%s\n", sns_entity_name2);
                        has_dev2 = true;
                    }

                    rk_aiq_camgroup_instance_cfg_t camgroup_cfg;
                    memset(&camgroup_cfg, 0, sizeof(camgroup_cfg));
                    camgroup_cfg.sns_num = 1;
                    if (has_dev2) {
                        camgroup_cfg.sns_num++;
                    }
                    camgroup_cfg.sns_ent_nm_array[0] = sns_entity_name;
                    if (has_dev2) {
                        camgroup_cfg.sns_ent_nm_array[1] = sns_entity_name2;
                    }
                    camgroup_cfg.config_file_dir = ctx->iqpath;
                    ctx->camgroup_ctx = rk_aiq_uapi_camgroup_create(&camgroup_cfg);
                    if (!ctx->camgroup_ctx) {
                        printf("create camgroup ctx error !\n");
                        exit(1);
                    }
                }
            }
        } else {
            if (ctx->camGroup) {
                printf("error! should specify iq path !\n");
                exit(1);
            }

            ctx->aiq_ctx = rk_aiq_uapi_sysctl_init(sns_entity_name, "/oem/etc/iqfiles", NULL, NULL);
        }

        if (ctx->aiq_ctx) {
            printf("%s:-------- init mipi tx/rx -------------\n", get_sensor_name(ctx));
            if (ctx->writeFileSync)
                rk_aiq_uapi_debug_captureRawYuvSync(ctx->aiq_ctx, CAPTURE_RAW_AND_YUV_SYNC);
#ifdef CUSTOM_AE_DEMO_TEST
            //ae_reg.stAeExpFunc.pfn_ae_init = ae_init;
            //ae_reg.stAeExpFunc.pfn_ae_run = ae_run;
            //ae_reg.stAeExpFunc.pfn_ae_ctrl = ae_ctrl;
            //ae_reg.stAeExpFunc.pfn_ae_exit = ae_exit;
            //rk_aiq_AELibRegCallBack(ctx->aiq_ctx, &ae_reg, 0);
            rk_aiq_customeAe_cbs_t cbs = {
                .pfn_ae_init = custom_ae_init,
                .pfn_ae_run = custom_ae_run,
                .pfn_ae_ctrl = custom_ae_ctrl,
                .pfn_ae_exit = custom_ae_exit,
            };
            rk_aiq_uapi_customAE_register(ctx->aiq_ctx, &cbs);
            rk_aiq_uapi_customAE_enable(ctx->aiq_ctx, true);
#endif

#ifdef OFFLINE_FRAME_TEST
            rk_aiq_raw_prop_t prop;
            prop.format = RK_PIX_FMT_SBGGR10;
            prop.frame_width = 2688;
            prop.frame_height = 1520;
            prop.rawbuf_type = RK_AIQ_RAW_FILE;
            rk_aiq_uapi_sysctl_prepareRkRaw(ctx->aiq_ctx, prop);
#endif
            /*
             * rk_aiq_uapi_setFecEn(ctx->aiq_ctx, true);
             * rk_aiq_uapi_setFecCorrectDirection(ctx->aiq_ctx, FEC_CORRECT_DIRECTION_Y);
            */
            rk_aiq_uapi_setRawBufNum(ctx->aiq_ctx, 4);
            rk_aiq_uapi_sysctl_setReadBackMode(ctx->aiq_ctx, true);
#ifdef TEST_MEMS_SENSOR_INTF
            extern rk_aiq_mems_sensor_intf_t g_rkiio_aiq_api;
            rk_aiq_uapi_sysctl_regMemsSensorIntf(ctx->aiq_ctx, &g_rkiio_aiq_api);
#endif

#if 1
#define LDCH_CUSTOM_MESH "ldch_custom_mesh.bin"
            rk_aiq_ldch_attrib_t ldchAttr;

            memset(&ldchAttr, 0, sizeof(ldchAttr));
            auto ret = rk_aiq_user_api_aldch_GetAttrib(ctx->aiq_ctx, &ldchAttr);
            if (ret == XCAM_RETURN_NO_ERROR) {
                char filename[512];
                sprintf(filename, "%s/%s",
                        ctx->iqpath,
                        LDCH_CUSTOM_MESH);

                ldchAttr.update_lut_mode = RK_AIQ_LDCH_UPDATE_LUT_FROM_EXTERNAL_BUFFER;
                if(ldch_read_lut_from_file(filename, &ldchAttr.lut.u.buffer.addr, \
                                           &ldchAttr.lut.u.buffer.size)) {
                    ldchAttr.en = true;
                    ldchAttr.lut.update_flag = true;
                    printf("lut addr: %p, size: %lu\n", ldchAttr.lut.u.buffer.addr,
                           ldchAttr.lut.u.buffer.size);
                    uint16_t *addr = (uint16_t *)ldchAttr.lut.u.buffer.addr;
                    printf("lut[0:15]: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                           addr[0],
                           addr[1],
                           addr[2],
                           addr[3],
                           addr[4],
                           addr[5],
                           addr[6],
                           addr[7],
                           addr[8],
                           addr[9],
                           addr[10],
                           addr[11],
                           addr[12],
                           addr[13],
                           addr[14],
                           addr[15]);

                    ret = rk_aiq_user_api_aldch_SetAttrib(ctx->aiq_ctx, ldchAttr);
                    if (ret != XCAM_RETURN_NO_ERROR) {
                        printf("Failed to set ldch attrib : %d\n", ret);
                    }
                }

                if (ldchAttr.lut.u.buffer.addr) {
                    free(ldchAttr.lut.u.buffer.addr);
                    ldchAttr.lut.u.buffer.addr =  nullptr;
                }
            }

#endif

            ret = rk_aiq_uapi_sysctl_prepare(ctx->aiq_ctx, ctx->width, ctx->height, work_mode);

            if (ret != XCAM_RETURN_NO_ERROR)
                ERR("%s:rk_aiq_uapi_sysctl_prepare failed: %d\n", get_sensor_name(ctx), ret);
            else {
#ifdef OFFLINE_FRAME_TEST
                rk_aiq_uapi_sysctl_registRkRawCb(ctx->aiq_ctx, release_buffer);
#endif
                ret = rk_aiq_uapi_sysctl_start(ctx->aiq_ctx );

                init_device(ctx);
                if (ctx->pponeframe)
                    init_device_pp_oneframe(ctx);
                start_capturing(ctx);
                if (ctx->pponeframe)
                    start_capturing_pp_oneframe(ctx);
                printf("%s:-------- stream on mipi tx/rx -------------\n", get_sensor_name(ctx));

                if (ctx->ctl_type != TEST_CTL_TYPE_DEFAULT) {
restart:
                    //static int test_ctl_cnts = 0;
                    ctx->frame_count = 10;
                    while ((ctx->frame_count-- > 0))
                        read_frame(ctx);
                    //printf("+++++++ TEST SYSCTL COUNTS %d ++++++++++++ \n", test_ctl_cnts++);
                    if (ctx->ctl_type == TEST_CTL_TYPE_REPEAT_GETSET_CALIB) {
                        CamCalibDbContext_t* calib = rk_aiq_uapi_sysctl_getCurCalib(ctx->aiq_ctx);
                        XCamReturn ret = rk_aiq_uapi_sysctl_upateCalib(ctx->aiq_ctx, calib);
                        if (ret == XCAM_RETURN_NO_ERROR) {
                            printf("update calib success.\n");
                        } else {
                            printf("update calib failed.\n");
                        }
                        goto restart;
                    } else if (ctx->ctl_type == TEST_CTL_TYPE_REPEAT_HDR_NORMAL_SWITCH) {
                        rk_aiq_working_mode_t new_mode;
                        if (ctx->hdrmode == RK_AIQ_WORKING_MODE_NORMAL) {
                            new_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
                        } else {
                            new_mode = RK_AIQ_WORKING_MODE_NORMAL;
                        }
                        printf("switch work mode from %d to %d\n", ctx->hdrmode, new_mode);
                        *((rk_aiq_working_mode_t*)(&ctx->hdrmode)) = new_mode;
                        rk_aiq_uapi_sysctl_swWorkingModeDyn(ctx->aiq_ctx, new_mode);
                        goto restart;
                    }
                    printf("aiq stop .....\n");
                    rk_aiq_uapi_sysctl_stop(ctx->aiq_ctx, false);
                    if (ctx->ctl_type == TEST_CTL_TYPE_REPEAT_INIT_PREPARE_START_STOP_DEINIT) {
                        printf("aiq deinit .....\n");
                        rk_aiq_uapi_sysctl_deinit(ctx->aiq_ctx);
                        printf("aiq init .....\n");
                        ctx->aiq_ctx = rk_aiq_uapi_sysctl_init(sns_entity_name, ctx->iqpath, NULL, NULL);
                        printf("aiq prepare .....\n");
                        XCamReturn ret = rk_aiq_uapi_sysctl_prepare(ctx->aiq_ctx, ctx->width, ctx->height, work_mode);
                    } else if (ctx->ctl_type == TEST_CTL_TYPE_REPEAT_PREPARE_START_STOP) {
                        printf("aiq prepare .....\n");
                        XCamReturn ret = rk_aiq_uapi_sysctl_prepare(ctx->aiq_ctx, ctx->width, ctx->height, work_mode);
                    } else if (ctx->ctl_type == TEST_CTL_TYPE_REPEAT_START_STOP) {
                        // do nothing
                    }
                    printf("aiq start .....\n");
                    ret = rk_aiq_uapi_sysctl_start(ctx->aiq_ctx );

                    goto restart;
                }
            }

        } else if (ctx->camgroup_ctx) {
            // only do once for cam group
            if (ctx->dev_using == 1) {
                rk_aiq_camgroup_camInfos_t camInfos;
                memset(&camInfos, 0, sizeof(camInfos));
                if (rk_aiq_uapi_camgroup_getCamInfos(ctx->camgroup_ctx, &camInfos) == XCAM_RETURN_NO_ERROR) {
                    for (int i = 0; i < camInfos.valid_sns_num; i++) {
                        rk_aiq_sys_ctx_t* aiq_ctx = NULL;
                        aiq_ctx = rk_aiq_uapi_camgroup_getAiqCtxBySnsNm(ctx->camgroup_ctx, camInfos.sns_ent_nm[i]);
                        if (!aiq_ctx)
                            continue;

                        printf("aiq_ctx sns name: %s, camPhyId %d\n",
                               camInfos.sns_ent_nm[i], camInfos.sns_camPhyId[i]);

                        rk_aiq_ldch_attrib_t ldchAttr;
                        memset(&ldchAttr, 0, sizeof(ldchAttr));

                        auto ret = rk_aiq_user_api_aldch_GetAttrib(aiq_ctx, &ldchAttr);
                        if (ret == XCAM_RETURN_NO_ERROR) {
                            char filename[512] = {0};
                            if (camInfos.sns_camPhyId[i] > 0) {
                                sprintf(filename, "%s/%s",
                                        ctx->iqpath,
                                        "cam1_ldch_mesh.bin");
                            } else {
                                sprintf(filename, "%s/%s",
                                        ctx->iqpath,
                                        "cam0_ldch_mesh.bin");
                            }

                            ldchAttr.update_lut_mode = RK_AIQ_LDCH_UPDATE_LUT_FROM_EXTERNAL_BUFFER;
                            if(ldch_read_lut_from_file(filename, &ldchAttr.lut.u.buffer.addr, \
                                                       &ldchAttr.lut.u.buffer.size)) {
                                ldchAttr.en = true;
                                ldchAttr.lut.update_flag = true;
                                printf("lut addr: %p, size: %d\n", ldchAttr.lut.u.buffer.addr,
                                       ldchAttr.lut.u.buffer.size);
                                uint16_t *addr = (uint16_t *)ldchAttr.lut.u.buffer.addr;
                                printf("lut[0:15]: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",
                                       addr[0],
                                       addr[1],
                                       addr[2],
                                       addr[3],
                                       addr[4],
                                       addr[5],
                                       addr[6],
                                       addr[7],
                                       addr[8],
                                       addr[9],
                                       addr[10],
                                       addr[11],
                                       addr[12],
                                       addr[13],
                                       addr[14],
                                       addr[15]);

                                ret = rk_aiq_user_api_aldch_SetAttrib(aiq_ctx, ldchAttr);
                                if (ret != XCAM_RETURN_NO_ERROR) {
                                    printf("Failed to set ldch attrib : %d\n", ret);
                                }
                            }

                            if (ldchAttr.lut.u.buffer.addr) {
                                free(ldchAttr.lut.u.buffer.addr);
                                ldchAttr.lut.u.buffer.addr =  nullptr;
                            }
                        }
                    }
                }

                XCamReturn ret = rk_aiq_uapi_camgroup_prepare(ctx->camgroup_ctx, work_mode);

                if (ret != XCAM_RETURN_NO_ERROR) {
                    ERR("%s:rk_aiq_uapi2_camgroup_prepare failed: %d\n", get_sensor_name(ctx), ret);
                } else {

                    ret = rk_aiq_uapi_camgroup_start(ctx->camgroup_ctx);

                    auto* aiq_ctx = rk_aiq_uapi_camgroup_getAiqCtxBySnsNm(ctx->camgroup_ctx, sns_entity_name);
                    if (!aiq_ctx) {
                        exit(1);
                    }
                    printf("==================== ctx %p %s ==========================\n", aiq_ctx, sns_entity_name);
                    Uapi_ExpSwAttr_t attr;
                    ret = rk_aiq_user_api_ae_getExpSwAttr(aiq_ctx, &attr);
                    if (ret) {
                        printf("get ae exp failed error !\n");
                        exit(1);
                    }
                    attr.stAuto.stFrmRate.isFpsFix = true;
                    attr.stAuto.stFrmRate.FpsValue = 15.0f;
                    printf("==================== %s %d api FPS : %f==========================\n", __func__, __LINE__, attr.stAuto.stFrmRate.FpsValue);
                    ret = rk_aiq_user_api_ae_setExpSwAttr(aiq_ctx, attr);
                    if (ret) {
                        printf("set ae exp failed error !\n");
                        exit(1);
                    }

                    char sns_entity_name2[64] = {'\0'};
                    if (strlen(ctx->dev_name2)) {
                        strcpy(sns_entity_name2, rk_aiq_uapi_sysctl_getBindedSnsEntNmByVd(ctx->dev_name2));
                        printf("sns_entity_name2:%s\n", sns_entity_name2);
                        auto* aiq_ctx = rk_aiq_uapi_camgroup_getAiqCtxBySnsNm(ctx->camgroup_ctx,
                                        sns_entity_name2);
                        if (!aiq_ctx) {
                            exit(1);
                        }
                        printf("==================== ctx %p %s ==========================\n",
                               aiq_ctx, sns_entity_name2);
                        Uapi_ExpSwAttr_t attr;
                        ret = rk_aiq_user_api_ae_getExpSwAttr(aiq_ctx, &attr);
                        if (ret) {
                            printf("get ae exp failed error !\n");
                            exit(1);
                        }
                        attr.stAuto.stFrmRate.isFpsFix = true;
                        attr.stAuto.stFrmRate.FpsValue = 15.0f;
                        printf("==================== %s %d api FPS : %f==========================\n", __func__, __LINE__, attr.stAuto.stFrmRate.FpsValue);
                        ret = rk_aiq_user_api_ae_setExpSwAttr(aiq_ctx, attr);
                        if (ret) {
                            printf("set ae exp failed error !\n");
                            exit(1);
                        }
                    }

                }
            }
            init_device(ctx);
            start_capturing(ctx);
        }
    }
    else {
        init_device(ctx);
        if (ctx->pponeframe)
            init_device_pp_oneframe(ctx);
        start_capturing(ctx);
        if (ctx->pponeframe)
            start_capturing_pp_oneframe(ctx);
    }
}

static void* secondary_thread(void* args) {
    pthread_detach (pthread_self());
    demo_context_t* ctx = (demo_context_t*) args;
    rkisp_routine(ctx);
    mainloop(ctx);
    deinit(ctx);
    return 0;
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    signal (SIGINT, signal_handle);
    signal (SIGQUIT, signal_handle);
    signal (SIGTERM, signal_handle);
#else
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigaddset(&mask, SIGTERM);
    sigaddset(&mask, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &mask, NULL);

    struct sigaction new_action, old_action;
    new_action.sa_handler = signal_handle;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction (SIGINT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGINT, &new_action, NULL);
    sigaction (SIGQUIT, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGQUIT, &new_action, NULL);
    sigaction (SIGTERM, NULL, &old_action);
    if (old_action.sa_handler != SIG_IGN)
        sigaction (SIGTERM, &new_action, NULL);
#endif

    demo_context_t main_ctx = {
        .out_file = {'\0'},
        .out_file2 = {'\0'},
        .dev_name = {'\0'},
        .dev_name2 = {'\0'},
        .sns_name = {'\0'},
        .dev_using = 1,
        .width = 640,
        .height = 480,
        .format = V4L2_PIX_FMT_NV12,
        .fd = -1,
        .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
        .buffers = NULL,
        .n_buffers = 0,
        .frame_count = -1,
        .fp = NULL,
        .fp2 = NULL,
        .aiq_ctx = NULL,
        .camgroup_ctx = NULL,
        .vop = 0,
        .rkaiq = 0,
        .writeFile = 0,
        .writeFileSync = 0,
        .pponeframe = 0,
        .hdrmode = 0,
        .limit_range = 0,
        .fd_pp_input = -1,
        .fd_isp_mp = -1,
        .buffers_mp = NULL,
        .outputCnt = 3,
        .skipCnt = 30,
        .yuv_dir_path = {'\0'},
        ._is_yuv_dir_exist = false,
        .capture_yuv_num = 0,
        .is_capture_yuv = false,
        .ctl_type = TEST_CTL_TYPE_DEFAULT,
        .iqpath = {'\0'},
        .camGroup = false,
    };
    demo_context_t second_ctx;

    parse_args(argc, argv, &main_ctx);

    if (main_ctx.vop) {
        if (strlen(main_ctx.dev_name) && strlen(main_ctx.dev_name2)) {
            if (display_init(720, 1280) < 0) {
                printf("display_init failed\n");
            }
        } else {
            if (initDrmDsp() < 0) {
                printf("initDrmDsp failed\n");
            }
        }
    }
    rkisp_routine(&main_ctx);
    g_main_ctx = &main_ctx;

    if(strlen(main_ctx.dev_name2)) {
        pthread_t sec_tid;
        second_ctx = main_ctx;
        second_ctx.dev_using = 2;
        g_second_ctx = &second_ctx;
        pthread_create(&sec_tid, NULL, secondary_thread, &second_ctx);
    }


#ifdef ENABLE_UAPI_TEST
    pthread_t tid;
    pthread_create(&tid, NULL, test_thread, &main_ctx);
#endif

#ifdef OFFLINE_FRAME_TEST
    pthread_t tid_offline;
    pthread_create(&tid_offline, NULL, test_offline_thread, &main_ctx);
#endif

//#define TEST_BLOCKED_STATS_FUNC
#ifdef TEST_BLOCKED_STATS_FUNC
    pthread_t stats_tid;
    pthread_create(&stats_tid, NULL, stats_thread, &main_ctx);
#endif

#ifdef ISPFEC_API
    g_ispfec_cfg.width     = main_ctx.width;
    g_ispfec_cfg.height    = main_ctx.height;
    g_ispfec_cfg.in_fourcc = 0;
    g_ispfec_cfg.out_fourcc = 0;
#if 1
    g_ispfec_cfg.mesh_upd_mode = RK_ISPFEC_UPDATE_MESH_FROM_FILE;
    strcpy(g_ispfec_cfg.u.mesh_file_path, "/etc/iqfiles/FEC_mesh_3840_2160_imx415_3.6mm/");
    strcpy(g_ispfec_cfg.mesh_xint.mesh_file, "meshxi_level0.bin");
    strcpy(g_ispfec_cfg.mesh_xfra.mesh_file, "meshxf_level0.bin");
    strcpy(g_ispfec_cfg.mesh_yint.mesh_file, "meshyi_level0.bin");
    strcpy(g_ispfec_cfg.mesh_yfra.mesh_file, "meshyf_level0.bin");
#else
    g_ispfec_cfg.mesh_upd_mode = RK_ISPFEC_UPDATE_MESH_ONLINE;
    g_ispfec_cfg.u.mesh_online.light_center[0] = 1956.3909119999998438;
    g_ispfec_cfg.u.mesh_online.light_center[1] = 1140.6355200000000422;
    g_ispfec_cfg.u.mesh_online.coeff[0] = -2819.4072493821618081;
    g_ispfec_cfg.u.mesh_online.coeff[1] = 0.0000316126581792;
    g_ispfec_cfg.u.mesh_online.coeff[2] = 0.0000000688410142;
    g_ispfec_cfg.u.mesh_online.coeff[3] = -0.0000000000130686;
    g_ispfec_cfg.u.mesh_online.correct_level = 250;
    g_ispfec_cfg.u.mesh_online.direction = RK_ISPFEC_CORRECT_DIRECTION_XY;
    g_ispfec_cfg.u.mesh_online.style = RK_ISPFEC_KEEP_ASPECT_RATIO_REDUCE_FOV;
#endif
    init_ispfec_bufs(&g_ispfec_cfg);
    g_ispfec_ctx = rk_ispfec_api_init(&g_ispfec_cfg);
#endif

    pthread_sigmask(SIG_UNBLOCK, &mask, NULL);
    mainloop(&main_ctx);
    deinit(&main_ctx);
    if (strlen(main_ctx.dev_name) && strlen(main_ctx.dev_name2)) {
        display_exit();
    }

    return 0;
}
