#ifndef RKAIQ_TOOL_SYSCTL_H_
#define RKAIQ_TOOL_SYSCTL_H_

#pragma pack(1)
typedef struct RkToolExpToolParam_s {

    float integration_time;

    float analog_gain;

    float digital_gain;

    float isp_dgain;

    int   iso;

    int   dcg_mode;

    int   longfrm_mode;
} RkToolExpRealParam_t;

typedef struct RkToolExpSensorParam_s {

    uint32_t fine_integration_time;

    uint32_t coarse_integration_time;

    uint32_t analog_gain_code_global;

    uint32_t digital_gain_global;

    uint32_t isp_digital_gain;
} RkToolExpSensorParam_t;

typedef struct RkToolExpParam_s {
    RkToolExpRealParam_t exp_real_params; //real value
    RkToolExpSensorParam_t exp_sensor_params;//reg value
} RkToolExpParam_t;

typedef struct rk_aiq_isp_tool_stats_s
{
    uint16_t version; // 0x0100 = v1.0
    uint32_t frameID;
    RkToolExpParam_t linearExp; // from RKAiqAecStats_t::ae_exp.LinearExp.exp_real_params
    RkToolExpParam_t hdrExp[3];
    uint8_t reserved[256];
}rk_aiq_isp_tool_stats_t;
#pragma pack()

int setCpsLtCfg(rk_aiq_sys_ctx_t* ctx, char* data);
int getCpsLtInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int queryCpsLtCap(rk_aiq_sys_ctx_t* ctx, char* data);
int setWorkingModeDyn(rk_aiq_sys_ctx_t* ctx, char* data);
int getVersionInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int setMergeAttribV10(rk_aiq_sys_ctx_t* ctx, char* data);
int getMergeAttribV10(rk_aiq_sys_ctx_t* ctx, char* data);
int setMergeAttribV11(rk_aiq_sys_ctx_t* ctx, char* data);
int getMergeAttribV11(rk_aiq_sys_ctx_t* ctx, char* data);
int setMergeAttribV12(rk_aiq_sys_ctx_t* ctx, char* data);
int getMergeAttribV12(rk_aiq_sys_ctx_t* ctx, char* data);
int setTmoAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getTmoAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setGammaAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getGammaAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setDpccAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getDpccAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int setCcmAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getCcmAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int queryCCMInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int setAwbAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getAwbAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getAwbCCT(rk_aiq_sys_ctx_t* ctx, char* data);
int queryWBInfo(rk_aiq_sys_ctx_t* ctx, char* data);
int setAcpAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int getAcpAttrib(rk_aiq_sys_ctx_t* ctx, char* data);
int enqueueRkRawFile(rk_aiq_sys_ctx_t* ctx, char* data);
int get3AStats(rk_aiq_sys_ctx_t* ctx, char* data);
int get3AStatsBlk(rk_aiq_sys_ctx_t* ctx, char* data);
int writeAwbIn(rk_aiq_sys_ctx_t* ctx, char* data);
int getTool3AStats(rk_aiq_sys_ctx_t* ctx, char* data);
int getTool3AStatsBlk(rk_aiq_sys_ctx_t* ctx, char* data);
#endif


