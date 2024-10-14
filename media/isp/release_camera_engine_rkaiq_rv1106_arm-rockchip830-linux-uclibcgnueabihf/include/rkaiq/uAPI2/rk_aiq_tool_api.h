
#ifndef _RK_AIQ_TOOL_API_H_
#define _RK_AIQ_TOOL_API_H_

#include "rk_aiq_user_api2_imgproc.h"
#include "rk_aiq_api_private.h"
#include "uAPI2/rk_aiq_user_api2_wrapper.h"

XCamReturn rk_aiq_tool_api_ae_setExpSwAttr
    (const rk_aiq_sys_ctx_t* sys_ctx, const Uapi_ExpSwAttrV2_t expSwAttr)
{
    return rk_aiq_user_api2_ae_setExpSwAttr(sys_ctx, expSwAttr);
}

XCamReturn rk_aiq_tool_api_setMWBGain
    (const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_wb_gain_t *gain)
{
    return rk_aiq_uapi2_setMWBGain(sys_ctx, gain);
}

XCamReturn rk_aiq_tool_api_setWBMode
    (const rk_aiq_sys_ctx_t* sys_ctx, opMode_t mode)
{
    return rk_aiq_uapi2_setWBMode(sys_ctx, mode);
}

XCamReturn rk_aiq_tool_api_sysctl_swWorkingModeDyn
    (const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_working_mode_t mode)
{
    return rk_aiq_uapi_sysctl_swWorkingModeDyn(sys_ctx, mode);
}

int rk_aiq_tool_api_set_scene
    (const rk_aiq_sys_ctx_t* sys_ctx, aiq_scene_t* scene)
{
    return rk_aiq_user_api2_set_scene(sys_ctx, scene);
}

#endif
