#ifndef __RKAIQ_PARSE_IQ_H__
#define __RKAIQ_PARSE_IQ_H__

#include "RkAiqCalibDbTypesV2.h"
#include "rkisp32-config.h"
#include "fast_ae.h"
#include "rkaiq_calib.h"
#include "../drivers/isp3/drv_isp3.h"

/******************************************************************************
 * TO DEFINE SELECTED CALIB PARAMS HERE
 *****************************************************************************/

#define FIXVSTINV       10      // VST fix
#define FIXNLMCALC      10      // NLM calc fix
#define EXP2RECISION_FIX (1<<(FIXNLMCALC+5))
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#define CLIP(a, min_v, max_v)               (((a) < (min_v)) ? (min_v) : (((a) > (max_v)) ? (max_v) : (a)))
#define RK_BAYERNR_V23_MAX_ISO_NUM    13


typedef struct RK_Bayer2dnr_Params_V23_Select_s {
    // v23 version register // edge filter params
    bool        enable;
    bool        hdrdgain_ctrl_en;
    float       filter_strength;
    uint16_t    lumapoint[16];
    uint16_t    sigma[16];
    uint8_t     gain_bypass;
    float       gain_scale;
    uint8_t     gain_lumapoint[16];
    uint16_t    gain_adj[16];
    float       edgesofts;
    float       ratio;
    float       weight;
    uint8_t     gauss_guide;
    uint16_t    pix_diff;
    uint16_t    diff_thld;
    uint8_t     trans_en;
    uint8_t     trans_mode;
    uint16_t    trans_offset;
    uint16_t    itrans_offset;
    int   trans_datmax;
    float hdr_dgain_scale_s;
    float hdr_dgain_scale_m;
} RK_Bayer2dnrV23_Params_Select_t;

typedef struct RK_Bayer2dnr_Params_V23_s
{
    // bayernr version
    bool enable;
    bool hdrdgain_ctrl_en;
    // v23 parse
    float iso[RK_BAYERNR_V23_MAX_ISO_NUM];
    RK_Bayer2dnrV23_Params_Select_t Bayer2dnrParamsISO[RK_BAYERNR_V23_MAX_ISO_NUM];

} RK_Bayer2dnr_Params_V23_t;

typedef struct RK_Bayertnr_Params_V23_Select_s
{
    bool enable;
    //calib
    uint16_t    lumapoint[16];
    uint16_t    sigma[16];
    uint16_t    lumapoint2[16];
    uint16_t    lo_sigma[16];
    uint16_t    hi_sigma[16];
    //tuning
    uint8_t     thumbds_w;
    uint8_t     thumbds_h;
    bool        lo_enable;
    bool        hi_enable;
    bool        lo_med_en;
    bool        lo_gsbay_en;
    bool        lo_gslum_en;
    bool        hi_med_en;
    bool        hi_gslum_en;
    bool        trans_en;
    bool        wgt_use_mode;
    bool        wgt_mge_mode;
    bool        hi_guass;
    bool        kl_guass;
    bool        global_pk_en;
    int         global_pksq;
    float       lo_filter_strength;
    float       hi_filter_strength;
    float       soft_threshold_ratio;
    float       lo_clipwgt;
    float       hi_wgt_comp;
    uint16_t    hidif_th;
    float       lo_filter_rat0;
    uint16_t    lo_filter_thed0;
    uint8_t     hi_filter_abs_ctrl;
    uint8_t     hi_filter_filt_bay;
    uint8_t     hi_filter_filt_avg;
    uint8_t     hi_filter_filt_mode;
    float       hi_filter_rat0;
    uint16_t    hi_filter_thed0;
    float       hi_filter_rat1;
    uint16_t    hi_filter_thed1;
    uint8_t     guass_guide_coeff0;
    uint8_t     guass_guide_coeff1;
    uint8_t     guass_guide_coeff2;
    uint8_t     guass_guide_coeff3;

} RK_Bayertnr_Params_V23_Select_t;

typedef struct RK_Bayertnr_Params_V23_s
{
    bool enable;
    float iso[RK_BAYERNR_V23_MAX_ISO_NUM];
    RK_Bayertnr_Params_V23_Select_t bayertnrParamISO[RK_BAYERNR_V23_MAX_ISO_NUM];
} RK_Bayertnr_Params_V23_t;

typedef struct AblcParams_V32_s {
    bool enable;
    float iso[ABLCV32_MAX_ISO_LEVEL];
    int blc_r[ABLCV32_MAX_ISO_LEVEL];
    int blc_gr[ABLCV32_MAX_ISO_LEVEL];
    int blc_gb[ABLCV32_MAX_ISO_LEVEL];
    int blc_b[ABLCV32_MAX_ISO_LEVEL];
} AblcParams_V32_t;

typedef struct AblcOBParams_V32_s {
    bool enable;
    float iso[ABLCV32_MAX_ISO_LEVEL];
    int ob_offset[ABLCV32_MAX_ISO_LEVEL];
    float ob_predgain[ABLCV32_MAX_ISO_LEVEL];
} AblcOBParams_V32_t;

typedef struct AblcSelect_V32_s {
    bool enable;
    int blc_r;
    int blc_gr;
    int blc_gb;
    int blc_b;
} AblcSelect_V32_t;
typedef struct AblcOBSelect_V32_s {
    bool enable;
    int ob_offset;
    float ob_predgain;
} AblcOBSelect_V32_t;

typedef struct RK_Blc_Params_V32_s
{
    AblcParams_V32_t stBlc0Params;
    AblcParams_V32_t stBlc1Params;
    AblcOBParams_V32_t stBlcOBParams;

    AblcSelect_V32_t stBlc0Select;
    AblcSelect_V32_t stBlc1Select;
    AblcOBSelect_V32_t stBlcOBSelect;

} RK_Blc_Params_V32_t;

typedef struct NR_BLC_Context_s {
    bool bayernr_params_init_flag;
    bool bayertnr_params_init_flag;
    bool blc_params_init_flag;
    RK_Bayer2dnr_Params_V23_t bayernr_params;
    RK_Bayertnr_Params_V23_t bayertnr_params;
    RK_Blc_Params_V32_t blc_params;

} NR_BLC_Context_t;

void set_isp_params_for_kernel(struct shared_data *gShare, CamCalibDbV2Context_t* g_main_calib, int is_ie_effect, uint8_t iq_bin_mode);
void parse_ae_params(CamCalibDbV2Context_t *parse_iq_param, struct fastae_init_info *parse_ae_init, uint8_t iq_bin_mode);
void set_baynr_params_for_rtt(struct isp_baynr_cfg* pbaynr);
void set_bay3d_params_for_rtt(struct isp_bay3d_cfg* pbay3d, uint32_t w, uint32_t h);
void get_tnr_sigma(CalibDbV2_BayerTnrV23_Calib_t* pCalibdb, int calib_idx, int iso, int meanluma, struct fastae_md_param* md_param);

#endif /*__RKAIQ_PARSE_IQ_H__*/
