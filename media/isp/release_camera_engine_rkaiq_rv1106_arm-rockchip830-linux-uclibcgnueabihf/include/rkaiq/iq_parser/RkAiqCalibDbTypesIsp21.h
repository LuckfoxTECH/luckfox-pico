#ifndef _RK_AIQ_CALIB_TYPES_ISP21_H_
#define _RK_AIQ_CALIB_TYPES_ISP21_H_

#include "RkAiqCalibDbTypesIsp20.h"

#pragma pack(4)

#define ISP21_DRC_Y_NUM             17
#define RK_BAYERNR_V2_MAX_ISO_NUM (CALIBDB_MAX_ISO_LEVEL)
#define RK_YNR_V2_MAX_ISO_NUM (CALIBDB_MAX_ISO_LEVEL)
#define RK_CNR_V1_MAX_ISO_NUM (CALIBDB_MAX_ISO_LEVEL)

#define RK_SHARP_V3_MAX_ISO_NUM (CALIBDB_MAX_ISO_LEVEL)
#define RK_SHARP_V3_LUMA_POINT_NUM (8)

#define CALD_AEC_GAIN_RANGE_MAX_LEN  350

typedef struct {
    int time_update;
    int gain_update;
    int dcg_update;
} CalibDb_ExpUpdate_t;

typedef struct CalibDb_CISTimeSet_s {
    char                    name[20];
    Cam2x1FloatMatrix_t     CISHdrTimeRegSumFac;
    Cam2x1FloatMatrix_t     CISLinTimeRegMaxFac;
    uint16_t                CISTimeRegMin;
    Cam1x3ShortMatrix_t     CISTimeRegMax; //specially for Hdr that has limit on sframe/mframe, requiring max time line.value 0: no limit
    Cam2x1FloatMatrix_t     CISTimeRegOdevity;
    uint8_t                 CISTimeRegUnEqualEn;
} CalibDb_CISTimeSet_t;

typedef struct {
    unsigned char hdr_en;
    rk_aiq_isp_hdr_mode_t hdr_mode;
    rk_aiq_sensor_hdr_line_mode_t line_mode;
} CalibDb_CISHdrSet_t;

typedef struct CalibDb_CISTimeSet_comb_s {
    CalibDb_CISTimeSet_t Normal;
    CalibDb_CISTimeSet_t Hdr[2];
} CalibDb_CISTimeSet_comb_t;

typedef struct CalibDb_CISGainSet_s {
    //CISGainSetting
    CalibDb_AeRange_t       CISAgainRange; //sensor Again or LCG range
    CalibDb_AeRange_t       CISExtraAgainRange; //add for HDR-DCG MODE, HCG range
    CalibDb_AeRange_t       CISDgainRange; //sensor Dgain
    CalibDb_AeRange_t       CISIspDgainRange; //Isp Dgain
    uint8_t                 CISHdrGainIndSetEn; //bit 0:use the same value; bit 1: support use different gain value
} CalibDb_CISGainSet_t;

typedef struct {
    CalibDb_ExpUpdate_t Hdr;
    CalibDb_ExpUpdate_t Normal;
} CalibDb_ExpUpdate_comb_t;

typedef enum  CalibDb_HdrFrmNum_e {
    EXPSET_HDR_TWO_FRAME = 0,
    EXPSET_HDR_THREE_FRAME = 1,
} CalibDb_HdrFrmNum_t;

typedef struct CalibDb_ExpSet_para_s {
    CalibDb_AecGainRange_t    Gain2Reg;
    float                     Time2Reg[4];
    CalibDb_CISGainSet_t      CISGainSet;
    CalibDb_CISTimeSet_comb_t CISTimeSet;
    CalibDb_CISHdrSet_t       CISHdrSet;
    CalibDb_Dcg_t             CISDcgSet;
    CalibDb_ExpUpdate_comb_t  CISExpUpdate;
    float                     CISMinFps;
    uint8_t                   CISFlip;
} CalibDb_ExpSet_para_t;

typedef struct CalibDb_Awb_Calib_Para_V201_s {
    struct list_head           listHead;
    char                scene[CALIBDB_MAX_SCENE_NAME_LENGTH];
    bool                lscBypEnable;
    bool                uvDetectionEnable;
    bool                xyDetectionEnable;
    bool                yuvDetectionEnable;
    int                 lsUsedForYuvDetNum;
    char                lsUsedForYuvDet[CALD_AWB_LS_NUM_MAX][CALD_AWB_ILLUMINATION_NAME];
    bool                wbGainClipEn;
    bool                wbGainDaylightClipEn;
    bool                wpDiffWeiEnable;
    bool                blkWeightEnable;
    bool                blkStatisticsEnable;
    unsigned char       dsMode;
    unsigned char       blkMeasureMode;
    bool                multiwindow_en;
    unsigned char       hdrFrameChooseMode;
    unsigned char       hdrFrameChoose;
    CalibDb_StatWindow_t measeureWindow;
    unsigned char       lightNum;
    char                lightName[CALD_AWB_LS_NUM_MAX][CALD_AWB_ILLUMINATION_NAME];
    unsigned char       doorType[CALD_AWB_LS_NUM_MAX];
    float               standardGainValue[CALD_AWB_LS_NUM_MAX][4];//rggb
    CalibDb_limit_range_t limitRange;
    CalibDb_Rgb2TcsParam_t     rgb2tcs_param;
    float               rgb2RYuv_matrix[16];
    CalibDb_UV_Range_Ill_t uvRange_param[CALD_AWB_LS_NUM_MAX];
    CalibDb_UV_Range_Ill_t uvRange_param_small[CALD_AWB_LS_NUM_MAX];
    CalibDb_tcs_range_ill_t xyRangeLight[CALD_AWB_LS_NUM_MAX];
    CalibDb_Yuv3D_2_Range_Ill_t yuv3D2Range_param[CALD_AWB_LS_NUM_MAX];

    unsigned short multiwindow[CALD_AWB_WINDOW_NUM_MAX][4];//8  windows in pixel domain ,hOffset,vOffser,hSize,vSize;
    //several winow in uv or xy domain
    CalibDb_ExcRange_t excludeWpRange[CALD_AWB_EXCRANGE_NUM_MAX];
    //with differernt luma ,the different weight in WP sum
    float wpDiffNoTh;
    unsigned int   wpDiffLvValueTh;
    unsigned char wpDiffwei_y[9];
    unsigned char  perfectBin[8];// ture means the luma is appropriate
    float wpDiffweiSet_w_LvValueTh[2];
    float wpDiffWeiRatioTh[3];
    float wpDiffweiSet_w_HigLV[3][9];
    float wpDiffweiSet_w_LowLV[3][9];
    unsigned short blkWeight[CALD_AWB_GRID_NUM_TOTAL];
    float spatialGain_H[4];//spatial gain
    float spatialGain_L[4];
    float temporalDefaultGain[4];
    float ca_targetGain[4];
    //single color
    unsigned short      sSelColorNUM;
    unsigned short      sIndSelColor[CALD_AWB_SGC_NUM_MAX];
    float               sMeanCh[2][CALD_AWB_SGC_NUM_MAX];
    float               srGain[CALD_AWB_LS_NUM_MAX];
    float               sbGain[CALD_AWB_LS_NUM_MAX];
    unsigned short      sIllEstNum;
    char                sNameIllEst[CALD_AWB_LS_NUM_MAX][CALD_AWB_ILLUMINATION_NAME];
    float               sAlpha;
    float lineRgBg[3];
    float lineRgProjCCT[3];
    //wbgain clip
    CalibDb_Awb_Cct_Clip_Cfg_t cct_clip_cfg;
} CalibDb_Awb_Calib_Para_V201_t;

typedef struct CalibDb_Awb_Para_Isp21_s
{
    CalibDb_Awb_Calib_Para_V201_t calib_para_v201;
    CalibDb_Awb_Adjust_Para_t adjust_para;
} CalibDb_Awb_Para_Isp21_t;

typedef struct CalibDb_Drc_ModeCell_s {
    char scene[CALIBDB_MAX_MODE_NAME_LENGTH];
    int sw_drc_offset_pow2;//
    int sw_drc_position;
    int sw_drc_hpdetail_ratio;
    int sw_drc_lpdetail_ratio;
    int sw_drc_weicur_pix;//
    int sw_drc_weipre_frame;//
    int sw_drc_force_sgm_inv0;//
    int sw_drc_motion_scl;//
    int sw_drc_edge_scl;//
    int sw_drc_space_sgm_inv1;//
    int sw_drc_space_sgm_inv0;//
    int sw_drc_range_sgm_inv1;//
    int sw_drc_range_sgm_inv0;//
    int sw_drc_weig_maxl;//
    int sw_drc_weig_bilat;//
    int sw_drc_scale_y[ISP21_DRC_Y_NUM];//
    int sw_drc_iir_frame;
    int sw_drc_gain;
    int sw_drc_min_ogain;//
} CalibDb_Drc_ModeCell_t;

typedef struct CalibDb_Adrc_Para_s {
    CalibDb_Drc_ModeCell_t calib[CALIBDB_MAX_MODE_NUM];
    CalibDb_Drc_ModeCell_t tuning[CALIBDB_MAX_MODE_NUM];
} CalibDb_Adrc_Para_t;


typedef struct CalibDb_Gic_setting_v21_s {
    float iso[CALIBDB_ISO_NUM];
    float min_busy_thre[CALIBDB_ISO_NUM];
    float min_grad_thr1[CALIBDB_ISO_NUM];
    float min_grad_thr2[CALIBDB_ISO_NUM] ;
    float k_grad1[CALIBDB_ISO_NUM];
    float k_grad2[CALIBDB_ISO_NUM] ;
    float gb_thre[CALIBDB_ISO_NUM] ;
    float maxCorV[CALIBDB_ISO_NUM] ;
    float maxCorVboth[CALIBDB_ISO_NUM];
    float dark_thre[CALIBDB_ISO_NUM];
    float dark_threHi[CALIBDB_ISO_NUM];
    float k_grad1_dark[CALIBDB_ISO_NUM];
    float k_grad2_dark[CALIBDB_ISO_NUM] ;
    float min_grad_thr_dark1[CALIBDB_ISO_NUM];
    float min_grad_thr_dark2[CALIBDB_ISO_NUM] ;
    float noiseCurve_0[CALIBDB_ISO_NUM];
    float noiseCurve_1[CALIBDB_ISO_NUM];
    float NoiseScale[CALIBDB_ISO_NUM] ;
    float NoiseBase[CALIBDB_ISO_NUM] ;
    float globalStrength[CALIBDB_ISO_NUM];
    float diff_clip[CALIBDB_ISO_NUM];
} CalibDb_Gic_setting_v21_t;

typedef struct CalibDb_Gic_ModeCell_v21_s {
    char scene[CALIBDB_MAX_MODE_NAME_LENGTH];
    unsigned char gic_en;
    unsigned char gr_ration;
    CalibDb_Gic_setting_v21_t setting;
} CalibDb_Gic_ModeCell_v21_t;

typedef struct CalibDb_Gic_ISP21_s {
    CalibDb_Gic_ModeCell_v21_t calib_v21[CALIBDB_MAX_MODE_NUM];
    CalibDb_Gic_ModeCell_v21_t tuning_v21[CALIBDB_MAX_MODE_NUM];
} CalibDb_Gic_Isp21_t;

typedef struct CalibDb_IIR_setting_v21_s {
    float stab_fnum;
    float sigma;
    float wt_sigma;
    float air_sigma;
    float tmax_sigma;
    float pre_wet;
} CalibDb_IIR_setting_v21_t;

typedef struct CalibDb_Dehaze_Setting_v21_s {
    float en;
    float iso[9];
    float air_lc_en[9];
    float dc_min_th[9];
    float dc_max_th[9];
    float yhist_th[9];
    float yblk_th[9];
    float dark_th[9];
    float bright_min[9];
    float bright_max[9];
    float wt_max[9];
    float air_min[9];
    float air_max[9];
    float tmax_base[9];
    float tmax_off[9];
    float tmax_max[9];
    float cfg_wt[9];
    float cfg_air[9];
    float cfg_tmax[9];
    float dc_weitcur[9];
    float bf_weight[9];
    float range_sigma[9];
    float space_sigma_pre[9];
    float space_sigma_cur[9];
    CalibDb_IIR_setting_v21_t IIR_setting;
} CalibDb_Dehaze_Setting_v21_t;

typedef struct CalibDb_Enhance_Setting_v21_s {
    float en;
    float iso[9];
    float enhance_value[9];
    float enhance_chroma[9];
    float enhance_curve[17];
} CalibDb_Enhance_Setting_v21_t;

typedef struct CalibDb_Hist_setting_v21_s {
    float en;
    float iso[9];
    unsigned char  hist_para_en[9];
    float hist_gratio[9];
    float hist_th_off[9];
    float hist_k[9];
    float hist_min[9];
    float hist_scale[9];
    float cfg_gratio[9];
} CalibDb_Hist_setting_v21_t;

typedef struct CalibDb_Dehaze_ModeCell_v21_s {
    char scene[CALIBDB_MAX_MODE_NAME_LENGTH];
    float en;
    float cfg_alpha;
    CalibDb_Dehaze_Setting_v21_t dehaze_setting;
    CalibDb_Enhance_Setting_v21_t enhance_setting;
    CalibDb_Hist_setting_v21_t hist_setting;

} CalibDb_Dehaze_ModeCell_v21_t;

typedef struct CalibDb_Dehaze_Isp21_s {
    CalibDb_Dehaze_ModeCell_v21_t calib_v21[CALIBDB_MAX_MODE_NUM];
    CalibDb_Dehaze_ModeCell_v21_t tuning_v21[CALIBDB_MAX_MODE_NUM];
} CalibDb_Dehaze_Isp21_t;

//bayernrV2

////////////////////calibdb type ////////////////////////////////////////////////

typedef struct Calibdb_Bayernr_2Dparams_V2_s {
    struct list_head listItem;

    char snr_mode[CALIBDB_NR_SHARP_NAME_LENGTH];
    char sensor_mode[CALIBDB_NR_SHARP_MODE_LENGTH];

    float iso[RK_BAYERNR_V2_MAX_ISO_NUM];

    float bayernrv2_filter_strength_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    int   bayernrv2_filter_lumapoint_r[16];
    int   bayernrv2_filter_sigma_r[RK_BAYERNR_V2_MAX_ISO_NUM][16];
    float bayernrv2_filter_edgesofts_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_filter_soft_threshold_ratio_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_filter_out_wgt_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    int   bayernrv2_gauss_guide_r[RK_BAYERNR_V2_MAX_ISO_NUM];

    //not use in xml param
    float bayernrv2_edge_filter_lumapoint_r[8];
    float bayernrv2_edge_filter_wgt_r[RK_BAYERNR_V2_MAX_ISO_NUM][8];

} Calibdb_Bayernr_2Dparams_V2_t;


typedef struct CalibDb_Bayernr_2DSetting_V2_s {
    // bayernr version
    int bayernrv2_2dnr_enable;

    // bayernr settting for hcg & lcg
    struct list_head listHead;

} CalibDb_Bayernr_2DSetting_V2_t;

typedef struct CalibDb_Bayernr_3DParams_V2_s {
    struct list_head listItem;

    char snr_mode[CALIBDB_NR_SHARP_NAME_LENGTH];
    char sensor_mode[CALIBDB_NR_SHARP_MODE_LENGTH];

    float iso[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_filter_strength_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_sp_filter_strength_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_lo_clipwgt_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_hi_clipwgt_r[RK_BAYERNR_V2_MAX_ISO_NUM];
    float bayernrv2_tnr_softwgt_r[RK_BAYERNR_V2_MAX_ISO_NUM];

    int   bayernrv2_lumapoint_r[16];
    int   bayernrv2_sigma_r[RK_BAYERNR_V2_MAX_ISO_NUM][16];
} CalibDb_Bayernr_3DParams_V2_t;

typedef struct CalibDb_Bayernr_3DSetting_V2_s {
    // bayernr version
    int bayernrv2_tnr_enable;

    // bayernr settting for hcg & lcg
    struct list_head listHead;
} CalibDb_Bayernr_3DSetting_V2_t;


typedef struct CalibDb_Bayernr_V2_t {
    struct list_head listItem;

    char modeName[64];

    CalibDb_Bayernr_2DSetting_V2_t st2DParams;
    CalibDb_Bayernr_3DSetting_V2_t st3DParams;

} CalibDb_Bayernr_V2_t;


typedef struct Calibdb_Ynr_params_V2_s {
    struct list_head listItem;
    char snr_mode[64];
    char sensor_mode[64];

    float iso[RK_YNR_V2_MAX_ISO_NUM];
    float ciISO_V2[2][RK_YNR_V2_MAX_ISO_NUM];
    float sigmaCurve[RK_YNR_V2_MAX_ISO_NUM][5];

    // low frequency
    float ynr_rnr_strength_V2[RK_YNR_V2_MAX_ISO_NUM][17];
    int ynr_bft3x3_bypass_V2[RK_YNR_V2_MAX_ISO_NUM];
    int ynr_lbft5x5_bypass_V2[RK_YNR_V2_MAX_ISO_NUM];
    int ynr_lgft3x3_bypass_V2[RK_YNR_V2_MAX_ISO_NUM];
    int ynr_flt1x1_bypass_V2[RK_YNR_V2_MAX_ISO_NUM];
    int ynr_sft5x5_bypass_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_bf_V2[2][RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_thred_adj_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_peak_supress_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_edge_adj_thresh_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_center_weight_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_dist_adj_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_weight_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_filt_strength_V2[2][RK_YNR_V2_MAX_ISO_NUM];
    float ynr_low_bi_weight_V2[RK_YNR_V2_MAX_ISO_NUM];

    // high frequency
    float ynr_base_filter_weight_V2[3][RK_YNR_V2_MAX_ISO_NUM];
    float ynr_high_thred_adj_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_high_weight_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_direction_weight_V2[RK_YNR_V2_MAX_ISO_NUM][8];
    float ynr_hi_min_adj_V2[RK_YNR_V2_MAX_ISO_NUM];
    float ynr_hi_edge_thed_V2[RK_YNR_V2_MAX_ISO_NUM];

} Calibdb_Ynr_params_V2_t;

typedef struct Calibdb_Ynr_V2_s {
    struct list_head listItem;
    char modeName[64];
    int enable;

    struct list_head listHead;
} Calibdb_Ynr_V2_t;


typedef struct Calibdb_Cnr_params_V1_s
{
    struct list_head listItem;
    char snr_mode[64];
    char sensor_mode[64];

    int enable;
    float iso[RK_CNR_V1_MAX_ISO_NUM];
    int rkcnr_hq_bila_bypass[RK_CNR_V1_MAX_ISO_NUM];
    int rkcnr_lq_bila_bypass[RK_CNR_V1_MAX_ISO_NUM];

    // gain
    float rkcnr_exgain[RK_CNR_V1_MAX_ISO_NUM];
    float rkcnr_g_gain[RK_CNR_V1_MAX_ISO_NUM];

    //
    float ratio[RK_CNR_V1_MAX_ISO_NUM];
    float offset[RK_CNR_V1_MAX_ISO_NUM];

    // step1
    // median filter
    float medRatio1[RK_CNR_V1_MAX_ISO_NUM];

    // bilateral filter
    float sigmaR1[RK_CNR_V1_MAX_ISO_NUM];
    float uvgain1[RK_CNR_V1_MAX_ISO_NUM];
    float bfRatio1[RK_CNR_V1_MAX_ISO_NUM];
    int  hbf_wgt_clip[RK_CNR_V1_MAX_ISO_NUM];


    // step2
    // median filter
    float medRatio2[RK_CNR_V1_MAX_ISO_NUM];

    // bilateral filter
    float sigmaR2[RK_CNR_V1_MAX_ISO_NUM];
    float uvgain2[RK_CNR_V1_MAX_ISO_NUM];


    // step3
    // bilateral filter
    float sigmaR3[RK_CNR_V1_MAX_ISO_NUM];
    float uvgain3[RK_CNR_V1_MAX_ISO_NUM];
    float bfRatio3[RK_CNR_V1_MAX_ISO_NUM];

    // bilateral filter kernels
    float kernel_5x5_table[5];
} Calibdb_Cnr_params_V1_t;

typedef struct Calibdb_Cnr_V1_s {
    struct list_head listItem;
    char modeName[64];
    int enable;

    struct list_head listHead;
} Calibdb_Cnr_V1_t;


typedef struct Calibdb_Sharp_params_V3_s
{
    struct list_head listItem;
    char snr_mode[64];
    char sensor_mode[64];
    float iso[RK_SHARP_V3_MAX_ISO_NUM];

    short luma_point        [RK_SHARP_V3_LUMA_POINT_NUM];
    short luma_sigma        [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    float pbf_gain          [RK_SHARP_V3_MAX_ISO_NUM];
    float pbf_add           [RK_SHARP_V3_MAX_ISO_NUM];
    float pbf_ratio         [RK_SHARP_V3_MAX_ISO_NUM];
    float gaus_ratio        [RK_SHARP_V3_MAX_ISO_NUM];
    float sharp_ratio       [RK_SHARP_V3_MAX_ISO_NUM];
    short lum_clip_h        [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    float bf_gain           [RK_SHARP_V3_MAX_ISO_NUM];
    float bf_add            [RK_SHARP_V3_MAX_ISO_NUM];
    float bf_ratio          [RK_SHARP_V3_MAX_ISO_NUM];
    short ehf_th            [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];

    float kernel_pre_bila_filter[9][RK_SHARP_V3_MAX_ISO_NUM];
    float kernel_range_filter   [9][RK_SHARP_V3_MAX_ISO_NUM];
    float kernel_bila_filter    [9][RK_SHARP_V3_MAX_ISO_NUM];

    //////////////////////////////////////////////////////////////////////////
    // test params
    float sharp_ratio_h     [RK_SHARP_V3_MAX_ISO_NUM];
    float sharp_ratio_m     [RK_SHARP_V3_MAX_ISO_NUM];
    float sharp_ratio_l     [RK_SHARP_V3_MAX_ISO_NUM];
    short clip_hf           [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    short clip_mf           [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    short clip_lf           [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];
    short local_wgt         [RK_SHARP_V3_MAX_ISO_NUM][RK_SHARP_V3_LUMA_POINT_NUM];

    short kernel_hf_filter  [RK_SHARP_V3_MAX_ISO_NUM][9];
    short kernel_mf_filter  [RK_SHARP_V3_MAX_ISO_NUM][9];
    short kernel_lf_filter  [RK_SHARP_V3_MAX_ISO_NUM][9];
} Calibdb_Sharp_params_V3_t;


typedef struct Calibdb_Sharp_V3_s {
    struct list_head listItem;
    char modeName[64];
    int enable;

    struct list_head listHead;
} Calibdb_Sharp_V3_t;

/*****************************************************************************/
/**
 * @brief   Global AEC calibration structure of isp2.0
 */
/*****************************************************************************/
typedef struct CalibDb_Aec_WinScale_V21_s {
    float h_offs;
    float v_offs;
    float h_size;
    float v_size;
} CalibDb_Aec_WinScale_V21_t;

typedef struct CalibDb_Aec_CalibPara_s {
    struct list_head                     listHead;
    char                          scene[CALIBDB_MAX_SCENE_NAME_LENGTH];
    CalibDb_AeEnvLvCalib_t        stEnvLvCalib;
    CalibDb_Aec_WinScale_V21_t        InRawWinScale;
    CalibDb_Aec_WinScale_V21_t        TmoRawWinScale;
    CalibDb_Aec_WinScale_V21_t        YuvWinScale;
} CalibDb_Aec_CalibPara_t;

typedef struct CalibDb_AeRoute_Attr_V21_s {
    CalibDb_LinAeRoute_Attr_t LinAeSeperate;
    CalibDb_HdrAeRoute_Attr_t HdrAeSeperate;
} CalibDb_AeRoute_Attr_V21_t;

#define AEC_MAX_GRIDWEIGHT_NUM (225)
typedef struct CalibDb_AecCommon_Attr_V21_s {
    uint8_t                          enable;
    uint8_t                          AecRunInterval;
    RKAiqOPMode_t                    AecOpType;
    CalibDb_CamRawStatsMode_t        RawStatsMode;
    CalibDb_CamHistStatsMode_t       HistStatsMode;
    CalibDb_CamYRangeMode_t          YRangeMode;
    uint8_t                          GridWeights[AEC_MAX_GRIDWEIGHT_NUM];

    CalibDb_AecIrisCtrl_t            stIris;
    CalibDb_AeSyncTest_t             stSyncTest;
    //antiflicker
    CalibDb_AntiFlickerAttr_t        stAntiFlicker;
    //initial exp
    CalibDb_ExpInitExp_t             stInitExp;
    //DayOrNight switch
    CalibDb_DNSwitch_Attr_t          stDNSwitch;
    //ExpSeperate
    CalibDb_AeRoute_Attr_V21_t        stAeRoute;
    //manual expvalue
    CalibDb_MeAttr_t                 stManual;
    CalibDb_AeAttr_t                 stAuto;
} CalibDb_AecCommon_Attr_V21_t;

typedef struct CalibDb_LinearAE_Attr_V21_s {
    uint8_t                 RawStatsEn;
    float                   SetPoint;                   /**< set point to hit by the ae control system */
    float                   NightSetPoint;
    float                   ToleranceIn;
    float                   ToleranceOut;
    float                   Evbias;
    CalibDb_AeStrategyMode_t        StrategyMode;
    bool                    DySetPointEn;
    CalibDb_AecDynamicSetpoint_t DySetpoint;
    CalibDb_AecBacklight_t  BackLightConf;
    CalibDb_AecOverExpCtrl_t OverExpCtrl;
} CalibDb_LinearAE_Attr_V21_t;

typedef struct CalibDb_Aec_TunePara_s {
    struct list_head                     listHead;
    char                          scene[CALIBDB_MAX_SCENE_NAME_LENGTH];
    CalibDb_AecCommon_Attr_V21_t      CommCtrl;
    CalibDb_LinearAE_Attr_V21_t   LinearAeCtrl;
    CalibDb_HdrAE_Attr_t          HdrAeCtrl;
} CalibDb_Aec_TunePara_t;

typedef struct CalibDb_Aec_Para_v21_s {
    CalibDb_Aec_CalibPara_t       CalibPara;
    CalibDb_Aec_TunePara_t        TunePara;
} CalibDb_Aec_Para_v21_t;


typedef struct CamCalibDbContextIsp21_s {
    CalibDb_Header_t header;
    struct list_head awb_calib_para_v201;
    struct list_head awb_adjust_para;
    struct list_head ae_calib_para;
    struct list_head ae_tuning_para;
    CalibDb_Lut3d_t lut3d;
    CalibDb_AF_t af;
    CalibDb_Amerge_Para_t amerge;
    CalibDb_Adrc_Para_t adrc;
    CalibDb_Blc_t blc;
    CalibDb_Dpcc_t dpcc;
    CalibDb_Lsc_t lsc;
    CalibDb_RKDM_t dm;
    CalibDb_Ccm_t ccm;
    CalibDb_Gamma_t gamma;
    CalibDb_Degamma_t degamma;
    CalibDb_Gic_Isp21_t gic;
    CalibDb_Dehaze_Isp21_t dehaze;
    CalibDb_LDCH_t aldch;
    CalibDb_LUMA_DETECT_t lumaDetect;
    CalibDb_Cpsl_t cpsl;
    CalibDb_ColorAsGrey_t colorAsGrey;
    CalibDb_cProc_t cProc;
    CalibDb_IE_t    ie;
    CalibDb_ExpSet_para_t expset;
    CalibDb_Module_Info_t module;
    //vw
    struct list_head list_bayernr_v2;
    struct list_head list_ynr_v2;
    struct list_head list_cnr_v1;
    struct list_head list_sharp_v3;
} CamCalibDbContextIsp21_t;

#pragma pack()

#endif
