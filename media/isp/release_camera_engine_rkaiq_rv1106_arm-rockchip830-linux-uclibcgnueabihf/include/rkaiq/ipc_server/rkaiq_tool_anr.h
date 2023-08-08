#ifndef RKAIQ_TOOL_ANR_H_
#define RKAIQ_TOOL_ANR_H_

#define RKAIQ_TOOL_ANR_H_
int setAnrAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getAnrAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setAnrIQPara(rk_aiq_sys_ctx_t* ctx, char* data);
int getAnrIQPara(rk_aiq_sys_ctx_t* ctx, char* data);
int setLumaSFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int setLumaTFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int getLumaSFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int getLumaTFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int setChromaSFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int setChromaTFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int getChromaSFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int getChromaTFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int setRawnrSFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
int getRawnrSFStrength(rk_aiq_sys_ctx_t* ctx, char* data);
#endif
