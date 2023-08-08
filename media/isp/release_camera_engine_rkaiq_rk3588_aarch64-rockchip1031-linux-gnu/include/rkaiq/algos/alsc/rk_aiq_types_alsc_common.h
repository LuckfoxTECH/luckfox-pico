#ifndef __RK_AIQ_TYPES_ALSC_COMMON_H__
#define __RK_AIQ_TYPES_ALSC_COMMON_H__

#include "rk_aiq_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef LSC_NAME
#define LSC_NAME (32U)
#endif

#define LSC_MAX_RESOLUTION 2
#define LSC_MAX_ILLUMINANT 7

typedef struct lsc_name_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char name[LSC_NAME];
} lsc_name_t;

typedef struct CalibDbV2_LscTableProfile_s {
    // M4_STRING_DESC("name", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char name[LSC_NAME];
    // M4_STRING_DESC("resolution", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char resolution[LSC_NAME];
    // M4_STRING_DESC("illumination", M4_SIZE(1,1), M4_RANGE(0, 32), "default", M4_DYNAMIC(0))
    char illumination[LSC_NAME];
    // M4_NUMBER_DESC("vignetting", "f32", M4_RANGE(0,100), "100", M4_DIGIT(0))
    float vignetting;

    // M4_STRUCT_DESC("lsc_samples_red", "normal_ui_style")
    Cam17x17UShortMatrix_t lsc_samples_red;
    // M4_STRUCT_DESC("lsc_samples_greenR", "normal_ui_style")
    Cam17x17UShortMatrix_t lsc_samples_greenR;
    // M4_STRUCT_DESC("lsc_samples_greenB", "normal_ui_style")
    Cam17x17UShortMatrix_t lsc_samples_greenB;
    // M4_STRUCT_DESC("lsc_samples_blue", "normal_ui_style")
    Cam17x17UShortMatrix_t lsc_samples_blue;
} CalibDbV2_LscTableProfile_t;

#ifdef __cplusplus
}
#endif

#endif /*__RK_AIQ_TYPES_ALSC_COMMON_H__*/
