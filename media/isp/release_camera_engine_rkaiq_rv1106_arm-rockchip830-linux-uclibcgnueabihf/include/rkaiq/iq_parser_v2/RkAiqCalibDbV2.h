/*
 *  Copyright (c) 2021 Rockchip Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef ___RK_AIQ_CALIB_DB_V2_H__
#define ___RK_AIQ_CALIB_DB_V2_H__

#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#include "RkAiqCalibDbTypesV2.h"
#include "RkAiqCalibDbV2Helper.h"
#include "ablc_head.h"
#include "ablc_head_V32.h"
#include "config.h"
#include "rk_aiq_algo_des.h"
#include "sharp_head_v33.h"
#include "xcam_log.h"
#include "xcam_mutex.h"

struct cJSON;

namespace RkCam {

typedef struct __map_index {
  void *dst_offset;
  void *ptr_offset;
  size_t len;
} map_index_t;

typedef std::shared_ptr<std::list<std::string>> ModuleNameList;
typedef std::shared_ptr<std::list<RkAiqAlgoType_t>> AlgoList;

class RkAiqCalibDbV2 {
public:
    explicit RkAiqCalibDbV2() = default;
    ~RkAiqCalibDbV2() = default;

public:
    class TuningCalib {
    public:
        CamCalibDbV2Context_t* calib;
        ModuleNameList ModuleNames;
    };

public:
    static CamCalibDbProj_t *createCalibDbProj(const char *jsfile);
    static CamCalibDbProj_t *createCalibDbProj(const void *bin_buff, size_t len);
    static CamCalibDbCamgroup_t* createCalibDbCamgroup(const char *jsfile);
    static int CamCalibDbCamgroupFree(CamCalibDbCamgroup_t* calib_camgroup);

    static CamCalibDbProj_t *json2calibproj(const char *jsfile);
    static CamCalibDbProj_t *json2calibproj(const char *jstr, size_t len);
    static CamCalibDbProj_t *bin2calibproj(const char *binfile);
    static CamCalibDbProj_t *bin2calibproj(const void *bin_buff, size_t len);
    static CamCalibDbV2Context_t *json2calib(const char *jsfile);
    static CamCalibDbV2Context_t *cjson2calib(cJSON *json);

    static int calib2json(const char *jsfile, CamCalibDbV2Context_t *calib);
    static cJSON *calib2cjson(const CamCalibDbV2Context_t *calib);
    static int calibproj2json(const char *jsfile, CamCalibDbProj_t *calibproj);

    static void releaseCalibDbProj();
    static CamCalibDbV2Context_t toDefaultCalibDb(CamCalibDbProj_t *calibproj);

    static CamCalibDbProj_t *CamCalibDbProjAlloc();
    static CamCalibDbV2Context_t *CalibV2Alloc();
    static int CamCalibDbProjFree(CamCalibDbProj_t *calibproj);
    static int CalibV2Free(CamCalibDbV2Context_t *calibv2);

    static RkAiqAlgoType_t string2algostype(const char* str);
    static ModuleNameList analyzChange(cJSON* patch);

    static cJSON* readIQNode(const CamCalibDbV2Context_t *calib,
                             const char* node_path);

    static char* readIQNodeStr(const CamCalibDbV2Context_t* calib,
                               const char* node_path);

    static char* readIQNodeStrFromJstr(const CamCalibDbV2Context_t* calib,
                                       const char* json_str);

    static CamCalibDbV2Context_t *applyPatch(const CamCalibDbV2Context_t *calib,
            cJSON *patch);
    static CamCalibDbV2Context_t *applyPatch2(const CamCalibDbV2Context_t *calib,
            cJSON *patch);

    static CamCalibDbV2Context_t *applyPatch(const CamCalibDbV2Context_t *calib,
            const char *patch_str);
    static CamCalibDbV2Context_t *
    applyPatchFile(const CamCalibDbV2Context_t *calib, const char *patch_file);
    static CamCalibDbV2Context_t *applyPatch(const char *jsfile,
            const char *patch_file);

    static TuningCalib analyzTuningCalib(const CamCalibDbV2Context_t *calib,
                                         cJSON* patch);
    static TuningCalib analyzTuningCalib(const CamCalibDbV2Context_t *calib,
                                         const char* patch_str);

    static int FreeCalibByJ2S(void* ctx);

    static void *loadWholeFile(const char *fpath, size_t *fsize);
    static int parseBinStructMap(uint8_t *data, size_t len);

private:
    static std::map<std::string, CamCalibDbProj_t *> mCalibDbsMap;
    static CalibDb_Sensor_ParaV2_t mSensorInfo;
    static std::mutex calib_mutex;
    // isp20
    static int CamCalibDbFreeSceneCtx(void* ctx);
    static int CamCalibDbFreeSensorCtx(CalibDb_Sensor_ParaV2_t* sensor);
    static int CamCalibDbFreeModuleCtx(CalibDb_Module_ParaV2_t* module);
    static int CamCalibDbFreeAeCtx(CalibDb_Aec_ParaV2_t* ae);
#if RKAIQ_HAVE_AWB_V20
    static int CamCalibDbFreeAwbV20Ctx(CalibDbV2_Wb_Para_V20_t* awb);
#endif
#if RKAIQ_HAVE_GAMMA_V10
    static int CamCalibDbFreeGammaCtx(CalibDbV2_gamma_v10_t* gamma);
#endif
    static int CamCalibDbFreeBlcCtx(CalibDbV2_Ablc_t* blc);
    static int CamCalibDbFreeDegammaCtx(CalibDbV2_Adegmma_t* degamma);
#if RKAIQ_HAVE_GIC_V1
    static int CamCalibDbFreeGicV20Ctx(CalibDbV2_Gic_V20_t* gic);
#endif
    static int CamCalibDbFreeDehazeV20Ctx(CalibDbV2_dehaze_v10_t* dehaze);
    static int CamCalibDbFreeDpccCtx(CalibDbV2_Dpcc_t* dpcc);
    static int CamCalibDbFreeTmoCtx(CalibDbV2_tmo_t* tmo);
#if RKAIQ_HAVE_ORB_V1
    static int CamCalibDbFreeOrbCtx(CalibDbV2_Orb_t* orb);
#endif
#if RKAIQ_HAVE_BAYERNR_V1
    static int CamCalibDbFreeBayerNrV1Ctx(CalibDbV2_BayerNrV1_t* bayernr_v1);
#endif
#if RKAIQ_HAVE_MFNR_V1
    static int CamCalibDbFreeMfnrCtx(CalibDbV2_MFNR_t* mfnr_v1);
#endif
#if RKAIQ_HAVE_UVNR_V1
    static int CamCalibDbFreeUvnrCtx(CalibDbV2_UVNR_t* uvnr_v1);
#endif
#if RKAIQ_HAVE_YNR_V1
    static int CamCalibDbFreeYnrV1Ctx(CalibDbV2_YnrV1_t* ynr_v1);
#endif
#if RKAIQ_HAVE_SHARP_V1
    static int CamCalibDbFreeSharpV1Ctx(CalibDbV2_SharpV1_t* sharp_v1);
    static int CamCalibDbFreeEdgefilterCtx(CalibDbV2_Edgefilter_t* edgefilter_v1);
#endif

#if RKAIQ_HAVE_DEBAYER_V1
    static int CamCalibDbFreeDebayerCtx(CalibDbV2_Debayer_t* debayer_v1);
#endif
#if RKAIQ_HAVE_ACP_V10
    static int CamCalibDbFreeCprocCtx(CalibDbV2_Cproc_t* cproc);
#endif
#if RKAIQ_HAVE_AIE_V10
    static int CamCalibDbFreeIeCtx(CalibDbV2_IE_t* ie);
#endif
    static int CamCalibDbFreeLscCtx(CalibDbV2_LSC_t* lsc);
#if RKAIQ_HAVE_EIS_V1
    static int CamCalibDbFreeEisCtx(CalibDbV2_Eis_t* eis);
#endif
    static int CamCalibDbFreeColorAsGreyCtx(CalibDbV2_ColorAsGrey_t* colorAsGrey);
    static int CamCalibDbFreeLumaDetectCtx(CalibDbV2_LUMA_DETECT_t* lumaDetect);
#if RKAIQ_HAVE_FEC_V10
    static int CamCalibDbFreeFectCtx(CalibDbV2_FEC_t* fec);
#endif
#if (RKAIQ_HAVE_LDCH_V10 || RKAIQ_HAVE_LDCH_V21)
    static int CamCalibDbFreeLdchCtx(CalibDbV2_LDCH_t* ldch);
#endif
#if RKAIQ_HAVE_CCM_V1
    static int CamCalibDbFreeCcmV1Ctx(CalibDbV2_Ccm_Para_V2_t* ccm);
#endif
#if RKAIQ_HAVE_3DLUT_V1
    static int CamCalibDbFreeLut3dCtx(CalibDbV2_Lut3D_Para_V2_t* lut3d);
#endif
#if RKAIQ_HAVE_AF_V20
    static int CamCalibDbFreeAfV2xCtx(CalibDbV2_AF_t* af);
#endif
    static int CamCalibDbFreeThumbnailsCtx(CalibDbV2_Thumbnails_t* thumbnails);

    //isp21
#if RKAIQ_HAVE_AWB_V21
    static int CamCalibDbFreeAwbV21Ctx(CalibDbV2_Wb_Para_V21_t* awb);
#endif
#if RKAIQ_HAVE_GIC_V2
    static int CamCalibDbFreeGicV21Ctx(CalibDbV2_Gic_V21_t* gic);
#endif
#if RKAIQ_HAVE_BAYERNR_V2
    static int CamCalibDbFreeBayerNrV2Ctx(CalibDbV2_BayerNrV2_t* bayernr_v1);
#endif
#if RKAIQ_HAVE_UVNR_V1
    static int CamCalibDbFreeCnrCtx(CalibDbV2_CNR_t* cnr);
#endif
#if RKAIQ_HAVE_YNR_V2
    static int CamCalibDbFreeYnrV2Ctx(CalibDbV2_YnrV2_t* ynr_v2);
#endif
#if RKAIQ_HAVE_SHARP_V3
    static int CamCalibDbFreeSharpV3Ctx(CalibDbV2_SharpV3_t* sharp_v3);
#endif

    static int CamCalibDbFreeUapiCtx(RkaiqUapi_t* uapi);
    static int CamCalibDbFreeSysStaticCtx(CalibDb_SysStaticCfg_ParaV2_t* sys_static);

    //isp30
#if RKAIQ_HAVE_GAMMA_V11
    static int CamCalibDbFreeGammaV2Ctx(CalibDbV2_gamma_v11_t* gamma);
#endif
#if RKAIQ_HAVE_CAC_V03
    static int CamCalibDbFreeCacV03Ctx(CalibDbV2_Cac_V03_t* cac_calib);
#endif
#if RKAIQ_HAVE_CAC_V10
    static int CamCalibDbFreeCacV10Ctx(CalibDbV2_Cac_V10_t* cac_calib);
#endif
#if RKAIQ_HAVE_CAC_V11
    static int CamCalibDbFreeCacV11Ctx(CalibDbV2_Cac_V11_t* cac_calib);
#endif
#if RKAIQ_HAVE_BAYER2DNR_V2
    static int CamCalibDbFreeBayer2dnrV2Ctx(CalibDbV2_Bayer2dnrV2_t* bayer2dnr_v2);
#endif
#if RKAIQ_HAVE_BAYERTNR_V2
    static int CamCalibDbFreeBayertnrV2Ctx(CalibDbV2_BayerTnrV2_t* bayertnr_v2);
#endif
#if RKAIQ_HAVE_CNR_V2
    static int CamCalibDbFreeCnrV2Ctx(CalibDbV2_CNRV2_t* cnr_v2);
#endif
#if RKAIQ_HAVE_YNR_V3
    static int CamCalibDbFreeYnrV3Ctx(CalibDbV2_YnrV3_t* ynr_v3);
#endif
#if RKAIQ_HAVE_SHARP_V4
    static int CamCalibDbFreeSharpV4Ctx(CalibDbV2_SharpV4_t* sharp_v4);
#endif
#if RKAIQ_HAVE_AF_V30
    static int CamCalibDbFreeAfV30Ctx(CalibDbV2_AFV30_t* af);
#endif

// isp 32
#if RKAIQ_HAVE_AWB_V32
    static int CamCalibDbFreeAwbV32Ctx(CalibDbV2_Wb_Para_V32_t* awb);
#endif
#if RKAIQ_HAVE_BLC_V32
    static int CamCalibDbFreeBlcV32Ctx(CalibDbV2_Blc_V32_t* blc_v32);
#endif
#if RKAIQ_HAVE_DEBAYER_V2
    static int CamCalibDbFreeDebayerV2Ctx(CalibDbV2_Debayer_v2_t * debayer_v2);
#endif
#if RKAIQ_HAVE_CCM_V2
    static int CamCalibDbFreeCcmV2Ctx(CalibDbV2_Ccm_Para_V32_t* ccm);
#endif
#if RKAIQ_HAVE_BAYER2DNR_V23
    static int CamCalibDbFreeBayer2dnrV23Ctx(CalibDbV2_Bayer2dnrV23_t* bayer2dnr_v23);
#endif
#if RKAIQ_HAVE_BAYERTNR_V23
    static int CamCalibDbFreeBayertnrV23Ctx(CalibDbV2_BayerTnrV23_t* bayertnr_v23);
#endif
#if RKAIQ_HAVE_CNR_V30
    static int CamCalibDbFreeCnrV30Ctx(CalibDbV2_CNRV30_t* cnr_v30);
#endif
#if RKAIQ_HAVE_YNR_V22
    static int CamCalibDbFreeYnrV22Ctx(CalibDbV2_YnrV22_t* ynr_v22);
#endif
#if RKAIQ_HAVE_SHARP_V33
    static int CamCalibDbFreeSharpV33Ctx(CalibDbV2_SharpV33_t* sharp_v33);
#endif
#if RKAIQ_HAVE_AF_V31
    static int CamCalibDbFreeAfV31Ctx(CalibDbV2_AFV31_t* af);
#endif
#if RKAIQ_HAVE_GAIN_V2
    static int CamCalibDbFreeGainV2Ctx(CalibDbV2_GainV2_t* gain_v2);
#endif

};

} // namespace RkCam

#endif /*___RK_AIQ_CALIB_DB_V2_H__*/
