#ifndef __CAC_HEAD_H__
#define __CAC_HEAD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define RKCAC_PSF_RADIUS_X       3
#define RKCAC_PSF_RADIUS_Y       2
#define RKCAC_STRENGTH_TABLE_LEN 22
#define RKCAC_STRENGTH_FIX_BITS  7
#define RKCAC_SQRT_TABLE_LEN     25
#define RKCAC_MAX_PATH_LEN       255
#define RKCAC_MAX_ISO_LEVEL      14

// clang-format off
typedef struct CalibDbV2_Cac_SettingByIso_s {
    // M4_NUMBER_DESC("iso", "u32", M4_RANGE(0,4294967295), "0", M4_DIGIT(0),M4_HIDE(0))
    uint32_t iso;
    // M4_NUMBER_DESC("bypass", "u8",M4_RANGE(0,1), "0", M4_DIGIT(0),M4_HIDE(0))
    uint8_t bypass;
    // M4_ARRAY_DESC("strength_table", "f32", M4_SIZE(0,22), M4_RANGE(0,1), "1.0", M4_DIGIT(1), M4_DYNAMIC(0))
    float strength_table[RKCAC_STRENGTH_TABLE_LEN];
} CalibDbV2_Cac_SettingByIso_t;

typedef struct CalibDbV2_Cac_TuningPara_s {
    // M4_STRUCT_LIST_DESC("SettingByIso", M4_SIZE_DYNAMIC, "double_index_list")
    CalibDbV2_Cac_SettingByIso_t* SettingByIso;
    int SettingByIso_len;
} CalibDbV2_Cac_TuningPara_t;

typedef struct CalibDbV2_Cac_SettingPara_s {
    // M4_NUMBER_DESC("enable", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0),M4_HIDE(0))
    uint8_t enable;
    // M4_STRING_DESC("psf_path", M4_SIZE(1,1), M4_RANGE(0, 255), "/etc/iqfiles/cac", M4_DYNAMIC(0))
    char psf_path[RKCAC_MAX_PATH_LEN];
    // M4_NUMBER_DESC("psf_shift_bits", "u8", M4_RANGE(0,10), "2", M4_DIGIT(0),M4_HIDE(0))
    uint8_t psf_shift_bits;
    // M4_NUMBER_DESC("center_en", "u8", M4_RANGE(0,1), "0", M4_DIGIT(0),M4_HIDE(0))
    uint8_t center_en;
    // M4_NUMBER_DESC("center_x", "u16", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(0))
    uint16_t center_x;
    // M4_NUMBER_DESC("center_y", "u16", M4_RANGE(0,65535), "0", M4_DIGIT(0),M4_HIDE(0))
    uint16_t center_y;
} CalibDbV2_Cac_SettingPara_t;

typedef struct CalibDbV2_Cac_s {
    // M4_STRUCT_DESC("SettingPara", "normal_ui_style")
    CalibDbV2_Cac_SettingPara_t SettingPara;
    // M4_STRUCT_DESC("TuningPara", "normal_ui_style")
    CalibDbV2_Cac_TuningPara_t TuningPara;
} CalibDbV2_Cac_t;
// clang-format on

#ifdef __cplusplus
}
#endif

#endif
