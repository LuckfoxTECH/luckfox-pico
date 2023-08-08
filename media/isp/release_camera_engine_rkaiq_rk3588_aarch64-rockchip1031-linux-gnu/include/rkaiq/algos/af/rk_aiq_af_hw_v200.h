#ifndef __RK_AIQ_AF_HW_V200_H__
#define __RK_AIQ_AF_HW_V200_H__

#include "rk_aiq_types_af_algo.h"

#define ISP_SHARPNESS_GRID_ITEMS 225

typedef struct {
    unsigned char sw_rawaf_en;

    unsigned char window_num;
    unsigned short wina_h_offs;
    unsigned short wina_v_offs;
    unsigned short wina_h_size;
    unsigned short wina_v_size;

    unsigned short winb_h_offs;
    unsigned short winb_v_offs;
    unsigned short winb_h_size;
    unsigned short winb_v_size;

    unsigned char gamma_flt_en;

    unsigned short gamma_y1;
    unsigned short gamma_y2;
    unsigned short gamma_y3;
    unsigned short gamma_y4;
    unsigned short gamma_y5;
    unsigned short gamma_y6;
    unsigned short gamma_y7;
    unsigned short gamma_y8;

    unsigned char gaus_flt_en;

    unsigned char gaus_h0;
    unsigned char gaus_h1;
    unsigned char gaus_h2;

    unsigned char int_lin0_en;
    unsigned char int_lin1_en;
    unsigned char int_lin2_en;
    unsigned char int_lin3_en;
    unsigned char int_lin4_en;

    unsigned char int_lin0;
    unsigned char int_lin1;
    unsigned char int_lin2;
    unsigned char int_lin3;
    unsigned char int_lin4;

    unsigned int roi_threshold;
    unsigned int global_threshold;

    unsigned char afm_var_shift_a;
    unsigned char afm_var_shift_b;
    unsigned char lum_var_shift_a;
    unsigned char lum_var_shift_b;

    unsigned long long roia_sharpness;
    unsigned int roia_luminance;
    unsigned int roib_sharpness;
    unsigned int roib_luminance;
    unsigned int global_sharpness[ISP_SHARPNESS_GRID_ITEMS];
} rawaf_isp_af_stat_t;

typedef struct {
    unsigned char sw_rawaf_en;

    unsigned char window_num;
    unsigned short wina_h_offs;
    unsigned short wina_v_offs;
    unsigned short wina_h_size;
    unsigned short wina_v_size;

    unsigned short winb_h_offs;
    unsigned short winb_v_offs;
    unsigned short winb_h_size;
    unsigned short winb_v_size;

    unsigned char gamma_flt_en;

    unsigned short gamma_y1;
    unsigned short gamma_y2;
    unsigned short gamma_y3;
    unsigned short gamma_y4;
    unsigned short gamma_y5;
    unsigned short gamma_y6;
    unsigned short gamma_y7;
    unsigned short gamma_y8;

    unsigned char gaus_flt_en;

    unsigned char gaus_h0;
    unsigned char gaus_h1;
    unsigned char gaus_h2;

    unsigned char int_lin0_en;
    unsigned char int_lin1_en;
    unsigned char int_lin2_en;
    unsigned char int_lin3_en;
    unsigned char int_lin4_en;

    unsigned char int_lin0;
    unsigned char int_lin1;
    unsigned char int_lin2;
    unsigned char int_lin3;
    unsigned char int_lin4;

    unsigned short roi_threshold;
    unsigned short global_threshold;

    unsigned char afm_var_shift_a;
    unsigned char afm_var_shift_b;
    unsigned char lum_var_shift_a;
    unsigned char lum_var_shift_b;
} rawaf_isp_af_meas_t;

typedef struct {
    bool vcm_config_valid;
    bool zoomfocus_modifypos;
    bool focus_correction;
    bool zoom_correction;
    bool lens_pos_valid;
    bool zoom_pos_valid;
    bool send_zoom_reback;
    bool send_focus_reback;
    bool end_zoom_chg;
    bool focus_noreback;
    int next_pos_num;
    int next_lens_pos[RKAIQ_RAWAF_NEXT_ZOOMFOCUS_NUM];
    int next_zoom_pos[RKAIQ_RAWAF_NEXT_ZOOMFOCUS_NUM];
    int use_manual;
    int auto_focpos;
    int auto_zoompos;
    int manual_focpos;
    int manual_zoompos;
    int vcm_start_ma;
    int vcm_end_ma;
} rawaf_focus_pos_meas_t;

#endif
