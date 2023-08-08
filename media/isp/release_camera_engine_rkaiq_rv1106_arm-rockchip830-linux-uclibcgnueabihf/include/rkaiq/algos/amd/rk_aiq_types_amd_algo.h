#ifndef __RKAIQ_TYPES_AMD_ALGO_H__
#define __RKAIQ_TYPES_AMD_ALGO_H__
//#include "media_buffer/media_buffer_pool.h"

struct MediaBuffer_s;

typedef struct _MotionBufMetaData_t
{
    uint32_t frame_id;
    //to do: add more for future
    //var1
    //var2
} MotionBufMetaData_t;

typedef struct rk_aiq_amd_params_s {
    //uint32_t frame_id;
    //uint8_t *ratio_cur;
    //uint8_t *ratio_prev;
    //int ratio_size;
    struct MediaBuffer_s* st_ratio;
} rk_aiq_amd_params_t;

#endif//__RKAIQ_TYPES_AMD_ALGO_H__
