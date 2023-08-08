/*
 * rk_aiq_algo_des.h
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

#ifndef _RK_AIQ_ALGO_DES_H_
#define _RK_AIQ_ALGO_DES_H_

#include "base/xcam_common.h"
#include "rk_aiq_comm.h"

/*
 * This file is used to define all the algos interfaces, eg. including ae, awb
 * af, anr and etc. . The typical call routine by user is as follow:
 * 1) create_context
 *    Should call this before any other interfaces, and should be called only
 *    once.
 * 2) prepare
 *    some algos have lots of pre-condtions, these condtions may consist of
 *    sensor_descriptors, tuning params for algo. This interface may be called
 *    after |create_context| or |post_process|, and shouldn't be called
 *    after |pre_process| or |processing|. That means the configs can't be
 *    changed during the process stage, and could be repeated(re-config) until
 *    one completed process routine.
 * 3) pre_process
 *    We separate the whole process to 3 steps: pre-process, processing and
 *    post-process. The caller will call all algos pre-process firstly, then
 *    call processing, then post-process at last.
 *    There may exist the dependancy between algos's params. pre-process aims
 *    to solve the dependancy. It collects all the dependancy before processing.
 * 4) processing
 *    runs the algos then outputs the result.
 * 5) post_process
 *    reserved.
 * 6) destroy_context
 *
 */

RKAIQ_BEGIN_DECLARE

typedef struct _RkAiqAlgoContext RkAiqAlgoContext;
typedef struct RKAiqAecExpInfo_s RKAiqAecExpInfo_t;
typedef void CamCalibDbContext_t;
typedef struct CamCalibDbV2Context_s CamCalibDbV2Context_t;
typedef struct _RkAiqResComb RkAiqResComb;

typedef enum RkAiqAlgoType_e {
    RK_AIQ_ALGO_TYPE_NONE = -1,
    RK_AIQ_ALGO_TYPE_AE,
    RK_AIQ_ALGO_TYPE_AWB,
    RK_AIQ_ALGO_TYPE_AF,
    RK_AIQ_ALGO_TYPE_ABLC,
    RK_AIQ_ALGO_TYPE_ADPCC,
    RK_AIQ_ALGO_TYPE_AMERGE,
    RK_AIQ_ALGO_TYPE_ATMO,
    RK_AIQ_ALGO_TYPE_ANR,
    RK_AIQ_ALGO_TYPE_ALSC,
    RK_AIQ_ALGO_TYPE_AGIC,
    RK_AIQ_ALGO_TYPE_ADEBAYER,
    RK_AIQ_ALGO_TYPE_ACCM,
    RK_AIQ_ALGO_TYPE_AGAMMA,
    RK_AIQ_ALGO_TYPE_AWDR,
    RK_AIQ_ALGO_TYPE_ADHAZ,
    RK_AIQ_ALGO_TYPE_A3DLUT,
    RK_AIQ_ALGO_TYPE_ALDCH,
    RK_AIQ_ALGO_TYPE_ACSM,
    RK_AIQ_ALGO_TYPE_ACP,
    RK_AIQ_ALGO_TYPE_AIE,
    RK_AIQ_ALGO_TYPE_ASHARP,
    RK_AIQ_ALGO_TYPE_AORB,
    RK_AIQ_ALGO_TYPE_ACGC,
    RK_AIQ_ALGO_TYPE_ASD,
    RK_AIQ_ALGO_TYPE_ADRC,
	RK_AIQ_ALGO_TYPE_ADEGAMMA,

    RK_AIQ_ALGO_TYPE_ARAWNR,
    RK_AIQ_ALGO_TYPE_AMFNR,
    RK_AIQ_ALGO_TYPE_AYNR,
    RK_AIQ_ALGO_TYPE_ACNR,
    RK_AIQ_ALGO_TYPE_AEIS,
    RK_AIQ_ALGO_TYPE_AFEC,
    RK_AIQ_ALGO_TYPE_AMD,
    RK_AIQ_ALGO_TYPE_AGAIN,
    RK_AIQ_ALGO_TYPE_ACAC,
    RK_AIQ_ALGO_TYPE_MAX
} RkAiqAlgoType_t;

typedef struct _AlgoCtxInstanceCfg {
    uint32_t isp_hw_version;
    uint32_t module_hw_version;
    CamCalibDbContext_t* calib;
    CamCalibDbV2Context_t* calibv2;
    bool isGroupMode;
} AlgoCtxInstanceCfg;

typedef struct _RkAiqAlgoDesComm {
    const char*                             version;
    const char*                             vendor;
    const char*                             description;
    RkAiqAlgoType_t                         type;
    int                                     id;
    XCamReturn (*create_context)            (RkAiqAlgoContext **context, const AlgoCtxInstanceCfg* cfg);
    XCamReturn (*destroy_context)           (RkAiqAlgoContext *context);
} RkAiqAlgoDesComm;

// base structs in order to abstract same interface
// for all algos

typedef enum RkAiqAlgoConfType_e {
    RK_AIQ_ALGO_CONFTYPE_INIT = 0,
    RK_AIQ_ALGO_CONFTYPE_UPDATECALIB = 0x01,
    RK_AIQ_ALGO_CONFTYPE_CHANGEMODE  = 0x02,
    RK_AIQ_ALGO_CONFTYPE_NEEDRESET   = 0x04,
    RK_AIQ_ALGO_CONFTYPE_CHANGERES   = 0x08,
    RK_AIQ_ALGO_CONFTYPE_KEEPSTATUS  = 0x10,
    RK_AIQ_ALGO_CONFTYPE_CHANGECAMS  = 0x20,
    RK_AIQ_ALGO_CONFTYPE_MAX
} RkAiqAlgoConfType_t;

typedef struct _RkAiqAlgoCom {
    RkAiqAlgoContext *ctx;
    uint32_t frame_id;
    union {
        struct {
            int working_mode; // real type is rk_aiq_working_mode_t or rk_aiq_isp_hdr_mode_t
            int sns_op_width;
            int sns_op_height;
            int conf_type;
            CamCalibDbContext_t* calib;
            CamCalibDbV2Context_t* calibv2;
        } prepare; //for prepare function

        struct {
            bool init;
            int iso;
            bool fill_light_on;
            bool gray_mode;
            bool is_bw_sensor;
            RKAiqAecExpInfo_t *preExp;
            RKAiqAecExpInfo_t *curExp;
            RKAiqAecExpInfo_t *nxtExp;
            RkAiqResComb* res_comb;
        } proc; //for pre/processing/post function
    } u;
    void* reserverd; //transfer whatever used by prepare/pre/processing/post
} RkAiqAlgoCom;

// generic result type
typedef struct _RkAiqAlgoResCom {
    char place_holder[1];
} RkAiqAlgoResCom;

typedef struct _RkAiqAlgoDescription {
    RkAiqAlgoDesComm    common;
    XCamReturn (*prepare)(RkAiqAlgoCom* params);
    XCamReturn (*pre_process)(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams);
    XCamReturn (*processing)(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams);
    XCamReturn (*post_process)(const RkAiqAlgoCom* inparams, RkAiqAlgoResCom* outparams);
} RkAiqAlgoDescription;

RKAIQ_END_DECLARE

#endif //_RK_AIQ_ALGO_DES_H_
