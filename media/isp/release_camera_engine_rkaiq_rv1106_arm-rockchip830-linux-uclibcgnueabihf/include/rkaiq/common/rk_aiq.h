/*
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

#ifndef _RK_AIQ_H_
#define _RK_AIQ_H_

#include "base/xcam_common.h"
#include "rk_aiq_types.h"

XCAM_BEGIN_DECLARE

//typedef struct rk_aiq_static_metas_s {
//} rk_aiq_static_metas_t;

typedef enum rk_aiq_status_e {
    RK_AIQ_STATUS_VICAP_READY = 1,
} rk_aiq_status_t;

typedef struct rk_aiq_metas_s {
    uint32_t frame_id;
} rk_aiq_metas_t;

typedef struct rk_aiq_hwevt_s {
    int cam_id;
    int aiq_status;
    void* ctx;
} rk_aiq_hwevt_t;

typedef enum rk_aiq_err_code {
    UNKNOWN,
} rk_aiq_err_code_e;

typedef struct rk_aiq_err_msg_s {
    int err_code;
} rk_aiq_err_msg_t;

typedef struct rk_aiq_ver_info_s{
    char aiq_ver[16];
    char iq_parser_ver[16];
    uint32_t iq_parser_magic_code;
    char awb_algo_ver[16];
    char ae_algo_ver[16];
    char af_algo_ver[16];
    char ahdr_algo_ver[16];
} rk_aiq_ver_info_t;

typedef XCamReturn (*rk_aiq_error_cb)(rk_aiq_err_msg_t* err_msg);
typedef XCamReturn (*rk_aiq_metas_cb)(rk_aiq_metas_t* metas);
typedef XCamReturn (*rk_aiq_hwevt_cb)(rk_aiq_hwevt_t* hwevt);

typedef enum rk_aiq_cam_type_e {
    RK_AIQ_CAM_TYPE_SINGLE,
    RK_AIQ_CAM_TYPE_GROUP,
} rk_aiq_cam_type_t;

typedef struct rk_aiq_iq_buffer_info_s {
    void *addr;
    size_t len;
} rk_aiq_iq_buffer_info_t;

typedef enum rk_aiq_prd_type_e {
    RK_AIQ_PRD_TYPE_NORMAL,
    RK_AIQ_PRD_TYPE_TB_BATIPC,
    RK_AIQ_PRD_TYPE_TB_DOORLOCK,
} rk_aiq_prd_type_t;

typedef struct rk_aiq_tb_info_s {
    uint16_t magic;
    bool is_pre_aiq;
    uint8_t prd_type;
} rk_aiq_tb_info_t;

XCAM_END_DECLARE

#endif
