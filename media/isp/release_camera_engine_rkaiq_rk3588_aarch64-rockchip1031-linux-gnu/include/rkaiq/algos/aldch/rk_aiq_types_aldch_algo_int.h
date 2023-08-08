#ifndef __RK_AIQ_TYPES_ALDCH_ALGO_INT_H__
#define __RK_AIQ_TYPES_ALDCH_ALGO_INT_H__

#include "rk_aiq_comm.h"
#include "rk_aiq_types_aldch_algo.h"

typedef struct rk_aiq_ldch_cfg_s {
    rk_aiq_uapi_sync_t sync;

    unsigned int en;
    int correct_level;
} rk_aiq_ldch_cfg_t;

#endif
