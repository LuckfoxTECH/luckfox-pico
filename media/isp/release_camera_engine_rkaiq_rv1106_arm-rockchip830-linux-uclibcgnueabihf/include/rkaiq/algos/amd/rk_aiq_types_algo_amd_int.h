#ifndef __RKAIQ_TYPES_ALGO_AMD_INT_H__
#define __RKAIQ_TYPES_ALGO_AMD_INT_H__

#include "xcam_buffer.h"

typedef struct {
        XCamVideoBuffer* spImage;
        XCamVideoBuffer* ispGain;
} rk_aiq_amd_algo_stat_t;

#endif//__RKAIQ_TYPES_ALGO_AMD_INT_H__