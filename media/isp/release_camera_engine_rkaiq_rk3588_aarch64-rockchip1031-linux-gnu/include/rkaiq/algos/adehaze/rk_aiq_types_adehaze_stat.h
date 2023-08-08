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
#ifndef __RK_AIQ_TYPES_ADEHAZE_ALGO_STAT_H__
#define __RK_AIQ_TYPES_ADEHAZE_ALGO_STAT_H__

typedef struct others_t {
    unsigned short tmo_luma[225];
    unsigned short short_luma[225];
    unsigned short middle_luma[25];
    unsigned short long_luma[225];
} others_t;

typedef struct dehaze_stats_v30_s {
    unsigned int dhaz_pic_sumh_left;
    unsigned int dhaz_pic_sumh_right;
    int dhaz_adp_air_base;
    int dhaz_adp_wt;
    int dhaz_adp_gratio;
    int dhaz_adp_tmax;
    int h_rgb_iir[64];
} dehaze_stats_v30_t;

typedef struct dehaze_stats_v21_s {
    int dhaz_adp_air_base;
    int dhaz_adp_wt;
    int dhaz_adp_gratio;
    int dhaz_adp_tmax;
    int h_rgb_iir[64];
} dehaze_stats_v21_t;

typedef struct dehaze_stats_v20_s {
    int dhaz_adp_air_base;
    int dhaz_adp_wt;
    int dhaz_adp_gratio;
    int dhaz_adp_tmax;
    int h_r_iir[64];
    int h_g_iir[64];
    int h_b_iir[64];
} dehaze_stats_v20_t;

typedef struct rkisp_adehaze_stats_s {
    union {
        dehaze_stats_v20_t dehaze_stats_v20;
        dehaze_stats_v21_t dehaze_stats_v21;
        dehaze_stats_v30_t dehaze_stats_v30;
    };
    others_t other_stats;
} rkisp_adehaze_stats_t;


#endif
