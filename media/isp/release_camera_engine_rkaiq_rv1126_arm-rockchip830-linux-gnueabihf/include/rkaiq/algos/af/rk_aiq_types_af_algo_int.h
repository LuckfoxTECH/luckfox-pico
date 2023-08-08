#ifndef __RK_AIQ_TYPES_AF_ALGO_INT_H__
#define __RK_AIQ_TYPES_AF_ALGO_INT_H__

#include "rk_aiq_types_af_algo.h"

typedef struct {
    unsigned char valid;
    int id;
    int depth;
} af_depth_info_t;

typedef struct {
    unsigned char valid;
    unsigned int af_target_pos;
    unsigned int af_target_width;
    unsigned int af_target_height;
} af_target_info_t;

typedef struct {
    unsigned char af_scene_changed;
    af_target_info_t af_target_info;
    af_depth_info_t af_depth_info[RKAIQ_RAWAF_SUMDATA_NUM];
    unsigned int global_sharpness_compensated[RKAIQ_RAWAF_SUMDATA_NUM];
} af_preprocess_result_t;

typedef enum _RKAIQ_AF_MODE
{
    RKAIQ_AF_MODE_NOT_SET = -1,
    RKAIQ_AF_MODE_AUTO,
    RKAIQ_AF_MODE_MACRO,
    RKAIQ_AF_MODE_INFINITY,
    RKAIQ_AF_MODE_FIXED,
    RKAIQ_AF_MODE_EDOF,
    RKAIQ_AF_MODE_CONTINUOUS_VIDEO,
    RKAIQ_AF_MODE_CONTINUOUS_PICTURE,
    RKAIQ_AF_MODE_ONESHOT_AFTER_ZOOM,
} RKAIQ_AF_MODE;

typedef struct rk_aiq_af_attrib_s {
    RKAIQ_AF_MODE AfMode;

    bool contrast_af;
    bool laser_af;
    bool pdaf;
    bool GammaEnable;
    bool GausEnable;

    int h_offs;
    int v_offs;
    unsigned int h_size;
    unsigned int v_size;

    short fixedModeDefCode;
    short macroModeDefCode;
    short infinityModeDefCode;

    rk_aiq_af_algo_meas_t manual_meascfg;
} rk_aiq_af_attrib_t;

#endif
