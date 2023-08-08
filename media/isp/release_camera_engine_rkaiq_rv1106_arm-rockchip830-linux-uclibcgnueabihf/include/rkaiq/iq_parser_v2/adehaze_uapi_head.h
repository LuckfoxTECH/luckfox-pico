#ifndef __ADEHAZE_UAPI_HEAD_H__
#define __ADEHAZE_UAPI_HEAD_H__

#include "adehaze_head.h"

typedef struct mDehazeDataV11_s {
    // M4_NUMBER_DESC("dc_min_th", "f32", M4_RANGE(0,255), "64", M4_DIGIT(4))
    float dc_min_th;
    // M4_NUMBER_DESC("dc_max_th", "f32", M4_RANGE(0,255), "192", M4_DIGIT(4))
    float dc_max_th;
    // M4_NUMBER_DESC("yhist_th", "f32", M4_RANGE(0,255), "249", M4_DIGIT(4))
    float yhist_th;
    // M4_NUMBER_DESC("yblk_th", "f32", M4_RANGE(0,512), "0.002", M4_DIGIT(4))
    float yblk_th;
    // M4_NUMBER_DESC("dark_th", "f32", M4_RANGE(0,255), "250", M4_DIGIT(4))
    float dark_th;
    // M4_NUMBER_DESC("bright_min", "f32", M4_RANGE(0,255), "180", M4_DIGIT(4))
    float bright_min;
    // M4_NUMBER_DESC("bright_max", "f32", M4_RANGE(0,255), "240", M4_DIGIT(4))
    float bright_max;
    // M4_NUMBER_DESC("wt_max", "f32", M4_RANGE(0,1), "0.9", M4_DIGIT(4))
    float wt_max;
    // M4_NUMBER_DESC("air_min", "f32", M4_RANGE(0,255), "200", M4_DIGIT(4))
    float air_min;
    // M4_NUMBER_DESC("air_max", "f32", M4_RANGE(0,255), "250", M4_DIGIT(4))
    float air_max;
    // M4_NUMBER_DESC("tmax_base", "f32", M4_RANGE(0,255), "125", M4_DIGIT(4))
    float tmax_base;
    // M4_NUMBER_DESC("tmax_off", "f32", M4_RANGE(0,1), "0.1", M4_DIGIT(4))
    float tmax_off;
    // M4_NUMBER_DESC("tmax_max", "f32", M4_RANGE(0,1), "0.8", M4_DIGIT(4))
    float tmax_max;
    // M4_NUMBER_DESC("cfg_wt", "f32", M4_RANGE(0,1), "0.8", M4_DIGIT(4))
    float cfg_wt;
    // M4_NUMBER_DESC("cfg_air", "f32", M4_RANGE(0,255), "210", M4_DIGIT(4))
    float cfg_air;
    // M4_NUMBER_DESC("cfg_tmax", "f32", M4_RANGE(0,1), "0.2", M4_DIGIT(4))
    float cfg_tmax;
    // M4_NUMBER_DESC("dc_weitcur", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float dc_weitcur;
    // M4_NUMBER_DESC("bf_weight", "f32", M4_RANGE(0,1), "0.5", M4_DIGIT(4))
    float bf_weight;
    // M4_NUMBER_DESC("range_sigma", "f32", M4_RANGE(0,1), "0.04", M4_DIGIT(4))
    float range_sigma;
    // M4_NUMBER_DESC("space_sigma_pre", "f32", M4_RANGE(0,1), "0.4", M4_DIGIT(4))
    float space_sigma_pre;
    // M4_NUMBER_DESC("space_sigma_cur", "f32", M4_RANGE(0,1), "0.8", M4_DIGIT(4))
    float space_sigma_cur;
} mDehazeDataV11_t;

typedef struct mDehaze_setting_v11_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("air_lc_en", "1")
    bool air_lc_en;
    // M4_NUMBER_DESC("stab_fnum", "f32", M4_RANGE(0,31), "8", M4_DIGIT(0))
    float stab_fnum;
    // M4_NUMBER_DESC("sigma", "f32", M4_RANGE(1,255), "255", M4_DIGIT(0))
    float sigma;
    // M4_NUMBER_DESC("wt_sigma", "f32", M4_RANGE(0,256), "8.0", M4_DIGIT(4))
    float wt_sigma;
    // M4_NUMBER_DESC("air_sigma", "f32", M4_RANGE(4,1024), "120.0", M4_DIGIT(4))
    float air_sigma;
    // M4_NUMBER_DESC("tmax_sigma", "f32", M4_RANGE(0.0004,1), "0.0100", M4_DIGIT(4))
    float tmax_sigma;
    // M4_NUMBER_DESC("pre_wet", "f32", M4_RANGE(0,16), "8", M4_DIGIT(0))
    float pre_wet;
    // M4_STRUCT_DESC("DehazeData", "normal_ui_style")
    mDehazeDataV11_t DehazeData;
} mDehaze_setting_v11_t;

typedef struct mEnhanceDataV11_s {
    // M4_NUMBER_DESC("enhance_value", "f32", M4_RANGE(0,32), "1", M4_DIGIT(4))
    float enhance_value;
    // M4_NUMBER_DESC("enhance_chroma", "f32", M4_RANGE(0,32), "1", M4_DIGIT(4))
    float enhance_chroma;
} mEnhanceDataV11_t;

typedef struct mEnhance_setting_v11_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_ARRAY_DESC("enhance_curve", "f32", M4_SIZE(1,17),  M4_RANGE(0, 1024), "[0,64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    float enhance_curve[DHAZ_ENHANCE_CURVE_KNOTS_NUM];
    // M4_STRUCT_DESC("EnhanceData", "normal_ui_style")
    mEnhanceDataV11_t EnhanceData;
} mEnhance_setting_v11_t;

typedef struct mHistDataV11_s {
    // M4_NUMBER_DESC("hist_gratio", "f32", M4_RANGE(0,32), "4", M4_DIGIT(4))
    float hist_gratio;
    // M4_NUMBER_DESC("hist_th_off", "f32", M4_RANGE(0,255), "64", M4_DIGIT(4))
    float hist_th_off;
    // M4_NUMBER_DESC("hist_k", "f32", M4_RANGE(0,8), "2", M4_DIGIT(4))
    float hist_k;
    // M4_NUMBER_DESC("hist_min", "f32", M4_RANGE(0,2), "0.016", M4_DIGIT(4))
    float hist_min;
    // M4_NUMBER_DESC("hist_scale", "f32", M4_RANGE(0,32), "0.09", M4_DIGIT(4))
    float hist_scale;
    // M4_NUMBER_DESC("cfg_gratio", "f32", M4_RANGE(0,32), "2", M4_DIGIT(4))
    float cfg_gratio;
} mHistDataV11_t;

typedef struct mHist_setting_v11_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("hist_para_en", "1")
    bool hist_para_en;
    // M4_STRUCT_DESC("HistData", "normal_ui_style")
    mHistDataV11_t HistData;
} mHist_setting_v11_t;

typedef struct mDehazeAttrV11_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_NUMBER_DESC("cfg_alpha", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float cfg_alpha;
    // M4_STRUCT_DESC("dehaze_setting", "normal_ui_style")
    mDehaze_setting_v11_t dehaze_setting;
    // M4_STRUCT_DESC("enhance_setting", "normal_ui_style")
    mEnhance_setting_v11_t enhance_setting;
    // M4_STRUCT_DESC("hist_setting", "normal_ui_style")
    mHist_setting_v11_t hist_setting;
} mDehazeAttrV11_t;

typedef struct mDehazeAttrInfoV11_s {
    // M4_NUMBER_DESC("ISO", "f32", M4_RANGE(50,1000000000), "0", M4_DIGIT(4))
    float ISO;
    // M4_NUMBER_DESC("EnvLv", "f32", M4_RANGE(0,1), "0", M4_DIGIT(6))
    float EnvLv;
    // M4_BOOL_DESC("updateMDehazeStrth", "0", "1")
    bool updateMDehazeStrth;
    // M4_NUMBER_DESC("MDehazeStrth", "u8", M4_RANGE(0,100), "50", M4_DIGIT(0))
    unsigned int MDehazeStrth;
    // M4_BOOL_DESC("updateMEnhanceStrth", "0", "1")
    bool updateMEnhanceStrth;
    // M4_NUMBER_DESC("MEnhanceStrth", "u8", M4_RANGE(0,100), "50", M4_DIGIT(0))
    unsigned int MEnhanceStrth;
    // M4_BOOL_DESC("updateMEnhanceChromeStrth", "0", "1")
    bool updateMEnhanceChromeStrth;
    // M4_NUMBER_DESC("MEnhanceChromeStrth", "u8", M4_RANGE(0,100), "50", M4_DIGIT(0))
    unsigned int MEnhanceChromeStrth;
} mDehazeAttrInfoV11_t;

typedef struct mEnhanceDataV12_s {
    // M4_ARRAY_DESC("enhance_curve", "f32", M4_SIZE(1,17),  M4_RANGE(0, 1023), "[0,64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1023]", M4_DIGIT(0), M4_DYNAMIC(0))
    float enhance_curve[DHAZ_ENHANCE_CURVE_KNOTS_NUM];
    // M4_ARRAY_DESC("enh_luma", "f32", M4_SIZE(1,17),	M4_RANGE(0, 16), "[1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]", M4_DIGIT(2), M4_DYNAMIC(0))
    float enh_luma[DHAZ_ENHANCE_CURVE_KNOTS_NUM];
    // M4_NUMBER_DESC("enhance_value", "f32", M4_RANGE(0,32), "1", M4_DIGIT(4))
    float enhance_value;
    // M4_NUMBER_DESC("enhance_chroma", "f32", M4_RANGE(0,32), "1", M4_DIGIT(4))
    float enhance_chroma;
} mEnhanceDataV12_t;

typedef struct mEnhance_setting_v12_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("color_deviate_en", "0")
    bool color_deviate_en;
    // M4_BOOL_DESC("enh_luma_en", "0")
    bool enh_luma_en;
    // M4_STRUCT_DESC("EnhanceData", "normal_ui_style")
    mEnhanceDataV12_t EnhanceData;
} mEnhance_setting_v12_t;

typedef struct mManual_curve_s {
    // M4_ARRAY_MARK_DESC("curve_x", "u32", M4_SIZE(1,17),  M4_RANGE(0, 1023), "[0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1023]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int curve_x[DHAZ_V12_HIST_WR_CURVE_NUM];
    // M4_ARRAY_MARK_DESC("curve_y", "u32", M4_SIZE(1,17),  M4_RANGE(0, 1023), "[0, 64, 128, 192, 256, 320, 384, 448, 512, 576, 640, 704, 768, 832, 896, 960, 1023]", M4_DIGIT(0), M4_DYNAMIC(0), 0)
    int curve_y[DHAZ_V12_HIST_WR_CURVE_NUM];
} mManual_curve_t;

typedef struct mhist_wr_semiauto_s {
    // M4_NUMBER_DESC("clim0", "f32", M4_RANGE(0.1,32), "1", M4_DIGIT(4))
    float clim0;
    // M4_NUMBER_DESC("clim1", "f32", M4_RANGE(0.1,32), "1", M4_DIGIT(4))
    float clim1;
    // M4_NUMBER_DESC("dark_th", "f32", M4_RANGE(0,1023), "1", M4_DIGIT(0))
    float dark_th;
} mhist_wr_semiauto_t;

typedef struct mHistWr_s {
    // M4_ENUM_DESC("mode", "HistWrMode_t", "HIST_WR_MANUAL")
    HistWrMode_t mode;
    // M4_STRUCT_DESC("manual_curve", "normal_ui_style")
    mManual_curve_t manual_curve;
    // M4_STRUCT_DESC("semiauto_curve", "normal_ui_style")
    mhist_wr_semiauto_t semiauto_curve;
} mHistWr_t;

typedef struct mHist_setting_v12_s {
    // M4_BOOL_DESC("en", "1")
    bool en;
    // M4_BOOL_DESC("hist_para_en", "1")
    bool hist_para_en;
    // M4_STRUCT_DESC("hist_wr", "normal_ui_style")
    mHistWr_t hist_wr;
    // M4_STRUCT_DESC("HistData", "normal_ui_style")
    mHistDataV11_t HistData;
} mHist_setting_v12_t;

typedef struct mDehazeAttrV12_s {
    // M4_BOOL_DESC("Enable", "1")
    bool Enable;
    // M4_NUMBER_DESC("cfg_alpha", "f32", M4_RANGE(0,1), "1", M4_DIGIT(4))
    float cfg_alpha;
    // M4_STRUCT_DESC("dehaze_setting", "normal_ui_style")
    mDehaze_setting_v11_t dehaze_setting;
    // M4_STRUCT_DESC("enhance_setting", "normal_ui_style")
    mEnhance_setting_v12_t enhance_setting;
    // M4_STRUCT_DESC("hist_setting", "normal_ui_style")
    mHist_setting_v12_t hist_setting;
} mDehazeAttrV12_t;

#endif /*__ADEHAZE_UAPI_HEAD_H__*/
