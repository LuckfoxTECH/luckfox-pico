/*
 *  Copyright (c) 2019 Rockchip Corporation
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

#ifdef SAMPLE_SMART_IR

#include <fcntl.h>
#include <linux/v4l2-subdev.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "rk_smart_ir_api.h"
#include "rkisp_demo.h"
#include "uAPI/rk_aiq_user_api_ae.h"
#include "uAPI/rk_aiq_user_api_sysctl.h"

#define CLEAR() printf("\x1b[2J\x1b[H");

#define RK_SMART_IR_AUTO_IRLED false

static void sample_smartIr_usage()
{
    printf("Usage : \n");
    printf("  SmartIr API: \n");
    printf("\t s) SmartIr:         Start SmartIr test.\n");
    printf("\t e) SmartIr:         Exit SmartIr test.\n");
    printf("\t c) SmartIr:         Ir wb calibration.\n");
    printf("\n");
    printf("\t h) SmartIr:         help.\n");
    printf("\t q) SmartIr:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_print_smartIr_info(const void* arg)
{
    printf("enter SmartIr modult test!\n");
}

typedef struct sample_smartIr_s {
    pthread_t tid;
    bool tquit;
    const rk_aiq_sys_ctx_t* aiq_ctx;
    bool started;
    const char* ir_cut_v4ldev;
    const char* ir_v4ldev;
    rk_smart_ir_ctx_t* ir_ctx;
    rk_smart_ir_params_t ir_configs;
} sample_smartIr_t;

static sample_smartIr_t g_sample_smartIr_ctx;

static void enableIrCutter(bool on)
{
    sample_smartIr_t* smartIr_ctx = &g_sample_smartIr_ctx;

    struct v4l2_control control;

    control.id = V4L2_CID_BAND_STOP_FILTER;
    if (on)
        control.value = 3; // filter ir
    else
        control.value = 0; // ir in

    int _fd = open(smartIr_ctx->ir_cut_v4ldev, O_RDWR | O_CLOEXEC);
    if (_fd != -1) {
        if (ioctl(_fd, VIDIOC_S_CTRL, &control) < 0) {
            printf("failed to set ircut value %d to device!\n", control.value);
        }
        close(_fd);
    }
}

static void switch_to_day()
{
    // ir-cutter on
    enableIrCutter(true);
    // ir off
    // switch to isp day params
}

static void switch_to_night()
{
    // switch to isp night params
    // ir-cutter off
    enableIrCutter(false);
    // ir on
}

static void load_ir_configs()
{
    sample_smartIr_t* smartIr_ctx = &g_sample_smartIr_ctx;
    rk_smart_ir_result_t ir_init_res;

    smartIr_ctx->ir_cut_v4ldev = NULL;
    smartIr_ctx->ir_v4ldev = NULL;

    smartIr_ctx->ir_cut_v4ldev = "/dev/v4l-subdev3";
    smartIr_ctx->ir_configs.d2n_envL_th = 0.04f;
    smartIr_ctx->ir_configs.n2d_envL_th = 0.20f;
    smartIr_ctx->ir_configs.rggain_base = 1.0f;
    smartIr_ctx->ir_configs.bggain_base = 1.0f;
    smartIr_ctx->ir_configs.awbgain_rad = 0.10f;
    smartIr_ctx->ir_configs.awbgain_dis = 0.22f;
    smartIr_ctx->ir_configs.switch_cnts_th = 100;
    rk_smart_ir_config(smartIr_ctx->ir_ctx, &smartIr_ctx->ir_configs);
    // set initial status to day
    ir_init_res.status = RK_SMART_IR_STATUS_DAY;
    rk_smart_ir_set_status(smartIr_ctx->ir_ctx, ir_init_res);
    switch_to_day();
}

static void* switch_ir_thread(void* args)
{
    sample_smartIr_t* smartIr_ctx = &g_sample_smartIr_ctx;
    rk_smart_ir_result_t ir_res;
    rk_aiq_isp_stats_t* stats_ref = NULL;
    XCamReturn ret = XCAM_RETURN_NO_ERROR;

    rk_smart_ir_autoled_t auto_irled = { 0 };
    int irled_cur_value = 100;
    if (RK_SMART_IR_AUTO_IRLED) {
        // TODO: set init irled pwm duty
        auto_irled.is_smooth_convert = false;
        auto_irled.auto_irled_val = irled_cur_value;
        auto_irled.auto_irled_min = 10;
        auto_irled.auto_irled_max = 100;
    }

    while (!smartIr_ctx->tquit) {
        ret = rk_aiq_uapi_sysctl_get3AStatsBlk(smartIr_ctx->aiq_ctx, &stats_ref, -1);
        if (ret == XCAM_RETURN_NO_ERROR && stats_ref != NULL) {
            rk_smart_ir_runOnce(smartIr_ctx->ir_ctx, stats_ref, &ir_res);

            rk_aiq_uapi_sysctl_release3AStatsRef(smartIr_ctx->aiq_ctx, stats_ref);

            if (RK_SMART_IR_AUTO_IRLED) {
                rk_smart_ir_auto_irled(smartIr_ctx->ir_ctx, &auto_irled);
                if (irled_cur_value != auto_irled.auto_irled_val) {
                    irled_cur_value = auto_irled.auto_irled_val;
                    // TODO: update irled pwm duty
                }
            }

            if (ir_res.status == RK_SMART_IR_STATUS_DAY) {
                switch_to_day();
            } else if (ir_res.status == RK_SMART_IR_STATUS_NIGHT) {
                switch_to_night();
            } else {
            }
            printf("SAMPLE_SMART_IR: switch to %s\n", ir_res.status == RK_SMART_IR_STATUS_DAY ? "DAY" : "Night");
        } else {
            if (ret == XCAM_RETURN_NO_ERROR) {
                printf("aiq has stopped !\n");
            } else if (ret == XCAM_RETURN_ERROR_TIMEOUT) {
                printf("aiq timeout!\n");
            } else if (ret == XCAM_RETURN_ERROR_FAILED) {
                printf("aiq failed!\n");
            }
        }
    }

    return NULL;
}

static void sample_smartIr_start(const void* arg)
{
    const rk_aiq_sys_ctx_t* ctx = (rk_aiq_sys_ctx_t*)(arg);
    sample_smartIr_t* smartIr_ctx = &g_sample_smartIr_ctx;

    smartIr_ctx->ir_ctx = rk_smart_ir_init((rk_aiq_sys_ctx_t*)arg);
    load_ir_configs();

    smartIr_ctx->tquit = false;
    pthread_create(&smartIr_ctx->tid, NULL, switch_ir_thread, NULL);
    smartIr_ctx->started = true;
}

static void sample_smartIr_stop(const void* arg)
{
    const rk_aiq_sys_ctx_t* ctx = (rk_aiq_sys_ctx_t*)(arg);
    sample_smartIr_t* smartIr_ctx = &g_sample_smartIr_ctx;

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

static void sample_smartIr_calib(const void* arg)
{
    const rk_aiq_sys_ctx_t* ctx = (rk_aiq_sys_ctx_t*)(arg);

    // 1. make sure no visible light
    // 2. ircutter off, ir on
    switch_to_night();
    // 3. query wb info
    float RGgain = 0.0f, BGgain = 0.0f;
    int counts = 0;
    rk_aiq_isp_stats_t* stats_ref = NULL;
    printf("SmartIr Calib start ...... \n");
    XCamReturn ret = XCAM_RETURN_NO_ERROR;
    while (counts++ < 100) {
        ret = rk_aiq_uapi_sysctl_get3AStatsBlk(ctx, &stats_ref, -1);
        if (ret == XCAM_RETURN_NO_ERROR && stats_ref != NULL) {
            printf("stats frame id %d \n", stats_ref->frame_id);
            float Rvalue = 0, Gvalue = 0, Bvalue = 0, RGgain = 0, BGgain = 0;
            for (int i = 0; i < RK_AIQ_AWB_GRID_NUM_TOTAL; i++) {
                Rvalue = (float)stats_ref->awb_stats_v200.blockResult[i].Rvalue;
                Gvalue = (float)stats_ref->awb_stats_v200.blockResult[i].Gvalue;
                Bvalue = (float)stats_ref->awb_stats_v200.blockResult[i].Bvalue;
                RGgain = RGgain + Rvalue / Gvalue;
                BGgain = BGgain + Bvalue / Gvalue;
            }
            RGgain /= RK_AIQ_AWB_GRID_NUM_TOTAL;
            BGgain /= RK_AIQ_AWB_GRID_NUM_TOTAL;
            printf("origin rggain_base:%0.3f, bggain_base:%0.3f\n", RGgain, BGgain);
            rk_aiq_uapi_sysctl_release3AStatsRef(ctx, stats_ref);
        } else {
            if (ret == XCAM_RETURN_NO_ERROR) {
                printf("aiq has stopped !\n");
            } else if (ret == XCAM_RETURN_ERROR_TIMEOUT) {
                printf("aiq timeout!\n");
            } else if (ret == XCAM_RETURN_ERROR_FAILED) {
                printf("aiq failed!\n");
            }
        }
    }
    printf("SmartIr Calib Done ...... \n");
}

XCamReturn sample_smartIr_module(const void* arg)
{
    int key = -1;
    CLEAR();

    const demo_context_t* demo_ctx = (demo_context_t*)arg;
    const rk_aiq_sys_ctx_t* ctx;
    /*
    if (demo_ctx->camGroup) {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->camgroup_ctx);
    } else {
        ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    }*/
    ctx = (rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);

    if (ctx == NULL) {
        ERR("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    sample_smartIr_usage();

    g_sample_smartIr_ctx.tquit = false;
    g_sample_smartIr_ctx.started = false;
    g_sample_smartIr_ctx.aiq_ctx = ctx;
    g_sample_smartIr_ctx.ir_ctx = NULL;

    do {
        key = getchar();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf("\n");

        switch (key) {
        case 'h':
            CLEAR();
            sample_smartIr_usage();
            break;
        case 'e':
            sample_smartIr_stop(ctx);
            break;
        case 's':
            sample_smartIr_start(ctx);
            break;
        case 'c':
            sample_smartIr_calib(ctx);
            break;
        default:
            break;
        }
    } while (key != 'q' && key != 'Q');

    sample_smartIr_stop(ctx);

    return XCAM_RETURN_NO_ERROR;
}

#else
#include <stdio.h>
#include "base/xcam_common.h"

void sample_print_smartIr_info(const void* arg)
{
    printf("enter SmartIr modult test!\n");
}

XCamReturn sample_smartIr_module(const void* arg)
{
    printf("Not enabled! Add option SAMPLE_SMART_IR in makefile \n");
    return XCAM_RETURN_NO_ERROR;
}
#endif
