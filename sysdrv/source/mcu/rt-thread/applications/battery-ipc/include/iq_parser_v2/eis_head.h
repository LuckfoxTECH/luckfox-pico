#ifndef __EIS_HEAD_H__
#define __EIS_HEAD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef enum eis_mode_e {
    EIS_MODE_IMU_ONLY,
    EIS_MODE_IMG_ONLY,
    EIS_MODE_IMU_AND_IMG,
} eis_mode_t;

// clang-format off
typedef struct CalibDbV2_Eis_s {
    // M4_BOOL_DESC("enable", "0")
    bool enable;
    // M4_ENUM_DESC("mode", "eis_mode_t", "EIS_MODE_IMU_ONLY")
    eis_mode_t mode;
    // M4_STRING_MASK_DESC("debug_xml_path", M4_SIZE(1,1), M4_RANGE(0, 255), "/tmp/eis_cfg.xml", M4_DYNAMIC(1))
    const char* debug_xml_path;
    // M4_NUMBER_DESC("src_image_width", u32, 2593, M4_RANGE(0,65535), 0)
    uint32_t src_image_width;
    // M4_NUMBER_DESC("src_image_height", u32, 1944, M4_RANGE(0,65535), 0)
    uint32_t src_image_height;
    // M4_NUMBER_DESC("dst_image_width", u32, 2592, M4_RANGE(0,65535), 0)
    uint32_t dst_image_width;
    // M4_NUMBER_DESC("dst_image_height", u32, 1944, M4_RANGE(0,65535), 0)
    uint32_t dst_image_height;
    // M4_NUMBER_DESC("clip_ratio_x", "f32", M4_RANGE(0,1), "0.1", M4_DIGIT(2))
    float clip_ratio_x;
    // M4_NUMBER_DESC("clip_ratio_y", "f32", M4_RANGE(0,1), "0.1", M4_DIGIT(2))
    float clip_ratio_y;
    // M4_ARRAY_DESC("light_center", "f64", M4_SIZE(1,2), M4_RANGE(0,255), "[1.3511178239999999e+003, 7.3948608000000002e+002]", M4_DIGIT(32), M4_DYNAMIC(0))
    double light_center[2];
    // M4_ARRAY_DESC("distortion_coeff", "f64", M4_SIZE(1,4), M4_RANGE(0,255), "[-1.8302636074851755e+003, 4.2379512840610002e-004, -2.5076748550000000e-007, 1.2724700000000000e-010]", M4_DIGIT(32), M4_DYNAMIC(0))
    double distortion_coeff[4];
} CalibDbV2_Eis_t;
// clang-format on

#ifdef __cplusplus
}
#endif

#endif
