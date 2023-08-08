#ifndef __RK_AIQ_TYPES_AF_ALGO_H__
#define __RK_AIQ_TYPES_AF_ALGO_H__

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

#define RKAIQ_RAWAF_WIN_NUM                2
#define RKAIQ_RAWAF_LINE_NUM               5
#define RKAIQ_RAWAF_GAMMA_NUM              17
#define RKAIQ_RAWAF_SUMDATA_ROW            15
#define RKAIQ_RAWAF_SUMDATA_COLUMN         15
#define RKAIQ_RAWAF_SUMDATA_NUM            225
#define RKAIQ_RAWAF_NEXT_ZOOMFOCUS_NUM     300
#define RKAIQ_RAWAF_ROI_SUBWINS_VCNT       15
#define RKAIQ_RAWAF_ROI_SUBWINS_HCNT       15
#define RKAIQ_RAWAF_ROI_SUBWINS_NUM        (RKAIQ_RAWAF_ROI_SUBWINS_VCNT * RKAIQ_RAWAF_ROI_SUBWINS_HCNT)

typedef struct {
    unsigned long long roia_sharpness;
    unsigned int roia_luminance;
    unsigned int roib_sharpness;
    unsigned int roib_luminance;
    unsigned int global_sharpness[RKAIQ_RAWAF_SUMDATA_NUM];
    struct timeval focus_starttim;
    struct timeval focus_endtim;
    struct timeval zoom_starttim;
    struct timeval zoom_endtim;
    int64_t sof_tim;
    int32_t focusCode;
    int32_t zoomCode;
    int32_t lowpass_fv4_4[RKAIQ_RAWAF_SUMDATA_NUM];
    int32_t lowpass_fv8_8[RKAIQ_RAWAF_SUMDATA_NUM];
    int32_t lowpass_highlht[RKAIQ_RAWAF_SUMDATA_NUM];
    int32_t lowpass_highlht2[RKAIQ_RAWAF_SUMDATA_NUM];
    int lowpass_id;
    bool focusCorrection;
    bool zoomCorrection;
} rk_aiq_af_algo_stat_t;

typedef struct {
    bool enable;
    int ldg_xl;
    int ldg_yl;
    int ldg_kl;
    int ldg_xh;
    int ldg_yh;
    int ldg_kh;
    int highlight_th;
    int highlight2_th;
} rk_aiq_af_algo_sp_meas_t;

typedef struct {
    unsigned char contrast_af_en;
    unsigned char rawaf_sel;

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
    unsigned short gamma_y[RKAIQ_RAWAF_GAMMA_NUM];

    unsigned char gaus_flt_en;
    unsigned char gaus_h0;
    unsigned char gaus_h1;
    unsigned char gaus_h2;

    unsigned char line_en[RKAIQ_RAWAF_LINE_NUM];
    unsigned char line_num[RKAIQ_RAWAF_LINE_NUM];

    unsigned short afm_thres;

    unsigned char lum_var_shift[RKAIQ_RAWAF_WIN_NUM];
    unsigned char afm_var_shift[RKAIQ_RAWAF_WIN_NUM];

    rk_aiq_af_algo_sp_meas_t sp_meas;
} rk_aiq_af_algo_meas_t;

typedef struct {
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
} rk_aiq_af_algo_focus_pos_t;

int get_lpfv(uint32_t sequence, uint8_t *image_buf, int32_t _img_width, int32_t _img_height,
    int32_t _img_width_align, int32_t _img_height_align, uint8_t *pAfTmp, uint32_t sub_shp4_4[225],
    uint32_t sub_shp8_8[225], uint32_t high_light[225], uint32_t high_light2[225], rk_aiq_af_algo_meas_t *param);

RKAIQ_END_DECLARE

#endif
