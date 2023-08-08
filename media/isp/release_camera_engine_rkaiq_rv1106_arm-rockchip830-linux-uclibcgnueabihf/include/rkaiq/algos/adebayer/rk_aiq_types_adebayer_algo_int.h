
#ifndef __RKAIQ_TYPES_ADEBAYER_ALGO_INT_H__
#define __RKAIQ_TYPES_ADEBAYER_ALGO_INT_H__

#include "rk_aiq_types_adebayer_algo.h"
#include "debayer_head.h"

typedef enum rk_aiq_debayer_op_mode_e {
    RK_AIQ_DEBAYER_MODE_INVALID                     = 0,        /**< initialization value */
    RK_AIQ_DEBAYER_MODE_MANUAL                      = 1,        /**< run manual lens shading correction */
    RK_AIQ_DEBAYER_MODE_AUTO                        = 2,        /**< run auto lens shading correction */
    RK_AIQ_DEBAYER_MODE_MAX
} rk_aiq_debayer_op_mode_t;

typedef enum AdebayerHwVersion_e
{
    ADEBAYER_HARDWARE_V1 = 0,  // rk1126/1109/356X/3588
    ADEBAYER_HARDWARE_V2 = 1,  //rk1106
    ADEBAYER_HARDWARE_MAX,
} AdebayerHwVersion_t;

typedef struct AdebayerSeletedParamV1_s {
    int8_t      filter1[5];
    int8_t      filter2[5];
    uint8_t     gain_offset;
    uint8_t     sharp_strength;
    uint8_t     hf_offset;
    uint8_t     offset;
    uint8_t     clip_en;
    uint8_t     filter_g_en;
    uint8_t     filter_c_en;
    uint8_t     thed0;
    uint8_t     thed1;
    uint8_t     dist_scale;
    uint8_t     cnr_strength;
    uint8_t     shift_num;
} AdebayerSeletedParamV1_t;

typedef struct AdebayerSeletedParamV2_s {
    bool debayer_en;

    //filter coef
    int lowfreq_filter1[4];
    int highfreq_filter2[4];
    int c_alpha_gaus_coe[3];
    int c_guid_gaus_coe[3];
    int c_ce_gaus_coe[3];

    //g_interp
    unsigned char debayer_clip_en;
    unsigned short debayer_gain_offset;
    unsigned char debayer_max_ratio;

    //g_drctwgt
    unsigned short debayer_hf_offset;
    unsigned char debayer_thed0;
    unsigned char debayer_thed1;
    unsigned char debayer_dist_scale;
    unsigned char debayer_select_thed;

    //g_filter
    unsigned char debayer_gfilter_en;
    unsigned short debayer_gfilter_offset;

    //c_filter
    unsigned char debayer_cfilter_en;

    unsigned short debayer_loggd_offset;

    float debayer_cfilter_str;
    float debayer_wet_clip;
    float debayer_wet_ghost;
    float debayer_wgtslope;

    float debayer_bf_sgm;
    unsigned char  debayer_bf_clip;
    unsigned char  debayer_bf_curwgt;

    unsigned short debayer_loghf_offset;

    unsigned short debayer_alpha_offset;
    float debayer_alpha_scale;
    unsigned short debayer_edge_offset;
    float debayer_edge_scale;

} AdebayerSeletedParamV2_t;

typedef struct AdebayerProcResultV1_s {
    AdebayerHwConfigV1_t config;
} AdebayerProcResultV1_t;

typedef struct AdebayerProcResultV2_s {
    AdebayerHwConfigV2_t config;
} AdebayerProcResultV2_t;

#endif//__RKAIQ_TYPES_ADEBAYER_ALGO_INT_H__
