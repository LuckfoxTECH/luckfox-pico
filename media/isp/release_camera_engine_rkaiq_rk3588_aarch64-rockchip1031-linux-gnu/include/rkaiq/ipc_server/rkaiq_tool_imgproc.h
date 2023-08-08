#ifndef RKAIQ_TOOL_IMGPROC_H_
#define RKAIQ_TOOL_IMGPROC_H_

#include "rk_aiq_user_api_imgproc.h"
extern "C" {
    int setGrayMode(rk_aiq_sys_ctx_t* ctx, char* data);

    int getGrayMode(rk_aiq_sys_ctx_t* ctx);
}

#endif // _TOOL_RKAIQ_API_IMGPROC_H_
