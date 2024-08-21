#ifndef __CAC_HEAD_H__
#define __CAC_HEAD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define RKCAC_PSF_RADIUS_X       3
#define RKCAC_PSF_RADIUS_Y       2
#define RKCAC_STRENGTH_TABLE_LEN 22
#define RKCAC_SQRT_TABLE_LEN     25
#define RKCAC_MAX_PATH_LEN       255
#define RKCAC_MAX_ISO_LEVEL      14

typedef enum CalibDbV2_Cac_ClipGMode_e {
    RKAIQ_CAC_CLIP_G_MODE_NONE,
    RKAIQ_CAC_CLIP_G_MODE_MINIMAL,
    RKAIQ_CAC_CLIP_G_MODE_AVERAGE,
} CalibDbV2_Cac_ClipGMode_t;

// clang-format off
typedef struct CalibDbV2_Cac_V03_SettingByIso_s {
    // M4_NUMBER_MARK_DESC("iso", "u32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index1")
    uint32_t iso;
    // M4_BOOL_DESC("bypass", "0")
    bool bypass;
    // M4_BOOL_DESC("AutoHighLightDetect", "0")
    bool AutoHighLightDetect;
    // M4_NUMBER_DESC("AutoHighLightOffset", "u8", M4_RANGE(0,127), "0", M4_DIGIT(0), M4_HIDE(0))
    uint8_t AutoHighLightOffset;
    // M4_NUMBER_DESC("FixHighLightBase", "u8", M4_RANGE(0,255), "0", M4_DIGIT(0), M4_HIDE(0))
    uint8_t FixHighLightBase;
    // M4_NUMBER_DESC("YCompensate", "f32", M4_RANGE(0, 1.00), "0.0", M4_DIGIT(2), M4_HIDE(0))
    float YCompensate;
    // M4_NUMBER_DESC("AutoStrengthU", "f32", M4_RANGE(0, 1.00), "0.0", M4_DIGIT(2), M4_HIDE(0))
    float AutoStrengthU;
    // M4_NUMBER_DESC("AutoStrengthV", "f32", M4_RANGE(0, 1.00), "0.0", M4_DIGIT(2), M4_HIDE(0))
    float AutoStrengthV;
    // M4_NUMBER_DESC("GrayStrengthU", "f32", M4_RANGE(0, 1.00), "0.0", M4_DIGIT(2), M4_HIDE(0))
    float GrayStrengthU;
    // M4_NUMBER_DESC("GrayStrengthV", "f32", M4_RANGE(0, 1.00), "0.0", M4_DIGIT(2), M4_HIDE(0))
    float GrayStrengthV;
} CalibV2_Cac_V03_SettingByIso_t;

typedef struct CalibDbV2_Cac_V10_SettingByIso_s {
    // M4_NUMBER_MARK_DESC("iso", "u32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index1")
    uint32_t iso;
    // M4_BOOL_DESC("bypass", "0")
    bool bypass;
    // M4_NUMBER_DESC("global_strength", "f32", M4_RANGE(0, 16.00), "0.0", M4_DIGIT(4), M4_HIDE(0))
    float global_strength;
    // M4_ARRAY_DESC("strength_table", "f32", M4_SIZE(1,22), M4_RANGE(0.00, 16.00), "1.00", M4_DIGIT(4), M4_DYNAMIC(0))
    float strength_table[RKCAC_STRENGTH_TABLE_LEN];
} CalibV2_Cac_V10_SettingByIso_t;

typedef struct CalibDbV2_Cac_V11_SettingByIso_s {
    // M4_NUMBER_MARK_DESC("iso", "u32", M4_RANGE(50, 204800), "50", M4_DIGIT(1), "index1")
    uint32_t iso;
    // M4_BOOL_DESC("bypass", "0")
    bool bypass;
    // M4_NUMBER_DESC("global_strength", "f32", M4_RANGE(0, 16.00), "0.00", M4_DIGIT(4), M4_HIDE(0))
    float global_strength;
    // M4_ARRAY_DESC("strength_table", "f32", M4_SIZE(1,22), M4_RANGE(0.00, 16.00), "1.00", M4_DIGIT(4), M4_DYNAMIC(0))
    float strength_table[RKCAC_STRENGTH_TABLE_LEN];
    // M4_ENUM_DESC("clip_g_mode", "CalibDbV2_Cac_ClipGMode_t","RKAIQ_CAC_CLIP_G_MODE_NONE,")
    CalibDbV2_Cac_ClipGMode_t clip_g_mode;
    // M4_BOOL_DESC("neg_clip0_enable", "0")
    bool neg_clip0_enable;
    // M4_BOOL_DESC("edge_detect_en", "0")
    bool edge_detect_en;
    // M4_NUMBER_DESC("flat_thed_b", "f32", M4_RANGE(0,1.00), "0", M4_DIGIT(2),M4_HIDE(0))
    float flat_thed_b;
    // M4_NUMBER_DESC("flat_thed_r", "f32", M4_RANGE(0,1.00), "0", M4_DIGIT(2),M4_HIDE(0))
    float flat_thed_r;
    // M4_NUMBER_DESC("offset_b", "u32", M4_RANGE(0,4095), "0", M4_DIGIT(0),M4_HIDE(0))
    uint32_t offset_b;
    // M4_NUMBER_DESC("offset_r", "u32", M4_RANGE(0,4095), "0", M4_DIGIT(0),M4_HIDE(0))
    uint32_t offset_r;
    // M4_BOOL_DESC("expo_det_b_en", "0")
    bool expo_det_b_en;
    // M4_BOOL_DESC("expo_det_r_en", "0")
    bool expo_det_r_en;
    // M4_NUMBER_DESC("expo_thed_b", "u32", M4_RANGE(0,4095), "0", M4_DIGIT(0),M4_HIDE(0))
    uint32_t expo_thed_b;
    // M4_NUMBER_DESC("expo_thed_r", "u32", M4_RANGE(0,4095), "0", M4_DIGIT(0),M4_HIDE(0))
    uint32_t expo_thed_r;
    // M4_NUMBER_DESC("expo_adj_b", "u32", M4_RANGE(0,4095), "0", M4_DIGIT(0),M4_HIDE(0))
    uint32_t expo_adj_b;
    // M4_NUMBER_DESC("expo_adj_r", "u32", M4_RANGE(0,4095), "0", M4_DIGIT(0),M4_HIDE(0))
    uint32_t expo_adj_r;
} CalibV2_Cac_V11_SettingByIso_t;

typedef struct CalibDbV2_Cac_V03_TuningPara_s {
    // M4_STRUCT_LIST_DESC("SettingByIso", M4_SIZE(1,14), "normal_ui_style")
    CalibV2_Cac_V03_SettingByIso_t* SettingByIso;
    int SettingByIso_len;
} CalibDbV2_Cac_V03_TuningPara_t;

typedef struct CalibDbV2_Cac_V10_TuningPara_s {
    // M4_STRUCT_LIST_DESC("SettingByIso", M4_SIZE(1,14), "normal_ui_style")
    CalibV2_Cac_V10_SettingByIso_t* SettingByIso;
    int SettingByIso_len;
} CalibDbV2_Cac_V10_TuningPara_t;

typedef struct CalibDbV2_Cac_V11_TuningPara_s {
    // M4_STRUCT_LIST_DESC("SettingByIso", M4_SIZE(1,14), "normal_ui_style")
    CalibV2_Cac_V11_SettingByIso_t* SettingByIso;
    int SettingByIso_len;
} CalibDbV2_Cac_V11_TuningPara_t;

typedef struct CalibDbV2_Cac_SettingPara_s {
    // M4_BOOL_DESC("enable", "0")
    bool enable;
    // M4_STRING_DESC("psf_path", M4_SIZE(1,1), M4_RANGE(0, 255), "/etc/iqfiles/cac_map.bin", M4_DYNAMIC(0))
    char psf_path[RKCAC_MAX_PATH_LEN];
    // M4_NUMBER_DESC("psf_shift_bits", "u8", M4_RANGE(0,10), "2", M4_DIGIT(0),M4_HIDE(0))
    uint8_t psf_shift_bits;
    // M4_BOOL_DESC("center_en", "0")
    uint8_t center_en;
    // M4_NUMBER_DESC("center_x", "u16", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(0))
    uint16_t center_x;
    // M4_NUMBER_DESC("center_y", "u16", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(0))
    uint16_t center_y;
} CalibDbV2_Cac_SettingPara_t;

typedef struct CalibDbV2_Cac_V03_SettingPara_s {
    // M4_BOOL_DESC("enable", "0")
    bool enable;
} CalibDbV2_Cac_V03_SettingPara_t;

typedef struct CalibDbV2_Cac_V03_s {
    // M4_STRUCT_DESC("SettingPara", "normal_ui_style")
    CalibDbV2_Cac_V03_SettingPara_t SettingPara;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_Cac_V03_TuningPara_t TuningPara;
} CalibDbV2_Cac_V03_t;

typedef struct CalibDbV2_Cac_V10_s {
    // M4_STRUCT_DESC("SettingPara", "normal_ui_style")
    CalibDbV2_Cac_SettingPara_t SettingPara;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_Cac_V10_TuningPara_t TuningPara;
} CalibDbV2_Cac_V10_t;

typedef struct CalibDbV2_Cac_V11_s {
    // M4_STRUCT_DESC("SettingPara", "normal_ui_style")
    CalibDbV2_Cac_SettingPara_t SettingPara;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_Cac_V11_TuningPara_t TuningPara;
} CalibDbV2_Cac_V11_t;

// clang-format on

#ifdef __cplusplus
}
#endif

#endif
