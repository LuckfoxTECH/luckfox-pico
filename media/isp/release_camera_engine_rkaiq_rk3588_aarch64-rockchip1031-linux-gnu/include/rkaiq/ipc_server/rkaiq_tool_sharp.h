#ifndef RKAIQ_TOOL_SHARP_H_
#define RKAIQ_TOOL_SHARP_H_

int setSharpAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getSharpAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setSharpIQPara(rk_aiq_sys_ctx_t* ctx, char* data);
int getSharpIQPara(rk_aiq_sys_ctx_t* ctx, char* data);
int setSharpStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int getSharpStrength(rk_aiq_sys_ctx_t* ctx, char* data);
#endif
