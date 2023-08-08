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
#define RKAIQ_PDAF_BLK_DATA_HEIGHT_MAX     64
#define RKAIQ_PDAF_FINE_SEARCH_RANGE_MAX   10
#define RKAIQ_PDAF_ISOPARA_NUM             16
#define RKAIQ_PDAF_STEPRATIO_NUM           7

typedef enum {
    PDAF_DATA_INVAL = 0,
    PDAF_DATA_HSPD  = 1,
    PDAF_DATA_MLPD  = 2,
    PDAF_DATA_DPD   = 3,
    PDAF_DATA_QPD   = 4
} PdafDataType_t;

typedef enum {
    PDAF_DIR_INVAL = 0,
    PDAF_DIR_LEFT  = 1,
    PDAF_DIR_RIGHT = 2,
    PDAF_DIR_TOP   = 3,
    PDAF_DIR_DOWN  = 4
} PdafDataDir_t;

typedef enum {
    PDAF_TRANS_LR_BYTURNS = 0,
    PDAF_TRANS_IN_PKT = 1,
} PdafTransType_t;

typedef struct {
    unsigned long long roia_sharpness;
    unsigned int roia_luminance;
    unsigned int roib_sharpness;
    unsigned int roib_luminance;
    unsigned int global_sharpness[RKAIQ_RAWAF_SUMDATA_NUM];
    int lowpass_fv4_4[RKAIQ_RAWAF_SUMDATA_NUM];
    int lowpass_fv8_8[RKAIQ_RAWAF_SUMDATA_NUM];
    int lowpass_highlht[RKAIQ_RAWAF_SUMDATA_NUM];
    int lowpass_highlht2[RKAIQ_RAWAF_SUMDATA_NUM];
    int lowpass_id;

    struct timeval focus_starttim;
    struct timeval focus_endtim;
    struct timeval zoom_starttim;
    struct timeval zoom_endtim;
    int64_t sof_tim;
    int focusCode;
    int zoomCode;
    bool focusCorrection;
    bool zoomCorrection;
    float angleZ;
} rk_aiq_af_algo_stat_v20_t;

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
} rk_aiq_af_algo_meas_v20_t;

typedef struct {
    unsigned int wndb_luma;
    unsigned int wndb_sharpness;
    unsigned int winb_highlit_cnt;
    unsigned int wnda_luma[RKAIQ_RAWAF_SUMDATA_NUM];
    unsigned int wnda_fv_v1[RKAIQ_RAWAF_SUMDATA_NUM];
    unsigned int wnda_fv_v2[RKAIQ_RAWAF_SUMDATA_NUM];
    unsigned int wnda_fv_h1[RKAIQ_RAWAF_SUMDATA_NUM];
    unsigned int wnda_fv_h2[RKAIQ_RAWAF_SUMDATA_NUM];
    unsigned int wina_highlit_cnt[RKAIQ_RAWAF_SUMDATA_NUM];
    int comp_bls;

    struct timeval focus_starttim;
    struct timeval focus_endtim;
    struct timeval zoom_starttim;
    struct timeval zoom_endtim;
    int64_t sof_tim;
    int focusCode;
    int zoomCode;
    bool focusCorrection;
    bool zoomCorrection;
    float angleZ;
} rk_aiq_af_algo_stat_v30_t;

typedef struct {
    unsigned char af_en;
    unsigned char rawaf_sel;
    unsigned char gamma_en;
    unsigned char gaus_en;
    unsigned char v1_fir_sel;
    unsigned char hiir_en;
    unsigned char viir_en;
    unsigned char v1_fv_outmode;    // 0 square, 1 absolute
    unsigned char v2_fv_outmode;    // 0 square, 1 absolute
    unsigned char h1_fv_outmode;    // 0 square, 1 absolute
    unsigned char h2_fv_outmode;    // 0 square, 1 absolute
    unsigned char ldg_en;
    unsigned char accu_8bit_mode;
    unsigned char ae_mode;
    unsigned char y_mode;
    unsigned char line_en[RKAIQ_RAWAF_LINE_NUM];
    unsigned char line_num[RKAIQ_RAWAF_LINE_NUM];

    unsigned char window_num;
    unsigned short wina_h_offs;
    unsigned short wina_v_offs;
    unsigned short wina_h_size;
    unsigned short wina_v_size;
    unsigned short winb_h_offs;
    unsigned short winb_v_offs;
    unsigned short winb_h_size;
    unsigned short winb_v_size;

    unsigned short gamma_y[RKAIQ_RAWAF_GAMMA_NUM];

    // [old version param]
    unsigned short thres;
    unsigned char shift_sum_a;
    unsigned char shift_sum_b;
    unsigned char shift_y_a;
    unsigned char shift_y_b;

    /**********[Vertical IIR (v1 & v2)]************/
    short v1_iir_coe[9];
    short v1_fir_coe[3];
    short v2_iir_coe[3];
    short v2_fir_coe[3];

    /**********[Horizontal IIR (h1 & h2)]************/
    short h1_iir1_coe[6];
    short h2_iir1_coe[6];
    short h1_iir2_coe[6];
    short h2_iir2_coe[6];

    /**********[Focus value statistic param]**********/
    // level depended gain
    // input8 lumi, output8bit gain
    unsigned char h_ldg_lumth[2];    //luminance thresh
    unsigned char h_ldg_gain[2];     //gain for [minLum,maxLum]
    unsigned short h_ldg_gslp[2];    //[slope_low,-slope_high]
    unsigned char v_ldg_lumth[2];
    unsigned char v_ldg_gain[2];
    unsigned short v_ldg_gslp[2];

    // coring
    unsigned short v_fv_thresh;
    unsigned short h_fv_thresh;

    // left shift, more needed if outmode=square
    unsigned char v1_fv_shift; //only for sel1
    unsigned char v2_fv_shift;
    unsigned char h1_fv_shift;
    unsigned char h2_fv_shift;

    /**********[High light]**********/
    unsigned short highlit_thresh;
}  rk_aiq_af_algo_meas_v30_t;

typedef struct {
    unsigned char af_en;
    unsigned char rawaf_sel;
    unsigned char gamma_en;
    unsigned char gaus_en;
    unsigned char v1_fir_sel;
    unsigned char hiir_en;
    unsigned char viir_en;
    unsigned char v1_fv_outmode;    // 0 square, 1 absolute
    unsigned char v2_fv_outmode;    // 0 square, 1 absolute
    unsigned char h1_fv_outmode;    // 0 square, 1 absolute
    unsigned char h2_fv_outmode;    // 0 square, 1 absolute
    unsigned char ldg_en;
    unsigned char accu_8bit_mode;
    unsigned char ae_mode;
    unsigned char y_mode;
    unsigned char vldg_sel;
    unsigned char sobel_sel;
    unsigned char v_dnscl_mode;
    unsigned char from_awb;
    unsigned char from_ynr;
    unsigned char ae_config_use;
    unsigned char line_en[RKAIQ_RAWAF_LINE_NUM];
    unsigned char line_num[RKAIQ_RAWAF_LINE_NUM];

    unsigned char window_num;
    unsigned short wina_h_offs;
    unsigned short wina_v_offs;
    unsigned short wina_h_size;
    unsigned short wina_v_size;
    unsigned short winb_h_offs;
    unsigned short winb_v_offs;
    unsigned short winb_h_size;
    unsigned short winb_v_size;

    unsigned short gamma_y[RKAIQ_RAWAF_GAMMA_NUM];

    // [old version param]
    unsigned short thres;
    unsigned char shift_sum_a;
    unsigned char shift_sum_b;
    unsigned char shift_y_a;
    unsigned char shift_y_b;

    char gaus_coe[9];

    /**********[Vertical IIR (v1 & v2)]************/
    short v1_iir_coe[3];
    short v1_fir_coe[3];
    short v2_iir_coe[3];
    short v2_fir_coe[3];

    /**********[Horizontal IIR (h1 & h2)]************/
    short h1_iir1_coe[6];
    short h2_iir1_coe[6];
    short h1_iir2_coe[6];
    short h2_iir2_coe[6];

    /**********[Focus value statistic param]**********/
    // level depended gain
    // input8 lumi, output8bit gain
    unsigned char h_ldg_lumth[2];    //luminance thresh
    unsigned char h_ldg_gain[2];     //gain for [minLum,maxLum]
    unsigned short h_ldg_gslp[2];    //[slope_low,-slope_high]
    unsigned char v_ldg_lumth[2];
    unsigned char v_ldg_gain[2];
    unsigned short v_ldg_gslp[2];

    // coring
    unsigned short v_fv_thresh;
    unsigned short h_fv_thresh;

    // left shift, more needed if outmode=square
    unsigned char v1_fv_shift; //only for sel1
    unsigned char v2_fv_shift;
    unsigned char h1_fv_shift;
    unsigned char h2_fv_shift;

    /**********[High light]**********/
    unsigned short highlit_thresh;
}  rk_aiq_af_algo_meas_v31_t;

typedef rk_aiq_af_algo_meas_v20_t rk_aiq_af_algo_meas_t;
typedef rk_aiq_af_algo_stat_v20_t rk_aiq_af_algo_stat_t;

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
} rk_aiq_af_algo_focus_pos_t;

typedef struct {
    bool pdMirror;
    unsigned int pdMean;
    unsigned short pdWidth;
    unsigned short pdHeight;
    unsigned short *pdLData;
    unsigned short *pdRData;
} rk_aiq_pdaf_algo_stat_t;

typedef struct {
    unsigned char pdMirrorInCalib;
    unsigned char pdLRInDiffLine;
    unsigned short pdWidth;
    unsigned short pdHeight;
    struct rkmodule_pdaf_inf *otp_pdaf;
} rk_aiq_pdaf_algo_meas_t;

typedef struct {
    int iso;
    float pdConfdRatio0;
    float pdConfdRatio1;
    float pdConfdRatio2;
    float pdConfdRatio3;
    float pdConfdThresh;
    float stablePdRatio;
    float stableCntRatio;
    unsigned short convergedInfPdThresh;
    unsigned short convergedMacPdThresh;
    unsigned short defocusInfPdThresh;
    unsigned short defocusMacPdThresh;
    unsigned short stablePdOffset;
    unsigned short noconfCntThresh;
    unsigned char blkCntW;
    unsigned char blkCntH;

    unsigned int fineSearchConfidence[RKAIQ_PDAF_FINE_SEARCH_RANGE_MAX];
    int fineSearchRange[RKAIQ_PDAF_FINE_SEARCH_RANGE_MAX];
    int fineSearchStepPos[RKAIQ_PDAF_FINE_SEARCH_RANGE_MAX];
    int fineSearchTblCnt;
} rk_aiq_pdaf_algo_isopara_t;

typedef struct {
    unsigned char pdVsImgoutMirror;
    unsigned char pdVsCdDebug;
    unsigned char pdDumpDebug;
    unsigned char pdVerBinning;
    unsigned char pdFrmInValid;
    unsigned char pdDgainValid;
    unsigned char pdGainMapNormEn;
    unsigned char pdConfdMode;
    unsigned char pdDiscardRegionEn;
    unsigned short pdDumpMaxFrm;
    unsigned short pdDataBit;
    unsigned short pdBlkLevel;
    unsigned short pdSearchRadius;
    unsigned short pdConfdMwinFactor;
    unsigned short pdGainMapW;
    unsigned short pdGainMapH;
    unsigned short pdDccMapW;
    unsigned short pdDccMapH;
    float pdStepRatio[RKAIQ_PDAF_STEPRATIO_NUM];
    unsigned short pdStepDefocus[RKAIQ_PDAF_STEPRATIO_NUM];
    rk_aiq_pdaf_algo_isopara_t pdIsoPara[RKAIQ_PDAF_ISOPARA_NUM];
    int pdIsoParaCnt;
    unsigned int pdCenterMinFv;
    float pdCenterMinRatio;
    float pdHighlightRatio;
    float pdNoiseSn;
    float pdNoiseRn[2];
    float pdNoisePn;
    float pdSatValRatio;
    float pdSatCntRatio;
    float pdLessTextureRatio;
} rk_aiq_pdaf_algo_config_t;

int get_lpfv(uint32_t sequence, uint8_t *image_buf, int32_t _img_width, int32_t _img_height,
    int32_t _img_width_align, int32_t _img_height_align, uint8_t *pAfTmp, uint32_t sub_shp4_4[225],
    uint32_t sub_shp8_8[225], uint32_t high_light[225], uint32_t high_light2[225], rk_aiq_af_algo_meas_v20_t *param);

RKAIQ_END_DECLARE

#endif
