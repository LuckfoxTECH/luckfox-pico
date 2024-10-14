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

typedef enum ais_use_algo_type_s {
    AIS_USE_ALGO_EIS,
    AIS_USE_ALGO_DIS,
} ais_use_algo_type_t;

typedef struct CalibDb_Eis_s {
    eis_mode_t mode;
    char debug_xml_path[256];
    int camera_model_type;
    double camera_rate;         // image sensor output fps
    double imu_rate;            // sample rate of imu
    uint32_t image_buffer_num;  // total buffer count of driver
    uint32_t src_image_width;
    uint32_t src_image_height;
    uint32_t dst_image_width;
    uint32_t dst_image_height;
    float clip_ratio_x;
    float clip_ratio_y;

    double focal_length[2];      // focal length x and y
    double light_center[2];      // light center x and y
    double distortion_coeff[5];  // distortion coeff k1 k2 r1 r2 k3
    double xi;       // if camera model is CMAI model, xi MUST be set.  if it is
                     // pinhole model, set to 0.
    double gbias_x;  // offset of gyroscope's x direction
    double gbias_y;  // offset of gyroscope's y direction
    double gbias_z;  // offset of gyroscope's z direction
    double time_offset;    // timestamp offset between gyroscope and image's
                           // timestamp
    int sensor_axes_type;  // coordinate axis system transform type between
                           // gyroscope and camera
    char save_objpts_path[100];  // save the path of world coordinate point.
                                 // it's not used by now.
} CalibDb_Eis_t;

typedef struct CalibDb_Dis_s {
    eis_mode_t mode;
    int pdt_type;
    int camera_steady;
    int motion_level;
    int dis_algorithm;
    int img_format;
    int crop_ratio;
    uint32_t src_width;
    uint32_t src_height;
    uint32_t src_stride;
    uint32_t dst_width;
    uint32_t dst_height;
    int is_scale;
    int img_pyr_num;
    int mesh_type;
    int fec_version;
    int dis_mode;
    uint32_t moving_obj_lvl;
    uint32_t rolling_shutter_coef;
    uint32_t horizontal_limit;
    uint32_t vertical_imit;
} CalibDb_Dis_t;

typedef struct CalibDb_Is_s {
    uint8_t enable;
    ais_use_algo_type_t algo_type;
    CalibDb_Eis_t eis;
    CalibDb_Dis_t dis;
} CalibDb_Is_t;

#ifdef __cplusplus
}
#endif

#endif
