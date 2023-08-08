#ifndef RKAIQ_TOOL_SYSCTL_H_
#define RKAIQ_TOOL_SYSCTL_H_

int setCpsLtCfg(rk_aiq_sys_ctx_t* ctx, char* data);
int getCpsLtInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int queryCpsLtCap(rk_aiq_sys_ctx_t* ctx, char* data);
int setWorkingModeDyn(rk_aiq_sys_ctx_t* ctx, char* data);
int getVersionInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int setHdrAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getHdrAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setGammaAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getGammaAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setDpccAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getDpccAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setDehazeAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getDehazeAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setCcmAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getCcmAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int queryCCMInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int setAwbAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getAwbAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getAwbCCT(rk_aiq_sys_ctx_t* ctx, char* data);
int queryWBInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int setAcpAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getAcpAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int enumStaticMetas(rk_aiq_sys_ctx_t* ctx, char* data);
#endif


