#ifndef _RK_AIQ_CALIB_TYPES_H_
#define _RK_AIQ_CALIB_TYPES_H_

#include "stdlib.h"
#include "string.h"

#pragma pack(4)

typedef void CamCalibDbContext_t;

#define CALIB_MODULE_RELATIVE_OFFSET(type, x) \
  (long)(&((type*)0)->x)

#define CALIB_MODULE_PTR(ctx, type, mn) \
    ((char*)(ctx) + CALIB_MODULE_RELATIVE_OFFSET(type, mn))


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

typedef struct calibdb_ctx_infos_s {
    int ver;
    calibdb_ctx_member_offset_info_t* offset_info;
} calibdb_ctx_infos_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic pop

static inline void*
calibdb_get_module_ptr(void* ctx,
                       calibdb_ctx_infos_t* info_array,
                       const char* module_name) {
    calibdb_ctx_infos_t* ctx_info = NULL;
    for (int i = 0; info_array[i].offset_info != NULL; i++) {
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
