#ifndef RKAIQ_TOOL_AE_H
#define RKAIQ_TOOL_AE_H
int setExpSwAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getExpSwAttr(rk_aiq_sys_ctx_t* ctx, char* data); 
int setLinAeDayRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getLinAeDayRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int setLinAeNightRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getLinAeNightRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int setHdrAeDayRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getHdrAeDayRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int setHdrAeNightRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getHdrAeNightRouteAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int queryExpResInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int setLinExpAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getLinExpAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int setHdrExpAttr(rk_aiq_sys_ctx_t* ctx, char* data);
int getHdrExpAttr(rk_aiq_sys_ctx_t* ctx, char* data);
#endif
