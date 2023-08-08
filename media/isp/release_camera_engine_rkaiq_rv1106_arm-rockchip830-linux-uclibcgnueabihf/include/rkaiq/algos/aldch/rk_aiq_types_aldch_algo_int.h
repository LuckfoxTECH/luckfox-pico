#ifndef __RK_AIQ_TYPES_ALDCH_ALGO_INT_H__
#define __RK_AIQ_TYPES_ALDCH_ALGO_INT_H__

#include <stdint.h>
#include "rk_aiq_types_aldch_algo.h"

typedef struct rk_aiq_uapi_sync_s rk_aiq_uapi_sync_t;

typedef struct rk_aiq_ldch_cfg_s {
    rk_aiq_uapi_sync_t sync;

    bool        en;
    uint8_t     correct_level;
} rk_aiq_ldch_cfg_t;

typedef struct rk_aiq_ldch_v21_cfg_s {
    rk_aiq_uapi_sync_t sync;

    bool        en;
    uint8_t     correct_level;

    uint8_t     bic_mode_en;
    uint8_t     zero_interp_en;
    uint8_t     sample_avr_en;

    uint8_t     bic_weight[ISP32_LDCH_BIC_NUM];
} rk_aiq_ldch_v21_cfg_t;

#endif
