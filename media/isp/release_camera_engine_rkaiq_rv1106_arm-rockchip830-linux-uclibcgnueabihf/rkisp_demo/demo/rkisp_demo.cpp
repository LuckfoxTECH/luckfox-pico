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
#include <dirent.h>
#if ISPDEMO_ENABLE_DRM
#include "drmDsp.h"
#endif
#include "uAPI2/rk_aiq_user_api2_sysctl.h"
#include "rk_aiq_user_api2_debug.h"
#include "sample_image_process.h"
#include "rkisp_demo.h"
#include <termios.h>


#include "ae_algo_demo/third_party_ae_algo.h"
//#include "awb_algo_demo/third_party_awb_algo.h"  //for rk3588
#include "awb_algo_demo/third_party_awbV32_algo.h" //for rv1106
#include "af_algo_demo/third_party_af_algo.h"

#if ISPDEMO_ENABLE_RGA && ISPDEMO_ENABLE_DRM
#include "display.h"
#include "rga.h"
#endif
#include <list>
#include <vector>
#include <string>
#include <algorithm>

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define FMT_NUM_PLANES 1

#define BUFFER_COUNT 3

#ifdef ANDROID
#define CAPTURE_RAW_PATH "/data"
#define DEFAULT_CAPTURE_RAW_PATH "/data/capture_image"
#else
#define CAPTURE_RAW_PATH "/tmp"
#define DEFAULT_CAPTURE_RAW_PATH "/tmp/capture_image"
#endif
#define CAPTURE_CNT_FILENAME ".capture_cnt"
//#define ENABLE_UAPI_TEST
#define IQFILE_PATH_MAX_LEN 256
// #define CUSTOM_AE_DEMO_TEST
// #define CUSTOM_GROUP_AE_DEMO_TEST
//#define CUSTOM_AWB_DEMO_TEST
// #define TEST_MEMS_SENSOR_INTF
// #define CUSTOM_AF_DEMO_TEST
// #define CUSTOM_GROUP_AWB_DEMO_TEST

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
};

static struct termios oldt;
static int silent;
static demo_context_t *g_main_ctx = NULL, *g_second_ctx = NULL;
static bool _if_quit = false;


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

void test_update_iqfile(const demo_context_t* demo_ctx)
{
    char iqfile[IQFILE_PATH_MAX_LEN] = {0};

    printf("\nspecial an new iqfile:\n");
    strcat(iqfile, demo_ctx->iqpath);
    strcat(iqfile, "/");
    char* ret = fgets(iqfile + strlen(iqfile), IQFILE_PATH_MAX_LEN, stdin);

    char* json = strstr(iqfile, "json");

    if (!json) {
        printf("[AIQ]input is not an valide json:%s\n", iqfile);
        return;
    }

    /* fgets may add '\n' in the end of input, delete it */
    json += strlen("json");
    *json = '\0';

    printf("[AIQ] appling new iq file:%s\n", iqfile);

    rk_aiq_uapi2_sysctl_updateIq(demo_ctx->aiq_ctx, iqfile);
}

#if 0
static int set_ae_onoff(const rk_aiq_sys_ctx_t* ctx, bool onoff);
void test_imgproc(const demo_context_t* demo_ctx) {

    if (demo_ctx == NULL) {
        return;
    }

    const rk_aiq_sys_ctx_t* ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);

    /*TODO: when rkaiq_3A_server & rkisp_demo run in two different shell, rk_aiq_sys_ctx_t would be null?*/
    if (ctx == NULL) {
        printf("ERROR : rk_aiq_sys_ctx_t is null.\n");
        _if_quit = true;
        return;
    }

    int key = getchar();
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
    case 'g':
        break;
    case 'h':
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
        rk_aiq_uapi_setSaturation(ctx, 50);
        printf("setSaturation\n");
        break;
    case 'm':
        unsigned int level1;
        rk_aiq_uapi_getSaturation(ctx, &level1);
        printf("getSaturation=%d\n", level1);
        break;
    case 'n':
        rk_aiq_uapi_setCrSuppsn(ctx, 50);
        printf("setCrSuppsn\n");
        break;
    case 'o':
        unsigned int level2;
        rk_aiq_uapi_getCrSuppsn(ctx, &level2);
        printf("getCrSuppsn=%d\n", level2);
        break;
    case 'p':
        //rk_aiq_uapi_setHDRMode(ctx, OP_AUTO);
        printf("setHDRMode\n");
        break;
    case 'q':
        //rk_aiq_uapi_setHDRMode(ctx, OP_MANUAL);
        printf("setHDRMode\n");
        break;
    case 'r':
        //rk_aiq_uapi_getHDRMode(ctx, &mode);
        printf("getHDRMode=%d\n", mode);
        break;
    case 's': {
        unsigned int set_anr_strength = 80;
        unsigned int get_anr_strength = 0;
        rk_aiq_uapi_setANRStrth(ctx, set_anr_strength);
        printf("setANRStrth %u \n", set_anr_strength);
        sleep(1);
        rk_aiq_uapi_getANRStrth(ctx, &get_anr_strength);
        printf("getANRStrth %u \n", get_anr_strength);
        break;
    }
    case 't': {
        unsigned int set_strength = 80;
        unsigned int get_space_strength = 0;
        unsigned int get_mfnr_strength = 0;
        bool state;
        rk_aiq_uapi_setMSpaNRStrth(ctx, true, set_strength);
        rk_aiq_uapi_setMTNRStrth(ctx, true, set_strength);
        printf("setMSpaNRStrth and setMTNRStrth :%u \n", set_strength);
        rk_aiq_uapi_getMSpaNRStrth(ctx, &state, &get_space_strength);
        rk_aiq_uapi_getMTNRStrth(ctx, &state, &get_mfnr_strength);
        printf("setMSpaNRStrth and setMTNRStrth :%u %u\n", get_space_strength, get_mfnr_strength);
        break;
    }
    case 'u':
        //rk_aiq_uapi_setDhzMode(ctx, OP_MANUAL);
        //printf("setDhzMode\n");
        break;
    case 'v':
        //rk_aiq_uapi_getDhzMode(ctx, &mode);
        // printf("getDhzMode=%d\n", mode);
        break;
    case 'w':
    {
        bool stat = false;
        //unsigned int level4 = 0;
        //rk_aiq_uapi_getMHDRStrth(ctx, &stat, &level4);
        //printf("getMHDRStrth: status:%d, level=%d\n", stat, level4);
    }
    break;
    case 'x':
        //rk_aiq_uapi_setMHDRStrth(ctx, true, 8);
        //printf("setMHDRStrth true\n");
        break;
    case 'y':
    {
        bool mod_en;
        rk_aiq_uapi2_sysctl_getModuleCtl(ctx, RK_MODULE_TNR, &mod_en);
        printf("getModuleCtl=%d\n", mod_en);
        if (mod_en) {
            rk_aiq_uapi2_sysctl_setModuleCtl(ctx, RK_MODULE_TNR, false);
        } else {
            rk_aiq_uapi2_sysctl_setModuleCtl(ctx, RK_MODULE_TNR, true);
        }
    }
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
        rk_aiq_af_zoomrange range;
        int code;

        rk_aiq_uapi_getZoomRange(ctx, &range);
        printf("zoom.min_pos %d, zoom.max_pos %d\n", range.min_pos, range.max_pos);

        rk_aiq_uapi_getOpZoomPosition(ctx, &code);
        printf("getOpZoomPosition code %d\n", code);

        code += 20;
        if (code > range.max_pos)
            code = range.max_pos;
        rk_aiq_uapi_setOpZoomPosition(ctx, code);
        rk_aiq_uapi_endOpZoomChange(ctx);
        printf("setOpZoomPosition %d\n", code);
    }
    break;
    case 'E': {
        rk_aiq_af_zoomrange range;
        int code;

        rk_aiq_uapi_getZoomRange(ctx, &range);
        printf("zoom.min_pos %d, zoom.max_pos %d\n", range.min_pos, range.max_pos);

        rk_aiq_uapi_getOpZoomPosition(ctx, &code);
        printf("getOpZoomPosition code %d\n", code);

        code -= 20;
        if (code < range.min_pos)
            code = range.min_pos;
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
    case 'I':
        if (CHECK_ISP_HW_V20()) {
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
            sleep(5);
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
        }
        break;
    case 'J':
        if (CHECK_ISP_HW_V20()) {
            rk_aiq_sharp_IQpara_t stSharpIQpara;
            rk_aiq_sharp_IQpara_t stGetSharpIQpara;
            stSharpIQpara.module_bits = (1 << ASHARP_MODULE_SHARP) | (1 << ASHARP_MODULE_EDGEFILTER) ;
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
            sleep(5);
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
        }
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
        printf("test fec correct level100\n");
        rk_aiq_uapi_setFecCorrectLevel(ctx, 100);
        break;
    case 'M':
        printf("test fec correct level255\n");
        rk_aiq_uapi_setFecCorrectLevel(ctx, 255);
        break;
    case 'N':
    {
        rk_aiq_dpcc_attrib_V20_t attr;
        rk_aiq_user_api2_adpcc_GetAttrib(ctx, &attr);
        rk_aiq_user_api2_adpcc_SetAttrib(ctx, &attr);
        adebayer_attrib_t attr2;
        rk_aiq_user_api2_adebayer_SetAttrib(ctx, attr2);
    }
    break;
    case 'O':
        printf("test not bypass fec\n");
        rk_aiq_uapi_setFecBypass(ctx, false);
        break;
    case 'P':
    {
        int work_mode = demo_ctx->hdrmode;
        rk_aiq_working_mode_t new_mode;
        if (work_mode == RK_AIQ_WORKING_MODE_NORMAL)
            new_mode = RK_AIQ_WORKING_MODE_ISP_HDR3;
        else
            new_mode = RK_AIQ_WORKING_MODE_NORMAL;
        printf("switch work mode from %d to %d\n", work_mode, new_mode);
        *const_cast<int*>(&demo_ctx->hdrmode) = work_mode = new_mode;
        rk_aiq_uapi_sysctl_swWorkingModeDyn(ctx, new_mode);
    }
    break;
    case 'Q':
    {
        rk_aiq_rotation_t rot = RK_AIQ_ROTATION_90;
        rk_aiq_mems_sensor_intf_t intf = {0};
        const char* main_scene = "good";
        const char* sub_scene = "bad";
        rk_aiq_uapi2_sysctl_setSharpFbcRotation(ctx, rot);
        rk_aiq_uapi2_sysctl_setMulCamConc(ctx, true);
        rk_aiq_uapi2_sysctl_regMemsSensorIntf(ctx, &intf);
        rk_aiq_uapi2_sysctl_switch_scene(ctx, main_scene, sub_scene);
    }
    break;
    case 'R':
    {
        rk_aiq_cpsl_info_t cpsl_info;
        rk_aiq_cpsl_cap_t cpsl_cap;
        rk_aiq_uapi2_sysctl_getCpsLtInfo(ctx, &cpsl_info);
        rk_aiq_uapi2_sysctl_queryCpsLtCap(ctx, &cpsl_cap);
        printf("sensitivity: %f, cap sensitivity: %f:%f:%f\n", cpsl_info.sensitivity,
               cpsl_cap.sensitivity.min, cpsl_cap.sensitivity.step, cpsl_cap.sensitivity.max);
        rk_aiq_cpsl_cfg_t cpsl_cfg;
        rk_aiq_uapi2_sysctl_setCpsLtCfg(ctx, &cpsl_cfg);
    }
    break;
    case 'S':
        printf("test ldch correct level100\n");
        rk_aiq_uapi_setLdchCorrectLevel(ctx, 100);
        break;
    case 'T':
    {
        rk_aiq_rect_t info;
        rk_aiq_uapi2_sysctl_getCrop(ctx, &info);
        printf("left:%d, top:%d, width:%d, height:%d\n", info.left, info.top, info.width, info.height);
        info.left += 64;
        info.top += 64;
        info.width = 640;
        info.height = 480;
        //rk_aiq_uapi2_sysctl_setCrop(ctx, info);
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
    case 'V':
    {
        test_update_iqfile(demo_ctx);
    }
    break;
    case 'W':
    {
        rk_aiq_ver_info_t vers;
        rk_aiq_uapi2_get_version_info(&vers);
        printf("aiq ver %s, parser ver %s, magic code %d, awb ver %s\n"
               "ae ver %s, af ver %s, ahdr ver %s\n", vers.aiq_ver,
               vers.iq_parser_ver, vers.iq_parser_magic_code,
               vers.awb_algo_ver, vers.ae_algo_ver,
               vers.af_algo_ver, vers.ahdr_algo_ver);
    }
    break;
    case 'X':
    {
        for (int type = RK_AIQ_ALGO_TYPE_AE; type < RK_AIQ_ALGO_TYPE_MAX; type++)
        {
            bool ret = rk_aiq_uapi2_sysctl_getAxlibStatus(ctx, type, 0);
            printf("%d is %s, \n", type, (ret ? "enabled" : "disabled or unregistered"));

            const RkAiqAlgoContext* algo_ptr = rk_aiq_uapi2_sysctl_getEnabledAxlibCtx(ctx, type);
            if (algo_ptr) {
                printf("type: %d ==> algo_ptr: %p\n", type, algo_ptr);
            }
            if (ret == false) {
                bool ret2 = rk_aiq_uapi2_sysctl_enableAxlib(ctx, type, 0, !ret);
            }
        }
    }
    break;
    case 'Y':
    {
        rk_aiq_lsc_querry_info_t info;
        rk_aiq_user_api2_alsc_QueryLscInfo(ctx, &info);
        printf("LSC: enable: %s, \n", (info.lsc_en ? "true" : "false"));
        printf("r: %d, %d, %d \n", info.r_data_tbl[0], info.r_data_tbl[1], info.r_data_tbl[2]);
        printf("gr: %d, %d, %d \n", info.gr_data_tbl[0], info.gr_data_tbl[1], info.gr_data_tbl[2]);
        printf("gb: %d, %d, %d \n", info.gb_data_tbl[0], info.gb_data_tbl[1], info.gb_data_tbl[2]);
        printf("b: %d, %d, %d \n", info.b_data_tbl[0], info.b_data_tbl[1], info.b_data_tbl[2]);
    }
    break;
    case 'Z':
    {
        rk_aiq_static_info_t info;
        rk_aiq_uapi2_sysctl_enumStaticMetas(0, &info);
        printf("isp version: %d, sensor name: %s\n", info.isp_hw_ver, info.sensor_info.sensor_name);
    }
    case '[':
        set_ae_onoff(ctx, true);
        printf("set ae on\n");
        break;
    case ']':
        set_ae_onoff(ctx, false);
        printf("set ae off\n");
        break;
    default:
        break;
    }
}
#endif

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
    if (ctx->fp && sequence >= ctx->skipCnt && ctx->outputCnt-- > 0) {
        printf(">\n");
        fwrite(p, size, 1, ctx->fp);
        fflush(ctx->fp);
        fsync(fileno(ctx->fp));
    } else if (ctx->fp && sequence >= ctx->skipCnt && ctx->outputCnt-- == 0) {
        fclose(ctx->fp);
        ctx->fp = NULL;
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
                creat_yuv_dir(DEFAULT_CAPTURE_RAW_PATH, ctx);
            }

            if (ctx->_is_yuv_dir_exist) {
                write_yuv_to_file(p, size, sequence, ctx);
                rk_aiq_uapi2_debug_captureRawNotify(ctx->aiq_ctx);
            }

            if (ctx->capture_yuv_num-- == 0) {
                ctx->is_capture_yuv = false;
                ctx->_is_yuv_dir_exist = false;
            }
        }
    }
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
            if (ctx->dev_using == 1)
                display_win1(ctx->buffers[i].start, ctx->buffers[i].export_fd,  RK_FORMAT_YCbCr_420_SP, dispWidth, dispHeight, 0);
            else
                display_win2(ctx->buffers[i].start, ctx->buffers[i].export_fd,  RK_FORMAT_YCbCr_420_SP, dispWidth, dispHeight, 0);
        } else {
#else
        {
#endif
            drmDspFrame(ctx->width, ctx->height, dispWidth, dispHeight, ctx->buffers[i].export_fd, DRM_FORMAT_NV12);
        }
    }
#endif

    process_image(ctx->buffers[i].start,  buf.sequence, bytesused, ctx);

    if (-1 == xioctl(ctx->fd, VIDIOC_QBUF, &buf))
        errno_exit(ctx, "VIDIOC_QBUF");

    return 1;
}

static int read_frame_pp_oneframe(demo_context_t *ctx)
{
    struct v4l2_buffer buf;
    struct v4l2_buffer buf_pp;
    int i, ii = 0, bytesused;
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

static void mainloop(demo_context_t *ctx)
{
    while ((ctx->frame_count == -1) || (ctx->frame_count-- > 0)) {
        if (ctx->pponeframe)
            read_frame_pp_oneframe(ctx);
        else
            read_frame(ctx);
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
            {"orp", required_argument, 0, '2' },
            //{"sensor",   required_argument,       0, 'b' },
            {"camgroup",   no_argument,       0, '3' },
            {0,          0,                 0,  0  }
        };

        //c = getopt_long(argc, argv, "w:h:f:i:d:o:c:ps",
        c = getopt_long(argc, argv, "w:h:f:i:d:o:c:n:k:a:t:1:2:3mpsevrl",
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
            break;
        case '1':
            strcpy(ctx->iqpath, optarg);
            break;
        case '2':
        {
            // parse raw fmt
            char* raw_dir = strstr(optarg, ",");
            size_t raw_dir_str_len = raw_dir - optarg;
            if (!raw_dir) {
                printf("orp dir error ! \n");
                exit(-1);
            }
            strncpy(ctx->orppath, optarg, raw_dir_str_len);

            char* raw_fmt_w_start = raw_dir + 1;
            int raw_width = atoi(raw_fmt_w_start);
            if (raw_width == 0) {
                printf("orp raw_width error ! \n");
                exit(-1);
            }
            ctx->orpRawW = raw_width;
            char* raw_fmt_h_start = strstr(raw_fmt_w_start, ":") + 1;
            if (!raw_fmt_h_start) {
                printf("orp raw_h error ! \n");
                exit(-1);
            }
            int raw_height = atoi(raw_fmt_h_start);
            ctx->orpRawH = raw_height;

            char* raw_fmt_pix_start = strstr(raw_fmt_h_start, ":") + 1;
            strcpy(ctx->orpRawFmt, raw_fmt_pix_start);

            printf("orp_path:%s, w:%d,h:%d, pix:%s \n",
                   ctx->orppath, raw_width, raw_height, raw_fmt_pix_start);
            ctx->isOrp = true;
        }
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
                "         --stream-count, default 3          optional, how many frames to write files\n"
                "         --stream-skip, default 30          optional, how many frames to skip befor writing file\n"
                "         --vop,                             optional, drm display\n"
                "         --rkaiq,                           optional, auto image quality\n"
                "         --silent,                          optional, subpress debug log\n"
                "         --pponeframe,                      optional, pp oneframe readback mode\n"
                "         --hdr <val>,                       optional, hdr mode, val 2 means hdrx2, 3 means hdrx3 \n"
                "         --sync-to-raw,                     optional, write yuv files in sync with raw\n"
                "         --limit,                           optional, yuv limit range\n"
                "         --ctl <val>,                       optional, sysctl procedure test \n"
                "         --iqpath <val>,                    optional, absolute path of iq file dir \n"
                "         --orp <raw_dir,w:h:raw_fmt>,       optional, absolute path of raw files dir \n"
                "                                            raw_fmt: BG10 -> SBGGR10, GB10 -> SGBRG10 \n"
                "                                                     BA10 -> SGRBG10, RG10 -> SRGGB10 \n"
                "         --sensor,  default os04a10,        optional, sensor names\n",
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
        rk_aiq_uapi2_sysctl_stop(ctx->aiq_ctx, false);
    } else if (ctx->camgroup_ctx) {
        if (ctx->dev_using == 1) {
            printf("%s:-------- stop aiq camgroup -------------\n", get_sensor_name(ctx));
            rk_aiq_uapi2_camgroup_stop(ctx->camgroup_ctx);
#ifdef CUSTOM_GROUP_AE_DEMO_TEST
            rk_aiq_uapi2_customAE_unRegister((const rk_aiq_sys_ctx_t*)(ctx->camgroup_ctx));
#endif
#ifdef CUSTOM_GROUP_AWB_DEMO_TEST
                rk_aiq_uapi2_customAWB_unRegister((const rk_aiq_sys_ctx_t*)(ctx->camgroup_ctx));
#endif

        }
    }

    if (ctx->aiq_ctx) {
        printf("%s:-------- deinit aiq -------------\n", get_sensor_name(ctx));
#ifdef CUSTOM_AE_DEMO_TEST
        //rk_aiq_AELibunRegCallBack(ctx->aiq_ctx, 0);
        rk_aiq_uapi2_customAE_unRegister(ctx->aiq_ctx);
#endif
#ifdef CUSTOM_AWB_DEMO_TEST
        //rk_aiq_AELibunRegCallBack(ctx->aiq_ctx, 0);
        rk_aiq_uapi2_customAWB_unRegister(ctx->aiq_ctx);
#endif
        rk_aiq_uapi2_sysctl_deinit(ctx->aiq_ctx);
        printf("%s:-------- deinit aiq end -------------\n", get_sensor_name(ctx));
    } else if (ctx->camgroup_ctx) {
        if (ctx->dev_using == 1) {
            printf("%s:-------- deinit aiq camgroup -------------\n", get_sensor_name(ctx));
            rk_aiq_uapi2_camgroup_destroy(ctx->camgroup_ctx);
            ctx->camgroup_ctx = NULL;
            printf("%s:-------- deinit aiq camgroup end -------------\n", get_sensor_name(ctx));
        }
    }

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
    while(!_if_quit) {
        // test_imgproc((demo_context_t*) args);
        sample_main(args);
    }
    printf("end test imgproc\n");
    restore_terminal_settings();
    return 0;
}

static void* test_offline_thread(void* args) {
    pthread_detach (pthread_self());
    demo_context_t* demo_ctx = (demo_context_t*) args;
    DIR* dir = opendir(demo_ctx->orppath);
    struct dirent* dir_ent = NULL;
    std::vector<std::string> raw_files;
    if (dir) {
        while ((dir_ent = readdir(dir))) {
            if (dir_ent->d_type == DT_REG) {
                // is raw file
                if (strstr(dir_ent->d_name, ".raw")) {
                    raw_files.push_back(dir_ent->d_name);
                }
            }
        }
        closedir(dir);
    }
    std::sort(raw_files.begin(), raw_files.end(),
    [](std::string str1, std::string str2) -> bool {
        std::string::size_type sz;
        int ind1 = std::stoi(str1, &sz);
        int ind2 = std::stoi(str2, &sz);
        return ind1 < ind2 ? true : false;
    });
    while (!demo_ctx->orpStop) {
        for (auto raw_file : raw_files) {
            std::string full_name = demo_ctx->orppath + raw_file;
            printf("process raw : %s \n", full_name.c_str());
            rk_aiq_uapi2_sysctl_enqueueRkRawFile(demo_ctx->aiq_ctx, full_name.c_str());
            //usleep(500000);
        }
        usleep(500000);
    }
    demo_ctx->orpStopped = true;
    return 0;
}

#if 0
static int set_ae_onoff(const rk_aiq_sys_ctx_t* ctx, bool onoff)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpSwAttr_t expSwAttr;

    ret = rk_aiq_user_api_ae_getExpSwAttr(ctx, &expSwAttr);
    expSwAttr.enable = onoff;
    ret = rk_aiq_user_api_ae_setExpSwAttr(ctx, expSwAttr);

    return 0;
}
#endif

static int query_ae_state(const rk_aiq_sys_ctx_t* ctx)
{
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    Uapi_ExpQueryInfo_t queryInfo;

    ret =  rk_aiq_user_api2_ae_queryExpResInfo(ctx, &queryInfo);
    printf("ae IsConverged: %d\n", queryInfo.IsConverged);

    return 0;
}

static void set_af_manual_meascfg(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_attrib_t attr;
    uint16_t gamma_y[RKAIQ_RAWAF_GAMMA_NUM] =
    {0, 45, 108, 179, 245, 344, 409, 459, 500, 567, 622, 676, 759, 833, 896, 962, 1023};

    rk_aiq_user_api2_af_GetAttrib(ctx, &attr);
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
    rk_aiq_user_api2_af_SetAttrib(ctx, &attr);
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
        ret = rk_aiq_uapi2_sysctl_get3AStatsBlk(ctx->aiq_ctx, &stats_ref, -1);
        if (ret == XCAM_RETURN_NO_ERROR && stats_ref != NULL) {
            printf("get one stats frame id %d \n", stats_ref->frame_id);
            query_ae_state(ctx->aiq_ctx);
            print_af_stats(stats_ref);
            rk_aiq_uapi2_sysctl_release3AStatsRef(ctx->aiq_ctx, stats_ref);
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

static void rkisp_routine(demo_context_t *ctx)
{
    char sns_entity_name[64];
    rk_aiq_working_mode_t work_mode = RK_AIQ_WORKING_MODE_NORMAL;

    if (ctx->hdrmode == 2)
        work_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
    else if (ctx->hdrmode == 3)
        work_mode = RK_AIQ_WORKING_MODE_ISP_HDR3;

    printf("work_mode %d\n", work_mode);

    strcpy(sns_entity_name, rk_aiq_uapi2_sysctl_getBindedSnsEntNmByVd(get_dev_name(ctx)));
    printf("sns_entity_name:%s\n", sns_entity_name);
    sscanf(&sns_entity_name[6], "%s", ctx->sns_name);
    printf("sns_name:%s\n", ctx->sns_name);
    rk_aiq_static_info_t s_info;
    rk_aiq_uapi2_sysctl_getStaticMetas(sns_entity_name, &s_info);
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
        ctx->fp = fopen(ctx->out_file, "w+");
        if (ctx->fp == NULL) {
            ERR("%s: fopen output file %s failed!\n", get_sensor_name(ctx), ctx->out_file);
        }
    }

    if (ctx->rkaiq) {
        XCamReturn ret = XCAM_RETURN_NO_ERROR;
        rk_aiq_tb_info_t tb_info;
        tb_info.magic = sizeof(rk_aiq_tb_info_t) - 2;
        tb_info.is_pre_aiq = false;
        ret = rk_aiq_uapi2_sysctl_preInit_tb_info(sns_entity_name, &tb_info);
        if (work_mode == RK_AIQ_WORKING_MODE_NORMAL)
            ret = rk_aiq_uapi2_sysctl_preInit_scene(sns_entity_name, "normal", "day");
        else
            ret = rk_aiq_uapi2_sysctl_preInit_scene(sns_entity_name, "hdr", "day");
        if (ret < 0)
            ERR("%s: failed to set %s scene\n",
                get_sensor_name(ctx),
                work_mode == RK_AIQ_WORKING_MODE_NORMAL ? "normal" : "hdr");

        if (strlen(ctx->iqpath))
        {
            if (!ctx->camGroup)
                ctx->aiq_ctx = rk_aiq_uapi2_sysctl_init(sns_entity_name, ctx->iqpath, NULL, NULL);
            else {
                // create once for mulitple cams
                if (ctx->dev_using == 1) {
                    char sns_entity_name2[64] = {'\0'};
                    bool has_dev2 = false;
                    if (strlen(ctx->dev_name2)) {
                        strcpy(sns_entity_name2, rk_aiq_uapi2_sysctl_getBindedSnsEntNmByVd(ctx->dev_name2));
                        printf("sns_entity_name2:%s\n", sns_entity_name2);
                        //sscanf(&sns_entity_name2[6], "%s", ctx->sns_name);
                        //printf("sns_name2:%s\n", ctx->sns_name);
                        has_dev2 = true;
                    }

                    rk_aiq_camgroup_instance_cfg_t camgroup_cfg;
                    memset(&camgroup_cfg, 0, sizeof(camgroup_cfg));
                    camgroup_cfg.sns_num = 1;
                    if (has_dev2)
                        camgroup_cfg.sns_num++;
                    camgroup_cfg.sns_ent_nm_array[0] = sns_entity_name;
                    if (has_dev2)
                        camgroup_cfg.sns_ent_nm_array[1] = sns_entity_name2;
                    camgroup_cfg.config_file_dir = ctx->iqpath;
                    camgroup_cfg.overlap_map_file = "srcOverlapMap.bin";
                    ctx->camgroup_ctx = rk_aiq_uapi2_camgroup_create(&camgroup_cfg);
                    if (!ctx->camgroup_ctx) {
                        printf("create camgroup ctx error !\n");
                        exit(1);
                    }

#ifdef CUSTOM_GROUP_AE_DEMO_TEST
                    rk_aiq_customeAe_cbs_t cbs = {
                        .pfn_ae_init = custom_ae_init,
                        .pfn_ae_run = custom_ae_run,
                        .pfn_ae_ctrl = custom_ae_ctrl,
                        .pfn_ae_exit = custom_ae_exit,
                    };
                    rk_aiq_uapi2_customAE_register((const rk_aiq_sys_ctx_t*)(ctx->camgroup_ctx), &cbs);
                    rk_aiq_uapi2_customAE_enable((const rk_aiq_sys_ctx_t*)(ctx->camgroup_ctx), true);
#endif
#ifdef CUSTOM_GROUP_AWB_DEMO_TEST
                rk_aiq_customeAwb_cbs_t awb_cbs = {
                    .pfn_awb_init = custom_awb_init,
                    .pfn_awb_run = custom_awb_run,
                    .pfn_awb_ctrl= custom_awb_ctrl,
                    .pfn_awb_exit = custom_awb_exit,
                };
                rk_aiq_uapi2_customAWB_register((const rk_aiq_sys_ctx_t*)(ctx->camgroup_ctx), &awb_cbs);
                rk_aiq_uapi2_customAWB_enable((const rk_aiq_sys_ctx_t*)(ctx->camgroup_ctx), true);
#endif

                }
            }
        } else {
            if (ctx->camGroup) {
                printf("error! should specify iq path !\n");
                exit(1);
            }
#ifndef ANDROID
            rk_aiq_uapi2_sysctl_preInit(sns_entity_name, RK_AIQ_WORKING_MODE_NORMAL, "ov5695_TongJu_CHT842-MD.xml");
            ctx->aiq_ctx = rk_aiq_uapi2_sysctl_init(sns_entity_name, "/oem/etc/iqfiles", NULL, NULL);
#else
            ctx->aiq_ctx = rk_aiq_uapi2_sysctl_init(sns_entity_name, "/data/etc/iqfiles", NULL, NULL);
#endif

        }
        if (ctx->aiq_ctx) {
            printf("%s:-------- init mipi tx/rx -------------\n", get_sensor_name(ctx));
            if (ctx->writeFileSync)
                rk_aiq_uapi2_debug_captureRawYuvSync(ctx->aiq_ctx, CAPTURE_RAW_AND_YUV_SYNC);
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
            rk_aiq_uapi2_customAE_register(ctx->aiq_ctx, &cbs);
            rk_aiq_uapi2_customAE_enable(ctx->aiq_ctx, true);
#endif
#ifdef CUSTOM_AWB_DEMO_TEST
            rk_aiq_customeAwb_cbs_t awb_cbs = {
                .pfn_awb_init = custom_awb_init,
                .pfn_awb_run = custom_awb_run,
                .pfn_awb_ctrl = custom_awb_ctrl,
                .pfn_awb_exit = custom_awb_exit,
            };
            rk_aiq_uapi2_customAWB_register(ctx->aiq_ctx, &awb_cbs);
            rk_aiq_uapi2_customAWB_enable(ctx->aiq_ctx, true);
#endif
            if (ctx->isOrp) {
                rk_aiq_raw_prop_t prop;
                if (strcmp(ctx->orpRawFmt, "BA81") == 0)
                    prop.format = RK_PIX_FMT_SBGGR8;
                else if (strcmp(ctx->orpRawFmt, "GBRG") == 0)
                    prop.format = RK_PIX_FMT_SGBRG8;
                else if (strcmp(ctx->orpRawFmt, "RGGB") == 0)
                    prop.format = RK_PIX_FMT_SRGGB8;
                else if (strcmp(ctx->orpRawFmt, "GRBG") == 0)
                    prop.format = RK_PIX_FMT_SGRBG8;
                else if (strcmp(ctx->orpRawFmt, "BG10") == 0)
                    prop.format = RK_PIX_FMT_SBGGR10;
                else if (strcmp(ctx->orpRawFmt, "GB10") == 0)
                    prop.format = RK_PIX_FMT_SGBRG10;
                else if (strcmp(ctx->orpRawFmt, "RG10") == 0)
                    prop.format = RK_PIX_FMT_SRGGB10;
                else if (strcmp(ctx->orpRawFmt, "BA10") == 0)
                    prop.format = RK_PIX_FMT_SGRBG10;
                else if (strcmp(ctx->orpRawFmt, "BG12") == 0)
                    prop.format = RK_PIX_FMT_SBGGR12;
                else if (strcmp(ctx->orpRawFmt, "GB12") == 0)
                    prop.format = RK_PIX_FMT_SGBRG12;
                else if (strcmp(ctx->orpRawFmt, "RG12") == 0)
                    prop.format = RK_PIX_FMT_SRGGB12;
                else if (strcmp(ctx->orpRawFmt, "BA12") == 0)
                    prop.format = RK_PIX_FMT_SGRBG12;
                else if (strcmp(ctx->orpRawFmt, "BG14") == 0)
                    prop.format = RK_PIX_FMT_SBGGR14;
                else if (strcmp(ctx->orpRawFmt, "GB14") == 0)
                    prop.format = RK_PIX_FMT_SGBRG14;
                else if (strcmp(ctx->orpRawFmt, "RG14") == 0)
                    prop.format = RK_PIX_FMT_SRGGB14;
                else if (strcmp(ctx->orpRawFmt, "BA14") == 0)
                    prop.format = RK_PIX_FMT_SGRBG14;
                else
                    prop.format = RK_PIX_FMT_SBGGR10;
                prop.frame_width = ctx->orpRawW;
                prop.frame_height = ctx->orpRawH;
                prop.rawbuf_type = RK_AIQ_RAW_FILE;
                rk_aiq_uapi2_sysctl_prepareRkRaw(ctx->aiq_ctx, prop);
            }
            /*
             * rk_aiq_uapi_setFecEn(ctx->aiq_ctx, true);
             * rk_aiq_uapi_setFecCorrectDirection(ctx->aiq_ctx, FEC_CORRECT_DIRECTION_Y);
             */
#ifdef TEST_MEMS_SENSOR_INTF
            rk_aiq_mems_sensor_intf_t g_rkiio_aiq_api;
            rk_aiq_uapi2_sysctl_regMemsSensorIntf(ctx->aiq_ctx, &g_rkiio_aiq_api);
#endif

            XCamReturn ret = rk_aiq_uapi2_sysctl_prepare(ctx->aiq_ctx, ctx->width, ctx->height, work_mode);

            if (ret != XCAM_RETURN_NO_ERROR)
                ERR("%s:rk_aiq_uapi2_sysctl_prepare failed: %d\n", get_sensor_name(ctx), ret);
            else {
                ret = rk_aiq_uapi2_setMirrorFlip(ctx->aiq_ctx, false, false, 3);
                // Ignore failure

                if (ctx->isOrp) {
                    rk_aiq_uapi2_sysctl_registRkRawCb(ctx->aiq_ctx, release_buffer);
                }
                ret = rk_aiq_uapi2_sysctl_start(ctx->aiq_ctx );

                init_device(ctx);
                if (ctx->pponeframe)
                    init_device_pp_oneframe(ctx);
                if (ctx->ctl_type == TEST_CTL_TYPE_DEFAULT) {
                    start_capturing(ctx);
                }
                if (ctx->pponeframe)
                    start_capturing_pp_oneframe(ctx);
                printf("%s:-------- stream on mipi tx/rx -------------\n", get_sensor_name(ctx));

                if (ctx->ctl_type != TEST_CTL_TYPE_DEFAULT) {
restart:
                    static int test_ctl_cnts = 0;
                    ctx->frame_count = 60;
                    start_capturing(ctx);
                    while ((ctx->frame_count-- > 0))
                        read_frame(ctx);
                    stop_capturing(ctx);
                    printf("+++++++ TEST SYSCTL COUNTS %d ++++++++++++ \n", test_ctl_cnts++);
                    printf("aiq stop .....\n");
                    rk_aiq_uapi2_sysctl_stop(ctx->aiq_ctx, false);
                    if (ctx->ctl_type == TEST_CTL_TYPE_REPEAT_INIT_PREPARE_START_STOP_DEINIT) {
                        printf("aiq deinit .....\n");
                        rk_aiq_uapi2_sysctl_deinit(ctx->aiq_ctx);
                        printf("aiq init .....\n");
                        if (work_mode == RK_AIQ_WORKING_MODE_NORMAL) {
                            ret = rk_aiq_uapi2_sysctl_preInit_scene(sns_entity_name, "normal", "day");
                            if (ctx->hdrmode == 2)
                                work_mode = RK_AIQ_WORKING_MODE_ISP_HDR2;
                            else if (ctx->hdrmode == 3)
                                work_mode = RK_AIQ_WORKING_MODE_ISP_HDR3;
                        } else {
                            ret = rk_aiq_uapi2_sysctl_preInit_scene(sns_entity_name, "hdr", "day");
                            work_mode = RK_AIQ_WORKING_MODE_NORMAL;
                        }
                        if (ret < 0)
                            ERR("%s: failed to set %s scene\n",
                                get_sensor_name(ctx),
                                work_mode == RK_AIQ_WORKING_MODE_NORMAL ? "normal" : "hdr");
                        ctx->aiq_ctx = rk_aiq_uapi2_sysctl_init(sns_entity_name, ctx->iqpath, NULL, NULL);
                        printf("aiq prepare .....\n");
                        XCamReturn ret = rk_aiq_uapi2_sysctl_prepare(ctx->aiq_ctx, ctx->width, ctx->height, work_mode);
                    } else if (ctx->ctl_type == TEST_CTL_TYPE_REPEAT_PREPARE_START_STOP) {
                        printf("aiq prepare .....\n");
                        XCamReturn ret = rk_aiq_uapi2_sysctl_prepare(ctx->aiq_ctx, ctx->width, ctx->height, work_mode);
                    } else if (ctx->ctl_type == TEST_CTL_TYPE_REPEAT_START_STOP) {
                        // do nothing
                    }
                    printf("aiq start .....\n");
                    ret = rk_aiq_uapi2_sysctl_start(ctx->aiq_ctx );
                    printf("aiq restart .....\n");
                    goto restart;
                }
            }

        } else if (ctx->camgroup_ctx) {
            // only do once for cam group
            if (ctx->dev_using == 1) {
                XCamReturn ret = rk_aiq_uapi2_camgroup_prepare(ctx->camgroup_ctx, work_mode);

                if (ret != XCAM_RETURN_NO_ERROR)
                    ERR("%s:rk_aiq_uapi2_camgroup_prepare failed: %d\n", get_sensor_name(ctx), ret);
                else {

                    ret = rk_aiq_uapi2_camgroup_start(ctx->camgroup_ctx);
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
        .orppath = {'\0'},
        .orpRawW = 0,
        .orpRawH = 0,
        .orpRawFmt = {'\0'},
        .isOrp = false,
        .orpStop = false,
        .orpStopped = false,
        .camGroup = false,
    };
    demo_context_t second_ctx;

    parse_args(argc, argv, &main_ctx);

#if ISPDEMO_ENABLE_DRM
    if (main_ctx.vop) {

#if ISPDEMO_ENABLE_RGA
        if (strlen(main_ctx.dev_name) && strlen(main_ctx.dev_name2)) {
            if (display_init(720, 1280) < 0) {
                printf("display_init failed\n");
            }
        } else {
#else
        {
#endif
            if (initDrmDsp() < 0) {
                printf("initDrmDsp failed\n");
            }
        }
    }
#endif

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

    if (main_ctx.isOrp) {
        pthread_t tid_offline;
        pthread_create(&tid_offline, NULL, test_offline_thread, &main_ctx);
    }

//#define TEST_BLOCKED_STATS_FUNC
#ifdef TEST_BLOCKED_STATS_FUNC
    pthread_t stats_tid;
    pthread_create(&stats_tid, NULL, stats_thread, &main_ctx);
#endif
#ifdef CUSTOM_AF_DEMO_TEST
    custom_af_run(main_ctx.aiq_ctx);
#endif

    pthread_sigmask(SIG_UNBLOCK, &mask, NULL);

    mainloop(&main_ctx);
    if (main_ctx.isOrp) {
        main_ctx.orpStop = true;
        while (!main_ctx.orpStopped) {
            printf("wait orp stopped ... \n");
            usleep(500000);
        }
    }
    deinit(&main_ctx);

#if ISPDEMO_ENABLE_DRM
    if (strlen(main_ctx.dev_name) && strlen(main_ctx.dev_name2)) {
        display_exit();
    }
    deInitDrmDsp();
#endif
    return 0;
}

