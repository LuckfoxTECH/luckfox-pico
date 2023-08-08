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

typedef struct dehaze_stats_v12_s {
    unsigned int dhaz_pic_sumh;
    unsigned short dhaz_adp_air_base;
    unsigned short dhaz_adp_wt;
    unsigned short dhaz_adp_gratio;
    unsigned short dhaz_adp_tmax;
    unsigned short h_rgb_iir[64];
} dehaze_stats_v12_t;

typedef struct dehaze_stats_v11_duo_s {
    unsigned int dhaz_pic_sumh_left;
    unsigned int dhaz_pic_sumh_right;
    int dhaz_adp_air_base;
    int dhaz_adp_wt;
    int dhaz_adp_gratio;
    int dhaz_adp_tmax;
    int h_rgb_iir[64];
} dehaze_stats_v11_duo_t;

typedef struct dehaze_stats_v11_s {
    int dhaz_adp_air_base;
    int dhaz_adp_wt;
    int dhaz_adp_gratio;
    int dhaz_adp_tmax;
    int h_rgb_iir[64];
} dehaze_stats_v11_t;

typedef struct dehaze_stats_v10_s {
    int dhaz_adp_air_base;
    int dhaz_adp_wt;
    int dhaz_adp_gratio;
    int dhaz_adp_tmax;
    int h_r_iir[64];
    int h_g_iir[64];
    int h_b_iir[64];
} dehaze_stats_v10_t;

typedef struct rkisp_adehaze_stats_s {
    bool stats_true;
#if RKAIQ_HAVE_DEHAZE_V10
    dehaze_stats_v10_t dehaze_stats_v10;
#endif
#if RKAIQ_HAVE_DEHAZE_V11
    dehaze_stats_v11_t dehaze_stats_v11;
#endif
#if RKAIQ_HAVE_DEHAZE_V11_DUO
    dehaze_stats_v11_duo_t dehaze_stats_v11_duo;
#endif
#if RKAIQ_HAVE_DEHAZE_V12
    dehaze_stats_v12_t dehaze_stats_v12;
#endif
} rkisp_adehaze_stats_t;


#endif
