/*
 *rk_aiq_types_adegamma_algo_int.h
 *
 *  Copyright (c) 2019 Rockchip Corporation
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
 *
 */

#ifndef _RK_AIQ_TYPE_ADEGAMMA_ALGO_INT_H_
#define _RK_AIQ_TYPE_ADEGAMMA_ALGO_INT_H_
#include "adegamma/rk_aiq_types_adegamma_algo.h"
#include "RkAiqCalibDbTypes.h"
#include "adegamma_head.h"

#define DEGAMMA_CRUVE_KNOTS                   (17)
#define DEGAMMA_CRUVE_X_KNOTS                 (16)
#define DEGAMMA_CRUVE_X_NORMALIZE_FACTOR     (4)
#define DEGAMMA_CRUVE_X_MAX                    (7)
#define DEGAMMA_CRUVE_X_MIN                    (0)
#define DEGAMMA_CRUVE_Y_KNOTS                  (17)
#define DEGAMMA_CRUVE_Y_MAX                    (4095)
#define DEGAMMA_CRUVE_Y_MIN                    (0)


enum {
    DEGAMMA_OUT_NORMAL = 0,
    DEGAMMA_OUT_HDR = 1,
    DEGAMMA_OUT_NIGHT = 2
};

typedef enum rk_aiq_degamma_op_mode_s {
    RK_AIQ_DEGAMMA_MODE_OFF                     = 0,        /**< run iq degamma */
    RK_AIQ_DEGAMMA_MODE_MANUAL                  = 1,        /**< run manual degamma */
    RK_AIQ_DEGAMMA_MODE_TOOL                    = 2,        /**< for tool*/
} rk_aiq_degamma_op_mode_t;

typedef struct Adegamma_api_manual_s {
    bool en;
    int X_axis[DEGAMMA_CRUVE_KNOTS];
    int curve_R[DEGAMMA_CRUVE_KNOTS];
    int curve_G[DEGAMMA_CRUVE_KNOTS];
    int curve_B[DEGAMMA_CRUVE_KNOTS];
}  Adegamma_api_manual_t;

typedef struct rk_aiq_degamma_cfg_s {
    bool degamma_en;
    int degamma_X[DEGAMMA_CRUVE_X_KNOTS];
    int degamma_tableR[DEGAMMA_CRUVE_Y_KNOTS];
    int degamma_tableG[DEGAMMA_CRUVE_Y_KNOTS];
    int degamma_tableB[DEGAMMA_CRUVE_Y_KNOTS];
}  rk_aiq_degamma_cfg_t;

typedef struct rk_aiq_degamma_attr_s {
    rk_aiq_degamma_op_mode_t mode;
    Adegamma_api_manual_t stManual;
    CalibDbV2_Adegmma_t stTool;
    int Scene_mode;
}  rk_aiq_degamma_attr_t;

typedef struct AdegammaProcRes_s {
    bool degamma_en;
    int degamma_X_d0;
    int degamma_X_d1;
    int degamma_tableR[DEGAMMA_CRUVE_Y_KNOTS];
    int degamma_tableG[DEGAMMA_CRUVE_Y_KNOTS];
    int degamma_tableB[DEGAMMA_CRUVE_Y_KNOTS];
}  AdegammaProcRes_t;

#endif
