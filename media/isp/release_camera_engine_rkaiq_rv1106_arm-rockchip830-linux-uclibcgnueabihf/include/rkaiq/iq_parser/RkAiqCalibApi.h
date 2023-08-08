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

#ifndef _RK_AIQ_CALIB_API_H_
#define _RK_AIQ_CALIB_API_H_
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "tinyxml2.h"

#include "RkAiqCalibDbTypes.h"

#if defined(_MSC_VER)
#define strcasecmp _stricmp
#define snprintf(buf,len, format,...) _snprintf_s(buf, len, len-1, format, __VA_ARGS__)
#endif
#ifndef container_of
#define container_of(ptr, type, member) \
  ((type *)(((unsigned long)(unsigned char *)(ptr)) - (unsigned long)(&((type *)0)->member)))

#endif
#if defined(__linux__)
#include "smartptr.h"
#include <xcam_common.h>
#include "xcam_log.h"

#ifdef DCT_ASSERT
#undef DCT_ASSERT
#endif
#define DCT_ASSERT  assert

#elif defined(_WIN32)

#ifdef DCT_ASSERT
#undef DCT_ASSERT
#endif
#define DCT_ASSERT(x) if(!(x))return false

#define LOGI printf
#define LOGD printf
#define LOGE printf
#define LOG1 printf

#endif
bool AddAecCalibProfile2AecCalibList(struct list_head *profile_list, CalibDb_Aec_CalibPara_t* pAddAec);
bool GetAecProfileFromAecCalibListBySceneName(const struct list_head* profile_list, const char* name, CalibDb_Aec_CalibPara_t** pAecProfile, int *name_index = NULL);
bool GetAecProfileFromAecCalibListByIdx(const struct list_head* profile_list, int idx, const CalibDb_Aec_CalibPara_t** pAecProfile) ;
bool AddAecTuneProfile2AecTuneList(struct list_head *profile_list, CalibDb_Aec_TunePara_t* pAddAec);
bool GetAecProfileFromAecTuneListBySceneName(const struct list_head* profile_list, const char* name, CalibDb_Aec_TunePara_t** pAecProfile);
bool GetAecProfileFromAecTuneListByIdx(const struct list_head* profile_list, int idx, const CalibDb_Aec_TunePara_t** pAecProfile) ;
bool AddAwbCalibV200Profile2AwbCalibV200List(struct list_head *profile_list, CalibDb_Awb_Calib_Para_V200_t* pAddAwb);
bool GetAwbProfileFromAwbCalibV200ListBySceneName(const struct list_head* profile_list, const char* name, CalibDb_Awb_Calib_Para_V200_t** pAwbProfile, int *name_index = NULL);
bool GetAwbProfileFromAwbCalibV200ListByIdx(const struct list_head* profile_list, int idx, const CalibDb_Awb_Calib_Para_V200_t** pAwbProfile) ;
bool AddAwbCalibV201Profile2AwbCalibV201List(struct list_head *profile_list, CalibDb_Awb_Calib_Para_V201_t* pAddAwb);
bool GetAwbProfileFromAwbCalibV201ListBySceneName(const struct list_head* profile_list, const char* name, CalibDb_Awb_Calib_Para_V201_t** pAwbProfile, int *name_index = NULL);
bool GetAwbProfileFromAwbCalibV201ListByIdx(const struct list_head* profile_list, int idx, const CalibDb_Awb_Calib_Para_V201_t** pAwbProfile) ;
bool AddAwbAdjustProfile2AwbAdjustList(struct list_head *profile_list, CalibDb_Awb_Adjust_Para_t* pAddAwb);
bool GetAwbProfileFromAwbAdjustListBySceneName(const struct list_head* profile_list, const char* name, CalibDb_Awb_Adjust_Para_t** pAwbProfile);
bool GetAwbProfileFromAwbAdjustListByIdx(const struct list_head* profile_list, int idx, const CalibDb_Awb_Adjust_Para_t** pAwbProfile) ;

bool CamCalibDbCreate
(
    CamCalibDbContext_t*  pCamCalibDbCtx
);

bool CamCalibDbRelease
(
    CamCalibDbContext_t*  pCamCalibDbCtx
) ;

bool CamCalibdbAddBayernrV2Setting2DProfile
(
    struct list_head *profile_list,
    Calibdb_Bayernr_2Dparams_V2_t* pAdd

);

bool CamCalibdbGetBayernrV2Setting2DByName
(
    const struct list_head* profile_list,
    char* name,
    Calibdb_Bayernr_2Dparams_V2_t** ppProfile
);

bool CamCalibdbGetBayernrV2Setting2DByIdx
(
    const struct list_head* profile_list,
    int idx,
    Calibdb_Bayernr_2Dparams_V2_t** ppProfile
);

bool CamCalibdbAddBayernrV2Setting3DProfile
(
    struct list_head *profile_list,
    CalibDb_Bayernr_3DParams_V2_t* pAdd
);

bool CamCalibdbGetBayernrV2Setting3DByName
(
    const struct list_head* profile_list,
    char* name,
    CalibDb_Bayernr_3DParams_V2_t** ppProfile
);

bool CamCalibdbGetBayernrV2Setting3DByIdx
(
    const struct list_head* profile_list,
    int idx,
    CalibDb_Bayernr_3DParams_V2_t** ppProfile
);

bool CamCalibdbAddBayernrV2Profile
(
    struct list_head *profile_list,
    CalibDb_Bayernr_V2_t* pAdd
) ;

bool CamCalibdbGetBayernrV2ProfileByName
(
    const struct list_head* profile_list,
    char* name,
    CalibDb_Bayernr_V2_t** ppProfile
);

bool CamCalibdbGetBayernrV2ProfileByIdx
(
    const struct list_head* profile_list,
    int idx,
    CalibDb_Bayernr_V2_t** ppProfile
);

bool CamCalibdbAddYnrV2SettingProfile
(
    struct list_head *profile_list,
    Calibdb_Ynr_params_V2_t* pAdd
);

bool CamCalibdbGetYnrV2SettingByName
(
    const struct list_head* profile_list,
    char* name,
    Calibdb_Ynr_params_V2_t** ppProfile
);

bool CamCalibdbGetYnrV2SettingByIdx
(
    const struct list_head* profile_list,
    int idx,
    Calibdb_Ynr_params_V2_t** ppProfile
);

bool CamCalibdbAddYnrV2Profile
(
    struct list_head *profile_list,
    Calibdb_Ynr_V2_t* pAdd
);

bool CamCalibdbGetYnrV2ProfileByName
(
    const struct list_head* profile_list,
    char* name,
    Calibdb_Ynr_V2_t** ppProfile
);

bool CamCalibdbGetYnrV2ProfileByIdx
(
    const struct list_head* profile_list,
    int idx,
    Calibdb_Ynr_V2_t** ppProfile
);

bool CamCalibdbAddCnrV1SettingProfile
(
    struct list_head *profile_list,
    Calibdb_Cnr_params_V1_t* pAdd

);

bool CamCalibdbGetCnrV1SettingByName
(
    const struct list_head* profile_list,
    char* name,
    Calibdb_Cnr_params_V1_t** ppProfile
);

bool CamCalibdbGetCnrV1SettingByIdx
(
    const struct list_head* profile_list,
    int idx,
    Calibdb_Cnr_params_V1_t** ppProfile
) ;


bool CamCalibdbAddCnrV1Profile
(
    struct list_head *profile_list,
    Calibdb_Cnr_V1_t* pAdd
);

bool CamCalibdbGetCnrV1ProfileByName
(
    const struct list_head* profile_list,
    char* name,
    Calibdb_Cnr_V1_t** ppProfile
);

bool CamCalibdbGetCnrV1ProfileByIdx
(
    const struct list_head* profile_list,
    int idx,
    Calibdb_Cnr_V1_t** ppProfile
);


bool CamCalibdbAddSharpV3SettingProfile
(
    struct list_head *profile_list,
    Calibdb_Sharp_params_V3_t* pAdd

);

bool CamCalibdbGetSharpV3SettingByName
(
    const struct list_head* profile_list,
    char* name,
    Calibdb_Sharp_params_V3_t** ppProfile
);

bool CamCalibdbGetSharpV3SettingByIdx
(
    const struct list_head* profile_list,
    int idx,
    Calibdb_Sharp_params_V3_t** ppProfile
);

bool CamCalibdbAddSharpV3Profile
(
    struct list_head *profile_list,
    Calibdb_Sharp_V3_t* pAdd
);


bool CamCalibdbGetSharpV3ProfileByName
(
    const struct list_head* profile_list,
    char* name,
    Calibdb_Sharp_V3_t** ppProfile
);

bool CamCalibdbGetSharpV3ProfileByIdx
(
    const struct list_head* profile_list,
    int idx,
    Calibdb_Sharp_V3_t** ppProfile
);
















#endif
