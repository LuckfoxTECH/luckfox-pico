/*
 *  Copyright (c) 2021 Rockchip Corporation
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

#ifndef ___RK_AIQ_CALIB_DB_V2_HELPER_H__
#define ___RK_AIQ_CALIB_DB_V2_HELPER_H__

#include "RkAiqCalibDbTypes.h"
#include "RkAiqCalibDbTypesV2.h"

//typedef void CamCalibDbV2Context_t;

#define CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(nm) \
    CALIB_MODULE_RELATIVE_OFFSET(CamCalibDbV2ContextIsp20_t, nm)

static calibdb_ctx_member_offset_info_t info_CamCalibDbV2ContextIsp20_t[] = {
    { "ae_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(ae_calib)},
    { "wb_v20", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(wb_v20)},
    { "agamma_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(agamma_calib)},
    { "ablc_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(ablc_calib)},
    { "adegamma_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(adegamma_calib)},
    { "agic_calib_v20", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(agic_calib_v20)},
    { "adehaze_calib_v20", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(adehaze_calib_v20)},
    { "adpcc_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(adpcc_calib)},
    { "amerge_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(amerge_calib)},
    { "atmo_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(atmo_calib)},
    { "cpsl", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(cpsl)},
    { "orb", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(orb)},
    { "bayernr_v1", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(bayernr_v1)},
    { "mfnr_v1", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(mfnr_v1)},
    { "uvnr_v1", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(uvnr_v1)},
    { "ynr_v1", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(ynr_v1)},
    { "sharp_v1", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(sharp_v1)},
    { "edgefilter_v1", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(edgefilter_v1)},
    { "debayer", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(debayer)},
    { "cproc", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(cproc)},
    { "ie", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(ie)},
    { "lsc_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(lsc_v2)},
    { "eis_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(eis_calib)},
    { "aldch", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(aldch)},
    { "afec", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(afec)},
    { "lumaDetect", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(lumaDetect)},
    { "colorAsGrey", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(colorAsGrey)},
    { "ccm_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(ccm_calib)},
    { "lut3d_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(lut3d_calib)},
    { "af", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(af)},
    { "thumbnails", CALIBV2_MODULE_RELATIVE_OFFSET_ISP20(thumbnails)},
    { NULL, 0},
};

#define CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(nm) \
    CALIB_MODULE_RELATIVE_OFFSET(CamCalibDbV2ContextIsp21_t, nm)

static calibdb_ctx_member_offset_info_t info_CamCalibDbV2ContextIsp21_t[] = {
    { "ae_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(ae_calib)},
    { "wb_v21", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(wb_v21)},
    { "agamma_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(agamma_calib)},
    { "ablc_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(ablc_calib)},
    { "adegamma_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(adegamma_calib)},
    { "agic_calib_v21", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(agic_calib_v21)},
    { "adehaze_calib_v21", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(adehaze_calib_v21)},
    { "adpcc_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(adpcc_calib)},
    { "amerge_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(amerge_calib)},
    { "cpsl", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(cpsl)},
    { "orb", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(orb)},
    { "debayer", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(debayer)},
    { "cproc", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(cproc)},
    { "ie", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(ie)},
    { "lsc_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(lsc_v2)},
    { "aldch", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(aldch)},
    { "lumaDetect", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(lumaDetect)},
    { "colorAsGrey", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(colorAsGrey)},
    { "ccm_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(ccm_calib)},
    { "lut3d_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(lut3d_calib)},
    { "af", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(af)},
    { "adrc_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(adrc_calib)},
    { "thumbnails", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(thumbnails)},
    { "bayernr_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(bayernr_v2)},
    { "cnr_v1", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(cnr_v1)},
    { "ynr_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(ynr_v2)},
    { "sharp_v3", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(sharp_v3)},
    { "csm", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(csm)},
    { "cgc", CALIBV2_MODULE_RELATIVE_OFFSET_ISP21(cgc)},


    { NULL, 0},
};

#define CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(nm) \
    CALIB_MODULE_RELATIVE_OFFSET(CamCalibDbV2ContextIsp30_t, nm)

static calibdb_ctx_member_offset_info_t info_CamCalibDbV2ContextIsp30_t[] = {
    { "ae_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(ae_calib)},
    { "wb_v21", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(wb_v21)},
    { "ablc_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(ablc_calib)},
    { "adegamma_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(adegamma_calib)},
    { "agic_calib_v21", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(agic_calib_v21)},
    { "debayer", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(debayer)},
    { "colorAsGrey", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(colorAsGrey)},
    { "ccm_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(ccm_calib)},
    { "lut3d_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(lut3d_calib)},
    { "aldch", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(aldch)},
    { "adpcc_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(adpcc_calib)},
    { "ie", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(ie)},
    { "cpsl", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(cpsl)},
    { "cproc", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(cproc)},
    { "amerge_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(amerge_calib_V2)},
    { "adrc_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(adrc_calib_V2)},
    { "agamma_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(agamma_calib_V30)},
    { "adehaze_calib_v30", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(adehaze_calib_v30)},
    { "lsc_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(lsc_v2)},
    { "ynr_v3", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(ynr_v3)},
    { "cnr_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(cnr_v2)},
    { "sharp_v4", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(sharp_v4)},
    { "bayer2dnr_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(bayer2dnr_v2)},
    { "bayertnr_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(bayertnr_v2)},
    { "cac_calib", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(cac_calib)},
    { "af_v30", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(af_v30)},
    { "gain_v2", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(gain_v2)},
    { "csm", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(csm)},
    { "cgc", CALIBV2_MODULE_RELATIVE_OFFSET_ISP30(cgc)},
    { NULL, 0},
};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static calibdb_ctx_infos_t info_CamCalibDbV2Context_array[] = {
    { 20, info_CamCalibDbV2ContextIsp20_t },
    { 21, info_CamCalibDbV2ContextIsp21_t },
    { 30, info_CamCalibDbV2ContextIsp30_t},
};
#pragma GCC diagnostic pop

// TODO: implement ops for different isp hw
static inline size_t calibdbV2_scene_ctx_size(CamCalibDbContext_t* ctx) {
    (void)(ctx);
    if (CHECK_ISP_HW_V20())
        return sizeof(CamCalibDbV2ContextIsp20_t);
    else if (CHECK_ISP_HW_V21())
        return sizeof(CamCalibDbV2ContextIsp21_t);
    else if (CHECK_ISP_HW_V30())
        return sizeof(CamCalibDbV2ContextIsp30_t);
    else
        return sizeof(CamCalibDbV2ContextIsp20_t);
}

CamCalibDbV2Context_t* calibdbV2_ctx_new();
void calibdbV2_ctx_delete(CamCalibDbV2Context_t* ctx);

static inline const char*
calibdbv2_get_scene_ctx_struct_name(const void* scene_ctx) {
    (void)(scene_ctx);
    if (CHECK_ISP_HW_V20())
        return "CamCalibDbV2ContextIsp20_t";
    else if (CHECK_ISP_HW_V21())
        return "CamCalibDbV2ContextIsp21_t";
    else if (CHECK_ISP_HW_V30())
        return "CamCalibDbV2ContextIsp30_t";
    else
        return NULL;
}

static inline void*
calibdbv2_get_scene_ptr(CamCalibSubSceneList_t* scene) {
    (void)(scene);
#if defined(ISP_HW_V20)
    if (CHECK_ISP_HW_V20())
        return (&scene->scene_isp20);
#elif defined(ISP_HW_V21)
    if (CHECK_ISP_HW_V21())
        return (&scene->scene_isp21);
#elif defined(ISP_HW_V30)
    if (CHECK_ISP_HW_V30())
        return (&scene->scene_isp30);
#else
    return NULL;
#endif
    return NULL;
}

static inline void*
calibdbV2_get_module_ptr(void* ctx,
                         calibdb_ctx_infos_t* info_array,
                         const char* module_name) {
    if (strcmp(module_name, "sensor_calib") == 0)
        return ((CamCalibDbV2Context_t*)ctx)->sensor_info;
    else if(strcmp(module_name, "module_calib") == 0)
        return ((CamCalibDbV2Context_t*)ctx)->module_info;
    else if(strcmp(module_name, "sys_static_cfg") == 0)
        return ((CamCalibDbV2Context_t*)ctx)->sys_cfg;
    else
        return calibdb_get_module_ptr(((CamCalibDbV2Context_t*)ctx)->calib_scene, info_array, module_name);
}

static inline int calibdbV2_to_tuningdb(CamCalibDbV2Tuning_t *dst,
                                        const CamCalibDbV2Context_t *src) {
    memcpy(&dst->sensor_calib, src->sensor_info, sizeof(CalibDb_Sensor_ParaV2_t));
    memcpy(&dst->module_calib, src->module_info, sizeof(CalibDb_Module_ParaV2_t));
    memcpy(&dst->sys_static_cfg, src->sys_cfg,
           sizeof(CalibDb_SysStaticCfg_ParaV2_t));
#if defined(ISP_HW_V20)
    memcpy(&dst->calib_scene, src->calib_scene,
           sizeof(CamCalibDbV2ContextIsp20_t));
#elif defined(ISP_HW_V21)
    memcpy(&dst->calib_scene, src->calib_scene,
           sizeof(CamCalibDbV2ContextIsp21_t));
#elif defined(ISP_HW_V30)
    memcpy(&dst->calib_scene, src->calib_scene,
           sizeof(CamCalibDbV2ContextIsp30_t));
#else
#error "WRONG ISP_HW_VERSION, ONLY SUPPORT V20 AND V21 AND V30 NOW !"
#endif
    return 0;
}

static inline int calibdbV2_from_tuningdb(CamCalibDbV2Context_t *dst,
        const CamCalibDbV2Tuning_t *src) {
    memcpy(dst->sensor_info, &src->sensor_calib, sizeof(CalibDb_Sensor_ParaV2_t));
    memcpy(dst->module_info, &src->module_calib, sizeof(CalibDb_Module_ParaV2_t));
    memcpy(dst->sys_cfg, &src->sys_static_cfg,
           sizeof(CalibDb_SysStaticCfg_ParaV2_t));
#if defined(ISP_HW_V20)
    memcpy(dst->calib_scene, &src->calib_scene,
           sizeof(CamCalibDbV2ContextIsp20_t));
#elif defined(ISP_HW_V21)
    memcpy(dst->calib_scene, &src->calib_scene,
           sizeof(CamCalibDbV2ContextIsp21_t));
#elif defined(ISP_HW_V30)
    memcpy(dst->calib_scene, &src->calib_scene,
           sizeof(CamCalibDbV2ContextIsp30_t));
#else
#error "WRONG ISP_HW_VERSION, ONLY SUPPORT V20 AND V21 AND V30 NOW !"
#endif
    return 0;
}

#define CALIBDBV2_GET_MODULE_PTR(ctx, module) \
        calibdbV2_get_module_ptr(ctx, info_CamCalibDbV2Context_array, #module)
#endif
