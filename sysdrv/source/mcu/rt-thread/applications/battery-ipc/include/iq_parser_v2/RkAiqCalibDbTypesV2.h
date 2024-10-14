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
#ifndef ___RK_AIQ_CALIB_DB_TYPES_V2_H__
#define ___RK_AIQ_CALIB_DB_TYPES_V2_H__

#include "moduleinfo_head.h"
#include "sensorinfo_head.h"
#include "sys_static_cfg_head.h"
#include "stdint.h"


#ifdef ISP_HW_V20
#include "RkAiqCalibDbV2TypesIsp20.h"
#endif
#ifdef ISP_HW_V21
#include "RkAiqCalibDbV2TypesIsp21.h"
#endif
#ifdef ISP_HW_V30
#include "RkAiqCalibDbV2TypesIsp3x.h"
#endif
#ifdef ISP_HW_V32
#include "RkAiqCalibDbV2TypesIsp32.h"
#endif

RKAIQ_BEGIN_DECLARE

/************************************

IQ
├── common info
├── main scene
│     ├─── name:SceneA
│     ├─── sub scene
│     │     ├── name:SubA0
│     │     ├── CalibV2
│     │     .
│     │     ├── name:SubA1
│     │     ├── CalibV2
│     ├─── name:SceneB
│     ├─── sub scene
│     │     ├── name:SubB0
│     │     ├── CalibV2
│     │     .
│     │     ├── name:SubB1
│     │     ├── CalibV2
│     ├─── name:SceneC
│     ├─── sub scene
│     │     ├── name:SubC0
│     │     ├── CalibV2
│     │     .
│     │     ├── name:SubC1
│     .     └── CalibV2
│     .
│     .
│     ├── name:sceneC
│     ├── ...
│     └── name:sceneN

IQ JSON:{
  platform common info: {},
  sensor common info: {},
  main scene: [
  {
    name: sceneA,
    sub_scene: [
          {
            name: subsceneA0,
                  CalibDbV2: {
                    ae,
                    awb,
                    ...
                  }
          },
          {
            name: subsceneA1,
                  CalibDbV2: {
                    ae,
                    awb,
                    ...
                  }
          },
          .
          .
          .
          ],
  },
  {
    name: sceneB,
          sub_scene: [
          {
            name: subsceneA0,
                  CalibDbV2: {
                    ae,
                    awb,
                    ...
                  }
          },

          ],

  }
  .
  .
  .
  ]
}
***********************************/

typedef struct CamCalibDbModuleDes_s {
    char* name;
    int hw_ver;
    char* sw_ver;
} CamCalibDbModuleDes_t;

typedef struct CamCalibDbV2Context_s {
    union {
        struct {
            CalibDb_Sensor_ParaV2_t  sensorinfo;
            CalibDb_Aec_ParaV2_t     ae_calib;
            CalibDbV2_Wb_Para_V32_t  awb_calib;
            CalibDbV2_BayerTnrV23_t  baytnr_calib;
            CalibDbV2_Bayer2dnrV23_t baynr_calib;
            CalibDbV2_Blc_V32_t      blc_calib;
        };
        struct {
            CalibDb_Sensor_ParaV2_t* sensor_info;
            int sensor_info_len;
            CalibDb_Module_ParaV2_t* module_info;
            int module_info_len;
            CalibDb_SysStaticCfg_ParaV2_t* sys_cfg;
            int sys_cfg_len;
            char* calib_scene;
        };
    };
} CamCalibDbV2Context_t;

/**
 * PC Tool's json patch is not based on CamCalibDbV2Context_t
 * Then we define an struct most like to PC UI ARCH, when we got new json patch
 * apply patch to CamCalibDbV2Tuning_t and convert to CamCalibDbV2Context_t
 */
typedef struct CamCalibDbV2Tuning_s {
    CalibDb_Sensor_ParaV2_t sensor_calib;
    CalibDb_Module_ParaV2_t module_calib;
    CalibDb_SysStaticCfg_ParaV2_t sys_static_cfg;
#if defined(ISP_HW_V20)
    CamCalibDbV2ContextIsp20_t calib_scene;
#elif defined(ISP_HW_V21)
    CamCalibDbV2ContextIsp21_t calib_scene;
#elif defined(ISP_HW_V30)
    CamCalibDbV2ContextIsp30_t calib_scene;
#elif defined(ISP_HW_V32)
    CamCalibDbV2ContextIsp32_t calib_scene;
#else
#error "WRONG ISP_HW_VERSION, ONLY SUPPORT V20 AND V21 AND V30 NOW !"
#endif
} CamCalibDbV2Tuning_t;

typedef struct CamCalibSubSceneList_s {
    char* name;
#if 0 //TODO zyc Union has bugs
    union {
        CamCalibDbV2ContextIsp20_t scene_isp20;
        CamCalibDbV2ContextIsp21_t scene_isp21;
    };
#endif

#if defined(ISP_HW_V20)
    CamCalibDbV2ContextIsp20_t scene_isp20;
#elif defined(ISP_HW_V21)
    CamCalibDbV2ContextIsp21_t scene_isp21;
#elif defined(ISP_HW_V30)
    CamCalibDbV2ContextIsp30_t scene_isp30;
#elif defined(ISP_HW_V32)
    CamCalibDbV2ContextIsp32_t scene_isp32;
#else
#error "WRONG ISP_HW_VERSION, ONLY SUPPORT V20 AND V21 AND V30 NOW !"
#endif

} CamCalibSubSceneList_t;

typedef struct CamCalibMainSceneList_s {
    char* name;
    // 'sub_scene' means Scene list of Ai ...
    CamCalibSubSceneList_t* sub_scene;
    int sub_scene_len;
} CamCalibMainSceneList_t;

typedef struct CamCalibDbProj_s {
    // TODO: move sensor info module etc. to an common struct
    // M4_STRUCT_DESC("SensorInfo", "normal_ui_style")
    CalibDb_Sensor_ParaV2_t sensor_calib;
    // M4_STRUCT_DESC("MoudleInfo", "normal_ui_style")
    CalibDb_Module_ParaV2_t module_calib;
    // 'main_scene' means Scene list of A,B,C ...,
    CamCalibMainSceneList_t* main_scene;
    int main_scene_len;
    void* uapi;
    int uapi_len;
    // M4_STRUCT_DESC("SysStaticCfg", "normal_ui_style")
    CalibDb_SysStaticCfg_ParaV2_t sys_static_cfg;
} CamCalibDbProj_t;

/*NOTE: SHOULD be defined next to CamCalibDbProj_t */
/*The storage struct of file camgroup.json */
typedef struct CamCalibDbCamgroup_s {
    //test
    int group_awb;
} CamCalibDbCamgroup_t;

typedef struct calib2bin_block_s {
    char     name[32];
    uint32_t   size;
    uint32_t   offset;
} __attribute__ ((aligned(4))) calib2bin_block_t;

typedef struct calib2bin_header_s {
    uint64_t    mask;
    uint32_t    bin_size;
    uint8_t     block_len;
    uint32_t    block_offset;
    uint32_t    bin_offset;
} __attribute__ ((aligned(4))) calib2bin_header_t;

typedef enum rk_aiq_iq_bin_mode_s {
    FULL_IQ_BIN_MODE = 0,
    SIM_IQ_BIN_MODE = 1,
    INVALID_IQ_BIN_MODE = 0xff, 
} rk_aiq_iq_bin_mode_t;

typedef struct rk_aiq_rtt_share_info_s {
    uint8_t     type;
    uint8_t     iq_bin_mode; //o: full mode; 1: simplified mode
    bool        flip;
    bool        mirror;
    uint32_t    vts;
    uintptr_t   aiq_iq_addr;
} __attribute__ ((aligned(4))) rk_aiq_rtt_share_info_t;

RKAIQ_END_DECLARE

#endif  /*___RK_AIQ_CALIB_DB_TYPES_V2_H__*/
