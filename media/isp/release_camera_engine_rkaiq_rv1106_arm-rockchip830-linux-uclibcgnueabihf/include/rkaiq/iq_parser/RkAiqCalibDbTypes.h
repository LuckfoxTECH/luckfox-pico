#ifndef _RK_AIQ_CALIB_TYPES_H_
#define _RK_AIQ_CALIB_TYPES_H_
#include "rk_aiq_algo_des.h"

#include "common/list.h"
#include "RkAiqCalibDbTypesIsp20.h"
#include "RkAiqCalibDbTypesIsp21.h"

#pragma pack(4)

#ifndef CAMCALIBDBCONTEXT_T
#define CAMCALIBDBCONTEXT_T
typedef void CamCalibDbContext_t;
#endif

#if 0
#define RKAIQ_CALIB_GET_HW_VER(ctx) \
        ((CalibDb_Header_t*)(ctx))->isp_hw_version
#endif

#define TO_CALIBDBV1_ISP20(ctx) \
        ((CamCalibDbContextIsp20_t*)ctx)

#define TO_CALIBDBV1_ISP21(ctx) \
        ((CamCalibDbContextIsp21_t*)ctx)

#define CALIB_MODULE_RELATIVE_OFFSET(type, x) \
  (long)(&((type*)0)->x)

#define CALIB_MODULE_PTR(ctx, type, mn) \
    ((char*)(ctx) + CALIB_MODULE_RELATIVE_OFFSET(type, mn))


#define CALIB_FN_DEFINE_COMMON(module) \
    static inline void* calibdb_get_##module##_ptr(void* ctx) { \
        if (CHECK_ISP_HW_V20()) \
            return CALIB_MODULE_PTR(ctx, CamCalibDbContextIsp20_t, module); \
        else if (CHECK_ISP_HW_V21()) \
            return CALIB_MODULE_PTR(ctx, CamCalibDbContextIsp21_t, module); \
        else \
            return NULL; \
    }

#define CALIB_FN_DEFINE_SPEC(ver, module) \
    static inline void* calibdb_get_##module##_ptr(void* ctx) { \
        if (g_rkaiq_isp_hw_ver == ver) \
            return CALIB_MODULE_PTR(ctx, CamCalibDbContextIsp##ver##_t, module); \
        else \
            return NULL; \
    }

typedef struct calibdb_ctx_member_offset_info_s {
    const char* module_name;
    long offset;
} calibdb_ctx_member_offset_info_t;

#define CALIB_MODULE_RELATIVE_OFFSET_ISP20(nm) \
    CALIB_MODULE_RELATIVE_OFFSET(CamCalibDbContextIsp20_t, nm)

static calibdb_ctx_member_offset_info_t info_CamCalibDbContextIsp20_t[] = {
    { "header", CALIB_MODULE_RELATIVE_OFFSET_ISP20(header)},
    { "awb_calib_para_v200", CALIB_MODULE_RELATIVE_OFFSET_ISP20(awb_calib_para_v200)},
    { "awb_adjust_para", CALIB_MODULE_RELATIVE_OFFSET_ISP20(awb_adjust_para)},
    { "lut3d", CALIB_MODULE_RELATIVE_OFFSET_ISP20(lut3d)},
    { "aec", CALIB_MODULE_RELATIVE_OFFSET_ISP20(aec)},
    { "af", CALIB_MODULE_RELATIVE_OFFSET_ISP20(af)},
    { "amerge", CALIB_MODULE_RELATIVE_OFFSET_ISP20(amerge)},
    { "atmo", CALIB_MODULE_RELATIVE_OFFSET_ISP20(atmo)},
    { "blc", CALIB_MODULE_RELATIVE_OFFSET_ISP20(blc)},
    { "dpcc", CALIB_MODULE_RELATIVE_OFFSET_ISP20(dpcc)},
    { "bayerNr", CALIB_MODULE_RELATIVE_OFFSET_ISP20(bayerNr)},
    { "lsc", CALIB_MODULE_RELATIVE_OFFSET_ISP20(lsc)},
    { "dm", CALIB_MODULE_RELATIVE_OFFSET_ISP20(dm)},
    { "ccm", CALIB_MODULE_RELATIVE_OFFSET_ISP20(ccm)},
    { "uvnr", CALIB_MODULE_RELATIVE_OFFSET_ISP20(uvnr)},
    { "gamma", CALIB_MODULE_RELATIVE_OFFSET_ISP20(gamma)},
    { "degamma", CALIB_MODULE_RELATIVE_OFFSET_ISP20(degamma)},
    { "ynr", CALIB_MODULE_RELATIVE_OFFSET_ISP20(ynr)},
    { "gic", CALIB_MODULE_RELATIVE_OFFSET_ISP20(gic)},
    { "mfnr", CALIB_MODULE_RELATIVE_OFFSET_ISP20(mfnr)},
    { "sharp", CALIB_MODULE_RELATIVE_OFFSET_ISP20(sharp)},
    { "edgeFilter", CALIB_MODULE_RELATIVE_OFFSET_ISP20(edgeFilter)},
    { "dehaze", CALIB_MODULE_RELATIVE_OFFSET_ISP20(dehaze)},
    { "afec", CALIB_MODULE_RELATIVE_OFFSET_ISP20(afec)},
    { "aldch", CALIB_MODULE_RELATIVE_OFFSET_ISP20(aldch)},
    { "lumaDetect", CALIB_MODULE_RELATIVE_OFFSET_ISP20(lumaDetect)},
    { "orb", CALIB_MODULE_RELATIVE_OFFSET_ISP20(orb)},
    { "sensor", CALIB_MODULE_RELATIVE_OFFSET_ISP20(sensor)},
    { "module", CALIB_MODULE_RELATIVE_OFFSET_ISP20(module)},
    { "cpsl", CALIB_MODULE_RELATIVE_OFFSET_ISP20(cpsl)},
    { "colorAsGrey", CALIB_MODULE_RELATIVE_OFFSET_ISP20(colorAsGrey)},
    { "cProc", CALIB_MODULE_RELATIVE_OFFSET_ISP20(cProc)},
    { "ie", CALIB_MODULE_RELATIVE_OFFSET_ISP20(ie)},
    { "sysContrl", CALIB_MODULE_RELATIVE_OFFSET_ISP20(sysContrl)},
    { NULL, 0},
};

#define CALIB_MODULE_RELATIVE_OFFSET_ISP21(nm) \
    CALIB_MODULE_RELATIVE_OFFSET(CamCalibDbContextIsp21_t, nm)

static calibdb_ctx_member_offset_info_t info_CamCalibDbContextIsp21_t[] = {
    { "header", CALIB_MODULE_RELATIVE_OFFSET_ISP21(header)},
    { "awb_calib_para_v201", CALIB_MODULE_RELATIVE_OFFSET_ISP21(awb_calib_para_v201)},
    { "awb_adjust_para", CALIB_MODULE_RELATIVE_OFFSET_ISP21(awb_adjust_para)},
    { "lut3d", CALIB_MODULE_RELATIVE_OFFSET_ISP21(lut3d)},
    { "ae_calib_para", CALIB_MODULE_RELATIVE_OFFSET_ISP21(ae_calib_para)},
    { "ae_tuning_para", CALIB_MODULE_RELATIVE_OFFSET_ISP21(ae_tuning_para)},
    { "af", CALIB_MODULE_RELATIVE_OFFSET_ISP21(af)},
    { "amerge", CALIB_MODULE_RELATIVE_OFFSET_ISP21(amerge)},
    { "adrc", CALIB_MODULE_RELATIVE_OFFSET_ISP21(adrc)},
    { "blc", CALIB_MODULE_RELATIVE_OFFSET_ISP21(blc)},
    { "dpcc", CALIB_MODULE_RELATIVE_OFFSET_ISP21(dpcc)},
    { "lsc", CALIB_MODULE_RELATIVE_OFFSET_ISP21(lsc)},
    { "dm", CALIB_MODULE_RELATIVE_OFFSET_ISP21(dm)},
    { "ccm", CALIB_MODULE_RELATIVE_OFFSET_ISP21(ccm)},
    { "gamma", CALIB_MODULE_RELATIVE_OFFSET_ISP21(gamma)},
    { "degamma", CALIB_MODULE_RELATIVE_OFFSET_ISP21(degamma)},
    { "gic", CALIB_MODULE_RELATIVE_OFFSET_ISP21(gic)},
    { "dehaze", CALIB_MODULE_RELATIVE_OFFSET_ISP21(dehaze)},
    { "aldch", CALIB_MODULE_RELATIVE_OFFSET_ISP21(aldch)},
    { "lumaDetect", CALIB_MODULE_RELATIVE_OFFSET_ISP21(lumaDetect)},
    { "expset", CALIB_MODULE_RELATIVE_OFFSET_ISP21(expset)},
    { "module", CALIB_MODULE_RELATIVE_OFFSET_ISP21(module)},
    { "cpsl", CALIB_MODULE_RELATIVE_OFFSET_ISP21(cpsl)},
    { "colorAsGrey", CALIB_MODULE_RELATIVE_OFFSET_ISP21(colorAsGrey)},
    { "cProc", CALIB_MODULE_RELATIVE_OFFSET_ISP21(cProc)},
    { "ie", CALIB_MODULE_RELATIVE_OFFSET_ISP21(ie)},
    { "list_bayernr_v2", CALIB_MODULE_RELATIVE_OFFSET_ISP21(list_bayernr_v2)},
    { "list_ynr_v2", CALIB_MODULE_RELATIVE_OFFSET_ISP21(list_ynr_v2)},
    { "list_cnr_v1", CALIB_MODULE_RELATIVE_OFFSET_ISP21(list_cnr_v1)},
    { "list_sharp_v3", CALIB_MODULE_RELATIVE_OFFSET_ISP21(list_sharp_v3)},
    { NULL, 0},
};

typedef struct calibdb_ctx_infos_s {
    int ver;
    calibdb_ctx_member_offset_info_t* offset_info;
} calibdb_ctx_infos_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
static calibdb_ctx_infos_t info_CamCalibDbContext_array[] = {
    { 20, info_CamCalibDbContextIsp20_t },
    { 21, info_CamCalibDbContextIsp21_t },
    { 0, NULL},
};
#pragma GCC diagnostic pop

static inline void*
calibdb_get_module_ptr(void* ctx,
                       calibdb_ctx_infos_t* info_array,
                       const char* module_name) {
    calibdb_ctx_infos_t* ctx_info = NULL;
    int i;
    for (i = 0; info_array[i].offset_info != NULL; i++) {
        if (g_rkaiq_isp_hw_ver == info_array[i].ver) {
            ctx_info =  &info_array[i];
            break;
        }
    }
    if (!ctx_info)
        return NULL;

    calibdb_ctx_member_offset_info_t* ctx_offset_infos = ctx_info->offset_info;
    while (ctx_offset_infos->module_name != NULL) {
        if (strcmp(ctx_offset_infos->module_name, module_name) == 0)
            break;
        ctx_offset_infos++;
    }
    if (ctx_offset_infos->module_name != NULL)
        return ((char*)(ctx) + ctx_offset_infos->offset);

    return NULL;
}

#define CALIBDB_GET_MODULE_PTR(ctx, module) \
        calibdb_get_module_ptr(ctx, info_CamCalibDbContext_array, #module)

#pragma pack()

#endif
