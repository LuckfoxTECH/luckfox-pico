#include "third_party_af_algo.h"
#include "rk_aiq_user_api2_sysctl.h"

static void set_af_manual_meascfg(const rk_aiq_sys_ctx_t* ctx)
{
    rk_aiq_af_attrib_t attr;
    uint16_t gamma_y[RKAIQ_RAWAF_GAMMA_NUM] =
        {0, 45, 108, 179, 245, 344, 409, 459, 500, 567, 622, 676, 759, 833, 896, 962, 1023};

    rk_aiq_user_api2_af_GetAttrib(ctx, &attr);
    attr.AfMode = RKAIQ_AF_MODE_FIXED;
    printf("AfHwVer %d\n", attr.AfHwVer);
    if (attr.AfHwVer == RKAIQ_AF_HW_V20) {
        // rv1126/rv1109 rk356x
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
    } else if (attr.AfHwVer == RKAIQ_AF_HW_V30) {
        // rk3588
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
        // face [0.025, 0.06], max=0.5
        int ver_flt_face[6] =
            { 503, 8, 9, -1997, 0, 1997 };

        // lowlit [0.025, 0.075], max=0.5
        int ver_flt_lowlit[6] =
            { 503, 8, 9, -1997, 0, 1997 };

        // normal [0.042, 0.14], max=0.5
        int ver_flt_normal[6] =
            { 483, 28, 28, -1186, 0, 1186 };

        // high [0.055, 0.125], max=0.5
        int ver_flt_high[6] =
            { 483, 27, 28, -1212, 0, 1212 };

        // dotlight [0.1 0.175], max=0.5
        int ver_flt_dotlight[6] =
            { 445, 63, 64, -531, 0, 531 };

        // full [0.025, 0.175], max=0.5
        int ver_flt_full[6] =
            { 476, 33, 34, -985, 0, 985 };

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
        // low [0.0125, 0.03], max=0.5
        int hor_flt_low[2][6] =
        {
            { 31, 960, -462, 546, 0, -546 },
            { 17, 994, -486, 665, 0, -665 },
        };
        // face [0.025, 0.06], max=0.5
        int hor_flt_face[2][6] =
        {
            { 117, 877, -417, 805, 0, -805 },
            { 23,  957, -460, 334, 0, -334 },
        };
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
        // high [0.055, 0.125], max=0.5
        int hor_flt_high[2][6] =
        {
            { 415, 648, -344, 403, 0, -403 },
            { 96,  854, -409, 156, 0, -156 },
        };
        // dotlight [0.1 0.175], max=0.5
        int hor_flt_dotlight[2][6] =
        {
            { 512, 447, -349, 319, 0, -319 },
            { 181, 698, -386, 96, 0, -96 },
        };
        // full [0.025, 0.175], max=0.5
        int hor_flt_full[2][6] =
        {
            { 512, 362, -171, 512, 0, -512 },
            { 114, 915, -417, 302, 0, -302 },
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
    } else if (attr.AfHwVer == RKAIQ_AF_HW_V31) {
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
        attr.manual_meascfg_v31.wina_h_size = 2550;
        attr.manual_meascfg_v31.wina_v_size = 1425;

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
        // face [0.025, 0.06], max=0.5
        int ver_flt_face[6] =
            { -410, 895, 330, -79, 0, 79 };

        // lowlit [0.025, 0.075], max=0.5
        int ver_flt_lowlit[6] =
            { -372, 851, 465, -77, 0, 77 };

        // normal [0.042, 0.14], max=0.5
        int ver_flt_normal[6] =
            { -265, 686, 512, -124, 0, 124 };

        // high [0.055, 0.125], max=0.5
        int ver_flt_high[6] =
            { -325, 724, 512, -94, 0, 94 };

        // dotlight [0.1 0.175], max=0.5
        int ver_flt_dotlight[6] =
            { -314, 552, 512, -99, 0, 99 };

        // full [0.025, 0.175], max=0.5
        int ver_flt_full[6] =
            { -166, 616, 512, -173, 0, 173 };

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
        // low [0.0125, 0.03], max=0.5
        int hor_flt_low[2][6] =
        {
            { 31, 960, -462, 491, 0, -491 },
            { 19, 994, -486, 665, 0, -665 },
        };
        // face [0.025, 0.06], max=0.5
        int hor_flt_face[2][6] =
        {
            { 117, 877, -417, 805, 0, -805 },
            { 23,  957, -460, 334, 0, -334 },
        };
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
        // high [0.055, 0.125], max=0.5
        int hor_flt_high[2][6] =
        {
            { 415, 648, -344, 403, 0, -403 },
            { 96,  854, -409, 156, 0, -156 },
        };
        // dotlight [0.1 0.175], max=0.5
        int hor_flt_dotlight[2][6] =
        {
            { 512, 447, -349, 319, 0, -319 },
            { 181, 698, -386,  96, 0, -96 },
        };
        // full [0.025, 0.175], max=0.5
        int hor_flt_full[2][6] =
        {
            { 512, 362, -171, 512, 0, -512 },
            { 114, 915, -417, 302, 0, -302 },
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

    attr.sync.sync_mode = RK_AIQ_UAPI_MODE_SYNC;
    rk_aiq_user_api2_af_SetAttrib(ctx, &attr);
    printf("setFocusMeasCfg\n");
}

static void print_af_stats(rk_aiq_isp_stats_t *stats_ref)
{
    unsigned long sof_time;

    // show af stats every 30 frames
    if (stats_ref->frame_id % 30 != 0)
        return;

    if (stats_ref->af_hw_ver == RKAIQ_AF_HW_V20) {
        // rv1126/rv1109 rk356x
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
    } else if (stats_ref->af_hw_ver == RKAIQ_AF_HW_V30 || stats_ref->af_hw_ver == RKAIQ_AF_HW_V31) {
        // rk3588 & rk1106
        sof_time = stats_ref->af_stats_v3x.sof_tim / 1000000LL;
        printf("wnda_fv_h1\n");
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                printf("0x%08x, ", stats_ref->af_stats_v3x.wnda_fv_h1[15 * i + j]);
            }
            printf("\n");
        }
        printf("wnda_fv_h2\n");
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                printf("0x%08x, ", stats_ref->af_stats_v3x.wnda_fv_h2[15 * i + j]);
            }
            printf("\n");
        }
        printf("wnda_fv_v1\n");
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                printf("0x%08x, ", stats_ref->af_stats_v3x.wnda_fv_v1[15 * i + j]);
            }
            printf("\n");
        }
        printf("wnda_fv_v2\n");
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                printf("0x%08x, ", stats_ref->af_stats_v3x.wnda_fv_v2[15 * i + j]);
            }
            printf("\n");
        }
        printf("wnda_luma\n");
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                printf("0x%08x, ", stats_ref->af_stats_v3x.wnda_luma[15 * i + j]);
            }
            printf("\n");
        }
        printf("wina_highlit_cnt\n");
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                printf("0x%08x, ", stats_ref->af_stats_v3x.wina_highlit_cnt[15 * i + j]);
            }
            printf("\n");
        }
    }
}

static void custom_af_algo(rk_aiq_isp_stats_t *stats_ref)
{
    // show af stats
    print_af_stats(stats_ref);

    // run af algo

    // move zoom/focus motor

}

static void* af_thread(void* args) {
    rk_aiq_sys_ctx_t* ctx = (rk_aiq_sys_ctx_t*)args;
    XCamReturn ret;
    pthread_detach (pthread_self());
    printf("begin af thread\n");

    // set af meas config
    set_af_manual_meascfg(ctx);
    while(1) {
        rk_aiq_isp_stats_t *stats_ref = NULL;
        // get 3a stats
        ret = rk_aiq_uapi2_sysctl_get3AStatsBlk(ctx, &stats_ref, -1);
        if (ret == XCAM_RETURN_NO_ERROR && stats_ref != NULL) {
            printf("get one stats frame id %d \n", stats_ref->frame_id);
            custom_af_algo(stats_ref);
            // release 3a stats
            rk_aiq_uapi2_sysctl_release3AStatsRef(ctx, stats_ref);
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

int32_t custom_af_run(rk_aiq_sys_ctx_t* ctx)
{
    pthread_t af_tid;
    pthread_create(&af_tid, NULL, af_thread, ctx);

    return 0;
}
