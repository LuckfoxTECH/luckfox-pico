/*
 * Copyright (c) 2019-2022 Rockchip Eletronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __RKAIQ_TYPES_ALGO_AGIC_INT_H__
#define __RKAIQ_TYPES_ALGO_AGIC_INT_H__

#include <stdint.h>

#include "RkAiqCalibDbTypes.h"
#include "rk_aiq_comm.h"
#include "agic_uapi_head.h"

typedef struct rkaiq_gic_v1_hw_param_s {
    uint8_t edge_open;

    uint16_t regmingradthrdark2;
    uint16_t regmingradthrdark1;
    uint16_t regminbusythre;

    uint16_t regdarkthre;
    uint16_t regmaxcorvboth;
    uint16_t regdarktthrehi;

    uint8_t regkgrad2dark;
    uint8_t regkgrad1dark;
    uint8_t regstrengthglobal_fix;
    uint8_t regdarkthrestep;
    uint8_t regkgrad2;
    uint8_t regkgrad1;
    uint8_t reggbthre;

    uint16_t regmaxcorv;
    uint16_t regmingradthr2;
    uint16_t regmingradthr1;

    uint8_t gr_ratio;
    uint16_t dnloscale;
    uint16_t dnhiscale;
    uint8_t reglumapointsstep;

    uint16_t gvaluelimitlo;
    uint16_t gvaluelimithi;
    uint8_t fusionratiohilimt1;

    uint8_t regstrength_fix;

    uint16_t sigma_y[15];

    uint8_t noise_cut_en;
    uint16_t noise_coe_a;

    uint16_t noise_coe_b;
    uint16_t diff_clip;
} __attribute__((packed)) rkaiq_gic_v1_hw_param_t;

typedef struct rkaiq_gic_v2_hw_param_s {
    uint16_t regmingradthrdark2;
    uint16_t regmingradthrdark1;
    uint16_t regminbusythre;

    uint16_t regdarkthre;
    uint16_t regmaxcorvboth;
    uint16_t regdarktthrehi;

    uint8_t regkgrad2dark;
    uint8_t regkgrad1dark;
    uint8_t regstrengthglobal_fix;
    uint8_t regdarkthrestep;
    uint8_t regkgrad2;
    uint8_t regkgrad1;
    uint8_t reggbthre;

    uint16_t regmaxcorv;
    uint16_t regmingradthr2;
    uint16_t regmingradthr1;

    uint8_t gr_ratio;
    uint8_t noise_scale;
    uint16_t noise_base;
    uint16_t diff_clip;

    uint16_t sigma_y[15];
} __attribute__((packed)) rkaiq_gic_v2_hw_param_t;

typedef struct AgicProcResult_s {
    union {
        rkaiq_gic_v1_hw_param_t ProcResV20;
        rkaiq_gic_v2_hw_param_t ProcResV21;
    };
    bool gic_en;
} AgicProcResult_t;

typedef struct AgicConfigV20_s {
    unsigned char gic_en;
    unsigned char edge_open;
    unsigned short regmingradthrdark2;
    unsigned short regmingradthrdark1;
    unsigned short regminbusythre;
    unsigned short regdarkthre;
    unsigned short regmaxcorvboth;
    unsigned short regdarktthrehi;
    unsigned char regkgrad2dark;
    unsigned char regkgrad1dark;
    float globalStrength;
    unsigned char regkgrad2;
    unsigned char regkgrad1;
    unsigned char reggbthre;
    unsigned short regmaxcorv;
    unsigned short regmingradthr2;
    unsigned short regmingradthr1;
    unsigned char gr_ratio;
    float dnloscale;
    float dnhiscale;
    unsigned char reglumapointsstep;
    float gvaluelimitlo;
    float gvaluelimithi;
    float fusionratiohilimt1;
    float textureStrength;
    float noiseCurve_0;
    float noiseCurve_1;
    float sigma_y[15];
    unsigned char noise_cut_en;
    unsigned short noise_coe_a;
    unsigned short noise_coe_b;
    unsigned short diff_clip;
} AgicConfigV20_t;

typedef struct AgicConfigV21_s {
    unsigned char gic_en;
    unsigned short regmingradthrdark2;
    unsigned short regmingradthrdark1;
    unsigned short regminbusythre;
    unsigned short regdarkthre;
    unsigned short regmaxcorvboth;
    unsigned short regdarktthrehi;
    unsigned char regkgrad2dark;
    unsigned char regkgrad1dark;
    unsigned char regkgrad2;
    unsigned char regkgrad1;
    unsigned char reggbthre;
    unsigned short regmaxcorv;
    unsigned short regmingradthr2;
    unsigned short regmingradthr1;
    unsigned char gr_ratio;
    uint8_t noise_scale;
    uint16_t noise_base;
    float noiseCurve_0;
    float noiseCurve_1;
    float sigma_y[15];
    float globalStrength;
    unsigned short diff_clip;
} AgicConfigV21_t;

typedef struct AgicConfig_s {
    union {
        AgicConfigV20_t ConfigV20;
        AgicConfigV21_t ConfigV21;
    };
} AgicConfig_t;

#endif  //__RKAIQ_TYPES_ALGO_AGIC_INT_H__
