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

#include "sample_comm.h"

static void sample_af_usage()
{
    printf("\n\nUsage : \n");
    printf("\t 0) AF:         setFocusMode-OP_AUTO.\n");
    printf("\t 1) AF:         setFocusMode-OP_MANUAL.\n");
    printf("\t 2) AF:         setFocusMode-OP_SEMI_AUTO.\n");
    printf("\t 3) AF:         getFocusMode.\n");
    printf("\t 4) AF:         setFocusWin.\n");
    printf("\t 5) AF:         setFocusWin to defalut.\n");
    printf("\t 6) AF:         getFocusWin.\n");
    printf("\t 7) AF:         lockFocus.\n");
    printf("\t 8) AF:         unlockFocus.\n");
    printf("\t 9) AF:         oneshotFocus.\n");
    printf("\t a) AF:         manualTrigerFocus.\n");
    printf("\t b) AF:         trackingFocus.\n");
    printf("\t c) AF:         startZoomCalib.\n");
    printf("\t d) AF:         resetZoom.\n");
    printf("\t e) AF:         setAngleZ.\n");
    printf("\t f) AF:         + ZoomPosition.\n");
    printf("\t g) AF:         - ZoomPosition.\n");
    printf("\t i) AF:         + FocusPosition.\n");
    printf("\t j) AF:         - FocusPosition.\n");
    printf("\t k) AF:         getSearchResult.\n");
    printf("\t l) AF:         setFocusMeasCfg sync.\n");
    printf("\t m) AF:         setFocusMeasCfg async.\n");
    printf("\t n) AF:         getAfAttrib.\n");
    printf("\t q) AF:         return to main sample screen.\n");

    printf("\n");
    printf("\t please press the key: ");

    return;
}

void sample_set_focus_automode(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_setFocusMode(ctx, OP_AUTO);
    printf("setFocusMode auto\n");
}

void sample_set_focus_manualmode(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_setFocusMode(ctx, OP_MANUAL);
    printf("setFocusMode manual\n");
}

void sample_set_focus_semiautomode(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_setFocusMode(ctx, OP_SEMI_AUTO);
    printf("setFocusMode semi-auto\n");
}

void sample_get_focus_mode(const rk_aiq_sys_ctx_t* ctx)
{
    opMode_t mode;

    rk_aiq_uapi2_getFocusMode(ctx, &mode);
    if (mode == OP_AUTO) {
        printf("getFocusMode auto\n");
    } else if (mode == OP_MANUAL) {
        printf("getFocusMode manual\n");
    } else if (mode == OP_SEMI_AUTO) {
        printf("getFocusMode semi-auto\n");
    } else {
        printf("getFocusMode unknow mode %d\n", mode);
    }
}

void sample_set_focus_win(const rk_aiq_sys_ctx_t* ctx)
{
    paRect_t rect;

    rect.x = 4;
    rect.y = 4;
    rect.w = 800;
    rect.h = 600;
    rk_aiq_uapi2_setFocusWin(ctx, &rect);
    printf("setFocusWin rect: x %d, y %d, w %d, h %d\n", rect.x, rect.y, rect.w, rect.h);
}

void sample_set_focus_defwin(const rk_aiq_sys_ctx_t* ctx)
{
    paRect_t rect;

    memset(&rect, 0, sizeof(rect));
    rk_aiq_uapi2_setFocusWin(ctx, &rect);
    printf("setFocusWin rect to default value\n");
}

void sample_get_focus_win(const rk_aiq_sys_ctx_t* ctx)
{
    paRect_t rect;

    memset(&rect, 0, sizeof(rect));
    rk_aiq_uapi2_getFocusWin(ctx, &rect);
    printf("getFocusWin rect: x %d, y %d, w %d, h %d\n", rect.x, rect.y, rect.w, rect.h);
}

void sample_lock_focus(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_lockFocus(ctx);
    printf("lockFocus\n");
}

void sample_unlock_focus(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_unlockFocus(ctx);
    printf("unlockFocus\n");
}

void sample_oneshot_focus(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_oneshotFocus(ctx);
    printf("oneshotFocus\n");
}

void sample_manual_triger_focus(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_manualTrigerFocus(ctx);
    printf("manualTrigerFocus\n");
}

void sample_tracking_focus(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_trackingFocus(ctx);
    printf("trackingFocus\n");
}

void sample_start_zoomcalib(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_startZoomCalib(ctx);
    printf("startZoomCalib\n");
}

void sample_reset_zoom(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_uapi2_resetZoom(ctx);
    printf("resetZoom\n");
}

void sample_set_anglez(const rk_aiq_sys_ctx_t* ctx)
{
    float angleZ = 90;
    rk_aiq_uapi2_setAngleZ(ctx, angleZ);
    printf("setAngleZ %f degree\n", angleZ);
}

void sample_add_zoom_position(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_zoomrange range;
    int code;

    rk_aiq_uapi2_getZoomRange(ctx, &range);
    printf("zoom.min_pos %d, zoom.max_pos %d\n", range.min_pos, range.max_pos);

    rk_aiq_uapi2_getOpZoomPosition(ctx, &code);
    printf("getOpZoomPosition code %d\n", code);

    code += 20;
    if (code > range.max_pos)
        code = range.max_pos;
    rk_aiq_uapi2_setOpZoomPosition(ctx, code);
    rk_aiq_uapi2_endOpZoomChange(ctx);
    printf("setOpZoomPosition %d\n", code);
}

void sample_sub_zoom_position(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_zoomrange range;
    int code;

    rk_aiq_uapi2_getZoomRange(ctx, &range);
    printf("zoom.min_pos %d, zoom.max_pos %d\n", range.min_pos, range.max_pos);

    rk_aiq_uapi2_getOpZoomPosition(ctx, &code);
    printf("getOpZoomPosition code %d\n", code);

    code -= 20;
    if (code < range.min_pos)
        code = range.min_pos;
    rk_aiq_uapi2_setOpZoomPosition(ctx, code);
    rk_aiq_uapi2_endOpZoomChange(ctx);
    printf("setOpZoomPosition %d\n", code);
}

void sample_add_focus_position(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_focusrange range;
    short code;

    rk_aiq_uapi2_getFocusRange(ctx, &range);
    printf("focus.min_pos %d, focus.max_pos %d\n", range.min_pos, range.max_pos);

    rk_aiq_uapi2_getFocusPosition(ctx, &code);

    code++;
    if (code > range.max_pos)
        code = range.max_pos;
    rk_aiq_uapi2_setFocusPosition(ctx, code);
    printf("set Focus Code %d\n", code);
}

void sample_sub_focus_position(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_focusrange range;
    short code;

    rk_aiq_uapi2_getFocusRange(ctx, &range);
    printf("focus.min_pos %d, focus.max_pos %d\n", range.min_pos, range.max_pos);

    rk_aiq_uapi2_getFocusPosition(ctx, &code);

    code--;
    if (code < range.min_pos)
        code = range.min_pos;
    rk_aiq_uapi2_setFocusPosition(ctx, code);
    printf("set Focus Code %d\n", code);
}

void sample_get_af_search_result(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_result_t result;
    rk_aiq_uapi2_getSearchResult(ctx, &result);
    printf("get search result, stat: %d, final_pos: %d\n", result.stat, result.final_pos);
}

void sample_set_af_manual_meascfg(const rk_aiq_sys_ctx_t* ctx, rk_aiq_uapi_mode_sync_e sync_mode)
{
    rk_aiq_af_attrib_t attr;
    uint16_t gamma_y[RKAIQ_RAWAF_GAMMA_NUM] =
        {0, 45, 108, 179, 245, 344, 409, 459, 500, 567, 622, 676, 759, 833, 896, 962, 1023};

    rk_aiq_user_api2_af_GetAttrib(ctx, &attr);
    attr.AfMode = RKAIQ_AF_MODE_FIXED;
    if (CHECK_ISP_HW_V20() || CHECK_ISP_HW_V21()) {
        memset(&attr.manual_meascfg, 0, sizeof(attr.manual_meascfg));
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
        attr.manual_meascfg.sp_meas.ldg_kl = (255-28)*256/45;
        attr.manual_meascfg.sp_meas.ldg_xh = 118;
        attr.manual_meascfg.sp_meas.ldg_yh = 8;
        attr.manual_meascfg.sp_meas.ldg_kh = (255-8)*256/15;
        attr.manual_meascfg.sp_meas.highlight_th = 245;
        attr.manual_meascfg.sp_meas.highlight2_th = 200;
    } else if (CHECK_ISP_HW_V30()) {
        memset(&attr.manual_meascfg_v30, 0, sizeof(attr.manual_meascfg_v30));
        attr.manual_meascfg_v30.af_en = 1;
        attr.manual_meascfg_v30.rawaf_sel = 0; // normal = 0; hdr = 1
        attr.manual_meascfg_v30.accu_8bit_mode = 1;
        attr.manual_meascfg_v30.ae_mode = 1;

        attr.manual_meascfg_v30.window_num = 2;
        attr.manual_meascfg_v30.wina_h_offs = 2;
        attr.manual_meascfg_v30.wina_v_offs = 2;
        attr.manual_meascfg_v30.wina_h_size = 2580;
        attr.manual_meascfg_v30.wina_v_size = 1935;

        attr.manual_meascfg_v30.winb_h_offs = 500;
        attr.manual_meascfg_v30.winb_v_offs = 600;
        attr.manual_meascfg_v30.winb_h_size = 300;
        attr.manual_meascfg_v30.winb_v_size = 300;

        attr.manual_meascfg_v30.gamma_en = 1;
        memcpy(attr.manual_meascfg_v30.gamma_y, gamma_y, RKAIQ_RAWAF_GAMMA_NUM * sizeof(uint16_t));

        // param for winb
        attr.manual_meascfg_v30.thres = 4;
        attr.manual_meascfg_v30.shift_sum_a = 0;
        attr.manual_meascfg_v30.shift_y_a = 0;
        attr.manual_meascfg_v30.shift_sum_b = 1;
        attr.manual_meascfg_v30.shift_y_b = 1;

        // Vertical filter
        // lowlit [0.025, 0.075], max=0.5
        int ver_flt_lowlit[6] =
            { 503, 8, 9, -1997, 0, 1997 };

        // normal [0.042, 0.14], max=0.5
        int ver_flt_normal[6] =
            { 483, 28, 28, -1186, 0, 1186 };

        attr.manual_meascfg_v30.gaus_en = 1;
        attr.manual_meascfg_v30.v1_fir_sel = 1; // 0:old 1:new
        attr.manual_meascfg_v30.viir_en = 1;
        attr.manual_meascfg_v30.v1_fv_outmode = 0; // 0 square, 1 absolute
        attr.manual_meascfg_v30.v2_fv_outmode = 0; // 0 square, 1 absolute
        attr.manual_meascfg_v30.v1_fv_shift = 1; //only for sel1
        attr.manual_meascfg_v30.v2_fv_shift = 1;
        attr.manual_meascfg_v30.v_fv_thresh = 0;
        attr.manual_meascfg_v30.v1_iir_coe[1] = ver_flt_lowlit[0];
        attr.manual_meascfg_v30.v1_iir_coe[4] = ver_flt_lowlit[1];
        attr.manual_meascfg_v30.v1_iir_coe[7] = ver_flt_lowlit[2];
        for (int i = 0; i < 3; i++) {
            attr.manual_meascfg_v30.v1_fir_coe[i] = ver_flt_lowlit[i + 3];
            attr.manual_meascfg_v30.v2_iir_coe[i] = ver_flt_normal[i];
            attr.manual_meascfg_v30.v2_fir_coe[i] = ver_flt_normal[i + 3];
        }

        // Horizontal filter
        // lowlit [0.025, 0.075], max=0.5
        int hor_flt_lowlit[2][6] =
        {
            { 203, 811, -375, 673, 0, -673 },
            { 31,  945, -448, 323, 0, -323 },
        };
        // normal [0.042, 0.14], max=0.5
        int hor_flt_normal[2][6] =
        {
            { 512, 557, -276, 460, 0, -460 },
            { 100, 870, -399, 191, 0, -191 },
        };

        attr.manual_meascfg_v30.hiir_en = 1;
        attr.manual_meascfg_v30.h1_fv_outmode = 0; // 0 square, 1 absolute
        attr.manual_meascfg_v30.h2_fv_outmode = 0; // 0 square, 1 absolute
        attr.manual_meascfg_v30.h1_fv_shift = 1;
        attr.manual_meascfg_v30.h2_fv_shift = 1;
        attr.manual_meascfg_v30.h_fv_thresh = 0;
        for (int i = 0; i < 6; i++) {
            attr.manual_meascfg_v30.h1_iir1_coe[i] = hor_flt_lowlit[0][i];
            attr.manual_meascfg_v30.h1_iir2_coe[i] = hor_flt_lowlit[1][i];
            attr.manual_meascfg_v30.h2_iir1_coe[i] = hor_flt_normal[0][i];
            attr.manual_meascfg_v30.h2_iir2_coe[i] = hor_flt_normal[1][i];
        }

        // level depended gain
        attr.manual_meascfg_v30.ldg_en = 0;
        attr.manual_meascfg_v30.h_ldg_lumth[0] = 64;
        attr.manual_meascfg_v30.h_ldg_gain[0]  = 28;
        attr.manual_meascfg_v30.h_ldg_gslp[0]  = (255-28)*255/45;
        attr.manual_meascfg_v30.h_ldg_lumth[1] = 185;
        attr.manual_meascfg_v30.h_ldg_gain[1]  = 8;
        attr.manual_meascfg_v30.h_ldg_gslp[1]  = (255-8)*255/45;
        attr.manual_meascfg_v30.v_ldg_lumth[0] = 64;
        attr.manual_meascfg_v30.v_ldg_gain[0]  = 28;
        attr.manual_meascfg_v30.v_ldg_gslp[0]  = (255-28)*255/45;
        attr.manual_meascfg_v30.v_ldg_lumth[1] = 185;
        attr.manual_meascfg_v30.v_ldg_gain[1]  = 8;
        attr.manual_meascfg_v30.v_ldg_gslp[1]  = (255-8)*255/45;

        // High light
        attr.manual_meascfg_v30.highlit_thresh = 912;
    } else if (CHECK_ISP_HW_V32()) {
        // rk1106
        memset(&attr.manual_meascfg_v31, 0, sizeof(attr.manual_meascfg_v31));
        attr.manual_meascfg_v31.af_en = 1;
        attr.manual_meascfg_v31.rawaf_sel = 0; // normal = 0; hdr = 1
        attr.manual_meascfg_v31.accu_8bit_mode = 1;
        attr.manual_meascfg_v31.ae_mode = 1;
        attr.manual_meascfg_v31.v_dnscl_mode = 1;

        attr.manual_meascfg_v31.window_num = 2;
        attr.manual_meascfg_v31.wina_h_offs = 2;
        attr.manual_meascfg_v31.wina_v_offs = 2;
        attr.manual_meascfg_v31.wina_h_size = 2580;
        attr.manual_meascfg_v31.wina_v_size = 1935;

        attr.manual_meascfg_v31.winb_h_offs = 500;
        attr.manual_meascfg_v31.winb_v_offs = 600;
        attr.manual_meascfg_v31.winb_h_size = 300;
        attr.manual_meascfg_v31.winb_v_size = 300;

        attr.manual_meascfg_v31.gamma_en = 1;
        memcpy(attr.manual_meascfg_v31.gamma_y, gamma_y, RKAIQ_RAWAF_GAMMA_NUM * sizeof(uint16_t));

        // param for winb
        attr.manual_meascfg_v31.thres = 4;
        attr.manual_meascfg_v31.shift_sum_a = 0;
        attr.manual_meascfg_v31.shift_y_a = 0;
        attr.manual_meascfg_v31.shift_sum_b = 1;
        attr.manual_meascfg_v31.shift_y_b = 1;

        // Vertical filter
        // lowlit [0.025, 0.075], max=0.5
        int ver_flt_lowlit[6] =
            { -372, 851, 465, -77, 0, 77 };

        // normal [0.042, 0.14], max=0.5
        int ver_flt_normal[6] =
            { -265, 686, 512, -124, 0, 124 };

        attr.manual_meascfg_v31.gaus_en = 1;
        attr.manual_meascfg_v31.gaus_coe[1] = 64;
        attr.manual_meascfg_v31.gaus_coe[4] = 64;
        attr.manual_meascfg_v31.v1_fir_sel = 1; // 0:old 1:new
        attr.manual_meascfg_v31.viir_en = 1;
        attr.manual_meascfg_v31.v1_fv_outmode = 0; // 0 square, 1 absolute
        attr.manual_meascfg_v31.v2_fv_outmode = 0; // 0 square, 1 absolute
        attr.manual_meascfg_v31.v1_fv_shift = 1; //only for sel1
        attr.manual_meascfg_v31.v2_fv_shift = 1;
        attr.manual_meascfg_v31.v_fv_thresh = 0;
        for (int i = 0; i < 3; i++) {
            attr.manual_meascfg_v31.v1_iir_coe[i] = ver_flt_lowlit[i];
            attr.manual_meascfg_v31.v1_fir_coe[i] = ver_flt_lowlit[i + 3];
            attr.manual_meascfg_v31.v2_iir_coe[i] = ver_flt_normal[i];
            attr.manual_meascfg_v31.v2_fir_coe[i] = ver_flt_normal[i + 3];
        }

        // Horizontal filter
        // lowlit [0.025, 0.075], max=0.5
        int hor_flt_lowlit[2][6] =
        {
            { 203, 811, -375, 673, 0, -673 },
            { 31,  945, -448, 323, 0, -323 },
        };
        // normal [0.042, 0.14], max=0.5
        int hor_flt_normal[2][6] =
        {
            { 512, 557, -276, 460, 0, -460 },
            { 100, 870, -399, 191, 0, -191 },
        };
        attr.manual_meascfg_v31.hiir_en = 1;
        attr.manual_meascfg_v31.h1_fv_outmode = 0; // 0 square, 1 absolute
        attr.manual_meascfg_v31.h2_fv_outmode = 0; // 0 square, 1 absolute
        attr.manual_meascfg_v31.h1_fv_shift = 1;
        attr.manual_meascfg_v31.h2_fv_shift = 1;
        attr.manual_meascfg_v31.h_fv_thresh = 0;
        for (int i = 0; i < 6; i++) {
            attr.manual_meascfg_v31.h1_iir1_coe[i] = hor_flt_lowlit[0][i];
            attr.manual_meascfg_v31.h1_iir2_coe[i] = hor_flt_lowlit[1][i];
            attr.manual_meascfg_v31.h2_iir1_coe[i] = hor_flt_normal[0][i];
            attr.manual_meascfg_v31.h2_iir2_coe[i] = hor_flt_normal[1][i];
        }

        // level depended gain
        attr.manual_meascfg_v31.ldg_en = 0;
        attr.manual_meascfg_v31.h_ldg_lumth[0] = 64;
        attr.manual_meascfg_v31.h_ldg_gain[0]  = 28;
        attr.manual_meascfg_v31.h_ldg_gslp[0]  = (255-28)*255/45;
        attr.manual_meascfg_v31.h_ldg_lumth[1] = 185;
        attr.manual_meascfg_v31.h_ldg_gain[1]  = 8;
        attr.manual_meascfg_v31.h_ldg_gslp[1]  = (255-8)*255/45;
        attr.manual_meascfg_v31.v_ldg_lumth[0] = 64;
        attr.manual_meascfg_v31.v_ldg_gain[0]  = 28;
        attr.manual_meascfg_v31.v_ldg_gslp[0]  = (255-28)*255/45;
        attr.manual_meascfg_v31.v_ldg_lumth[1] = 185;
        attr.manual_meascfg_v31.v_ldg_gain[1]  = 8;
        attr.manual_meascfg_v31.v_ldg_gslp[1]  = (255-8)*255/45;

        // High light
        attr.manual_meascfg_v31.highlit_thresh = 912;
    }

    attr.sync.sync_mode = sync_mode;
    rk_aiq_user_api2_af_SetAttrib(ctx, &attr);
    printf("setFocusMeasCfg\n");
}

void sample_get_af_attrib(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_attrib_t attr;

    rk_aiq_user_api2_af_GetAttrib(ctx, &attr);
    printf("get sync.done %d, sync_mode %d\n", attr.sync.done, attr.sync.sync_mode);
}

void sample_print_af_info(const void *arg)
{
    printf ("enter AF modult test!\n");
}

XCamReturn sample_af_module(const void *arg)
{
    int key = -1;
    CLEAR();
    rk_aiq_wb_scene_t scene;
    rk_aiq_wb_gain_t gain;
    rk_aiq_wb_cct_t ct;
    opMode_t mode;
    const demo_context_t *demo_ctx = (demo_context_t *)arg;
    const rk_aiq_sys_ctx_t* ctx = (const rk_aiq_sys_ctx_t*)(demo_ctx->aiq_ctx);
    if (ctx == nullptr) {
        ERR ("%s, ctx is nullptr\n", __FUNCTION__);
        return XCAM_RETURN_ERROR_PARAM;
    }

    do {
        sample_af_usage();

        key = getchar ();
        while (key == '\n' || key == '\r')
            key = getchar();
        printf ("\n\n");

        switch (key)
        {
            case 'h':
                sample_af_usage();
                break;
            case '0':
                sample_set_focus_automode(ctx);
                break;
            case '1':
                sample_set_focus_manualmode(ctx);
                break;
            case '2':
                sample_set_focus_semiautomode(ctx);
                break;
            case '3':
                sample_get_focus_mode(ctx);
                break;
            case '4':
                sample_set_focus_win(ctx);
                break;
            case '5':
                sample_set_focus_defwin(ctx);
                break;
            case '6':
                sample_get_focus_win(ctx);
                break;
            case '7':
                sample_lock_focus(ctx);
                break;
            case '8':
                sample_unlock_focus(ctx);
                break;
            case '9':
                sample_oneshot_focus(ctx);
                break;
            case 'a':
                sample_manual_triger_focus(ctx);
                break;
            case 'b':
                sample_tracking_focus(ctx);
                break;
            case 'c':
                sample_start_zoomcalib(ctx);
                break;
            case 'd':
                sample_reset_zoom(ctx);
                break;
            case 'e':
                sample_set_anglez(ctx);
                break;
            case 'f':
                sample_add_zoom_position(ctx);
                break;
            case 'g':
                sample_sub_zoom_position(ctx);
                break;
            case 'i':
                sample_add_focus_position(ctx);
                break;
            case 'j':
                sample_sub_focus_position(ctx);
                break;
            case 'k':
                sample_get_af_search_result(ctx);
                break;
            case 'l':
                sample_set_af_manual_meascfg(ctx, RK_AIQ_UAPI_MODE_SYNC);
                break;
            case 'm':
                sample_set_af_manual_meascfg(ctx, RK_AIQ_UAPI_MODE_ASYNC);
                break;
            case 'n':
                sample_get_af_attrib(ctx);
                break;
            default:
                break;
        }
    } while (key != 'q' && key != 'Q');

    return XCAM_RETURN_NO_ERROR;
}


