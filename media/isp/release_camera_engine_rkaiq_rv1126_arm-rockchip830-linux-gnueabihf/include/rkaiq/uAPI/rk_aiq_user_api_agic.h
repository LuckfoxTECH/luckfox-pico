#ifndef _RK_AIQ_USER_API_AGIC_H_
#define _RK_AIQ_USER_API_AGIC_H_

#include "agic/rk_aiq_uapi_agic_int.h"

XCamReturn
rk_aiq_user_api_agic_SetAttrib(rk_aiq_sys_ctx_t* sys_ctx, agic_attrib_t attr);

XCamReturn
rk_aiq_user_api_agic_GetAttrib(rk_aiq_sys_ctx_t* sys_ctx, agic_attrib_t *attr);
#endif
