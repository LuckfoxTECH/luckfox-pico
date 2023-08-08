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

#include "RkAiqCalibDbTypesV2.h"
#include "RkAiqCalibDbV2Helper.h"
#include "j2s.h"
#include "xcam_log.h"
#include "xcam_mutex.h"
#include <list>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <mutex>
#include "rk_aiq_algo_des.h"

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
    static int CamCalibDbFreeAwbV20Ctx(CalibDbV2_Wb_Para_V20_t* awb);
    static int CamCalibDbFreeGammaCtx(CalibDbV2_gamma_t* gamma);
    static int CamCalibDbFreeBlcCtx(CalibDbV2_Ablc_t* blc);
    static int CamCalibDbFreeDegammaCtx(CalibDbV2_Adegmma_t* degamma);
    static int CamCalibDbFreeGicV20Ctx(CalibDbV2_Gic_V20_t* gic);
    static int CamCalibDbFreeDehazeV20Ctx(CalibDbV2_dehaze_V20_t* dehaze);
    static int CamCalibDbFreeDpccCtx(CalibDbV2_Dpcc_t* dpcc);
    static int CamCalibDbFreeMergeCtx(CalibDbV2_merge_t* merge);
    static int CamCalibDbFreeTmoCtx(CalibDbV2_tmo_t* tmo);
    static int CamCalibDbFreeCpslCtx(CalibDbV2_Cpsl_t* cpsl);
    static int CamCalibDbFreeOrbCtx(CalibDbV2_Orb_t* orb);
    static int CamCalibDbFreeBayerNrV1Ctx(CalibDbV2_BayerNrV1_t* bayernr_v1);
    static int CamCalibDbFreeMfnrCtx(CalibDbV2_MFNR_t* mfnr_v1);
    static int CamCalibDbFreeUvnrCtx(CalibDbV2_UVNR_t* uvnr_v1);
    static int CamCalibDbFreeYnrV1Ctx(CalibDbV2_YnrV1_t* ynr_v1);
    static int CamCalibDbFreeSharpV1Ctx(CalibDbV2_SharpV1_t* sharp_v1);
    static int CamCalibDbFreeEdgefilterCtx(CalibDbV2_Edgefilter_t* edgefilter_v1);
    static int CamCalibDbFreeDebayerCtx(CalibDbV2_Debayer_t* edgefilter_v1);
    static int CamCalibDbFreeCprocCtx(CalibDbV2_Cproc_t* cproc);
    static int CamCalibDbFreeIeCtx(CalibDbV2_IE_t* ie);
    static int CamCalibDbFreeLscCtx(CalibDbV2_LSC_t* lsc);
    static int CamCalibDbFreeEisCtx(CalibDbV2_Eis_t* eis);
    static int CamCalibDbFreeColorAsGreyCtx(CalibDbV2_ColorAsGrey_t* colorAsGrey);
    static int CamCalibDbFreeLumaDetectCtx(CalibDbV2_LUMA_DETECT_t* lumaDetect);
    static int CamCalibDbFreeFectCtx(CalibDbV2_FEC_t* fec);
    static int CamCalibDbFreeLdchCtx(CalibDbV2_LDCH_t* ldch);
    static int CamCalibDbFreeCcmCtx(CalibDbV2_Ccm_Para_V2_t* ccm);
    static int CamCalibDbFreeLut3dCtx(CalibDbV2_Lut3D_Para_V2_t* lut3d);
    static int CamCalibDbFreeAfV2xCtx(CalibDbV2_AF_t* af);
    static int CamCalibDbFreeThumbnailsCtx(CalibDbV2_Thumbnails_t* thumbnails);

    //isp21
    static int CamCalibDbFreeAwbV21Ctx(CalibDbV2_Wb_Para_V21_t* awb);
    static int CamCalibDbFreeGicV21Ctx(CalibDbV2_Gic_V21_t* gic);
    static int CamCalibDbFreeDehazeV21Ctx(CalibDbV2_dehaze_V21_t* dehaze);
    static int CamCalibDbFreeDrcCtx(CalibDbV2_drc_t* drc);
    static int CamCalibDbFreeBayerNrV2Ctx(CalibDbV2_BayerNrV2_t* bayernr_v1);
    static int CamCalibDbFreeCnrCtx(CalibDbV2_CNR_t* cnr);
    static int CamCalibDbFreeYnrV2Ctx(CalibDbV2_YnrV2_t* ynr_v2);
    static int CamCalibDbFreeSharpV3Ctx(CalibDbV2_SharpV3_t* sharp_v3);

    static int CamCalibDbFreeUapiCtx(RkaiqUapi_t* uapi);
    static int CamCalibDbFreeSysStaticCtx(CalibDb_SysStaticCfg_ParaV2_t* sys_static);

    //isp30
    static int CamCalibDbFreeGammaV2Ctx(CalibDbV2_gamma_V30_t* gamma);
    static int CamCalibDbFreeDehazeV30Ctx(CalibDbV2_dehaze_V21_t* dehaze);
    static int CamCalibDbFreeDrcV2Ctx(CalibDbV2_drc_V2_t* drc);
    static int CamCalibDbFreeMergeV2Ctx(CalibDbV2_merge_V2_t* merge);
    static int CamCalibDbFreeCacCtx(CalibDbV2_Cac_t* cac_calib);

    static int CamCalibDbFreeBayer2dnrV2Ctx(CalibDbV2_Bayer2dnr_V2_t* bayer2dnr_v2);

    static int CamCalibDbFreeBayertnrV2Ctx(CalibDbV2_BayerTnr_V2_t* bayertnr_v2);

    static int CamCalibDbFreeCnrV2Ctx(CalibDbV2_CNRV2_t* cnr_v2);
    static int CamCalibDbFreeYnrV3Ctx(CalibDbV2_YnrV3_t* ynr_v3);
    static int CamCalibDbFreeSharpV4Ctx(CalibDbV2_SharpV4_t* sharp_v4);

    static int CamCalibDbFreeAfV30Ctx(CalibDbV2_AFV30_t* af);
    static int CamCalibDbFreeGainV2Ctx(CalibDbV2_GainV2_t* gain_v2);
};

} // namespace RkCam

#endif /*___RK_AIQ_CALIB_DB_V2_H__*/
