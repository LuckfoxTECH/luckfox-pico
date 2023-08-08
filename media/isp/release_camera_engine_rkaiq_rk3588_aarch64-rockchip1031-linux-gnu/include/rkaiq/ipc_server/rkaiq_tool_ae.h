#ifndef RKAIQ_TOOL_AE_H
#define RKAIQ_TOOL_AE_H
int setExpSwAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getExpSwAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int setLinAeRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getLinAeRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int setHdrAeRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getHdrAeRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int queryExpResInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int setLinExpAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getLinExpAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int setHdrExpAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getHdrExpAttr(rk_aiq_sys_ctx_t* ctx, char* data);
#endif
