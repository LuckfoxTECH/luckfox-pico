#ifndef _RK_AIQ_UAPI_AGIC_INT_H_
#define _RK_AIQ_UAPI_AGIC_INT_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"

typedef struct agic_attrib_s {
    unsigned char edge_en;
    unsigned char gr_ration;
    unsigned char noise_cut_en;
    float min_busy_thre[9];
    float min_grad_thr1[9];
    float min_grad_thr2[9];
    float k_grad1[9];
    float k_grad2[9];
    //float smoothness_gb[9];
    //float smoothness_gb_weak[9];
    float gb_thre[9];
    float maxCorV[9];
    float maxCorVboth[9];
    //float maxCutV[9];
    float dark_thre[9];
    float dark_threHi[9];
    float k_grad1_dark[9];
    float k_grad2_dark[9];
    float min_grad_thr_dark1[9];
    float min_grad_thr_dark2[9];
    float GValueLimitLo[9];
    float GValueLimitHi[9];
    float textureStrength[9];
    float ScaleLo[9];
    float ScaleHi[9];
    float noiseCurve_0[9];
    float noiseCurve_1[9];
    float globalStrength[9];
    float noise_coea[9];
    float noise_coeb[9];
    float diff_clip[9];
} agic_attrib_t;

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAdebayerHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.

XCamReturn
rk_aiq_uapi_agic_SetAttrib
(
    RkAiqAlgoContext* ctx,
    agic_attrib_t attr,
    bool need_sync
);

XCamReturn
rk_aiq_uapi_agic_GetAttrib
(
    RkAiqAlgoContext*  ctx,
    agic_attrib_t* attr
);

#endif//_RK_AIQ_UAPI_AGIC_INT_H_
