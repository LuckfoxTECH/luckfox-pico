/******************************************************************************
 *
 * Copyright 2019, Fuzhou Rockchip Electronics Co.Ltd . All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Fuzhou Rockchip Electronics Co.Ltd .
 *
 *
 *****************************************************************************/
#ifndef __RK_AIQ_TYPES_ADRC_ALGO_V200_H__
#define __RK_AIQ_TYPES_ADRC_ALGO_V200_H__

typedef struct OtherStats_s {
    
} OtherStats_t;

typedef struct drc_stats_s {
    
} drc_stats_t;

typedef struct rkisp_adrc_stats_s {
    drc_stats_t drc_stats;
    OtherStats_t other_stats;
} rkisp_adrc_stats_t;


#endif
